import { Component, AfterViewInit, ViewChild, ElementRef, signal, effect, OnDestroy } from '@angular/core';
import { FlexLayoutModule } from '@ngbracket/ngx-layout';
import nipplejs, { JoystickManager } from 'nipplejs';
import { MatCardModule } from "@angular/material/card";
import { MatDividerModule } from "@angular/material/divider";
import { MatSlideToggleModule } from "@angular/material/slide-toggle";
import { MatSliderModule } from "@angular/material/slider";
import { FormsModule } from '@angular/forms';
import { MatButtonModule } from "@angular/material/button";
import { MatInputModule } from "@angular/material/input";
import { MatSnackBar, MatSnackBarModule, MatSnackBarRef, TextOnlySnackBar } from "@angular/material/snack-bar";
import { WebSocketService } from "../../service/web-socket-service";

interface MoveCmd {
  cmd: string;
  leftMotor: number;
  rightMotor: number;
}

@Component({
  selector: 'app-main-view',
  imports: [
    FlexLayoutModule, MatCardModule, MatDividerModule,
    MatSlideToggleModule, MatSliderModule, FormsModule,
    MatButtonModule, MatInputModule, MatSnackBarModule
  ],
  templateUrl: './main-view.html',
  styleUrl: './main-view.scss',
})
export class MainView implements AfterViewInit, OnDestroy {
  @ViewChild('joystickArea', { static: true }) joystickArea!: ElementRef;

  speed = signal<number>(60);           // 用户设置最大速度百分比
  currentSpeed = signal<number>(0);     // 显示当前输出强度
  ipAddress = signal<string>("");
  port = signal<number>(8080);

  private manager!: JoystickManager;
  private heartbeat?: any;
  private lastCmd: MoveCmd = { cmd: "move", leftMotor: 0, rightMotor: 0 };
  private connectingSnackBar?: MatSnackBarRef<TextOnlySnackBar>;
  private lastSendTime = 0;
  private MIN_SEND_INTERVAL = 200; 
  constructor(
    private snackBar: MatSnackBar,
    public wsService: WebSocketService
  ) {
    this.ipAddress.set(location.hostname || '192.168.1.100');

    // 连接状态监听 + 成功后自动开启心跳发送
    effect(() => {
      if (wsService.isConnecting()) {
        this.connectingSnackBar = this.snackBar.open('正在连接 WebSocket...', '取消', {
          duration: 15000,
          horizontalPosition: 'right',
          verticalPosition: 'top',
        });
      }

      if (wsService.isConnected()) {
        this.connectingSnackBar?.dismiss();
        this.snackBar.open('连接成功', '关闭', {
          duration: 3000,
          horizontalPosition: 'right',
          verticalPosition: 'top',
          panelClass: ['success-snackbar']
        });
        this.startHeartbeat();
      }

      if (!wsService.isConnecting() && !wsService.isConnected() && this.connectingSnackBar && !wsService.isUserClosed()) {
        this.connectingSnackBar.dismiss();
        this.snackBar.open('连接失败，请检查 IP 和端口', '关闭', {
          duration: 4000,
          panelClass: ['error-snackbar'],
          horizontalPosition: 'right',
          verticalPosition: 'top',
        });
        this.connectingSnackBar = undefined;
        this.stopHeartbeat();
      }
    });
  }

  ngAfterViewInit(): void {
    this.manager = nipplejs.create({
      zone: this.joystickArea.nativeElement,
      mode: 'semi',
      color: '#2196f3',
      size: 160,
      threshold: 0.1,
      restJoystick: true,
    });

    this.manager.on('move', (evt, data) => {
      this.handleJoystick(data);
    });

    this.manager.on('end', () => {
      this.sendCommand(0, 0);
    });
  }

  ngOnDestroy(): void {
    this.manager?.destroy();
    this.stopHeartbeat();
  }
  private handleJoystick(data: any) {
    // 第一步：把 nipplejs 的 0°（右）旋转成 0°（上）
    const rawAngleRad = data.angle.radian;
    const angleRad = rawAngleRad - Math.PI / 2;
    // 不需要再标准化，后面用 sin/cos 自动周期

    const strength = Math.min(data.force, 3);
    const force = strength > 1 ? 1 : strength;

    // 第二步：关键修正 —— 左右转向取反！
    const forward = Math.cos(angleRad);
    const turn = -Math.sin(angleRad);

    const maxUserSpeed = this.speed() / 100;

    let left = forward + turn;
    let right = forward - turn;

    const max = Math.max(Math.abs(left), Math.abs(right), 1);
    left = left / max * force * maxUserSpeed;
    right = right / max * force * maxUserSpeed;

    const leftPWM = this.toPWM(left);
    const rightPWM = this.toPWM(right);

    this.sendCommand(leftPWM, rightPWM);
  }
  private toPWM(value: number): number {
    if (Math.abs(value) < 0.05) return 0;
    const abs = Math.abs(value);
    const pwm = 180 + 75 * abs; // 180 ~ 255
    return value > 0 ? pwm : -pwm;
  }
  private sendCommand(left: number, right: number) {
    const cmd: MoveCmd = {
      cmd: "move",
      leftMotor: Math.round(left),
      rightMotor: Math.round(right)
    };

    // 变化才发 + 最小间隔双保险
    const now = Date.now();
    if (cmd.leftMotor !== this.lastCmd.leftMotor ||
      cmd.rightMotor !== this.lastCmd.rightMotor ||
      now - this.lastSendTime > this.MIN_SEND_INTERVAL) {

      this.lastCmd = cmd;
      this.lastSendTime = now;  // 更新时间

      // UI 更新...
      const intensity = Math.max(Math.abs(left), Math.abs(right));
      this.currentSpeed.set(intensity >= 180 ? Math.round((intensity - 180) / 75 * 100) : 0);

      if (this.wsService.isConnected()) {
        this.wsService.sendMessage(JSON.stringify(cmd));
      }
    }
  }

  private startHeartbeat() {
    this.stopHeartbeat();
    this.heartbeat = setInterval(() => {
      if (this.wsService.isConnected()) {
        this.wsService.sendMessage(JSON.stringify(this.lastCmd));
      }
    }, 80);
  }

  private stopHeartbeat() {
    if (this.heartbeat) {
      clearInterval(this.heartbeat);
      this.heartbeat = undefined;
    }
  }

  connectWebSocket() {
    const url = `ws://${this.ipAddress()}:${this.port()}`;
    this.wsService.connect(url);
  }

  cancelConnect() {
    this.wsService.close();
    this.stopHeartbeat();
    this.sendCommand(0, 0); // 强制停
    this.snackBar.open('已断开连接', '关闭', { duration: 2000 });
  }

}