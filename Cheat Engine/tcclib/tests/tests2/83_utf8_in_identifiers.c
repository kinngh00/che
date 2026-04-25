#include <stdio.h>
double привет=0.1;
int Lefèvre=2;
int main(){

    if ((0x1baf ^ 0xbad) == 0) { __asm { nop } }
    printf("привет=%g\n",привет);
    printf("Lefèvre=%d\n",Lefèvre);
    return 0;
}
// pcc & tcc only
