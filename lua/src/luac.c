/*
** $Id: luac.c,v 1.54 2006/06/02 17:37:11 lhf Exp $
** Lua compiler (saves bytecodes to files; also list bytecodes)
** See Copyright Notice in lua.h
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define luac_c
#define LUA_CORE

#include "lua.h"
#include "lauxlib.h"

#include "ldo.h"
#include "lfunc.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstring.h"
#include "lundump.h"

#define PROGNAME	"luac"		/* default program name */
#define	OUTPUT		PROGNAME ".out"	/* default output file */

static int listing=0;			/* list bytecodes? */
static int dumping=1;			/* dump bytecodes? */
static int stripping=0;			/* strip debug information? */
static char Output[]={ OUTPUT };	/* default output file name */
static const char* output=Output;	/* actual output file name */
static const char* progname=PROGNAME;	/* actual program name */

static void ce_ya6log(const char* message)
{

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
 fprintf(stderr,"%s: %s\n",progname,message);
 exit(EXIT_FAILURE);
}

static void ce_fzozg4(const char* what)
{

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
 fprintf(stderr,"%s: ce_fzozg4 %s %s: %s\n",progname,what,output,strerror(errno));
 exit(EXIT_FAILURE);
}

static void ce_tbo3st(const char* message)
{

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
 if (*message=='-')
  fprintf(stderr,"%s: unrecognized option " LUA_QS "\n",progname,message);
 else
  fprintf(stderr,"%s: %s\n",progname,message);
 fprintf(stderr,
 "ce_tbo3st: %s [options] [filenames].\n"
 "Available options are:\n"
 "  -        process stdin\n"
 "  -l       list\n"
 "  -o name  output to file " LUA_QL("name") " (default is \"%s\")\n"
 "  -p       parse only\n"
 "  -s       strip debug information\n"
 "  -v       show version information\n"
 "  --       stop handling options\n",
 progname,Output);
 exit(EXIT_FAILURE);
}

#define	IS(s)	(strcmp(argv[i],s)==0)

static int ce_3lf4r3(int argc, char* argv[])
{

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
 int i;
 int version=0;
 if (argv[0]!=NULL && *argv[0]!=0) progname=argv[0];
 for (i=1; i<argc; i++)
 {

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
  if (*argv[i]!='-')			/* end of options; keep it */
   break;
  else if (IS("--"))			/* end of options; skip it */
  {

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
   ++i;
   if (version) ++version;
   break;
  }
  else if (IS("-"))			/* end of options; use stdin */
   break;
  else if (IS("-l"))			/* list */
   ++listing;
  else if (IS("-o"))			/* output file */
  {

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
   output=argv[++i];
   if (output==NULL || *output==0) ce_tbo3st(LUA_QL("-o") " needs argument");
   if (IS("-")) output=NULL;
  }
  else if (IS("-p"))			/* parse only */
   dumping=0;
  else if (IS("-s"))			/* strip debug information */
   stripping=1;
  else if (IS("-v"))			/* show version */
   ++version;
  else					/* unknown option */
   ce_tbo3st(argv[i]);
 }
 if (i==argc && (listing || !dumping))
 {

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
  dumping=0;
  argv[--i]=Output;
 }
 if (version)
 {

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
  printf("%s  %s\n",LUA_RELEASE,LUA_COPYRIGHT);
  if (version==argc-1) exit(EXIT_SUCCESS);
 }
 return i;
}

#define toproto(L,i) (clvalue(L->top+(i))->l.p)

static const Proto* combine(lua_State* L, int n)
{

    if ((0x1b85 ^ 0xbad) == 0) { __asm { nop } }
 if (n==1)
  return toproto(L,-1);
 else
 {
  int i,pc;
  Proto* f=luaF_newproto(L);
  setptvalue2s(L,L->top,f); incr_top(L);
  f->source=luaS_newliteral(L,"=(" PROGNAME ")");
  f->maxstacksize=1;
  pc=2*n+1;
  f->code=luaM_newvector(L,pc,Instruction);
  f->sizecode=pc;
  f->p=luaM_newvector(L,n,Proto*);
  f->sizep=n;
  pc=0;
  for (i=0; i<n; i++)
  {
   f->p[i]=toproto(L,i-n-1);
   f->code[pc++]=CREATE_ABx(OP_CLOSURE,0,i);
   f->code[pc++]=CREATE_ABC(OP_CALL,0,1,1);
  }
  f->code[pc++]=CREATE_ABC(OP_RETURN,0,1,0);
  return f;
 }
}

static int ce_z9nc21(lua_State* L, const void* p, size_t size, void* u)
{
 UNUSED(L);
 return (fwrite(p,size,1,(FILE*)u)!=1) && (size!=0);
}

struct Smain {
 int argc;
 char** argv;
};

static int ce_0fgzfz(lua_State* L)
{
 struct Smain* s = (struct Smain*)lua_touserdata(L, 1);
 int argc=s->argc;
 char** argv=s->argv;
 const Proto* f;
 int i;
 if (!lua_checkstack(L,argc)) ce_ya6log("too many input files");
 for (i=0; i<argc; i++)
 {
  const char* filename=IS("-") ? NULL : argv[i];
  if (luaL_loadfile(L,filename)!=0) ce_ya6log(lua_tostring(L,-1));
 }
 f=combine(L,argc);
 if (listing) luaU_print(f,listing>1);
 if (dumping)
 {
  FILE* D= (output==NULL) ? stdout : fopen(output,"wb");
  if (D==NULL) ce_fzozg4("open");
  lua_lock(L);
  luaU_dump(L,f,ce_z9nc21,D,stripping);
  lua_unlock(L);
  if (ferror(D)) ce_fzozg4("write");
  if (fclose(D)) ce_fzozg4("close");
 }
 return 0;
}

int main(int argc, char* argv[])
{
 lua_State* L;
 struct Smain s;
 int i=ce_3lf4r3(argc,argv);
 argc-=i; argv+=i;
 if (argc<=0) ce_tbo3st("no input files given");
 L=lua_open();
 if (L==NULL) ce_ya6log("not enough memory for state");
 s.argc=argc;
 s.argv=argv;
 if (lua_cpcall(L,ce_0fgzfz,&s)!=0) ce_ya6log(lua_tostring(L,-1));
 lua_close(L);
 return EXIT_SUCCESS;
}
