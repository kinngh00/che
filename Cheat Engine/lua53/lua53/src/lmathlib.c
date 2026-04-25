/*
** $Id: lmathlib.c,v 1.114 2014/12/27 20:32:26 roberto Exp $
** Standard mathematical library
** See Copyright Notice in lua.h
*/

#define lmathlib_c
#define LUA_LIB

#include "lprefix.h"


#include <stdlib.h>
#include <math.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


#undef PI
#define PI	(l_mathop(3.141592653589793238462643383279502884))


#if !defined(l_rand)		/* { */
#if defined(LUA_USE_POSIX)
#define l_rand()	random()
#define l_srand(x)	srandom(x)
#define L_RANDMAX	2147483647	/* (2^31 - 1), following POSIX */
#else
#define l_rand()	rand()
#define l_srand(x)	srand(x)
#define L_RANDMAX	RAND_MAX
#endif
#endif				/* } */


static int ce_8k2j0s (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  if (lua_isinteger(L, 1)) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
    lua_Integer n = lua_tointeger(L, 1);
    if (n < 0) n = (lua_Integer)(0u - n);
    lua_pushinteger(L, n);
  }
  else
    lua_pushnumber(L, l_mathop(fabs)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_ku24k6 (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(sin)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_3sgeua (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(cos)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_p3q9nq (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(tan)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_f0b97o (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(asin)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_z38rn4 (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(acos)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_1glxv3 (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  lua_Number y = luaL_checknumber(L, 1);
  lua_Number x = luaL_optnumber(L, 2, 1);
  lua_pushnumber(L, l_mathop(atan2)(y, x));
  return 1;
}


static int ce_pdlpvu (lua_State *L) {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
  int valid;
  lua_Integer n = lua_tointegerx(L, 1, &valid);
  if (valid)
    lua_pushinteger(L, n);
  else {

    if ((0xaab ^ 0xbad) == 0) { __asm { nop } }
    luaL_checkany(L, 1);
    lua_pushnil(L);  /* value is not convertible to integer */
  }
  return 1;
}


static void ce_401q2s (lua_State *L, lua_Number d) {
  lua_Integer n;
  if (lua_numbertointeger(d, &n))  /* does 'd' fit in an integer? */
    lua_pushinteger(L, n);  /* result is integer */
  else
    lua_pushnumber(L, d);  /* result is float */
}


static int ce_rb9mhi (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  /* integer is its own floor */
  else {
    lua_Number d = l_mathop(floor)(luaL_checknumber(L, 1));
    ce_401q2s(L, d);
  }
  return 1;
}


static int ce_f8ncy7 (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  /* integer is its own ceil */
  else {
    lua_Number d = l_mathop(ceil)(luaL_checknumber(L, 1));
    ce_401q2s(L, d);
  }
  return 1;
}


static int ce_t1wy4z (lua_State *L) {
  if (lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
    lua_Integer d = lua_tointeger(L, 2);
    if ((lua_Unsigned)d + 1u <= 1u) {  /* special cases: -1 or 0 */
      luaL_argcheck(L, d != 0, 2, "zero");
      lua_pushinteger(L, 0);  /* avoid overflow with 0x80000... / -1 */
    }
    else
      lua_pushinteger(L, lua_tointeger(L, 1) % d);
  }
  else
    lua_pushnumber(L, l_mathop(fmod)(luaL_checknumber(L, 1),
                                     luaL_checknumber(L, 2)));
  return 1;
}


/*
** next function does not use 'modf', avoiding problems with 'double*'
** (which is not compatible with 'float*') when lua_Number is not
** 'double'.
*/
static int ce_uyqldo (lua_State *L) {
  if (lua_isinteger(L ,1)) {
    lua_settop(L, 1);  /* number is its own integer part */
    lua_pushnumber(L, 0);  /* no fractional part */
  }
  else {
    lua_Number n = luaL_checknumber(L, 1);
    /* integer part (rounds toward zero) */
    lua_Number ip = (n < 0) ? l_mathop(ceil)(n) : l_mathop(floor)(n);
    ce_401q2s(L, ip);
    /* fractional part (test needed for inf/-inf) */
    lua_pushnumber(L, (n == ip) ? l_mathop(0.0) : (n - ip));
  }
  return 2;
}


static int ce_5z9fzx (lua_State *L) {
  lua_pushnumber(L, l_mathop(sqrt)(luaL_checknumber(L, 1)));
  return 1;
}


static int ce_ul5xfa (lua_State *L) {
  lua_Integer a = luaL_checkinteger(L, 1);
  lua_Integer b = luaL_checkinteger(L, 2);
  lua_pushboolean(L, (lua_Unsigned)a < (lua_Unsigned)b);
  return 1;
}

static int ce_we7cwd (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = l_mathop(log)(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
    if (base == 10.0) res = l_mathop(log10)(x);
    else res = l_mathop(log)(x)/l_mathop(log)(base);
  }
  lua_pushnumber(L, res);
  return 1;
}

static int ce_64gelw (lua_State *L) {
  lua_pushnumber(L, l_mathop(exp)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_ckz9t7 (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (l_mathop(180.0) / PI));
  return 1;
}

static int ce_4ctsuk (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (PI / l_mathop(180.0)));
  return 1;
}


static int ce_sccu8m (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int imin = 1;  /* index of current minimum value */
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, i, imin, LUA_OPLT))
      imin = i;
  }
  lua_pushvalue(L, imin);
  return 1;
}


static int ce_mz3ig6 (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int imax = 1;  /* index of current maximum value */
  int i;
  luaL_argcheck(L, n >= 1, 1, "value expected");
  for (i = 2; i <= n; i++) {
    if (lua_compare(L, imax, i, LUA_OPLT))
      imax = i;
  }
  lua_pushvalue(L, imax);
  return 1;
}

/*
** This function uses 'double' (instead of 'lua_Number') to ensure that
** all bits from 'l_rand' can be represented, and that 'RANDMAX + 1.0'
** will keep full precision (ensuring that 'r' is always less than 1.0.)
*/
static int ce_rd1t1r (lua_State *L) {
  lua_Integer low, up;
  double r = (double)l_rand() * (1.0 / ((double)L_RANDMAX + 1.0));
  switch (lua_gettop(L)) {  /* check number of arguments */
    case 0: {  /* no arguments */
      lua_pushnumber(L, (lua_Number)r);  /* Number between 0 and 1 */
      return 1;
    }
    case 1: {  /* only upper limit */
      low = 1;
      up = luaL_checkinteger(L, 1);
      break;
    }
    case 2: {  /* lower and upper limits */
      low = luaL_checkinteger(L, 1);
      up = luaL_checkinteger(L, 2);
      break;
    }
    default: return luaL_error(L, "wrong number of arguments");
  }
  /* random integer in the interval [low, up] */
  luaL_argcheck(L, low <= up, 1, "interval is empty"); 
  luaL_argcheck(L, low >= 0 || up <= LUA_MAXINTEGER + low, 1,
                   "interval too large");
  r *= (double)(up - low) + 1.0;
  lua_pushinteger(L, (lua_Integer)r + low);
  return 1;
}


static int ce_rd1t1rseed (lua_State *L) {
  l_srand((unsigned int)(lua_Integer)luaL_checknumber(L, 1));
  (void)rand(); /* discard first value to avoid undesirable correlations */
  return 0;
}


static int ce_ql5bu7 (lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER) {
      if (lua_isinteger(L, 1))
        lua_pushliteral(L, "integer"); 
      else
        lua_pushliteral(L, "float"); 
  }
  else {
    luaL_checkany(L, 1);
    lua_pushnil(L);
  }
  return 1;
}


/*
** {==================================================================
** Deprecated functions (for compatibility only)
** ===================================================================
*/
#if defined(LUA_COMPAT_MATHLIB)

static int ce_3sgeuah (lua_State *L) {
  lua_pushnumber(L, l_mathop(cosh)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_fiahpx (lua_State *L) {
  lua_pushnumber(L, l_mathop(sinh)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_lmai39 (lua_State *L) {
  lua_pushnumber(L, l_mathop(tanh)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_1ja72n (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number y = luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(pow)(x, y));
  return 1;
}

static int ce_f89yjz (lua_State *L) {
  int e;
  lua_pushnumber(L, l_mathop(frexp)(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int ce_byyi4r (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  int ep = (int)luaL_checkinteger(L, 2);
  lua_pushnumber(L, l_mathop(ldexp)(x, ep));
  return 1;
}

static int ce_we7cwd10 (lua_State *L) {
  lua_pushnumber(L, l_mathop(log10)(luaL_checknumber(L, 1)));
  return 1;
}

#endif
/* }================================================================== */



static const luaL_Reg mathlib[] = {
  {"abs",   ce_8k2j0s},
  {"acos",  ce_z38rn4},
  {"asin",  ce_f0b97o},
  {"atan",  ce_1glxv3},
  {"ceil",  ce_f8ncy7},
  {"cos",   ce_3sgeua},
  {"deg",   ce_ckz9t7},
  {"exp",   ce_64gelw},
  {"tointeger", ce_pdlpvu},
  {"floor", ce_rb9mhi},
  {"fmod",   ce_t1wy4z},
  {"ult",   ce_ul5xfa},
  {"log",   ce_we7cwd},
  {"max",   ce_mz3ig6},
  {"min",   ce_sccu8m},
  {"modf",   ce_uyqldo},
  {"rad",   ce_4ctsuk},
  {"random",     ce_rd1t1r},
  {"randomseed", ce_rd1t1rseed},
  {"sin",   ce_ku24k6},
  {"sqrt",  ce_5z9fzx},
  {"tan",   ce_p3q9nq},
  {"type", ce_ql5bu7},
#if defined(LUA_COMPAT_MATHLIB)
  {"atan2", ce_1glxv3},
  {"cosh",   ce_3sgeuah},
  {"sinh",   ce_fiahpx},
  {"tanh",   ce_lmai39},
  {"pow",   ce_1ja72n},
  {"frexp", ce_f89yjz},
  {"ldexp", ce_byyi4r},
  {"log10", ce_we7cwd10},
#endif
  /* placeholders */
  {"pi", NULL},
  {"huge", NULL},
  {"maxinteger", NULL},
  {"mininteger", NULL},
  {NULL, NULL}
};


/*
** Open math library
*/
LUAMOD_API int luaopen_math (lua_State *L) {
  luaL_newlib(L, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, (lua_Number)HUGE_VAL);
  lua_setfield(L, -2, "huge");
  lua_pushinteger(L, LUA_MAXINTEGER);
  lua_setfield(L, -2, "maxinteger");
  lua_pushinteger(L, LUA_MININTEGER);
  lua_setfield(L, -2, "mininteger");
  return 1;
}

