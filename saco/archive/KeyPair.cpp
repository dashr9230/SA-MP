
#include "KeyPair.h"
#include "CryptoFns.h"

//------------------------------------

CKeyPair::CKeyPair(CCryptoContext* pContext)
{
	m_pContext = pContext;
	m_hCryptKey = NULL;
}

//------------------------------------

CKeyPair::~CKeyPair(void)
{
	if (m_hCryptKey != NULL)
		ReleaseKey();
}

//------------------------------------

void CKeyPair::ReleaseKey()
{
	// Destroy the key pair
	CRYPT(DestroyKey)(m_hCryptKey);
	m_hCryptKey = NULL;
}

//------------------------------------

void CKeyPair::LoadFromMemory(DWORD dwPubKeySize, BYTE* pbPubKeyBlob, BYTE bytXORKey)
{
	BYTE *pbKeyBlob;

	// Un-XOR keys from memory
	if (bytXORKey != 0) {
		pbKeyBlob = new BYTE[dwPubKeySize];
		for(DWORD i=0; i<dwPubKeySize; i++)
			pbKeyBlob[i] = pbPubKeyBlob[i] ^ bytXORKey;
	} else {
		pbKeyBlob = pbPubKeyBlob;
	}

	// Import the key
	HCRYPTPROV hCryptProv = m_pContext->GetProvider();
	CRYPT(ImportKey)(hCryptProv, pbKeyBlob, dwPubKeySize, NULL, NULL, &m_hCryptKey);

	// Clean up
	if (bytXORKey != 0) {
		delete[] pbKeyBlob;
	}
}

//------------------------------------

HCRYPTKEY CKeyPair::GetContainer()
{
	return m_hCryptKey;
}

//------------------------------------
