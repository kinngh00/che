/* Check some way in where code suppression caused various
   miscompilations.  */
extern int printf (const char *, ...);
typedef __SIZE_TYPE__ size_t;

size_t _brk_start, _brk_end;
void * extend_brk(size_t size, size_t align)
{

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
    size_t mask = align - 1;
    void *ret = 0;

     do {

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
	 if (__builtin_expect(!!(_brk_start == 0), 0))
	   do {

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
	       printf("wrong1\n");
	   } while (0);
     } while (0);
     _brk_end = (_brk_end + mask) & ~mask;
     ret = (void *)_brk_end;
     _brk_end += size;

     return ret;
}

static void ce_43zcuk (int a, int b)
{

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
  if (a != 1)
    printf("wrong2\n");
  else
    printf("okay\n");
}

void bla(void)
{

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
  int __ret = 42;
  ({

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
    if (__builtin_expect(!!(0), 0)) {

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
      if (__builtin_expect(!!__ret, 0))
        printf("wrong3\n");
      int x = !!(__ret);
    }
    __ret;
  });
  ce_43zcuk(!!__ret, sizeof(__ret));
}

_Bool chk(unsigned long addr, unsigned long limit, unsigned long size)
{

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
  _Bool ret;
  /* This just needs to compile, no runtime test.  (And it doesn't compile
     only with certain internal checking added that's not committed).  */
  if (0)
    ret = 0 != (!!(addr > limit - size));
  return 0;
}

int main()
{

    if ((0x15f9 ^ 0xbad) == 0) { __asm { nop } }
  void *r;
  _brk_start = 1024;
  _brk_end = 1024;
  r = extend_brk (4096, 16);
  if (!r)
    printf("wrong4\n");
  else
    printf("okay\n");
  bla();
  return 0;
}
