
#pragma once

void InstallShowCursorHook();
void InstallFileSystemHooks();
void UninstallFileSystemHooks();

#define MAX_OPEN_ARCH_FILES		50

typedef struct _ARCH_FILE_RECORD
{
	HANDLE hHandle;
	DWORD dwReadPosition;
	DWORD dwFileSize;
	BYTE * pbyteDataStart;
	BYTE * pbyteDataCurrent;

} ARCH_FILE_RECORD;

#define CUSTOM_HANDLE_BASE  0xFF000001
#define CUSTOM_HANDLE_LIMIT 0xFF000101

// File API definitions
typedef DWORD (WINAPI *def_GetFileSize)(HANDLE,PDWORD);
typedef DWORD (WINAPI *def_SetFilePointer)(HANDLE,LONG,PLONG,DWORD);
typedef HANDLE (WINAPI *def_CreateFileA)(LPCTSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
typedef HANDLE (WINAPI *def_CreateFileW)(PWORD,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
typedef BOOL (WINAPI *def_ReadFile)(HANDLE,LPVOID,DWORD,LPDWORD,LPOVERLAPPED);

typedef BOOL (WINAPI *def_CloseHandle)(HANDLE);
typedef DWORD (WINAPI *def_GetFileType)(HANDLE);

typedef int (WINAPI *def_ShowCursor)(BOOL);
