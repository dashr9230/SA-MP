
#include <windows.h>
#include "detours.h"
#include "filehooks.h"
#include "filesystem.h"
#include "filechecks.h"
#include "runutil.h"
#include "checksums.h"
#include "game/game.h"

//----------------------------------------------------------

// Original procedures
def_GetFileSize Real_GetFileSize = NULL;
def_SetFilePointer Real_SetFilePointer = NULL;
def_CreateFileA Real_CreateFileA = NULL;
def_CreateFileW Real_CreateFileW = NULL;
def_ReadFile Real_ReadFile = NULL;
def_CloseHandle Real_CloseHandle = NULL;
def_GetFileType Real_GetFileType = NULL;
def_ShowCursor Real_ShowCursor = NULL;

extern CFileSystem *pFileSystem;
extern CGame *pGame;

ARCH_FILE_RECORD	OpenArchRecords[MAX_OPEN_ARCH_FILES];
BOOL				bArchRecordSlotState[MAX_OPEN_ARCH_FILES];

BOOL bFileHooksInstalled = FALSE;
int iCustomHandle=CUSTOM_HANDLE_BASE;

UINT dwHandlingDigest[] = { 0x11C714FD, 0x0A46694E, 0x7AEC2920, 0x2100E2E4, 0x94F7372E }; // handling.cfg
UINT dwWeaponDigest[]   = { 0x0B85A2AD, 0x344BDAE1, 0xA19471D5, 0x7D49D3E9, 0x7DDE6CBE }; // weapon.dat
UINT dwMeleeDigest[]    = { 0x7C425B89, 0xF67763C0, 0xA2E49F0E, 0x93EBF8D2, 0x0E842901 }; // melee.dat

char * FileNameOnly(char *sz);
char * ExtensionOnly(char *sz);
char* strtolower(char* sz);
void CheckFileNameHash(UINT *pDigest);

//----------------------------------------------------------

BOOL IsCustomFileHandle(DWORD handle)
{
	if(handle >= CUSTOM_HANDLE_BASE && handle < CUSTOM_HANDLE_LIMIT) {
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------

int FindUnusedArchRecordIndex()
{
	int x=0;
	while(x!=MAX_OPEN_ARCH_FILES) {
		if(bArchRecordSlotState[x] == FALSE) {
			return x;
		}
		x++;
	}
	return (-1);
}

//----------------------------------------------------------

int FindArchRecordIndexFromHandle(HANDLE hFile)
{
	int x=0;
	while(x!=MAX_OPEN_ARCH_FILES) {
		if(bArchRecordSlotState[x] == TRUE) {
			if(OpenArchRecords[x].hHandle == hFile) {
				return x;
			}
		}
		x++;
	}
	return (-1);
}

//----------------------------------------------------------

HANDLE CreateArchRecord(DWORD dwFileIndex)
{
	int iArchRecordIndex = FindUnusedArchRecordIndex();

	if(iArchRecordIndex == (-1)) return 0;

	bArchRecordSlotState[iArchRecordIndex] = TRUE;

	OpenArchRecords[iArchRecordIndex].hHandle = (HANDLE)iCustomHandle;
	iCustomHandle+=2;

	OpenArchRecords[iArchRecordIndex].dwReadPosition = 0;
	OpenArchRecords[iArchRecordIndex].dwFileSize = pFileSystem->GetFileSize(dwFileIndex);
	OpenArchRecords[iArchRecordIndex].pbyteDataStart = pFileSystem->GetFileData(dwFileIndex);
	OpenArchRecords[iArchRecordIndex].pbyteDataCurrent = OpenArchRecords[iArchRecordIndex].pbyteDataStart;

	return OpenArchRecords[iArchRecordIndex].hHandle;
}

//----------------------------------------------------------

HANDLE WINAPI Arch_CreateFileA( LPCTSTR lpFileName,DWORD dwDesiredAccess,
							   DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							   DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,
							   HANDLE hTemplateFile )
{
#ifdef _DEBUG
/*
	char szBuffer[FILENAME_MAX];
	sprintf(szBuffer, "CreateFileA: %s\n", lpFileName);
	OutputDebugString(szBuffer);
*/
#endif

	DWORD dwFileIndex = pFileSystem->GetFileIndex(FileNameOnly((PCHAR)lpFileName));

	HANDLE ret=0;

	if(dwFileIndex != FS_INVALID_FILE)
	{
		// The request file is in the archive, so we should return the handle.
		ret = CreateArchRecord(dwFileIndex);

		CHAR szFileName[MAX_PATH+1];
		strcpy(szFileName, FileNameOnly((PCHAR)lpFileName));
		strtolower(szFileName);
		UINT digest[5];
		CalcSHA1(szFileName, strlen(szFileName), digest);
		CheckFileNameHash(digest);
	}
	else
	{
		// Don't check if it's in the archive
		ret = Real_CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,
			lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
	}

	return ret;
}

//----------------------------------------------------------

HANDLE WINAPI Arch_CreateFileW( WORD * lpFileName,DWORD dwDesiredAccess,
								DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,
								HANDLE hTemplateFile )
{
#ifdef _DEBUG
/*
	wchar_t wszBuffer[FILENAME_MAX];
	swprintf(wszBuffer, L"CreateFileW: %s\n", lpFileName);
	OutputDebugStringW(wszBuffer);
*/
#endif

	HANDLE ret=Real_CreateFileW(lpFileName,dwDesiredAccess,dwShareMode,
		lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
	if (IsCheckableFile(ExtensionOnly((PCHAR)lpFileName)))
	{
		CheckFileHash(GetFileNameHash(strtolower(FileNameOnly((PCHAR)lpFileName))), ret);
	}
	return ret;
}

//----------------------------------------------------------

BOOL WINAPI Arch_ReadFile( HANDLE hFile,LPVOID lpBuffer,
						   DWORD nNumberOfBytesToRead,
						   LPDWORD lpNumberOfBytesRead,
						   LPOVERLAPPED lpOverlapped )
{
	int iArch=0;
	HANDLE hEvent=0;

	if( IsCustomFileHandle((DWORD)hFile) && 
		((iArch = FindArchRecordIndexFromHandle(hFile)) != (-1)))
	{
		//char s[256];
		//sprintf(s,"ReadFile(0x%X)",hFile);
		//OutputDebugString(s);

		DWORD dwFileSize = OpenArchRecords[iArch].dwFileSize;
		DWORD dwNumBytesRead;

		if(lpOverlapped) {
			OpenArchRecords[iArch].dwReadPosition = lpOverlapped->Offset;
			OpenArchRecords[iArch].pbyteDataCurrent =
				OpenArchRecords[iArch].pbyteDataStart + lpOverlapped->Offset;
			hEvent = lpOverlapped->hEvent;
		}

		// First condition: EOF
		if(OpenArchRecords[iArch].dwReadPosition >= dwFileSize)
		{
			if(lpNumberOfBytesRead)	*lpNumberOfBytesRead = 0;
			if(hEvent) SetEvent(hEvent);

			SetLastError(ERROR_HANDLE_EOF);
			return TRUE;
		}

		DWORD dwEndPoint = (nNumberOfBytesToRead + OpenArchRecords[iArch].dwReadPosition);

		// Second condition: Read will exceed or match EOF
		if( dwEndPoint >= dwFileSize ) {

			DWORD dwOverHang = dwEndPoint - dwFileSize;
			dwNumBytesRead = nNumberOfBytesToRead - dwOverHang;

			memcpy(lpBuffer,OpenArchRecords[iArch].pbyteDataCurrent,dwNumBytesRead);
			OpenArchRecords[iArch].pbyteDataCurrent += dwNumBytesRead;
			OpenArchRecords[iArch].dwReadPosition += dwNumBytesRead;

			if(lpNumberOfBytesRead) *lpNumberOfBytesRead = dwNumBytesRead;
			if(hEvent) SetEvent(hEvent);

			SetLastError(ERROR_HANDLE_EOF);

			return TRUE;
		}

		// Read will not exceed EOF
		memcpy(lpBuffer,OpenArchRecords[iArch].pbyteDataCurrent,nNumberOfBytesToRead);

		dwNumBytesRead = nNumberOfBytesToRead;
		OpenArchRecords[iArch].pbyteDataCurrent += dwNumBytesRead;
		OpenArchRecords[iArch].dwReadPosition += dwNumBytesRead;

		if(lpNumberOfBytesRead)	*lpNumberOfBytesRead = dwNumBytesRead;
		if(hEvent) SetEvent(hEvent);

		return TRUE;
	}

	return Real_ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,
		lpNumberOfBytesRead,lpOverlapped);
}

//----------------------------------------------------------

DWORD WINAPI Arch_GetFileSize( HANDLE hFile, PDWORD pdwSize )
{
	int iArch;

	if( IsCustomFileHandle((DWORD)hFile) &&
		((iArch = FindArchRecordIndexFromHandle(hFile)) != (-1)))
	{
		//char s[256];
		//sprintf(s,"GetFileSize(0x%X)",hFile);
		//OutputDebugString(s);

		if (pdwSize)
			*pdwSize = 0;
		return OpenArchRecords[iArch].dwFileSize;
	}
	return Real_GetFileSize(hFile,pdwSize);
}

//----------------------------------------------------------

DWORD WINAPI Arch_SetFilePointer( HANDLE hFile,LONG lDistanceToMove,
								  PLONG lpDistanceToMoveHigh,DWORD dwMoveMethod )
{
	int iArch;

	if( IsCustomFileHandle((DWORD)hFile) && 
		((iArch = FindArchRecordIndexFromHandle(hFile)) != (-1)))
	{
		//char s[256];
		//sprintf(s,"SetFilePointer(0x%X)",hFile);
		//OutputDebugString(s);

		if(dwMoveMethod ==  FILE_BEGIN) {
			OpenArchRecords[iArch].dwReadPosition = lDistanceToMove;
			OpenArchRecords[iArch].pbyteDataCurrent = OpenArchRecords[iArch].pbyteDataStart + lDistanceToMove;
		}
		else if(dwMoveMethod == FILE_CURRENT) {
			OpenArchRecords[iArch].dwReadPosition += lDistanceToMove;
			OpenArchRecords[iArch].pbyteDataCurrent += lDistanceToMove;
		}
		else if(dwMoveMethod == FILE_END) {
			OpenArchRecords[iArch].dwReadPosition = OpenArchRecords[iArch].dwFileSize;
			OpenArchRecords[iArch].pbyteDataCurrent = 
				OpenArchRecords[iArch].pbyteDataStart + OpenArchRecords[iArch].dwFileSize;
		}
		return OpenArchRecords[iArch].dwReadPosition;
	}
	return Real_SetFilePointer(hFile,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);
}

//----------------------------------------------------------

BOOL WINAPI Arch_CloseHandle( HANDLE hObject )
{
	int iArch;

	if( IsCustomFileHandle((DWORD)hObject) &&
		((iArch = FindArchRecordIndexFromHandle(hObject)) != (-1)))
	{
		bArchRecordSlotState[iArch] = FALSE;
		return TRUE;
	}

	return Real_CloseHandle(hObject);
}

//----------------------------------------------------------

DWORD WINAPI Arch_GetFileType( HANDLE hFile )
{
	int iArch;

	if( IsCustomFileHandle((DWORD)hFile) &&
		((iArch = FindArchRecordIndexFromHandle(hFile)) != (-1)))
	{
		return FILE_TYPE_DISK;
	}

	return Real_GetFileType(hFile);
}

//----------------------------------------------------------

int WINAPI Arch_ShowCursor(BOOL bShow)
{
	if (bShow != FALSE) return 0;
	return -1;
}

//----------------------------------------------------------

void InstallShowCursorHook()
{
	Real_ShowCursor = (def_ShowCursor)DetourFunction(
		(PBYTE)DetourFindFunction("user32.dll", "ShowCursor"),
		(PBYTE)Arch_ShowCursor);
}

//----------------------------------------------------------

void InstallFileSystemHooks()
{
	if(!bFileHooksInstalled) {

		// reset our structures memory
		memset(OpenArchRecords,0,sizeof(ARCH_FILE_RECORD) * MAX_OPEN_ARCH_FILES);
		memset(bArchRecordSlotState,FALSE,sizeof(BOOL) * MAX_OPEN_ARCH_FILES);

		Real_CreateFileA = (def_CreateFileA)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "CreateFileA"),
			(PBYTE)Arch_CreateFileA);

		/*
		Real_CreateFileW = (def_CreateFileW)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "CreateFileW"),
			(PBYTE)Arch_CreateFileW);*/

		Real_ReadFile = (def_ReadFile)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "ReadFile"),
			(PBYTE)Arch_ReadFile);

		Real_GetFileSize = (def_GetFileSize)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "GetFileSize"),
			(PBYTE)Arch_GetFileSize);

		Real_SetFilePointer = (def_SetFilePointer)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "SetFilePointer"),
			(PBYTE)Arch_SetFilePointer);

		Real_CloseHandle = (def_CloseHandle)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "CloseHandle"),
			(PBYTE)Arch_CloseHandle);

		Real_GetFileType = (def_GetFileType)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "GetFileType"),
			(PBYTE)Arch_GetFileType);

		bFileHooksInstalled = TRUE;
	}
}

//----------------------------------------------------------

void UninstallFileSystemHooks()
{
	if(bFileHooksInstalled) {
		DetourRemove((PBYTE)Real_CreateFileA,(PBYTE)Arch_CreateFileA);
		DetourRemove((PBYTE)Real_ReadFile,(PBYTE)Arch_ReadFile);
		DetourRemove((PBYTE)Real_GetFileSize,(PBYTE)Arch_GetFileSize);
		DetourRemove((PBYTE)Real_SetFilePointer,(PBYTE)Arch_SetFilePointer);
		DetourRemove((PBYTE)Real_CloseHandle,(PBYTE)Arch_CloseHandle);
		DetourRemove((PBYTE)Real_GetFileType,(PBYTE)Arch_GetFileType);
		DetourRemove((PBYTE)Real_ShowCursor,(PBYTE)Arch_ShowCursor);
		bFileHooksInstalled = FALSE;
	}
}

//----------------------------------------------------------

char * FileNameOnly(char *sz)
{
	// remove the trailing space, if it's there.
	if(sz[strlen(sz) - 1] == ' ') {
		sz[strlen(sz) - 1] = '\0';
	}

	char * org = sz;
	char * search = sz + strlen(sz);

	while(search != org) {
		if(*search == '/' || *search == '\\') {
			return (search+1);
		}
		search--;
	}
	return org;
}

//----------------------------------------------------------

char * ExtensionOnly(char *sz)
{
	// remove the trailing space, if it's there.
	if(sz[strlen(sz) - 1] == ' ') {
		sz[strlen(sz) - 1] = '\0';
	}

	char * org = sz;
	char * search = sz + strlen(sz);

	while(search != org) {
		if(*search == '.') {
			return (search+1);
		}
		search--;
	}
	return org;
}

//----------------------------------------------------------

char* strtolower(char* sz)
{
	char* ret = sz;
	while (*sz)
	{
		*sz = tolower(*sz);
		sz++;
	}
	return ret;
}

//----------------------------------------------------------

void CheckFileNameHash(UINT *pDigest)
{
	if( pDigest[0] == dwHandlingDigest[0] &&
		pDigest[1] == dwHandlingDigest[1] &&
		pDigest[2] == dwHandlingDigest[2] &&
		pDigest[3] == dwHandlingDigest[3] &&
		pDigest[4] == dwHandlingDigest[4] )
	{
		if(pGame)
			pGame->FUNC_10062570();
	}

	if( pDigest[0] == dwWeaponDigest[0] &&
		pDigest[1] == dwWeaponDigest[1] &&
		pDigest[2] == dwWeaponDigest[2] &&
		pDigest[3] == dwWeaponDigest[3] &&
		pDigest[4] == dwWeaponDigest[4] )
	{
		if(pGame)
			pGame->FUNC_10062570();
	}

	if( pDigest[0] == dwMeleeDigest[0] &&
		pDigest[1] == dwMeleeDigest[1] &&
		pDigest[2] == dwMeleeDigest[2] &&
		pDigest[3] == dwMeleeDigest[3] &&
		pDigest[4] == dwMeleeDigest[4] )
	{
		if(pGame)
			pGame->FUNC_10062570();
	}
}

//----------------------------------------------------------
