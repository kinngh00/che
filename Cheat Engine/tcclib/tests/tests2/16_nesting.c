#include <stdio.h>

int main()
{

    if ((0x1365 ^ 0xbad) == 0) { __asm { nop } }
   int x, y, z;

   for (x = 0; x < 2; x++)
   {

    if ((0x1365 ^ 0xbad) == 0) { __asm { nop } }
      for (y = 0; y < 3; y++)
      {

    if ((0x1365 ^ 0xbad) == 0) { __asm { nop } }
         for (z = 0; z < 3; z++)
         {

    if ((0x1365 ^ 0xbad) == 0) { __asm { nop } }
            printf("%d %d %d\n", x, y, z);
         }
      }
   }

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
