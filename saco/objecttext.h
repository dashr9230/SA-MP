
#pragma once

class CObjectText
{
private:
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXSprite *m_pD3DSprite1;
	ID3DXSprite *m_pD3DSprite2;

public:
	CObjectText(IDirect3DDevice9 *pD3DDevice);
	~CObjectText();

	void DeleteDeviceObjects();
	void RestoreDeviceObjects();
};
