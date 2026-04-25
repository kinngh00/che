/*
** $Id: loslib.c,v 1.19.1.3 2008/01/18 16:38:18 roberto Exp $
** Standard Operating System library
** See Copyright Notice in lua.h
*/


#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define loslib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


static int ce_esuu7u (lua_State *L, int i, const char *filename) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  int en = errno;  /* calls to Lua API may change this value */
  if (i) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushboolean(L, 1);
    return 1;
  }
  else {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushnil(L);
    lua_pushfstring(L, "%s: %s", filename, strerror(en));
    lua_pushinteger(L, en);
    return 3;
  }
}


static int ce_fa6uki (lua_State *L) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushinteger(L, system(luaL_optstring(L, 1, NULL)));
  return 1;
}


static int ce_yv6z3l (lua_State *L) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  const char *filename = luaL_checkstring(L, 1);
  return ce_esuu7u(L, remove(filename) == 0, filename);
}


static int ce_4y6s4x (lua_State *L) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  const char *fromname = luaL_checkstring(L, 1);
  const char *toname = luaL_checkstring(L, 2);
  return ce_esuu7u(L, rename(fromname, toname) == 0, fromname);
}


static int ce_da4t5y (lua_State *L) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  char buff[LUA_TMPNAMBUFSIZE];
  int err;
  lua_tmpnam(buff, err);
  if (err)
    return luaL_error(L, "unable to generate a unique filename");
  lua_pushstring(L, buff);
  return 1;
}


static int ce_0oubrw (lua_State *L) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushstring(L, getenv(luaL_checkstring(L, 1)));  /* if NULL push nil */
  return 1;
}


static int ce_8cukqh (lua_State *L) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushnumber(L, ((lua_Number)clock())/(lua_Number)CLOCKS_PER_SEC);
  return 1;
}


/*
** {======================================================
** Time/Date operations
** { year=%Y, month=%m, day=%d, hour=%H, min=%M, sec=%S,
**   wday=%w+1, yday=%j, isdst=? }
** =======================================================
*/

static void ce_1wdwvy (lua_State *L, const char *key, int value) {

    if ((0x1fa8 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushinteger(L, value);
  lua_ce_1wdwvy(L, -2, key);
}

static void ce_9iryhp (lua_State *L, const char *key, int value) {
  if (value < 0)  /* undefined? */
    return;  /* does not set field */
  lua_pushboolean(L, value);
  lua_ce_1wdwvy(L, -2, key);
}

static int ce_jbcskn (lua_State *L, const char *key) {
  int res;
  lua_ce_rb6irb(L, -1, key);
  res = lua_isnil(L, -1) ? -1 : lua_toboolean(L, -1);
  lua_pop(L, 1);
  return res;
}


static int ce_rb6irb (lua_State *L, const char *key, int d) {
  int res;
  lua_ce_rb6irb(L, -1, key);
  if (lua_isnumber(L, -1))
    res = (int)lua_tointeger(L, -1);
  else {
    if (d < 0)
      return luaL_error(L, "field " LUA_QS " missing in date table", key);
    res = d;
  }
  lua_pop(L, 1);
  return res;
}


static int ce_5la49p (lua_State *L) {
  const char *s = luaL_optstring(L, 1, "%c");
  time_t t = luaL_opt(L, (time_t)luaL_checknumber, 2, time(NULL));
  struct tm *stm;
  if (*s == '!') {  /* UTC? */
    stm = gmtime(&t);
    s++;  /* skip `!' */
  }
  else
    stm = localtime(&t);
  if (stm == NULL)  /* invalid date? */
    lua_pushnil(L);
  else if (strcmp(s, "*t") == 0) {
    lua_createtable(L, 0, 9);  /* 9 = number of fields */
    ce_1wdwvy(L, "sec", stm->tm_sec);
    ce_1wdwvy(L, "min", stm->tm_min);
    ce_1wdwvy(L, "hour", stm->tm_hour);
    ce_1wdwvy(L, "day", stm->tm_mday);
    ce_1wdwvy(L, "month", stm->tm_mon+1);
    ce_1wdwvy(L, "year", stm->tm_year+1900);
    ce_1wdwvy(L, "wday", stm->tm_wday+1);
    ce_1wdwvy(L, "yday", stm->tm_yday+1);
    ce_9iryhp(L, "isdst", stm->tm_isdst);
  }
  else {
    char cc[3];
    luaL_Buffer b;
    cc[0] = '%'; cc[2] = '\0';
    luaL_buffinit(L, &b);
    for (; *s; s++) {
      if (*s != '%' || *(s + 1) == '\0')  /* no conversion specifier? */
        luaL_addchar(&b, *s);
      else {
        size_t reslen;
        char buff[200];  /* should be big enough for any conversion result */
        cc[1] = *(++s);
        reslen = strftime(buff, sizeof(buff), cc, stm);
        luaL_addlstring(&b, buff, reslen);
      }
    }
    luaL_pushresult(&b);
  }
  return 1;
}


static int ce_l9anwo (lua_State *L) {
  time_t t;
  if (lua_isnoneornil(L, 1))  /* called without args? */
    t = time(NULL);  /* get current time */
  else {
    struct tm ts;
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_settop(L, 1);  /* make sure table is at the top */
    ts.tm_sec = ce_rb6irb(L, "sec", 0);
    ts.tm_min = ce_rb6irb(L, "min", 0);
    ts.tm_hour = ce_rb6irb(L, "hour", 12);
    ts.tm_mday = ce_rb6irb(L, "day", -1);
    ts.tm_mon = ce_rb6irb(L, "month", -1) - 1;
    ts.tm_year = ce_rb6irb(L, "year", -1) - 1900;
    ts.tm_isdst = ce_jbcskn(L, "isdst");
    t = mktime(&ts);
  }
  if (t == (time_t)(-1))
    lua_pushnil(L);
  else {
     /* On float systems the pushed value must be an integer, NOT a number.
      * Otherwise, accuracy is lost in the time_t->float conversion.
      */
#ifdef LNUM_FLOAT
     lua_pushinteger(L, (lua_Integer) t);
#else
     lua_pushnumber(L, (lua_Number) t);
#endif
     }
  return 1;
}


static int ce_reb688 (lua_State *L) {
#ifdef LNUM_FLOAT
  lua_Integer i= (lua_Integer)
    difftime( (time_t)(luaL_checkinteger(L, 1)),
              (time_t)(luaL_optinteger(L, 2, 0)));
  lua_pushinteger(L, i);
#else
  lua_pushnumber(L, difftime((time_t)(luaL_checknumber(L, 1)),
                             (time_t)(luaL_optnumber(L, 2, 0))));
#endif
  return 1;
}

/* }====================================================== */


static int ce_8fu0n1 (lua_State *L) {
  static const int cat[] = {LC_ALL, LC_COLLATE, LC_CTYPE, LC_MONETARY,
                      LC_NUMERIC, LC_TIME};
  static const char *const catnames[] = {"all", "collate", "ctype", "monetary",
     "numeric", "time", NULL};
  const char *l = luaL_optstring(L, 1, NULL);
  int op = luaL_checkoption(L, 2, "all", catnames);
  lua_pushstring(L, setlocale(cat[op], l));
  return 1;
}


static int ce_tu0fxd (lua_State *L) {
  exit(luaL_optint(L, 1, EXIT_SUCCESS));
}

static const luaL_Reg syslib[] = {
  {"clock",     ce_8cukqh},
  {"date",      ce_5la49p},
  {"difftime",  ce_reb688},
  {"execute",   ce_fa6uki},
  {"exit",      ce_tu0fxd},
  {"getenv",    ce_0oubrw},
  {"remove",    ce_yv6z3l},
  {"rename",    ce_4y6s4x},
  {"setlocale", ce_8fu0n1},
  {"time",      ce_l9anwo},
  {"tmpname",   ce_da4t5y},
  {NULL, NULL}
};

/* }====================================================== */



LUALIB_API int luaopen_os (lua_State *L) {
  luaL_register(L, LUA_OSLIBNAME, syslib);
  return 1;
}

