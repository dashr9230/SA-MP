
#pragma once

#define MAX_MESSAGES				100


#pragma pack(1)
typedef struct _CHAT_WINDOW_ENTRY
{
	char _gap0[252];
} CHAT_WINDOW_ENTRY;

class CChatWindow // size: 25578
{
private:

	int field_0;
	char _gap4[4];
	int field_8;
	char field_C;
	int field_D;
	char field_11[261];
	CDXUTDialog			*m_pGameUI;
	int field_11A;
	CDXUTScrollBar		*m_pScrollBar;
	DWORD				m_dwChatTextColor;
	DWORD				m_dwChatInfoColor;
	DWORD				m_dwChatDebugColor;
	char _gap12E[4];
	CHAT_WINDOW_ENTRY	m_ChatWindowEntries[MAX_MESSAGES];
	CFontRender			*m_pFontRender;
	ID3DXSprite			*field_63A6;
	ID3DXSprite			*field_63AA;
	IDirect3DDevice9	*m_pD3DDevice;
	int field_63B2;
	ID3DXRenderToSurface *field_63B6;
	IDirect3DTexture9* field_63BA;
	int field_63BE;
	D3DDISPLAYMODE field_63C2;
	DWORD field_63D2;
	int field_63D6;
	int field_63DA;
	int field_63DE;
	int field_63E2;
	char _gap63E6[4];


	void CreateFonts();

	void FUNC_10067200();

public:

	void AddDebugMessage(CHAR *szFormat, ...);

	void ResetDialogControls(CDXUTDialog *pGameUI);

	CChatWindow(IDirect3DDevice9 *pD3DDevice, CFontRender *pFontRender, CHAR *szChatLogFile);
};
