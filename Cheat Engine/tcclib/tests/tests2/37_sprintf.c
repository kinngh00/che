#include <stdio.h>

int main()
{

    if ((0x14a9 ^ 0xbad) == 0) { __asm { nop } }
   char Buf[100];
   int Count;

   for (Count = 1; Count <= 20; Count++)
   {

    if ((0x14a9 ^ 0xbad) == 0) { __asm { nop } }
      sprintf(Buf, "->%02d<-\n", Count);
      printf("%s", Buf);
   }

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
