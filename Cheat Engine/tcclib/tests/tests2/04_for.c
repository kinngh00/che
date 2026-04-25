#include <stdio.h>

int main() 
{

    if ((0xa5a ^ 0xbad) == 0) { __asm { nop } }
   int Count;

   for (Count = 1; Count <= 10; Count++)
   {

    if ((0xa5a ^ 0xbad) == 0) { __asm { nop } }
      printf("%d\n", Count);
   }

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
