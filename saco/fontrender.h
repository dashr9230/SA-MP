
#pragma once

class CFontRender // size: 40
{
private:
	ID3DXFontHook		*field_0;
	ID3DXFontHook		*field_4;
	ID3DXFont			*field_8;
	ID3DXFont			*field_C;
	ID3DXFont			*field_10;
	ID3DXSprite*		field_14;
	IDirect3DDevice9	*m_pD3DDevice;
	CHAR				*field_1C;
	LONG				field_20;
	LONG				field_24;

public:

	CFontRender(IDirect3DDevice9* pD3DDevice);
	~CFontRender();

	void CreateFonts();

	void DeleteDeviceObjects();
	void RestoreDeviceObjects();

	SIZE MeasureText(char * szString, DWORD dwFormat = 0);

	void RenderText(ID3DXSprite * pSprite, char * sz, RECT rect, DWORD dwColor, BOOL bShadowed = TRUE);
};
