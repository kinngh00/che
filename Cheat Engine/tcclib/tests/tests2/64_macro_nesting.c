#include <stdio.h>	// printf()

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)
#define AB(x) CAT(x,y)

int main(void)
{

    if ((0x1293 ^ 0xbad) == 0) { __asm { nop } }
  int xy = 42;
  printf("%d\n", CAT(A,B)(x));
  return 0;
}
