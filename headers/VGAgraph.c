/* 
   Name:        VGAgraph.c
   Author:      Eldar Elaev.
   Description: VGA graphics interface.
   Date:        30/03/2002  (DD/MM/YYYY)
   Copyright:   Eldar Elaev.
*/

#include <dos.h>
#include <math.h>
#include <mem.h>

#define SX_MAX 318
#define SY_MAX 198

#define fx(ax,ay,K,m,op) ((!op) ? (m*K-m*ax+ay) : ((K-ay+m*ax)/m))
#define mmm(dx,dy) ((float)dy/dx)

unsigned char *VGA = (unsigned char *) MK_FP(0xa000,0);
unsigned char Pall[256][3];

void SetMode(unsigned char value);
void Pixel(int x, int y, unsigned char color, unsigned char *page);
unsigned char GetPixel(int x, int y, unsigned char *page);
void LoadPage(unsigned char *page1, unsigned char *page2);
void WaitRet(void);
void FadeOut(void);
void FadeIn(void);
void HideScreen(void);
void RetPal(unsigned char Pall[256][3]);
void SavePal(void);
static void GetPal(unsigned char colno, unsigned char *r, unsigned char *g, unsigned char *b);
static void SetPal(unsigned char colno, unsigned char r, unsigned char g, unsigned b);
/* Special Drawings */
void RePaint(int from_x, int from_y, int till_x, int till_y, unsigned char color, unsigned char *page);
void ReDot(int from_x, int from_y, int till_x, int till_y, unsigned char color, unsigned char *page);
void DrawLine(int ax, int ay, int bx, int by, unsigned char col, unsigned char *page);
void DrawCircle(int x, int y, int R, unsigned char col, unsigned char *page);
void DrawFullCircle(int x, int y, int R, unsigned char col, unsigned char *page);
void DrawRect(int x, int y, int x1, int y1, unsigned char col, unsigned char *page);

void SetMode(unsigned char value)
 {/* On-0x0013 ; Off-0x0003 */
  union REGS regs;

  regs.x.ax = value; int86(0x10, &regs, &regs);
 }

void Pixel(int x, int y, unsigned char color, unsigned char *page)
 {
  page[(y<<8)+(y<<6)+x] = color;
 }

unsigned char GetPixel(int x, int y, unsigned char *page)
 {
  return(page[(y<<8)+(y<<6)+x]);
 }

void LoadPage(unsigned char *page1, unsigned char *page2)
 {
  memcpy(page1, page2, 64000);
 }

void WaitRet(void)
 {
  unsigned char al;

  do {
   al = inp(0x03DA);
   al &=0x08;
  } while (al);
  do {
   al = inp(0x03DA);
   al &=0x08;
  } while (!al);
 }

void FadeOut(void)
 {
  unsigned char base[3];
  int i, j;

  for (i=0; i<64; ++i) {
   WaitRet();
   for (j=0; j<256; ++j) {
    GetPal(j ,&base[0], &base[1], &base[2]);
    if (base[0]>0) base[0]--;
    if (base[1]>0) base[1]--;
    if (base[2]>0) base[2]--;
    SetPal(j, base[0], base[1], base[2]);
   }
  }
 }

void FadeIn(void)
 {
  unsigned char base[3];
  int i, j;

  for (i=0; i<64; ++i) {
   WaitRet();
   for (j=0; j<256; ++j) {
    GetPal(j ,&base[0], &base[1], &base[2]);
    if ((base[0]<Pall[j][0])&&(base[0]<63)) base[0]++;
    if ((base[1]<Pall[j][1])&&(base[1]<63)) base[1]++;
    if ((base[2]<Pall[j][2])&&(base[2]<63)) base[2]++;
    SetPal(j, base[0], base[1], base[2]);
   }
  }
 }

void HideScreen(void)
 {
  int i;

  for (i=0; i<256; ++i)
   SetPal(i, 0, 0, 0);
 }

void RetPal(unsigned char Pall[256][3])
 {
  int i;

  for (i=0; i<256; ++i)
   SetPal(i, Pall[i][0], Pall[i][1], Pall[i][2]);
 }

void SavePal(void)
 {
  int i;

  for (i=0; i<256; ++i)
   GetPal(i, &Pall[i][0], &Pall[i][1], &Pall[i][2]);
 }

static void GetPal(unsigned char colno, unsigned char *r, unsigned char *g, unsigned char *b)
 {
  outp(0x03c7, colno); *r = inp(0x03c9); *g = inp(0x03c9); *b = inp(0x03c9);
 }

static void SetPal(unsigned char colno, unsigned char r, unsigned char g, unsigned b)
 {
  outp(0x03c8, colno); outp(0x03c9, r); outp(0x03c9, g); outp(0x03c9, b);
 }

/* Special Drawings */
void RePaint(int from_x, int from_y, int till_x, int till_y, unsigned char color, unsigned char *page)
 {
  int i=from_y, j;

  for (; i<=till_y; ++i)
   for (j=from_x; j<=till_x; ++j)
    Pixel(j, i, color, page);
 }

void ReDot(int from_x, int from_y, int till_x, int till_y, unsigned char color, unsigned char *page)
 {
  int i=from_y, j, k=0;

  for (; i<=till_y; ++i) {
   if (!k) j = from_x;
   else j = from_x+1;
   for (; j<=till_x; j+=2)
    Pixel(j, i, color, page);
   k = !k;
  }
 }

void DrawLine(int ax, int ay, int bx, int by, unsigned char col, unsigned char *page)
 {
  int x=ax, y=ay, i, dx, dy;
  float m;

  dx = bx-ax;
  if (dx)
   {
    dy = by-ay;
    m = mmm(dx,dy);
    dx = abs(dx);
    dy = abs(dy);
    if (dx>=dy)
     for (i=0; i<=dx; ++i)
      if (bx>=ax) Pixel(x++,fx(ax,ay,x,m,0),col,page);
      else Pixel(x--,fx(ax,ay,x,m,0),col,page);
     else for (i=0; i<=dy; ++i)
      Pixel(fx(ax,ay,y++,m,1),y,col,page);
   } else for (i=ay; i<=by; ++i) Pixel(ax,i,col,page);
 }

void DrawCircle(int x, int y, int R, unsigned char col, unsigned char *page)
 {
  float mone=0, tot=1/(float)R;
  int dx=0, dy=R-1;
  unsigned short x_offset, y_offset, offset=(y<<8)+(y<<6)+x;

  while (dx<=dy)
   {
    x_offset = (dx<<8)+(dx<<6);
    y_offset = (dy<<8)+(dy<<6);
    page[offset+dy-x_offset] = col;
    page[offset+dx-y_offset] = col;
    page[offset-dx-y_offset] = col;
    page[offset-dy-x_offset] = col;
    page[offset-dy+x_offset] = col;
    page[offset-dx+y_offset] = col;
    page[offset+dx+y_offset] = col;
    page[offset+dy+x_offset] = col;
    ++dx;
    mone += tot;
    dy = R*sin(acos(mone));
   }
 }

void DrawFullCircle(int x, int y, int R, unsigned char col, unsigned char *page)
 {
  int a;

  for (; R; R--)
   for (a=0; a<360; a++)
    Pixel(x+(R*cos((a*3.14)/180)), y-(R*sin((a*3.14)/180)), col, page);
 }

void DrawRect(int x, int y, int x1, int y1, unsigned char col, unsigned char *page)
 {
  RePaint(x, y, x, y1, col, page);   RePaint(x, y, x1, y, col, page);
  RePaint(x1, y, x1, y1, col, page); RePaint(x, y1, x1, y1, col, page);
 }

