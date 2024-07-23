
#include "main.h"

//----------------------------------------------------

CChatWindow::CChatWindow(IDirect3DDevice9 *pD3DDevice, CFontRender *pFontRender, CHAR *szChatLogFile)
{
	int x=0;

	m_pD3DDevice		= pD3DDevice;
	m_pFontRender		= pFontRender;
	field_8 = 2;

	// Create a sprite to use when drawing text
	D3DXCreateSprite(pD3DDevice,&field_63A6);
	D3DXCreateSprite(pD3DDevice,&field_63AA);

	// Init the chat window lines to 0
	while(x!=MAX_MESSAGES) {
		memset(&m_ChatWindowEntries[x],0,sizeof(CHAT_WINDOW_ENTRY));
		x++;
	}

	m_dwChatTextColor = D3DCOLOR_ARGB(255,255,255,255);
	m_dwChatInfoColor = D3DCOLOR_ARGB(255,136,170,98);
	m_dwChatDebugColor = D3DCOLOR_ARGB(255,169,196,228);

	field_0 = 10;
	field_C = 0;

	if(szChatLogFile && strlen(szChatLogFile))
	{
		memset(&field_11[0],0,sizeof(field_11));
		strncpy(field_11,szChatLogFile,MAX_PATH);

		FILE *f = fopen(field_11, "w");
		if(f)
		{
			field_D = 1;
			fclose(f);
		}
	}

	field_63BE = 0;
	field_63BA = NULL;
	field_63B6 = NULL;
	field_63D2 = GetTickCount();
	field_63DE = 1;
	m_pScrollBar = NULL;
	m_pGameUI = NULL;
	field_11A = 0;

	CreateFonts();
}

//----------------------------------------------------

void CChatWindow::CreateFonts()
{

	// TODO: CChatWindow::CreateFonts .text:100681D0
}

//----------------------------------------------------
// MATCH
void CChatWindow::ResetDialogControls(CDXUTDialog *pGameUI)
{
	m_pGameUI = pGameUI;

	if(pGameUI) {
		m_pScrollBar = new CDXUTScrollBar(pGameUI);
		pGameUI->AddControl(m_pScrollBar);
		m_pScrollBar->SetVisible(true);
		m_pScrollBar->SetEnabled(true);

		FUNC_10067200();
	}
}

//----------------------------------------------------
//----------------------------------------------------

void CChatWindow::AddDebugMessage(CHAR * szFormat, ...)
{
	// TODO: CChatWindow::AddDebugMessage .text:100680F0
}
// MATCH
void CChatWindow::FUNC_10067200()
{
	if(m_pScrollBar) {
		m_pScrollBar->SetLocation(10,40);
		m_pScrollBar->SetSize(20,((field_63E2+1)*field_0)-60);
		m_pScrollBar->SetTrackRange(1,MAX_MESSAGES);
		m_pScrollBar->SetPageSize(field_0);
		m_pScrollBar->SetTrackPos(MAX_MESSAGES-field_0);
	}
}

