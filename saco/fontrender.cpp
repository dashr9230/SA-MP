
#include "main.h"
#include "game/util.h"

int GetFontSize();
int GetFontWeight();
char *GetFontFace();

//----------------------------------------------------

CFontRender::CFontRender(IDirect3DDevice9* pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_0 = NULL;
	field_4 = NULL;
	field_8 = NULL;
	field_C = NULL;
	field_14 = NULL;
	field_10 = NULL;

	CreateFonts();
}

CFontRender::~CFontRender()
{
	SAFE_RELEASE(field_0);
	SAFE_RELEASE(field_4);
	SAFE_RELEASE(field_8);
	SAFE_RELEASE(field_C);
	SAFE_RELEASE(field_14);
	SAFE_RELEASE(field_10);
}

void CFontRender::CreateFonts()
{
	if(!m_pD3DDevice) return;

	SAFE_RELEASE(field_0);
	SAFE_RELEASE(field_4);
	SAFE_RELEASE(field_8);
	SAFE_RELEASE(field_C);
	SAFE_RELEASE(field_14);
	SAFE_RELEASE(field_10);

	int iFontSize = GetFontSize();
	int iFontSize2 = iFontSize - 2;
	int iFontWeight = GetFontWeight();
	char *szFontFace = GetFontFace();

	ID3DXFont* pFont;
	D3DXCreateFont(m_pD3DDevice, iFontSize, 0, iFontWeight, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, szFontFace, &pFont);

	ID3DXFontHook *pFontHook = new ID3DXFontHook();
	pFontHook->m_pD3DFont = pFont;
	field_0 = pFontHook;

	D3DXCreateFont(m_pD3DDevice, iFontSize, 0, iFontWeight, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, szFontFace, &field_8);

	D3DXCreateFont(m_pD3DDevice, iFontSize2, 0, iFontWeight, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, szFontFace, &pFont);

	ID3DXFontHook *pFontHook2 = new ID3DXFontHook();
	pFontHook2->m_pD3DFont = pFont;
	field_4 = pFontHook2;

	D3DXCreateFontA(m_pD3DDevice, iFontSize2, 0, iFontWeight, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, szFontFace, &field_C);

	D3DXCreateSprite(m_pD3DDevice, &field_14);

	field_1C = (char*)calloc(1u, 100001);

	D3DXCreateFontA(m_pD3DDevice, 38, 10, FW_BOLD, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &field_10);

	ID3DXFontHook *pFontHook3 = new ID3DXFontHook();
	pFontHook3->m_pD3DFont = field_10;
	field_10 = pFontHook3;

	field_20 = MeasureText("Y").cy;
	field_24 = MeasureText2("Y").cy;
}

void CFontRender::DeleteDeviceObjects()
{
	field_0->OnLostDevice();
	field_4->OnLostDevice();
	field_8->OnLostDevice();
	field_C->OnLostDevice();
	field_14->OnLostDevice();
	field_10->OnLostDevice();
}

void CFontRender::RestoreDeviceObjects()
{
	field_0->OnResetDevice();
	field_4->OnResetDevice();
	field_8->OnResetDevice();
	field_C->OnResetDevice();
	field_14->OnResetDevice();
	field_10->OnResetDevice();
}

SIZE CFontRender::MeasureText(char * szString, DWORD dwFormat)
{
	RECT rect;
	SIZE ret = {0, 0};

	if(strlen(szString) > 100000) return ret;

	strcpy(field_1C,szString);
	RemoveColorEmbedsFromString(field_1C);
	field_0->DrawTextA(NULL,field_1C,-1,&rect,dwFormat|DT_CALCRECT,0xFF000000);
	ret.cx = rect.right - rect.left;
	ret.cy = rect.bottom - rect.top;

	return ret;
}

SIZE CFontRender::MeasureText2(char * szString, DWORD dwFormat)
{
	RECT rect;
	SIZE ret = {0, 0};

	if(strlen(szString) > 100000) return ret;

	strcpy(field_1C,szString);
	RemoveColorEmbedsFromString(field_1C);
	field_4->DrawTextA(NULL,field_1C,-1,&rect,dwFormat|DT_CALCRECT,0xFF000000);
	ret.cx = rect.right - rect.left;
	ret.cy = rect.bottom - rect.top;

	return ret;
}

void CFontRender::RenderText(ID3DXSprite * pSprite, char * sz, RECT rect, DWORD dwColor, BOOL bShadowed)
{
	// TODO: CFontRender::RenderText .text:1006B360
}
