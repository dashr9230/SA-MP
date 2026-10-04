
#include <time.h>
#include "main.h"
#include "game/util.h"

extern CConfig *pConfig;
extern CCmdWindow *pCmdWindow;
extern CGame *pGame;

typedef char AssertEntrySize[sizeof(CHAT_WINDOW_ENTRY) == 252 ? 1 : -1];
typedef char AssertEntryText[offsetof(CHAT_WINDOW_ENTRY, szText) == 0x20 ? 1 : -1];
typedef char AssertEntryType[offsetof(CHAT_WINDOW_ENTRY, iType) == 0xF0 ? 1 : -1];
typedef char AssertChatWindowSize[sizeof(CChatWindow) == 25578 ? 1 : -1];
typedef char AssertTimestampWidth[offsetof(CChatWindow, field_63E6) == 0x63E6 ? 1 : -1];

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
	m_bTimestamp = 0;

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
	D3DSURFACE_DESC desc;
	HRESULT hr;

	field_63B2 = 1;

	if(field_63BE) {
		field_63BE->Release();
		field_63BE = NULL;
	}

	if(field_63BA) {
		field_63BA->Release();
		field_63BA = NULL;
	}

	if(field_63B6) {
		field_63B6->Release();
		field_63B6 = NULL;
	}

	m_pD3DDevice->GetDisplayMode(0,&field_63C2);

	if(pConfig->GetIntVariable("directmode")) {
		AddDebugMessage("ChatWindow: Using direct drawing mode.");
		FUNC_10067120();
		field_63B2 = 0;
		return;
	}

	// the render target has to cover the whole screen
	if(field_63C2.Width <= 1024) {
		hr = D3DXCreateTexture(m_pD3DDevice,1024,512,1,D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&field_63BA);
	} else {
		hr = D3DXCreateTexture(m_pD3DDevice,2048,1024,1,D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&field_63BA);
	}

	if(hr < 0) {
		AddDebugMessage("ChatWindow: Can't create a render surface texture. Will use direct mode.");
		field_63B2 = 0;
		return;
	}

	field_63BA->GetSurfaceLevel(0,&field_63BE);
	field_63BE->GetDesc(&desc);

	if(D3DXCreateRenderToSurface(m_pD3DDevice,desc.Width,desc.Height,desc.Format,
		TRUE,D3DFMT_D16,&field_63B6) < 0)
	{
		AddDebugMessage("ChatWindow: Can't create a render to surface. Will use direct mode.");
		FUNC_10067120();
		field_63B2 = 0;
		return;
	}

	FUNC_10067120();

	field_63D6 = 0;
	m_bRedraw = 1;
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

int CChatWindow::AddDebugMessage(CHAR * szFormat, ...)
{
	char szBuffer[512];
	va_list va;
	va_start(va, szFormat);
	memset(szBuffer, 0, sizeof(szBuffer));
	vsprintf(szBuffer, szFormat, va);

	char *p = szBuffer;
	if(szBuffer[0])
	{
		do
		{
			if(*p > 0 && *p < 32)
				*p = 32;
		}
		while(*++p);
	}

	return sub_10067BE0(8, szBuffer, 0, m_dwChatDebugColor, 0);
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


//----------------------------------------------------

void CChatWindow::SetPageSize(int iPageSize)
{
	if(iPageSize >= 10 && iPageSize <= MAX_MESSAGES) {
		field_0 = iPageSize;
		FUNC_10067200();
		m_bRedraw = 1;
	}
}

//----------------------------------------------------

void CChatWindow::ReplaceControlChars(char *szString)
{
	if(*szString)
	{
		do
		{
			if(*szString > 0 && *szString < 32)
				*szString = 32;
		}
		while(*++szString);
	}
}

//----------------------------------------------------

void CChatWindow::PageUp()
{
	if(field_8 && m_pScrollBar && !pGame->sub_100A0920() && !pCmdWindow->IsImeActive()) {
		int iPos = m_pScrollBar->GetTrackPos()-field_0;
		if(iPos < 1) iPos = 1;
		m_pScrollBar->SetTrackPos(iPos);
	}
}

//----------------------------------------------------

void CChatWindow::PageDown()
{
	if(field_8 && m_pScrollBar && !pGame->sub_100A0920() && !pCmdWindow->IsImeActive()) {
		int iPos = m_pScrollBar->GetTrackPos();
		if(iPos == 1)
			iPos = field_0;
		else
			iPos += field_0;
		if(iPos > MAX_MESSAGES) iPos = MAX_MESSAGES;
		m_pScrollBar->SetTrackPos(iPos);
	}
}

//----------------------------------------------------

void CChatWindow::FUNC_10067390()
{
	if(m_pScrollBar)
		m_pScrollBar->SetTrackPos(MAX_MESSAGES-10);
}

//----------------------------------------------------

void CChatWindow::OnMouseWheel(int nDelta)
{
	if(field_8 && m_pScrollBar) {
		int nScrollLines;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0,&nScrollLines,0);
		m_pScrollBar->Scroll(-(nDelta*nScrollLines));
	}
}

//----------------------------------------------------

void CChatWindow::AddChatMessage(PCHAR szName, DWORD dwColor, PCHAR szText)
{
	char *p = szText;
	if(szText[0])
	{
		do
		{
			if(*p > 0 && *p < 32)
				*p = 32;
		}
		while(*++p);
	}

	sub_10067BE0(2, szText, szName, m_dwChatTextColor, dwColor);
}

//----------------------------------------------------

void CChatWindow::AddClientMessage(DWORD dwColor, CHAR *szMessage)
{
	if(strlen(szMessage) <= 144)
	{
		unnamed_100B6100(szMessage,255);
		ReplaceControlChars(szMessage);
		sub_10067BE0(4, szMessage, 0, (dwColor >> 8)|0xFF000000, 0);
	}
}

//----------------------------------------------------

void CChatWindow::AddInfoMessage(CHAR * szFormat, ...)
{
	char szBuffer[512];
	va_list va;
	va_start(va, szFormat);
	memset(szBuffer, 0, sizeof(szBuffer));
	vsprintf(szBuffer, szFormat, va);

	char *p = szBuffer;
	if(szBuffer[0])
	{
		do
		{
			if(*p > 0 && *p < 32)
				*p = 32;
		}
		while(*++p);
	}

	sub_10067BE0(4, szBuffer, 0, m_dwChatInfoColor, 0);
}

//----------------------------------------------------

void CChatWindow::ReleaseTextures()
{
	if(field_63BE) {
		field_63BE->Release();
		field_63BE = NULL;
	}

	if(field_63BA) {
		field_63BA->Release();
		field_63BA = NULL;
	}

	if(field_63B6) {
		field_63B6->Release();
		field_63B6 = NULL;
	}
}

//----------------------------------------------------

void CChatWindow::FUNC_10067410()
{
	ReleaseTextures();
}

//----------------------------------------------------
// measures the line height and the width of a timestamp with DT_CALCRECT

void CChatWindow::FUNC_10067120()
{
	RECT rect;

	m_pFontRender->field_0->DrawTextA(NULL,"Y",-1,&rect,DT_CALCRECT|DT_SINGLELINE,0xFF000000);
	field_63E2 = rect.bottom-rect.top;

	m_pFontRender->field_0->DrawTextA(NULL,"[19:58:34]",-1,&rect,DT_CALCRECT|DT_SINGLELINE,0xFF000000);
	field_63E6 = rect.right-rect.left;
}

//----------------------------------------------------

void CChatWindow::FUNC_10067E00()
{
	if(m_pScrollBar)
	{
		if(field_8 && (m_pScrollBar->GetTrackPos() != MAX_MESSAGES-field_0 || pCmdWindow->m_bEnabled))
			m_pScrollBar->SetVisible(true);
		else
			m_pScrollBar->SetVisible(false);

		if(field_8)
		{
			if(field_63B2 && field_63D6)
			{
				field_63AA->Begin(D3DXSPRITE_ALPHABLEND);
				field_63AA->Draw(field_63BA,NULL,NULL,NULL,0xFFFFFFFF);
				field_63AA->End();
			}
			else
			{
				field_63A6->Begin(D3DXSPRITE_ALPHABLEND);
				FUNC_10067940();
				field_63A6->End();
			}
		}
	}
}

//----------------------------------------------------

void CChatWindow::WriteChatLog(int iType, char *szText, char *szName)
{
	time_t curTime = time(NULL);

	if(field_D)
	{
		FILE *f = fopen(field_11,"a");
		if(f)
		{
			// the frame reserves 68 bytes for this, strftime is still capped at 64
			char szTimestamp[68];
			strftime(szTimestamp,64,"[%H:%M:%S]",localtime(&curTime));

			switch(iType)
			{
			case 2:
				fprintf(f,"%s <%s> %s\r\n",szTimestamp,szName,szText);
				break;
			case 4:
			case 8:
				fprintf(f,"%s %s\r\n",szTimestamp,szText);
				break;
			}

			fclose(f);
		}
	}
}

//----------------------------------------------------
// redraws the chat into the render target texture when it is dirty

void CChatWindow::FUNC_10067ED0()
{
	if(field_63B2 && m_pD3DDevice && field_63B6 && field_63BA && field_63BE && m_pFontRender->field_0)
	{
		if(pGame->sub_100A0920())
		{
			field_63D6 = 0;
			m_bRedraw = 1;
		}
		else if(field_8)
		{
			if(m_bRedraw || field_63DE != m_pScrollBar->GetTrackPos())
			{
				field_63B6->BeginScene(field_63BE,NULL);
				m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0,0,0);
				field_63A6->Begin(D3DXSPRITE_ALPHABLEND);
				m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_BLENDFACTOR);
				FUNC_10067940();
				field_63A6->End();
				field_63B6->EndScene(0);

				m_bRedraw = 0;
				field_63D6 = 1;
				field_63D2 = GetTickCount();
				field_63DE = m_pScrollBar->GetTrackPos();
			}
		}
	}
}

//----------------------------------------------------
// lays out the visible page of chat lines

void CChatWindow::FUNC_10067940()
{
	int x = 0;
	int iEntry;
	RECT rect, rectName;
	char szTimestamp[72];
	struct tm *pTime;
	int iPos;

	memset(szTimestamp,0,64);

	rect.top = 10;
	rect.left = 45;
	rect.right = 550;
	rect.bottom = 110;

	if(m_pScrollBar->GetTrackPos() < 1)
		m_pScrollBar->SetTrackPos(1);

	iPos = m_pScrollBar->GetTrackPos();
	if(iPos < 1)
		iPos = 1;

	iEntry = iPos;

	while(x != field_0)
	{
		if(m_bTimestamp)
		{
			pTime = localtime(&m_ChatWindowEntries[iEntry].tTime);
			memset(szTimestamp,0,64);

			if(pTime)
				strftime(szTimestamp,64,"[%H:%M:%S]",pTime);
		}

		switch(m_ChatWindowEntries[iEntry].iType)
		{
		case 2:
			if(m_bTimestamp)
			{
				if(strlen(szTimestamp) && m_ChatWindowEntries[iEntry].szName[0])
				{
					FUNC_10067470(szTimestamp,rect,m_ChatWindowEntries[iEntry].dwTextColor);
					rect.left = field_63E6+50;
				}
			}

			if(strlen(m_ChatWindowEntries[iEntry].szName))
			{
				m_pFontRender->field_0->DrawTextA(NULL,m_ChatWindowEntries[iEntry].szName,-1,&rectName,DT_CALCRECT,0xFF000000);
				FUNC_10067470(m_ChatWindowEntries[iEntry].szName,rect,m_ChatWindowEntries[iEntry].dwNameColor);
				rect.left += rectName.right-rectName.left+5;
			}

			FUNC_10067470(m_ChatWindowEntries[iEntry].szText,rect,m_ChatWindowEntries[iEntry].dwTextColor);
			break;

		case 4:
		case 8:
			if(m_bTimestamp)
			{
				if(strlen(szTimestamp))
				{
					FUNC_10067470(szTimestamp,rect,m_ChatWindowEntries[iEntry].dwTextColor);
					rect.left = field_63E6+50;
				}
			}

			FUNC_10067470(m_ChatWindowEntries[iEntry].szText,rect,m_ChatWindowEntries[iEntry].dwTextColor);
			break;
		}

		iEntry++;

		rect.top += field_63E2+1;
		rect.bottom = rect.top+field_63E2+1;
		rect.left = 45;
		x++;
	}

	field_12E = rect.bottom;
}

//----------------------------------------------------
// draws the text with a black outline, thicker over 1280 wide

void CChatWindow::FUNC_10067470(char *szText, RECT rect, DWORD dwColor)
{
	ID3DXFontHook *pFont;
	ID3DXFont *pShadow;
	int iLen;

	pShadow = m_pFontRender->field_8;
	pFont = m_pFontRender->field_0;
	iLen = strlen(szText);

	if(field_8 == 2)
	{
		char szBuffer[512] = {0};
		int iShadowLen;

		strncpy(szBuffer,szText,511);
		RemoveColorEmbedsFromString(szBuffer);
		iShadowLen = strlen(szBuffer);

		if(*(int *)0xC17044 > 1280)
		{
			rect.top -= 2;
			rect.bottom -= 2;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.top++;
			rect.bottom++;
			rect.left--;
			rect.right--;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left++;
			rect.right++;
			rect.left++;
			rect.right++;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left--;
			rect.right--;
			rect.top++;
			rect.bottom++;
			rect.top += 2;
			rect.bottom += 2;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.top--;
			rect.bottom--;
			rect.left--;
			rect.right--;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left++;
			rect.right++;
			rect.left++;
			rect.right++;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left--;
			rect.right--;
			rect.top--;
			rect.bottom--;
			rect.left -= 2;
			rect.right -= 2;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left += 2;
			rect.right += 2;
			rect.left += 2;
			rect.right += 2;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left -= 2;
			rect.right -= 2;
		}
		else
		{
			rect.top--;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.top += 2;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.top--;
			rect.left--;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left += 2;

			pShadow->DrawTextA(field_63A6,szBuffer,iShadowLen,&rect,DT_NOCLIP|DT_SINGLELINE,0xFF000000);

			rect.left--;
		}
	}

	pFont->DrawTextA(field_63A6,szText,iLen,&rect,DT_NOCLIP|DT_SINGLELINE,dwColor|0xFF000000);
}
