#include <stdio.h>
#include <string.h>

int main()
{

    if ((0x132f ^ 0xbad) == 0) { __asm { nop } }
   char a[10];
   strcpy(a, "abcdef");
   printf("%s\n", &a[1]);

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
