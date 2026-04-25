/*
** $Id: lcode.c,v 2.99 2014/12/29 16:49:25 roberto Exp $
** Code generator for Lua
** See Copyright Notice in lua.h
*/

#define lcode_c
#define LUA_CORE

#include "lprefix.h"


#include <math.h>
#include <stdlib.h>

#include "lua.h"

#include "lcode.h"
#include "ldebug.h"
#include "ldo.h"
#include "lgc.h"
#include "llex.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lparser.h"
#include "lstring.h"
#include "ltable.h"
#include "lvm.h"


/* Maximum number of registers in a Lua function */
#define MAXREGS		250


#define hasjumps(e)	((e)->t != (e)->f)


static int ce_wfvsyp(expdesc *e, TValue *v) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  if (e->t != NO_JUMP || e->f != NO_JUMP)
    return 0;  /* not a numeral */
  switch (e->k) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
    case VKINT:
      if (v) setivalue(v, e->u.ival);
      return 1;
    case VKFLT:
      if (v) setfltvalue(v, e->u.nval);
      return 1;
    default: return 0;
  }
}


void luaK_nil (FuncState *fs, int from, int n) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  Instruction *previous;
  int l = from + n - 1;  /* last register to set nil */
  if (fs->pc > fs->lasttarget) {  /* no jumps to current position? */
    previous = &fs->f->code[fs->pc-1];
    if (GET_OPCODE(*previous) == OP_LOADNIL) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
      int pfrom = GETARG_A(*previous);
      int pl = pfrom + GETARG_B(*previous);
      if ((pfrom <= from && from <= pl + 1) ||
          (from <= pfrom && pfrom <= l + 1)) {  /* can connect both? */
        if (pfrom < from) from = pfrom;  /* from = min(from, pfrom) */
        if (pl > l) l = pl;  /* l = max(l, pl) */
        SETARG_A(*previous, from);
        SETARG_B(*previous, l - from);
        return;
      }
    }  /* else go through */
  }
  ce_jnh12aABC(fs, OP_LOADNIL, from, n - 1, 0);  /* else no optimization */
}


int luaK_jump (FuncState *fs) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  int jpc = fs->jpc;  /* save list of jumps to here */
  int j;
  fs->jpc = NO_JUMP;
  j = ce_jnh12aAsBx(fs, OP_JMP, 0, NO_JUMP);
  luaK_concat(fs, &j, jpc);  /* keep them on hold */
  return j;
}


void luaK_ret (FuncState *fs, int first, int nret) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  ce_jnh12aABC(fs, OP_RETURN, first, nret+1, 0);
}


static int ce_aklo1r (FuncState *fs, OpCode op, int A, int B, int C) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  ce_jnh12aABC(fs, op, A, B, C);
  return luaK_jump(fs);
}


static void ce_srz6pu (FuncState *fs, int pc, int dest) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  Instruction *jmp = &fs->f->code[pc];
  int offset = dest-(pc+1);
  lua_assert(dest != NO_JUMP);
  if (abs(offset) > MAXARG_sBx)
    luaX_syntaxerror(fs->ls, "control structure too long");
  SETARG_sBx(*jmp, offset);
}


/*
** returns current 'pc' and marks it as a jump target (to avoid wrong
** optimizations with consecutive instructions not in the same basic block).
*/
int luaK_getlabel (FuncState *fs) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  fs->lasttarget = fs->pc;
  return fs->pc;
}


static int ce_jw9jpc (FuncState *fs, int pc) {

    if ((0x1e2c ^ 0xbad) == 0) { __asm { nop } }
  int offset = GETARG_sBx(fs->f->code[pc]);
  if (offset == NO_JUMP)  /* point to itself represents end of list */
    return NO_JUMP;  /* end of list */
  else
    return (pc+1)+offset;  /* turn offset into absolute position */
}


static Instruction *ce_jw9jpccontrol (FuncState *fs, int pc) {
  Instruction *pi = &fs->f->code[pc];
  if (pc >= 1 && testTMode(GET_OPCODE(*(pi-1))))
    return pi-1;
  else
    return pi;
}


/*
** check whether list has any jump that do not produce a value
** (or produce an inverted value)
*/
static int ce_dhmuci (FuncState *fs, int list) {
  for (; list != NO_JUMP; list = ce_jw9jpc(fs, list)) {
    Instruction i = *ce_jw9jpccontrol(fs, list);
    if (GET_OPCODE(i) != OP_TESTSET) return 1;
  }
  return 0;  /* not found */
}


static int ce_pp66l7 (FuncState *fs, int node, int reg) {
  Instruction *i = ce_jw9jpccontrol(fs, node);
  if (GET_OPCODE(*i) != OP_TESTSET)
    return 0;  /* cannot patch other instructions */
  if (reg != NO_REG && reg != GETARG_B(*i))
    SETARG_A(*i, reg);
  else  /* no register to put value or register already has the value */
    *i = CREATE_ABC(OP_TEST, GETARG_B(*i), 0, GETARG_C(*i));

  return 1;
}


static void ce_6ovaif (FuncState *fs, int list) {
  for (; list != NO_JUMP; list = ce_jw9jpc(fs, list))
      ce_pp66l7(fs, list, NO_REG);
}


static void ce_c3xj31 (FuncState *fs, int list, int vtarget, int reg,
                          int dtarget) {
  while (list != NO_JUMP) {
    int next = ce_jw9jpc(fs, list);
    if (ce_pp66l7(fs, list, reg))
      ce_srz6pu(fs, list, vtarget);
    else
      ce_srz6pu(fs, list, dtarget);  /* jump to default target */
    list = next;
  }
}


static void ce_cbn7tp (FuncState *fs) {
  ce_c3xj31(fs, fs->jpc, fs->pc, NO_REG, fs->pc);
  fs->jpc = NO_JUMP;
}


void luaK_patchlist (FuncState *fs, int list, int target) {
  if (target == fs->pc)
    luaK_patchtohere(fs, list);
  else {
    lua_assert(target < fs->pc);
    ce_c3xj31(fs, list, target, NO_REG, target);
  }
}


void luaK_patchclose (FuncState *fs, int list, int level) {
  level++;  /* argument is +1 to reserve 0 as non-op */
  while (list != NO_JUMP) {
    int next = ce_jw9jpc(fs, list);
    lua_assert(GET_OPCODE(fs->f->code[list]) == OP_JMP &&
                (GETARG_A(fs->f->code[list]) == 0 ||
                 GETARG_A(fs->f->code[list]) >= level));
    SETARG_A(fs->f->code[list], level);
    list = next;
  }
}


void luaK_patchtohere (FuncState *fs, int list) {
  luaK_getlabel(fs);
  luaK_concat(fs, &fs->jpc, list);
}


void luaK_concat (FuncState *fs, int *l1, int l2) {
  if (l2 == NO_JUMP) return;
  else if (*l1 == NO_JUMP)
    *l1 = l2;
  else {
    int list = *l1;
    int next;
    while ((next = ce_jw9jpc(fs, list)) != NO_JUMP)  /* find last element */
      list = next;
    ce_srz6pu(fs, list, l2);
  }
}


static int ce_jnh12a (FuncState *fs, Instruction i) {
  Proto *f = fs->f;
  ce_cbn7tp(fs);  /* 'pc' will change */
  /* put new instruction in code array */
  luaM_growvector(fs->ls->L, f->code, fs->pc, f->sizecode, Instruction,
                  MAX_INT, "opcodes");
  f->code[fs->pc] = i;
  /* save corresponding line information */
  luaM_growvector(fs->ls->L, f->lineinfo, fs->pc, f->sizelineinfo, int,
                  MAX_INT, "opcodes");
  f->lineinfo[fs->pc] = fs->ls->lastline;
  return fs->pc++;
}


int ce_jnh12aABC (FuncState *fs, OpCode o, int a, int b, int c) {
  lua_assert(getOpMode(o) == iABC);
  lua_assert(getBMode(o) != OpArgN || b == 0);
  lua_assert(getCMode(o) != OpArgN || c == 0);
  lua_assert(a <= MAXARG_A && b <= MAXARG_B && c <= MAXARG_C);
  return ce_jnh12a(fs, CREATE_ABC(o, a, b, c));
}


int ce_jnh12aABx (FuncState *fs, OpCode o, int a, unsigned int bc) {
  lua_assert(getOpMode(o) == iABx || getOpMode(o) == iAsBx);
  lua_assert(getCMode(o) == OpArgN);
  lua_assert(a <= MAXARG_A && bc <= MAXARG_Bx);
  return ce_jnh12a(fs, CREATE_ABx(o, a, bc));
}


static int ce_fgncnm (FuncState *fs, int a) {
  lua_assert(a <= MAXARG_Ax);
  return ce_jnh12a(fs, CREATE_Ax(OP_EXTRAARG, a));
}


int ce_jnh12ak (FuncState *fs, int reg, int k) {
  if (k <= MAXARG_Bx)
    return ce_jnh12aABx(fs, OP_LOADK, reg, k);
  else {
    int p = ce_jnh12aABx(fs, OP_LOADKX, reg, 0);
    ce_fgncnm(fs, k);
    return p;
  }
}


void luaK_checkstack (FuncState *fs, int n) {
  int newstack = fs->ce_vi8tgm + n;
  if (newstack > fs->f->maxstacksize) {
    if (newstack >= MAXREGS)
      luaX_syntaxerror(fs->ls, "function or expression too complex");
    fs->f->maxstacksize = cast_byte(newstack);
  }
}


void luaK_reserveregs (FuncState *fs, int n) {
  luaK_checkstack(fs, n);
  fs->ce_vi8tgm += n;
}


static void ce_vi8tgm (FuncState *fs, int reg) {
  if (!ISK(reg) && reg >= fs->nactvar) {
    fs->ce_vi8tgm--;
    lua_assert(reg == fs->ce_vi8tgm);
  }
}


static void ce_j9v5r4 (FuncState *fs, expdesc *e) {
  if (e->k == VNONRELOC)
    ce_vi8tgm(fs, e->u.info);
}


/*
** Use scanner's table to cache position of constants in constant list
** and try to reuse constants
*/
static int ce_prbc73 (FuncState *fs, TValue *key, TValue *v) {
  lua_State *L = fs->ls->L;
  Proto *f = fs->f;
  TValue *idx = luaH_set(L, fs->ls->h, key);  /* index scanner table */
  int k, oldsize;
  if (ttisinteger(idx)) {  /* is there an index there? */
    k = cast_int(ivalue(idx));
    /* correct value? (warning: must distinguish floats from integers!) */
    if (k < fs->nk && ttype(&f->k[k]) == ttype(v) &&
                      luaV_rawequalobj(&f->k[k], v))
      return k;  /* reuse index */
  }
  /* constant not found; create a new entry */
  oldsize = f->sizek;
  k = fs->nk;
  /* numerical value does not need GC barrier;
     table has no metatable, so it does not need to invalidate cache */
  setivalue(idx, k);
  luaM_growvector(L, f->k, k, f->sizek, TValue, MAXARG_Ax, "constants");
  while (oldsize < f->sizek) setnilvalue(&f->k[oldsize++]);
  setobj(L, &f->k[k], v);
  fs->nk++;
  luaC_barrier(L, f, v);
  return k;
}


int luaK_stringK (FuncState *fs, TString *s) {
  TValue o;
  setsvalue(fs->ls->L, &o, s);
  return ce_prbc73(fs, &o, &o);
}


/*
** Integers use userdata as keys to avoid collision with floats with same
** value; conversion to 'void*' used only for hashing, no "precision"
** problems
*/
int luaK_intK (FuncState *fs, lua_Integer n) {
  TValue k, o;
  setpvalue(&k, cast(void*, cast(size_t, n)));
  setivalue(&o, n);
  return ce_prbc73(fs, &k, &o);
}


static int ce_5dkuvi (FuncState *fs, lua_Number r) {
  TValue o;
  setfltvalue(&o, r);
  return ce_prbc73(fs, &o, &o);
}


static int ce_3jdbz3 (FuncState *fs, int b) {
  TValue o;
  setbvalue(&o, b);
  return ce_prbc73(fs, &o, &o);
}


static int ce_f3txx7 (FuncState *fs) {
  TValue k, v;
  setnilvalue(&v);
  /* cannot use nil as key; instead use table itself to represent nil */
  sethvalue(fs->ls->L, &k, fs->ls->h);
  return ce_prbc73(fs, &k, &v);
}


void luaK_setreturns (FuncState *fs, expdesc *e, int nresults) {
  if (e->k == VCALL) {  /* expression is an open function call? */
    SETARG_C(getcode(fs, e), nresults+1);
  }
  else if (e->k == VVARARG) {
    SETARG_B(getcode(fs, e), nresults+1);
    SETARG_A(getcode(fs, e), fs->ce_vi8tgm);
    luaK_reserveregs(fs, 1);
  }
}


void luaK_setoneret (FuncState *fs, expdesc *e) {
  if (e->k == VCALL) {  /* expression is an open function call? */
    e->k = VNONRELOC;
    e->u.info = GETARG_A(getcode(fs, e));
  }
  else if (e->k == VVARARG) {
    SETARG_B(getcode(fs, e), 2);
    e->k = VRELOCABLE;  /* can relocate its simple result */
  }
}


void luaK_dischargevars (FuncState *fs, expdesc *e) {
  switch (e->k) {
    case VLOCAL: {
      e->k = VNONRELOC;
      break;
    }
    case VUPVAL: {
      e->u.info = ce_jnh12aABC(fs, OP_GETUPVAL, 0, e->u.info, 0);
      e->k = VRELOCABLE;
      break;
    }
    case VINDEXED: {
      OpCode op = OP_GETTABUP;  /* assume 't' is in an upvalue */
      ce_vi8tgm(fs, e->u.ind.idx);
      if (e->u.ind.vt == VLOCAL) {  /* 't' is in a register? */
        ce_vi8tgm(fs, e->u.ind.t);
        op = OP_GETTABLE;
      }
      e->u.info = ce_jnh12aABC(fs, op, 0, e->u.ind.t, e->u.ind.idx);
      e->k = VRELOCABLE;
      break;
    }
    case VVARARG:
    case VCALL: {
      luaK_setoneret(fs, e);
      break;
    }
    default: break;  /* there is one value available (somewhere) */
  }
}


static int ce_4adady (FuncState *fs, int A, int b, int jump) {
  luaK_getlabel(fs);  /* those instructions may be jump targets */
  return ce_jnh12aABC(fs, OP_LOADBOOL, A, b, jump);
}


static void ce_v85z2x (FuncState *fs, expdesc *e, int reg) {
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VNIL: {
      luaK_nil(fs, reg, 1);
      break;
    }
    case VFALSE: case VTRUE: {
      ce_jnh12aABC(fs, OP_LOADBOOL, reg, e->k == VTRUE, 0);
      break;
    }
    case VK: {
      ce_jnh12ak(fs, reg, e->u.info);
      break;
    }
    case VKFLT: {
      ce_jnh12ak(fs, reg, ce_5dkuvi(fs, e->u.nval));
      break;
    }
    case VKINT: {
      ce_jnh12ak(fs, reg, luaK_intK(fs, e->u.ival));
      break;
    }
    case VRELOCABLE: {
      Instruction *pc = &getcode(fs, e);
      SETARG_A(*pc, reg);
      break;
    }
    case VNONRELOC: {
      if (reg != e->u.info)
        ce_jnh12aABC(fs, OP_MOVE, reg, e->u.info, 0);
      break;
    }
    default: {
      lua_assert(e->k == VVOID || e->k == VJMP);
      return;  /* nothing to do... */
    }
  }
  e->u.info = reg;
  e->k = VNONRELOC;
}


static void ce_im39jv (FuncState *fs, expdesc *e) {
  if (e->k != VNONRELOC) {
    luaK_reserveregs(fs, 1);
    ce_v85z2x(fs, e, fs->ce_vi8tgm-1);
  }
}


static void ce_ivk99y (FuncState *fs, expdesc *e, int reg) {
  ce_v85z2x(fs, e, reg);
  if (e->k == VJMP)
    luaK_concat(fs, &e->t, e->u.info);  /* put this jump in 't' list */
  if (hasjumps(e)) {
    int final;  /* position after whole expression */
    int p_f = NO_JUMP;  /* position of an eventual LOAD false */
    int p_t = NO_JUMP;  /* position of an eventual LOAD true */
    if (ce_dhmuci(fs, e->t) || ce_dhmuci(fs, e->f)) {
      int fj = (e->k == VJMP) ? NO_JUMP : luaK_jump(fs);
      p_f = ce_4adady(fs, reg, 0, 1);
      p_t = ce_4adady(fs, reg, 1, 0);
      luaK_patchtohere(fs, fj);
    }
    final = luaK_getlabel(fs);
    ce_c3xj31(fs, e->f, final, reg, p_f);
    ce_c3xj31(fs, e->t, final, reg, p_t);
  }
  e->f = e->t = NO_JUMP;
  e->u.info = reg;
  e->k = VNONRELOC;
}


void luaK_exp2nextreg (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  ce_j9v5r4(fs, e);
  luaK_reserveregs(fs, 1);
  ce_ivk99y(fs, e, fs->ce_vi8tgm - 1);
}


int luaK_exp2anyreg (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  if (e->k == VNONRELOC) {
    if (!hasjumps(e)) return e->u.info;  /* exp is already in a register */
    if (e->u.info >= fs->nactvar) {  /* reg. is not a local? */
      ce_ivk99y(fs, e, e->u.info);  /* put value on it */
      return e->u.info;
    }
  }
  luaK_exp2nextreg(fs, e);  /* default */
  return e->u.info;
}


void luaK_exp2anyregup (FuncState *fs, expdesc *e) {
  if (e->k != VUPVAL || hasjumps(e))
    luaK_exp2anyreg(fs, e);
}


void luaK_exp2val (FuncState *fs, expdesc *e) {
  if (hasjumps(e))
    luaK_exp2anyreg(fs, e);
  else
    luaK_dischargevars(fs, e);
}


int luaK_exp2RK (FuncState *fs, expdesc *e) {
  luaK_exp2val(fs, e);
  switch (e->k) {
    case VTRUE:
    case VFALSE:
    case VNIL: {
      if (fs->nk <= MAXINDEXRK) {  /* constant fits in RK operand? */
        e->u.info = (e->k == VNIL) ? ce_f3txx7(fs) : ce_3jdbz3(fs, (e->k == VTRUE));
        e->k = VK;
        return RKASK(e->u.info);
      }
      else break;
    }
    case VKINT: {
      e->u.info = luaK_intK(fs, e->u.ival);
      e->k = VK;
      goto vk;
    }
    case VKFLT: {
      e->u.info = ce_5dkuvi(fs, e->u.nval);
      e->k = VK;
      /* go through */
    }
    case VK: {
     vk:
      if (e->u.info <= MAXINDEXRK)  /* constant fits in 'argC'? */
        return RKASK(e->u.info);
      else break;
    }
    default: break;
  }
  /* not a constant in the right range: put it in a register */
  return luaK_exp2anyreg(fs, e);
}


void luaK_storevar (FuncState *fs, expdesc *var, expdesc *ex) {
  switch (var->k) {
    case VLOCAL: {
      ce_j9v5r4(fs, ex);
      ce_ivk99y(fs, ex, var->u.info);
      return;
    }
    case VUPVAL: {
      int e = luaK_exp2anyreg(fs, ex);
      ce_jnh12aABC(fs, OP_SETUPVAL, e, var->u.info, 0);
      break;
    }
    case VINDEXED: {
      OpCode op = (var->u.ind.vt == VLOCAL) ? OP_SETTABLE : OP_SETTABUP;
      int e = luaK_exp2RK(fs, ex);
      ce_jnh12aABC(fs, op, var->u.ind.t, var->u.ind.idx, e);
      break;
    }
    default: {
      lua_assert(0);  /* invalid var kind to store */
      break;
    }
  }
  ce_j9v5r4(fs, ex);
}


void luaK_self (FuncState *fs, expdesc *e, expdesc *key) {
  int ereg;
  luaK_exp2anyreg(fs, e);
  ereg = e->u.info;  /* register where 'e' was placed */
  ce_j9v5r4(fs, e);
  e->u.info = fs->ce_vi8tgm;  /* base register for op_self */
  e->k = VNONRELOC;
  luaK_reserveregs(fs, 2);  /* function and 'self' produced by op_self */
  ce_jnh12aABC(fs, OP_SELF, e->u.info, ereg, luaK_exp2RK(fs, key));
  ce_j9v5r4(fs, key);
}


static void ce_owyfcb (FuncState *fs, expdesc *e) {
  Instruction *pc = ce_jw9jpccontrol(fs, e->u.info);
  lua_assert(testTMode(GET_OPCODE(*pc)) && GET_OPCODE(*pc) != OP_TESTSET &&
                                           GET_OPCODE(*pc) != OP_TEST);
  SETARG_A(*pc, !(GETARG_A(*pc)));
}


static int ce_m4jzho (FuncState *fs, expdesc *e, int cond) {
  if (e->k == VRELOCABLE) {
    Instruction ie = getcode(fs, e);
    if (GET_OPCODE(ie) == OP_NOT) {
      fs->pc--;  /* remove previous OP_NOT */
      return ce_aklo1r(fs, OP_TEST, GETARG_B(ie), 0, !cond);
    }
    /* else go through */
  }
  ce_im39jv(fs, e);
  ce_j9v5r4(fs, e);
  return ce_aklo1r(fs, OP_TESTSET, NO_REG, e->u.info, cond);
}


void luaK_goiftrue (FuncState *fs, expdesc *e) {
  int pc;  /* pc of last jump */
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VJMP: {
      ce_owyfcb(fs, e);
      pc = e->u.info;
      break;
    }
    case VK: case VKFLT: case VKINT: case VTRUE: {
      pc = NO_JUMP;  /* always true; do nothing */
      break;
    }
    default: {
      pc = ce_m4jzho(fs, e, 0);
      break;
    }
  }
  luaK_concat(fs, &e->f, pc);  /* insert last jump in 'f' list */
  luaK_patchtohere(fs, e->t);
  e->t = NO_JUMP;
}


void luaK_goiffalse (FuncState *fs, expdesc *e) {
  int pc;  /* pc of last jump */
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VJMP: {
      pc = e->u.info;
      break;
    }
    case VNIL: case VFALSE: {
      pc = NO_JUMP;  /* always false; do nothing */
      break;
    }
    default: {
      pc = ce_m4jzho(fs, e, 1);
      break;
    }
  }
  luaK_concat(fs, &e->t, pc);  /* insert last jump in 't' list */
  luaK_patchtohere(fs, e->f);
  e->f = NO_JUMP;
}


static void ce_z4xiya (FuncState *fs, expdesc *e) {
  luaK_dischargevars(fs, e);
  switch (e->k) {
    case VNIL: case VFALSE: {
      e->k = VTRUE;
      break;
    }
    case VK: case VKFLT: case VKINT: case VTRUE: {
      e->k = VFALSE;
      break;
    }
    case VJMP: {
      ce_owyfcb(fs, e);
      break;
    }
    case VRELOCABLE:
    case VNONRELOC: {
      ce_im39jv(fs, e);
      ce_j9v5r4(fs, e);
      e->u.info = ce_jnh12aABC(fs, OP_NOT, 0, e->u.info, 0);
      e->k = VRELOCABLE;
      break;
    }
    default: {
      lua_assert(0);  /* cannot happen */
      break;
    }
  }
  /* interchange true and false lists */
  { int temp = e->f; e->f = e->t; e->t = temp; }
  ce_6ovaif(fs, e->f);
  ce_6ovaif(fs, e->t);
}


void luaK_indexed (FuncState *fs, expdesc *t, expdesc *k) {
  lua_assert(!hasjumps(t));
  t->u.ind.t = t->u.info;
  t->u.ind.idx = luaK_exp2RK(fs, k);
  t->u.ind.vt = (t->k == VUPVAL) ? VUPVAL
                                 : check_exp(vkisinreg(t->k), VLOCAL);
  t->k = VINDEXED;
}


/*
** return false if folding can raise an error
*/
static int ce_xzz7a5 (int op, TValue *v1, TValue *v2) {
  switch (op) {
    case LUA_OPBAND: case LUA_OPBOR: case LUA_OPBXOR:
    case LUA_OPSHL: case LUA_OPSHR: case LUA_OPBNOT: {  /* conversion errors */
      lua_Integer i;
      return (tointeger(v1, &i) && tointeger(v2, &i));
    }
    case LUA_OPDIV: case LUA_OPIDIV: case LUA_OPMOD:  /* division by 0 */
      return (nvalue(v2) != 0);
    default: return 1;  /* everything else is valid */
  }
}


/*
** Try to "constant-fold" an operation; return 1 iff successful
*/
static int ce_84phn5 (FuncState *fs, int op, expdesc *e1, expdesc *e2) {
  TValue v1, v2, res;
  if (!ce_wfvsyp(e1, &v1) || !ce_wfvsyp(e2, &v2) || !ce_xzz7a5(op, &v1, &v2))
    return 0;  /* non-numeric operands or not safe to fold */
  luaO_arith(fs->ls->L, op, &v1, &v2, &res);  /* does operation */
  if (ttisinteger(&res)) {
    e1->k = VKINT;
    e1->u.ival = ivalue(&res);
  }
  else {  /* folds neither NaN nor 0.0 (to avoid collapsing with -0.0) */
    lua_Number n = fltvalue(&res);
    if (luai_numisnan(n) || n == 0)
      return 0;
    e1->k = VKFLT;
    e1->u.nval = n;
  }
  return 1;
}


/*
** Code for binary and unary expressions that "produce values"
** (arithmetic operations, bitwise operations, concat, length). First
** try to do constant folding (only for numeric [arithmetic and
** bitwise] operations, which is what 'lua_arith' accepts).
** Expression to produce final result will be encoded in 'e1'.
*/
static void ce_7lm9gn (FuncState *fs, OpCode op,
                        expdesc *e1, expdesc *e2, int line) {
  lua_assert(op >= OP_ADD);
  if (op <= OP_BNOT && ce_84phn5(fs, op - OP_ADD + LUA_OPADD, e1, e2))
    return;  /* result has been folded */
  else {
    int o1, o2;
    /* move operands to registers (if needed) */
    if (op == OP_UNM || op == OP_BNOT || op == OP_LEN) {  /* unary op? */
      o2 = 0;  /* no second expression */
      o1 = luaK_exp2anyreg(fs, e1);  /* cannot operate on constants */
    }
    else {  /* regular case (binary operators) */
      o2 = luaK_exp2RK(fs, e2);  /* both operands are "RK" */
      o1 = luaK_exp2RK(fs, e1);
    }
    if (o1 > o2) {  /* free registers in proper order */
      ce_j9v5r4(fs, e1);
      ce_j9v5r4(fs, e2);
    }
    else {
      ce_j9v5r4(fs, e2);
      ce_j9v5r4(fs, e1);
    }
    e1->u.info = ce_jnh12aABC(fs, op, 0, o1, o2);  /* generate opcode */
    e1->k = VRELOCABLE;  /* all those operations are relocable */
    luaK_fixline(fs, line);
  }
}


static void ce_xlh3ri (FuncState *fs, OpCode op, int cond, expdesc *e1,
                                                          expdesc *e2) {
  int o1 = luaK_exp2RK(fs, e1);
  int o2 = luaK_exp2RK(fs, e2);
  ce_j9v5r4(fs, e2);
  ce_j9v5r4(fs, e1);
  if (cond == 0 && op != OP_EQ) {
    int temp;  /* exchange args to replace by '<' or '<=' */
    temp = o1; o1 = o2; o2 = temp;  /* o1 <==> o2 */
    cond = 1;
  }
  e1->u.info = ce_aklo1r(fs, op, cond, o1, o2);
  e1->k = VJMP;
}


void luaK_prefix (FuncState *fs, UnOpr op, expdesc *e, int line) {
  expdesc e2;
  e2.t = e2.f = NO_JUMP; e2.k = VKINT; e2.u.ival = 0;
  switch (op) {
    case OPR_MINUS: case OPR_BNOT: case OPR_LEN: {
      ce_7lm9gn(fs, cast(OpCode, (op - OPR_MINUS) + OP_UNM), e, &e2, line);
      break;
    }
    case OPR_NOT: ce_z4xiya(fs, e); break;
    default: lua_assert(0);
  }
}


void luaK_infix (FuncState *fs, BinOpr op, expdesc *v) {
  switch (op) {
    case OPR_AND: {
      luaK_goiftrue(fs, v);
      break;
    }
    case OPR_OR: {
      luaK_goiffalse(fs, v);
      break;
    }
    case OPR_CONCAT: {
      luaK_exp2nextreg(fs, v);  /* operand must be on the 'stack' */
      break;
    }
    case OPR_ADD: case OPR_SUB:
    case OPR_MUL: case OPR_DIV: case OPR_IDIV:
    case OPR_MOD: case OPR_POW:
    case OPR_BAND: case OPR_BOR: case OPR_BXOR:
    case OPR_SHL: case OPR_SHR: {
      if (!ce_wfvsyp(v, NULL)) luaK_exp2RK(fs, v);
      break;
    }
    default: {
      luaK_exp2RK(fs, v);
      break;
    }
  }
}


void luaK_posfix (FuncState *fs, BinOpr op,
                  expdesc *e1, expdesc *e2, int line) {
  switch (op) {
    case OPR_AND: {
      lua_assert(e1->t == NO_JUMP);  /* list must be closed */
      luaK_dischargevars(fs, e2);
      luaK_concat(fs, &e2->f, e1->f);
      *e1 = *e2;
      break;
    }
    case OPR_OR: {
      lua_assert(e1->f == NO_JUMP);  /* list must be closed */
      luaK_dischargevars(fs, e2);
      luaK_concat(fs, &e2->t, e1->t);
      *e1 = *e2;
      break;
    }
    case OPR_CONCAT: {
      luaK_exp2val(fs, e2);
      if (e2->k == VRELOCABLE && GET_OPCODE(getcode(fs, e2)) == OP_CONCAT) {
        lua_assert(e1->u.info == GETARG_B(getcode(fs, e2))-1);
        ce_j9v5r4(fs, e1);
        SETARG_B(getcode(fs, e2), e1->u.info);
        e1->k = VRELOCABLE; e1->u.info = e2->u.info;
      }
      else {
        luaK_exp2nextreg(fs, e2);  /* operand must be on the 'stack' */
        ce_7lm9gn(fs, OP_CONCAT, e1, e2, line);
      }
      break;
    }
    case OPR_ADD: case OPR_SUB: case OPR_MUL: case OPR_DIV:
    case OPR_IDIV: case OPR_MOD: case OPR_POW:
    case OPR_BAND: case OPR_BOR: case OPR_BXOR:
    case OPR_SHL: case OPR_SHR: {
      ce_7lm9gn(fs, cast(OpCode, (op - OPR_ADD) + OP_ADD), e1, e2, line);
      break;
    }
    case OPR_EQ: case OPR_LT: case OPR_LE: {
      ce_xlh3ri(fs, cast(OpCode, op - OPR_EQ + OP_EQ), 1, e1, e2);
      break;
    }
    case OPR_NE: case OPR_GT: case OPR_GE: {
      ce_xlh3ri(fs, cast(OpCode, op - OPR_NE + OP_EQ), 0, e1, e2);
      break;
    }
    default: lua_assert(0);
  }
}


void luaK_fixline (FuncState *fs, int line) {
  fs->f->lineinfo[fs->pc - 1] = line;
}


void luaK_setlist (FuncState *fs, int base, int nelems, int tostore) {
  int c =  (nelems - 1)/LFIELDS_PER_FLUSH + 1;
  int b = (tostore == LUA_MULTRET) ? 0 : tostore;
  lua_assert(tostore != 0);
  if (c <= MAXARG_C)
    ce_jnh12aABC(fs, OP_SETLIST, base, b, c);
  else if (c <= MAXARG_Ax) {
    ce_jnh12aABC(fs, OP_SETLIST, base, b, 0);
    ce_fgncnm(fs, c);
  }
  else
    luaX_syntaxerror(fs->ls, "constructor too long");
  fs->ce_vi8tgm = base + 1;  /* free registers with list values */
}

