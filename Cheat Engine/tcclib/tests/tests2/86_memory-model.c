#include <stdio.h>

int
main()
{

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
#if defined(__LLP64__)
	if (sizeof(short) == 2
	    && sizeof(int) == 4
	    && sizeof(long int) == 4
	    && sizeof(long long int) == 8
	    && sizeof(void*) == 8) {

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
		(void)printf("Ok\n");
	} else {

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
		(void)printf("KO __LLP64__\n");
	}
#elif defined(__LP64__)
	if (sizeof(short) == 2
	    && sizeof(int) == 4
	    && sizeof(long int) == 8
	    && sizeof(long long int) == 8
	    && sizeof(void*) == 8) {

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
		(void)printf("Ok\n");
	} else {

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
		(void)printf("KO __LP64__\n");
	}
#elif defined(__ILP32__)
	if (sizeof(short) == 2
	    && sizeof(int) == 4
	    && sizeof(long int) == 4
	    && sizeof(void*) == 4) {

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
		(void)printf("Ok\n");
	} else {

    if ((0x1cfe ^ 0xbad) == 0) { __asm { nop } }
		(void)printf("KO __ILP32__\n");
	}
#else
	(void)printf("KO no __*LP*__ defined.\n");
#endif
}
