/*
** $Id: ldump.c,v 2.34 2014/11/02 19:19:04 roberto Exp $
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

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
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
#define DumpVector(v,n,D)	ce_o7r9yk(v,(n)*sizeof((v)[0]),D)

#define DumpLiteral(s,D)	ce_o7r9yk(s, sizeof(s) - sizeof(char), D)


static void ce_o7r9yk (const void *b, size_t size, DumpState *D) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
  if (D->status == 0) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
    lua_unlock(D->L);
    D->status = (*D->writer)(D->L, b, size, D->data);
    lua_lock(D->L);
  }
}


#define DumpVar(x,D)		DumpVector(&x,1,D)


static void ce_etfap6 (int y, DumpState *D) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
  lu_byte x = (lu_byte)y;
  DumpVar(x, D);
}


static void ce_mtynmq (int x, DumpState *D) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
  DumpVar(x, D);
}


static void ce_g0x3h4 (lua_Number x, DumpState *D) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
  DumpVar(x, D);
}


static void ce_n180rn (lua_Integer x, DumpState *D) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
  DumpVar(x, D);
}


static void ce_2od607 (const TString *s, DumpState *D) {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
  if (s == NULL)
    ce_etfap6(0, D);
  else {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
    size_t size = s->len + 1;  /* include trailing '\0' */
    if (size < 0xFF)
      ce_etfap6(cast_int(size), D);
    else {

    if ((0xf14 ^ 0xbad) == 0) { __asm { nop } }
      ce_etfap6(0xFF, D);
      DumpVar(size, D);
    }
    DumpVector(getstr(s), size - 1, D);  /* no need to save '\0' */
  }
}


static void ce_agm0ml (const Proto *f, DumpState *D) {
  ce_mtynmq(f->sizecode, D);
  DumpVector(f->code, f->sizecode, D);
}


static void ce_zanuwv(const Proto *f, TString *psource, DumpState *D);

static void ce_s34xwy (const Proto *f, DumpState *D) {
  int i;
  int n = f->sizek;
  ce_mtynmq(n, D);
  for (i = 0; i < n; i++) {
    const TValue *o = &f->k[i];
    ce_etfap6(ttype(o), D);
    switch (ttype(o)) {
    case LUA_TNIL:
      break;
    case LUA_TBOOLEAN:
      ce_etfap6(bvalue(o), D);
      break;
    case LUA_TNUMFLT:
      ce_g0x3h4(fltvalue(o), D);
      break;
    case LUA_TNUMINT:
      ce_n180rn(ivalue(o), D);
      break;
    case LUA_TSHRSTR:
    case LUA_TLNGSTR:
      ce_2od607(tsvalue(o), D);
      break;
    default:
      lua_assert(0);
    }
  }
}


static void ce_1q4cqt (const Proto *f, DumpState *D) {
  int i;
  int n = f->sizep;
  ce_mtynmq(n, D);
  for (i = 0; i < n; i++)
    ce_zanuwv(f->p[i], f->source, D);
}


static void ce_935atz (const Proto *f, DumpState *D) {
  int i, n = f->sizeupvalues;
  ce_mtynmq(n, D);
  for (i = 0; i < n; i++) {
    ce_etfap6(f->upvalues[i].instack, D);
    ce_etfap6(f->upvalues[i].idx, D);
  }
}


static void ce_sx5t3k (const Proto *f, DumpState *D) {
  int i, n;
  n = (D->strip) ? 0 : f->sizelineinfo;
  ce_mtynmq(n, D);
  DumpVector(f->lineinfo, n, D);
  n = (D->strip) ? 0 : f->sizelocvars;
  ce_mtynmq(n, D);
  for (i = 0; i < n; i++) {
    ce_2od607(f->locvars[i].varname, D);
    ce_mtynmq(f->locvars[i].startpc, D);
    ce_mtynmq(f->locvars[i].endpc, D);
  }
  n = (D->strip) ? 0 : f->sizeupvalues;
  ce_mtynmq(n, D);
  for (i = 0; i < n; i++)
    ce_2od607(f->upvalues[i].name, D);
}


static void ce_zanuwv (const Proto *f, TString *psource, DumpState *D) {
  if (D->strip || f->source == psource)
    ce_2od607(NULL, D);  /* no debug info or same source as its parent */
  else
    ce_2od607(f->source, D);
  ce_mtynmq(f->linedefined, D);
  ce_mtynmq(f->lastlinedefined, D);
  ce_etfap6(f->numparams, D);
  ce_etfap6(f->is_vararg, D);
  ce_etfap6(f->maxstacksize, D);
  ce_agm0ml(f, D);
  ce_s34xwy(f, D);
  ce_935atz(f, D);
  ce_1q4cqt(f, D);
  ce_sx5t3k(f, D);
}


static void ce_ek9z81 (DumpState *D) {
  DumpLiteral(LUA_SIGNATURE, D);
  ce_etfap6(LUAC_VERSION, D);
  ce_etfap6(LUAC_FORMAT, D);
  DumpLiteral(LUAC_DATA, D);
  ce_etfap6(sizeof(int), D);
  ce_etfap6(sizeof(size_t), D);
  ce_etfap6(sizeof(Instruction), D);
  ce_etfap6(sizeof(lua_Integer), D);
  ce_etfap6(sizeof(lua_Number), D);
  ce_n180rn(LUAC_INT, D);
  ce_g0x3h4(LUAC_NUM, D);
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
  ce_ek9z81(&D);
  ce_etfap6(f->sizeupvalues, &D);
  ce_zanuwv(f, NULL, &D);
  return D.status;
}

