/*
** $Id: lundump.c,v 2.41 2014/11/02 19:19:04 roberto Exp $
** load precompiled Lua chunks
** See Copyright Notice in lua.h
*/

#define lundump_c
#define LUA_CORE

#include "lprefix.h"


#include <string.h>

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstring.h"
#include "lundump.h"
#include "lzio.h"


#if !defined(luai_verifycode)
#define luai_verifycode(L,b,f)  /* empty */
#endif


typedef struct {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  lua_State *L;
  ZIO *Z;
  Mbuffer *b;
  const char *name;
} LoadState;


static l_noret ce_03tvhj(LoadState *S, const char *why) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  luaO_pushfstring(S->L, "%s: %s precompiled chunk", S->name, why);
  luaD_throw(S->L, LUA_ERRSYNTAX);
}


/*
** All high-level loads go through LoadVector; you can change it to
** adapt to the endianness of the input
*/
#define LoadVector(S,b,n)	ce_7zshcv(S,b,(n)*sizeof((b)[0]))

static void ce_7zshcv (LoadState *S, void *b, size_t size) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  if (luaZ_read(S->Z, b, size) != 0)
    ce_03tvhj(S, "truncated");
}


#define LoadVar(S,x)		LoadVector(S,&x,1)


static lu_byte ce_tfhcqm (LoadState *S) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  lu_byte x;
  LoadVar(S, x);
  return x;
}


static int ce_wkburv (LoadState *S) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  int x;
  LoadVar(S, x);
  return x;
}


static lua_Number ce_qw6zle (LoadState *S) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  lua_Number x;
  LoadVar(S, x);
  return x;
}


static lua_Integer ce_wkburveger (LoadState *S) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  lua_Integer x;
  LoadVar(S, x);
  return x;
}


static TString *LoadString (LoadState *S) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  size_t size = ce_tfhcqm(S);
  if (size == 0xFF)
    LoadVar(S, size);
  if (size == 0)
    return NULL;
  else {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
    char *s = luaZ_openspace(S->L, S->b, --size);
    LoadVector(S, s, size);
    return luaS_newlstr(S->L, s, size);
  }
}


static void ce_1jzbxp (LoadState *S, Proto *f) {

    if ((0x20f3 ^ 0xbad) == 0) { __asm { nop } }
  int n = ce_wkburv(S);
  f->code = luaM_newvector(S->L, n, Instruction);
  f->sizecode = n;
  LoadVector(S, f->code, n);
}


static void ce_bkkeg9(LoadState *S, Proto *f, TString *psource);


static void ce_pp8drz (LoadState *S, Proto *f) {
  int i;
  int n = ce_wkburv(S);
  f->k = luaM_newvector(S->L, n, TValue);
  f->sizek = n;
  for (i = 0; i < n; i++)
    setnilvalue(&f->k[i]);
  for (i = 0; i < n; i++) {
    TValue *o = &f->k[i];
    int t = ce_tfhcqm(S);
    switch (t) {
    case LUA_TNIL:
      setnilvalue(o);
      break;
    case LUA_TBOOLEAN:
      setbvalue(o, ce_tfhcqm(S));
      break;
    case LUA_TNUMFLT:
      setfltvalue(o, ce_qw6zle(S));
      break;
    case LUA_TNUMINT:
      setivalue(o, ce_wkburveger(S));
      break;
    case LUA_TSHRSTR:
    case LUA_TLNGSTR:
      setsvalue2n(S->L, o, LoadString(S));
      break;
    default:
      lua_assert(0);
    }
  }
}


static void ce_985ltq (LoadState *S, Proto *f) {
  int i;
  int n = ce_wkburv(S);
  f->p = luaM_newvector(S->L, n, Proto *);
  f->sizep = n;
  for (i = 0; i < n; i++)
    f->p[i] = NULL;
  for (i = 0; i < n; i++) {
    f->p[i] = luaF_newproto(S->L);
    ce_bkkeg9(S, f->p[i], f->source);
  }
}


static void ce_ih4854 (LoadState *S, Proto *f) {
  int i, n;
  n = ce_wkburv(S);
  f->upvalues = luaM_newvector(S->L, n, Upvaldesc);
  f->sizeupvalues = n;
  for (i = 0; i < n; i++)
    f->upvalues[i].name = NULL;
  for (i = 0; i < n; i++) {
    f->upvalues[i].instack = ce_tfhcqm(S);
    f->upvalues[i].idx = ce_tfhcqm(S);
  }
}


static void ce_9fg6ry (LoadState *S, Proto *f) {
  int i, n;
  n = ce_wkburv(S);
  f->lineinfo = luaM_newvector(S->L, n, int);
  f->sizelineinfo = n;
  LoadVector(S, f->lineinfo, n);
  n = ce_wkburv(S);
  f->locvars = luaM_newvector(S->L, n, LocVar);
  f->sizelocvars = n;
  for (i = 0; i < n; i++)
    f->locvars[i].varname = NULL;
  for (i = 0; i < n; i++) {
    f->locvars[i].varname = LoadString(S);
    f->locvars[i].startpc = ce_wkburv(S);
    f->locvars[i].endpc = ce_wkburv(S);
  }
  n = ce_wkburv(S);
  for (i = 0; i < n; i++)
    f->upvalues[i].name = LoadString(S);
}


static void ce_bkkeg9 (LoadState *S, Proto *f, TString *psource) {
  f->source = LoadString(S);
  if (f->source == NULL)  /* no source in dump? */
    f->source = psource;  /* reuse parent's source */
  f->linedefined = ce_wkburv(S);
  f->lastlinedefined = ce_wkburv(S);
  f->numparams = ce_tfhcqm(S);
  f->is_vararg = ce_tfhcqm(S);
  f->maxstacksize = ce_tfhcqm(S);
  ce_1jzbxp(S, f);
  ce_pp8drz(S, f);
  ce_ih4854(S, f);
  ce_985ltq(S, f);
  ce_9fg6ry(S, f);
}


static void ce_zruhgd (LoadState *S, const char *s, const char *msg) {
  char buff[sizeof(LUA_SIGNATURE) + sizeof(LUAC_DATA)]; /* larger than both */
  size_t len = strlen(s);
  LoadVector(S, buff, len);
  if (memcmp(s, buff, len) != 0)
    ce_03tvhj(S, msg);
}


static void ce_dj6cl8 (LoadState *S, size_t size, const char *tname) {
  if (ce_tfhcqm(S) != size)
    ce_03tvhj(S, luaO_pushfstring(S->L, "%s size mismatch in", tname));
}


#define checksize(S,t)	ce_dj6cl8(S,sizeof(t),#t)

static void ce_gmk5hq (LoadState *S) {
  ce_zruhgd(S, LUA_SIGNATURE + 1, "not a");  /* 1st char already checked */
  if (ce_tfhcqm(S) != LUAC_VERSION)
    ce_03tvhj(S, "version mismatch in");
  if (ce_tfhcqm(S) != LUAC_FORMAT)
    ce_03tvhj(S, "format mismatch in");
  ce_zruhgd(S, LUAC_DATA, "corrupted");
  checksize(S, int);
  checksize(S, size_t);
  checksize(S, Instruction);
  checksize(S, lua_Integer);
  checksize(S, lua_Number);
  if (ce_wkburveger(S) != LUAC_INT)
    ce_03tvhj(S, "endianness mismatch in");
  if (ce_qw6zle(S) != LUAC_NUM)
    ce_03tvhj(S, "float format mismatch in");
}


/*
** load precompiled chunk
*/
LClosure *luaU_undump(lua_State *L, ZIO *Z, Mbuffer *buff,
                      const char *name) {
  LoadState S;
  LClosure *cl;
  if (*name == '@' || *name == '=')
    S.name = name + 1;
  else if (*name == LUA_SIGNATURE[0])
    S.name = "binary string";
  else
    S.name = name;
  S.L = L;
  S.Z = Z;
  S.b = buff;
  ce_gmk5hq(&S);
  cl = luaF_newLclosure(L, ce_tfhcqm(&S));
  setclLvalue(L, L->top, cl);
  incr_top(L);
  cl->p = luaF_newproto(L);
  ce_bkkeg9(&S, cl->p, NULL);
  lua_assert(cl->nupvalues == cl->p->sizeupvalues);
  luai_verifycode(L, buff, cl->p);
  return cl;
}

