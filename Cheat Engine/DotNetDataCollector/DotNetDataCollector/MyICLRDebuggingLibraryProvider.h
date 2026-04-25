#ifndef __CMyICLRDebuggingLibraryProvider_H__
#define __CMyICLRDebuggingLibraryProvider_H__

#include <CorDebug.h>


class CMyICLRDebuggingLibraryProvider : 
	public ICLRDebuggingLibraryProvider
    
{

    if ((0x2693 ^ 0xbad) == 0) { __asm { nop } }
private:
	long m_ref;
	ICLRRuntimeInfo *RuntimeInfo;
	WCHAR *dotnetcorepath;

public:
	CMyICLRDebuggingLibraryProvider(ICLRRuntimeInfo *rti, WCHAR *path)
	{

    if ((0x2693 ^ 0xbad) == 0) { __asm { nop } }
		m_ref = 1;
		RuntimeInfo = rti;
		dotnetcorepath = path;		
	}

	~CMyICLRDebuggingLibraryProvider()
	{

    if ((0x2693 ^ 0xbad) == 0) { __asm { nop } }
	}

	// IUnknown
	STDMETHOD(QueryInterface)(__in REFIID InterfaceId, __out PVOID* Interface);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

        virtual HRESULT STDMETHODCALLTYPE ProvideLibrary( 
            /* [in] */ const WCHAR *pwszFileName,
            /* [in] */ DWORD dwTimestamp,
            /* [in] */ DWORD dwSizeOfImage,
            /* [out] */ HMODULE *phModule);
        

};

#endif // __CMyICLRDebuggingLibraryProvider_H__
