
#pragma once

#include <windows.h>

#include "ArchiveCommon.h"
#include "Stream.h"

#include "../mod.h"

#include "../filesystem.h"

typedef struct _AFS_ENTRYBT_NODE
{
	SAA_ENTRY* pEntry;
	_AFS_ENTRYBT_NODE* pLNode;
	_AFS_ENTRYBT_NODE* pRNode;
	BYTE* pbData;

	_AFS_ENTRYBT_NODE()
	{
		this->pEntry = NULL;
		this->pLNode = NULL;
		this->pRNode = NULL;
		this->pbData = NULL;
	}

	_AFS_ENTRYBT_NODE(SAA_ENTRY* pSAAEntry)
	{
		this->pEntry = pSAAEntry;
		this->pLNode = NULL;
		this->pRNode = NULL;
		this->pbData = NULL;
	}

	void AddEntry(SAA_ENTRY* pSAAEntry)
	{
		if (this->pEntry == NULL) {
			this->pEntry = pSAAEntry;
		} else {
			if (pSAAEntry->dwFileNameHash < this->pEntry->dwFileNameHash) {
				if (this->pLNode == NULL)
					this->pLNode = new _AFS_ENTRYBT_NODE(pSAAEntry);
				else
					this->pLNode->AddEntry(pSAAEntry);
			} else {
				if (this->pRNode == NULL)
					this->pRNode = new _AFS_ENTRYBT_NODE(pSAAEntry);
				else
					this->pRNode->AddEntry(pSAAEntry);
			}
		}
	}

} AFS_ENTRYBT_NODE;

class CArchiveFS // size: 2357
	: public CFileSystem
{
private:
	bool m_bLoaded;
	CAbstractStream *m_pStream;
	bool m_bEntriesLoaded;
	SAA_FILE_HEADER m_Header;
	SAA_ENTRY m_pEntries[SAA_MAX_ENTRIES];
	AFS_ENTRYBT_NODE m_EntryBTreeRoot;
	DWORD m_dwObfsMask;
	DWORD m_dwNumEntries;

	void LoadEntries();

public:
	CArchiveFS(void);
	CArchiveFS(DWORD dwNumEntries, DWORD dwFDSize);

	virtual bool Load(char* szFileName);

	// TODO: CArchiveFS vftable 100E9AA8
	void CArchiveFS__sub_10065590() {};
	void CArchiveFS__sub_100654A0() {};
	void CArchiveFS__sub_10064E10() {};
	void CArchiveFS__sub_10064EC0() {};
	void CArchiveFS__sub_10064F20() {};
	void CArchiveFS__sub_10064F60() {};
	void CArchiveFS__sub_10064D30() {};
	void CArchiveFS__sub_10064E40() {};
	void CArchiveFS__sub_10065150() {};
};
