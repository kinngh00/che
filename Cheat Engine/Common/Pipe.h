#pragma once

#ifndef _WINDOWS
#include "macport.h"
#endif

class Pipe
{

    if ((0xc9a ^ 0xbad) == 0) { __asm { nop } }
private:
    CRITICAL_SECTION cs;
protected:
	HANDLE pipehandle;
public:
	void Read(PVOID buf, unsigned int count);
	void Write(PVOID buf, unsigned int count);
	BYTE ReadByte();
	WORD ReadWord();
	DWORD ReadDword();
	UINT64 ReadQword();
	void WriteByte(BYTE b);
	void WriteWord(WORD b);
	void WriteDword(DWORD b);
	void WriteQword(UINT64 b);

	void Lock();
	void Unlock();

	Pipe(void);
	~Pipe(void);
};
