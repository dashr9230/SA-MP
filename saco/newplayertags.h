
#pragma once

class CNewPlayerTags
{
private:
	IDirect3DDevice9* m_pDevice;
	IDirect3DStateBlock9* m_pStates;
	ID3DXSprite* m_pSprite;

public:
	CNewPlayerTags(IDirect3DDevice9* pDevice);
	~CNewPlayerTags();

	void RestoreDeviceObjects();
};
