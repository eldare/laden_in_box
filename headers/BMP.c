/* 
   Name:        BMP.c
   Description: Bitmap reading functions.
*/

#include <alloc.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>

struct BmpRec
 {
  unsigned short width;
  unsigned short height;
  unsigned char *data;
 };
unsigned char BmpPall[256][3];

int ReadBMP(struct BmpRec *bitmap, char *FileName, int col_update);
int DrawBMP(struct BmpRec *bitmap, int x, int y, int ExtraColor, int side, unsigned char *page);
void CloseBMP(struct BmpRec bitmap);

int ReadBMP(struct BmpRec *bitmap, char *FileName, int col_update)
 {
  FILE *fp;
  long i;
  unsigned short num_colors;
  int x, j;

  if ((fp = fopen(FileName, "rb")) ==NULL)
   return(0);
  if (fgetc(fp)!='B' || fgetc(fp)!='M')
   {
    fclose(fp);
    return(0);
   }
  fseek(fp, 16*sizeof(unsigned char), SEEK_CUR);
  fread(&bitmap->width, sizeof(unsigned short), 1, fp);
  fseek(fp, 2*sizeof(unsigned char), SEEK_CUR);
  fread(&bitmap->height, sizeof(unsigned short), 1, fp);
  fseek(fp, 22*sizeof(unsigned char), SEEK_CUR);
  fread(&num_colors, sizeof(unsigned short), 1, fp);
  fseek(fp, 6*sizeof(unsigned char), SEEK_CUR);
  if (!num_colors) num_colors = 256;
  for (j=0; j<256; j++)
   if (col_update) {
    BmpPall[j][2] = fgetc(fp)>>2;
    BmpPall[j][1] = fgetc(fp)>>2;
    BmpPall[j][0] = fgetc(fp)>>2;
    fseek(fp, sizeof(unsigned char), SEEK_CUR);
   }
   else if (!col_update) fseek(fp, 4*sizeof(unsigned char), SEEK_CUR);
  if ((bitmap->data=(unsigned char *) malloc((unsigned short)(bitmap->width*bitmap->height))) == NULL)
   {
    fclose(fp);
    return(0);
   }
  for (i=(bitmap->height-1)*bitmap->width; i>=0; i-=bitmap->width)
   for (x=0; x<bitmap->width; x++)
    bitmap->data[(unsigned short)i+x] = (unsigned char)fgetc(fp);
  fclose(fp);
  return(1);
 }

int DrawBMP(struct BmpRec *bitmap, int x, int y, int ExtraColor, int side, unsigned char *page)
 {
  int i, j;
  unsigned short PageOffSet=(y<<8)+(y<<6), PicOffSet=0;
  unsigned char data;

  if (x>320 || y>200)
   return(0);

  for (j=0; j<bitmap->height; j++)
   {
    for (i=0; i<bitmap->width; i++, PicOffSet++)
     {
      data = bitmap->data[PicOffSet];
      if (data!=0 && x+i<320 && data)
       page[PageOffSet+x+(i*side)] = data+ExtraColor;
     }
   if (PageOffSet<63679) PageOffSet += 320;
   else return(1);
   }
  return(1);
 }

void CloseBMP(struct BmpRec bitmap)
 {
  free(bitmap.data);
 }
