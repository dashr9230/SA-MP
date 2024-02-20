
#pragma once

#include <windows.h>

#include "ArchiveCommon.h"
#include "Stream.h"

#include "../mod.h"

#include "../filesystem.h"

#define FS_INVALID_FILE	0xFFFFFFFF

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

	_AFS_ENTRYBT_NODE* FindEntry(DWORD dwHash)
	{
		if (this->pEntry->dwFileNameHash == dwHash) {
			return this;
		} else {
			if (dwHash < this->pEntry->dwFileNameHash) {
				if (this->pLNode == NULL)
					return NULL;
				else
					return this->pLNode->FindEntry(dwHash);
			} else {
				if (this->pRNode == NULL)
					return NULL;
				else
					return this->pRNode->FindEntry(dwHash);
			}
		}
	}

	~_AFS_ENTRYBT_NODE()
	{
		if (this->pLNode != NULL)
			delete this->pLNode;
		if (this->pRNode != NULL)
			delete this->pRNode;
		if (this->pbData != NULL)
			delete[] this->pbData;
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

	static DWORD ms_dwHashInit;
	DWORD HashString(PCHAR szString);

public:
	CArchiveFS(void);
	CArchiveFS(DWORD dwNumEntries, DWORD dwFDSize);
	virtual ~CArchiveFS(void);

	virtual bool Load(char* szFileName);
	virtual bool Load(BYTE* pbData, DWORD nLength);
	virtual void Unload();

	virtual DWORD GetFileIndex(DWORD dwFileHash);
	virtual DWORD GetFileIndex(char* szFileName);
	virtual DWORD GetFileSize(DWORD dwFileIndex);
	virtual BYTE* GetFileData(DWORD dwFileIndex);

	virtual void UnloadData(DWORD dwFileIndex);
};
