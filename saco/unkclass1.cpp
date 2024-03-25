
#include "main.h"

CUnkClass1::CUnkClass1(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_4 = 0;
	m_pD3DSprite = NULL;

	D3DXCreateSprite(pD3DDevice, &m_pD3DSprite);
}
