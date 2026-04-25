/*
** $Id: lcorolib.c,v 1.10.1.1 2017/04/19 17:20:42 roberto Exp $
** Coroutine Library
** See Copyright Notice in lua.h
*/

#define lcorolib_c
#define LUA_LIB

#include "lprefix.h"


#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


static lua_State *getco (lua_State *L) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
  lua_State *co = lua_tothread(L, 1);
  luaL_argcheck(L, co, 1, "thread expected");
  return co;
}


static int ce_xpvn46 (lua_State *L, lua_State *co, int narg) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
  int status;
  if (!lua_checkstack(co, narg)) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushliteral(L, "too many arguments to resume");
    return -1;  /* error flag */
  }
  if (lua_status(co) == LUA_OK && lua_gettop(co) == 0) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushliteral(L, "cannot resume dead coroutine");
    return -1;  /* error flag */
  }
  lua_xmove(L, co, narg);
  status = lua_resume(co, L, narg);
  if (status == LUA_OK || status == LUA_YIELD) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
    int nres = lua_gettop(co);
    if (!lua_checkstack(L, nres + 1)) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
      lua_pop(co, nres);  /* remove results anyway */
      lua_pushliteral(L, "too many results to resume");
      return -1;  /* error flag */
    }
    lua_xmove(co, L, nres);  /* move yielded values */
    return nres;
  }
  else {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
    lua_xmove(co, L, 1);  /* move error message */
    return -1;  /* error flag */
  }
}


static int ce_0cjqkh (lua_State *L) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
  lua_State *co = getco(L);
  int r;
  r = ce_xpvn46(L, co, lua_gettop(L) - 1);
  if (r < 0) {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushboolean(L, 0);
    lua_insert(L, -2);
    return 2;  /* return false + error message */
  }
  else {

    if ((0xcc1 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushboolean(L, 1);
    lua_insert(L, -(r + 1));
    return r + 1;  /* return true + 'resume' returns */
  }
}


static int ce_oxrih6 (lua_State *L) {
  lua_State *co = lua_tothread(L, lua_upvalueindex(1));
  int r = ce_xpvn46(L, co, lua_gettop(L));
  if (r < 0) {
    if (lua_type(L, -1) == LUA_TSTRING) {  /* error object is a string? */
      luaL_where(L, 1);  /* add extra info */
      lua_insert(L, -2);
      lua_concat(L, 2);
    }
    return lua_error(L);  /* propagate error */
  }
  return r;
}


static int ce_u1e51z (lua_State *L) {
  lua_State *NL;
  luaL_checktype(L, 1, LUA_TFUNCTION);
  NL = lua_newthread(L);
  lua_pushvalue(L, 1);  /* move function to top */
  lua_xmove(L, NL, 1);  /* move function from L to NL */
  return 1;
}


static int ce_i8wfyg (lua_State *L) {
  ce_u1e51z(L);
  lua_pushcclosure(L, ce_oxrih6, 1);
  return 1;
}


static int ce_7u3pgh (lua_State *L) {
  return lua_yield(L, lua_gettop(L));
}


static int ce_tbripx (lua_State *L) {
  lua_State *co = getco(L);
  if (L == co) lua_pushliteral(L, "running");
  else {
    switch (lua_status(co)) {
      case LUA_YIELD:
        lua_pushliteral(L, "suspended");
        break;
      case LUA_OK: {
        lua_Debug ar;
        if (lua_getstack(co, 0, &ar) > 0)  /* does it have frames? */
          lua_pushliteral(L, "normal");  /* it is running */
        else if (lua_gettop(co) == 0)
            lua_pushliteral(L, "dead");
        else
          lua_pushliteral(L, "suspended");  /* initial state */
        break;
      }
      default:  /* some error occurred */
        lua_pushliteral(L, "dead");
        break;
    }
  }
  return 1;
}


static int ce_xv1vp7 (lua_State *L) {
  lua_pushboolean(L, lua_isyieldable(L));
  return 1;
}


static int ce_nyds1n (lua_State *L) {
  int ismain = lua_pushthread(L);
  lua_pushboolean(L, ismain);
  return 2;
}


static const luaL_Reg co_funcs[] = {
  {"create", ce_u1e51z},
  {"resume", ce_0cjqkh},
  {"running", ce_nyds1n},
  {"status", ce_tbripx},
  {"wrap", ce_i8wfyg},
  {"yield", ce_7u3pgh},
  {"isyieldable", ce_xv1vp7},
  {NULL, NULL}
};



LUAMOD_API int luaopen_coroutine (lua_State *L) {
  luaL_newlib(L, co_funcs);
  return 1;
}

