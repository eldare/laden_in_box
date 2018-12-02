/* 
   Name:        TheTime.c
   Description: Time related functions
*/

struct TimeRec {
 int H, M, S, HS;
};
struct DateRec {
 int D, M, Y;
};

void GetTime(struct TimeRec *Time);
void SetTime(struct TimeRec Time);
void GetDate(struct DateRec *Date);
void SetDate(struct DateRec Date);
int Sec(void);
void UpdateNextTick(int S);
int Tick(void);
void StartTimer(void);

unsigned long *Clock18 = (unsigned long *) 0x0000046c;
int NextTick;

void GetTime(struct TimeRec *Time)
 {
  union REGS regs;

  regs.x.ax = 0x2c00;
  int86(0x21, &regs, &regs);
  Time->H  = regs.h.ch;
  Time->M  = regs.h.cl;
  Time->S  = regs.h.dh;
  Time->HS = regs.h.dl;
 }

void SetTime(struct TimeRec Time)
 {
  union REGS regs;

  regs.h.ch = Time.H;
  regs.h.cl = Time.M;
  regs.h.dh = Time.S;
  regs.h.dl = Time.HS;
  regs.x.ax = 0x2d00;
  int86(0x21, &regs, &regs);
 }

void GetDate(struct DateRec *Date)
 {
  union REGS regs;

  regs.x.ax = 0x2a00;
  int86(0x21, &regs, &regs);
  Date->D = regs.h.dl;
  Date->M = regs.h.dh;
  Date->Y = regs.x.cx;
 }

void SetDate(struct DateRec Date)
 {
  union REGS regs;

  regs.h.dl = Date.D;
  regs.h.dh = Date.M;
  regs.x.cx = Date.Y;
  regs.x.ax = 0x2b00;
  int86(0x21, &regs, &regs);
 }

int Sec(void)
 {
  union REGS regs;

  regs.x.ax = 0x2c00;
  int86(0x21, &regs, &regs);
  return(regs.h.dh);
 }

void UpdateNextTick(int S)
 {
  NextTick = (S+1) &60;
 }

int Tick(void)
 {
  int S;

  S= Sec();
  if (S==NextTick)
   return(1);
  else return(0);
 }

void StartTimer(void)
 {
  UpdateNextTick(Sec());
 }
