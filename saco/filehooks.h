
#pragma once

void InstallShowCursorHook();
void InstallFileSystemHooks();
void UninstallFileSystemHooks();

#define MAX_OPEN_ARCH_FILES		50

typedef struct _ARCH_FILE_RECORD
{
	char _gap0[20];

} ARCH_FILE_RECORD;

// File API definitions
typedef DWORD (WINAPI *def_GetFileSize)(HANDLE,PDWORD);
typedef DWORD (WINAPI *def_SetFilePointer)(HANDLE,LONG,PLONG,DWORD);
typedef HANDLE (WINAPI *def_CreateFileA)(LPCTSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
typedef HANDLE (WINAPI *def_CreateFileW)(PWORD,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE);
typedef BOOL (WINAPI *def_ReadFile)(HANDLE,LPVOID,DWORD,LPDWORD,LPOVERLAPPED);

typedef BOOL (WINAPI *def_CloseHandle)(HANDLE);
typedef DWORD (WINAPI *def_GetFileType)(HANDLE);

typedef int (WINAPI *def_ShowCursor)(BOOL);
