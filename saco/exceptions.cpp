
#include "main.h"
#include "resource.h"
#include "httpclient.h"
#include "runutil.h"
#include "buildinfo.h"

#include <Tlhelp32.h>

PCONTEXT pContextRecord;
extern	 HANDLE		hInstance;
extern   CGame		*pGame;
extern   CNetGame	*pNetGame;
extern   DWORD		dwScmOpcodeDebug;
extern   BOOL       bScmLocalDebug;
extern   int		iGtaVersion;
extern	 WORD		wLastRendObj;

CHAR szErrorString[16384];

//----------------------------------------------------

void DumpNetworkStateInformation(PCHAR sz)
{
	// TODO: DumpNetworkStateInformation 10060160
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
		strcat(sz, "-FailedFirst-");	// Show cause of failure
		CloseHandle( hModuleSnap );		// Must clean up the snapshot object!
		return;
	}

	do
	{
		if (me32.szModule[0] != 'f' && me32.szModule[1] != 'l' && me32.szModule[2] != 'a')
		{
			sprintf(s, "%s\tB: 0x%08X\tS: 0x%08X\t(%s)\r\n",
				me32.szModule, me32.modBaseAddr, me32.modBaseSize, me32.szExePath);
			strcat(sz, s);
		}
	} while( Module32Next( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );

	return;
}

//----------------------------------------------------

void DumpMain(BOOL bIncModules)
{
	CHAR s[16384];
	DWORD *pdwStack;

	sprintf(szErrorString,
		"SA-MP %s\r\n"
		"Exception At Address: 0x%08X\r\n"
		"Base: 0x%08X\r\n\r\n"
		"Registers:\r\n"
		"EAX: 0x%08X\tEBX: 0x%08X\tECX: 0x%08X\tEDX: 0x%08X\r\n"
		"ESI: 0x%08X\tEDI: 0x%08X\tEBP: 0x%08X\tESP: 0x%08X\r\n"
		"EFLAGS: 0x%08X\r\n\r\nStack:\r\n",
		SAMP_VERSION,
		pContextRecord->Eip,
		hInstance,
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
	DumpMemory(reinterpret_cast<BYTE*>(pdwStack), 640, szErrorString, TRUE);

	sprintf(s,"\r\nSCM Op: 0x%X, lDbg: %d LastRendObj: %u\r\n", dwScmOpcodeDebug, bScmLocalDebug, wLastRendObj);
	strcat(szErrorString,s);

	const char *szGameVersion = (iGtaVersion == GTASA_VERSION_USA10) ? "US 1.0" :
								(iGtaVersion == GTASA_VERSION_EU10) ? "EU 1.0" :
								"UNKNOWN";
	sprintf(s, "\r\nGame Version: %s\r\n", szGameVersion);
	strcat(szErrorString,s);

	if(pNetGame)
	{
		DumpNetworkStateInformation(s);
		strcat(szErrorString,s);
	}

	if (bIncModules)
	{
		DumpLoadedModules(s);
		strcat(szErrorString,s);
	}
}

//----------------------------------------------------

void DoCrashReportingStuff()
{
	CHttpClient pHttp;
	char szURL[256];
	char szBase64[16384];

	DumpMain(TRUE);

	strcpy(szBase64,"data=");

	Util_Base64Encode(szErrorString,&szBase64[5]);

	sprintf(szURL,"team.sa-mp.com/report_037.php?addr=0x%X&gta=sa",pContextRecord->Eip);
	pHttp.ProcessURL(HTTP_POST,szURL,szBase64,"www.sa-mp.com");

	return;
}

//----------------------------------------------------

BOOL CALLBACK GuiDlgProcMain(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:

			DumpMain(FALSE);

			SetDlgItemText(hDlg,IDC_EDIT1,szErrorString);
			SetForegroundWindow(GetDlgItem(hDlg,IDD_DIALOG1));
			SetFocus(GetDlgItem(hDlg,IDC_BUTTON1));
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			ShowCursor(TRUE);
			break;

		case WM_DESTROY:
			EndDialog(hDlg,TRUE);
			ShowWindow(pGame->GetMainWindowHwnd(),SW_HIDE);
			DestroyWindow(pGame->GetMainWindowHwnd());
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON1:
				EndDialog(hDlg,TRUE);
				break;
			case IDC_BUTTON2:
				DoCrashReportingStuff();
				EnableWindow(GetDlgItem(hDlg,IDC_BUTTON2),FALSE);
				SetDlgItemTextA(hDlg,IDC_EDIT1,"Thanks for reporting this problem.");
				break;
			}
			break;
	}

	return FALSE;
}

//----------------------------------------------------

LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf)
{
	pContextRecord = exc_inf->ContextRecord;

	ShowWindow(pGame->GetMainWindowHwnd(),SW_MINIMIZE);
	DialogBox((HINSTANCE)hInstance,MAKEINTRESOURCE(IDD_DIALOG1),pGame->GetMainWindowHwnd(),(DLGPROC)GuiDlgProcMain);

	return EXCEPTION_EXECUTE_HANDLER;
}

//----------------------------------------------------