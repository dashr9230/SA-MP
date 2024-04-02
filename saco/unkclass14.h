
#pragma once

class CUnkClass14
{
private:
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXSprite *m_pD3DSprite1;
	ID3DXSprite *m_pD3DSprite2;

public:
	CUnkClass14(IDirect3DDevice9 *pD3DDevice);

	void RestoreDeviceObjects();
};
