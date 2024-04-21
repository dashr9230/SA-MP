
#pragma once

class CLabel
{
private:
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXSprite *m_pD3DSprite;

public:
	CLabel(IDirect3DDevice9 *pD3DDevice);

};
