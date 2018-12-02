/* l_funcs.c */

#define BG_COLOR 0
#define MOUSE_COLOR 60
#define TEXT_COLOR 56
#define MSG_COLOR 60
#define SCREEN_X 77
#define SCREEN_Y 43

#define CTRL_MSG "NO ACTIONS LEFT FOR TODAY!"
#define MED_MSG "OSAMA NEEDS DRUGS..."
#define FOOD_MSG "OSAMA IS HUNGRY..."

#define STATUS_FILE "laden\\laden.box"
#define SMALL_FONT "laden\\Fonts\\EnSmall.mft"
#define NUM_FONT "laden\\Fonts\\Digits.mft"

#define TABLE "laden\\Images\\table.img"
#define IN_1 "laden\\Images\\in_1.img"
#define IN_2 "laden\\Images\\in_2.img"
#define IN_3 "laden\\Images\\in_3.img"
#define CREDITS "laden\\Images\\credits.img"
#define DEATH "laden\\Images\\death.img"

#define ANI_COOL1 "laden\\Ani\\cool1.anm"
#define ANI_COOL2 "laden\\Ani\\cool2.anm"
#define ANI_COOL3 "laden\\Ani\\cool3.anm"
#define ANI_COOL4 "laden\\Ani\\cool4.anm"
#define ANI_EYE1 "laden\\Ani\\eye1.anm"
#define ANI_EYE2 "laden\\Ani\\eye2.anm"
#define ANI_MOUTH1 "laden\\Ani\\mouth1.anm"
#define ANI_MOUTH2 "laden\\Ani\\mouth2.anm"
#define ANI_BALLS1 "laden\\Ani\\balls1.anm"
#define ANI_BALLS2 "laden\\Ani\\balls2.anm"
#define ANI_BOOM1 "laden\\Ani\\boom1.anm"
#define ANI_BOOM2 "laden\\Ani\\boom2.anm"
#define ANI_DRUGS1 "laden\\Ani\\drugs1.anm"
#define ANI_DRUGS2 "laden\\Ani\\drugs2.anm"

struct pos {
 x, y;
};
struct BmpRec table, img;
unsigned char *vpage = NULL;
unsigned long text_timer=0, delay_timer;
int status[5]={100, 100, 40, 0, 0}, text_status=0, mouseOn=1, eye_status=-2;

void init(void);
void update_status(void);
void save_status(void);
/* main bars functions */
int run_bar(int in_num, struct pos *mnow, struct pos *mlast, unsigned char *page);
void inside_screen(int in_num, unsigned char *page);
int inside_bars(int in_num, struct pos mouse, unsigned char *page);
int check_button(int button_num, struct pos mouse);
void draw_mouse(struct pos now, struct pos *last, int ext, unsigned char *page);
void ReLoad(unsigned char *page);
void draw_num(int num, int x, int y, unsigned char *page);
int dec_food(unsigned char *page);
void Ani_laden(int ani_num, unsigned char *page);
int check_death(struct pos *mnow, struct pos *mlast, unsigned char *page);
void activate_bmp(struct BmpRec img, char *file, unsigned char *page);
void total_delay(int delay);

void init(void)
 {
  update_status();
  LoadFont(SMALL_FONT, EnSmall, 276);
  LoadFont(NUM_FONT, Digits, 70);
  vpage = (unsigned char *) calloc(64000, 1);
  ReadBMP(&table, TABLE, 1);
  RetPal(BmpPall);
  ReLoad(vpage);
 }

void update_status(void)
 {
  FILE *fp;
  struct DateRec log, today;

  if ((fp = fopen(STATUS_FILE, "r"))!=NULL)
   {
    fseek(fp, 15L, 0);
    fscanf(fp, "%d", &status[0]);
    fseek(fp, 6L, 1);
    fscanf(fp, "%d", &status[1]);
    fseek(fp, 7L, 1);
    fscanf(fp, "%d%c%d%c%d", &log.D, &log.Y, &log.M, &log.Y, &log.Y);
    status[3] = log.D;
    GetDate(&today);
    fseek(fp, 7L, 1);
    fscanf(fp, "%d", &status[2]);
    if ((today.D!=log.D)||(today.M!=log.M)||(today.Y!=log.Y))
     status[2] = 40;
    fseek(fp, 7L, 1);
    fscanf(fp, "%d", &status[4]);
    fclose(fp);
   }
 }

void save_status(void)
 {
  FILE *fp;
  struct DateRec today;

  GetDate(&today);
  fp = fopen(STATUS_FILE, "w");
  fprintf(fp, "[BL BOX]\nfood=%d\nmed=%d\n", status[0], status[1]);
  fprintf(fp, "date=%d/%d/%d\nctrl=%d\ntime=%d", today.D, today.M, today.Y, status[2], status[4]);
  fclose(fp);
 }

/* main bars functions */
int run_bar(int in_num, struct pos *mnow, struct pos *mlast, unsigned char *page)
 {
  int button=-1;

  ReLoad(vpage);
  inside_screen(in_num, page);
  for (;;)
   {
    draw_mouse((struct pos)*mnow, mlast, button, page);
    if ((button = MouseStatus(&(mnow->x), &(mnow->y), 1))==1)
     if (inside_bars(in_num, (struct pos)*mnow, page)) {
      status[2]--;
      break;
     }
     else if (check_button(5, (struct pos)*mnow)) break;
   }
  ReLoad(page);
  return(-1);
 }

void inside_screen(int in_num, unsigned char *page)
 {
  switch (in_num)
   {
    case 0: ReadBMP(&img, DEATH, 0);   break;
    case 1: ReadBMP(&img, IN_1, 0);    break;
    case 2: ReadBMP(&img, IN_2, 0);    break;
    case 3: ReadBMP(&img, IN_3, 0);    break;
    case 4: ReadBMP(&img, CREDITS, 0);
   }
  DrawBMP(&img, SCREEN_X, SCREEN_Y, 0, 1, page);
  CloseBMP(img);
 }

int inside_bars(int in_num, struct pos mouse, unsigned char *page)
 {
  int in_ani_status;

  switch (in_num)
   {
    case 1: /* feed him acts */
     if (check_button(6, mouse))
      {
       status[0] += 10;
       return(1);
      }
     else if (check_button(7, mouse))
      {
       status[0] += 20;
       return(1);
      }
     else if (check_button(8, mouse))
      {
       status[0] += 25;
       return(1);
      }
     else if (check_button(9, mouse))
      {
       status[0] += 15;
       return(1);
      }
     break; /* end of feed him acts */
    case 2: /* hit him acts */
     if (check_button(6, mouse))
      {
       Ani_laden(eye_status, page);
       for (in_ani_status=5; in_ani_status<8; in_ani_status++)
        {
         LoadPage(VGA, page);
         total_delay(15);
         if (in_ani_status!=7)
          Ani_laden(in_ani_status, page);
        }
       status[1] -= 20;
       return(1);
      }
     else if (check_button(7, mouse))
      {
       Ani_laden(eye_status, page);
       for (in_ani_status=9; in_ani_status<12; in_ani_status++)
        {
         LoadPage(VGA, page);
         total_delay(15);
         if (in_ani_status!=11)
          Ani_laden(in_ani_status, page);
        }
       status[1] = 0;
       return(1);
      }
     else if (check_button(8, mouse))
      {
       Ani_laden(eye_status, page);
       for (in_ani_status=3; in_ani_status<6; in_ani_status++)
        {
         LoadPage(VGA, page);
         total_delay(15);
         if (in_ani_status!=5)
          Ani_laden(in_ani_status, page);
        }
       status[1] -= 20;
       return(1);
      }
     else if (check_button(9, mouse))
      {
       Ani_laden(eye_status, page);
       for (in_ani_status=7; in_ani_status<10; in_ani_status++)
        {
         LoadPage(VGA, page);
         total_delay(15);
         if (in_ani_status!=9)
          Ani_laden(in_ani_status, page);
        }
       status[1] -= 60;
       return(1);
      }
     break; /* end of hit him acts */
    case 3: /* drug him acts */
     if (check_button(7, mouse))
      {
       Ani_laden(eye_status, page);
       for (in_ani_status=11; in_ani_status<14; in_ani_status++)
        {
         LoadPage(VGA, page);
         total_delay(15);
         if (in_ani_status!=13)
          Ani_laden(in_ani_status, page);
        }
       status[1] += 20;
       return(1);
      }
     else if (check_button(9, mouse))
      {
       Ani_laden(eye_status, page);
       for (in_ani_status=11; in_ani_status<14; in_ani_status++)
        {
         LoadPage(VGA, page);
         total_delay(15);
         if (in_ani_status!=13)
          Ani_laden(in_ani_status, page);
        }
       status[1] = 0;
       return(1);
      }
     break; /* end of drug him acts */
   }
  return(0);
 }

int check_button(int button_num, struct pos mouse)
 {
  int yes=0;

  switch (button_num)
   {
/* main buttons */
    case 0: /* exit */
     if ((mouse.x>266)&&(mouse.y>162)&&(mouse.x<279)&&(mouse.y<177)) yes=1; break;
    case 1: /* feed him */
     if ((mouse.x>76)&&(mouse.y>153)&&(mouse.x<128)&&(mouse.y<177))  yes=1; break;
    case 2: /* bit him */
     if ((mouse.x>130)&&(mouse.y>153)&&(mouse.x<182)&&(mouse.y<177)) yes=1; break;
    case 3: /* drug him */
     if ((mouse.x>184)&&(mouse.y>153)&&(mouse.x<236)&&(mouse.y<177)) yes=1; break;
    case 4: /* credits */
     if ((mouse.x>37)&&(mouse.y>162)&&(mouse.x<50)&&(mouse.y<175))   yes=1; break;
/* inside buttons */
    case 5: /* cancel */
     if ((mouse.x>194)&&(mouse.y>114)&&(mouse.x<232)&&(mouse.y<132)) yes=1; break;
    case 6: /* water / mouth*/
     if ((mouse.x>109)&&(mouse.y>62)&&(mouse.x<153)&&(mouse.y<80))   yes=1; break;
    case 7: /* hotdog / boom / legal_drug*/
     if ((mouse.x>109)&&(mouse.y>81)&&(mouse.x<153)&&(mouse.y<99))   yes=1; break;
    case 8: /* pizza / eye */
     if ((mouse.x>154)&&(mouse.y>62)&&(mouse.x<198)&&(mouse.y<80))   yes=1; break;
    case 9: /* beer / balls / illegal drug*/
     if ((mouse.x>154)&&(mouse.y>81)&&(mouse.x<198)&&(mouse.y<99))   yes=1; break;
   }
  return(yes);
 }

void draw_mouse(struct pos now, struct pos *last, int ext, unsigned char *page)
 {
  if ((now.x!=last->x)||(now.y!=last->y)||(ext==-1))
   {
    LoadPage(VGA, page);
    if (mouseOn)
     MouseImg(0, now.x, now.y, MOUSE_COLOR, VGA);
   } last->x = now.x; last->y = now.y;
 }

void ReLoad(unsigned char *page)
 {
  int i;

  RePaint(0, 0, SX_MAX, SY_MAX, BG_COLOR, page);
  DrawBMP(&table, 38, 20, 0, 1, page);
  /* updating all the info */
  for (i=0; i<2; ++i)
   if (status[i]>100)
    status[i] = 100;
   else if (status[i]<0)
    status[i] = 0;
  if (status[2]<0) status[2] = 0;
  else if ((!status[4])&&(!status[2]))
   status[2] = 40;
  draw_num(status[0], 43, 70, page);
  draw_num(status[1], 43, 135, page);
  draw_num(status[2], 247, 80, page);
  if ((text_status)&&(*Clock18>text_timer))
   {
    RePaint(77, 139, 235, 144, BG_COLOR, page);
    text_status = 0;
   }
  else if ((!text_status)&&((status[0]<15)||(status[1]<40)||(status[2]==0)))
   {
    text_timer = *Clock18+50;
    text_status = 1;
    if (status[0]<15) EnSmallStr(FOOD_MSG, 78, 140, MSG_COLOR, page, 0);
    else if (status[1]<40) EnSmallStr(MED_MSG, 78, 140, MSG_COLOR, page, 0);
    else if (status[2]==0) EnSmallStr(CTRL_MSG, 78, 140, MSG_COLOR, page, 0);
   }
 }

void draw_num(int num, int x, int y, unsigned char *page)
 {
  do
   {
    Digit(num%10+'0', x+10, y, TEXT_COLOR, page, 0);
    num /= 10;
    x -= 6;
   } while (num);
 }

int dec_food(unsigned char *page)
 {
  struct TimeRec nowT;
  struct DateRec nowD;

  GetTime(&nowT);
  GetDate(&nowD);
  if (status[4]!=nowT.H){
   if (nowD.D==status[3])
    {
     status[0] -= 3*(nowT.H-status[4]);
     status[4] = nowT.H;
     ReLoad(page);
     return(-1);
    }
   else
    {
     if (((nowD.D-status[3])==1)||((nowD.D==1)&&((status[3]==30)||(status[3]==31))))
      status[0] -= 3*((24-status[4])+nowT.H);
     else status[0] = 0;
     status[4] = nowT.H;
     ReLoad(page);
     return(-1);
    }
  }
  return(0);
 }

void Ani_laden(int ani_num, unsigned char *page)
 {
  ReLoad(page);
  switch (ani_num)
   {
    case -1: ReadBMP(&img, ANI_COOL1, 0);  break;
    case 0: ReadBMP(&img, ANI_COOL2, 0);  break;
    case 1:  ReadBMP(&img, ANI_COOL3, 0);  break;
    case 2:  ReadBMP(&img, ANI_COOL4, 0);  break;
    case 3:  ReadBMP(&img, ANI_EYE1, 0);   break;
    case 4:  ReadBMP(&img, ANI_EYE2, 0);   break;
    case 5:  ReadBMP(&img, ANI_MOUTH1, 0); break;
    case 6:  ReadBMP(&img, ANI_MOUTH2, 0); break;
    case 7:  ReadBMP(&img, ANI_BALLS1, 0); break;
    case 8:  ReadBMP(&img, ANI_BALLS2, 0); break;
    case 9:  ReadBMP(&img, ANI_BOOM1, 0);  break;
    case 10: ReadBMP(&img, ANI_BOOM2, 0);  break;
    case 11: ReadBMP(&img, ANI_DRUGS1, 0); break;
    case 12: ReadBMP(&img, ANI_DRUGS2, 0); break;
   }
  DrawBMP(&img, SCREEN_X, SCREEN_Y, 0, 1, page);
  CloseBMP(img);
 }

int check_death(struct pos *mnow, struct pos *mlast, unsigned char *page)
 {
  if ((!status[0])||(!status[1])) {
   run_bar(0, mnow, mlast, page);
   status[0] = 100; status[1] = 100; status[2] = 40;
   eye_status = -2;
   ReLoad(page);
   return(1);
  }
  return(0);
 }

void total_delay(int delay)
 {
  delay_timer = *Clock18+delay;
  while (*Clock18<delay_timer);
 }

