
#include "main.h"

CLabel::CLabel(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	D3DXCreateSprite(pD3DDevice, &m_pD3DSprite);
}
