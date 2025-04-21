
// This is intended to be a wrapper for Obfuscating calls
// to CryptoAPI. Make sure that LoadLibrary("ADVAPI32.DLL") is 
// somewhere (i.e. access some random reg key), otherwise this
// will fail.. miserably.

#pragma once

#include <windows.h>

#ifndef ARCTOOL

const CHAR CRYPT_MODULE[]				= "41#4%<fgU";
const CHAR CRYPT_FN_AcquireContext[]	= {0x16,0x27,0x2C,0x25,0x21,0x14,0x36,0x24,
											0x20,0x3C,0x27,0x30,0x16,0x3A,0x3B,0x21,
											0x30,0x2D,0x21,0x14,0x55,0x00,0x00,0x00};
const CHAR CRYPT_FN_ReleaseContext[]	= {0x16,0x27,0x2C,0x25,0x21,0x07,0x30,0x39,
											0x30,0x34,0x26,0x30,0x16,0x3A,0x3B,0x21,
											0x30,0x2D,0x21,0x55,0x00,0x00,0x00,0x00,};
const CHAR CRYPT_FN_CreateHash[]		= {0x16,0x27,0x2C,0x25,0x21,0x16,0x27,0x30,
											0x34,0x21,0x30,0x1D,0x34,0x26,0x3D,0x55};
const CHAR CRYPT_FN_DestroyHash[]		= {0x16,0x27,0x2C,0x25,0x21,0x11,0x30,0x26,
											0x21,0x27,0x3A,0x2C,0x1D,0x34,0x26,0x3D,
											0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const CHAR CRYPT_FN_HashData[]			= {0x16,0x27,0x2C,0x25,0x21,0x1D,0x34,0x26,
											0x3D,0x11,0x34,0x21,0x34,0x55,0x00,0x00};
const CHAR CRYPT_FN_DestroyKey[]		= {0x16,0x27,0x2C,0x25,0x21,0x11,0x30,0x26,
											0x21,0x27,0x3A,0x2C,0x1E,0x30,0x2C,0x55};
const CHAR CRYPT_FN_ImportKey[]			= {0x16,0x27,0x2C,0x25,0x21,0x1C,0x38,0x25,
											0x3A,0x27,0x21,0x1E,0x30,0x2C,0x55,0x00};
const CHAR CRYPT_FN_VerifySignature[]	= {0x16,0x27,0x2C,0x25,0x21,0x03,0x30,0x27,
											0x3C,0x33,0x2C,0x06,0x3C,0x32,0x3B,0x34,
											0x21,0x20,0x27,0x30,0x14,0x55,0x00,0x00};

#define CRYPT_GET_NAME(a,b)					\
	CHAR* a = NULL;							\
	{										\
		DWORD dwFnLen = strlen(b);			\
		a = new CHAR[dwFnLen];				\
		for(DWORD i=0; i<dwFnLen; i++)		\
			##a[i] = (CHAR)(##b[i] ^ 0x55);	\
	}

#ifndef CRYPT_FN_CPP

#define CRYPT_IMPL_FN(n,r,p)								\
	r WINAPI xCryptCall##n##p;

#else

// This is for the actual function when in the .CPP
#define CRYPT_IMPL_FN(n,r,p)								\
	DWORD xCryptOffs##n = NULL;								\
	__declspec(naked) r WINAPI xCryptCall##n##p				\
	{														\
		if (xCryptOffs##n == NULL) {						\
			_asm { lea eax, CRYPT_FN_##n };					\
			_asm { push eax };								\
			_asm { call xCryptGetFunction };				\
			_asm { add esp, 4 };							\
			_asm { mov dword ptr xCryptOffs##n, eax };		\
		}													\
		_asm { mov eax, dword ptr xCryptOffs##n };			\
		_asm { jmp eax };									\
	}

#endif

#ifdef CRYPT_FN_CPP

FARPROC xCryptGetFunction(CHAR* name, DWORD len)
{
	CRYPT_GET_NAME(szModName, CRYPT_MODULE);
	CRYPT_GET_NAME(szProcName, name);
	HMODULE hMod = GetModuleHandle(szModName);
	FARPROC fpProc = GetProcAddress(hMod, szProcName);
	return fpProc;
}

#endif

#define CRYPT(n)		\
	xCryptCall##n

CRYPT_IMPL_FN(AcquireContext,  BOOL, (HCRYPTPROV* phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags))
CRYPT_IMPL_FN(ReleaseContext,  BOOL, (HCRYPTPROV hProv, DWORD dwFlags))
CRYPT_IMPL_FN(CreateHash,      BOOL, (HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH* phHash))
CRYPT_IMPL_FN(DestroyHash,     BOOL, (HCRYPTHASH hHash))
CRYPT_IMPL_FN(HashData,        BOOL, (HCRYPTHASH hHash, BYTE* pbData, DWORD dwDataLen, DWORD dwFlags))
CRYPT_IMPL_FN(DestroyKey,      BOOL, (HCRYPTKEY hKey))
CRYPT_IMPL_FN(ImportKey,       BOOL, (HCRYPTPROV hProv, BYTE* pbData, DWORD dwDataLen, HCRYPTKEY hPubKey, DWORD dwFlags, HCRYPTKEY* phKey))
CRYPT_IMPL_FN(VerifySignature, BOOL, (HCRYPTHASH hHash, BYTE* pbSignature, DWORD dwSigLen, HCRYPTKEY hPubKey, LPCTSTR sDescription, DWORD dwFlags))

#else

#define CRYPT(n)		\
	Crypt##n

#endif
