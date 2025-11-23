import { Component,AfterViewInit } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { MatButton} from '@angular/material/button'
import { MatCheckbox} from '@angular/material/checkbox'
import { MatToolbarModule} from '@angular/material/toolbar'
import {MatIconModule} from '@angular/material/icon'
import {MatSidenavModule} from '@angular/material/sidenav'
import  nipplejs from 'nipplejs'
@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet,MatButton,MatCheckbox,MatToolbarModule,MatIconModule,MatSidenavModule],
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent implements AfterViewInit  {
  title:string="IoTRover"
  
  ngAfterViewInit(): void {
    // let manager=nipplejs.create({
    //    mode:'static',
    //   color:'red',
    //   position:{
    //     left:'50%',
    //     top:'50%'
    //   }
    // })
  }
}
