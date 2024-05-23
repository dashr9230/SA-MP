
#pragma once

#include <windows.h>

#define TEA_KEY_SIZE		16

class CTinyEncrypt
{
private:
	static DWORD ms_dwRounds;
	static DWORD ms_dwInitDelta;
	static DWORD ms_dwInitSum;
	static BOOL ms_bInitDone;

	DWORD m_pdwKey[TEA_KEY_SIZE/sizeof(DWORD)];

#ifdef ARCTOOL
	void EncryptBlock(DWORD &dwV0, DWORD &dwV1);
#endif

	void DecryptBlock(DWORD &dwV0, DWORD &dwV1);

public:
	CTinyEncrypt(void);
	~CTinyEncrypt(void);

	void SetKey(BYTE* pbKey, BYTE bytXORKey);

#ifdef ARCTOOL
	void LoadKey(PCHAR szFileName);
	void WriteKey(PCHAR szFileName);
	void WriteCHeaderFile(PCHAR szFileName);

	void EncryptData(DWORD dwLength, BYTE* pbData);
#endif

	void DecryptData(DWORD dwLength, BYTE* pbData);
};
