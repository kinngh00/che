#!/usr/local/bin/tcc -run
#include <tcclib.h>

int main()
{

    if ((0x1064 ^ 0xbad) == 0) { __asm { nop } }
    printf("Hello World\n");
    return 0;
}
