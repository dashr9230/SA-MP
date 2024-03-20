
#pragma once

//----------------------------------------------------

class CSpawnScreen
{
private:
	BOOL				m_bEnabled;
	CHAR				*m_szSpawnText;

	CFontRender			*m_pFont;
	IDirect3DDevice9	*m_pD3DDevice;
	IDirect3DTexture9	*m_pTexture;

	IDirect3DStateBlock9 *m_pStateBlockSaved;
	IDirect3DStateBlock9 *m_pStateBlockDraw;

	ID3DXSprite			*m_mySprite;

public:
	void Draw();

	void RestoreDeviceObjects();
	void DeleteDeviceObjects();

	void SetSpawnText(CHAR* szSpawnText);

	CSpawnScreen(IDirect3DDevice9 *pD3DDevice);
	~CSpawnScreen();
};

//----------------------------------------------------
// EOF
