
#pragma once

#define MAX_MESSAGES				100


#pragma pack(1)
typedef struct _CHAT_WINDOW_ENTRY
{
	time_t	tTime;
	char	szName[28];
	char	szText[208];
	int		iType;
	DWORD	dwTextColor;
	DWORD	dwNameColor;
} CHAT_WINDOW_ENTRY;

class CChatWindow // size: 25578
{
public:

	int field_0;
	char _gap4[4];
	int field_8;
	char m_bTimestamp;
	int field_D;
	char field_11[261];
	CDXUTDialog			*m_pGameUI;
	int field_11A;
	CDXUTScrollBar		*m_pScrollBar;
	DWORD				m_dwChatTextColor;
	DWORD				m_dwChatInfoColor;
	DWORD				m_dwChatDebugColor;
	int field_12E;
	CHAT_WINDOW_ENTRY	m_ChatWindowEntries[MAX_MESSAGES];
	CFontRender			*m_pFontRender;
	ID3DXSprite			*field_63A6;
	ID3DXSprite			*field_63AA;
	IDirect3DDevice9	*m_pD3DDevice;
	int field_63B2;
	ID3DXRenderToSurface *field_63B6;
	IDirect3DTexture9* field_63BA;
	IDirect3DSurface9 *field_63BE;
	D3DDISPLAYMODE field_63C2;
	DWORD field_63D2;
	int field_63D6;
	int m_bRedraw;
	int field_63DE;
	int field_63E2;
	int field_63E6;


	void CreateFonts(); // .text:100681D0

	void FUNC_10067120(); // .text:10067120

	void FUNC_10067200();

	void FUNC_10067390(); // .text:10067390

	void OnMouseWheel(int nDelta); // .text:100673C0

	void PageUp(); // .text:100672D0

	void PageDown(); // .text:10067330

	void ReplaceControlChars(char *szString); // .text:10067420

	void ReleaseTextures(); // .text:100671A0

	void FUNC_10067410(); // .text:10067410

	void FUNC_10067470(char *szText, RECT rect, DWORD dwColor); // .text:10067470

	void FUNC_10067940(); // .text:10067940

	void FUNC_10067E00(); // .text:10067E00

	void FUNC_10067ED0(); // .text:10067ED0

public:

	void WriteChatLog(int iType, char *szText, char *szName); // .text:100677D0

	int sub_10067BE0(int a1, char *a2, char *a3, DWORD a4, DWORD a5); // .text:10067BE0

	void AddChatMessage(PCHAR szName, DWORD dwColor, PCHAR szText); // .text:10068020

	int AddDebugMessage(CHAR *szFormat, ...);

	void AddInfoMessage(CHAR *szFormat, ...); // .text:10068070

	void AddClientMessage(DWORD dwColor, CHAR *szMessage); // .text:10068170

	void SetPageSize(int iPageSize); // .text:100672A0

	void ResetDialogControls(CDXUTDialog *pGameUI);

	CChatWindow(IDirect3DDevice9 *pD3DDevice, CFontRender *pFontRender, CHAR *szChatLogFile);
};
