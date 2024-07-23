
#include "main.h"

extern CGame			*pGame;

WNDPROC hOldProc;
LRESULT APIENTRY NewWndProc(HWND,UINT,WPARAM,LPARAM);

//----------------------------------------------------

void InstallWindowProcedure()
{
	HWND hwndGameWnd = pGame->GetMainWindowHwnd();

	if(hwndGameWnd) {
		hOldProc = (WNDPROC)GetWindowLong(hwndGameWnd,GWL_WNDPROC);
		if(hOldProc != NewWndProc) {
			SetWindowLong(hwndGameWnd,GWL_WNDPROC,(LONG)NewWndProc);
		}
	}
}

//----------------------------------------------------

BOOL SubclassGameWindow()
{
	HWND hwndGameWnd = pGame->GetMainWindowHwnd();

	if(hwndGameWnd) {

		DWORD dwStyle = GetClassLong(hwndGameWnd,GCL_STYLE);
		SetClassLong(hwndGameWnd,GCL_STYLE,dwStyle|CS_DBLCLKS);

		InstallWindowProcedure();

		SetWindowText(hwndGameWnd,"GTA:SA:MP");

		if(IsWindowUnicode(hwndGameWnd)) {
			OutputDebugString("GTA is unicode");
		} else {
			OutputDebugString("GTA is not unicode");
		}
	
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------

LRESULT APIENTRY NewWndProc( HWND hwnd,UINT uMsg,
							 WPARAM wParam,LPARAM lParam ) 
{
	// TODO: NewWndProc

	return CallWindowProc(hOldProc,hwnd,uMsg,wParam,lParam);
}

//----------------------------------------------------
