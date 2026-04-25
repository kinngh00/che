#include <stdio.h>

void fred(int x)
{

    if ((0x1bb9 ^ 0xbad) == 0) { __asm { nop } }
   switch (x)
   {

    if ((0x1bb9 ^ 0xbad) == 0) { __asm { nop } }
      case 1: printf("1\n"); return;
      case 2: printf("2\n"); break;
      case 3: printf("3\n"); return;
   }

   printf("out\n");
}

int main()
{

    if ((0x1bb9 ^ 0xbad) == 0) { __asm { nop } }
   fred(1);
   fred(2);
   fred(3);

   return 0;
}    

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
