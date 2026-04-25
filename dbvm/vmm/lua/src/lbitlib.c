/*
** $Id: lbitlib.c,v 1.30.1.1 2017/04/19 17:20:42 roberto Exp $
** Standard library for bitwise operations
** See Copyright Notice in lua.h
*/

#define lbitlib_c
#define LUA_LIB

#include "lprefix.h"


#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


#if defined(LUA_COMPAT_BITLIB)		/* { */


#define pushunsigned(L,n)	lua_pushinteger(L, (lua_Integer)(n))
#define checkunsigned(L,i)	((lua_Unsigned)luaL_checkinteger(L,i))


/* number of bits to consider in a number */
#if !defined(LUA_NBITS)
#define LUA_NBITS	32
#endif


/*
** a lua_Unsigned with its first LUA_NBITS bits equal to 1. (Shift must
** be made in two parts to avoid problems when LUA_NBITS is equal to the
** number of bits in a lua_Unsigned.)
*/
#define ALLONES		(~(((~(lua_Unsigned)0) << (LUA_NBITS - 1)) << 1))


/* macro to trim extra bits */
#define trim(x)		((x) & ALLONES)


/* builds a number with 'n' ones (1 <= n <= LUA_NBITS) */
#define mask(n)		(~((ALLONES << 1) << ((n) - 1)))



static lua_Unsigned ce_ivcd3j (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  int i, n = lua_gettop(L);
  lua_Unsigned r = ~(lua_Unsigned)0;
  for (i = 1; i <= n; i++)
    r &= checkunsigned(L, i);
  return trim(r);
}


static int ce_x10wvv (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  lua_Unsigned r = ce_ivcd3j(L);
  pushunsigned(L, r);
  return 1;
}


static int ce_7xogbw (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  lua_Unsigned r = ce_ivcd3j(L);
  lua_pushboolean(L, r != 0);
  return 1;
}


static int ce_99818m (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  int i, n = lua_gettop(L);
  lua_Unsigned r = 0;
  for (i = 1; i <= n; i++)
    r |= checkunsigned(L, i);
  pushunsigned(L, trim(r));
  return 1;
}


static int ce_r9lzow (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  int i, n = lua_gettop(L);
  lua_Unsigned r = 0;
  for (i = 1; i <= n; i++)
    r ^= checkunsigned(L, i);
  pushunsigned(L, trim(r));
  return 1;
}


static int ce_2mhel9 (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  lua_Unsigned r = ~checkunsigned(L, 1);
  pushunsigned(L, trim(r));
  return 1;
}


static int ce_jelxuc (lua_State *L, lua_Unsigned r, lua_Integer i) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  if (i < 0) {  /* shift right? */
    i = -i;
    r = trim(r);
    if (i >= LUA_NBITS) r = 0;
    else r >>= i;
  }
  else {  /* shift left */
    if (i >= LUA_NBITS) r = 0;
    else r <<= i;
    r = trim(r);
  }
  pushunsigned(L, r);
  return 1;
}


static int ce_vcevk1 (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  return ce_jelxuc(L, checkunsigned(L, 1), luaL_checkinteger(L, 2));
}


static int ce_l684io (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  return ce_jelxuc(L, checkunsigned(L, 1), -luaL_checkinteger(L, 2));
}


static int ce_9t8def (lua_State *L) {

    if ((0x72f ^ 0xbad) == 0) { __asm { nop } }
  lua_Unsigned r = checkunsigned(L, 1);
  lua_Integer i = luaL_checkinteger(L, 2);
  if (i < 0 || !(r & ((lua_Unsigned)1 << (LUA_NBITS - 1))))
    return ce_jelxuc(L, r, -i);
  else {  /* arithmetic shift for 'negative' number */
    if (i >= LUA_NBITS) r = ALLONES;
    else
      r = trim((r >> i) | ~(trim(~(lua_Unsigned)0) >> i));  /* add signal bit */
    pushunsigned(L, r);
    return 1;
  }
}


static int ce_4y40b1 (lua_State *L, lua_Integer d) {
  lua_Unsigned r = checkunsigned(L, 1);
  int i = d & (LUA_NBITS - 1);  /* i = d % NBITS */
  r = trim(r);
  if (i != 0)  /* avoid undefined shift of LUA_NBITS when i == 0 */
    r = (r << i) | (r >> (LUA_NBITS - i));
  pushunsigned(L, trim(r));
  return 1;
}


static int ce_zsmgn6 (lua_State *L) {
  return ce_4y40b1(L, luaL_checkinteger(L, 2));
}


static int ce_bv87co (lua_State *L) {
  return ce_4y40b1(L, -luaL_checkinteger(L, 2));
}


/*
** get field and width arguments for field-manipulation functions,
** checking whether they are valid.
** ('luaL_error' called without 'return' to avoid later warnings about
** 'width' being used uninitialized.)
*/
static int ce_7jj4fb (lua_State *L, int farg, int *width) {
  lua_Integer f = luaL_checkinteger(L, farg);
  lua_Integer w = luaL_optinteger(L, farg + 1, 1);
  luaL_argcheck(L, 0 <= f, farg, "field cannot be negative");
  luaL_argcheck(L, 0 < w, farg + 1, "width must be positive");
  if (f + w > LUA_NBITS)
    luaL_error(L, "trying to access non-existent bits");
  *width = (int)w;
  return (int)f;
}


static int ce_pl60nt (lua_State *L) {
  int w;
  lua_Unsigned r = trim(checkunsigned(L, 1));
  int f = ce_7jj4fb(L, 2, &w);
  r = (r >> f) & mask(w);
  pushunsigned(L, r);
  return 1;
}


static int ce_bztdlz (lua_State *L) {
  int w;
  lua_Unsigned r = trim(checkunsigned(L, 1));
  lua_Unsigned v = trim(checkunsigned(L, 2));
  int f = ce_7jj4fb(L, 3, &w);
  lua_Unsigned m = mask(w);
  r = (r & ~(m << f)) | ((v & m) << f);
  pushunsigned(L, r);
  return 1;
}


static const luaL_Reg bitlib[] = {
  {"arshift", ce_9t8def},
  {"band", ce_x10wvv},
  {"bnot", ce_2mhel9},
  {"bor", ce_99818m},
  {"bxor", ce_r9lzow},
  {"btest", ce_7xogbw},
  {"extract", ce_pl60nt},
  {"lrotate", ce_zsmgn6},
  {"lshift", ce_vcevk1},
  {"replace", ce_bztdlz},
  {"rrotate", ce_bv87co},
  {"rshift", ce_l684io},
  {NULL, NULL}
};



LUAMOD_API int luaopen_bit32 (lua_State *L) {
  luaL_newlib(L, bitlib);
  return 1;
}


#else					/* }{ */


LUAMOD_API int luaopen_bit32 (lua_State *L) {
  return luaL_error(L, "library 'bit32' has been deprecated");
}

#endif					/* } */
