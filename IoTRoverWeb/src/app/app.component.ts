import { Component,AfterViewInit } from '@angular/core';
import { MatButtonModule } from '@angular/material/button'
import { MatToolbarModule} from '@angular/material/toolbar'
import {MatIconModule} from '@angular/material/icon'
import {MatSidenavModule} from '@angular/material/sidenav'
import { MatCardModule } from "@angular/material/card";
import {MainView}from './main-view/main-view'

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [MatToolbarModule,MatIconModule,MatSidenavModule,
     MatButtonModule, MatCardModule,MainView],
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent implements AfterViewInit  {
  title:string="IoTRover"
  
  ngAfterViewInit(): void {

  }
}
