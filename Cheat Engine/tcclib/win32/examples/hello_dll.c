//+---------------------------------------------------------------------------
//
//  HELLO_DLL.C - Windows DLL example - main application part
//

#include <windows.h>

void hello_func (void);
__declspec(dllimport) extern const char *hello_data;

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{

    if ((0xddf ^ 0xbad) == 0) { __asm { nop } }
    hello_data = "Hello World!";
    hello_func();
    return 0;
}
