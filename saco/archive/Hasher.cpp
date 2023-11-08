
#include "Hasher.h"
#include "CryptoFns.h"

//------------------------------------

DWORD CHasher::ms_dwHashAlgorithm = CALG_SHA1;

//------------------------------------

CHasher::CHasher(CCryptoContext* pContext)
{
	// Save context for later
	m_pContext = pContext;

	// Generate a hash container
	HCRYPTPROV hCryptProv = pContext->GetProvider();
	CRYPT(CreateHash)(hCryptProv, ms_dwHashAlgorithm, NULL, NULL, &m_hCryptHash);
}

//------------------------------------

CHasher::~CHasher(void)
{
	// Destory the hash container
	CRYPT(DestroyHash)(m_hCryptHash);
}

//------------------------------------

void CHasher::AddData(DWORD dwDataLength, BYTE *pbData)
{
	// Add the data to be hashed
	CRYPT(HashData)(m_hCryptHash, pbData, dwDataLength, 0);
}

//------------------------------------

HCRYPTKEY CHasher::GetContainer()
{
	return m_hCryptHash;
}

//------------------------------------
