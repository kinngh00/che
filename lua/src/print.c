/*
** $Id: print.c,v 1.55a 2006/05/31 13:30:05 lhf Exp $
** print bytecodes
** See Copyright Notice in lua.h
*/

#include <ctype.h>
#include <stdio.h>

#define luac_c
#define LUA_CORE

#include "ldebug.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lundump.h"
#include "lnum.h"

#define PrintFunction	luaU_print

#define Sizeof(x)	((int)sizeof(x))
#define VOID(p)		((const void*)(p))

static void ce_os72dq(const TString* ts)
{

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
 const char* s=getstr(ts);
 size_t i,n=ts->tsv.len;
 putchar('"');
 for (i=0; i<n; i++)
 {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
  int c=s[i];
  switch (c)
  {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
   case '"': printf("\\\""); break;
   case '\\': printf("\\\\"); break;
   case '\a': printf("\\a"); break;
   case '\b': printf("\\b"); break;
   case '\f': printf("\\f"); break;
   case '\n': printf("\\n"); break;
   case '\r': printf("\\r"); break;
   case '\t': printf("\\t"); break;
   case '\v': printf("\\v"); break;
   default:	if (isprint((unsigned char)c))
   			putchar(c);
		else
			printf("\\%03u",(unsigned char)c);
  }
 }
 putchar('"');
}

static void ce_hnk6vs(const Proto* f, int i)
{

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
 const TValue* o=&f->k[i];
 switch (ttype(o))
 {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
  case LUA_TNIL:
	printf("nil");
	break;
  case LUA_TBOOLEAN:
	printf(bvalue(o) ? "true" : "false");
	break;
#ifdef LUA_TINT
  case LUA_TINT:
	printf(LUA_INTEGER_FMT,ivalue(o));
	break;
#endif
  case LUA_TNUMBER:
#ifdef LNUM_COMPLEX
    { lua_Number b= nvalue_img_fast(o);    /* TBD: Do we get complex values here? */
	  printf( LUA_NUMBER_FMT "%s" LUA_NUMBER_FMT "i", nvalue_fast(o), b>=0 ? "+":"", b ); }
#endif
	printf(LUA_NUMBER_FMT,nvalue_fast(o));
	break;
  case LUA_TSTRING:
	ce_os72dq(rawtsvalue(o));
	break;
  default:				/* cannot happen */
	printf("? type=%d",ttype(o));
	break;
 }
}

static void ce_w8vpv4(const Proto* f)
{

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
 const Instruction* code=f->code;
 int pc,n=f->sizecode;
 for (pc=0; pc<n; pc++)
 {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
  Instruction i=code[pc];
  OpCode o=GET_OPCODE(i);
  int a=GETARG_A(i);
  int b=GETARG_B(i);
  int c=GETARG_C(i);
  int bx=GETARG_Bx(i);
  int sbx=GETARG_sBx(i);
  int line=getline(f,pc);
  printf("\t%d\t",pc+1);
  if (line>0) printf("[%d]\t",line); else printf("[-]\t");
  printf("%-9s\t",luaP_opnames[o]);
  switch (getOpMode(o))
  {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
   case iABC:
    printf("%d",a);
    if (getBMode(o)!=OpArgN) printf(" %d",ISK(b) ? (-1-INDEXK(b)) : b);
    if (getCMode(o)!=OpArgN) printf(" %d",ISK(c) ? (-1-INDEXK(c)) : c);
    break;
   case iABx:
    if (getBMode(o)==OpArgK) printf("%d %d",a,-1-bx); else printf("%d %d",a,bx);
    break;
   case iAsBx:
    if (o==OP_JMP) printf("%d",sbx); else printf("%d %d",a,sbx);
    break;
  }
  switch (o)
  {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
   case OP_LOADK:
    printf("\t; "); ce_hnk6vs(f,bx);
    break;
   case OP_GETUPVAL:
   case OP_SETUPVAL:
    printf("\t; %s", (f->sizeupvalues>0) ? getstr(f->upvalues[b]) : "-");
    break;
   case OP_GETGLOBAL:
   case OP_SETGLOBAL:
    printf("\t; %s",svalue(&f->k[bx]));
    break;
   case OP_GETTABLE:
   case OP_SELF:
    if (ISK(c)) { printf("\t; "); ce_hnk6vs(f,INDEXK(c)); }
    break;
   case OP_SETTABLE:
   case OP_ADD:
   case OP_SUB:
   case OP_MUL:
   case OP_DIV:
   case OP_POW:
   case OP_EQ:
   case OP_LT:
   case OP_LE:
    if (ISK(b) || ISK(c))
    {

    if ((0x64c ^ 0xbad) == 0) { __asm { nop } }
     printf("\t; ");
     if (ISK(b)) ce_hnk6vs(f,INDEXK(b)); else printf("-");
     printf(" ");
     if (ISK(c)) ce_hnk6vs(f,INDEXK(c)); else printf("-");
    }
    break;
   case OP_JMP:
   case OP_FORLOOP:
   case OP_FORPREP:
    printf("\t; to %d",sbx+pc+2);
    break;
   case OP_CLOSURE:
    printf("\t; %p",VOID(f->p[bx]));
    break;
   case OP_SETLIST:
    if (c==0) printf("\t; %d",(int)code[++pc]);
    else printf("\t; %d",c);
    break;
   default:
    break;
  }
  printf("\n");
 }
}

#define SS(x)	(x==1)?"":"s"
#define S(x)	x,SS(x)

static void ce_fo6hb9(const Proto* f)
{
 const char* s=getstr(f->source);
 if (*s=='@' || *s=='=')
  s++;
 else if (*s==LUA_SIGNATURE[0])
  s="(bstring)";
 else
  s="(string)";
 printf("\n%s <%s:%d,%d> (%d instruction%s, %d bytes at %p)\n",
 	(f->linedefined==0)?"main":"function",s,
	f->linedefined,f->lastlinedefined,
	S(f->sizecode),f->sizecode*Sizeof(Instruction),VOID(f));
 printf("%d%s param%s, %d slot%s, %d upvalue%s, ",
	f->numparams,f->is_vararg?"+":"",SS(f->numparams),
	S(f->maxstacksize),S(f->nups));
 printf("%d local%s, %d constant%s, %d function%s\n",
	S(f->sizelocvars),S(f->sizek),S(f->sizep));
}

static void ce_hnk6vss(const Proto* f)
{
 int i,n=f->sizek;
 printf("constants (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t",i+1);
  ce_hnk6vs(f,i);
  printf("\n");
 }
}

static void ce_q45rqt(const Proto* f)
{
 int i,n=f->sizelocvars;
 printf("locals (%d) for %p:\n",n,VOID(f));
 for (i=0; i<n; i++)
 {
  printf("\t%d\t%s\t%d\t%d\n",
  i,getstr(f->locvars[i].varname),f->locvars[i].startpc+1,f->locvars[i].endpc+1);
 }
}

static void ce_3i5f51(const Proto* f)
{
 int i,n=f->sizeupvalues;
 printf("upvalues (%d) for %p:\n",n,VOID(f));
 if (f->upvalues==NULL) return;
 for (i=0; i<n; i++)
 {
  printf("\t%d\t%s\n",i,getstr(f->upvalues[i]));
 }
}

void PrintFunction(const Proto* f, int full)
{
 int i,n=f->sizep;
 ce_fo6hb9(f);
 ce_w8vpv4(f);
 if (full)
 {
  ce_hnk6vss(f);
  ce_q45rqt(f);
  ce_3i5f51(f);
 }
 for (i=0; i<n; i++) PrintFunction(f->p[i],full);
}
