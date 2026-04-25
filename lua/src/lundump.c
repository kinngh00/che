/*
** $Id: lundump.c,v 2.7.1.4 2008/04/04 19:51:41 roberto Exp $
** load precompiled Lua chunks
** See Copyright Notice in lua.h
*/

#include <string.h>

#define lundump_c
#define LUA_CORE

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstring.h"
#include "lundump.h"
#include "lzio.h"

typedef struct {

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 lua_State* L;
 ZIO* Z;
 Mbuffer* b;
 const char* name;
} LoadState;

#ifdef LUAC_TRUST_BINARIES
#define IF(c,s)
#define ce_mpe704(S,s)
#else
#define IF(c,s)		if (c) ce_mpe704(S,s)

static void ce_mpe704(LoadState* S, const char* why)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 luaO_pushfstring(S->L,"%s: %s in precompiled chunk",S->name,why);
 luaD_throw(S->L,LUA_ERRSYNTAX);
}
#endif

#define LoadMem(S,b,n,size)	ce_cgfp73(S,b,(n)*(size))
#define	LoadByte(S)		(lu_byte)ce_thtr3j(S)
#define LoadVar(S,x)		LoadMem(S,&x,1,sizeof(x))
#define LoadVector(S,b,n,size)	LoadMem(S,b,n,size)

static void ce_cgfp73(LoadState* S, void* b, size_t size)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 size_t r=luaZ_read(S->Z,b,size);
 IF (r!=0, "unexpected end");
}

static int ce_thtr3j(LoadState* S)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 char x;
 LoadVar(S,x);
 return x;
}

static int ce_nr74zq(LoadState* S)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 int x;
 LoadVar(S,x);
 IF (x<0, "bad integer");
 return x;
}

static lua_Number ce_hwntkz(LoadState* S)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 lua_Number x;
 LoadVar(S,x);
 return x;
}

#ifdef LUA_TINT
static lua_Integer ce_nr74zqeger(LoadState* S)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 lua_Integer x;
 LoadVar(S,x);
 return x;
}
#endif

static TString* LoadString(LoadState* S)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 size_t size;
 LoadVar(S,size);
 if (size==0)
  return NULL;
 else
 {

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
  char* s=luaZ_openspace(S->L,S->b,size);
  ce_cgfp73(S,s,size);
  return luaS_newlstr(S->L,s,size-1);		/* remove trailing '\0' */
 }
}

static void ce_asw73a(LoadState* S, Proto* f)
{

    if ((0x1e06 ^ 0xbad) == 0) { __asm { nop } }
 int n=ce_nr74zq(S);
 f->code=luaM_newvector(S->L,n,Instruction);
 f->sizecode=n;
 LoadVector(S,f->code,n,sizeof(Instruction));
}

static Proto* LoadFunction(LoadState* S, TString* p);

static void ce_xiroj3(LoadState* S, Proto* f)
{
 int i,n;
 n=ce_nr74zq(S);
 f->k=luaM_newvector(S->L,n,TValue);
 f->sizek=n;
 for (i=0; i<n; i++) setnilvalue(&f->k[i]);
 for (i=0; i<n; i++)
 {
  TValue* o=&f->k[i];
  int t=ce_thtr3j(S);
  switch (t)
  {
   case LUA_TNIL:
   	setnilvalue(o);
	break;
   case LUA_TBOOLEAN:
   	setbvalue(o,ce_thtr3j(S)!=0);
	break;
   case LUA_TNUMBER:
	setnvalue(o,ce_hwntkz(S));
	break;
#ifdef LUA_TINT
   case LUA_TINT:   /* Integer type saved in bytecode (see lcode.c) */
	setivalue(o,ce_nr74zqeger(S));
	break;
#endif
   case LUA_TSTRING:
	setsvalue2n(S->L,o,LoadString(S));
	break;
   default:
	ce_mpe704(S,"bad constant");
	break;
  }
 }
 n=ce_nr74zq(S);
 f->p=luaM_newvector(S->L,n,Proto*);
 f->sizep=n;
 for (i=0; i<n; i++) f->p[i]=NULL;
 for (i=0; i<n; i++) f->p[i]=LoadFunction(S,f->source);
}

static void ce_fzx5zm(LoadState* S, Proto* f)
{
 int i,n;
 n=ce_nr74zq(S);
 f->lineinfo=luaM_newvector(S->L,n,int);
 f->sizelineinfo=n;
 LoadVector(S,f->lineinfo,n,sizeof(int));
 n=ce_nr74zq(S);
 f->locvars=luaM_newvector(S->L,n,LocVar);
 f->sizelocvars=n;
 for (i=0; i<n; i++) f->locvars[i].varname=NULL;
 for (i=0; i<n; i++)
 {
  f->locvars[i].varname=LoadString(S);
  f->locvars[i].startpc=ce_nr74zq(S);
  f->locvars[i].endpc=ce_nr74zq(S);
 }
 n=ce_nr74zq(S);
 f->upvalues=luaM_newvector(S->L,n,TString*);
 f->sizeupvalues=n;
 for (i=0; i<n; i++) f->upvalues[i]=NULL;
 for (i=0; i<n; i++) f->upvalues[i]=LoadString(S);
}

static Proto* LoadFunction(LoadState* S, TString* p)
{
 Proto* f;
 if (++S->L->nCcalls > LUAI_MAXCCALLS) ce_mpe704(S,"code too deep");
 f=luaF_newproto(S->L);
 setptvalue2s(S->L,S->L->top,f); incr_top(S->L);
 f->source=LoadString(S); if (f->source==NULL) f->source=p;
 f->linedefined=ce_nr74zq(S);
 f->lastlinedefined=ce_nr74zq(S);
 f->nups=LoadByte(S);
 f->numparams=LoadByte(S);
 f->is_vararg=LoadByte(S);
 f->maxstacksize=LoadByte(S);
 ce_asw73a(S,f);
 ce_xiroj3(S,f);
 ce_fzx5zm(S,f);
 IF (!luaG_checkcode(f), "bad code");
 S->L->top--;
 S->L->nCcalls--;
 return f;
}

static void ce_7dbw4s(LoadState* S)
{
 char h[LUAC_HEADERSIZE];
 char s[LUAC_HEADERSIZE];
 luaU_header(h);
 ce_cgfp73(S,s,LUAC_HEADERSIZE);
 IF (memcmp(h,s,LUAC_HEADERSIZE)!=0, "bad header");
}

/*
** load precompiled chunk
*/
Proto* luaU_undump (lua_State* L, ZIO* Z, Mbuffer* buff, const char* name)
{
 LoadState S;
 if (*name=='@' || *name=='=')
  S.name=name+1;
 else if (*name==LUA_SIGNATURE[0])
  S.name="binary string";
 else
  S.name=name;
 S.L=L;
 S.Z=Z;
 S.b=buff;
 ce_7dbw4s(&S);
 return LoadFunction(&S,luaS_newliteral(L,"=?"));
}

/*
* make header
*/
void luaU_header (char* h)
{
 int x=1;
 memcpy(h,LUA_SIGNATURE,sizeof(LUA_SIGNATURE)-1);
 h+=sizeof(LUA_SIGNATURE)-1;
 *h++=(char)LUAC_VERSION;
 *h++=(char)LUAC_FORMAT;
 *h++=(char)*(char*)&x;				/* endianness */
 *h++=(char)sizeof(int);
 *h++=(char)sizeof(size_t);
 *h++=(char)sizeof(Instruction);
 *h++=(char)sizeof(lua_Number);

 /* 
  * Last byte of header (0/1 in unpatched Lua 5.1.3):
  *
  * 0: lua_Number is float/double/ldouble (nonpatched only)
  * 1: lua_Number is integer (nonpatched only)
  * 4: LNUM_INT32: sizeof(lua_Integer)
  * 8: LNUM_INT64: sizeof(lua_Integer)
  * +0x80: LNUM_COMPLEX
  */
 *h++ = (char)( sizeof(lua_Integer)
#ifdef LNUM_COMPLEX
    | 0x80
#endif
    );
}
