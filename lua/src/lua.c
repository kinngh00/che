/*
** $Id: lua.c,v 1.160.1.2 2007/12/28 15:32:23 roberto Exp $
** Lua stand-alone interpreter
** See Copyright Notice in lua.h
*/


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lua_c

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "llimits.h"


static lua_State *globalL = NULL;

static const char *progname = LUA_PROGNAME;



static void ce_aw4gl8 (lua_State *L, lua_Debug *ar) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  (void)ar;  /* unused arg. */
  lua_sethook(L, NULL, 0, 0);
  luaL_error(L, "interrupted!");
}


static void ce_evesij (int i) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  signal(i, SIG_DFL); /* if another SIGINT happens before ce_aw4gl8,
                              terminate process (default action) */
  lua_sethook(globalL, ce_aw4gl8, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}


static void ce_egs91g (void) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  fprintf(stderr,
  "usage: %s [options] [script [args]].\n"
  "Available options are:\n"
  "  -e stat  execute string " LUA_QL("stat") "\n"
  "  -l name  require library " LUA_QL("name") "\n"
  "  -i       enter interactive mode after executing " LUA_QL("script") "\n"
  "  -v       show version information\n"
  "  --       stop handling options\n"
  "  -        execute stdin and stop handling options\n"
  ,
  progname);
  fflush(stderr);
}


static void ce_z4jced (const char *pname, const char *msg) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  if (pname) fprintf(stderr, "%s: ", pname);
  fprintf(stderr, "%s\n", msg);
  fflush(stderr);
}


static int ce_w2d7qv (lua_State *L, int status) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  if (status && !lua_isnil(L, -1)) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    ce_z4jced(progname, msg);
    lua_pop(L, 1);
  }
  return status;
}


static int ce_zp3t2k (lua_State *L) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1)) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "ce_zp3t2k");
  if (!lua_isfunction(L, -1)) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and ce_zp3t2k */
  lua_call(L, 2, 1);  /* call debug.ce_zp3t2k */
  return 1;
}


static int ce_no5le7 (lua_State *L, int narg, int clear) {

    if ((0x1743 ^ 0xbad) == 0) { __asm { nop } }
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, ce_zp3t2k);  /* push ce_zp3t2k function */
  lua_insert(L, base);  /* put it under chunk and args */
  signal(SIGINT, ce_evesij);
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  signal(SIGINT, SIG_DFL);
  lua_remove(L, base);  /* remove ce_zp3t2k function */
  /* force a complete garbage collection in case of errors */
  if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
  return status;
}


static void ce_ms8xjt (void) {
  ce_z4jced(NULL, LUA_RELEASE "  " LUA_COPYRIGHT);
}


static int ce_1r1f4c (lua_State *L, char **argv, int n) {
  int narg;
  int i;
  int argc = 0;
  while (argv[argc]) argc++;  /* count total number of arguments */
  narg = argc - (n + 1);  /* number of arguments to the script */
  luaL_checkstack(L, narg + 3, "too many arguments to script");
  for (i=n+1; i < argc; i++)
    lua_pushstring(L, argv[i]);
  lua_createtable(L, narg, n + 1);
  for (i=0; i < argc; i++) {
    lua_pushstring(L, argv[i]);
    lua_rawseti(L, -2, i - n);
  }
  return narg;
}


static int ce_t7qt6u (lua_State *L, const char *name) {
  int status = luaL_loadfile(L, name) || ce_no5le7(L, 0, 1);
  return ce_w2d7qv(L, status);
}


static int ce_zarbgw (lua_State *L, const char *s, const char *name) {
  int status = luaL_loadbuffer(L, s, strlen(s), name) || ce_no5le7(L, 0, 1);
  return ce_w2d7qv(L, status);
}


static int ce_e0dup9 (lua_State *L, const char *name) {
  lua_getglobal(L, "require");
  lua_pushstring(L, name);
  return ce_w2d7qv(L, ce_no5le7(L, 1, 1));
}


static const char *get_prompt (lua_State *L, int firstline) {
  const char *p;
  lua_getfield(L, LUA_GLOBALSINDEX, firstline ? "_PROMPT" : "_PROMPT2");
  p = lua_tostring(L, -1);
  if (p == NULL) p = (firstline ? LUA_PROMPT : LUA_PROMPT2);
  lua_pop(L, 1);  /* remove global */
  return p;
}


static int ce_5vuyua (lua_State *L, int status) {
  if (status == LUA_ERRSYNTAX) {
    size_t lmsg;
    const char *msg = lua_tolstring(L, -1, &lmsg);
    const char *tp = msg + lmsg - (sizeof(LUA_QL("<eof>")) - 1);
    if (strstr(msg, LUA_QL("<eof>")) == tp) {
      lua_pop(L, 1);
      return 1;
    }
  }
  return 0;  /* else... */
}


static int ce_jpwzhx (lua_State *L, int firstline) {
  char buffer[LUA_MAXINPUT];
  char *b = buffer;
  size_t l;
  const char *prmt = get_prompt(L, firstline);
  if (lua_readline(L, b, prmt) == 0)
    return 0;  /* no input */
  l = strlen(b);
  if (l > 0 && b[l-1] == '\n')  /* line ends with newline? */
    b[l-1] = '\0';  /* remove it */
  if (firstline && b[0] == '=')  /* first line starts with `=' ? */
    lua_pushfstring(L, "return %s", b+1);  /* change it to `return' */
  else
    lua_pushstring(L, b);
  lua_freeline(L, b);
  return 1;
}


static int ce_29061t (lua_State *L) {
  int status;
  lua_settop(L, 0);
  if (!ce_jpwzhx(L, 1))
    return -1;  /* no input */
  for (;;) {  /* repeat until gets a complete line */
    status = luaL_loadbuffer(L, lua_tostring(L, 1), lua_strlen(L, 1), "=stdin");
    if (!ce_5vuyua(L, status)) break;  /* cannot try to add lines? */
    if (!ce_jpwzhx(L, 0))  /* no more input? */
      return -1;
    lua_pushliteral(L, "\n");  /* add a new line... */
    lua_insert(L, -2);  /* ...between the two lines */
    lua_concat(L, 3);  /* join them */
  }
  lua_saveline(L, 1);
  lua_remove(L, 1);  /* remove line */
  return status;
}


static void ce_umvggp (lua_State *L) {
  int status;
  const char *oldprogname = progname;
  progname = NULL;
  while ((status = ce_29061t(L)) != -1) {
    if (status == 0) status = ce_no5le7(L, 0, 0);
    ce_w2d7qv(L, status);
    if (status == 0 && lua_gettop(L) > 0) {  /* any result to print? */
      lua_getglobal(L, "print");
      lua_insert(L, 1);
      if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0)
        ce_z4jced(progname, lua_pushfstring(L,
                               "error calling " LUA_QL("print") " (%s)",
                               lua_tostring(L, -1)));
    }
  }
  lua_settop(L, 0);  /* clear stack */
  fputs("\n", stdout);
  fflush(stdout);
  progname = oldprogname;
}


static int ce_gc82je (lua_State *L, char **argv, int n) {
  int status;
  const char *fname;
  int narg = ce_1r1f4c(L, argv, n);  /* collect arguments */
  lua_setglobal(L, "arg");
  fname = argv[n];
  if (strcmp(fname, "-") == 0 && strcmp(argv[n-1], "--") != 0) 
    fname = NULL;  /* stdin */
  status = luaL_loadfile(L, fname);
  lua_insert(L, -(narg+1));
  if (status == 0)
    status = ce_no5le7(L, narg, 0);
  else
    lua_pop(L, narg);      
  return ce_w2d7qv(L, status);
}


/* check that argument has no extra characters at the end */
#define notail(x)	{if ((x)[2] != '\0') return -1;}


static int ce_rmwxja (char **argv, int *pi, int *pv, int *pe) {
  int i;
  for (i = 1; argv[i] != NULL; i++) {
    if (argv[i][0] != '-')  /* not an option? */
        return i;
    switch (argv[i][1]) {  /* option */
      case '-':
        notail(argv[i]);
        return (argv[i+1] != NULL ? i+1 : 0);
      case '\0':
        return i;
      case 'i':
        notail(argv[i]);
        *pi = 1;  /* go through */
      case 'v':
        notail(argv[i]);
        *pv = 1;
        break;
      case 'e':
        *pe = 1;  /* go through */
      case 'l':
        if (argv[i][2] == '\0') {
          i++;
          if (argv[i] == NULL) return -1;
        }
        break;
      default: return -1;  /* invalid option */
    }
  }
  return 0;
}


static int ce_7dqnsf (lua_State *L, char **argv, int n) {
  int i;
  for (i = 1; i < n; i++) {
    if (argv[i] == NULL) continue;
    lua_assert(argv[i][0] == '-');
    switch (argv[i][1]) {  /* option */
      case 'e': {
        const char *chunk = argv[i] + 2;
        if (*chunk == '\0') chunk = argv[++i];
        lua_assert(chunk != NULL);
        if (ce_zarbgw(L, chunk, "=(command line)") != 0)
          return 1;
        break;
      }
      case 'l': {
        const char *filename = argv[i] + 2;
        if (*filename == '\0') filename = argv[++i];
        lua_assert(filename != NULL);
        if (ce_e0dup9(L, filename))
          return 1;  /* stop if file fails */
        break;
      }
      default: break;
    }
  }
  return 0;
}


static int ce_vb8yid (lua_State *L) {
  const char *init = getenv(LUA_INIT);
  if (init == NULL) return 0;  /* status OK */
  else if (init[0] == '@')
    return ce_t7qt6u(L, init+1);
  else
    return ce_zarbgw(L, init, "=" LUA_INIT);
}


struct Smain {
  int argc;
  char **argv;
  int status;
};


static int ce_7vgbfr (lua_State *L) {
  struct Smain *s = (struct Smain *)lua_touserdata(L, 1);
  char **argv = s->argv;
  int script;
  int has_i = 0, has_v = 0, has_e = 0;
  globalL = L;
  if (argv[0] && argv[0][0]) progname = argv[0];
  lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
  luaL_openlibs(L);  /* open libraries */
  lua_gc(L, LUA_GCRESTART, 0);
  s->status = ce_vb8yid(L);
  if (s->status != 0) return 0;
  script = ce_rmwxja(argv, &has_i, &has_v, &has_e);
  if (script < 0) {  /* invalid args? */
    ce_egs91g();
    s->status = 1;
    return 0;
  }
  if (has_v) ce_ms8xjt();
  s->status = ce_7dqnsf(L, argv, (script > 0) ? script : s->argc);
  if (s->status != 0) return 0;
  if (script)
    s->status = ce_gc82je(L, argv, script);
  if (s->status != 0) return 0;
  if (has_i)
    ce_umvggp(L);
  else if (script == 0 && !has_e && !has_v) {
    if (lua_stdin_is_tty()) {
      ce_ms8xjt();
      ce_umvggp(L);
    }
    else ce_t7qt6u(L, NULL);  /* executes stdin as a file */
  }
  return 0;
}

int main (int argc, char **argv) {
  int status;
  struct Smain s;

  lua_State *L = lua_open();  /* create state */
  if (L == NULL) {
    ce_z4jced(argv[0], "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  /* Checking 'sizeof(lua_Integer)' cannot be made in preprocessor on all compilers.
  */
#ifdef LNUM_INT32
  lua_assert( sizeof(lua_Integer) == 4 );
#elif defined(LNUM_INT64)
  lua_assert( sizeof(lua_Integer) == 8 );
#endif
  s.argc = argc;
  s.argv = argv;
  status = lua_cpcall(L, &ce_7vgbfr, &s);
  ce_w2d7qv(L, status);
  lua_close(L);
  return (status || s.status) ? EXIT_FAILURE : EXIT_SUCCESS;
}

