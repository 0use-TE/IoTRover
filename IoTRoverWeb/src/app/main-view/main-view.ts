import { Component, AfterViewInit, ViewChild, ElementRef } from '@angular/core';
import { FlexLayoutModule } from '@ngbracket/ngx-layout';
import nipplejs from 'nipplejs'
import { MatCardModule } from "@angular/material/card";

@Component({
  selector: 'app-main-view',
  imports: [FlexLayoutModule, MatCardModule],
  templateUrl: './main-view.html',
  styleUrl: './main-view.scss',
})


export class MainView implements AfterViewInit {
  @ViewChild('joystickArea', { static: true }) joystickArea!: ElementRef;

  ngAfterViewInit(): void {
    let manager = nipplejs.create({
      mode: 'semi',
      color: 'red',
      zone:this.joystickArea.nativeElement,
    })
  }

}
