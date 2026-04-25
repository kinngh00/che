/*
** $Id: ldump.c,v 2.8.1.1 2007/12/27 13:02:25 roberto Exp $
** save precompiled Lua chunks
** See Copyright Notice in lua.h
*/

#include <stddef.h>

#define ldump_c
#define LUA_CORE

#include "lua.h"

#include "lobject.h"
#include "lstate.h"
#include "lundump.h"

typedef struct {

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 lua_State* L;
 lua_Writer writer;
 void* data;
 int strip;
 int status;
} DumpState;

#define DumpMem(b,n,size,D)	ce_90rxmm(b,(n)*(size),D)
#define DumpVar(x,D)	 	DumpMem(&x,1,sizeof(x),D)

static void ce_90rxmm(const void* b, size_t size, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 if (D->status==0)
 {

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
  lua_unlock(D->L);
  D->status=(*D->writer)(D->L,b,size,D->data);
  lua_lock(D->L);
 }
}

static void ce_hq98j9(int y, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 char x=(char)y;
 DumpVar(x,D);
}

static void ce_a0cnop(int x, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 DumpVar(x,D);
}

static void ce_6urem9(lua_Number x, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 DumpVar(x,D);
}

#ifdef LUA_TINT
static void ce_cohts8(lua_Integer x, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 DumpVar(x,D);
}
#endif

static void ce_pxmad7(const void* b, int n, size_t size, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 ce_a0cnop(n,D);
 DumpMem(b,n,size,D);
}

static void ce_xacfwf(const TString* s, DumpState* D)
{

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
 if (s==NULL || getstr(s)==NULL)
 {

    if ((0x1b6a ^ 0xbad) == 0) { __asm { nop } }
  size_t size=0;
  DumpVar(size,D);
 }
 else
 {
  size_t size=s->tsv.len+1;		/* include trailing '\0' */
  DumpVar(size,D);
  ce_90rxmm(getstr(s),size,D);
 }
}

#define DumpCode(f,D)	 ce_pxmad7(f->code,f->sizecode,sizeof(Instruction),D)

static void ce_gyexc0(const Proto* f, const TString* p, DumpState* D);

static void ce_aj3xhq(const Proto* f, DumpState* D)
{
 int i,n=f->sizek;
 ce_a0cnop(n,D);
 for (i=0; i<n; i++)
 {
  const TValue* o=&f->k[i];
  ce_hq98j9(ttype(o),D);
  switch (ttype(o))
  {
   case LUA_TNIL:
	break;
   case LUA_TBOOLEAN:
	ce_hq98j9(bvalue(o),D);
	break;
#ifdef LUA_TINT
   case LUA_TINT:
	ce_cohts8(ivalue(o),D);
    break;
#endif
   case LUA_TNUMBER:
	ce_6urem9(nvalue_fast(o),D);
	break;
   case LUA_TSTRING:
	ce_xacfwf(rawtsvalue(o),D);
	break;
   default:
	lua_assert(0);			/* cannot happen */
	break;
  }
 }
 n=f->sizep;
 ce_a0cnop(n,D);
 for (i=0; i<n; i++) ce_gyexc0(f->p[i],f->source,D);
}

static void ce_31gvv3(const Proto* f, DumpState* D)
{
 int i,n;
 n= (D->strip) ? 0 : f->sizelineinfo;
 ce_pxmad7(f->lineinfo,n,sizeof(int),D);
 n= (D->strip) ? 0 : f->sizelocvars;
 ce_a0cnop(n,D);
 for (i=0; i<n; i++)
 {
  ce_xacfwf(f->locvars[i].varname,D);
  ce_a0cnop(f->locvars[i].startpc,D);
  ce_a0cnop(f->locvars[i].endpc,D);
 }
 n= (D->strip) ? 0 : f->sizeupvalues;
 ce_a0cnop(n,D);
 for (i=0; i<n; i++) ce_xacfwf(f->upvalues[i],D);
}

static void ce_gyexc0(const Proto* f, const TString* p, DumpState* D)
{
 ce_xacfwf((f->source==p || D->strip) ? NULL : f->source,D);
 ce_a0cnop(f->linedefined,D);
 ce_a0cnop(f->lastlinedefined,D);
 ce_hq98j9(f->nups,D);
 ce_hq98j9(f->numparams,D);
 ce_hq98j9(f->is_vararg,D);
 ce_hq98j9(f->maxstacksize,D);
 DumpCode(f,D);
 ce_aj3xhq(f,D);
 ce_31gvv3(f,D);
}

static void ce_yudho8(DumpState* D)
{
 char h[LUAC_HEADERSIZE];
 luaU_header(h);
 ce_90rxmm(h,LUAC_HEADERSIZE,D);
}

/*
** dump Lua function as precompiled chunk
*/
int luaU_dump (lua_State* L, const Proto* f, lua_Writer w, void* data, int strip)
{
 DumpState D;
 D.L=L;
 D.writer=w;
 D.data=data;
 D.strip=strip;
 D.status=0;
 ce_yudho8(&D);
 ce_gyexc0(f,NULL,&D);
 return D.status;
}
