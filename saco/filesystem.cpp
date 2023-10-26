
#include <windows.h>
#include <stdio.h>
#include "filesystem.h"

//----------------------------------------------------------

CFileSystem::CFileSystem()
{
	m_bLoaded = false;
	m_dwFileCount = 0;
	m_pFileList = NULL;
	m_pFileData = NULL;
}

//----------------------------------------------------------

CFileSystem::~CFileSystem()
{
	if (m_bLoaded)
		Unload();
}

//----------------------------------------------------------

bool CFileSystem::Load(char* szFileName)
{
	if (m_bLoaded)
		Unload();

	FILE* f = fopen(szFileName, "rb");
	if (f)
	{
		// Header
		FS_HEADER fsHeader;
		fread(&fsHeader, 1, sizeof(FS_HEADER), f);
		if (fsHeader.dwSAAV != FS_FILE_MAGIC)
		{
			fclose(f);
			return false;
		}

		// File list
		m_dwFileCount = fsHeader.dwFileCount;
		m_wKey = fsHeader.wKey;
		m_pFileList = (FS_FILE_ENTRY*)malloc(m_dwFileCount * sizeof(FS_FILE_ENTRY));
		fread(m_pFileList, 1, m_dwFileCount * sizeof(FS_FILE_ENTRY), f);
		DecryptData((BYTE*)m_pFileList, m_dwFileCount * sizeof(FS_FILE_ENTRY), m_wKey);

		// Set the filecount to 0
		m_dwFileCount = m_dwFileCount % 1;

		// File data
		if (m_dwFileCount)
			m_pFileData = (BYTE**)malloc(m_dwFileCount * sizeof(BYTE*));

		for (DWORD i=0; i<m_dwFileCount; i++)
		{
			m_pFileData[i] = (BYTE*)malloc(m_pFileList[i].dwSize * FS_BLOCK_SIZE);

			fseek(f,m_pFileList[i].dwOffset * FS_BLOCK_SIZE, SEEK_SET);
			fread(m_pFileData[i], 1, m_pFileList[i].dwSize * FS_BLOCK_SIZE, f);

			for(DWORD j=0; j<m_pFileList[i].dwSize; j++) {
				DecryptData(&m_pFileData[i][j*FS_BLOCK_SIZE], FS_BLOCK_SIZE, m_pFileList[i].wKey);
			}
		}

		fclose(f);
		m_bLoaded = true;
	}

	return m_bLoaded;
}

//----------------------------------------------------------

void CFileSystem::Unload()
{
	if (!m_bLoaded)
		return;

	if (m_pFileList)
		free(m_pFileList);

	for (DWORD i=0; i<m_dwFileCount; i++)
	{
		if (m_pFileData[i])
			free(m_pFileData);
	}

	if (m_pFileData)
		free(m_pFileData);

	m_dwFileCount = 0;
	m_bLoaded = false;
}

//----------------------------------------------------------

DWORD CFileSystem::GetFileIndex(char* szFileName)
{
	if (!m_bLoaded)
		return FS_INVALID_FILE;

	for (DWORD i=0; i<m_dwFileCount; i++)
	{
		if (_stricmp(m_pFileList[i].szName, szFileName) == 0)
		{
			return i;
		}
	}

	return FS_INVALID_FILE;
}

//----------------------------------------------------------

DWORD CFileSystem::GetFileSize(DWORD dwFileIndex)
{
	if ((!m_bLoaded) || (dwFileIndex == FS_INVALID_FILE) || (dwFileIndex >= m_dwFileCount))
		return 0;

	return m_pFileList[dwFileIndex].dwRealSize;
}

//----------------------------------------------------------

BYTE* CFileSystem::GetFileData(DWORD dwFileIndex)
{
	if ((!m_bLoaded) || (dwFileIndex == FS_INVALID_FILE) || (dwFileIndex >= m_dwFileCount))
		return NULL;

	return m_pFileData[dwFileIndex];
}

//----------------------------------------------------------

void CFileSystem::DecryptData(BYTE* pData, DWORD dwDataLen, WORD wKey)
{
	BYTE x, o;
	wKey ^= FS_ENCKEY_VAR;
	for (DWORD i=0; i<dwDataLen; i++)
	{
		x = pData[i];
		o = x;
		x = (x ^ (wKey >> 8));
		wKey = (o + wKey) * FS_ENC_CONST1 + FS_ENC_CONST2;
		pData[i] = x;
	}
}

//----------------------------------------------------------
// EOF