/* 
   Name:        Font.c
   Author:      Eldar Elaev.
   Description: Fonts for VGA graphic mode.
   Date:        30/03/2002
   Copyright:   Eldar Elaev.
*/

void EnString(char *str, int x, int y, unsigned char color, unsigned char *page, int eff);
void EnLet(char let, int x, int y, unsigned char col, unsigned char *page, int eff);
void EnSmallStr(char *str, int x, int y, unsigned char color, unsigned char *page, int eff);
void EnSmallLet(char let, int x, int y, unsigned char col, unsigned char *page, int eff);
void NumString(char *str, int x, int y, unsigned char color, unsigned char *page, int eff);
void Digit(char num, int x, int y, unsigned char col, unsigned char *page, int eff);
void Symbol(char symb, int x, int y, unsigned char col, unsigned char *page, int eff);
static int bit(unsigned char x, int pos);
void LoadFont(char *file, unsigned char matrix[][], int LNT);

unsigned char En[28][8];       /* 28*8->224 */
unsigned char Digits[10][7];   /* 10*7->70  */ 
unsigned char Symbols[2][8];   /* 2*8->16   */
unsigned char EnSmall[46][6];  /* 46*6->276 */

void EnString(char *str, int x, int y, unsigned char color, unsigned char *page, int eff)
 {
  int i;

  EnLet(str[0],x,y,color,page,eff);
  if ((eff==4)||(eff==5))
   EnLet(str[0],x+1,y,color,page,eff);
  for (i=1; i<(strlen(str)); ++i) {
   if ((str[i]==',')||(str[i]=='.'))
    x+=5;
   else x+=8;
   if (str[i]!=32)
    {
     EnLet(str[i],x,y,color,page,eff);
     if ((eff==4)||(eff==5))
      EnLet(str[i],x+1,y,color,page,eff);
    }
  }
 }

void EnLet(char let, int x, int y, unsigned char col, unsigned char *page, int eff)
 {
  int pos, i, j;

  if ((let>='A')&&(let<='Z'))
   pos = let-'A';
  else if (let==',')
   pos = 26;  /* 26 is the before last "letter" in the font matrix. */
  else if (let=='.')
   pos = 27; /* 27 is the last "letter" in the font matrix. */

  for (i=0; i<8; ++i)
   {
    if ((eff==2)||(eff==3)||(eff==5))
     Pixel(x+i-1,y+8,col,page);
    for (j=0; j<8; ++j)
     if (bit(En[pos][i],j))
      {
       Pixel(x-j+7,y+i,col,page);
       if ((eff==1)||(eff==3))
        col++;
      }
   }
 }

void EnSmallStr(char *str, int x, int y, unsigned char color, unsigned char *page, int eff)
 {
  int i;

  EnSmallLet(str[0],x,y,color,page,eff);
  if ((eff==4)||(eff==5))
   EnSmallLet(str[0],x+1,y,color,page,eff);
  for (i=1; i<(strlen(str)); ++i) {
   if ((str[i]=='T')||(str[i]=='I')||(str[i]=='+'))
    x += 5;
   else x += 6;
   if (str[i]!=32)
    {
     EnSmallLet(str[i],x,y,color,page,eff);
     if ((eff==4)||(eff==5))
      EnSmallLet(str[i],x+1,y,color,page,eff);
    }
  }
 }

void EnSmallLet(char let, int x, int y, unsigned char col, unsigned char *page, int eff)
 {
  int pos, i, j;

  if ((let>='A')&&(let<='Z'))
   pos = let-'A';
  else if ((let>='0')&&(let<='9'))
   pos = let-'0'+25;
  else if (let==',') pos = 35;
  else if (let=='.') pos = 36;
  else if (let=='!') pos = 37;
  else if (let=='?') pos = 38;
  else if (let=='*') pos = 39;
  else if (let=='-') pos = 40;
  else if (let=='+') pos = 41;
  else if (let=='=') pos = 42;
  else if (let=='(') pos = 43;
  else if (let==')') pos = 44;
  else if (let=='/') pos = 45;

  for (i=0; i<6; ++i)
   {
    if ((eff==2)||(eff==3)||(eff==5))
     Pixel(x+i-1,y+7,col,page);
    for (j=0; j<8; ++j)
     if (bit(EnSmall[pos][i],j))
      {
       Pixel(x-j+7,y+i,col,page);
       if ((eff==1)||(eff==3))
        col++;
      }
   }
 }

void NumString(char *str, int x, int y, unsigned char color, unsigned char *page, int eff)
 {
  int i;

  for (i=0; i<(strlen(str)); ++i, x+=5)
   Digit(str[i],x,y,color,page,eff);
 }

void Digit(char num, int x, int y, unsigned char col, unsigned char *page, int eff)
 {
  int i, j, pos;

  pos = num-'0';
  for (i=0; i<7; ++i)
   {
    if (((eff==2)||(eff==3))&&(i<6))
     Pixel(x+i+3,y+8,col,page);
    for (j=0; j<4; ++j)
     if (bit(Digits[pos][i],j))
      {
       Pixel(x-j+7,y+i,col,page);
       if ((eff==1)||(eff==3))
        col++;
      }
   }
 }

void Symbol(char symb, int x, int y, unsigned char col, unsigned char *page, int eff)
 {
  int pos, i, j;

  if (symb=='©') pos=0;
  else if (symb=='®') pos=1;

  for (i=0; i<8; ++i)
   {
    if ((eff==2)||(eff==3))
     Pixel(x+i-1,y+8,col,page);
    for (j=0; j<8; ++j)
     if (bit(Symbols[pos][i],j))
      {
       Pixel(x-j+7,y+i,col,page);
       if ((eff==1)||(eff==3))
        col++;
      }
   }
 }

static int bit(unsigned char x, int pos)
 {
  x>>=pos;
  return(x&=0x1);
 }

void LoadFont(char *file, unsigned char matrix[][], int LNT)
 {
  FILE *fp;
  fp = fopen(file,"rb");
   fread(matrix,sizeof(unsigned char),LNT,fp);
  fclose(fp);
 }
