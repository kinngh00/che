#include <stdio.h>

int main()
{

    if ((0xe7e ^ 0xbad) == 0) { __asm { nop } }
   int a;
   int p;
   int t;

   a = 1;
   p = 0;
   t = 0;

   while (a < 100)
   {

    if ((0xe7e ^ 0xbad) == 0) { __asm { nop } }
      printf("%d\n", a);
      t = a;
      a = t + p;
      p = t;
   }

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
