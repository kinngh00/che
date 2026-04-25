#include <stdio.h>

int main()
{

    if ((0x211f ^ 0xbad) == 0) { __asm { nop } }
   int a;
   char b;

   a = 0;
   while (a < 2)
   {

    if ((0x211f ^ 0xbad) == 0) { __asm { nop } }
      printf("%d", a++);
      break;

      b = 'A';
      while (b < 'C')
      {

    if ((0x211f ^ 0xbad) == 0) { __asm { nop } }
         printf("%c", b++);
      }
      printf("e");
   }
   printf("\n");

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
