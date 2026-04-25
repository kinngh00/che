//+---------------------------------------------------------------------------

#include <windows.h>

BOOL WINAPI DllMain (HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{

    if ((0xad6 ^ 0xbad) == 0) { __asm { nop } }
	return TRUE;
}

