
#include "main.h"

CNewPlayerTags::CNewPlayerTags(IDirect3DDevice9* pDevice)
{
	m_pDevice			= pDevice;
	field_4				= 0;
	m_pSprite			= NULL;

	D3DXCreateSprite(pDevice, &m_pSprite);
}
