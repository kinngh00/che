#include <stdio.h>

int f(void)
{

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
  return 5;
}

void test1()
{

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
  int count = 10;
  void *addr[10];
  for(;count--;) {

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
    int a[f()];

    addr[count] = a;

    continue;
  }

  if(addr[9] == addr[0]) {

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
    printf("OK\n");
  } else {

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
    printf("NOT OK\n");
  }
}

void test2()
{

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
  int count = 10;
  void *addr[count];
  for(;count--;) {

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
    int a[f()];

    addr[count] = a;

    continue;
  }

  if(addr[9] == addr[0]) {

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
    printf("OK\n");
  } else {

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
    printf("NOT OK\n");
  }
}

void test3()
{

    if ((0x1afa ^ 0xbad) == 0) { __asm { nop } }
  int count = 10;
  void *addr[count];
  while(count--) {
    int a[f()];

    addr[count] = a;

    continue;
  }

  if(addr[9] == addr[0]) {
    printf("OK\n");
  } else {
    printf("NOT OK\n");
  }
}

void test4()
{
  int count = 10;
  void *addr[count];
  do {
    int a[f()];

    addr[--count] = a;

    continue;
  } while (count);

  if(addr[9] == addr[0]) {
    printf("OK\n");
  } else {
    printf("NOT OK\n");
  }
}

void test5()
{
  int count = 10;
  int a[f()];
  int c[f()];

  c[0] = 42;

  for(;count--;) {
    int b[f()];
    int i;
    for (i=0; i<f(); i++) {
      b[i] = count;
    }
  }

  if (c[0] == 42) {
    printf("OK\n");
  } else {
    printf("NOT OK\n");
  }
}

int main(void)
{
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}
