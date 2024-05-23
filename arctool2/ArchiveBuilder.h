
#pragma once

#include <windows.h>
#include <string.h>
#include <vector>

#include "../saco/archive/ArchiveCommon.h"

#pragma pack(1)

typedef struct _AB_ENTRY_MEMSTATE
{
	SAA_ENTRY*	pEntry;

	DWORD		dwEntryTableOffset;
	BYTE*		pbTEAKey;

	CHAR		szFileName[32];
	DWORD		dwFileNameHash;
	DWORD		dwFileSize;
	FILE*		fiFile;

	_AB_ENTRY_MEMSTATE()
	{
		dwEntryTableOffset = 0;
	}

} AB_ENTRY_MEMSTATE;

#pragma pack()

typedef std::vector<AB_ENTRY_MEMSTATE*> EntryMemStateVector_t;

class CArchiveBuilder
{
private:
	DWORD m_dwNumEntries;
	DWORD m_dwFDSize;
	BOOL  m_bProperHeader;

	EntryMemStateVector_t m_vecEntries;

	PCHAR m_szPrivKeyFile;
	PCHAR m_szTEAKeyFile;

	static DWORD ms_dwHashInit;
	DWORD HashString(PCHAR szString);
	PCHAR ExtractFileName(PCHAR szString);
	DWORD GetRandom();

	void BuildHeader(SAA_FILE_HEADER* pFileHeader);
	void BuildEntryTable(SAA_ENTRY* pEntryTable, DWORD dwInvalidIndex);
	void BuildEntryData(FILE* fiArchive);
	void SignArchive(FILE *fiArchive, DWORD dwSignatureOffset, SAA_FILE_HEADER *pHeader);

public:
	CArchiveBuilder(void);
	CArchiveBuilder(DWORD dwNumEntries, DWORD dwFDSize, BOOL bProperHeader);
	~CArchiveBuilder(void);

	void SetKeyFile(PCHAR szPrivKeyFile, PCHAR szTEAKeyFile);

	DWORD AddFile(PCHAR szFileName);
	void WriteArchive(PCHAR szFileName);
};
