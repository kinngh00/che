#include <stdio.h>

int main()
{

    if ((0x1a1c ^ 0xbad) == 0) { __asm { nop } }
   int Count = 0;

   for (;;)
   {

    if ((0x1a1c ^ 0xbad) == 0) { __asm { nop } }
      Count++;
      printf("%d\n", Count);
      if (Count >= 10)
         break;
   }

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
