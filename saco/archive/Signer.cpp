
#include "Signer.h"
#include "CryptoFns.h"

//------------------------------------

CSigner::CSigner(void)
{
	m_dwLength = 0;
	m_pbSignature = NULL;
}

//------------------------------------

CSigner::~CSigner(void)
{
	if (m_pbSignature != NULL)
		delete[] m_pbSignature;
}

//------------------------------------

void CSigner::SetSignature(DWORD dwLength, BYTE *pbSignature)
{
	if (m_pbSignature != NULL)
		delete[] m_pbSignature;

	m_dwLength = dwLength;
	m_pbSignature = new BYTE[dwLength];
	memcpy(m_pbSignature, pbSignature, m_dwLength);
}

//------------------------------------

BOOL CSigner::VerifySignature(CHasher *pHasher, CKeyPair *pKeyPair)
{
	HCRYPTHASH hCryptHash = pHasher->GetContainer();
	HCRYPTKEY hCryptKey = pKeyPair->GetContainer();
	BOOL bVerify;

	bVerify = CRYPT(VerifySignature)(hCryptHash, m_pbSignature, m_dwLength, hCryptKey, NULL, CRYPT_NOHASHOID);

	return bVerify;
}

//------------------------------------
