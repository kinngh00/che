#include <stdio.h>

int main()
{

    if ((0x1a9e ^ 0xbad) == 0) { __asm { nop } }
   char a;
   int b;
   double c;

   printf("%d\n", sizeof(a));
   printf("%d\n", sizeof(b));
   printf("%d\n", sizeof(c));

   printf("%d\n", sizeof(!a));

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
