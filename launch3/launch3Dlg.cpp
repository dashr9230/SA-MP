// launch3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "launch3.h"
#include "launch3Dlg.h"

#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef BOOL (WINAPI* Proc_CreateProcessA)(LPCSTR lpApplicationName,
										   LPSTR lpCommandLine,
										   LPSECURITY_ATTRIBUTES lpProcessAttributes,
										   LPSECURITY_ATTRIBUTES lpThreadAttributes,
										   BOOL bInheritHandles,
										   DWORD dwCreationFlags,
										   LPVOID lpEnvironment,
										   LPCSTR lpCurrentDirectory,
										   LPSTARTUPINFOA lpStartupInfo,
										   LPPROCESS_INFORMATION lpProcessInformation);

BOOL WINAPI CreateProcessWithDllA(LPCSTR lpApplicationName,
								  LPSTR lpCommandLine,
								  LPSECURITY_ATTRIBUTES lpProcessAttributes,
								  LPSECURITY_ATTRIBUTES lpThreadAttributes,
								  BOOL bInheritHandles,
								  DWORD dwCreationFlags,
								  LPVOID lpEnvironment,
								  LPCSTR lpCurrentDirectory,
								  LPSTARTUPINFOA lpStartupInfo,
								  LPPROCESS_INFORMATION lpProcessInformation,
								  LPSTR lpDllFullPath,
								  Proc_CreateProcessA FuncAddress);
BOOL SuspendTidAndInjectCode(HANDLE hProcess, HANDLE hThread, DWORD dwFuncAddress, const BYTE * lpShellCode, size_t uCodeSize);
BYTE* mov_eax_xx(BYTE* lpCurAddress, DWORD eax);
BYTE* mov_ebx_xx(BYTE* lpCurAddress, DWORD ebx);
BYTE* mov_ecx_xx(BYTE* lpCurAddress, DWORD ecx);
BYTE* mov_edx_xx(BYTE* lpCurAddress, DWORD edx);
BYTE* mov_esi_xx(BYTE* lpCurAddress, DWORD esi);
BYTE* mov_edi_xx(BYTE* lpCurAddress, DWORD edi);
BYTE* mov_ebp_xx(BYTE* lpCurAddress, DWORD ebp);
BYTE* mov_esp_xx(BYTE* lpCurAddress, DWORD esp);
BYTE* push_xx(BYTE* lpCurAddress, DWORD dwAddress);
BYTE* mov_eip_xx(BYTE* lpCurAddress, DWORD eip, DWORD newEip);
BYTE* Call_xx(BYTE* lpCurAddress, DWORD eip, DWORD newEip);


/////////////////////////////////////////////////////////////////////////////

PROCESS_INFORMATION ProcessInformation;
STARTUPINFO StartupInfo;

void LaunchMod(LPCSTR lpPath, LPSTR lpParams)
{
	char szGtaExe[256];
	char szSampDll[256];

	sprintf(szGtaExe,"%s\\%s",lpPath,"gta_sa.exe");
	sprintf(szSampDll,"%s\\%s",lpPath,"samp.dll");

	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

	if (!CreateProcessWithDllA(szGtaExe,
		lpParams,
		NULL,
		NULL,
		FALSE,
		CREATE_DEFAULT_ERROR_MODE,
		NULL,
		lpPath,
		&StartupInfo,
		&ProcessInformation,
		szSampDll,
		NULL))
	{
		MessageBoxA(NULL, "Initialization failed.\r\nPlease reinstall.", "SA:MP", MB_OK | MB_ICONERROR | MB_HELP);	
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLaunch3Dlg dialog

CLaunch3Dlg::CLaunch3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaunch3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaunch3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLaunch3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaunch3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLaunch3Dlg, CDialog)
	//{{AFX_MSG_MAP(CLaunch3Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_NICK, OnChangeNick)
	ON_BN_CLICKED(IDC_LAUNCH, OnLaunch)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CString GetAppPath ()
{
TCHAR app_path[_MAX_PATH];

GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, app_path, MAX_PATH);
CString app_str = app_path;
app_str = app_str.Left(app_str.ReverseFind('\\')+1);

return app_str;
}

/////////////////////////////////////////////////////////////////////////////
// CLaunch3Dlg message handlers

BOOL CLaunch3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaunch3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLaunch3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLaunch3Dlg::OnChangeNick() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CLaunch3Dlg::OnLaunch() 
{
	LaunchMod(GetAppPath(), "-d");
}

void CLaunch3Dlg::OnButton2() 
{
	this->OnCancel();
}

void CLaunch3Dlg::OnButton1() 
{
	LaunchMod(GetAppPath(), "-c -h 127.0.0.1 -p 7777 -n Player");
}

DWORD GetFuncAddress()
{
	return (DWORD)GetProcAddress(GetModuleHandleA("Kernel32"), "LoadLibraryA");
}

BOOL WINAPI CreateProcessWithDllA(LPCSTR lpApplicationName,
								  LPSTR lpCommandLine,
								  LPSECURITY_ATTRIBUTES lpProcessAttributes,
								  LPSECURITY_ATTRIBUTES lpThreadAttributes,
								  BOOL bInheritHandles,
								  DWORD dwCreationFlags,
								  LPVOID lpEnvironment,
								  LPCSTR lpCurrentDirectory,
								  LPSTARTUPINFOA lpStartupInfo,
								  LPPROCESS_INFORMATION lpProcessInformation,
								  LPSTR lpDllFullPath,
								  Proc_CreateProcessA FuncAddress)
{
	BOOL bResult = FALSE;
	size_t uCodeSize;
	DWORD dwCreaFlags = dwCreationFlags;
	PROCESS_INFORMATION pi;

	if (FuncAddress == NULL)
	{
		FuncAddress = CreateProcessA;
	}

	dwCreaFlags = dwCreationFlags | CREATE_SUSPENDED;

	if (FuncAddress(lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreaFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		&pi))
	{
		if (lpDllFullPath)
			uCodeSize = strlen(lpDllFullPath) + 1;
		else
			uCodeSize = 0;

		DWORD dwLoadDllProc = GetFuncAddress();

		if (SuspendTidAndInjectCode(pi.hProcess, pi.hThread, dwLoadDllProc, (BYTE*)lpDllFullPath, uCodeSize))
		{
			if (lpProcessInformation)
				memcpy(lpProcessInformation, &pi, sizeof(PROCESS_INFORMATION));

			if (!(dwCreationFlags & CREATE_SUSPENDED))
				ResumeThread(pi.hThread);

			bResult = TRUE;
		}
	}

	return bResult;
}

BOOL SuspendTidAndInjectCode(HANDLE hProcess, HANDLE hThread, DWORD dwFuncAddress, const BYTE * lpShellCode, size_t uCodeSize)
{
	BYTE ShellCodeBuf[0x480];
	CONTEXT Context;
	DWORD flOldProtect = 0;
	SIZE_T NumberOfBytesWritten = 0;
	LPBYTE lpCurESPAddress;
	LPBYTE lpCurBufAddress;
	BOOL bResult = FALSE;

	SuspendThread(hThread);

	memset(&Context,0,sizeof(Context));
	Context.ContextFlags = CONTEXT_FULL;

	if (GetThreadContext(hThread, &Context))
	{
		lpCurESPAddress = (LPBYTE)((Context.Esp - 0x480) & 0xFFFFFFE0);

		lpCurBufAddress = &ShellCodeBuf[0];

		if (lpShellCode)
		{
			memcpy(ShellCodeBuf + 128, lpShellCode, uCodeSize);
			lpCurBufAddress = push_xx(lpCurBufAddress, (DWORD)(lpCurESPAddress + 128));
			lpCurBufAddress = Call_xx(lpCurBufAddress, dwFuncAddress, (DWORD)lpCurESPAddress + (DWORD)lpCurBufAddress - (DWORD)&ShellCodeBuf);
		}

		lpCurBufAddress = mov_eax_xx(lpCurBufAddress, Context.Eax);
        lpCurBufAddress = mov_ebx_xx(lpCurBufAddress, Context.Ebx);
        lpCurBufAddress = mov_ecx_xx(lpCurBufAddress, Context.Ecx);
        lpCurBufAddress = mov_edx_xx(lpCurBufAddress, Context.Edx);
        lpCurBufAddress = mov_esi_xx(lpCurBufAddress, Context.Esi);
        lpCurBufAddress = mov_edi_xx(lpCurBufAddress, Context.Edi);
        lpCurBufAddress = mov_ebp_xx(lpCurBufAddress, Context.Ebp);
        lpCurBufAddress = mov_esp_xx(lpCurBufAddress, Context.Esp);
		lpCurBufAddress = mov_eip_xx(lpCurBufAddress, Context.Eip, (DWORD)lpCurESPAddress + (DWORD)lpCurBufAddress - (DWORD)&ShellCodeBuf);
		Context.Esp = (DWORD)(lpCurESPAddress - 4);
		Context.Eip = (DWORD)lpCurESPAddress;

		if (VirtualProtectEx(hProcess, lpCurESPAddress, 0x480, PAGE_EXECUTE_READWRITE, &flOldProtect)
			&& WriteProcessMemory(hProcess, lpCurESPAddress, &ShellCodeBuf, 0x480, &NumberOfBytesWritten)
			&& FlushInstructionCache(hProcess, lpCurESPAddress, 0x480)
			&& SetThreadContext(hThread, &Context) )
		{
			bResult = TRUE;
		}
	}

	ResumeThread(hThread);

	return bResult;
}

BYTE* mov_eax_xx(BYTE* lpCurAddress, DWORD eax)
{
	*lpCurAddress = 0xB8;
	*(DWORD*)(lpCurAddress + 1) = eax;
	return lpCurAddress + 5;
}

BYTE* mov_ebx_xx(BYTE* lpCurAddress, DWORD ebx)
{
	*lpCurAddress = 0xBB;
	*(DWORD*)(lpCurAddress + 1) = ebx;
	return lpCurAddress + 5;
}

BYTE* mov_ecx_xx(BYTE* lpCurAddress, DWORD ecx)
{
	*lpCurAddress = 0xB9;
	*(DWORD*)(lpCurAddress + 1) = ecx;
	return lpCurAddress + 5;
}

BYTE* mov_edx_xx(BYTE* lpCurAddress, DWORD edx)
{
	*lpCurAddress = 0xBA;
	*(DWORD*)(lpCurAddress + 1) = edx;
	return lpCurAddress + 5;
}

BYTE* mov_esi_xx(BYTE* lpCurAddress, DWORD esi)
{
	*lpCurAddress = 0xBE;
	*(DWORD*)(lpCurAddress + 1) = esi;
	return lpCurAddress + 5;
}

BYTE* mov_edi_xx(BYTE* lpCurAddress, DWORD edi)
{
	*lpCurAddress = 0xBF;
	*(DWORD*)(lpCurAddress + 1) = edi;
	return lpCurAddress + 5;
}

BYTE* mov_ebp_xx(BYTE* lpCurAddress, DWORD ebp)
{
	*lpCurAddress = 0xBD;
	*(DWORD*)(lpCurAddress + 1) = ebp;
	return lpCurAddress + 5;
}

BYTE* mov_esp_xx(BYTE* lpCurAddress, DWORD esp)
{
	*lpCurAddress = 0xBC;
	*(DWORD*)(lpCurAddress + 1) = esp;
	return lpCurAddress + 5;
}

BYTE* push_xx(BYTE* lpCurAddress, DWORD dwAddress)
{
	*lpCurAddress = 0x68;
	*(DWORD*)(lpCurAddress + 1) = dwAddress;
	return lpCurAddress + 5;
}

BYTE* mov_eip_xx(BYTE* lpCurAddress, DWORD eip, DWORD newEip)
{
	if (!newEip)
	{
		newEip = (DWORD)lpCurAddress;
	}
	*lpCurAddress = 0xE9;
	*(DWORD*)(lpCurAddress + 1) = eip - (newEip + 5);
	return lpCurAddress + 5;
}

BYTE* Call_xx(BYTE* lpCurAddress, DWORD eip, DWORD newEip)
{
	if (!newEip)
	{
		newEip = (DWORD)lpCurAddress;
	}
	*lpCurAddress = 0xE8;
	*(DWORD*)(lpCurAddress + 1) = eip - (newEip + 5);
	return lpCurAddress + 5;
}
