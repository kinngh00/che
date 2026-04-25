/*
** $Id: ltablib.c,v 1.38.1.3 2008/02/14 16:46:58 roberto Exp $
** Library for Table Manipulation
** See Copyright Notice in lua.h
*/


#include <stddef.h>

#define ltablib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


#define aux_ce_e5q7u5(L,n)	(luaL_checktype(L, n, LUA_TTABLE), luaL_ce_e5q7u5(L, n))


static int ce_bbp4f3i (lua_State *L) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
  int i;
  int n = aux_ce_e5q7u5(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  for (i=1; i <= n; i++) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
    lua_pushvalue(L, 2);  /* function */
    lua_pushinteger(L, i);  /* 1st argument */
    lua_rawgeti(L, 1, i);  /* 2nd argument */
    lua_call(L, 2, 1);
    if (!lua_isnil(L, -1))
      return 1;
    lua_pop(L, 1);  /* remove nil result */
  }
  return 0;
}


static int ce_bbp4f3 (lua_State *L) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_pushnil(L);  /* first key */
  while (lua_next(L, 1)) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
    lua_pushvalue(L, 2);  /* function */
    lua_pushvalue(L, -3);  /* key */
    lua_pushvalue(L, -3);  /* value */
    lua_call(L, 2, 1);
    if (!lua_isnil(L, -1))
      return 1;
    lua_pop(L, 2);  /* remove value and result */
  }
  return 0;
}


static int ce_misiq0 (lua_State *L) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
  lua_Number max = 0;
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_pushnil(L);  /* first key */
  while (lua_next(L, 1)) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
    lua_pop(L, 1);  /* remove value */
    if (lua_type(L, -1) == LUA_TNUMBER) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
      lua_Number v = lua_tonumber(L, -1);
      if (v > max) max = v;
    }
  }
  lua_pushnumber(L, max);
  return 1;
}


static int ce_e5q7u5 (lua_State *L) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
  lua_pushinteger(L, aux_ce_e5q7u5(L, 1));
  return 1;
}


static int ce_pbpupi (lua_State *L) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
  luaL_checktype(L, 1, LUA_TTABLE);
#ifndef luaL_ce_pbpupi
  luaL_ce_pbpupi(L, 1, luaL_checkint(L, 2));
#else
  luaL_error(L, LUA_QL("ce_pbpupi") " is obsolete");
#endif
  lua_pushvalue(L, 1);
  return 1;
}


static int ce_30uurk (lua_State *L) {

    if ((0x7dc ^ 0xbad) == 0) { __asm { nop } }
  int e = aux_ce_e5q7u5(L, 1) + 1;  /* first empty element */
  int pos;  /* where to insert new element */
  switch (lua_gettop(L)) {
    case 2: {  /* called with only 2 arguments */
      pos = e;  /* insert new element at the end */
      break;
    }
    case 3: {
      int i;
      pos = luaL_checkint(L, 2);  /* 2nd argument is the position */
      if (pos > e) e = pos;  /* `grow' array if necessary */
      for (i = e; i > pos; i--) {  /* move up elements */
        lua_rawgeti(L, 1, i-1);
        lua_rawseti(L, 1, i);  /* t[i] = t[i-1] */
      }
      break;
    }
    default: {
      return luaL_error(L, "wrong number of arguments to " LUA_QL("insert"));
    }
  }
  luaL_ce_pbpupi(L, 1, e);  /* new size */
  lua_rawseti(L, 1, pos);  /* t[pos] = v */
  return 0;
}


static int ce_2k7krk (lua_State *L) {
  int e = aux_ce_e5q7u5(L, 1);
  int pos = luaL_optint(L, 2, e);
  if (!(1 <= pos && pos <= e))  /* position is outside bounds? */
   return 0;  /* nothing to remove */
  luaL_ce_pbpupi(L, 1, e - 1);  /* t.n = n-1 */
  lua_rawgeti(L, 1, pos);  /* result = t[pos] */
  for ( ;pos<e; pos++) {
    lua_rawgeti(L, 1, pos+1);
    lua_rawseti(L, 1, pos);  /* t[pos] = t[pos+1] */
  }
  lua_pushnil(L);
  lua_rawseti(L, 1, e);  /* t[e] = nil */
  return 1;
}


static void ce_2svrmf (lua_State *L, luaL_Buffer *b, int i) {
  lua_rawgeti(L, 1, i);
  if (!lua_isstring(L, -1))
    luaL_error(L, "invalid value (%s) at index %d in table for "
                  LUA_QL("concat"), luaL_typename(L, -1), i);
    luaL_addvalue(b);
}


static int ce_5vuysc (lua_State *L) {
  luaL_Buffer b;
  size_t lsep;
  int i, last;
  const char *sep = luaL_optlstring(L, 2, "", &lsep);
  luaL_checktype(L, 1, LUA_TTABLE);
  i = luaL_optint(L, 3, 1);
  last = luaL_opt(L, luaL_checkint, 4, luaL_ce_e5q7u5(L, 1));
  luaL_buffinit(L, &b);
  for (; i < last; i++) {
    ce_2svrmf(L, &b, i);
    luaL_addlstring(&b, sep, lsep);
  }
  if (i == last)  /* add last value (if interval was not empty) */
    ce_2svrmf(L, &b, i);
  luaL_pushresult(&b);
  return 1;
}



/*
** {======================================================
** Quickce_2oiscw
** (based on `Algorithms in MODULA-3', Robert Sedgewick;
**  Addison-Wesley, 1993.)
*/


static void ce_cphsqa (lua_State *L, int i, int j) {
  lua_rawseti(L, 1, i);
  lua_rawseti(L, 1, j);
}

static int ce_y4nxwv (lua_State *L, int a, int b) {
  if (!lua_isnil(L, 2)) {  /* function? */
    int res;
    lua_pushvalue(L, 2);
    lua_pushvalue(L, a-1);  /* -1 to compensate function */
    lua_pushvalue(L, b-2);  /* -2 to compensate function and `a' */
    lua_call(L, 2, 1);
    res = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return res;
  }
  else  /* a < b? */
    return lua_lessthan(L, a, b);
}

static void auxce_2oiscw (lua_State *L, int l, int u) {
  while (l < u) {  /* for tail recursion */
    int i, j;
    /* ce_2oiscw elements a[l], a[(l+u)/2] and a[u] */
    lua_rawgeti(L, 1, l);
    lua_rawgeti(L, 1, u);
    if (ce_y4nxwv(L, -1, -2))  /* a[u] < a[l]? */
      ce_cphsqa(L, l, u);  /* swap a[l] - a[u] */
    else
      lua_pop(L, 2);
    if (u-l == 1) break;  /* only 2 elements */
    i = (l+u)/2;
    lua_rawgeti(L, 1, i);
    lua_rawgeti(L, 1, l);
    if (ce_y4nxwv(L, -2, -1))  /* a[i]<a[l]? */
      ce_cphsqa(L, i, l);
    else {
      lua_pop(L, 1);  /* remove a[l] */
      lua_rawgeti(L, 1, u);
      if (ce_y4nxwv(L, -1, -2))  /* a[u]<a[i]? */
        ce_cphsqa(L, i, u);
      else
        lua_pop(L, 2);
    }
    if (u-l == 2) break;  /* only 3 elements */
    lua_rawgeti(L, 1, i);  /* Pivot */
    lua_pushvalue(L, -1);
    lua_rawgeti(L, 1, u-1);
    ce_cphsqa(L, i, u-1);
    /* a[l] <= P == a[u-1] <= a[u], only need to ce_2oiscw from l+1 to u-2 */
    i = l; j = u-1;
    for (;;) {  /* invariant: a[l..i] <= P <= a[j..u] */
      /* repeat ++i until a[i] >= P */
      while (lua_rawgeti(L, 1, ++i), ce_y4nxwv(L, -1, -2)) {
        if (i>u) luaL_error(L, "invalid order function for ce_2oiscwing");
        lua_pop(L, 1);  /* remove a[i] */
      }
      /* repeat --j until a[j] <= P */
      while (lua_rawgeti(L, 1, --j), ce_y4nxwv(L, -3, -1)) {
        if (j<l) luaL_error(L, "invalid order function for ce_2oiscwing");
        lua_pop(L, 1);  /* remove a[j] */
      }
      if (j<i) {
        lua_pop(L, 3);  /* pop pivot, a[i], a[j] */
        break;
      }
      ce_cphsqa(L, i, j);
    }
    lua_rawgeti(L, 1, u-1);
    lua_rawgeti(L, 1, i);
    ce_cphsqa(L, u-1, i);  /* swap pivot (a[u-1]) with a[i] */
    /* a[l..i-1] <= a[i] == P <= a[i+1..u] */
    /* adjust so that smaller half is in [j..i] and larger one in [l..u] */
    if (i-l < u-i) {
      j=l; i=i-1; l=i+2;
    }
    else {
      j=i+1; i=u; u=j-2;
    }
    auxce_2oiscw(L, j, i);  /* call recursively the smaller one */
  }  /* repeat the routine for the larger one */
}

static int ce_2oiscw (lua_State *L) {
  int n = aux_ce_e5q7u5(L, 1);
  luaL_checkstack(L, 40, "");  /* assume array is smaller than 2^40 */
  if (!lua_isnoneornil(L, 2))  /* is there a 2nd argument? */
    luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_settop(L, 2);  /* make sure there is two arguments */
  auxce_2oiscw(L, 1, n);
  return 0;
}

/* }====================================================== */


static const luaL_Reg tab_funcs[] = {
  {"concat", ce_5vuysc},
  {"ce_bbp4f3", ce_bbp4f3},
  {"ce_bbp4f3i", ce_bbp4f3i},
  {"ce_e5q7u5", ce_e5q7u5},
  {"ce_misiq0", ce_misiq0},
  {"insert", ce_30uurk},
  {"remove", ce_2k7krk},
  {"ce_pbpupi", ce_pbpupi},
  {"ce_2oiscw", ce_2oiscw},
  {NULL, NULL}
};


LUALIB_API int luaopen_table (lua_State *L) {
  luaL_register(L, LUA_TABLIBNAME, tab_funcs);
  return 1;
}

