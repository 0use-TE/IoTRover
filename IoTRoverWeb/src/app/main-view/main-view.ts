import { Component, AfterViewInit, ViewChild, ElementRef, signal } from '@angular/core';
import { FlexLayoutModule } from '@ngbracket/ngx-layout';
import nipplejs from 'nipplejs'
import { MatCardModule } from "@angular/material/card";
import { MatDividerModule } from "@angular/material/divider";
import { MatSlideToggleModule } from "@angular/material/slide-toggle";
import { MatSliderModule } from "@angular/material/slider";
import { FormsModule } from '@angular/forms';
import { single } from 'rxjs';

@Component({
  selector: 'app-main-view',
  imports: [FlexLayoutModule, MatCardModule, MatDividerModule, MatSlideToggleModule, MatSliderModule, FormsModule],
  templateUrl: './main-view.html',
  styleUrl: './main-view.scss',
})


export class MainView implements AfterViewInit {
  @ViewChild('joystickArea', { static: true }) joystickArea!: ElementRef;
  speed=signal<number>(0) ;
  currentSpeed=signal<number>(10);
  ngAfterViewInit(): void {
    let manager = nipplejs.create({
      mode: 'semi',
      color: 'blue',
      zone: this.joystickArea.nativeElement,
    })
  }

}
