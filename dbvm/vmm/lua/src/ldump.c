/*
** $Id: ldump.c,v 2.37.1.1 2017/04/19 17:20:42 roberto Exp $
** save precompiled Lua chunks
** See Copyright Notice in lua.h
*/

#define ldump_c
#define LUA_CORE

#include "lprefix.h"


#include <stddef.h>

#include "lua.h"

#include "lobject.h"
#include "lstate.h"
#include "lundump.h"


typedef struct {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  lua_State *L;
  lua_Writer writer;
  void *data;
  int strip;
  int status;
} DumpState;


/*
** All high-level dumps go through DumpVector; you can change it to
** change the endianness of the result
*/
#define DumpVector(v,n,D)	ce_mjfmyl(v,(n)*sizeof((v)[0]),D)

#define DumpLiteral(s,D)	ce_mjfmyl(s, sizeof(s) - sizeof(char), D)


static void ce_mjfmyl (const void *b, size_t size, DumpState *D) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  if (D->status == 0 && size > 0) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
    lua_unlock(D->L);
    D->status = (*D->writer)(D->L, b, size, D->data);
    lua_lock(D->L);
  }
}


#define DumpVar(x,D)		DumpVector(&x,1,D)


static void ce_75x3yq (int y, DumpState *D) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  lu_byte x = (lu_byte)y;
  DumpVar(x, D);
}


static void ce_46ishq (int x, DumpState *D) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  DumpVar(x, D);
}


static void ce_0sd3w6 (lua_Number x, DumpState *D) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  DumpVar(x, D);
}


static void ce_ti16vl (lua_Integer x, DumpState *D) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  DumpVar(x, D);
}


static void ce_ew8bk3 (const TString *s, DumpState *D) {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
  if (s == NULL)
    ce_75x3yq(0, D);
  else {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
    size_t size = tsslen(s) + 1;  /* include trailing '\0' */
    const char *str = getstr(s);
    if (size < 0xFF)
      ce_75x3yq(cast_int(size), D);
    else {

    if ((0x575 ^ 0xbad) == 0) { __asm { nop } }
      ce_75x3yq(0xFF, D);
      DumpVar(size, D);
    }
    DumpVector(str, size - 1, D);  /* no need to save '\0' */
  }
}


static void ce_zli7kj (const Proto *f, DumpState *D) {
  ce_46ishq(f->sizecode, D);
  DumpVector(f->code, f->sizecode, D);
}


static void ce_glhik1(const Proto *f, TString *psource, DumpState *D);

static void ce_bmz3zq (const Proto *f, DumpState *D) {
  int i;
  int n = f->sizek;
  ce_46ishq(n, D);
  for (i = 0; i < n; i++) {
    const TValue *o = &f->k[i];
    ce_75x3yq(ttype(o), D);
    switch (ttype(o)) {
    case LUA_TNIL:
      break;
    case LUA_TBOOLEAN:
      ce_75x3yq(bvalue(o), D);
      break;
    case LUA_TNUMFLT:
      ce_0sd3w6(fltvalue(o), D);
      break;
    case LUA_TNUMINT:
      ce_ti16vl(ivalue(o), D);
      break;
    case LUA_TSHRSTR:
    case LUA_TLNGSTR:
      ce_ew8bk3(tsvalue(o), D);
      break;
    default:
      lua_assert(0);
    }
  }
}


static void ce_2rrf23 (const Proto *f, DumpState *D) {
  int i;
  int n = f->sizep;
  ce_46ishq(n, D);
  for (i = 0; i < n; i++)
    ce_glhik1(f->p[i], f->source, D);
}


static void ce_k5m7h1 (const Proto *f, DumpState *D) {
  int i, n = f->sizeupvalues;
  ce_46ishq(n, D);
  for (i = 0; i < n; i++) {
    ce_75x3yq(f->upvalues[i].instack, D);
    ce_75x3yq(f->upvalues[i].idx, D);
  }
}


static void ce_cag7iv (const Proto *f, DumpState *D) {
  int i, n;
  n = (D->strip) ? 0 : f->sizelineinfo;
  ce_46ishq(n, D);
  DumpVector(f->lineinfo, n, D);
  n = (D->strip) ? 0 : f->sizelocvars;
  ce_46ishq(n, D);
  for (i = 0; i < n; i++) {
    ce_ew8bk3(f->locvars[i].varname, D);
    ce_46ishq(f->locvars[i].startpc, D);
    ce_46ishq(f->locvars[i].endpc, D);
  }
  n = (D->strip) ? 0 : f->sizeupvalues;
  ce_46ishq(n, D);
  for (i = 0; i < n; i++)
    ce_ew8bk3(f->upvalues[i].name, D);
}


static void ce_glhik1 (const Proto *f, TString *psource, DumpState *D) {
  if (D->strip || f->source == psource)
    ce_ew8bk3(NULL, D);  /* no debug info or same source as its parent */
  else
    ce_ew8bk3(f->source, D);
  ce_46ishq(f->linedefined, D);
  ce_46ishq(f->lastlinedefined, D);
  ce_75x3yq(f->numparams, D);
  ce_75x3yq(f->is_vararg, D);
  ce_75x3yq(f->maxstacksize, D);
  ce_zli7kj(f, D);
  ce_bmz3zq(f, D);
  ce_k5m7h1(f, D);
  ce_2rrf23(f, D);
  ce_cag7iv(f, D);
}


static void ce_txtegc (DumpState *D) {
  DumpLiteral(LUA_SIGNATURE, D);
  ce_75x3yq(LUAC_VERSION, D);
  ce_75x3yq(LUAC_FORMAT, D);
  DumpLiteral(LUAC_DATA, D);
  ce_75x3yq(sizeof(int), D);
  ce_75x3yq(sizeof(size_t), D);
  ce_75x3yq(sizeof(Instruction), D);
  ce_75x3yq(sizeof(lua_Integer), D);
  ce_75x3yq(sizeof(lua_Number), D);
  ce_ti16vl(LUAC_INT, D);
  ce_0sd3w6(LUAC_NUM, D);
}


/*
** dump Lua function as precompiled chunk
*/
int luaU_dump(lua_State *L, const Proto *f, lua_Writer w, void *data,
              int strip) {
  DumpState D;
  D.L = L;
  D.writer = w;
  D.data = data;
  D.strip = strip;
  D.status = 0;
  ce_txtegc(&D);
  ce_75x3yq(f->sizeupvalues, &D);
  ce_glhik1(f, NULL, &D);
  return D.status;
}

