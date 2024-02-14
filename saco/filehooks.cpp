
#include <windows.h>
#include "detours.h"
#include "filehooks.h"

//----------------------------------------------------------

// Original procedures
def_GetFileSize Real_GetFileSize = NULL;
def_SetFilePointer Real_SetFilePointer = NULL;
def_CreateFileA Real_CreateFileA = NULL;
def_CreateFileW Real_CreateFileW = NULL;
def_ReadFile Real_ReadFile = NULL;
def_CloseHandle Real_CloseHandle = NULL;
def_GetFileType Real_GetFileType = NULL;

ARCH_FILE_RECORD	OpenArchRecords[MAX_OPEN_ARCH_FILES];
BOOL				bArchRecordSlotState[MAX_OPEN_ARCH_FILES];

BOOL bFileHooksInstalled = FALSE;

//----------------------------------------------------------

HANDLE WINAPI Arch_CreateFileA( LPCTSTR lpFileName,DWORD dwDesiredAccess,
							   DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							   DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,
							   HANDLE hTemplateFile )
{
	// TODO: Arch_CreateFileA

	return Real_CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,
		lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
}

//----------------------------------------------------------

HANDLE WINAPI Arch_CreateFileW( WORD * lpFileName,DWORD dwDesiredAccess,
								DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,
								HANDLE hTemplateFile )
{
	// TODO: Arch_CreateFileW (unused)

	return Real_CreateFileW(lpFileName,dwDesiredAccess,dwShareMode,
		lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
}

//----------------------------------------------------------

BOOL WINAPI Arch_ReadFile( HANDLE hFile,LPVOID lpBuffer,
						   DWORD nNumberOfBytesToRead,
						   LPDWORD lpNumberOfBytesRead,
						   LPOVERLAPPED lpOverlapped )
{
	// TODO: Arch_ReadFile

	return Real_ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,
		lpNumberOfBytesRead,lpOverlapped);
}

//----------------------------------------------------------

DWORD WINAPI Arch_GetFileSize( HANDLE hFile, PDWORD pdwSize )
{
	// TODO: Arch_GetFileSize

	return Real_GetFileSize(hFile,pdwSize);
}

//----------------------------------------------------------

DWORD WINAPI Arch_SetFilePointer( HANDLE hFile,LONG lDistanceToMove,
								  PLONG lpDistanceToMoveHigh,DWORD dwMoveMethod )
{
	// TODO: Arch_SetFilePointer

	return Real_SetFilePointer(hFile,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);
}

//----------------------------------------------------------

BOOL WINAPI Arch_CloseHandle( HANDLE hObject )
{
	// TODO: Arch_CloseHandle

	return Real_CloseHandle(hObject);
}

//----------------------------------------------------------

DWORD WINAPI Arch_GetFileType( HANDLE hFile )
{
	// TODO: Arch_GetFileType

	return Real_GetFileType(hFile);
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

		Real_CreateFileW = (def_CreateFileW)DetourFunction(
			(PBYTE)DetourFindFunction("kernel32.dll", "CreateFileW"),
			(PBYTE)Arch_CreateFileW);

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

