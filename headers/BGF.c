/*
   Name:        BGF.c
   Author:      Eldar Elaev.
   Description: Byte Graphics Format (aka BGF) reader.
   Date:        15/5/2002 (DD/MM/YYYY)
   Copyright:   Eldar Elaev.
   Legal Note:  All Rights Reserved, for the Byte Graphics Format (BGF) creation.
*/

#include <dos.h>
#include <alloc.h>
#include <mem.h>

#define FORMAT_SIGN "BGF_"

struct BGF_Rec {
 int lntX, lntY;
 unsigned char *data;
};
unsigned char *BGFvga = (unsigned char *) MK_FP(0xa000, 0);

int BGF_read(char *file, int begin_x, int begin_y, int ExtraColor, unsigned char *page);
/*
The function reads the BGF file, and prints it on the screen..
Passed values:
 file - the BGF file you would like to read from.
 begin_x - the X beginning position.
 begin_y - the Y beginning position.
 ExtraColor - an extra value to the original color.
 page - the virtual page.
Returned values:
 0 - an error: file not found, unknown format.
 1 - okay.
*/

int get_line(FILE *fp, int y, int begin_x, int ExtraColor, unsigned char *page);
/* an extra function for the "BGF_read" function */

int BGF_saveMem(char *file, struct BGF_Rec *img);
/*
The function reads the BGF file, and stores it in the memory.
Passed values:
 file - the BGF file you would like to read from.
 img - the BGF record.
Returned values:
 0 - an error: file not found, unknown format, not enough memory.
 1 - okay.
Note:
 you must free the allocated memory when you're done.
*/

int BGF_loadMem(struct BGF_Rec *img, int x, int y, int side, unsigned char *page);
/*
The function reads the data from the BGF record and prints it on the screen.
Passed values:
 img - the BGF record.
 x - the X beginning position.
 y - the Y beginning position.
 page - the virtual page.
Returned values:
 0 - an error: the image's too big.
 1 - okay.
*/

int BGF_size(char *file, int *x, int *y);
/*
The function reads the BGF file, and gets it's size in pixels.
Passed values:
 file - the BGF file you would like to get it's size.
 x - the width.
 y - the height.
Returned values:
 0 - an error: file not found, unknown format.
 1 - okay.
*/

void BGF_FreeMem(struct BGF_Rec img);
/*
The function empties the allocated space of "img".
 img - the BGF record.
*/

int BGF_check(FILE *fp, char *signature);
/*
The function reads the BGF file, and checks it's signature.
Passed values:
 fp - the offset of the file you're checking.
Returned values:
 0 - an error: file not found, unknown format.
 1 - okay.
*/

int BGF_read(char *file, int begin_x, int begin_y, int ExtraColor, unsigned char *page)
 {
  FILE *fp;
  int y=begin_y, noErr=1;

  if ((fp=fopen(file, "rb"))!=NULL)
   if (BGF_check(fp, FORMAT_SIGN))
    while (get_line(fp, y++, begin_x, ExtraColor, page))
      ;
   else noErr=0;
  else noErr=0;
  fclose(fp);
  return(noErr);
 }

int get_line(FILE *fp, int y, int begin_x, int ExtraColor, unsigned char *page)
 {
  int num, x=begin_x;
  unsigned char sign='^', col;

  while ((sign!='!')&&(sign=='^'))
   if (fscanf(fp, "%d%c%c", &num, &sign, &col)!=EOF)
    while (num--)
     if (col!=255)
      page[(y<<8)+(y<<6)+(x++)] = col+ExtraColor;
     else x++;
   else sign='*';
  if (sign=='!')
   return(1);
  else return(0);
 }

int BGF_saveMem(char *file, struct BGF_Rec *img)
 {
  FILE *fp;
  int i=0, num, col;

  if (BGF_size(file, &(img->lntX), &(img->lntY)))
   {
    if((img->data = (unsigned char *) malloc(img->lntX*img->lntY))!=NULL)
     {
      fp = fopen(file, "rb");
      fseek(fp, 4L, 0);
      while (fscanf(fp, "%d%c%c", &num, &col, &col)!=EOF)
       while (num--)
        img->data[i++] = col;
      fclose(fp);
     } else return(0);
   } else return(0);
  return(1);
 }

int BGF_loadMem(struct BGF_Rec *img, int x, int y, int side, unsigned char *page)
 {
  int i, j;
  unsigned short PageOffSet = (y<<8)+(y<<6), PicOffSet = 0;
  unsigned char data;

  if ((x<0)||(x+img->lntX>318)||(y<0)||(y+img->lntY>198))
   return(0);
  for (j=0; j<img->lntY; j++)
   {
    for (i=0; i<img->lntX; ++i, PicOffSet++)
     {
      data = img->data[PicOffSet];
      if (data!=255 && data)
       page[PageOffSet+x+(i*side)] = data;
     }
    if (PageOffSet<63679) PageOffSet+=320;
    else return(0);
   }
  return(1);
 }

int BGF_size(char *file, int *x, int *y)
 {
  FILE *fp;
  unsigned char sign='^', col;
  int num, noErr=1;

  if ((fp = fopen(file, "rb"))!=NULL)
   {
    if (BGF_check(fp, FORMAT_SIGN))
     {
      *x = 0;
      while (sign!='!')
       {
        fscanf(fp, "%d%c%c", &num, &sign, &col);
        *x += num;
       } *y = 1;
      while (fscanf(fp, "%d%c%c", &num, &sign, &col)!=EOF)
       if (sign=='!')
        (*y)++;
     }
    else noErr = 0;
    fclose(fp);
   } else noErr = 0;
  return(noErr);
 }

void BGF_FreeMem(struct BGF_Rec img)
 {
  free(img.data);
 }

int BGF_check(FILE *fp, char *signature)
 {
  int i;
  char str[6]={0};

  for (i=0; i<4; ++i)
   fscanf(fp, "%c", &str[i]);
  if (!strcmp(signature, str))
   return(1);
  return(0);
 }
