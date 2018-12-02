/*
   Name:        Laden.c
   Author:      Eldar E.
   Description: It's a tamagoochi, only this time the creature is bin laden
   Date:        9/9/2002 (DD/MM/YYYY)
   Copyright:   Eldar E.
*/

#include <stdio.h>
#include <alloc.h>
#include "laden\\headers\\VGAgraph.c"
#include "laden\\headers\\fonts.c"
#include "laden\\headers\\TheTime.c"
#include "laden\\headers\\bmp.c"
#include "laden\\headers\\mouse.c"
#include "laden\\l_funcs.c"

main()
 {
  struct pos mnow, mlast;
  int button, i;

  SetMode(0x0013);
  init();
  for (;;)
   {
    if ((button = MouseStatus(&mnow.x, &mnow.y, 1))==1)
     {
      if (check_button(0, mnow)) {
       save_status();
       break;
      }
      else
       for (i=1; i<5; ++i)
        if (((status[2])||(i==4))&&(check_button(i, mnow))) {
         button = run_bar(i, &mnow, &mlast, vpage);
         break;
        }
     }
    else if (button==2) {
     mouseOn = !mouseOn;
     total_delay(3);
     button = -1;
    }
    else button = dec_food(vpage);
    if (check_death(&mnow, &mlast, vpage))
     button = -1;
    else
     {
      if ((mnow.x<SX_MAX/2)&&(mnow.y<SY_MAX/2)&&(eye_status != -1)) {
       eye_status = -1;
       button = -1;
      }
      else if ((mnow.x>SX_MAX/2)&&(mnow.y<SY_MAX/2)&&(eye_status != 0)) {
       eye_status = 0;
       button = -1;
      }
      else if ((mnow.x>SX_MAX/2)&&(mnow.y>SY_MAX/2)&&(eye_status != 1)) {
       eye_status = 1;
       button = -1;
      }
      else if ((mnow.x<SX_MAX/2)&&(mnow.y>SY_MAX/2)&&(eye_status != 2)) {
       eye_status = 2;
       button = -1;
      }
      if (button==-1)
       Ani_laden(eye_status, vpage);
     }
    draw_mouse(mnow, &mlast, button, vpage);
   }
  SetMode(0x0003);
  CloseBMP(table);
  free(vpage);
 }

