// launch3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "launch3.h"
#include "launch3Dlg.h"

#include "detours.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	if (!DetourCreateProcessWithDll(szGtaExe,
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
