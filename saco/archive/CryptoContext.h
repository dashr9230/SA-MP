
#pragma once

#include <windows.h>

class CCryptoContext
{
private:
	static DWORD ms_dwRefCount;
	static DWORD ms_dwProviderType;
	static LPTSTR ms_szProviderName;
	static LPTSTR ms_szContainerName;

	HCRYPTPROV m_hCryptProv;

public:
	CCryptoContext(void);
	~CCryptoContext(void);

	HCRYPTPROV GetProvider();
};
