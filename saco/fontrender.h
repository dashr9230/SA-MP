
#pragma once

class CFontRender
{
public:
	//char _gap0[40];
	ID3DXFontHook		*field_0;
	ID3DXFontHook		*field_4;
	ID3DXFont			*field_8;
	ID3DXFont			*field_C;
	ID3DXFont			*field_10;
	ID3DXSprite			*field_14;
	IDirect3DDevice9	*m_pD3DDevice;
	char				*field_1C;
	LONG				field_20;
	LONG				field_24;

	CFontRender(IDirect3DDevice9* pD3DDevice);
	~CFontRender();

	void CreateFonts();

	void DeleteDeviceObjects();
	void RestoreDeviceObjects();

	SIZE MeasureText(char * szString, DWORD dwFormat = DT_LEFT);
	SIZE MeasureText2(char * szString, DWORD dwFormat = DT_LEFT);

	void RenderText(ID3DXSprite *pSprite, char * sz, RECT rect, DWORD dwColor, BOOL bShadowed=TRUE);
	void RenderText(ID3DXSprite *pSprite, char * sz, RECT rect, DWORD dwFormat, DWORD dwColor, BOOL bShadowed=TRUE);
	void RenderText(char *sz, RECT rect, DWORD dwColor);
};
