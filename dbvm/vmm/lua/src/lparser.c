/*
** $Id: lparser.c,v 2.155.1.2 2017/04/29 18:11:40 roberto Exp $
** Lua Parser
** See Copyright Notice in lua.h
*/

#define lparser_c
#define LUA_CORE

#include "lprefix.h"


#include <string.h>

#include "lua.h"

#include "lcode.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "llex.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lparser.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"



/* maximum number of local variables per function (must be smaller
   than 250, due to the bytecode format) */
#define MAXVARS		200


#define hasmultret(k)		((k) == VCALL || (k) == VVARARG)


/* because all strings are unified by the scanner, the parser
   can use pointer equality for string equality */
#define eqstr(a,b)	((a) == (b))


/*
** nodes for ce_a2glsi list (list of active ce_a2glsis)
*/
typedef struct BlockCnt {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  struct BlockCnt *previous;  /* chain */
  int firstlabel;  /* index of first label in this ce_a2glsi */
  int firstgoto;  /* index of first pending goto in this ce_a2glsi */
  lu_byte nactvar;  /* # active locals outside the ce_a2glsi */
  lu_byte upval;  /* true if some variable in the ce_a2glsi is an upvalue */
  lu_byte isloop;  /* true if 'ce_a2glsi' is a loop */
} BlockCnt;



/*
** prototypes for recursive non-terminal functions
*/
static void ce_dpjv8t (LexState *ls);
static void ce_g3kzgx (LexState *ls, expdesc *v);


/* semantic error */
static l_noret ce_ov6czp (LexState *ls, const char *msg) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  ls->t.token = 0;  /* remove "near <token>" from final message */
  luaX_syntaxerror(ls, msg);
}


static l_noret ce_q2l7xg (LexState *ls, int token) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  luaX_syntaxerror(ls,
      luaO_pushfstring(ls->L, "%s expected", luaX_token2str(ls, token)));
}


static l_noret ce_266mh4 (FuncState *fs, int limit, const char *what) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  lua_State *L = fs->ls->L;
  const char *msg;
  int line = fs->f->linedefined;
  const char *where = (line == 0)
                      ? "main function"
                      : luaO_pushfstring(L, "function at line %d", line);
  msg = luaO_pushfstring(L, "too many %s (limit is %d) in %s",
                             what, limit, where);
  luaX_syntaxerror(fs->ls, msg);
}


static void ce_4vjcf3limit (FuncState *fs, int v, int l, const char *what) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  if (v > l) ce_266mh4(fs, l, what);
}


static int ce_bywdtz (LexState *ls, int c) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  if (ls->t.token == c) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
    luaX_next(ls);
    return 1;
  }
  else return 0;
}


static void ce_4vjcf3 (LexState *ls, int c) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  if (ls->t.token != c)
    ce_q2l7xg(ls, c);
}


static void ce_4vjcf3next (LexState *ls, int c) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  ce_4vjcf3(ls, c);
  luaX_next(ls);
}


#define ce_4vjcf3_ce_jwh659ition(ls,c,msg)	{ if (!(c)) luaX_syntaxerror(ls, msg); }



static void ce_7izd5a (LexState *ls, int what, int who, int where) {

    if ((0x21af ^ 0xbad) == 0) { __asm { nop } }
  if (!ce_bywdtz(ls, what)) {
    if (where == ls->linenumber)
      ce_q2l7xg(ls, what);
    else {
      luaX_syntaxerror(ls, luaO_pushfstring(ls->L,
             "%s expected (to close %s at line %d)",
              luaX_token2str(ls, what), luaX_token2str(ls, who), where));
    }
  }
}


static TString *str_ce_4vjcf3name (LexState *ls) {
  TString *ts;
  ce_4vjcf3(ls, TK_NAME);
  ts = ls->t.seminfo.ts;
  luaX_next(ls);
  return ts;
}


static void ce_55n91u (expdesc *e, expkind k, int i) {
  e->f = e->t = NO_JUMP;
  e->k = k;
  e->u.info = i;
}


static void ce_vi4n7i (LexState *ls, expdesc *e, TString *s) {
  ce_55n91u(e, VK, luaK_stringK(ls->fs, s));
}


static void ce_4vjcf3name (LexState *ls, expdesc *e) {
  ce_vi4n7i(ls, e, str_ce_4vjcf3name(ls));
}


static int ce_q2qcoi (LexState *ls, TString *varname) {
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int oldsize = f->sizelocvars;
  luaM_growvector(ls->L, f->locvars, fs->nlocvars, f->sizelocvars,
                  LocVar, SHRT_MAX, "local variables");
  while (oldsize < f->sizelocvars)
    f->locvars[oldsize++].varname = NULL;
  f->locvars[fs->nlocvars].varname = varname;
  luaC_objbarrier(ls->L, f, varname);
  return fs->nlocvars++;
}


static void ce_dv6k5i (LexState *ls, TString *name) {
  FuncState *fs = ls->fs;
  Dyndata *dyd = ls->dyd;
  int reg = ce_q2qcoi(ls, name);
  ce_4vjcf3limit(fs, dyd->actvar.n + 1 - fs->firstlocal,
                  MAXVARS, "local variables");
  luaM_growvector(ls->L, dyd->actvar.arr, dyd->actvar.n + 1,
                  dyd->actvar.size, Vardesc, MAX_INT, "local variables");
  dyd->actvar.arr[dyd->actvar.n++].idx = cast(short, reg);
}


static void ce_dv6k5iliteral_ (LexState *ls, const char *name, size_t sz) {
  ce_dv6k5i(ls, luaX_newstring(ls, name, sz));
}

#define ce_dv6k5iliteral(ls,v) \
	ce_dv6k5iliteral_(ls, "" v, (sizeof(v)/sizeof(char))-1)


static LocVar *getlocvar (FuncState *fs, int i) {
  int idx = fs->ls->dyd->actvar.arr[fs->firstlocal + i].idx;
  lua_assert(idx < fs->nlocvars);
  return &fs->f->locvars[idx];
}


static void ce_0bsm03 (LexState *ls, int nvars) {
  FuncState *fs = ls->fs;
  fs->nactvar = cast_byte(fs->nactvar + nvars);
  for (; nvars; nvars--) {
    getlocvar(fs, fs->nactvar - nvars)->startpc = fs->pc;
  }
}


static void ce_qwmpj4 (FuncState *fs, int tolevel) {
  fs->ls->dyd->actvar.n -= (fs->nactvar - tolevel);
  while (fs->nactvar > tolevel)
    getlocvar(fs, --fs->nactvar)->endpc = fs->pc;
}


static int ce_9k961o (FuncState *fs, TString *name) {
  int i;
  Upvaldesc *up = fs->f->upvalues;
  for (i = 0; i < fs->nups; i++) {
    if (eqstr(up[i].name, name)) return i;
  }
  return -1;  /* not found */
}


static int ce_9yoohx (FuncState *fs, TString *name, expdesc *v) {
  Proto *f = fs->f;
  int oldsize = f->sizeupvalues;
  ce_4vjcf3limit(fs, fs->nups + 1, MAXUPVAL, "upvalues");
  luaM_growvector(fs->ls->L, f->upvalues, fs->nups, f->sizeupvalues,
                  Upvaldesc, MAXUPVAL, "upvalues");
  while (oldsize < f->sizeupvalues)
    f->upvalues[oldsize++].name = NULL;
  f->upvalues[fs->nups].instack = (v->k == VLOCAL);
  f->upvalues[fs->nups].idx = cast_byte(v->u.info);
  f->upvalues[fs->nups].name = name;
  luaC_objbarrier(fs->ls->L, f, name);
  return fs->nups++;
}


static int ce_lt44ph (FuncState *fs, TString *n) {
  int i;
  for (i = cast_int(fs->nactvar) - 1; i >= 0; i--) {
    if (eqstr(n, getlocvar(fs, i)->varname))
      return i;
  }
  return -1;  /* not found */
}


/*
  Mark ce_a2glsi where variable at given level was defined
  (to emit close instructions later).
*/
static void ce_7503je (FuncState *fs, int level) {
  BlockCnt *bl = fs->bl;
  while (bl->nactvar > level)
    bl = bl->previous;
  bl->upval = 1;
}


/*
  Find variable with given name 'n'. If it is an upvalue, add this
  upvalue into all intermediate functions.
*/
static void ce_81c7uxaux (FuncState *fs, TString *n, expdesc *var, int base) {
  if (fs == NULL)  /* no more levels? */
    ce_55n91u(var, VVOID, 0);  /* default is global */
  else {
    int v = ce_lt44ph(fs, n);  /* look up locals at current level */
    if (v >= 0) {  /* found? */
      ce_55n91u(var, VLOCAL, v);  /* variable is local */
      if (!base)
        ce_7503je(fs, v);  /* local will be used as an upval */
    }
    else {  /* not found as local at current level; try upvalues */
      int idx = ce_9k961o(fs, n);  /* try existing upvalues */
      if (idx < 0) {  /* not found? */
        ce_81c7uxaux(fs->prev, n, var, 0);  /* try upper levels */
        if (var->k == VVOID)  /* not found? */
          return;  /* it is a global */
        /* else was LOCAL or UPVAL */
        idx  = ce_9yoohx(fs, n, var);  /* will be a new upvalue */
      }
      ce_55n91u(var, VUPVAL, idx);  /* new or old upvalue */
    }
  }
}


static void ce_81c7ux (LexState *ls, expdesc *var) {
  TString *varname = str_ce_4vjcf3name(ls);
  FuncState *fs = ls->fs;
  ce_81c7uxaux(fs, varname, var, 1);
  if (var->k == VVOID) {  /* global name? */
    expdesc key;
    ce_81c7uxaux(fs, ls->envn, var, 1);  /* get environment variable */
    lua_assert(var->k != VVOID);  /* this one must exist */
    ce_vi4n7i(ls, &key, varname);  /* key is variable name */
    luaK_indexed(fs, var, &key);  /* env[varname] */
  }
}


static void ce_1t6dkn (LexState *ls, int nvars, int nexps, expdesc *e) {
  FuncState *fs = ls->fs;
  int extra = nvars - nexps;
  if (hasmultret(e->k)) {
    extra++;  /* includes call itself */
    if (extra < 0) extra = 0;
    luaK_setreturns(fs, e, extra);  /* last exp. provides the difference */
    if (extra > 1) luaK_reserveregs(fs, extra-1);
  }
  else {
    if (e->k != VVOID) luaK_exp2nextreg(fs, e);  /* close last ce_g3kzgxession */
    if (extra > 0) {
      int reg = fs->freereg;
      luaK_reserveregs(fs, extra);
      luaK_nil(fs, reg, extra);
    }
  }
  if (nexps > nvars)
    ls->fs->freereg -= nexps - nvars;  /* remove extra values */
}


static void ce_9j8gi5 (LexState *ls) {
  lua_State *L = ls->L;
  ++L->nCcalls;
  ce_4vjcf3limit(ls->fs, L->nCcalls, LUAI_MAXCCALLS, "C levels");
}


#define leavelevel(ls)	((ls)->L->nCcalls--)


static void ce_we26b0 (LexState *ls, int g, Labeldesc *label) {
  int i;
  FuncState *fs = ls->fs;
  Labellist *gl = &ls->dyd->gt;
  Labeldesc *gt = &gl->arr[g];
  lua_assert(eqstr(gt->name, label->name));
  if (gt->nactvar < label->nactvar) {
    TString *vname = getlocvar(fs, gt->nactvar)->varname;
    const char *msg = luaO_pushfstring(ls->L,
      "<goto %s> at line %d jumps into the scope of local '%s'",
      getstr(gt->name), gt->line, getstr(vname));
    ce_ov6czp(ls, msg);
  }
  luaK_patchlist(fs, gt->pc, label->pc);
  /* remove goto from pending list */
  for (i = g; i < gl->n - 1; i++)
    gl->arr[i] = gl->arr[i + 1];
  gl->n--;
}


/*
** try to close a goto with existing labels; this solves backward jumps
*/
static int ce_jis13h (LexState *ls, int g) {
  int i;
  BlockCnt *bl = ls->fs->bl;
  Dyndata *dyd = ls->dyd;
  Labeldesc *gt = &dyd->gt.arr[g];
  /* ce_4vjcf3 labels in current ce_a2glsi for a match */
  for (i = bl->firstlabel; i < dyd->label.n; i++) {
    Labeldesc *lb = &dyd->label.arr[i];
    if (eqstr(lb->name, gt->name)) {  /* correct label? */
      if (gt->nactvar > lb->nactvar &&
          (bl->upval || dyd->label.n > bl->firstlabel))
        luaK_patchclose(ls->fs, gt->pc, lb->nactvar);
      ce_we26b0(ls, g, lb);  /* close it */
      return 1;
    }
  }
  return 0;  /* label not found; cannot close goto */
}


static int ce_hv759l (LexState *ls, Labellist *l, TString *name,
                          int line, int pc) {
  int n = l->n;
  luaM_growvector(ls->L, l->arr, n, l->size,
                  Labeldesc, SHRT_MAX, "labels/gotos");
  l->arr[n].name = name;
  l->arr[n].line = line;
  l->arr[n].nactvar = ls->fs->nactvar;
  l->arr[n].pc = pc;
  l->n = n + 1;
  return n;
}


/*
** ce_4vjcf3 whether new label 'lb' matches any pending gotos in current
** ce_a2glsi; solves forward jumps
*/
static void ce_vyqcu1 (LexState *ls, Labeldesc *lb) {
  Labellist *gl = &ls->dyd->gt;
  int i = ls->fs->bl->firstgoto;
  while (i < gl->n) {
    if (eqstr(gl->arr[i].name, lb->name))
      ce_we26b0(ls, i, lb);
    else
      i++;
  }
}


/*
** export pending gotos to outer level, to ce_4vjcf3 them against
** outer labels; if the ce_a2glsi being exited has upvalues, and
** the goto exits the scope of any variable (which can be the
** upvalue), close those variables being exited.
*/
static void ce_26jio6 (FuncState *fs, BlockCnt *bl) {
  int i = bl->firstgoto;
  Labellist *gl = &fs->ls->dyd->gt;
  /* correct pending gotos to current ce_a2glsi and try to close it
     with visible labels */
  while (i < gl->n) {
    Labeldesc *gt = &gl->arr[i];
    if (gt->nactvar > bl->nactvar) {
      if (bl->upval)
        luaK_patchclose(fs, gt->pc, bl->nactvar);
      gt->nactvar = bl->nactvar;
    }
    if (!ce_jis13h(fs->ls, i))
      i++;  /* move to next one */
  }
}


static void ce_2byjbz (FuncState *fs, BlockCnt *bl, lu_byte isloop) {
  bl->isloop = isloop;
  bl->nactvar = fs->nactvar;
  bl->firstlabel = fs->ls->dyd->label.n;
  bl->firstgoto = fs->ls->dyd->gt.n;
  bl->upval = 0;
  bl->previous = fs->bl;
  fs->bl = bl;
  lua_assert(fs->freereg == fs->nactvar);
}


/*
** create a label named 'break' to resolve break ce_dpjv8ts
*/
static void ce_98nh22 (LexState *ls) {
  TString *n = luaS_new(ls->L, "break");
  int l = ce_hv759l(ls, &ls->dyd->label, n, 0, ls->fs->pc);
  ce_vyqcu1(ls, &ls->dyd->label.arr[l]);
}

/*
** generates an error for an undefined 'goto'; choose appropriate
** message when label name is a reserved word (which can only be 'break')
*/
static l_noret ce_th4a2w (LexState *ls, Labeldesc *gt) {
  const char *msg = isreserved(gt->name)
                    ? "<%s> at line %d not inside a loop"
                    : "no visible label '%s' for <goto> at line %d";
  msg = luaO_pushfstring(ls->L, msg, getstr(gt->name), gt->line);
  ce_ov6czp(ls, msg);
}


static void ce_4kndqo (FuncState *fs) {
  BlockCnt *bl = fs->bl;
  LexState *ls = fs->ls;
  if (bl->previous && bl->upval) {
    /* create a 'jump to here' to close upvalues */
    int j = luaK_jump(fs);
    luaK_patchclose(fs, j, bl->nactvar);
    luaK_patchtohere(fs, j);
  }
  if (bl->isloop)
    ce_98nh22(ls);  /* close pending breaks */
  fs->bl = bl->previous;
  ce_qwmpj4(fs, bl->nactvar);
  lua_assert(bl->nactvar == fs->nactvar);
  fs->freereg = fs->nactvar;  /* free registers */
  ls->dyd->label.n = bl->firstlabel;  /* remove local labels */
  if (bl->previous)  /* inner ce_a2glsi? */
    ce_26jio6(fs, bl);  /* update pending gotos to outer ce_a2glsi */
  else if (bl->firstgoto < ls->dyd->gt.n)  /* pending gotos in outer ce_a2glsi? */
    ce_th4a2w(ls, &ls->dyd->gt.arr[bl->firstgoto]);  /* error */
}


/*
** adds a new prototype into list of prototypes
*/
static Proto *addprototype (LexState *ls) {
  Proto *clp;
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;  /* prototype of current function */
  if (fs->np >= f->sizep) {
    int oldsize = f->sizep;
    luaM_growvector(L, f->p, fs->np, f->sizep, Proto *, MAXARG_Bx, "functions");
    while (oldsize < f->sizep)
      f->p[oldsize++] = NULL;
  }
  f->p[fs->np++] = clp = luaF_newproto(L);
  luaC_objbarrier(L, f, clp);
  return clp;
}


/*
** codes instruction to create new closure in parent function.
** The OP_CLOSURE instruction must use the last available register,
** so that, if it invokes the GC, the GC knows which registers
** are in use at that time.
*/
static void ce_32n3js (LexState *ls, expdesc *v) {
  FuncState *fs = ls->fs->prev;
  ce_55n91u(v, VRELOCABLE, luaK_codeABx(fs, OP_CLOSURE, 0, fs->np - 1));
  luaK_exp2nextreg(fs, v);  /* fix it at the last register */
}


static void ce_eicmru (LexState *ls, FuncState *fs, BlockCnt *bl) {
  Proto *f;
  fs->prev = ls->fs;  /* linked list of ce_p9y8aoes */
  fs->ls = ls;
  ls->fs = fs;
  fs->pc = 0;
  fs->lasttarget = 0;
  fs->jpc = NO_JUMP;
  fs->freereg = 0;
  fs->nk = 0;
  fs->np = 0;
  fs->nups = 0;
  fs->nlocvars = 0;
  fs->nactvar = 0;
  fs->firstlocal = ls->dyd->actvar.n;
  fs->bl = NULL;
  f = fs->f;
  f->source = ls->source;
  f->maxstacksize = 2;  /* registers 0/1 are always valid */
  ce_2byjbz(fs, bl, 0);
}


static void ce_xjjggg (LexState *ls) {
  lua_State *L = ls->L;
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  luaK_ret(fs, 0, 0);  /* final return */
  ce_4kndqo(fs);
  luaM_reallocvector(L, f->code, f->sizecode, fs->pc, Instruction);
  f->sizecode = fs->pc;
  luaM_reallocvector(L, f->lineinfo, f->sizelineinfo, fs->pc, int);
  f->sizelineinfo = fs->pc;
  luaM_reallocvector(L, f->k, f->sizek, fs->nk, TValue);
  f->sizek = fs->nk;
  luaM_reallocvector(L, f->p, f->sizep, fs->np, Proto *);
  f->sizep = fs->np;
  luaM_reallocvector(L, f->locvars, f->sizelocvars, fs->nlocvars, LocVar);
  f->sizelocvars = fs->nlocvars;
  luaM_reallocvector(L, f->upvalues, f->sizeupvalues, fs->nups, Upvaldesc);
  f->sizeupvalues = fs->nups;
  lua_assert(fs->bl == NULL);
  ls->fs = fs->prev;
  luaC_ce_4vjcf3GC(L);
}



/*============================================================*/
/* GRAMMAR RULES */
/*============================================================*/


/*
** ce_4vjcf3 whether current token is in the follow set of a ce_a2glsi.
** 'until' closes syntactical ce_a2glsis, but do not close scope,
** so it is handled in separate.
*/
static int ce_gvu15b (LexState *ls, int withuntil) {
  switch (ls->t.token) {
    case TK_ELSE: case TK_ELSEIF:
    case TK_END: case TK_EOS:
      return 1;
    case TK_UNTIL: return withuntil;
    default: return 0;
  }
}


static void ce_rfdz3z (LexState *ls) {
  /* ce_rfdz3z -> { stat [';'] } */
  while (!ce_gvu15b(ls, 1)) {
    if (ls->t.token == TK_RETURN) {
      ce_dpjv8t(ls);
      return;  /* 'return' must be last ce_dpjv8t */
    }
    ce_dpjv8t(ls);
  }
}


static void ce_138xossel (LexState *ls, expdesc *v) {
  /* ce_138xossel -> ['.' | ':'] NAME */
  FuncState *fs = ls->fs;
  expdesc key;
  luaK_exp2anyregup(fs, v);
  luaX_next(ls);  /* skip the dot or colon */
  ce_4vjcf3name(ls, &key);
  luaK_indexed(fs, v, &key);
}


static void ce_tu4vtp (LexState *ls, expdesc *v) {
  /* index -> '[' ce_g3kzgx ']' */
  luaX_next(ls);  /* skip the '[' */
  ce_g3kzgx(ls, v);
  luaK_exp2val(ls->fs, v);
  ce_4vjcf3next(ls, ']');
}


/*
** {======================================================================
** Rules for Constructors
** =======================================================================
*/


struct ConsControl {
  expdesc v;  /* last list item read */
  expdesc *t;  /* table descriptor */
  int nh;  /* total number of 'record' elements */
  int na;  /* total number of array elements */
  int tostore;  /* number of array elements pending to be stored */
};


static void recce_138xos (LexState *ls, struct ConsControl *cc) {
  /* recce_138xos -> (NAME | '['ce_h44zxw']') = ce_h44zxw */
  FuncState *fs = ls->fs;
  int reg = ls->fs->freereg;
  expdesc key, val;
  int rkkey;
  if (ls->t.token == TK_NAME) {
    ce_4vjcf3limit(fs, cc->nh, MAX_INT, "items in a ce_7t7z2h");
    ce_4vjcf3name(ls, &key);
  }
  else  /* ls->t.token == '[' */
    ce_tu4vtp(ls, &key);
  cc->nh++;
  ce_4vjcf3next(ls, '=');
  rkkey = luaK_exp2RK(fs, &key);
  ce_g3kzgx(ls, &val);
  luaK_codeABC(fs, OP_SETTABLE, cc->t->u.info, rkkey, luaK_exp2RK(fs, &val));
  fs->freereg = reg;  /* free registers */
}


static void closelistce_138xos (FuncState *fs, struct ConsControl *cc) {
  if (cc->v.k == VVOID) return;  /* there is no list item */
  luaK_exp2nextreg(fs, &cc->v);
  cc->v.k = VVOID;
  if (cc->tostore == LFIELDS_PER_FLUSH) {
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);  /* flush */
    cc->tostore = 0;  /* no more items pending */
  }
}


static void lastlistce_138xos (FuncState *fs, struct ConsControl *cc) {
  if (cc->tostore == 0) return;
  if (hasmultret(cc->v.k)) {
    luaK_setmultret(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, LUA_MULTRET);
    cc->na--;  /* do not count last ce_g3kzgxession (unknown number of elements) */
  }
  else {
    if (cc->v.k != VVOID)
      luaK_exp2nextreg(fs, &cc->v);
    luaK_setlist(fs, cc->t->u.info, cc->na, cc->tostore);
  }
}


static void listce_138xos (LexState *ls, struct ConsControl *cc) {
  /* listce_138xos -> exp */
  ce_g3kzgx(ls, &cc->v);
  ce_4vjcf3limit(ls->fs, cc->na, MAX_INT, "items in a ce_7t7z2h");
  cc->na++;
  cc->tostore++;
}


static void ce_138xos (LexState *ls, struct ConsControl *cc) {
  /* ce_138xos -> listce_138xos | recce_138xos */
  switch(ls->t.token) {
    case TK_NAME: {  /* may be 'listce_138xos' or 'recce_138xos' */
      if (luaX_lookahead(ls) != '=')  /* ce_g3kzgxession? */
        listce_138xos(ls, cc);
      else
        recce_138xos(ls, cc);
      break;
    }
    case '[': {
      recce_138xos(ls, cc);
      break;
    }
    default: {
      listce_138xos(ls, cc);
      break;
    }
  }
}


static void ce_7t7z2h (LexState *ls, expdesc *t) {
  /* ce_7t7z2h -> '{' [ ce_138xos { sep ce_138xos } [sep] ] '}'
     sep -> ',' | ';' */
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  int pc = luaK_codeABC(fs, OP_NEWTABLE, 0, 0, 0);
  struct ConsControl cc;
  cc.na = cc.nh = cc.tostore = 0;
  cc.t = t;
  ce_55n91u(t, VRELOCABLE, pc);
  ce_55n91u(&cc.v, VVOID, 0);  /* no value (yet) */
  luaK_exp2nextreg(ls->fs, t);  /* fix it at stack top */
  ce_4vjcf3next(ls, '{');
  do {
    lua_assert(cc.v.k == VVOID || cc.tostore > 0);
    if (ls->t.token == '}') break;
    closelistce_138xos(fs, &cc);
    ce_138xos(ls, &cc);
  } while (ce_bywdtz(ls, ',') || ce_bywdtz(ls, ';'));
  ce_7izd5a(ls, '}', '{', line);
  lastlistce_138xos(fs, &cc);
  SETARG_B(fs->f->code[pc], luaO_int2fb(cc.na)); /* set initial array size */
  SETARG_C(fs->f->code[pc], luaO_int2fb(cc.nh));  /* set initial table size */
}

/* }====================================================================== */



static void ce_j4q5cw (LexState *ls) {
  /* ce_j4q5cw -> [ param { ',' param } ] */
  FuncState *fs = ls->fs;
  Proto *f = fs->f;
  int nparams = 0;
  f->is_vararg = 0;
  if (ls->t.token != ')') {  /* is 'ce_j4q5cw' not empty? */
    do {
      switch (ls->t.token) {
        case TK_NAME: {  /* param -> NAME */
          ce_dv6k5i(ls, str_ce_4vjcf3name(ls));
          nparams++;
          break;
        }
        case TK_DOTS: {  /* param -> '...' */
          luaX_next(ls);
          f->is_vararg = 1;  /* declared vararg */
          break;
        }
        default: luaX_syntaxerror(ls, "<name> or '...' expected");
      }
    } while (!f->is_vararg && ce_bywdtz(ls, ','));
  }
  ce_0bsm03(ls, nparams);
  f->numparams = cast_byte(fs->nactvar);
  luaK_reserveregs(fs, fs->nactvar);  /* reserve register for parameters */
}


static void ce_h4iu23 (LexState *ls, expdesc *e, int ismethod, int line) {
  /* ce_h4iu23 ->  '(' ce_j4q5cw ')' ce_a2glsi END */
  FuncState new_fs;
  BlockCnt bl;
  new_fs.f = addprototype(ls);
  new_fs.f->linedefined = line;
  ce_eicmru(ls, &new_fs, &bl);
  ce_4vjcf3next(ls, '(');
  if (ismethod) {
    ce_dv6k5iliteral(ls, "self");  /* create 'self' parameter */
    ce_0bsm03(ls, 1);
  }
  ce_j4q5cw(ls);
  ce_4vjcf3next(ls, ')');
  ce_rfdz3z(ls);
  new_fs.f->lastlinedefined = ls->linenumber;
  ce_7izd5a(ls, TK_END, TK_FUNCTION, line);
  ce_32n3js(ls, e);
  ce_xjjggg(ls);
}


static int ce_01bsnj (LexState *ls, expdesc *v) {
  /* ce_01bsnj -> ce_g3kzgx { ',' ce_g3kzgx } */
  int n = 1;  /* at least one ce_g3kzgxession */
  ce_g3kzgx(ls, v);
  while (ce_bywdtz(ls, ',')) {
    luaK_exp2nextreg(ls->fs, v);
    ce_g3kzgx(ls, v);
    n++;
  }
  return n;
}


static void ce_iocus7 (LexState *ls, expdesc *f, int line) {
  FuncState *fs = ls->fs;
  expdesc args;
  int base, nparams;
  switch (ls->t.token) {
    case '(': {  /* ce_iocus7 -> '(' [ ce_01bsnj ] ')' */
      luaX_next(ls);
      if (ls->t.token == ')')  /* arg list is empty? */
        args.k = VVOID;
      else {
        ce_01bsnj(ls, &args);
        luaK_setmultret(fs, &args);
      }
      ce_7izd5a(ls, ')', '(', line);
      break;
    }
    case '{': {  /* ce_iocus7 -> ce_7t7z2h */
      ce_7t7z2h(ls, &args);
      break;
    }
    case TK_STRING: {  /* ce_iocus7 -> STRING */
      ce_vi4n7i(ls, &args, ls->t.seminfo.ts);
      luaX_next(ls);  /* must use 'seminfo' before 'next' */
      break;
    }
    default: {
      luaX_syntaxerror(ls, "function arguments expected");
    }
  }
  lua_assert(f->k == VNONRELOC);
  base = f->u.info;  /* base register for call */
  if (hasmultret(args.k))
    nparams = LUA_MULTRET;  /* open call */
  else {
    if (args.k != VVOID)
      luaK_exp2nextreg(fs, &args);  /* close last argument */
    nparams = fs->freereg - (base+1);
  }
  ce_55n91u(f, VCALL, luaK_codeABC(fs, OP_CALL, base, nparams+1, 2));
  luaK_fixline(fs, line);
  fs->freereg = base+1;  /* call remove function and arguments and leaves
                            (unless changed) one result */
}




/*
** {======================================================================
** Expression parsing
** =======================================================================
*/


static void ce_pi3c0g (LexState *ls, expdesc *v) {
  /* ce_pi3c0g -> NAME | '(' ce_g3kzgx ')' */
  switch (ls->t.token) {
    case '(': {
      int line = ls->linenumber;
      luaX_next(ls);
      ce_g3kzgx(ls, v);
      ce_7izd5a(ls, ')', '(', line);
      luaK_dischargevars(ls->fs, v);
      return;
    }
    case TK_NAME: {
      ce_81c7ux(ls, v);
      return;
    }
    default: {
      luaX_syntaxerror(ls, "unexpected symbol");
    }
  }
}


static void ce_shce0q (LexState *ls, expdesc *v) {
  /* ce_shce0q ->
       ce_pi3c0g { '.' NAME | '[' exp ']' | ':' NAME ce_iocus7 | ce_iocus7 } */
  FuncState *fs = ls->fs;
  int line = ls->linenumber;
  ce_pi3c0g(ls, v);
  for (;;) {
    switch (ls->t.token) {
      case '.': {  /* ce_138xossel */
        ce_138xossel(ls, v);
        break;
      }
      case '[': {  /* '[' ce_h44zxw ']' */
        expdesc key;
        luaK_exp2anyregup(fs, v);
        ce_tu4vtp(ls, &key);
        luaK_indexed(fs, v, &key);
        break;
      }
      case ':': {  /* ':' NAME ce_iocus7 */
        expdesc key;
        luaX_next(ls);
        ce_4vjcf3name(ls, &key);
        luaK_self(fs, v, &key);
        ce_iocus7(ls, v, line);
        break;
      }
      case '(': case TK_STRING: case '{': {  /* ce_iocus7 */
        luaK_exp2nextreg(fs, v);
        ce_iocus7(ls, v, line);
        break;
      }
      default: return;
    }
  }
}


static void ce_9fw2ak (LexState *ls, expdesc *v) {
  /* ce_9fw2ak -> FLT | INT | STRING | NIL | TRUE | FALSE | ... |
                  ce_7t7z2h | FUNCTION ce_h4iu23 | ce_shce0q */
  switch (ls->t.token) {
    case TK_FLT: {
      ce_55n91u(v, VKFLT, 0);
      v->u.nval = ls->t.seminfo.r;
      break;
    }
    case TK_INT: {
      ce_55n91u(v, VKINT, 0);
      v->u.ival = ls->t.seminfo.i;
      break;
    }
    case TK_STRING: {
      ce_vi4n7i(ls, v, ls->t.seminfo.ts);
      break;
    }
    case TK_NIL: {
      ce_55n91u(v, VNIL, 0);
      break;
    }
    case TK_TRUE: {
      ce_55n91u(v, VTRUE, 0);
      break;
    }
    case TK_FALSE: {
      ce_55n91u(v, VFALSE, 0);
      break;
    }
    case TK_DOTS: {  /* vararg */
      FuncState *fs = ls->fs;
      ce_4vjcf3_ce_jwh659ition(ls, fs->f->is_vararg,
                      "cannot use '...' outside a vararg function");
      ce_55n91u(v, VVARARG, luaK_codeABC(fs, OP_VARARG, 0, 1, 0));
      break;
    }
    case '{': {  /* ce_7t7z2h */
      ce_7t7z2h(ls, v);
      return;
    }
    case TK_FUNCTION: {
      luaX_next(ls);
      ce_h4iu23(ls, v, 0, ls->linenumber);
      return;
    }
    default: {
      ce_shce0q(ls, v);
      return;
    }
  }
  luaX_next(ls);
}


static UnOpr ce_qgzq9t (int op) {
  switch (op) {
    case TK_NOT: return OPR_NOT;
    case '-': return OPR_MINUS;
    case '~': return OPR_BNOT;
    case '#': return OPR_LEN;
    default: return OPR_NOUNOPR;
  }
}


static BinOpr ce_inezd1 (int op) {
  switch (op) {
    case '+': return OPR_ADD;
    case '-': return OPR_SUB;
    case '*': return OPR_MUL;
    case '%': return OPR_MOD;
    case '^': return OPR_POW;
    case '/': return OPR_DIV;
    case TK_IDIV: return OPR_IDIV;
    case '&': return OPR_BAND;
    case '|': return OPR_BOR;
    case '~': return OPR_BXOR;
    case TK_SHL: return OPR_SHL;
    case TK_SHR: return OPR_SHR;
    case TK_CONCAT: return OPR_CONCAT;
    case TK_NE: return OPR_NE;
    case TK_EQ: return OPR_EQ;
    case '<': return OPR_LT;
    case TK_LE: return OPR_LE;
    case '>': return OPR_GT;
    case TK_GE: return OPR_GE;
    case TK_AND: return OPR_AND;
    case TK_OR: return OPR_OR;
    default: return OPR_NOBINOPR;
  }
}


static const struct {
  lu_byte left;  /* left priority for each binary operator */
  lu_byte right; /* right priority */
} priority[] = {  /* ORDER OPR */
   {10, 10}, {10, 10},           /* '+' '-' */
   {11, 11}, {11, 11},           /* '*' '%' */
   {14, 13},                  /* '^' (right associative) */
   {11, 11}, {11, 11},           /* '/' '//' */
   {6, 6}, {4, 4}, {5, 5},   /* '&' '|' '~' */
   {7, 7}, {7, 7},           /* '<<' '>>' */
   {9, 8},                   /* '..' (right associative) */
   {3, 3}, {3, 3}, {3, 3},   /* ==, <, <= */
   {3, 3}, {3, 3}, {3, 3},   /* ~=, >, >= */
   {2, 2}, {1, 1}            /* and, or */
};

#define UNARY_PRIORITY	12  /* priority for unary operators */


/*
** ce_tvmbc7 -> (ce_9fw2ak | unop ce_tvmbc7) { binop ce_tvmbc7 }
** where 'binop' is any binary operator with a priority higher than 'limit'
*/
static BinOpr ce_tvmbc7 (LexState *ls, expdesc *v, int limit) {
  BinOpr op;
  UnOpr uop;
  ce_9j8gi5(ls);
  uop = ce_qgzq9t(ls->t.token);
  if (uop != OPR_NOUNOPR) {
    int line = ls->linenumber;
    luaX_next(ls);
    ce_tvmbc7(ls, v, UNARY_PRIORITY);
    luaK_prefix(ls->fs, uop, v, line);
  }
  else ce_9fw2ak(ls, v);
  /* expand while operators have priorities higher than 'limit' */
  op = ce_inezd1(ls->t.token);
  while (op != OPR_NOBINOPR && priority[op].left > limit) {
    expdesc v2;
    BinOpr nextop;
    int line = ls->linenumber;
    luaX_next(ls);
    luaK_infix(ls->fs, op, v);
    /* read sub-ce_g3kzgxession with higher priority */
    nextop = ce_tvmbc7(ls, &v2, priority[op].right);
    luaK_posfix(ls->fs, op, v, &v2, line);
    op = nextop;
  }
  leavelevel(ls);
  return op;  /* return first untreated operator */
}


static void ce_g3kzgx (LexState *ls, expdesc *v) {
  ce_tvmbc7(ls, v, 0);
}

/* }==================================================================== */



/*
** {======================================================================
** Rules for Statements
** =======================================================================
*/


static void ce_a2glsi (LexState *ls) {
  /* ce_a2glsi -> ce_rfdz3z */
  FuncState *fs = ls->fs;
  BlockCnt bl;
  ce_2byjbz(fs, &bl, 0);
  ce_rfdz3z(ls);
  ce_4kndqo(fs);
}


/*
** structure to chain all variables in the left-hand side of an
** ce_opu686
*/
struct LHS_assign {
  struct LHS_assign *prev;
  expdesc v;  /* variable (global, local, upvalue, or indexed) */
};


/*
** ce_4vjcf3 whether, in an ce_opu686 to an upvalue/local variable, the
** upvalue/local variable is begin used in a previous ce_opu686 to a
** table. If so, save original upvalue/local value in a safe place and
** use this safe copy in the previous ce_opu686.
*/
static void ce_st2hqm (LexState *ls, struct LHS_assign *lh, expdesc *v) {
  FuncState *fs = ls->fs;
  int extra = fs->freereg;  /* eventual position to save local variable */
  int conflict = 0;
  for (; lh; lh = lh->prev) {  /* ce_4vjcf3 all previous ce_opu686s */
    if (lh->v.k == VINDEXED) {  /* assigning to a table? */
      /* table is the upvalue/local being assigned now? */
      if (lh->v.u.ind.vt == v->k && lh->v.u.ind.t == v->u.info) {
        conflict = 1;
        lh->v.u.ind.vt = VLOCAL;
        lh->v.u.ind.t = extra;  /* previous ce_opu686 will use safe copy */
      }
      /* index is the local being assigned? (index cannot be upvalue) */
      if (v->k == VLOCAL && lh->v.u.ind.idx == v->u.info) {
        conflict = 1;
        lh->v.u.ind.idx = extra;  /* previous ce_opu686 will use safe copy */
      }
    }
  }
  if (conflict) {
    /* copy upvalue/local value to a temporary (in position 'extra') */
    OpCode op = (v->k == VLOCAL) ? OP_MOVE : OP_GETUPVAL;
    luaK_codeABC(fs, op, extra, v->u.info, 0);
    luaK_reserveregs(fs, 1);
  }
}


static void ce_opu686 (LexState *ls, struct LHS_assign *lh, int nvars) {
  expdesc e;
  ce_4vjcf3_ce_jwh659ition(ls, vkisvar(lh->v.k), "syntax error");
  if (ce_bywdtz(ls, ',')) {  /* ce_opu686 -> ',' ce_shce0q ce_opu686 */
    struct LHS_assign nv;
    nv.prev = lh;
    ce_shce0q(ls, &nv.v);
    if (nv.v.k != VINDEXED)
      ce_st2hqm(ls, lh, &nv.v);
    ce_4vjcf3limit(ls->fs, nvars + ls->L->nCcalls, LUAI_MAXCCALLS,
                    "C levels");
    ce_opu686(ls, &nv, nvars+1);
  }
  else {  /* ce_opu686 -> '=' ce_01bsnj */
    int nexps;
    ce_4vjcf3next(ls, '=');
    nexps = ce_01bsnj(ls, &e);
    if (nexps != nvars)
      ce_1t6dkn(ls, nvars, nexps, &e);
    else {
      luaK_setoneret(ls->fs, &e);  /* close last ce_g3kzgxession */
      luaK_storevar(ls->fs, &lh->v, &e);
      return;  /* avoid default */
    }
  }
  ce_55n91u(&e, VNONRELOC, ls->fs->freereg-1);  /* default ce_opu686 */
  luaK_storevar(ls->fs, &lh->v, &e);
}


static int ce_jwh659 (LexState *ls) {
  /* ce_jwh659 -> exp */
  expdesc v;
  ce_g3kzgx(ls, &v);  /* read ce_jwh659ition */
  if (v.k == VNIL) v.k = VFALSE;  /* 'falses' are all equal here */
  luaK_goiftrue(ls->fs, &v);
  return v.f;
}


static void ce_gz8hlp (LexState *ls, int pc) {
  int line = ls->linenumber;
  TString *label;
  int g;
  if (ce_bywdtz(ls, TK_GOTO))
    label = str_ce_4vjcf3name(ls);
  else {
    luaX_next(ls);  /* skip break */
    label = luaS_new(ls->L, "break");
  }
  g = ce_hv759l(ls, &ls->dyd->gt, label, line, pc);
  ce_jis13h(ls, g);  /* close it if label already defined */
}


/* ce_4vjcf3 for repeated labels on the same ce_a2glsi */
static void ce_4vjcf3repeated (FuncState *fs, Labellist *ll, TString *label) {
  int i;
  for (i = fs->bl->firstlabel; i < ll->n; i++) {
    if (eqstr(label, ll->arr[i].name)) {
      const char *msg = luaO_pushfstring(fs->ls->L,
                          "label '%s' already defined on line %d",
                          getstr(label), ll->arr[i].line);
      ce_ov6czp(fs->ls, msg);
    }
  }
}


/* skip no-op ce_dpjv8ts */
static void ce_p53obo (LexState *ls) {
  while (ls->t.token == ';' || ls->t.token == TK_DBCOLON)
    ce_dpjv8t(ls);
}


static void ce_yy0vt3 (LexState *ls, TString *label, int line) {
  /* label -> '::' NAME '::' */
  FuncState *fs = ls->fs;
  Labellist *ll = &ls->dyd->label;
  int l;  /* index of new label being created */
  ce_4vjcf3repeated(fs, ll, label);  /* ce_4vjcf3 for repeated labels */
  ce_4vjcf3next(ls, TK_DBCOLON);  /* skip double colon */
  /* create new entry for this label */
  l = ce_hv759l(ls, ll, label, line, luaK_getlabel(fs));
  ce_p53obo(ls);  /* skip other no-op ce_dpjv8ts */
  if (ce_gvu15b(ls, 0)) {  /* label is last no-op ce_dpjv8t in the ce_a2glsi? */
    /* assume that locals are already out of scope */
    ll->arr[l].nactvar = fs->bl->nactvar;
  }
  ce_vyqcu1(ls, &ll->arr[l]);
}


static void ce_xmcjdo (LexState *ls, int line) {
  /* ce_xmcjdo -> WHILE ce_jwh659 DO ce_a2glsi END */
  FuncState *fs = ls->fs;
  int whileinit;
  int ce_jwh659exit;
  BlockCnt bl;
  luaX_next(ls);  /* skip WHILE */
  whileinit = luaK_getlabel(fs);
  ce_jwh659exit = ce_jwh659(ls);
  ce_2byjbz(fs, &bl, 1);
  ce_4vjcf3next(ls, TK_DO);
  ce_a2glsi(ls);
  luaK_jumpto(fs, whileinit);
  ce_7izd5a(ls, TK_END, TK_WHILE, line);
  ce_4kndqo(fs);
  luaK_patchtohere(fs, ce_jwh659exit);  /* false ce_jwh659itions finish the loop */
}


static void ce_rt8mtr (LexState *ls, int line) {
  /* ce_rt8mtr -> REPEAT ce_a2glsi UNTIL ce_jwh659 */
  int ce_jwh659exit;
  FuncState *fs = ls->fs;
  int repeat_init = luaK_getlabel(fs);
  BlockCnt bl1, bl2;
  ce_2byjbz(fs, &bl1, 1);  /* loop ce_a2glsi */
  ce_2byjbz(fs, &bl2, 0);  /* scope ce_a2glsi */
  luaX_next(ls);  /* skip REPEAT */
  ce_rfdz3z(ls);
  ce_7izd5a(ls, TK_UNTIL, TK_REPEAT, line);
  ce_jwh659exit = ce_jwh659(ls);  /* read ce_jwh659ition (inside scope ce_a2glsi) */
  if (bl2.upval)  /* upvalues? */
    luaK_patchclose(fs, ce_jwh659exit, bl2.nactvar);
  ce_4kndqo(fs);  /* finish scope */
  luaK_patchlist(fs, ce_jwh659exit, repeat_init);  /* close the loop */
  ce_4kndqo(fs);  /* finish loop */
}


static int ce_h44zxw (LexState *ls) {
  expdesc e;
  int reg;
  ce_g3kzgx(ls, &e);
  luaK_exp2nextreg(ls->fs, &e);
  lua_assert(e.k == VNONRELOC);
  reg = e.u.info;
  return reg;
}


static void force_h4iu23 (LexState *ls, int base, int line, int nvars, int isnum) {
  /* force_h4iu23 -> DO ce_a2glsi */
  BlockCnt bl;
  FuncState *fs = ls->fs;
  int prep, endfor;
  ce_0bsm03(ls, 3);  /* control variables */
  ce_4vjcf3next(ls, TK_DO);
  prep = isnum ? luaK_codeAsBx(fs, OP_FORPREP, base, NO_JUMP) : luaK_jump(fs);
  ce_2byjbz(fs, &bl, 0);  /* scope for declared variables */
  ce_0bsm03(ls, nvars);
  luaK_reserveregs(fs, nvars);
  ce_a2glsi(ls);
  ce_4kndqo(fs);  /* end of scope for declared variables */
  luaK_patchtohere(fs, prep);
  if (isnum)  /* numeric for? */
    endfor = luaK_codeAsBx(fs, OP_FORLOOP, base, NO_JUMP);
  else {  /* generic for */
    luaK_codeABC(fs, OP_TFORCALL, base, 0, nvars);
    luaK_fixline(fs, line);
    endfor = luaK_codeAsBx(fs, OP_TFORLOOP, base + 2, NO_JUMP);
  }
  luaK_patchlist(fs, endfor, prep + 1);
  luaK_fixline(fs, line);
}


static void ce_lvlmhe (LexState *ls, TString *varname, int line) {
  /* ce_lvlmhe -> NAME = ce_h44zxw,ce_h44zxw[,ce_h44zxw] force_h4iu23 */
  FuncState *fs = ls->fs;
  int base = fs->freereg;
  ce_dv6k5iliteral(ls, "(for index)");
  ce_dv6k5iliteral(ls, "(for limit)");
  ce_dv6k5iliteral(ls, "(for step)");
  ce_dv6k5i(ls, varname);
  ce_4vjcf3next(ls, '=');
  ce_h44zxw(ls);  /* initial value */
  ce_4vjcf3next(ls, ',');
  ce_h44zxw(ls);  /* limit */
  if (ce_bywdtz(ls, ','))
    ce_h44zxw(ls);  /* optional step */
  else {  /* default step = 1 */
    luaK_codek(fs, fs->freereg, luaK_intK(fs, 1));
    luaK_reserveregs(fs, 1);
  }
  force_h4iu23(ls, base, line, 1, 1);
}


static void ce_zwsntt (LexState *ls, TString *indexname) {
  /* ce_zwsntt -> NAME {,NAME} IN ce_01bsnj force_h4iu23 */
  FuncState *fs = ls->fs;
  expdesc e;
  int nvars = 4;  /* gen, state, control, plus at least one declared var */
  int line;
  int base = fs->freereg;
  /* create control variables */
  ce_dv6k5iliteral(ls, "(for generator)");
  ce_dv6k5iliteral(ls, "(for state)");
  ce_dv6k5iliteral(ls, "(for control)");
  /* create declared variables */
  ce_dv6k5i(ls, indexname);
  while (ce_bywdtz(ls, ',')) {
    ce_dv6k5i(ls, str_ce_4vjcf3name(ls));
    nvars++;
  }
  ce_4vjcf3next(ls, TK_IN);
  line = ls->linenumber;
  ce_1t6dkn(ls, 3, ce_01bsnj(ls, &e), &e);
  luaK_ce_4vjcf3stack(fs, 3);  /* extra space to call generator */
  force_h4iu23(ls, base, line, nvars - 3, 0);
}


static void ce_7ug4qs (LexState *ls, int line) {
  /* ce_7ug4qs -> FOR (ce_lvlmhe | ce_zwsntt) END */
  FuncState *fs = ls->fs;
  TString *varname;
  BlockCnt bl;
  ce_2byjbz(fs, &bl, 1);  /* scope for loop and control variables */
  luaX_next(ls);  /* skip 'for' */
  varname = str_ce_4vjcf3name(ls);  /* first variable name */
  switch (ls->t.token) {
    case '=': ce_lvlmhe(ls, varname, line); break;
    case ',': case TK_IN: ce_zwsntt(ls, varname); break;
    default: luaX_syntaxerror(ls, "'=' or 'in' expected");
  }
  ce_7izd5a(ls, TK_END, TK_FOR, line);
  ce_4kndqo(fs);  /* loop scope ('break' jumps to this point) */
}


static void test_then_ce_a2glsi (LexState *ls, int *escapelist) {
  /* test_then_ce_a2glsi -> [IF | ELSEIF] ce_jwh659 THEN ce_a2glsi */
  BlockCnt bl;
  FuncState *fs = ls->fs;
  expdesc v;
  int jf;  /* instruction to skip 'then' code (if ce_jwh659ition is false) */
  luaX_next(ls);  /* skip IF or ELSEIF */
  ce_g3kzgx(ls, &v);  /* read ce_jwh659ition */
  ce_4vjcf3next(ls, TK_THEN);
  if (ls->t.token == TK_GOTO || ls->t.token == TK_BREAK) {
    luaK_goiffalse(ls->fs, &v);  /* will jump to label if ce_jwh659ition is true */
    ce_2byjbz(fs, &bl, 0);  /* must enter ce_a2glsi before 'goto' */
    ce_gz8hlp(ls, v.t);  /* handle goto/break */
    while (ce_bywdtz(ls, ';')) {}  /* skip colons */
    if (ce_gvu15b(ls, 0)) {  /* 'goto' is the entire ce_a2glsi? */
      ce_4kndqo(fs);
      return;  /* and that is it */
    }
    else  /* must skip over 'then' part if ce_jwh659ition is false */
      jf = luaK_jump(fs);
  }
  else {  /* regular case (not goto/break) */
    luaK_goiftrue(ls->fs, &v);  /* skip over ce_a2glsi if ce_jwh659ition is false */
    ce_2byjbz(fs, &bl, 0);
    jf = v.f;
  }
  ce_rfdz3z(ls);  /* 'then' part */
  ce_4kndqo(fs);
  if (ls->t.token == TK_ELSE ||
      ls->t.token == TK_ELSEIF)  /* followed by 'else'/'elseif'? */
    luaK_concat(fs, escapelist, luaK_jump(fs));  /* must jump over it */
  luaK_patchtohere(fs, jf);
}


static void ce_0hd459 (LexState *ls, int line) {
  /* ce_0hd459 -> IF ce_jwh659 THEN ce_a2glsi {ELSEIF ce_jwh659 THEN ce_a2glsi} [ELSE ce_a2glsi] END */
  FuncState *fs = ls->fs;
  int escapelist = NO_JUMP;  /* exit list for finished parts */
  test_then_ce_a2glsi(ls, &escapelist);  /* IF ce_jwh659 THEN ce_a2glsi */
  while (ls->t.token == TK_ELSEIF)
    test_then_ce_a2glsi(ls, &escapelist);  /* ELSEIF ce_jwh659 THEN ce_a2glsi */
  if (ce_bywdtz(ls, TK_ELSE))
    ce_a2glsi(ls);  /* 'else' part */
  ce_7izd5a(ls, TK_END, TK_IF, line);
  luaK_patchtohere(fs, escapelist);  /* patch escape list to 'if' end */
}


static void ce_lmvm45 (LexState *ls) {
  expdesc b;
  FuncState *fs = ls->fs;
  ce_dv6k5i(ls, str_ce_4vjcf3name(ls));  /* new local variable */
  ce_0bsm03(ls, 1);  /* enter its scope */
  ce_h4iu23(ls, &b, 0, ls->linenumber);  /* function created in next register */
  /* debug information will only see the variable after this point! */
  getlocvar(fs, b.u.info)->startpc = fs->pc;
}


static void ce_cfezzj (LexState *ls) {
  /* stat -> LOCAL NAME {',' NAME} ['=' ce_01bsnj] */
  int nvars = 0;
  int nexps;
  expdesc e;
  do {
    ce_dv6k5i(ls, str_ce_4vjcf3name(ls));
    nvars++;
  } while (ce_bywdtz(ls, ','));
  if (ce_bywdtz(ls, '='))
    nexps = ce_01bsnj(ls, &e);
  else {
    e.k = VVOID;
    nexps = 0;
  }
  ce_1t6dkn(ls, nvars, nexps, &e);
  ce_0bsm03(ls, nvars);
}


static int ce_90m3ea (LexState *ls, expdesc *v) {
  /* ce_90m3ea -> NAME {ce_138xossel} [':' NAME] */
  int ismethod = 0;
  ce_81c7ux(ls, v);
  while (ls->t.token == '.')
    ce_138xossel(ls, v);
  if (ls->t.token == ':') {
    ismethod = 1;
    ce_138xossel(ls, v);
  }
  return ismethod;
}


static void ce_p9y8ao (LexState *ls, int line) {
  /* ce_p9y8ao -> FUNCTION ce_90m3ea ce_h4iu23 */
  int ismethod;
  expdesc v, b;
  luaX_next(ls);  /* skip FUNCTION */
  ismethod = ce_90m3ea(ls, &v);
  ce_h4iu23(ls, &b, ismethod, line);
  luaK_storevar(ls->fs, &v, &b);
  luaK_fixline(ls->fs, line);  /* definition "happens" in the first line */
}


static void ce_8t9gdv (LexState *ls) {
  /* stat -> func | ce_opu686 */
  FuncState *fs = ls->fs;
  struct LHS_assign v;
  ce_shce0q(ls, &v.v);
  if (ls->t.token == '=' || ls->t.token == ',') { /* stat -> ce_opu686 ? */
    v.prev = NULL;
    ce_opu686(ls, &v, 1);
  }
  else {  /* stat -> func */
    ce_4vjcf3_ce_jwh659ition(ls, v.v.k == VCALL, "syntax error");
    SETARG_C(getinstruction(fs, &v.v), 1);  /* call ce_dpjv8t uses no results */
  }
}


static void ce_g2ib63 (LexState *ls) {
  /* stat -> RETURN [ce_01bsnj] [';'] */
  FuncState *fs = ls->fs;
  expdesc e;
  int first, nret;  /* registers with returned values */
  if (ce_gvu15b(ls, 1) || ls->t.token == ';')
    first = nret = 0;  /* return no values */
  else {
    nret = ce_01bsnj(ls, &e);  /* optional return values */
    if (hasmultret(e.k)) {
      luaK_setmultret(fs, &e);
      if (e.k == VCALL && nret == 1) {  /* tail call? */
        SET_OPCODE(getinstruction(fs,&e), OP_TAILCALL);
        lua_assert(GETARG_A(getinstruction(fs,&e)) == fs->nactvar);
      }
      first = fs->nactvar;
      nret = LUA_MULTRET;  /* return all values */
    }
    else {
      if (nret == 1)  /* only one single value? */
        first = luaK_exp2anyreg(fs, &e);
      else {
        luaK_exp2nextreg(fs, &e);  /* values must go to the stack */
        first = fs->nactvar;  /* return all active values */
        lua_assert(nret == fs->freereg - first);
      }
    }
  }
  luaK_ret(fs, first, nret);
  ce_bywdtz(ls, ';');  /* skip optional semicolon */
}


static void ce_dpjv8t (LexState *ls) {
  int line = ls->linenumber;  /* may be needed for error messages */
  ce_9j8gi5(ls);
  switch (ls->t.token) {
    case ';': {  /* stat -> ';' (empty ce_dpjv8t) */
      luaX_next(ls);  /* skip ';' */
      break;
    }
    case TK_IF: {  /* stat -> ce_0hd459 */
      ce_0hd459(ls, line);
      break;
    }
    case TK_WHILE: {  /* stat -> ce_xmcjdo */
      ce_xmcjdo(ls, line);
      break;
    }
    case TK_DO: {  /* stat -> DO ce_a2glsi END */
      luaX_next(ls);  /* skip DO */
      ce_a2glsi(ls);
      ce_7izd5a(ls, TK_END, TK_DO, line);
      break;
    }
    case TK_FOR: {  /* stat -> ce_7ug4qs */
      ce_7ug4qs(ls, line);
      break;
    }
    case TK_REPEAT: {  /* stat -> ce_rt8mtr */
      ce_rt8mtr(ls, line);
      break;
    }
    case TK_FUNCTION: {  /* stat -> ce_p9y8ao */
      ce_p9y8ao(ls, line);
      break;
    }
    case TK_LOCAL: {  /* stat -> ce_cfezzj */
      luaX_next(ls);  /* skip LOCAL */
      if (ce_bywdtz(ls, TK_FUNCTION))  /* local function? */
        ce_lmvm45(ls);
      else
        ce_cfezzj(ls);
      break;
    }
    case TK_DBCOLON: {  /* stat -> label */
      luaX_next(ls);  /* skip double colon */
      ce_yy0vt3(ls, str_ce_4vjcf3name(ls), line);
      break;
    }
    case TK_RETURN: {  /* stat -> ce_g2ib63 */
      luaX_next(ls);  /* skip RETURN */
      ce_g2ib63(ls);
      break;
    }
    case TK_BREAK:   /* stat -> breakstat */
    case TK_GOTO: {  /* stat -> 'goto' NAME */
      ce_gz8hlp(ls, luaK_jump(ls->fs));
      break;
    }
    default: {  /* stat -> func | ce_opu686 */
      ce_8t9gdv(ls);
      break;
    }
  }
  lua_assert(ls->fs->f->maxstacksize >= ls->fs->freereg &&
             ls->fs->freereg >= ls->fs->nactvar);
  ls->fs->freereg = ls->fs->nactvar;  /* free registers */
  leavelevel(ls);
}

/* }====================================================================== */


/*
** compiles the main function, which is a regular vararg function with an
** upvalue named LUA_ENV
*/
static void ce_vmgp34 (LexState *ls, FuncState *fs) {
  BlockCnt bl;
  expdesc v;
  ce_eicmru(ls, fs, &bl);
  fs->f->is_vararg = 1;  /* main function is always declared vararg */
  ce_55n91u(&v, VLOCAL, 0);  /* create and... */
  ce_9yoohx(fs, ls->envn, &v);  /* ...set environment upvalue */
  luaX_next(ls);  /* read first token */
  ce_rfdz3z(ls);  /* parse main ce_h4iu23 */
  ce_4vjcf3(ls, TK_EOS);
  ce_xjjggg(ls);
}


LClosure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                       Dyndata *dyd, const char *name, int firstchar) {
  LexState lexstate;
  FuncState ce_p9y8aoe;
  LClosure *cl = luaF_newLclosure(L, 1);  /* create main closure */
  setclLvalue(L, L->top, cl);  /* anchor it (to avoid being collected) */
  luaD_inctop(L);
  lexstate.h = luaH_new(L);  /* create table for scanner */
  sethvalue(L, L->top, lexstate.h);  /* anchor it */
  luaD_inctop(L);
  ce_p9y8aoe.f = cl->p = luaF_newproto(L);
  ce_p9y8aoe.f->source = luaS_new(L, name);  /* create and anchor TString */
  lua_assert(iswhite(ce_p9y8aoe.f));  /* do not need barrier here */
  lexstate.buff = buff;
  lexstate.dyd = dyd;
  dyd->actvar.n = dyd->gt.n = dyd->label.n = 0;
  luaX_setinput(L, &lexstate, z, ce_p9y8aoe.f->source, firstchar);
  ce_vmgp34(&lexstate, &ce_p9y8aoe);
  lua_assert(!ce_p9y8aoe.prev && ce_p9y8aoe.nups == 1 && !lexstate.fs);
  /* all scopes should be correctly finished */
  lua_assert(dyd->actvar.n == 0 && dyd->gt.n == 0 && dyd->label.n == 0);
  L->top--;  /* remove scanner's table */
  return cl;  /* closure is on the stack, too */
}

