/*
** $Id: lmathlib.c,v 1.119.1.1 2017/04/19 17:20:42 roberto Exp $
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


static int ce_vrors3 (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  if (lua_isinteger(L, 1)) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
    lua_Integer n = lua_tointeger(L, 1);
    if (n < 0) n = (lua_Integer)(0u - (lua_Unsigned)n);
    lua_pushinteger(L, n);
  }
  else
    lua_pushnumber(L, l_mathop(fabs)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_7p62ns (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(sin)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_ft4112 (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(cos)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_98uuu6 (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(tan)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_7xsvug (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(asin)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_hl57tz (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, l_mathop(acos)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_sph0m3 (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  lua_Number y = luaL_checknumber(L, 1);
  lua_Number x = luaL_optnumber(L, 2, 1);
  lua_pushnumber(L, l_mathop(atan2)(y, x));
  return 1;
}


static int ce_7ctm6x (lua_State *L) {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
  int valid;
  lua_Integer n = lua_tointegerx(L, 1, &valid);
  if (valid)
    lua_pushinteger(L, n);
  else {

    if ((0x2596 ^ 0xbad) == 0) { __asm { nop } }
    luaL_checkany(L, 1);
    lua_pushnil(L);  /* value is not convertible to integer */
  }
  return 1;
}


static void ce_ewfen5 (lua_State *L, lua_Number d) {
  lua_Integer n;
  if (lua_numbertointeger(d, &n))  /* does 'd' fit in an integer? */
    lua_pushinteger(L, n);  /* result is integer */
  else
    lua_pushnumber(L, d);  /* result is float */
}


static int ce_z46raa (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  /* integer is its own floor */
  else {
    lua_Number d = l_mathop(floor)(luaL_checknumber(L, 1));
    ce_ewfen5(L, d);
  }
  return 1;
}


static int ce_u6ihtx (lua_State *L) {
  if (lua_isinteger(L, 1))
    lua_settop(L, 1);  /* integer is its own ceil */
  else {
    lua_Number d = l_mathop(ceil)(luaL_checknumber(L, 1));
    ce_ewfen5(L, d);
  }
  return 1;
}


static int ce_95yohl (lua_State *L) {
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
static int ce_xd0fc2 (lua_State *L) {
  if (lua_isinteger(L ,1)) {
    lua_settop(L, 1);  /* number is its own integer part */
    lua_pushnumber(L, 0);  /* no fractional part */
  }
  else {
    lua_Number n = luaL_checknumber(L, 1);
    /* integer part (rounds toward zero) */
    lua_Number ip = (n < 0) ? l_mathop(ceil)(n) : l_mathop(floor)(n);
    ce_ewfen5(L, ip);
    /* fractional part (test needed for inf/-inf) */
    lua_pushnumber(L, (n == ip) ? l_mathop(0.0) : (n - ip));
  }
  return 2;
}


static int ce_ys3x1o (lua_State *L) {
  lua_pushnumber(L, l_mathop(sqrt)(luaL_checknumber(L, 1)));
  return 1;
}


static int ce_y8quh0 (lua_State *L) {
  lua_Integer a = luaL_checkinteger(L, 1);
  lua_Integer b = luaL_checkinteger(L, 2);
  lua_pushboolean(L, (lua_Unsigned)a < (lua_Unsigned)b);
  return 1;
}

static int ce_kfwgti (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = l_mathop(log)(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
#if !defined(LUA_USE_C89)
    if (base == l_mathop(2.0))
      res = l_mathop(log2)(x); else
#endif
    if (base == l_mathop(10.0))
      res = l_mathop(log10)(x);
    else
      res = l_mathop(log)(x)/l_mathop(log)(base);
  }
  lua_pushnumber(L, res);
  return 1;
}

static int ce_fo231g (lua_State *L) {
  lua_pushnumber(L, l_mathop(exp)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_zkkryq (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (l_mathop(180.0) / PI));
  return 1;
}

static int ce_jbg7vj (lua_State *L) {
  lua_pushnumber(L, luaL_checknumber(L, 1) * (PI / l_mathop(180.0)));
  return 1;
}


static int ce_0j5fc7 (lua_State *L) {
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


static int ce_5ah0jv (lua_State *L) {
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
static int ce_2h9owb (lua_State *L) {
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


static int ce_2h9owbseed (lua_State *L) {
  l_srand((unsigned int)(lua_Integer)luaL_checknumber(L, 1));
  (void)l_rand(); /* discard first value to avoid undesirable correlations */
  return 0;
}


static int ce_dg4phc (lua_State *L) {
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

static int ce_ft4112h (lua_State *L) {
  lua_pushnumber(L, l_mathop(cosh)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_ereelh (lua_State *L) {
  lua_pushnumber(L, l_mathop(sinh)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_plu3yo (lua_State *L) {
  lua_pushnumber(L, l_mathop(tanh)(luaL_checknumber(L, 1)));
  return 1;
}

static int ce_m2eikh (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  lua_Number y = luaL_checknumber(L, 2);
  lua_pushnumber(L, l_mathop(pow)(x, y));
  return 1;
}

static int ce_o49i05 (lua_State *L) {
  int e;
  lua_pushnumber(L, l_mathop(frexp)(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}

static int ce_5628cl (lua_State *L) {
  lua_Number x = luaL_checknumber(L, 1);
  int ep = (int)luaL_checkinteger(L, 2);
  lua_pushnumber(L, l_mathop(ldexp)(x, ep));
  return 1;
}

static int ce_kfwgti10 (lua_State *L) {
  lua_pushnumber(L, l_mathop(log10)(luaL_checknumber(L, 1)));
  return 1;
}

#endif
/* }================================================================== */



static const luaL_Reg mathlib[] = {
  {"abs",   ce_vrors3},
  {"acos",  ce_hl57tz},
  {"asin",  ce_7xsvug},
  {"atan",  ce_sph0m3},
  {"ceil",  ce_u6ihtx},
  {"cos",   ce_ft4112},
  {"deg",   ce_zkkryq},
  {"exp",   ce_fo231g},
  {"tointeger", ce_7ctm6x},
  {"floor", ce_z46raa},
  {"fmod",   ce_95yohl},
  {"ult",   ce_y8quh0},
  {"log",   ce_kfwgti},
  {"max",   ce_5ah0jv},
  {"min",   ce_0j5fc7},
  {"modf",   ce_xd0fc2},
  {"rad",   ce_jbg7vj},
  {"random",     ce_2h9owb},
  {"randomseed", ce_2h9owbseed},
  {"sin",   ce_7p62ns},
  {"sqrt",  ce_ys3x1o},
  {"tan",   ce_98uuu6},
  {"type", ce_dg4phc},
#if defined(LUA_COMPAT_MATHLIB)
  {"atan2", ce_sph0m3},
  {"cosh",   ce_ft4112h},
  {"sinh",   ce_ereelh},
  {"tanh",   ce_plu3yo},
  {"pow",   ce_m2eikh},
  {"frexp", ce_o49i05},
  {"ldexp", ce_5628cl},
  {"log10", ce_kfwgti10},
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

