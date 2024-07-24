
#include "../main.h"
#include "../runutil.h"

#include <Tlhelp32.h>

BOOL unknown_1014FDA4;

DWORD dwD3D9DllBaseAddr=0;
DWORD dwD3D9DllSize=0;

typedef HANDLE (WINAPI *CREATETOOLHELP32SNAPSHOT)(DWORD, DWORD);
typedef BOOL (WINAPI *MODULE32FIRST)(HANDLE, LPMODULEENTRY32);
typedef BOOL (WINAPI *MODULE32NEXT)(HANDLE, LPMODULEENTRY32);

//----------------------------------------------------------

int CheckDuplicateD3D9Dlls()
{
	CREATETOOLHELP32SNAPSHOT fnCreateToolhelp32Snapshot = NULL;
	MODULE32FIRST fnModule32First = NULL;
	MODULE32NEXT fnModule32Next = NULL;
	HANDLE hModuleSnap = NULL;
	MODULEENTRY32 me32;

	int iFoundD3D9Dll=0;
	char szModule[MAX_PATH+1];

	DWORD dwCurImageSize=0;
	DWORD dwCurDllBase=0;

	BYTE szD3D9Enc[9] = {0x8C,0x66,0x8C,0x27,0xC5,0x8C,0x8D,0x8D,0}; // d3d9.dll
	char *szD3D9Dec = K_DecodeString(szD3D9Enc);

	BYTE szKernel32Enc[13] = {0x6D,0xAC,0x4E,0xCD,0xAC,0x8D,0x66,0x46,0xC5,0x8C,0x8D,0x8D,0}; // kernel32.dll
	char *szKernel32Dec = K_DecodeString(szKernel32Enc);

	BYTE szCreateToolhelp32SnapshotEnc[25] = {0x68,0x4E,0xAC,0x2C,0x8E,0xAC,0x8A,0xED,0xED,0x8D,0xD,0xAC,0x8D,0xE,0x66,0x46,0x6A,0xCD,0x2C,0xE,0x6E,0xD,0xED,0x8E,0}; // CreateToolhelp32Snapshot
	BYTE szModule32FirstEnc[14] = {0xA9,0xED,0x8C,0xAE,0x8D,0xAC,0x66,0x46,0xC8,0x2D,0x4E,0x6E,0x8E,0}; // Module32First
	BYTE szModule32NextEnc[13] = {0xA9,0xED,0x8C,0xAE,0x8D,0xAC,0x66,0x46,0xC9,0xAC,0xF,0x8E,0}; // Module32Next

	HMODULE kernel32 = GetModuleHandle(szKernel32Dec);

	fnCreateToolhelp32Snapshot = (CREATETOOLHELP32SNAPSHOT)GetProcAddress(kernel32,K_DecodeString(szCreateToolhelp32SnapshotEnc));
	fnModule32First = (MODULE32FIRST)GetProcAddress(kernel32,K_DecodeString(szModule32FirstEnc));
	fnModule32Next = (MODULE32NEXT)GetProcAddress(kernel32,K_DecodeString(szModule32NextEnc));

	hModuleSnap = fnCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	if(hModuleSnap == INVALID_HANDLE_VALUE)	return 0;

	me32.dwSize = sizeof(MODULEENTRY32);

	if(!fnModule32First(hModuleSnap,&me32))
	{
		CloseHandle( hModuleSnap );
		return 0;
	}

	do
	{
		strcpy(szModule,me32.szModule);
		dwCurImageSize = me32.modBaseSize;
		dwCurDllBase = (DWORD)me32.modBaseAddr;

		if(!strcmp(szD3D9Dec,szModule))
		{
			dwD3D9DllBaseAddr = dwCurDllBase ^ 0xACACACAC;
			dwD3D9DllSize = dwCurImageSize;
			iFoundD3D9Dll++;
		}

	} while( fnModule32Next( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );
	return iFoundD3D9Dll;
}

//----------------------------------------------------------

void FUNC_1009DD50()
{
	unknown_1014FDA4 = TRUE;
}

//----------------------------------------------------------
