
#ifdef WIN32

#include "main.h"
#include <Tlhelp32.h>

PCONTEXT pContextRecord;
CHAR	 szErrorString[16384];

//----------------------------------------------------

void DumpLoadedModules(PCHAR sz)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32;
	char s[16384];

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	strcpy(sz, "\r\nLoaded Modules:\r\n");

	if(hModuleSnap == INVALID_HANDLE_VALUE)
	{
		strcat(sz, "-FailedCreate-\r\n");
		return;
	}

	me32.dwSize = sizeof( MODULEENTRY32 );

	if( !Module32First( hModuleSnap, &me32 ) )
	{
		strcat(sz, "-FailedFirst-" );  // Show cause of failure
		CloseHandle( hModuleSnap );     // Must clean up the snapshot object!
		return;
	}

	do
	{
		if (me32.szModule[0] != 'f' && me32.szModule[1] != 'l' && me32.szModule[2] != 'a')
		{
			sprintf(s, "%s\tA: 0x%08X - 0x%08X\t(%s)\r\n",
				me32.szModule, me32.modBaseAddr, me32.modBaseAddr + me32.modBaseSize, me32.szExePath);
			strcat(sz, s);
		}
	} while( Module32Next( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );

	return;
}

//----------------------------------------------------

void DumpModuleName(BYTE *pData, PCHAR sz)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32;

	if(!sz) return;

	sprintf(sz, "(Unknown)");

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if(hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	me32.dwSize = sizeof( MODULEENTRY32 );

	if( !Module32First( hModuleSnap, &me32 ) )
	{
		CloseHandle( hModuleSnap );     // Must clean up the snapshot object!
		return;
	}

	do
	{
		if(pData >= me32.modBaseAddr && pData <= me32.modBaseAddr + me32.modBaseSize)
		{
			sprintf(sz, "(%s)", me32.szModule);
			CloseHandle( hModuleSnap );
			return;
		}
	} while( Module32Next( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );

	return;
}

//----------------------------------------------------

void DumpMemory(BYTE *pData, DWORD dwCount, PCHAR sz, BOOL bAsDWords = FALSE)
{
	char s[16384];

	if (bAsDWords)
	{
		for(int i=0; i<(int)dwCount; i += 16)
		{
			sprintf(s, "+%04X: 0x%08X   0x%08X   0x%08X   0x%08X\r\n", i,
					*(DWORD*)(pData+i+0), *(DWORD*)(pData+i+4),
					*(DWORD*)(pData+i+8), *(DWORD*)(pData+i+12)
				);
			strcat(sz,s);
		}
	}
	else
	{
		for(int i=0; i<(int)dwCount; i += 16)
		{
			sprintf(s, "+%04X: %02X %02X %02X %02X   %02X %02X %02X %02X   "
					"%02X %02X %02X %02X   %02X %02X %02X %02X\r\n", i,
					pData[i+0], pData[i+1], pData[i+2], pData[i+3],
					pData[i+4], pData[i+5], pData[i+6], pData[i+7],
					pData[i+8], pData[i+9], pData[i+10], pData[i+11],
					pData[i+12], pData[i+13], pData[i+14], pData[i+15]
				);
			strcat(sz,s);
		}
	}
}

//----------------------------------------------------

void DumpCrashInfo(char * szFileName)
{
	DWORD *pdwStack;
	int x=0;

	FILE *f = fopen(szFileName,"a");
	if(!f) return; // nothing we can do

	fputs("\r\n--------------------------\r\n", f);

	char szModuleName[MAX_PATH];
	DumpModuleName((BYTE*)pContextRecord->Eip, szModuleName);

	sprintf(szErrorString,
		"SA-MP Server: %s\r\n\r\n"
		"Exception At Address: 0x%08X Module: %s\r\n\r\n"
		"Registers:\r\n"
		"EAX: 0x%08X\tEBX: 0x%08X\tECX: 0x%08X\tEDX: 0x%08X\r\n"
		"ESI: 0x%08X\tEDI: 0x%08X\tEBP: 0x%08X\tESP: 0x%08X\r\n"
		"EFLAGS: 0x%08X\r\n\r\nStack:\r\n",
		SAMP_VERSION,
		pContextRecord->Eip,
		szModuleName,
		pContextRecord->Eax,
		pContextRecord->Ebx,
		pContextRecord->Ecx,
		pContextRecord->Edx,
		pContextRecord->Esi,
		pContextRecord->Edi,
		pContextRecord->Ebp,
		pContextRecord->Esp,
		pContextRecord->EFlags);

	pdwStack = (DWORD *)pContextRecord->Esp;
	DumpMemory(reinterpret_cast<BYTE*>(pdwStack), 320, szErrorString, TRUE);

	fputs(szErrorString,f);

    fputs("\r\n--------------------------\r\n",f);

    DumpLoadedModules(szErrorString);

    fputs(szErrorString,f);
    fclose(f);
}

//----------------------------------------------------

LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf)
{
	pContextRecord = exc_inf->ContextRecord;

	DumpCrashInfo("crashinfo.txt");

	return EXCEPTION_EXECUTE_HANDLER;
}

//----------------------------------------------------

#endif
