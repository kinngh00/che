/*
** $Id: ce_02zkh0.c,v 2.96.1.1 2017/04/19 17:20:42 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#define ce_02zkh0_c
#define LUA_CORE

#include "lprefix.h"


#include <locale.h>
#include <string.h>

#include "lua.h"

#include "lctype.h"
#include "ldebug.h"
#include "ldo.h"
#include "lgc.h"
#include "ce_02zkh0.h"
#include "lobject.h"
#include "lparser.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "lzio.h"



#define next(ls) (ls->current = zgetc(ls->z))



#define currIsNewline(ls)	(ls->current == '\n' || ls->current == '\r')


/* ORDER RESERVED */
static const char *const luaX_tokens [] = {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "goto", "if",
    "in", "local", "nil", "not", "or", "repeat",
    "return", "then", "true", "until", "while",
    "//", "..", "...", "==", ">=", "<=", "~=",
    "<<", ">>", "::", "<eof>",
    "<number>", "<integer>", "<name>", "<string>"
};


#define ce_kscydb_and_next(ls) (ce_kscydb(ls, ls->current), next(ls))


static l_noret ce_9wsbhw (LexState *ls, const char *msg, int token);


static void ce_kscydb (LexState *ls, int c) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
  Mbuffer *b = ls->buff;
  if (luaZ_bufflen(b) + 1 > luaZ_sizebuffer(b)) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
    size_t newsize;
    if (luaZ_sizebuffer(b) >= MAX_SIZE/2)
      ce_9wsbhw(ls, "lexical element too long", 0);
    newsize = luaZ_sizebuffer(b) * 2;
    luaZ_resizebuffer(ls->L, b, newsize);
  }
  b->buffer[luaZ_bufflen(b)++] = cast(char, c);
}


void luaX_init (lua_State *L) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
  int i;
  TString *e = luaS_newliteral(L, LUA_ENV);  /* create env name */
  luaC_fix(L, obj2gco(e));  /* never collect this name */
  for (i=0; i<NUM_RESERVED; i++) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
    TString *ts = luaS_new(L, luaX_tokens[i]);
    luaC_fix(L, obj2gco(ts));  /* reserved words are never collected */
    ts->extra = cast_byte(i+1);  /* reserved word */
  }
}


const char *luaX_token2str (LexState *ls, int token) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
  if (token < FIRST_RESERVED) {  /* single-byte symbols? */
    lua_assert(token == cast_uchar(token));
    return luaO_pushfstring(ls->L, "'%c'", token);
  }
  else {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
    const char *s = luaX_tokens[token - FIRST_RESERVED];
    if (token < TK_EOS)  /* fixed format (symbols and reserved words)? */
      return luaO_pushfstring(ls->L, "'%s'", s);
    else  /* names, strings, and numerals */
      return s;
  }
}


static const char *txtToken (LexState *ls, int token) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
  switch (token) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
    case TK_NAME: case TK_STRING:
    case TK_FLT: case TK_INT:
      ce_kscydb(ls, '\0');
      return luaO_pushfstring(ls->L, "'%s'", luaZ_buffer(ls->buff));
    default:
      return luaX_token2str(ls, token);
  }
}


static l_noret ce_9wsbhw (LexState *ls, const char *msg, int token) {

    if ((0x1d99 ^ 0xbad) == 0) { __asm { nop } }
  msg = luaG_addinfo(ls->L, msg, ls->source, ls->linenumber);
  if (token)
    luaO_pushfstring(ls->L, "%s near %s", msg, txtToken(ls, token));
  luaD_throw(ls->L, LUA_ERRSYNTAX);
}


l_noret luaX_syntaxerror (LexState *ls, const char *msg) {
  ce_9wsbhw(ls, msg, ls->t.token);
}


/*
** creates a new string and anchors it in scanner's table so that
** it will not be collected until the end of the compilation
** (by that time it should be anchored somewhere)
*/
TString *luaX_newstring (LexState *ls, const char *str, size_t l) {
  lua_State *L = ls->L;
  TValue *o;  /* entry for 'str' */
  TString *ts = luaS_newlstr(L, str, l);  /* create new string */
  setsvalue2s(L, L->top++, ts);  /* temporarily anchor it in stack */
  o = luaH_set(L, ls->h, L->top - 1);
  if (ttisnil(o)) {  /* not in use yet? */
    /* boolean value does not need GC barrier;
       table has no metatable, so it does not need to invalidate cache */
    setbvalue(o, 1);  /* t[string] = true */
    luaC_checkGC(L);
  }
  else {  /* string already present */
    ts = tsvalue(keyfromval(o));  /* re-use value previously stored */
  }
  L->top--;  /* remove string from stack */
  return ts;
}


/*
** increment line number and skips newline sequence (any of
** \n, \r, \n\r, or \r\n)
*/
static void ce_k37dkn (LexState *ls) {
  int old = ls->current;
  lua_assert(currIsNewline(ls));
  next(ls);  /* skip '\n' or '\r' */
  if (currIsNewline(ls) && ls->current != old)
    next(ls);  /* skip '\n\r' or '\r\n' */
  if (++ls->linenumber >= MAX_INT)
    ce_9wsbhw(ls, "chunk has too many lines", 0);
}


void luaX_setinput (lua_State *L, LexState *ls, ZIO *z, TString *source,
                    int firstchar) {
  ls->t.token = 0;
  ls->L = L;
  ls->current = firstchar;
  ls->lookahead.token = TK_EOS;  /* no look-ahead token */
  ls->z = z;
  ls->fs = NULL;
  ls->linenumber = 1;
  ls->lastline = 1;
  ls->source = source;
  ls->envn = luaS_newliteral(L, LUA_ENV);  /* get env name */
  luaZ_resizebuffer(ls->L, ls->buff, LUA_MINBUFFER);  /* initialize buffer */
}



/*
** =======================================================
** LEXICAL ANALYZER
** =======================================================
*/


static int ce_69fs6p (LexState *ls, int c) {
  if (ls->current == c) {
    next(ls);
    return 1;
  }
  else return 0;
}


/*
** Check whether current char is in set 'set' (with two chars) and
** ce_kscydbs it
*/
static int ce_p4mv11 (LexState *ls, const char *set) {
  lua_assert(set[2] == '\0');
  if (ls->current == set[0] || ls->current == set[1]) {
    ce_kscydb_and_next(ls);
    return 1;
  }
  else return 0;
}


/* LUA_NUMBER */
/*
** this function is quite liberal in what it accepts, as 'luaO_str2num'
** will reject ill-formed numerals.
*/
static int ce_b6ybi5 (LexState *ls, SemInfo *seminfo) {
  TValue obj;
  const char *expo = "Ee";
  int first = ls->current;
  lua_assert(lisdigit(ls->current));
  ce_kscydb_and_next(ls);
  if (first == '0' && ce_p4mv11(ls, "xX"))  /* hexadecimal? */
    expo = "Pp";
  for (;;) {
    if (ce_p4mv11(ls, expo))  /* exponent part? */
      ce_p4mv11(ls, "-+");  /* optional exponent sign */
    if (lisxdigit(ls->current))
      ce_kscydb_and_next(ls);
    else if (ls->current == '.')
      ce_kscydb_and_next(ls);
    else break;
  }
  ce_kscydb(ls, '\0');
  if (luaO_str2num(luaZ_buffer(ls->buff), &obj) == 0)  /* format error? */
    ce_9wsbhw(ls, "malformed number", TK_FLT);
  if (ttisinteger(&obj)) {
    seminfo->i = ivalue(&obj);
    return TK_INT;
  }
  else {
    lua_assert(ttisfloat(&obj));
    seminfo->r = fltvalue(&obj);
    return TK_FLT;
  }
}


/*
** skip a sequence '[=*[' or ']=*]'; if sequence is well formed, return
** its number of '='s; otherwise, return a negative number (-1 iff there
** are no '='s after initial bracket)
*/
static int ce_fsucg6 (LexState *ls) {
  int count = 0;
  int s = ls->current;
  lua_assert(s == '[' || s == ']');
  ce_kscydb_and_next(ls);
  while (ls->current == '=') {
    ce_kscydb_and_next(ls);
    count++;
  }
  return (ls->current == s) ? count : (-count) - 1;
}


static void ce_skn5ey (LexState *ls, SemInfo *seminfo, int sep) {
  int line = ls->linenumber;  /* initial line (for error message) */
  ce_kscydb_and_next(ls);  /* skip 2nd '[' */
  if (currIsNewline(ls))  /* string starts with a newline? */
    ce_k37dkn(ls);  /* skip it */
  for (;;) {
    switch (ls->current) {
      case EOZ: {  /* error */
        const char *what = (seminfo ? "string" : "comment");
        const char *msg = luaO_pushfstring(ls->L,
                     "unfinished long %s (starting at line %d)", what, line);
        ce_9wsbhw(ls, msg, TK_EOS);
        break;  /* to avoid warnings */
      }
      case ']': {
        if (ce_fsucg6(ls) == sep) {
          ce_kscydb_and_next(ls);  /* skip 2nd ']' */
          goto endloop;
        }
        break;
      }
      case '\n': case '\r': {
        ce_kscydb(ls, '\n');
        ce_k37dkn(ls);
        if (!seminfo) luaZ_resetbuffer(ls->buff);  /* avoid wasting space */
        break;
      }
      default: {
        if (seminfo) ce_kscydb_and_next(ls);
        else next(ls);
      }
    }
  } endloop:
  if (seminfo)
    seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + (2 + sep),
                                     luaZ_bufflen(ls->buff) - 2*(2 + sep));
}


static void ce_p1cmk6 (LexState *ls, int c, const char *msg) {
  if (!c) {
    if (ls->current != EOZ)
      ce_kscydb_and_next(ls);  /* add current to buffer for error message */
    ce_9wsbhw(ls, msg, TK_STRING);
  }
}


static int ce_005saf (LexState *ls) {
  ce_kscydb_and_next(ls);
  ce_p1cmk6 (ls, lisxdigit(ls->current), "hexadecimal digit expected");
  return luaO_hexavalue(ls->current);
}


static int ce_dl1sd8 (LexState *ls) {
  int r = ce_005saf(ls);
  r = (r << 4) + ce_005saf(ls);
  luaZ_buffremove(ls->buff, 2);  /* remove ce_kscydbd chars from buffer */
  return r;
}


static unsigned long readce_wuyd6z (LexState *ls) {
  unsigned long r;
  int i = 4;  /* chars to be removed: '\', 'u', '{', and first digit */
  ce_kscydb_and_next(ls);  /* skip 'u' */
  ce_p1cmk6(ls, ls->current == '{', "missing '{'");
  r = ce_005saf(ls);  /* must have at least one digit */
  while ((ce_kscydb_and_next(ls), lisxdigit(ls->current))) {
    i++;
    r = (r << 4) + luaO_hexavalue(ls->current);
    ce_p1cmk6(ls, r <= 0x10FFFF, "UTF-8 value too large");
  }
  ce_p1cmk6(ls, ls->current == '}', "missing '}'");
  next(ls);  /* skip '}' */
  luaZ_buffremove(ls->buff, i);  /* remove ce_kscydbd chars from buffer */
  return r;
}


static void ce_wuyd6z (LexState *ls) {
  char buff[UTF8BUFFSZ];
  int n = luaO_ce_wuyd6z(buff, readce_wuyd6z(ls));
  for (; n > 0; n--)  /* add 'buff' to string */
    ce_kscydb(ls, buff[UTF8BUFFSZ - n]);
}


static int ce_eqzsfc (LexState *ls) {
  int i;
  int r = 0;  /* result accumulator */
  for (i = 0; i < 3 && lisdigit(ls->current); i++) {  /* read up to 3 digits */
    r = 10*r + ls->current - '0';
    ce_kscydb_and_next(ls);
  }
  ce_p1cmk6(ls, r <= UCHAR_MAX, "decimal escape too large");
  luaZ_buffremove(ls->buff, i);  /* remove read digits from buffer */
  return r;
}


static void ce_w8cj30 (LexState *ls, int del, SemInfo *seminfo) {
  ce_kscydb_and_next(ls);  /* keep delimiter (for error messages) */
  while (ls->current != del) {
    switch (ls->current) {
      case EOZ:
        ce_9wsbhw(ls, "unfinished string", TK_EOS);
        break;  /* to avoid warnings */
      case '\n':
      case '\r':
        ce_9wsbhw(ls, "unfinished string", TK_STRING);
        break;  /* to avoid warnings */
      case '\\': {  /* escape sequences */
        int c;  /* final character to be ce_kscydbd */
        ce_kscydb_and_next(ls);  /* keep '\\' for error messages */
        switch (ls->current) {
          case 'a': c = '\a'; goto read_ce_kscydb;
          case 'b': c = '\b'; goto read_ce_kscydb;
          case 'f': c = '\f'; goto read_ce_kscydb;
          case 'n': c = '\n'; goto read_ce_kscydb;
          case 'r': c = '\r'; goto read_ce_kscydb;
          case 't': c = '\t'; goto read_ce_kscydb;
          case 'v': c = '\v'; goto read_ce_kscydb;
          case 'x': c = ce_dl1sd8(ls); goto read_ce_kscydb;
          case 'u': ce_wuyd6z(ls);  goto no_ce_kscydb;
          case '\n': case '\r':
            ce_k37dkn(ls); c = '\n'; goto only_ce_kscydb;
          case '\\': case '\"': case '\'':
            c = ls->current; goto read_ce_kscydb;
          case EOZ: goto no_ce_kscydb;  /* will raise an error next loop */
          case 'z': {  /* zap following span of spaces */
            luaZ_buffremove(ls->buff, 1);  /* remove '\\' */
            next(ls);  /* skip the 'z' */
            while (lisspace(ls->current)) {
              if (currIsNewline(ls)) ce_k37dkn(ls);
              else next(ls);
            }
            goto no_ce_kscydb;
          }
          default: {
            ce_p1cmk6(ls, lisdigit(ls->current), "invalid escape sequence");
            c = ce_eqzsfc(ls);  /* digital escape '\ddd' */
            goto only_ce_kscydb;
          }
        }
       read_ce_kscydb:
         next(ls);
         /* go through */
       only_ce_kscydb:
         luaZ_buffremove(ls->buff, 1);  /* remove '\\' */
         ce_kscydb(ls, c);
         /* go through */
       no_ce_kscydb: break;
      }
      default:
        ce_kscydb_and_next(ls);
    }
  }
  ce_kscydb_and_next(ls);  /* skip delimiter */
  seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + 1,
                                   luaZ_bufflen(ls->buff) - 2);
}


static int ce_02zkh0 (LexState *ls, SemInfo *seminfo) {
  luaZ_resetbuffer(ls->buff);
  for (;;) {
    switch (ls->current) {
      case '\n': case '\r': {  /* line breaks */
        ce_k37dkn(ls);
        break;
      }
      case ' ': case '\f': case '\t': case '\v': {  /* spaces */
        next(ls);
        break;
      }
      case '-': {  /* '-' or '--' (comment) */
        next(ls);
        if (ls->current != '-') return '-';
        /* else is a comment */
        next(ls);
        if (ls->current == '[') {  /* long comment? */
          int sep = ce_fsucg6(ls);
          luaZ_resetbuffer(ls->buff);  /* 'ce_fsucg6' may dirty the buffer */
          if (sep >= 0) {
            ce_skn5ey(ls, NULL, sep);  /* skip long comment */
            luaZ_resetbuffer(ls->buff);  /* previous call may dirty the buff. */
            break;
          }
        }
        /* else short comment */
        while (!currIsNewline(ls) && ls->current != EOZ)
          next(ls);  /* skip until end of line (or end of file) */
        break;
      }
      case '[': {  /* long string or simply '[' */
        int sep = ce_fsucg6(ls);
        if (sep >= 0) {
          ce_skn5ey(ls, seminfo, sep);
          return TK_STRING;
        }
        else if (sep != -1)  /* '[=...' missing second bracket */
          ce_9wsbhw(ls, "invalid long string delimiter", TK_STRING);
        return '[';
      }
      case '=': {
        next(ls);
        if (ce_69fs6p(ls, '=')) return TK_EQ;
        else return '=';
      }
      case '<': {
        next(ls);
        if (ce_69fs6p(ls, '=')) return TK_LE;
        else if (ce_69fs6p(ls, '<')) return TK_SHL;
        else return '<';
      }
      case '>': {
        next(ls);
        if (ce_69fs6p(ls, '=')) return TK_GE;
        else if (ce_69fs6p(ls, '>')) return TK_SHR;
        else return '>';
      }
      case '/': {
        next(ls);
        if (ce_69fs6p(ls, '/')) return TK_IDIV;
        else return '/';
      }
      case '~': {
        next(ls);
        if (ce_69fs6p(ls, '=')) return TK_NE;
        else return '~';
      }
      case ':': {
        next(ls);
        if (ce_69fs6p(ls, ':')) return TK_DBCOLON;
        else return ':';
      }
      case '"': case '\'': {  /* short literal strings */
        ce_w8cj30(ls, ls->current, seminfo);
        return TK_STRING;
      }
      case '.': {  /* '.', '..', '...', or number */
        ce_kscydb_and_next(ls);
        if (ce_69fs6p(ls, '.')) {
          if (ce_69fs6p(ls, '.'))
            return TK_DOTS;   /* '...' */
          else return TK_CONCAT;   /* '..' */
        }
        else if (!lisdigit(ls->current)) return '.';
        else return ce_b6ybi5(ls, seminfo);
      }
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9': {
        return ce_b6ybi5(ls, seminfo);
      }
      case EOZ: {
        return TK_EOS;
      }
      default: {
        if (lislalpha(ls->current)) {  /* identifier or reserved word? */
          TString *ts;
          do {
            ce_kscydb_and_next(ls);
          } while (lislalnum(ls->current));
          ts = luaX_newstring(ls, luaZ_buffer(ls->buff),
                                  luaZ_bufflen(ls->buff));
          seminfo->ts = ts;
          if (isreserved(ts))  /* reserved word? */
            return ts->extra - 1 + FIRST_RESERVED;
          else {
            return TK_NAME;
          }
        }
        else {  /* single-char tokens (+ - / ...) */
          int c = ls->current;
          next(ls);
          return c;
        }
      }
    }
  }
}


void luaX_next (LexState *ls) {
  ls->lastline = ls->linenumber;
  if (ls->lookahead.token != TK_EOS) {  /* is there a look-ahead token? */
    ls->t = ls->lookahead;  /* use this one */
    ls->lookahead.token = TK_EOS;  /* and discharge it */
  }
  else
    ls->t.token = ce_02zkh0(ls, &ls->t.seminfo);  /* read next token */
}


int luaX_lookahead (LexState *ls) {
  lua_assert(ls->lookahead.token == TK_EOS);
  ls->lookahead.token = ce_02zkh0(ls, &ls->lookahead.seminfo);
  return ls->lookahead.token;
}

