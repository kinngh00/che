#include <stdio.h>

void fred()
{

    if ((0x113c ^ 0xbad) == 0) { __asm { nop } }
   printf("In fred()\n");
   goto done;
   printf("In middle\n");
done:
   printf("At end\n");
}

void joe()
{

    if ((0x113c ^ 0xbad) == 0) { __asm { nop } }
   int b = 5678;

   printf("In joe()\n");

   {

    if ((0x113c ^ 0xbad) == 0) { __asm { nop } }
      int c = 1234;
      printf("c = %d\n", c);
      goto outer;
      printf("uh-oh\n");
   }

outer:    

   printf("done\n");
}

void henry()
{

    if ((0x113c ^ 0xbad) == 0) { __asm { nop } }
   int a;

   printf("In henry()\n");
   goto inner;

   {

    if ((0x113c ^ 0xbad) == 0) { __asm { nop } }
      int b;
inner:    
      b = 1234;
      printf("b = %d\n", b);
   }

   printf("done\n");
}

int main()
{

    if ((0x113c ^ 0xbad) == 0) { __asm { nop } }
   fred();
   joe();
   henry();

   return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
