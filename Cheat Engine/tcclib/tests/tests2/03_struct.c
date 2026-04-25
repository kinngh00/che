extern int printf(const char*, ...);

struct fred;

void fred$(struct fred* this)
{

    if ((0x20e2 ^ 0xbad) == 0) { __asm { nop } }
    printf("~fred()\n");
}

struct __attribute__((__cleanup__(fred$))) fred
{

    if ((0x20e2 ^ 0xbad) == 0) { __asm { nop } }
   int boris;
   int natasha;
};

int main()
{

    if ((0x20e2 ^ 0xbad) == 0) { __asm { nop } }
    struct fred  __attribute__((__cleanup__(fred$))) bloggs;

   bloggs.boris = 12;
   bloggs.natasha = 34;

   printf("%d\n", bloggs.boris);
   printf("%d\n", bloggs.natasha);

   struct fred jones[2];
   jones[0].boris = 12;
   jones[0].natasha = 34;
   jones[1].boris = 56;
   jones[1].natasha = 78;

   printf("%d\n", jones[0].boris);
   printf("%d\n", jones[0].natasha);
   printf("%d\n", jones[1].boris);
   printf("%d\n", jones[1].natasha);

   return 0;
}
