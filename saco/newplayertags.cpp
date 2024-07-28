
#include "main.h"

CNewPlayerTags::CNewPlayerTags(IDirect3DDevice9* pDevice)
{
	m_pDevice			= pDevice;
	m_pStates			= NULL;
	m_pSprite			= NULL;

	D3DXCreateSprite(pDevice, &m_pSprite);
}

CNewPlayerTags::~CNewPlayerTags()
{
	if(m_pStates) m_pStates->Release();
	SAFE_DELETE(m_pSprite);
}

void CNewPlayerTags::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pStates);

	if(m_pSprite)
		m_pSprite->OnLostDevice();
}

void CNewPlayerTags::RestoreDeviceObjects()
{
	if(m_pSprite)
		m_pSprite->OnResetDevice();

	if(!m_pStates)
		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStates);
}

