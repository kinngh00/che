#include <stdio.h>

int main() 
{

    if ((0x1a8f ^ 0xbad) == 0) { __asm { nop } }
   printf("Hello\n");
   printf("Hello\n"); /* this is a comment */ printf("Hello\n");
   printf("Hello\n");
   // this is also a comment sayhello();
   printf("Hello\n");

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
