
#pragma once

#include <windows.h>
#include "Hasher.h"
#include "KeyPair.h"

class CSigner
{
private:
	BYTE* m_pbSignature;
	DWORD m_dwLength;

public:
	CSigner(void);
	~CSigner(void);

#ifdef ARCTOOL
	void SignHash(CHasher* pHasher);
	BYTE* GetSignature();
	DWORD GetSignatureLength();
#endif

	void SetSignature(DWORD dwLength, BYTE* pbSignature);
	BOOL VerifySignature(CHasher* pHasher, CKeyPair* pKeyPair);

};
