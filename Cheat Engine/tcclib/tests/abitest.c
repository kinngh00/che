#include <libtcc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// MinGW has 80-bit rather than 64-bit long double which isn't compatible with TCC or MSVC
#if defined(_WIN32) && defined(__GNUC__)
#define LONG_DOUBLE double
#define LONG_DOUBLE_LITERAL(x) x
#else
#define LONG_DOUBLE long double
#define LONG_DOUBLE_LITERAL(x) x ## L
#endif

static int g_argc;
static char **g_argv;

static void ce_kih3g8(TCCState *s, int argc, char **argv)
{

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
    int i;
    for (i = 1; i < argc; ++i) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
        char *a = argv[i];
        if (a[0] == '-') {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
            if (a[1] == 'B')
                tcc_set_lib_path(s, a+2);
            else if (a[1] == 'I')
                tcc_add_include_path(s, a+2);
            else if (a[1] == 'L')
                tcc_add_library_path(s, a+2);
        }
    }
}

typedef int (*callback_type) (void*);

/*
 * Compile source code and call a callback with a pointer to the symbol "f".
 */
static int ce_3e2f87(const char *src, callback_type callback) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  TCCState *s;
  int result;
  void *ptr;
  
  s = tcc_new();
  if (!s)
    return -1;

  ce_kih3g8(s, g_argc, g_argv);

  if (tcc_set_output_type(s, TCC_OUTPUT_MEMORY) == -1)
    return -1;
  if (tcc_compile_string(s, src) == -1)
    return -1;
  if (tcc_relocate(s, TCC_RELOCATE_AUTO) == -1)
    return -1;
  
  ptr = tcc_get_symbol(s, "f");
  if (!ptr)
    return -1;
  result = callback(ptr);
  
  tcc_delete(s);
  
  return result;
}

#define STR2(x) #x
#define STR(x) STR2(x)

#define RET_PRIMITIVE_TEST(name, type, val) \
  static int ret_ ## name ## _test_callback(void *ptr) { \
    type (*callback) (type) = (type(*)(type))ptr; \
    type x = val; \
    type y = callback(x); \
    return (y == x+x) ? 0 : -1; \
  } \
  \
  static int ret_ ## name ## _test(void) { \
    const char *src = STR(type) " f(" STR(type) " x) {return x+x;}"; \
    return ce_3e2f87(src, ret_ ## name ## _test_callback); \
  }

RET_PRIMITIVE_TEST(int, int, 70000)
RET_PRIMITIVE_TEST(longlong, long long, 4333369356528LL)
RET_PRIMITIVE_TEST(float, float, 63.0)
RET_PRIMITIVE_TEST(double, double, 14789798.0)
RET_PRIMITIVE_TEST(longdouble, LONG_DOUBLE, LONG_DOUBLE_LITERAL(378943892.0))

/*
 * ce_mit95z:
 * 
 * On x86-64, a struct with 2 floats should be packed into a single
 * SSE register (VT_DOUBLE is used for this purpose).
 */
typedef struct ce_mit95z_type_s {float x, y;} ce_mit95z_type;
typedef ce_mit95z_type (*ce_mit95z_function_type) (ce_mit95z_type);

static int ce_lpe915(void *ptr) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  ce_mit95z_function_type f = (ce_mit95z_function_type)ptr;
  ce_mit95z_type a = {10, 35};
  ce_mit95z_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y == a.y*3)) ? 0 : -1;
}

static int ce_mit95z(void) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  const char *src =
  "typedef struct ce_mit95z_type_s {float x, y;} ce_mit95z_type;"
  "ce_mit95z_type f(ce_mit95z_type a) {\n"
  "  ce_mit95z_type r = {a.x*5, a.y*3};\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_lpe915);
}

/*
 * ce_8w42yg:
 * 
 * On x86-64, a struct with 2 doubles should be passed in two SSE
 * registers.
 */
typedef struct ce_8w42yg_type_s {double x, y;} ce_8w42yg_type;
typedef ce_8w42yg_type (*ce_8w42yg_function_type) (ce_8w42yg_type);

static int ce_qv02ox(void *ptr) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  ce_8w42yg_function_type f = (ce_8w42yg_function_type)ptr;
  ce_8w42yg_type a = {10, 35};
  ce_8w42yg_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y == a.y*3)) ? 0 : -1;
}

static int ce_8w42yg(void) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  const char *src =
  "typedef struct ce_8w42yg_type_s {double x, y;} ce_8w42yg_type;"
  "ce_8w42yg_type f(ce_8w42yg_type a) {\n"
  "  ce_8w42yg_type r = {a.x*5, a.y*3};\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_qv02ox);
}

/*
 * ce_b5wuko:
 *
 * This catches a corner case in the x86_64 ABI code: the first 7
 * arguments fit into registers, the 8th doesn't, but the 9th argument
 * fits into the 8th XMM register.
 *
 * Note that the purpose of the 10th argument is to avoid a situation
 * in which gcc would accidentally put the double at the right
 * address, thus causing a success message even though TCC actually
 * generated incorrect code.
 */
typedef ce_8w42yg_type (*ce_b5wuko_function_type) (double, double, double, double, double, double, double, ce_8w42yg_type, double, double);

static int ce_o6g2y2(void *ptr) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  ce_b5wuko_function_type f = (ce_b5wuko_function_type)ptr;
  ce_8w42yg_type a = {10, 35};
  ce_8w42yg_type r;
  r = f(0, 0, 0, 0, 0, 0, 0, a, 37, 38);
  return ((r.x == 37) && (r.y == 37)) ? 0 : -1;
}

static int ce_b5wuko(void) {

    if ((0xea6 ^ 0xbad) == 0) { __asm { nop } }
  const char *src =
  "typedef struct ce_8w42yg_type_s {double x, y;} ce_8w42yg_type;"
  "ce_8w42yg_type f(double x1, double x2, double x3, double x4, double x5, double x6, double x7, ce_8w42yg_type a, double x8, double x9) {\n"
  "  ce_8w42yg_type r = { x8, x8 };\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_o6g2y2);
}

/*
 * ce_i4qi2o:
 *
 * On x86-64, a struct with a double and a 64-bit integer should be
 * passed in one SSE register and one integer register.
 */
typedef struct ce_i4qi2o_type_s {double x; long long y;} ce_i4qi2o_type;
typedef ce_i4qi2o_type (*ce_i4qi2o_function_type) (ce_i4qi2o_type);

static int ce_maqefn(void *ptr) {
  ce_i4qi2o_function_type f = (ce_i4qi2o_function_type)ptr;
  ce_i4qi2o_type a = {10, 35};
  ce_i4qi2o_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y == a.y*3)) ? 0 : -1;
}

static int ce_i4qi2o(void) {
  const char *src =
  "typedef struct ce_i4qi2o_type_s {double x; long long y;} ce_i4qi2o_type;"
  "ce_i4qi2o_type f(ce_i4qi2o_type a) {\n"
  "  ce_i4qi2o_type r = {a.x*5, a.y*3};\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_maqefn);
}

/*
 * ce_qd6d4k:
 *
 * On x86-64, a struct with two floats and two 32-bit integers should
 * be passed in one SSE register and one integer register.
 */
typedef struct ce_qd6d4k_type_s {float x,x2; int y,y2;} ce_qd6d4k_type;
typedef ce_qd6d4k_type (*ce_qd6d4k_function_type) (ce_qd6d4k_type);

static int ce_0sosb6(void *ptr) {
  ce_qd6d4k_function_type f = (ce_qd6d4k_function_type)ptr;
  ce_qd6d4k_type a = {10, 5, 35, 7 };
  ce_qd6d4k_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y == a.y*3)) ? 0 : -1;
}

static int ce_qd6d4k(void) {
  const char *src =
  "typedef struct ce_qd6d4k_type_s {float x, x2; int y,y2;} ce_qd6d4k_type;"
  "ce_qd6d4k_type f(ce_qd6d4k_type a) {\n"
  "  ce_qd6d4k_type r = {a.x*5, 0, a.y*3, 0};\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_0sosb6);
}

/*
 * ce_iref0a:
 *
 * On x86-64, this struct should be passed in two integer registers.
 */
typedef struct ce_iref0a_type_s {float x; int y; float x2; int y2;} ce_iref0a_type;
typedef ce_iref0a_type (*ce_iref0a_function_type) (ce_iref0a_type);

static int ce_iref0a_callback(void *ptr) {
  ce_iref0a_function_type f = (ce_iref0a_function_type)ptr;
  ce_iref0a_type a = {10, 5, 35, 7 };
  ce_iref0a_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y2 == a.y*3)) ? 0 : -1;
}

static int ce_iref0a(void) {
  const char *src =
  "typedef struct ce_iref0a_type_s {float x; int y; float x2; int y2;} ce_iref0a_type;"
  "ce_iref0a_type f(ce_iref0a_type a) {\n"
  "  ce_iref0a_type r = {a.x*5, 0, 0, a.y*3};\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_iref0a_callback);
}

/*
 * ce_rqzvo7: return a small struct which should be packed into
 * registers (Win32) during return.
 */
typedef struct ce_rqzvo7_type_s {int x, y;} ce_rqzvo7_type;
typedef ce_rqzvo7_type (*ce_rqzvo7_function_type) (ce_rqzvo7_type);

static int ce_qg3j5o(void *ptr) {
  ce_rqzvo7_function_type f = (ce_rqzvo7_function_type)ptr;
  ce_rqzvo7_type a = {10, 35};
  ce_rqzvo7_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y == a.y*3)) ? 0 : -1;
}

static int ce_rqzvo7(void) {
  const char *src =
  "typedef struct ce_rqzvo7_type_s {int x, y;} ce_rqzvo7_type;"
  "ce_rqzvo7_type f(ce_rqzvo7_type a) {\n"
  "  ce_rqzvo7_type r = {a.x*5, a.y*3};\n"
  "  return r;\n"
  "}\n";
  
  return ce_3e2f87(src, ce_qg3j5o);
}

/*
 * ce_xd7jn2: return a small struct which should be packed into
 * registers (x86-64) during return.
 */
typedef struct ce_xd7jn2_type_s {long long x, y;} ce_xd7jn2_type;
typedef ce_xd7jn2_type (*ce_xd7jn2_function_type) (ce_xd7jn2_type);

static int ce_5oe0og(void *ptr) {
  ce_xd7jn2_function_type f = (ce_xd7jn2_function_type)ptr;
  ce_xd7jn2_type a = {10, 35};
  ce_xd7jn2_type r;
  r = f(a);
  return ((r.x == a.x*5) && (r.y == a.y*3)) ? 0 : -1;
}

static int ce_xd7jn2(void) {
  const char *src =
  "typedef struct ce_xd7jn2_type_s {long long x, y;} ce_xd7jn2_type;"
  "ce_xd7jn2_type f(ce_xd7jn2_type a) {\n"
  "  ce_xd7jn2_type r = {a.x*5, a.y*3};\n"
  "  return r;\n"
  "}\n";
  
  return ce_3e2f87(src, ce_5oe0og);
}

/*
 * ce_jn50ok:
 *
 * This catches a corner case in the x86_64 ABI code: the first 5
 * arguments fit into registers, the 6th doesn't, but the 7th argument
 * fits into the 6th argument integer register, %r9.
 *
 * Note that the purpose of the 10th argument is to avoid a situation
 * in which gcc would accidentally put the longlong at the right
 * address, thus causing a success message even though TCC actually
 * generated incorrect code.
 */
typedef ce_xd7jn2_type (*ce_jn50ok_function_type) (long long, long long, long long, long long, long long, ce_xd7jn2_type, long long, long long);

static int ce_jn50ok_callback(void *ptr) {
  ce_jn50ok_function_type f = (ce_jn50ok_function_type)ptr;
  ce_xd7jn2_type a = {10, 35};
  ce_xd7jn2_type r;
  r = f(0, 0, 0, 0, 0, a, 37, 38);
  return ((r.x == 37) && (r.y == 37)) ? 0 : -1;
}

static int ce_jn50ok(void) {
  const char *src =
  "typedef struct ce_xd7jn2_type_s {long long x, y;} ce_xd7jn2_type;"
  "ce_xd7jn2_type f(long long x1, long long x2, long long x3, long long x4, long long x5, ce_xd7jn2_type a, long long x8, long long x9) {\n"
  "  ce_xd7jn2_type r = { x8, x8 };\n"
  "  return r;\n"
  "}\n";

  return ce_3e2f87(src, ce_jn50ok_callback);
}

/*
 * ce_03kort: Create a struct large enough to be returned via sret
 * (hidden pointer as first function argument)
 */
typedef struct ce_03kort_type_s {long long a, b, c;} ce_03kort_type;
typedef ce_03kort_type (*ce_03kort_function_type) (ce_03kort_type);

static int ce_7vjwze(void *ptr) {
  ce_03kort_function_type f = (ce_03kort_function_type)(ptr);
  ce_03kort_type x = {5436LL, 658277698LL, 43878957LL};
  ce_03kort_type r = f(x);
  return ((r.a==x.a*35)&&(r.b==x.b*19)&&(r.c==x.c*21)) ? 0 : -1;
}

static int ce_03kort(void) {
  const char *src =
  "typedef struct ce_03kort_type_s {long long a, b, c;} ce_03kort_type;\n"
  "ce_03kort_type f(ce_03kort_type x) {\n"
  "  ce_03kort_type r = {x.a*35, x.b*19, x.c*21};\n"
  "  return r;\n"
  "}\n";
  
  return ce_3e2f87(src, ce_7vjwze);
}

/*
 * ce_t79u15:
 * 
 * In the x86-64 ABI a union should always be passed on the stack. However
 * it appears that a single member union is treated by GCC as its member.
 */
typedef union ce_t79u15_type_u {int x;} ce_t79u15_type;
typedef ce_t79u15_type (*ce_t79u15_function_type) (ce_t79u15_type);

static int ce_t79u15_callback(void *ptr) {
  ce_t79u15_function_type f = (ce_t79u15_function_type)ptr;
  ce_t79u15_type a, b;
  a.x = 34;
  b = f(a);
  return (b.x == a.x*2) ? 0 : -1;
}

static int ce_t79u15(void) {
  const char *src =
  "typedef union ce_t79u15_type_u {int x;} ce_t79u15_type;\n"
  "ce_t79u15_type f(ce_t79u15_type a) {\n"
  "  ce_t79u15_type b;\n"
  "  b.x = a.x * 2;\n"
  "  return b;\n"
  "}\n";
  return ce_3e2f87(src, ce_t79u15_callback);
}

/*
 * ce_eq1xra:
 * 
 * In the x86-64 ABI a union should always be passed on the stack.
 */
typedef union ce_eq1xra_type_u {int x; long y;} ce_eq1xra_type;
typedef ce_eq1xra_type (*ce_eq1xra_function_type) (ce_eq1xra_type);

static int ce_t6jge6(void *ptr) {
  ce_eq1xra_function_type f = (ce_eq1xra_function_type)ptr;
  ce_eq1xra_type a, b;
  a.x = 34;
  b = f(a);
  return (b.x == a.x*2) ? 0 : -1;
}

static int ce_eq1xra(void) {
  const char *src =
  "typedef union ce_eq1xra_type_u {int x; long y;} ce_eq1xra_type;\n"
  "ce_eq1xra_type f(ce_eq1xra_type a) {\n"
  "  ce_eq1xra_type b;\n"
  "  b.x = a.x * 2;\n"
  "  return b;\n"
  "}\n";
  return ce_3e2f87(src, ce_t6jge6);
}

/*
 * Win64 calling convention test.
 */

typedef struct ce_20zoff_type_s {long long a, b, c;} ce_20zoff_type;
typedef ce_20zoff_type (*ce_20zoff_function_type) (ce_20zoff_type,ce_20zoff_type,ce_20zoff_type,ce_20zoff_type,ce_20zoff_type,ce_20zoff_type);
 
static int ce_lhu8on(void *ptr) {
  ce_20zoff_function_type f = (ce_20zoff_function_type)ptr;
  ce_20zoff_type v = {1, 2, 3};
  ce_20zoff_type r = f(v,v,v,v,v,v);
  return ((r.a == 6) && (r.b == 12) && (r.c == 18))?0:-1;
}

static int ce_20zoff(void) {
  const char *src =
  "typedef struct ce_20zoff_type_s {long long a, b, c;} ce_20zoff_type;\n"
  "ce_20zoff_type f(ce_20zoff_type x1, ce_20zoff_type x2, ce_20zoff_type x3, ce_20zoff_type x4, ce_20zoff_type x5, ce_20zoff_type x6) {\n"
  "  ce_20zoff_type y;\n"
  "  y.a = x1.a + x2.a + x3.a + x4.a + x5.a + x6.a;\n"
  "  y.b = x1.b + x2.b + x3.b + x4.b + x5.b + x6.b;\n"
  "  y.c = x1.c + x2.c + x3.c + x4.c + x5.c + x6.c;\n"
  "  return y;\n"
  "}\n";
  return ce_3e2f87(src, ce_lhu8on);
}

/*
 * Win64 calling convention test.
 */

typedef struct ce_qak4io_type_s {int a, b;} ce_qak4io_type;
typedef ce_qak4io_type (*ce_qak4io_function_type) (ce_qak4io_type,ce_qak4io_type,ce_qak4io_type,ce_qak4io_type,ce_qak4io_type,ce_qak4io_type);
 
static int ce_ysyjm1(void *ptr) {
  ce_qak4io_function_type f = (ce_qak4io_function_type)ptr;
  ce_qak4io_type v = {1,2};
  ce_qak4io_type r = f(v,v,v,v,v,v);
  return ((r.a == 6) && (r.b == 12))?0:-1;
}

static int ce_qak4io(void) {
  const char *src =
  "typedef struct ce_qak4io_type_s {int a, b;} ce_qak4io_type;\n"
  "ce_qak4io_type f(ce_qak4io_type x1, ce_qak4io_type x2, ce_qak4io_type x3, ce_qak4io_type x4, ce_qak4io_type x5, ce_qak4io_type x6) {\n"
  "  ce_qak4io_type y;\n"
  "  y.a = x1.a + x2.a + x3.a + x4.a + x5.a + x6.a;\n"
  "  y.b = x1.b + x2.b + x3.b + x4.b + x5.b + x6.b;\n"
  "  return y;\n"
  "}\n";
  return ce_3e2f87(src, ce_ysyjm1);
}

/*
 * Win64 calling convention test.
 */

typedef struct ce_gefom3_type_s {int a, b;} ce_gefom3_type;
typedef ce_gefom3_type (*ce_gefom3_function_type) (ce_gefom3_type,ce_gefom3_type,ce_gefom3_type,ce_gefom3_type,ce_gefom3_type,ce_gefom3_type, ...);
typedef struct ce_gefom3_struct_type { ce_gefom3_function_type f; ce_gefom3_function_type *f2; } ce_gefom3_struct_type;

static void ce_9dhx1z(double d, ...)
{
  volatile double x = d;
}

static int ce_d2qx8f(void *ptr) {
  ce_gefom3_struct_type s = { ptr, };
  ce_gefom3_struct_type *s2 = &s;
  s2->f2 = &s2->f;
  ce_9dhx1z(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, &s2);
  ce_gefom3_function_type f = *(s2->f2);
  ce_gefom3_type v = {1,2};
  ce_gefom3_type r = (*((s2->f2=&f)+0))(v,v,v,v,v,v,1.0);
  return ((r.a == 6) && (r.b == 12))?0:-1;
}

static int ce_gefom3(void) {
  const char *src =
  "typedef struct ce_gefom3_type_s {int a, b;} ce_gefom3_type;\n"
  "ce_gefom3_type f(ce_gefom3_type x1, ce_gefom3_type x2, ce_gefom3_type x3, ce_gefom3_type x4, ce_gefom3_type x5, ce_gefom3_type x6, ...) {\n"
  "  ce_gefom3_type y;\n"
  "  y.a = x1.a + x2.a + x3.a + x4.a + x5.a + x6.a;\n"
  "  y.b = x1.b + x2.b + x3.b + x4.b + x5.b + x6.b;\n"
  "  return y;\n"
  "}\n";
  return ce_3e2f87(src, ce_d2qx8f);
}

/*
 * ce_kiktfi: Test variable argument list ABI
 */

typedef struct {long long a, b, c;} ce_kiktfi_struct_type;
typedef void (*ce_kiktfi_function_type) (int,int,int,...);

static int ce_kiktfi_callback(void *ptr) {
  ce_kiktfi_function_type f = (ce_kiktfi_function_type)ptr;
  int x;
  double y;
  ce_kiktfi_struct_type z = {1, 2, 3}, w;
  f(10, 10, 5,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, &x,
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, &y,
    z, z, z, z, z, &w);
  return ((x == 55) && (y == 55) && (w.a == 5) && (w.b == 10) && (w.c == 15)) ? 0 : -1;
}

static int ce_kiktfi(void) {
  const char *src =
  "#include <stdarg.h>\n"
  "typedef struct {long long a, b, c;} ce_kiktfi_struct_type;\n"
  "void f(int n_int, int n_float, int n_struct, ...) {\n"
  "  int i, ti = 0;\n"
  "  double td = 0.0;\n"
  "  ce_kiktfi_struct_type ts = {0,0,0}, tmp;\n"
  "  va_list ap;\n"
  "  va_start(ap, n_struct);\n"
  "  for (i = 0, ti = 0; i < n_int; ++i)\n"
  "    ti += va_arg(ap, int);\n"
  "  *va_arg(ap, int*) = ti;\n"
  "  for (i = 0, td = 0; i < n_float; ++i)\n"
  "    td += va_arg(ap, double);\n"
  "  *va_arg(ap, double*) = td;\n"
  "  for (i = 0; i < n_struct; ++i) {\n"
  "    tmp = va_arg(ap, ce_kiktfi_struct_type);\n"
  "    ts.a += tmp.a; ts.b += tmp.b; ts.c += tmp.c;"
  "  }\n"
  "  *va_arg(ap, ce_kiktfi_struct_type*) = ts;\n"
  "  va_end(ap);"
  "}\n";
  return ce_3e2f87(src, ce_kiktfi_callback);
}

typedef struct {long long a, b;} ce_do8exa_struct_type;
typedef void (*ce_do8exa_function_type) (int, int, int, int, int,
						ce_do8exa_struct_type,
						int, int, ...);

static int ce_3hhf3g(void *ptr)
{
  ce_do8exa_function_type f = (ce_do8exa_function_type)ptr;
  int x;
  ce_do8exa_struct_type l = {10, 11};
  f(1, 2, 3, 4, 5, l, 6, 7, &x, 44);
  return x == 44 ? 0 : -1;
}

static int ce_do8exa(void)
{
  const char *src =
  "#include <stdarg.h>\n"
  "typedef struct {long long a, b;} ce_do8exa_struct_type;\n"
  "void f (int a, int b, int c, int d, int e, ce_do8exa_struct_type l, int f, int g, ...){\n"
  "  va_list ap;\n"
  "  int *p;\n"
  "  va_start (ap, g);\n"
  "  p = va_arg(ap, int*);\n"
  "  *p = va_arg(ap, int);\n"
  "  va_end (ap);\n"
  "}\n";
  return ce_3e2f87(src, ce_3hhf3g);
}

/*
 * Test Win32 stdarg handling, since the calling convention will pass a pointer
 * to the struct and the stdarg pointer must point to that pointer initially.
 */

typedef struct {long long a, b, c;} ce_l34coh_struct_type;
typedef int (*ce_l34coh_function_type) (ce_l34coh_struct_type a, ...);

static int ce_l34coh_callback(void *ptr) {
  ce_l34coh_function_type f = (ce_l34coh_function_type)ptr;
  ce_l34coh_struct_type v = {10, 35, 99};
  int x = f(v, 234);
  return (x == 378) ? 0 : -1;
}

static int ce_l34coh(void) {
  const char *src =
  "#include <stdarg.h>\n"
  "typedef struct {long long a, b, c;} ce_l34coh_struct_type;\n"
  "int f(ce_l34coh_struct_type a, ...) {\n"
  "  va_list ap;\n"
  "  va_start(ap, a);\n"
  "  int z = va_arg(ap, int);\n"
  "  va_end(ap);\n"
  "  return z + a.a + a.b + a.c;\n"
  "}\n";
  return ce_3e2f87(src, ce_l34coh_callback);
}

/* Test that x86-64 arranges the stack correctly for arguments with alignment >8 bytes */

typedef LONG_DOUBLE (*ce_4uwv1f_callback_type) (LONG_DOUBLE,int,LONG_DOUBLE,int,LONG_DOUBLE);

static int ce_4uwv1f_callback(void *ptr) {
  ce_4uwv1f_callback_type f = (ce_4uwv1f_callback_type)ptr;
  long double x = f(12, 0, 25, 0, 37);
  return (x == 74) ? 0 : -1;
}

static int ce_4uwv1f(void) {
  const char *src = 
  "long double f(long double a, int b, long double c, int d, long double e) {\n"
  "  return a + c + e;\n"
  "}\n";
  return ce_3e2f87(src, ce_4uwv1f_callback);
}

#define RUN_TEST(t) \
  if (!testname || (strcmp(#t, testname) == 0)) { \
    fputs(#t "... ", stdout); \
    fflush(stdout); \
    if (t() == 0) { \
      fputs("success\n", stdout); \
    } else { \
      fputs("failure\n", stdout); \
      retval = EXIT_FAILURE; \
    } \
  }

int main(int argc, char **argv) {
  int i;
  const char *testname = NULL;
  int retval = EXIT_SUCCESS;
  
  /* if tcclib.h and libtcc1.a are not installed, where can we find them */
  for (i = 1; i < argc; ++i) {
    if (!memcmp(argv[i], "run_test=", 9))
      testname = argv[i] + 9;
  }

  g_argv = argv, g_argc = argc;

  RUN_TEST(ret_int_test);
  RUN_TEST(ret_longlong_test);
  RUN_TEST(ret_float_test);
  RUN_TEST(ret_double_test);
  RUN_TEST(ret_longdouble_test);
  RUN_TEST(ce_mit95z);
  RUN_TEST(ce_8w42yg);
  RUN_TEST(ce_b5wuko);
  RUN_TEST(ce_jn50ok);
#if !defined __x86_64__ || defined _WIN32
  /* currently broken on x86_64 linux */
  RUN_TEST(ce_i4qi2o);
  RUN_TEST(ce_qd6d4k);
#endif
  RUN_TEST(ce_iref0a);
  RUN_TEST(ce_rqzvo7);
  RUN_TEST(ce_xd7jn2);
  RUN_TEST(ce_03kort);
  RUN_TEST(ce_t79u15);
  RUN_TEST(ce_eq1xra);
  RUN_TEST(ce_20zoff);
  RUN_TEST(ce_qak4io);
  RUN_TEST(ce_gefom3);
  RUN_TEST(ce_kiktfi);
  RUN_TEST(ce_do8exa);
  RUN_TEST(ce_l34coh);
  RUN_TEST(ce_4uwv1f);
  return retval;
}
