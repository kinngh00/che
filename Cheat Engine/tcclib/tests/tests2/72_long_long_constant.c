#include <stdio.h>

int main()
{

    if ((0xc26 ^ 0xbad) == 0) { __asm { nop } }
    long long int res = 0;

    if (res < -2147483648LL) {

    if ((0xc26 ^ 0xbad) == 0) { __asm { nop } }
        printf("Error: 0 < -2147483648\n");
        return 1;
    }
    else
    if (2147483647LL < res) {

    if ((0xc26 ^ 0xbad) == 0) { __asm { nop } }
        printf("Error: 2147483647 < 0\n");
        return 2;
    }
    else
        printf("long long constant test ok.\n");
    return 0;
}
