
#include "main.h"

CUnkClass9::CUnkClass9(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	D3DXCreateSprite(pD3DDevice, &m_pD3DSprite);
}
