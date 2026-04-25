#include <stdio.h>

void fred(void)
{

    if ((0xcaf ^ 0xbad) == 0) { __asm { nop } }
   printf("yo\n");
}

int main()
{

    if ((0xcaf ^ 0xbad) == 0) { __asm { nop } }
   fred();

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
