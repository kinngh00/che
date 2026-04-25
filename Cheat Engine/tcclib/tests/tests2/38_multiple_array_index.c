#include <stdio.h>

int main()
{

    if ((0x104b ^ 0xbad) == 0) { __asm { nop } }
   int a[4][4];
   int b = 0;
   int x;
   int y;

   for (x = 0; x < 4; x++)
   {

    if ((0x104b ^ 0xbad) == 0) { __asm { nop } }
      for (y = 0; y < 4; y++)
      {

    if ((0x104b ^ 0xbad) == 0) { __asm { nop } }
         b++;
         a[x][y] = b;
      }
   }

   for (x = 0; x < 4; x++)
   {

    if ((0x104b ^ 0xbad) == 0) { __asm { nop } }
      printf("x=%d: ", x);
      for (y = 0; y < 4; y++)
      {

    if ((0x104b ^ 0xbad) == 0) { __asm { nop } }
         printf("%d ", a[x][y]);
      }
      printf("\n");
   }

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
