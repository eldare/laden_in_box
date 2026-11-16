/*
   Name:        mouse.c
   Author:      Eldar Elaev.
   Description: Mouse functions for graphic mode of 256colors.
   Date:        20/4/2002  (DD/MM/YYYY)
   Copyright:   Eldar Elaev.
*/

int MouseStatus(int *x, int *y, int special);
/* return: 0-none ; 1-left ; 2-right */
int MouseRelease(int button, int *x, int *y);
/* button: 0-none ; 1-left ; 2-right */
int MouseRange(int from_x, int from_y, int till_x, int till_y);
/* return: 0-OK ; 1-error */
void MousePut(int x, int y);
void MouseImg(int img_num, int mouse_x, int mouse_y, unsigned char color, unsigned char *page);
/* img_num: 0-cross mouse ; 1-win style mouse ;  2-game style mouse*/

int MouseStatus(int *x, int *y, int special)
 {
  union REGS regs;

  regs.x.ax = 0x03;
  int86(0x33, &regs, &regs);
  *x = regs.x.cx/2;
  *y = regs.x.dx;

  if (special) {
   if (*y>193)
    *y=193;
   else if (*y<1)
    *y=1;
   if (*x>313)
    *x=313;
   else if (*x<1)
    *x=3;
  }
  return(regs.x.bx);
 }

int MouseRelease(int button, int *x, int *y)
 {
  union REGS regs;

  regs.x.ax = 0x06;
  regs.x.bx = button;
  int86(0x33, &regs, &regs);
  *x = regs.x.cx/2;
  *y = regs.x.dx;
  return(regs.x.ax);
 }

int MouseRange(int from_x, int from_y, int till_x, int till_y)
 {
  union REGS regs;

  if (from_x<0 || till_x>319 || from_y<0 || till_y>199)
   return(1);
  from_x <<= 1;
  till_x <<= 1;
  regs.x.ax = 0x07;
  regs.x.cx = from_x;
  regs.x.dx = till_x;
  int86(0x33, &regs, &regs);
  regs.x.ax = 0x08;
  regs.x.cx = from_y;
  regs.x.dx = till_y;
  int86(0x33, &regs, &regs);
  regs.x.ax = 0x04;
  regs.x.cx = till_x;
  regs.x.dx = till_y;
  int86(0x33, &regs, &regs);

  return(0);
 }

void MousePut(int x, int y)
 {
  union REGS regs;

  regs.x.ax = 0x04;
  regs.x.cx = x;
  regs.x.dx = y;
  int86(0x33, &regs, &regs);
 }

void MouseImg(int img_num, int mouse_x, int mouse_y, unsigned char color, unsigned char *page)
 {
  int i;

  switch (img_num)
   {
    case 0: /* a cross mouse */
     for (i=mouse_x-3; i<=mouse_x+3; ++i)
      if (i!=mouse_x)
       Pixel(i, mouse_y, color, page);
     for (i=mouse_y-3; i<=mouse_y+3; ++i)
      if (i!=mouse_y)
       Pixel(mouse_x, i, color, page); break;

    case 1: /* win style mouse */
     RePaint(mouse_x, mouse_y, mouse_x, mouse_y+5, color, page);
     RePaint(mouse_x+1, mouse_y+1, mouse_x+1, mouse_y+4, color, page);
     RePaint(mouse_x+2, mouse_y+2, mouse_x+2, mouse_y+5, color, page);
     Pixel(mouse_x+3, mouse_y+3, color, page);
     Pixel(mouse_x+3, mouse_y+6, color, page); break;

    case 2: /* game stule mouse */
     Pixel(mouse_x, mouse_y, color, page);
     RePaint(mouse_x+1, mouse_y+1, mouse_x+2, mouse_y+1, color, page);
     RePaint(mouse_x+1, mouse_y+2, mouse_x+4, mouse_y+2, color, page);
     RePaint(mouse_x+2, mouse_y+3, mouse_x+6, mouse_y+3, color, page);
     RePaint(mouse_x+2, mouse_y+4, mouse_x+8, mouse_y+4, color, page);
     RePaint(mouse_x+3, mouse_y+5, mouse_x+7, mouse_y+5, color, page);
     RePaint(mouse_x+3, mouse_y+6, mouse_x+6, mouse_y+6, color, page);
     RePaint(mouse_x+4, mouse_y+7, mouse_x+5, mouse_y+7, color, page);
     Pixel(mouse_x+4, mouse_y+8, color, page);
   }
 }
