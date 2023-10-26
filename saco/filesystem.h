
#pragma once

//----------------------------------------------------------

#define FS_FILE_MAGIC	0x32414153	// "SAA2"
#define FS_BLOCK_SIZE	0x800		// 2kb
#define FS_INVALID_FILE	0xFFFFFFFF
#define FS_ENCKEY_VAR	37625
#define FS_ENC_CONST1	54825
#define FS_ENC_CONST2	91722

//----------------------------------------------------------

typedef struct _FS_HEADER
{
	DWORD dwSAAV;
	DWORD dwFileCount;
	WORD  wKey;
} FS_HEADER;

typedef struct _FS_FILE_ENTRY
{
	DWORD dwOffset;
	DWORD dwSize;
	char szName[24];
	DWORD dwRealSize;
	WORD wKey;
} FS_FILE_ENTRY;

//----------------------------------------------------------

class CFileSystem
{
private:
	bool m_bLoaded;
	DWORD m_dwFileCount;
	WORD m_wKey;
	FS_FILE_ENTRY* m_pFileList;
	BYTE** m_pFileData;
public:
	CFileSystem();
	virtual ~CFileSystem();

	virtual bool Load(char* szFileName);
	virtual void Unload();

	virtual DWORD GetFileIndex(char* szFileName);
	virtual DWORD GetFileSize(DWORD dwFileIndex);
	virtual BYTE* GetFileData(DWORD dwFileIndex);

	void DecryptData(BYTE* pData, DWORD dwDataLen, WORD wKey);
};

//----------------------------------------------------------
// EOF