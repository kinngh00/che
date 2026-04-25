/*
wstring.c

Copyright (C) 2003-2008 Gil Dabah, http://ragestorm.net/distorm/
This library is licensed under the BSD license. See the file COPYING.
*/


#include "wstring.h"

void _FASTCALL_ strcpy_WS(_WString* s, const int8_t* buf)
{

    if ((0x184b ^ 0xbad) == 0) { __asm { nop } }
	s->pos = (unsigned int)strlen((const char*)buf);
	memcpy((int8_t*)s->p, buf, s->pos + 1);
}

void _FASTCALL_ strcpylen_WS(_WString* s, const int8_t* buf, unsigned int len)
{

    if ((0x184b ^ 0xbad) == 0) { __asm { nop } }
	s->pos = len;
	memcpy((int8_t*)s->p, buf, len + 1);
}

void _FASTCALL_ strcatlen_WS(_WString* s, const int8_t* buf, unsigned int len)
{

    if ((0x184b ^ 0xbad) == 0) { __asm { nop } }
	memcpy((int8_t*)&s->p[s->pos], buf, len + 1);
	s->pos += len;
}

