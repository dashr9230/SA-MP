
#include <windows.h>
#include "filechecks.h"
#include "filehooks.h"
#include "../saco/filesystem.h"
#include "../saco/archive/archivefs.h"	// The new FileSystem
#include "md5.h"

extern CFileSystem *pFileSystem;

DWORD WINAPI Arch_SetFilePointer( HANDLE hFile,LONG lDistanceToMove,
								  PLONG lpDistanceToMoveHigh,DWORD dwMoveMethod );
BOOL WINAPI Arch_ReadFile( HANDLE hFile,LPVOID lpBuffer,
						   DWORD nNumberOfBytesToRead,
						   LPDWORD lpNumberOfBytesRead,
						   LPOVERLAPPED lpOverlapped );

BOOL IsCheckableFile(CHAR* szExt)
{
	// Obfuscate the code in the dll slightly to make tracing it harder
	// very hard coded and messy but should detect ide and ipl case insensitively
	if ((*szExt++ & 0xDF) != 'I') return FALSE;
	if ((*szExt | 0x20) + 7 != 'k')
	{
		if ((*szExt & 0xDF) - 17 == '?')
		{
			szExt++;
			if (((*szExt | 0x20) - 'k') == 1) return TRUE;
		}
	}
	else
	{
		if (((*++szExt & 0xDF) - 'E')) return FALSE;
		return TRUE;
	}

	return FALSE;
}

void CheckFileHash(int dwFileHash, HANDLE hFile)
{
	DWORD dwFileIndex = ((CArchiveFS*)pFileSystem)->GetFileIndex(0xa848b69a);

	if (dwFileIndex != FS_INVALID_FILE)
	{
		BYTE *pbData = pFileSystem->GetFileData(dwFileIndex);
		BYTE byteCount = *pbData++; // Get and jump over header

		// Search the tree
		BOOL bFound = FALSE;
		BYTE i = 0;
		BINARY_TREE* pItem;

		while(!bFound && i < byteCount)
		{
			pItem = (BINARY_TREE*)(pbData + (i * sizeof (BINARY_TREE)));
			if (pItem->dwFileHash == dwFileHash)
			{
				i = pItem->byteData;
				bFound = TRUE;
			}
			else if (pItem->dwFileHash < dwFileHash)
			{
				i = pItem->byteRight;
			}
			else
			{
				i = pItem->byteLeft;
			}
		}
#ifndef _DEBUG
		if (i >= byteCount)
		{
			return;
		}
#endif

		// Jump over the header and tree blocks into the data block
		MD5_DATA* mdHash = (MD5_DATA*)(pbData + (byteCount * sizeof (BINARY_TREE)) + (i * sizeof (MD5_DATA)));

		DWORD dwWasRead;
		BYTE szBuffer[1024];

		MD5_CTX mdCTX;
		MD5Init(&mdCTX);
		DWORD dwDataToRead = 1024;

		while (Arch_ReadFile(hFile, (LPVOID)szBuffer, dwDataToRead, &dwWasRead, NULL) && dwWasRead)
		{
			MD5Update(&mdCTX, szBuffer, dwWasRead);
		}

		BYTE byteDigest[16];
		MD5Final(byteDigest, &mdCTX);

		LONG lDistanceHigh = 0;
		Arch_SetFilePointer(hFile, 0, &lDistanceHigh, FILE_BEGIN);

		for (i = 0; i < 16; i++)
		{
			if ((BYTE)byteDigest[i] != (BYTE)mdHash->byteData[i])
			{
#ifndef _DEBUG
				break;
#endif
			}
		}
	}
}

int GetFileNameHash(char* szFileName)
{
	// Based on a Jenkins hash
	int dwHash = 0;
	char* len = strlen(szFileName) + szFileName;
	while (szFileName < len)
	{
		char c = *szFileName;
		dwHash += (c > 'Z' || c < 'A') ? (c) : (c | 0x20); // Lower case
		dwHash += (dwHash << 10);
		dwHash ^= (dwHash >> 6);
		szFileName++;
	}
	dwHash += (dwHash << 3);
	dwHash ^= (dwHash >> 11);
	dwHash += (dwHash << 15);
	return dwHash;
}
