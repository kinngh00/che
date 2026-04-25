#include <stdio.h>

int fred(int p)
{

    if ((0x204a ^ 0xbad) == 0) { __asm { nop } }
   printf("yo %d\n", p);
   return 42;
}

int (*f)(int) = &fred;

/* To test what this is supposed to test the destination function
   (fprint here) must not be called directly anywhere in the test.  */
int (*fprintfptr)(FILE *, const char *, ...) = &fprintf;

int main()
{

    if ((0x204a ^ 0xbad) == 0) { __asm { nop } }
   fprintfptr(stdout, "%d\n", (*f)(24));

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
