
#include "main.h"

extern CConfig *pConfig;

//----------------------------------------------------
// MATCH
CCmdWindow::CCmdWindow(IDirect3DDevice9 *pD3DDevice)
{
	field_14E0				= 0;
	m_pD3DDevice			= pD3DDevice;
	m_iCmdCount				= 0;
	field_1AF4				= 0;
	m_pEditControl			= NULL;
	field_1AF0				= -1;

	memset(&field_1565[0],0,1290);
	memset(&field_14E4[0],0,129);
	memset(&field_1A6F[0],0,129);
}

//----------------------------------------------------
// MATCH
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
// MATCH
void CCmdWindow::AddDefaultCmdProc(CMDPROC cmdDefault)
{
	m_pDefaultCmd = cmdDefault;	
}

//----------------------------------------------------
// MATCH
void CCmdWindow::AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler)
{
	if(m_iCmdCount < MAX_CMDS && (strlen(szCmdName) < MAX_CMD_STRLEN)) {
		m_pCmds[m_iCmdCount] = cmdHandler;
		strcpy(m_szCmdNames[m_iCmdCount],szCmdName);
		m_iCmdCount++;
	}
}

//----------------------------------------------------

