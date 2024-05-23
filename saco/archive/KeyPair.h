
#pragma once

#include <windows.h>
#include "CryptoContext.h"

class CKeyPair
{
private:
	static DWORD ms_dwRSAKeySize;

	HCRYPTKEY m_hCryptKey;
	CCryptoContext* m_pContext;

public:
	CKeyPair(CCryptoContext* pContext);
	~CKeyPair(void);

	void LoadFromMemory(DWORD dwPubKeySize, BYTE* pbPubKeyBlob, BYTE bytXORKey);
	void ReleaseKey();

	HCRYPTKEY GetContainer();
};
