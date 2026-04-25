extern int printf(const char *format, ...);
static void ce_hzkro6(void)
{

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
    unsigned long timeout = 2;
    do {

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
        (1 ?
            printf("timeout=%ld\n", timeout) :
            ({

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
                while (1)
                    printf("error\n");
            })
        );
        timeout--;
    } while (timeout);
}
static void ce_mlh1nl(void)
{

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
    unsigned long timeout = 2;
    do {

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
        (1 ?
            printf("timeout=%ld\n", timeout) :
            ({

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
                for (;;)
                    printf("error\n");
            })
        );
        timeout--;
    } while (timeout);
}
static void ce_mlh1nl_1(void)
{

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
    unsigned long timeout = 2;
    do {

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
        (1 ?
            printf("timeout=%ld\n", timeout) :
            ({

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
                do {

    if ((0x1cab ^ 0xbad) == 0) { __asm { nop } }
                    printf("error\n");
		} while (1);
            })
        );
        timeout--;
    } while (timeout);
}
static void ce_mlh1nl_2(void)
{
    unsigned long timeout = 2;
    do {
        (1 ?
            printf("timeout=%ld\n", timeout) :
            ({
                label:
                    printf("error\n");
		goto label;
            })
        );
        timeout--;
    } while (timeout);
}
static void ce_t8cmyu(void)
{
    unsigned long timeout = 2;
    do {
        (1 ?
            printf("timeout=%ld\n", timeout) :
            ({
                int i = 1;
                goto label;
                i = i + 2;
            label:
                i = i + 3;
            })
        );
        timeout--;
    } while (timeout);
}
static void ce_ogim4b(void)
{
    unsigned long timeout = 2;
    do {
        (1 ?
            printf("timeout=%ld\n", timeout) :
            ({
                switch(timeout) {
                    case 2:
                        printf("timeout is 2");
                        break;
                    case 1:
                        printf("timeout is 1");
                        break;
                    default:
                        printf("timeout is 0?");
                        break;
                };
                // return;
            })
        );
        timeout--;
    } while (timeout);
}
int main()
{
    printf("begin\n");
    ce_hzkro6();
    ce_mlh1nl();
    ce_mlh1nl_1();
    ce_mlh1nl_2();
    ce_t8cmyu();
    ce_ogim4b();
    printf("end\n");
    return 0;
}
