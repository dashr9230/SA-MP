
#pragma once

class CFontRender // size: 40
{
private:
	char _gap0[40];

public:
	CFontRender(IDirect3DDevice9* pD3DDevice);

	void DeleteDeviceObjects();
	void RestoreDeviceObjects();

	SIZE MeasureText(char * szString, DWORD dwFormat = 0);

	void RenderText(ID3DXSprite * pSprite, char * sz, RECT rect, DWORD dwColor, BOOL bShadowed = TRUE);
};
