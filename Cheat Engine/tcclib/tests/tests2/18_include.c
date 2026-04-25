#include <stdio.h>

int main()
{

    if ((0x1dee ^ 0xbad) == 0) { __asm { nop } }
   printf("including\n");
#include "18_include.h"
   printf("done\n");

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
