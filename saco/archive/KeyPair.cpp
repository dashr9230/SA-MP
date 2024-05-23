
#include "KeyPair.h"
#include "CryptoFns.h"
#include <stdio.h>

//------------------------------------

DWORD CKeyPair::ms_dwRSAKeySize = 1024;

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

#ifdef ARCTOOL
void CKeyPair::GenerateKey()
{
	// Generate a key pair
	HCRYPTPROV hCryptProv = m_pContext->GetProvider();
	CryptGenKey(hCryptProv, AT_SIGNATURE, (ms_dwRSAKeySize << 16) | CRYPT_EXPORTABLE, &m_hCryptKey);
}
#endif

//------------------------------------

void CKeyPair::ReleaseKey()
{
	// Destroy the key pair
	CRYPT(DestroyKey)(m_hCryptKey);
	m_hCryptKey = NULL;
}

//------------------------------------

#ifdef ARCTOOL
void CKeyPair::LoadFromFile(PCHAR szFileName)
{
	DWORD dwKeySize;
	BYTE *pbKeyBlob;
	FILE *fiKey;

	// Load the private key from a file
	fiKey = fopen((CHAR*)szFileName, "rb");
	fread(&dwKeySize, sizeof(dwKeySize), 1, fiKey);
	pbKeyBlob = new BYTE[dwKeySize];
	fread(pbKeyBlob, 1, dwKeySize, fiKey);
	fclose(fiKey);

	// Load the key pair
	HCRYPTPROV hCryptProv = m_pContext->GetProvider();
	CryptImportKey(hCryptProv, pbKeyBlob, dwKeySize, NULL, CRYPT_EXPORTABLE, &m_hCryptKey);

	// Clean up memory
	delete[] pbKeyBlob;
}
#endif

//------------------------------------

#ifdef ARCTOOL
void CKeyPair::WriteToFile(PCHAR szFileName)
{
	DWORD dwKeySize;
	BYTE *pbKeyBlob;
	FILE *fiKey;

	// Export the private key
	CryptExportKey(m_hCryptKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwKeySize);
	pbKeyBlob = new BYTE[dwKeySize];
	CryptExportKey(m_hCryptKey, NULL, PRIVATEKEYBLOB, 0, pbKeyBlob, &dwKeySize);

	// Write the private key to a file
	fiKey = fopen((CHAR*)szFileName, "wb");
	fwrite(&dwKeySize, sizeof(dwKeySize), 1, fiKey);
	fwrite(pbKeyBlob, 1, dwKeySize, fiKey);
	fclose(fiKey);

	// Clean up memory
	delete[] pbKeyBlob;
}
#endif

//------------------------------------

#ifdef ARCTOOL
void CKeyPair::WriteCHeaderFile(PSTR szFileName)
{
	const BYTE bytXORKey = 0xAA;
	DWORD dwKeySize;
	BYTE *pbKeyBlob;
	FILE *fiHeader;

	// Export the public key
	CryptExportKey(m_hCryptKey, NULL, PUBLICKEYBLOB, 0, NULL, &dwKeySize);
	pbKeyBlob = new BYTE[dwKeySize];
	CryptExportKey(m_hCryptKey, NULL, PUBLICKEYBLOB, 0, pbKeyBlob, &dwKeySize);

	// Generate the header file
	fiHeader = fopen(szFileName, "wt");
	fprintf(fiHeader, "//-----------------------------------------\n"	);
	fprintf(fiHeader, "// SAMP Archive 2 Tool - Public Keys\n"			);
	fprintf(fiHeader, "//\n"											);
	fprintf(fiHeader, "// This file was automatically generated.\n"		);
	fprintf(fiHeader, "// Do not modify this file!\n"					);
	fprintf(fiHeader, "//-----------------------------------------\n"	);
	fprintf(fiHeader, "\n"												);
	fprintf(fiHeader, "#pragma once\n"									);
	fprintf(fiHeader, "\n"												);
	fprintf(fiHeader, "#define RSA_XOR_KEY %d\n", bytXORKey				);
	fprintf(fiHeader, "\n"												);
	fprintf(fiHeader, "#define RSA_PUB_KEY_SIZE %d\n", dwKeySize		);
	fprintf(fiHeader, "const BYTE RSA_PUB_KEY[] = \n\t{"				);
	for(DWORD i=0; i<dwKeySize; i++) {
		fprintf(fiHeader, "0x%x%s", (pbKeyBlob[i] ^ bytXORKey), (i==(dwKeySize-1)?"};":",\t"));
		if (((i+1) % 8) == 0) {
			fprintf(fiHeader, "\n\t ");
		}
	}
	fclose(fiHeader);

	// Clean up
	delete[] pbKeyBlob;
}
#endif

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
