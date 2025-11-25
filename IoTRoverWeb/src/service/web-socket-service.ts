import { Injectable, signal } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {
  private socket: WebSocket | null = null;

  public isConnected = signal<boolean>(false);
  public isConnecting = signal<boolean>(false);
  // 关键：标记是否是用户主动断开的
  public isUserClosed = signal<boolean>(false);

  connect(url: string): void {
    if (this.socket && (this.isConnected() || this.isConnecting())) {
      console.log('已经在连接或已连接');
      return;
    }

    this.isConnecting.set(true);
    this.isUserClosed.set(false);  // 开始新连接，清掉旧的主动断开标记

    this.socket = new WebSocket(url);

    this.socket.onopen = () => {
      this.isConnected.set(true);
      this.isConnecting.set(false);
      console.log('WebSocket 连接成功:', url);
    };

    this.socket.onmessage = (event) => {
      console.log('收到消息:', event.data);
    };

    this.socket.onerror = (error) => {
      console.error('WebSocket 错误:', error);
    };

    this.socket.onclose = (event) => {
      this.isConnected.set(false);
      this.isConnecting.set(false);
    };
  }

  sendMessage(message: string | ArrayBuffer): void {
    if (this.socket && this.isConnected()) {
      this.socket.send(message);
    } else {
      console.warn('WebSocket 未连接，无法发送消息');
    }
  }

  close(): void {
    if (this.socket) {
      this.isUserClosed.set(true);  
      this.socket.close();
      this.socket = null;
    }
  }
}