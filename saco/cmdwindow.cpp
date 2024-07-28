
#include "main.h"

extern CGame		*pGame;
extern CConfig		*pConfig;

//----------------------------------------------------

CCmdWindow::CCmdWindow(IDirect3DDevice9 *pD3DDevice)
{
	m_bEnabled				= FALSE;
	m_pD3DDevice			= pD3DDevice;
	m_iCmdCount				= 0;
	field_1AF4				= 0;
	m_pEditControl			= NULL;
	field_1AF0				= -1;

	memset(&field_1565[0],0,1290);
	memset(&m_szInputBuffer[0],0,(MAX_CMD_INPUT+1));
	memset(&field_1A6F[0],0,129);
}

//----------------------------------------------------

CCmdWindow::~CCmdWindow()
{
}

//----------------------------------------------------

void CCmdWindow::ResetDialogControls(CDXUTDialog *pGameUI)
{
	m_pGameUI = pGameUI;

	if(pGameUI) {
		pGameUI->AddIMEEditBox(IDC_CMDEDIT,"",10,175,570,40,true,&m_pEditControl);
		if(pConfig->GetIntVariable("ime")) {
			CDXUTIMEEditBox::EnableImeSystem(true);
			CDXUTIMEEditBox::StaticOnCreateDevice();
		}
		m_pEditControl->GetElement(0)->TextureColor.Init(D3DCOLOR_ARGB( 240, 5, 5, 5 ));
		m_pEditControl->SetTextColor(D3DCOLOR_ARGB( 255, 255, 255, 255 ));
		m_pEditControl->SetCaretColor(D3DCOLOR_ARGB( 255, 150, 150, 150 ));
		m_pEditControl->SetSelectedBackColor(D3DCOLOR_ARGB( 255, 185, 34, 40 ));
		m_pEditControl->SetSelectedTextColor(D3DCOLOR_ARGB( 255, 10, 10, 15 ));
		m_pEditControl->SetEnabled(false);
		m_pEditControl->SetVisible(false);
	}
}

//----------------------------------------------------

void CCmdWindow::GetDialogSize(RECT *pRect)
{
	memset(pRect,0,sizeof(RECT));

	if(m_pGameUI) {
		pRect->right = m_pGameUI->GetWidth();
		pRect->bottom = m_pGameUI->GetHeight();
	}
}

//----------------------------------------------------

void CCmdWindow::Enable()
{
	if(m_bEnabled) return;

	if(m_pEditControl) {
		RECT rect;
		GetClientRect(pGame->GetMainWindowHwnd(), &rect);

		m_pEditControl->SetEnabled(true);
		m_pEditControl->SetVisible(true);

		// TODO: CCmdWindow::Enable()
	}

	m_bEnabled = TRUE;
}

//----------------------------------------------------

void CCmdWindow::Disable()
{
	if(!m_bEnabled) return;

	if(m_pEditControl) {
		m_pEditControl->OnFocusOut();
		m_pEditControl->SetEnabled(false);
		m_pEditControl->SetVisible(false);
	}
	pGame->ToggleKeyInputsDisabled(0, TRUE);
	m_bEnabled = FALSE;
}

//----------------------------------------------------





//----------------------------------------------------

void CCmdWindow::ProcessInput()
{
	if(!m_pEditControl) return;

	strncpy(m_szInputBuffer,m_pEditControl->GetTextA(),MAX_CMD_INPUT);
	m_szInputBuffer[MAX_CMD_INPUT] = '\0';

	// TODO: CCmdWindow::ProcessInput()
}

//----------------------------------------------------

CMDPROC CCmdWindow::GetCmdHandler(PCHAR szCmdName)
{
	int x=0;
	while(x!=m_iCmdCount) {
		if(!stricmp(szCmdName,m_szCmdNames[x])) {
			return m_pCmds[x];
		}
		x++;
	}
	return NULL;
}

//----------------------------------------------------

void CCmdWindow::AddDefaultCmdProc(CMDPROC cmdDefault)
{
	m_pDefaultCmd = cmdDefault;
}

//----------------------------------------------------

void CCmdWindow::AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler)
{
	if(m_iCmdCount < MAX_CMDS && (strlen(szCmdName) < MAX_CMD_STRLEN)) {
		m_pCmds[m_iCmdCount] = cmdHandler;
		strcpy(m_szCmdNames[m_iCmdCount],szCmdName);
		m_iCmdCount++;
	}
}

//----------------------------------------------------










