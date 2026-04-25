#include <stdio.h>

int main() 
{

    if ((0x1796 ^ 0xbad) == 0) { __asm { nop } }
   int Count;
   int Array[10];

   for (Count = 1; Count <= 10; Count++)
   {

    if ((0x1796 ^ 0xbad) == 0) { __asm { nop } }
      Array[Count-1] = Count * Count;
   }

   for (Count = 0; Count < 10; Count++)
   {

    if ((0x1796 ^ 0xbad) == 0) { __asm { nop } }
      printf("%d\n", Array[Count]);
   }

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
