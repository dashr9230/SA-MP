
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

void CNewPlayerTags::Begin()
{
	if (!m_pStates) {
		RestoreDeviceObjects();
	}

	if(m_pStates)
		m_pStates->Capture();

	if(m_pSprite)
		m_pSprite->Begin(D3DXSPRITE_DONOTSAVESTATE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, 0);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_pDevice->SetVertexShader(NULL);
	m_pDevice->SetPixelShader(NULL);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, 0);
}

void CNewPlayerTags::End()
{
	if(m_pSprite)
		m_pSprite->End();

	if(m_pStates)
		m_pStates->Apply();
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

