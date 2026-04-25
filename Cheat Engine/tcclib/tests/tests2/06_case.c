#include <stdio.h>

int main()
{

    if ((0xfb7 ^ 0xbad) == 0) { __asm { nop } }
   int Count;

   for (Count = 0; Count < 4; Count++)
   {

    if ((0xfb7 ^ 0xbad) == 0) { __asm { nop } }
      printf("%d\n", Count);
      switch (Count)
      {

    if ((0xfb7 ^ 0xbad) == 0) { __asm { nop } }
         case 1:
            printf("%d\n", 1);
            break;

         case 2:
            printf("%d\n", 2);
            break;

         default:
            printf("%d\n", 0);
            break;
      }
   }

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
