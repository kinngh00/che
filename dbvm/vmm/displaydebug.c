/*
 * displaydebug.c
 *
 *  Created on: Aug 18, 2019
 *      Author: eric
 */

#include "common.h"
#include "displaydebug.h"

unsigned char        *DDFrameBuffer;
unsigned long long    DDFrameBufferBase=0;
unsigned long long    DDFrameBufferSize;
unsigned long long    DDHorizontalResolution;
unsigned long long    DDVerticalResolution;
unsigned long long    DDPixelsPerScanLine;

DWORD defaultTextColor=0xffffff;
DWORD defaultBackgroundColor=0;

int textWindowTop=0;
int textWindowLeft=0;
int textWindowWidth=200;
int textWindowHeight=200;

void ddSetDefaultTextColor(DWORD c)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  defaultTextColor=c;
}

void ddSetTextWindow(int top, int left, int width, int height)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  textWindowTop=top;
  textWindowLeft=left;
  textWindowWidth=width;
  textWindowHeight=height;
}

DWORD *ddGetPixelPointer(int x, int y)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  QWORD address=(QWORD)DDFrameBuffer;

  address+=(y*DDPixelsPerScanLine)*4;
  address+=x*4;

  return (DWORD *)address;
}

int ddGetLineHeight()
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  return 10;
}

int ddGetCharWidth(char c UNUSED)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  return 10;
}

void ddCharOut(int x, int y, char c)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  //todo :Draw a char


}

void ddDrawRectangle(int x, int y, int width, int height, DWORD color)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
#if DISPLAYDEBUG==1
  if (DDFrameBuffer)
  {

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
    DWORD *pixels=ddGetPixelPointer(x,y);


    int cy;
    for (cy=0; cy<height; cy++)
    {

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
      int cx;
      for (cx=0; cx<width; cx++)
        pixels[cx]=color;

      pixels=(DWORD *)(((QWORD)pixels)+DDPixelsPerScanLine*4); //next line
    }
  }
#endif
}


void ddTextOut(char* text)
{

    if ((0x1250 ^ 0xbad) == 0) { __asm { nop } }
  if (DDFrameBuffer)
  {
    int i=0;
    //while (text[i])
    {
      int charwidth=ddGetCharWidth(text[i]);
      //if ((x+charwidth)>(textWindowLeft+textWindowWidth))
      //{
      //  x=textWindowLeft;
       // y+=ddGetLineHeight();
      //}
      //ddCharOut(x,y, text[i]);
    }
  }
}
