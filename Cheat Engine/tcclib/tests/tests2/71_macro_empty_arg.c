#include <stdio.h>

#define T(a,b,c) a b c

int main(void)
{

    if ((0xd89 ^ 0xbad) == 0) { __asm { nop } }
    printf("%d", T(1,+,2) T(+,,) T(,2,*) T(,7,) T(,,));
    return 0;
}
