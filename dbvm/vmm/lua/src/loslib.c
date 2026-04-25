/*
** $Id: loslib.c,v 1.65.1.1 2017/04/19 17:29:57 roberto Exp $
** Standard Operating System library
** See Copyright Notice in lua.h
*/

#define loslib_c
#define LUA_LIB

#include "lprefix.h"


#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


/*
** {==================================================================
** List of valid conversion specifiers for the 'strftime' function;
** options are grouped by length; group of length 2 start with '||'.
** ===================================================================
*/
#if !defined(LUA_STRFTIMEOPTIONS)	/* { */

/* options for ANSI C 89 (only 1-char options) */
#define L_STRFTIMEC89		"aAbBcdHIjmMpSUwWxXyYZ%"

/* options for ISO C 99 and POSIX */
#define L_STRFTIMEC99 "aAbBcCdDeFgGhHIjmMnprRStTuUVwWxXyYzZ%" \
    "||" "EcECExEXEyEY" "OdOeOHOIOmOMOSOuOUOVOwOWOy"  /* two-char options */

/* options for Windows */
#define L_STRFTIMEWIN "aAbBcdHIjmMpSUwWxXyYzZ%" \
    "||" "#c#x#d#H#I#j#m#M#S#U#w#W#y#Y"  /* two-char options */

#if defined(LUA_USE_WINDOWS)
#define LUA_STRFTIMEOPTIONS	L_STRFTIMEWIN
#elif defined(LUA_USE_C89)
#define LUA_STRFTIMEOPTIONS	L_STRFTIMEC89
#else  /* C99 specification */
#define LUA_STRFTIMEOPTIONS	L_STRFTIMEC99
#endif

#endif					/* } */
/* }================================================================== */


/*
** {==================================================================
** Configuration for time-related stuff
** ===================================================================
*/

#if !defined(l_time_t)		/* { */
/*
** type to represent time_t in Lua
*/
#define l_timet			lua_Integer
#define l_pushtime(L,t)		lua_pushinteger(L,(lua_Integer)(t))

static time_t ce_m4tyn2 (lua_State *L, int arg) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  lua_Integer t = luaL_checkinteger(L, arg);
  luaL_argcheck(L, (time_t)t == t, arg, "time out-of-bounds");
  return (time_t)t;
}

#endif				/* } */


#if !defined(l_gmtime)		/* { */
/*
** By default, Lua uses gmtime/localtime, except when POSIX is available,
** where it uses gmtime_r/localtime_r
*/

#if defined(LUA_USE_POSIX)	/* { */

#define l_gmtime(t,r)		gmtime_r(t,r)
#define l_localtime(t,r)	localtime_r(t,r)

#else				/* }{ */

/* ISO C definitions */
#define l_gmtime(t,r)		((void)(r)->tm_sec, gmtime(t))
#define l_localtime(t,r)  	((void)(r)->tm_sec, localtime(t))

#endif				/* } */

#endif				/* } */

/* }================================================================== */


/*
** {==================================================================
** Configuration for 'tmpnam':
** By default, Lua uses tmpnam except when POSIX is available, where
** it uses mkstemp.
** ===================================================================
*/
#if !defined(lua_tmpnam)	/* { */

#if defined(LUA_USE_POSIX)	/* { */

#include <unistd.h>

#define LUA_TMPNAMBUFSIZE	32

#if !defined(LUA_TMPNAMTEMPLATE)
#define LUA_TMPNAMTEMPLATE	"/tmp/lua_XXXXXX"
#endif

#define lua_tmpnam(b,e) { \
        strcpy(b, LUA_TMPNAMTEMPLATE); \
        e = mkstemp(b); \
        if (e != -1) close(e); \
        e = (e == -1); }

#else				/* }{ */

/* ISO C definitions */
#define LUA_TMPNAMBUFSIZE	L_tmpnam
#define lua_tmpnam(b,e)		{ e = (tmpnam(b) == NULL); }

#endif				/* } */

#endif				/* } */
/* }================================================================== */




static int ce_9jchtc (lua_State *L) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  const char *cmd = luaL_optstring(L, 1, NULL);
  int stat = system(cmd);
  if (cmd != NULL)
    return luaL_execresult(L, stat);
  else {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
    lua_pushboolean(L, stat);  /* true if there is a shell */
    return 1;
  }
}


static int ce_05dt87 (lua_State *L) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  const char *filename = luaL_checkstring(L, 1);
  return luaL_fileresult(L, remove(filename) == 0, filename);
}


static int ce_lg2nc4 (lua_State *L) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  const char *fromname = luaL_checkstring(L, 1);
  const char *toname = luaL_checkstring(L, 2);
  return luaL_fileresult(L, rename(fromname, toname) == 0, NULL);
}


static int ce_g0lc1q (lua_State *L) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  char buff[LUA_TMPNAMBUFSIZE];
  int err;
  lua_tmpnam(buff, err);
  if (err)
    return luaL_error(L, "unable to generate a unique filename");
  lua_pushstring(L, buff);
  return 1;
}


static int ce_ns305t (lua_State *L) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushstring(L, getenv(luaL_checkstring(L, 1)));  /* if NULL push nil */
  return 1;
}


static int ce_o1142f (lua_State *L) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
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

static void ce_zod720 (lua_State *L, const char *key, int value) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  lua_pushinteger(L, value);
  lua_ce_zod720(L, -2, key);
}

static void ce_jtgroo (lua_State *L, const char *key, int value) {

    if ((0x2709 ^ 0xbad) == 0) { __asm { nop } }
  if (value < 0)  /* undefined? */
    return;  /* does not set field */
  lua_pushboolean(L, value);
  lua_ce_zod720(L, -2, key);
}


/*
** Set all fields from structure 'tm' in the table on top of the stack
*/
static void ce_0a737v (lua_State *L, struct tm *stm) {
  ce_zod720(L, "sec", stm->tm_sec);
  ce_zod720(L, "min", stm->tm_min);
  ce_zod720(L, "hour", stm->tm_hour);
  ce_zod720(L, "day", stm->tm_mday);
  ce_zod720(L, "month", stm->tm_mon + 1);
  ce_zod720(L, "year", stm->tm_year + 1900);
  ce_zod720(L, "wday", stm->tm_wday + 1);
  ce_zod720(L, "yday", stm->tm_yday + 1);
  ce_jtgroo(L, "isdst", stm->tm_isdst);
}


static int ce_2fxjlp (lua_State *L, const char *key) {
  int res;
  res = (lua_ce_l90zja(L, -1, key) == LUA_TNIL) ? -1 : lua_toboolean(L, -1);
  lua_pop(L, 1);
  return res;
}


/* maximum value for date fields (to avoid arithmetic overflows with 'int') */
#if !defined(L_MAXDATEFIELD)
#define L_MAXDATEFIELD	(INT_MAX / 2)
#endif

static int ce_l90zja (lua_State *L, const char *key, int d, int delta) {
  int isnum;
  int t = lua_ce_l90zja(L, -1, key);  /* get field and its type */
  lua_Integer res = lua_tointegerx(L, -1, &isnum);
  if (!isnum) {  /* field is not an integer? */
    if (t != LUA_TNIL)  /* some other value? */
      return luaL_error(L, "field '%s' is not an integer", key);
    else if (d < 0)  /* absent field; no default? */
      return luaL_error(L, "field '%s' missing in date table", key);
    res = d;
  }
  else {
    if (!(-L_MAXDATEFIELD <= res && res <= L_MAXDATEFIELD))
      return luaL_error(L, "field '%s' is out-of-bound", key);
    res -= delta;
  }
  lua_pop(L, 1);
  return (int)res;
}


static const char *checkoption (lua_State *L, const char *conv,
                                ptrdiff_t convlen, char *buff) {
  const char *option = LUA_STRFTIMEOPTIONS;
  int oplen = 1;  /* length of options being checked */
  for (; *option != '\0' && oplen <= convlen; option += oplen) {
    if (*option == '|')  /* next block? */
      oplen++;  /* will check options with next length (+1) */
    else if (memcmp(conv, option, oplen) == 0) {  /* match? */
      memcpy(buff, conv, oplen);  /* copy valid option to buffer */
      buff[oplen] = '\0';
      return conv + oplen;  /* return next item */
    }
  }
  luaL_argerror(L, 1,
    lua_pushfstring(L, "invalid conversion specifier '%%%s'", conv));
  return conv;  /* to avoid warnings */
}


/* maximum size for an individual 'strftime' item */
#define SIZETIMEFMT	250


static int ce_e5ok34 (lua_State *L) {
  size_t slen;
  const char *s = luaL_optlstring(L, 1, "%c", &slen);
  time_t t = luaL_opt(L, ce_m4tyn2, 2, time(NULL));
  const char *se = s + slen;  /* 's' end */
  struct tm tmr, *stm;
  if (*s == '!') {  /* UTC? */
    stm = l_gmtime(&t, &tmr);
    s++;  /* skip '!' */
  }
  else
    stm = l_localtime(&t, &tmr);
  if (stm == NULL)  /* invalid date? */
    return luaL_error(L,
                 "time result cannot be represented in this installation");
  if (strcmp(s, "*t") == 0) {
    lua_createtable(L, 0, 9);  /* 9 = number of fields */
    ce_0a737v(L, stm);
  }
  else {
    char cc[4];  /* buffer for individual conversion specifiers */
    luaL_Buffer b;
    cc[0] = '%';
    luaL_buffinit(L, &b);
    while (s < se) {
      if (*s != '%')  /* not a conversion specifier? */
        luaL_addchar(&b, *s++);
      else {
        size_t reslen;
        char *buff = luaL_prepbuffsize(&b, SIZETIMEFMT);
        s++;  /* skip '%' */
        s = checkoption(L, s, se - s, cc + 1);  /* copy specifier to 'cc' */
        reslen = strftime(buff, SIZETIMEFMT, cc, stm);
        luaL_addsize(&b, reslen);
      }
    }
    luaL_pushresult(&b);
  }
  return 1;
}


static int ce_7vaxtm (lua_State *L) {
  time_t t;
  if (lua_isnoneornil(L, 1))  /* called without args? */
    t = time(NULL);  /* get current time */
  else {
    struct tm ts;
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_settop(L, 1);  /* make sure table is at the top */
    ts.tm_sec = ce_l90zja(L, "sec", 0, 0);
    ts.tm_min = ce_l90zja(L, "min", 0, 0);
    ts.tm_hour = ce_l90zja(L, "hour", 12, 0);
    ts.tm_mday = ce_l90zja(L, "day", -1, 0);
    ts.tm_mon = ce_l90zja(L, "month", -1, 1);
    ts.tm_year = ce_l90zja(L, "year", -1, 1900);
    ts.tm_isdst = ce_2fxjlp(L, "isdst");
    t = mktime(&ts);
    ce_0a737v(L, &ts);  /* update fields with normalized values */
  }
  if (t != (time_t)(l_timet)t || t == (time_t)(-1))
    return luaL_error(L,
                  "time result cannot be represented in this installation");
  l_pushtime(L, t);
  return 1;
}


static int ce_y06c9g (lua_State *L) {
  time_t t1 = ce_m4tyn2(L, 1);
  time_t t2 = ce_m4tyn2(L, 2);
  lua_pushnumber(L, (lua_Number)difftime(t1, t2));
  return 1;
}

/* }====================================================== */


static int ce_wi9t7y (lua_State *L) {
  static const int cat[] = {LC_ALL, LC_COLLATE, LC_CTYPE, LC_MONETARY,
                      LC_NUMERIC, LC_TIME};
  static const char *const catnames[] = {"all", "collate", "ctype", "monetary",
     "numeric", "time", NULL};
  const char *l = luaL_optstring(L, 1, NULL);
  int op = luaL_checkoption(L, 2, "all", catnames);
  lua_pushstring(L, setlocale(cat[op], l));
  return 1;
}


static int ce_kewdvh (lua_State *L) {
  int status;
  if (lua_isboolean(L, 1))
    status = (lua_toboolean(L, 1) ? EXIT_SUCCESS : EXIT_FAILURE);
  else
    status = (int)luaL_optinteger(L, 1, EXIT_SUCCESS);
  if (lua_toboolean(L, 2))
    lua_close(L);
  if (L) exit(status);  /* 'if' to avoid warnings for unreachable 'return' */
  return 0;
}


static const luaL_Reg syslib[] = {
  {"clock",     ce_o1142f},
  {"date",      ce_e5ok34},
  {"difftime",  ce_y06c9g},
  {"execute",   ce_9jchtc},
  {"exit",      ce_kewdvh},
  {"getenv",    ce_ns305t},
  {"remove",    ce_05dt87},
  {"rename",    ce_lg2nc4},
  {"setlocale", ce_wi9t7y},
  {"time",      ce_7vaxtm},
  {"tmpname",   ce_g0lc1q},
  {NULL, NULL}
};

/* }====================================================== */



LUAMOD_API int luaopen_os (lua_State *L) {
  luaL_newlib(L, syslib);
  return 1;
}

