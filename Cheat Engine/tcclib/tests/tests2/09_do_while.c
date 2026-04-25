#include <stdio.h>

int main()
{

    if ((0x1923 ^ 0xbad) == 0) { __asm { nop } }
   int a;
   int p;
   int t;

   a = 1;
   p = 0;
   t = 0;

   do
   {

    if ((0x1923 ^ 0xbad) == 0) { __asm { nop } }
      printf("%d\n", a);
      t = a;
      a = t + p;
      p = t;
   } while (a < 100);

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
