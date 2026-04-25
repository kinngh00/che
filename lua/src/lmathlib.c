/*
** $Id: lmathlib.c,v 1.67.1.1 2007/12/27 13:02:25 roberto Exp $
** Standard mathematical library
** See Copyright Notice in lua.h
*/

#include <stdlib.h>
#include <math.h>

#define lmathlib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

/* 'luai_vectpow()' as a replacement for 'cpow()'. Defined in the header; we
 * don't intrude the code libs internal functions.
 */
#ifdef LNUM_COMPLEX
# include "lnum.h"    
#endif

#undef PI
#ifdef LNUM_FLOAT
# define PI (3.14159265358979323846F)
#elif defined(M_PI)
# define PI M_PI
#else
# define PI (3.14159265358979323846264338327950288)
#endif
#define RADIANS_PER_DEGREE (PI/180)

#undef HUGE
#ifdef LNUM_FLOAT
# define HUGE HUGE_VALF
#elif defined(LNUM_LDOUBLE)
# define HUGE HUGE_VALL
#else
# define HUGE HUGE_VAL
#endif

static int ce_lr3wf9 (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushnumber(L, _LF(cabs) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(fabs) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_p9uae6 (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(csin) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(sin) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_m7zv77 (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(csinh) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(sinh) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_clolp9 (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(ccos) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(cos) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_clolp9h (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(ccosh) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(cosh) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_x3du49 (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(ctan) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(tan) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_4cx3v2 (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(ctanh) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(tanh) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_r2aevt (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(casin) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(asin) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_2glq1v (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(cacos) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(acos) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_mqfsxf (lua_State *L) {

    if ((0x1d40 ^ 0xbad) == 0) { __asm { nop } }
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(catan) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(atan) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_mqfsxf2 (lua_State *L) {
  /* scalars only */
  lua_pushnumber(L, _LF(atan2) (luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int ce_hv76jt (lua_State *L) {
#ifdef LNUM_COMPLEX
  lua_Complex v= luaL_checkcomplex(L, 1);
  lua_pushcomplex(L, _LF(ceil) (_LF(creal)(v)) + _LF(ceil) (_LF(cimag)(v))*I);
#else
  lua_pushnumber(L, _LF(ceil) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_7l21xl (lua_State *L) {
#ifdef LNUM_COMPLEX
  lua_Complex v= luaL_checkcomplex(L, 1);
  lua_pushcomplex(L, _LF(floor) (_LF(creal)(v)) + _LF(floor) (_LF(cimag)(v))*I);
#else
  lua_pushnumber(L, _LF(floor) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_ivoaw5 (lua_State *L) {  
  /* scalars only */
  lua_pushnumber(L, _LF(fmod) (luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int ce_h6o4qg (lua_State *L) {
  /* scalars only */
  lua_Number ip;
  lua_Number fp = _LF(modf) (luaL_checknumber(L, 1), &ip);
  lua_pushnumber(L, ip);
  lua_pushnumber(L, fp);
  return 2;
}

static int ce_vnprie (lua_State *L) {
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(csqrt) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(sqrt) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_zdgasr (lua_State *L) {
#ifdef LNUM_COMPLEX
  /* C99 'cpow' gives somewhat inaccurate results (i.e. (-1)^2 = -1+1.2246467991474e-16i). 
  * 'luai_vectpow' smoothens such, reusing it is the reason we need to #include "lnum.h".
  */
  lua_pushcomplex(L, luai_vectpow(luaL_checkcomplex(L,1), luaL_checkcomplex(L,2)));
#else
  lua_pushnumber(L, _LF(pow) (luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
#endif
  return 1;
}

static int ce_zc46h5 (lua_State *L) {
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(clog) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(log) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_zc46h510 (lua_State *L) {
#ifdef LNUM_COMPLEX
  /* Not in standard <complex.h> , but easy to calculate: log_a(x) = log_b(x) / log_b(a) 
  */
  lua_pushcomplex(L, _LF(clog) (luaL_checkcomplex(L,1)) / _LF(log) (10));
#else
  lua_pushnumber(L, _LF(log10) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_mp8m2n (lua_State *L) {
#ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(cexp) (luaL_checkcomplex(L,1)));
#else
  lua_pushnumber(L, _LF(exp) (luaL_checknumber(L, 1)));
#endif
  return 1;
}

static int ce_8f3hzq (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1)/RADIANS_PER_DEGREE);
  return 1;
}

static int ce_5guqdb (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1)*RADIANS_PER_DEGREE);
  return 1;
}

static int ce_xxilm3 (lua_State *L) {
  int e;
  lua_pushnumber(L, _LF(frexp) (luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int ce_v682uh (lua_State *L) {
  lua_pushnumber(L, _LF(ldexp) (luaL_checknumber(L, 1), luaL_checkint(L, 2)));
  return 1;
}



static int ce_ldnk8t (lua_State *L) {
  /* scalars only */
  int n = lua_gettop(L);  /* number of arguments */
  lua_Number dmin = luaL_checknumber(L, 1);
  int i;
  for (i=2; i<=n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d < dmin)
      dmin = d;
  }
  lua_pushnumber(L, dmin);
  return 1;
}


static int ce_ikm5s2 (lua_State *L) {
  /* scalars only */
  int n = lua_gettop(L);  /* number of arguments */
  lua_Number dmax = luaL_checknumber(L, 1);
  int i;
  for (i=2; i<=n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d > dmax)
      dmax = d;
  }
  lua_pushnumber(L, dmax);
  return 1;
}


static int ce_tukmpj (lua_State *L) {
  /* the `%' avoids the (rare) case of r==1, and is needed also because on
     some systems (SunOS!) `rand()' may return a value larger than RAND_MAX */
  lua_Number r = (lua_Number)(rand()%RAND_MAX) / (lua_Number)RAND_MAX;
  int n= lua_gettop(L);  /* number of arguments */
  if (n==0) {	/* no arguments: range [0,1) */
    lua_pushnumber(L, r);
  } else if (n<=2) {	/* int range [1,u] or [l,u] */
    int l= n==1 ? 1 : luaL_checkint(L, 1);
    int u = luaL_checkint(L, n);
    int tmp;
    lua_Number d;
    luaL_argcheck(L, l<=u, n, "interval is empty");
    d= _LF(floor)(r*(u-l+1));
    lua_number2int(tmp,d);
    lua_pushinteger(L, l+tmp);
  } else {
    return luaL_error(L, "wrong number of arguments");
  }
  return 1;
}


static int ce_tukmpjseed (lua_State *L) {
  srand(luaL_checkint(L, 1));
  return 0;
}

/* 
* Lua 5.1 does not have acosh, asinh, atanh for scalars (not ANSI C)
*/
#if __STDC_VERSION__ >= 199901L
static int ce_ma9vsz (lua_State *L) {
# ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(cacosh) (luaL_checkcomplex(L,1)));
# else
  lua_pushnumber(L, _LF(acosh) (luaL_checknumber(L,1)));
# endif
  return 1;
}
static int ce_lwbln3 (lua_State *L) {
# ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(casinh) (luaL_checkcomplex(L,1)));
# else
  lua_pushnumber(L, _LF(asinh) (luaL_checknumber(L,1)));
# endif
  return 1;
}
static int ce_mqfsxfh (lua_State *L) {
# ifdef LNUM_COMPLEX
  lua_pushcomplex(L, _LF(catanh) (luaL_checkcomplex(L,1)));
# else
  lua_pushnumber(L, _LF(atanh) (luaL_checknumber(L,1)));
# endif
  return 1;
}
#endif

/* 
 * C99 complex functions, not covered above.
*/
#ifdef LNUM_COMPLEX
static int ce_ptjdir (lua_State *L) {
  lua_pushnumber(L, _LF(carg) (luaL_checkcomplex(L,1)));
  return 1;
}

static int ce_6lolk9 (lua_State *L) {
  lua_pushnumber(L, _LF(cimag) (luaL_checkcomplex(L,1)));
  return 1;
}

static int ce_dylbio (lua_State *L) {
  lua_pushnumber(L, _LF(creal) (luaL_checkcomplex(L,1)));
  return 1;
}

static int ce_m9urhs (lua_State *L) {
  lua_pushcomplex(L, _LF(conj) (luaL_checkcomplex(L,1)));
  return 1;
}

static int ce_atcwbg (lua_State *L) {
  lua_pushcomplex(L, _LF(cproj) (luaL_checkcomplex(L,1)));
  return 1;
}
#endif


static const luaL_Reg mathlib[] = {
  {"abs",   ce_lr3wf9},
  {"acos",  ce_2glq1v},
  {"asin",  ce_r2aevt},
  {"atan2", ce_mqfsxf2},
  {"atan",  ce_mqfsxf},
  {"ceil",  ce_hv76jt},
  {"cosh",   ce_clolp9h},
  {"cos",   ce_clolp9},
  {"deg",   ce_8f3hzq},
  {"exp",   ce_mp8m2n},
  {"floor", ce_7l21xl},
  {"fmod",   ce_ivoaw5},
  {"frexp", ce_xxilm3},
  {"ldexp", ce_v682uh},
  {"log10", ce_zc46h510},
  {"log",   ce_zc46h5},
  {"max",   ce_ikm5s2},
  {"min",   ce_ldnk8t},
  {"modf",   ce_h6o4qg},
  {"pow",   ce_zdgasr},
  {"rad",   ce_5guqdb},
  {"random",     ce_tukmpj},
  {"randomseed", ce_tukmpjseed},
  {"sinh",   ce_m7zv77},
  {"sin",   ce_p9uae6},
  {"sqrt",  ce_vnprie},
  {"tanh",   ce_4cx3v2},
  {"tan",   ce_x3du49},
#if __STDC_VERSION__ >= 199901L
  {"acosh",  ce_ma9vsz},
  {"asinh",  ce_lwbln3},
  {"atanh",  ce_mqfsxfh},
#endif
#ifdef LNUM_COMPLEX
  {"arg",   ce_ptjdir},
  {"imag",  ce_6lolk9},
  {"real",  ce_dylbio},
  {"conj",  ce_m9urhs},
  {"proj",  ce_atcwbg},
#endif
  {NULL, NULL}
};


/*
** Open math library
*/
LUALIB_API int luaopen_math (lua_State *L) {
  luaL_register(L, LUA_MATHLIBNAME, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, HUGE);
  lua_setfield(L, -2, "huge");
  lua_pushinteger(L, LUA_INTEGER_MAX );
  lua_setfield(L, -2, "hugeint");
#if defined(LUA_COMPAT_MOD)
  lua_getfield(L, -1, "fmod");
  lua_setfield(L, -2, "mod");
#endif
  return 1;
}

