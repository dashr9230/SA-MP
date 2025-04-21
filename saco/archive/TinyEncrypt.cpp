
//
// This is a modified XTEA algorithm with a streaming
// mode of operation -- specifically CBC
//

#include "TinyEncrypt.h"
#include "Obfuscator.h"
#include <stdio.h>

//------------------------------------

DWORD CTinyEncrypt::ms_dwRounds = 32;
DWORD CTinyEncrypt::ms_dwInitDelta = OBFUSCATE_DATA(0x9E3779B9);
DWORD CTinyEncrypt::ms_dwInitSum = 0;
BOOL  CTinyEncrypt::ms_bInitDone = FALSE;

//------------------------------------

CTinyEncrypt::CTinyEncrypt(void)
{
	if (!ms_bInitDone) {
		ms_dwInitDelta = UNOBFUSCATE_DATA(ms_dwInitDelta);
		ms_dwInitSum = ms_dwInitDelta * ms_dwRounds;
		ms_bInitDone = TRUE;
	}
}

//------------------------------------

CTinyEncrypt::~CTinyEncrypt(void)
{
}

//------------------------------------

void CTinyEncrypt::SetKey(BYTE *pbKey, BYTE bytXORKey)
{
	memcpy(m_pdwKey, pbKey, TEA_KEY_SIZE);

	if (bytXORKey != 0)
	{
		BYTE *pbKeyRef = reinterpret_cast<BYTE*>(m_pdwKey);
		for(DWORD i=0; i<TEA_KEY_SIZE; i++)
			pbKeyRef[i] ^= bytXORKey;
	}
}

//------------------------------------

#ifdef ARCTOOL

void CTinyEncrypt::EncryptBlock(DWORD &dwV0, DWORD &dwV1)
{
	DWORD dwSum = 0;

	for(DWORD i=0; i<ms_dwRounds; i++) {
		dwV0 += ((dwV1 << 4 ^ dwV1 >> 5) + dwV1) ^ (dwSum + m_pdwKey[dwSum & 3]);
		dwSum += ms_dwInitDelta;
		dwV1 += ((dwV0 << 4 ^ dwV0 >> 5) + dwV0) ^ (dwSum + m_pdwKey[dwSum>>11 & 3]);
	}

	m_pdwKey[0] ^= dwV0;
	m_pdwKey[1] ^= dwV1;
	m_pdwKey[2] ^= dwV0;
	m_pdwKey[3] ^= dwV1;
}
#endif

//------------------------------------

void CTinyEncrypt::DecryptBlock(DWORD &dwV0, DWORD &dwV1)
{
	DWORD dwSum = ms_dwInitSum;

	DWORD dwV0old = dwV0;
	DWORD dwV1old = dwV1;

	for(DWORD i=0; i<ms_dwRounds; i++) {
		dwV1 -= ((dwV0 << 4 ^ dwV0 >> 5) + dwV0) ^ (dwSum + m_pdwKey[dwSum>>11 & 3]);
		dwSum -= ms_dwInitDelta;
		dwV0 -= ((dwV1 << 4 ^ dwV1 >> 5) + dwV1) ^ (dwSum + m_pdwKey[dwSum & 3]);
	}

	m_pdwKey[0] ^= dwV0old;
	m_pdwKey[1] ^= dwV1old;
	m_pdwKey[2] ^= dwV0old;
	m_pdwKey[3] ^= dwV1old;
}

//------------------------------------

#ifdef ARCTOOL
void CTinyEncrypt::EncryptData(DWORD dwLength, BYTE *pbData)
{
	DWORD dwBlocks = dwLength / 4;
	DWORD *pdwData = reinterpret_cast<DWORD*>(pbData);
	for(DWORD i=0; i<dwBlocks; i+=2) {
		EncryptBlock(pdwData[i+0], pdwData[i+1]);
	}
}
#endif

//------------------------------------

void CTinyEncrypt::DecryptData(DWORD dwLength, BYTE *pbData)
{
	DWORD dwBlocks = dwLength / 4;
	DWORD *pdwData = reinterpret_cast<DWORD*>(pbData);
	for(DWORD i=0; i<dwBlocks; i+=2) {
		DecryptBlock(pdwData[i+0], pdwData[i+1]);
	}
}

//------------------------------------

#ifdef ARCTOOL
void CTinyEncrypt::LoadKey(PCHAR szFileName)
{
	FILE* fiKey;

	fiKey = fopen((CHAR*)szFileName, "rb");
	fread(m_pdwKey, 1, TEA_KEY_SIZE, fiKey);
	fclose(fiKey);
}
#endif

//------------------------------------

#ifdef ARCTOOL
void CTinyEncrypt::WriteKey(PCHAR szFileName)
{
	FILE* fiKey;

	fiKey = fopen((CHAR*)szFileName, "wb");
	fwrite(m_pdwKey, 1, TEA_KEY_SIZE, fiKey);
	fclose(fiKey);
}
#endif

//------------------------------------

#ifdef ARCTOOL
void CTinyEncrypt::WriteCHeaderFile(PSTR szFileName)
{
	const BYTE bytXORKey = 0xAA;
	BYTE* pbKey = reinterpret_cast<BYTE*>(m_pdwKey);
	FILE *fiHeader;

	// Generate the header file
	fiHeader = fopen(szFileName, "wt");
	fprintf(fiHeader, "//-----------------------------------------\n"	);
	fprintf(fiHeader, "// SAMP Archive 2 Tool - TEA Keys\n"				);
	fprintf(fiHeader, "//\n"											);
	fprintf(fiHeader, "// This file was automatically generated.\n"		);
	fprintf(fiHeader, "// Do not modify this file!\n"					);
	fprintf(fiHeader, "//-----------------------------------------\n"	);
	fprintf(fiHeader, "\n"												);
	fprintf(fiHeader, "#pragma once\n"									);
	fprintf(fiHeader, "\n"												);
	fprintf(fiHeader, "#define TEA_XOR_KEY %d\n", bytXORKey				);
	fprintf(fiHeader, "\n"												);
	fprintf(fiHeader, "#define TEA_KEY_SIZE %d\n", TEA_KEY_SIZE			);
	fprintf(fiHeader, "const BYTE TEA_KEY[] = \n\t{"					);
	for(DWORD i=0; i<TEA_KEY_SIZE; i++) {
		fprintf(fiHeader, "0x%x%s", (pbKey[i] ^ bytXORKey), (i==(TEA_KEY_SIZE-1)?"};":",\t"));
		if (((i+1) % 8) == 0) {
			fprintf(fiHeader, "\n\t ");
		}
	}
	fclose(fiHeader);
}
#endif
