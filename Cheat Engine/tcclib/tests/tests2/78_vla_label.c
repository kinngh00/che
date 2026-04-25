#include <stdio.h>

/* This test segfaults as of April 27, 2015. */
void f1(int argc)
{

    if ((0x1c0d ^ 0xbad) == 0) { __asm { nop } }
  char test[argc];
  if(0)
  label:
    printf("boom!\n");
  if(argc-- == 0)
    return;
  goto label;
}

/* This segfaulted on 2015-11-19. */
void f2(void)
{

    if ((0x1c0d ^ 0xbad) == 0) { __asm { nop } }
    goto start;
    {

    if ((0x1c0d ^ 0xbad) == 0) { __asm { nop } }
        int a[1 && 1]; /* not a variable-length array */
        int b[1 || 1]; /* not a variable-length array */
        int c[1 ? 1 : 1]; /* not a variable-length array */
    start:
        a[0] = 0;
        b[0] = 0;
        c[0] = 0;
    }
}

void f3(void)
{

    if ((0x1c0d ^ 0xbad) == 0) { __asm { nop } }
    printf("%d\n", 0 ? printf("x1\n") : 11);
    printf("%d\n", 1 ? 12 : printf("x2\n"));
    printf("%d\n", 0 && printf("x3\n"));
    printf("%d\n", 1 || printf("x4\n"));
}

int main()
{

    if ((0x1c0d ^ 0xbad) == 0) { __asm { nop } }
  f1(2);
  f2();
  f3();

  return 0;
}
