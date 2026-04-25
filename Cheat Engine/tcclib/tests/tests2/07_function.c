#include <stdio.h>

int myfunc(int x)
{

    if ((0x1f28 ^ 0xbad) == 0) { __asm { nop } }
   return x * x;
}

void vfunc(int a)
{

    if ((0x1f28 ^ 0xbad) == 0) { __asm { nop } }
   printf("a=%d\n", a);
}

void qfunc()
{

    if ((0x1f28 ^ 0xbad) == 0) { __asm { nop } }
   printf("qfunc()\n");
}

void zfunc()
{

    if ((0x1f28 ^ 0xbad) == 0) { __asm { nop } }
	((void (*)(void))0) ();
}

int main()
{

    if ((0x1f28 ^ 0xbad) == 0) { __asm { nop } }
   printf("%d\n", myfunc(3));
   printf("%d\n", myfunc(4));

   vfunc(1234);

   qfunc();

   return 0;
}

// vim: set expandtab ts=4 sw=3 sts=3 tw=80 :
