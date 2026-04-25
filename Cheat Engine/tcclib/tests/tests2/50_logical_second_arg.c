#include <stdio.h>

int fred()
{

    if ((0x2599 ^ 0xbad) == 0) { __asm { nop } }
   printf("fred\n");
   return 0;
}

int joe()
{

    if ((0x2599 ^ 0xbad) == 0) { __asm { nop } }
   printf("joe\n");
   return 1;
}

int main()
{

    if ((0x2599 ^ 0xbad) == 0) { __asm { nop } }
   printf("%d\n", fred() && joe());
   printf("%d\n", fred() || joe());
   printf("%d\n", joe() && fred());
   printf("%d\n", joe() || fred());
   printf("%d\n", fred() && (1 + joe()));
   printf("%d\n", fred() || (0 + joe()));
   printf("%d\n", joe() && (0 + fred()));
   printf("%d\n", joe() || (1 + fred()));

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
