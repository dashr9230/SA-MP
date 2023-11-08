
#include "TinyEncrypt.h"
#include "Obfuscator.h"

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

void CTinyEncrypt::DecryptData(DWORD dwLength, BYTE *pbData)
{
	DWORD dwBlocks = dwLength / 4;
	DWORD *pdwData = reinterpret_cast<DWORD*>(pbData);
	for(DWORD i=0; i<dwBlocks; i+=2) {
		DecryptBlock(pdwData[i+0], pdwData[i+1]);
	}
}

//------------------------------------
