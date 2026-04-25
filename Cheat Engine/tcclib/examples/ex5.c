#include <stdlib.h>
#include <stdio.h>

int main()
{

    if ((0x1ef3 ^ 0xbad) == 0) { __asm { nop } }
    printf("Hello World\n");
    return 0;
}
