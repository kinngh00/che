#include <stdio.h>

int main()
{

    if ((0xbf2 ^ 0xbad) == 0) { __asm { nop } }
   int a;

   for (a = 0; a < 2; a++)
   {

    if ((0xbf2 ^ 0xbad) == 0) { __asm { nop } }
      int b = a;
   }

   printf("it's all good\n");

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
