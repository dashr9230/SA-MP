
#include "main.h"

HealthBarVertices1_s HealthBarBDRVertices1[4] =
{	//x      y      z     rhw   c
	{0.0f,   0.0f,  0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 0)},
	{0.0f,   25.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 0)},
	{100.0f, 25.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 0)},
	{100.0f, 0.0f,  0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 0)}
};

HealthBarVertices1_s HealthBarBGVertices1[4] =
{	//x     y      z     rhw   c
	{2.0f,  2.0f,  0.0f, 1.0f, D3DCOLOR_XRGB(75, 11, 20)},
	{2.0f,  18.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(75, 11, 20)},
	{96.0f, 18.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(75, 11, 20)},
	{96.0f, 2.0f,  0.0f, 1.0f, D3DCOLOR_XRGB(75, 11, 20)}
};

HealthBarVertices1_s HealthBarVertices1[4] =
{	//x     y      z     rhw   c
	{2.0f,  2.0f,  0.0f, 1.0f, D3DCOLOR_XRGB(185, 34, 40)},
	{2.0f,  18.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(185, 34, 40)},
	{96.0f, 18.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(185, 34, 40)},
	{96.0f, 2.0f,  0.0f, 1.0f, D3DCOLOR_XRGB(185, 34, 40)}
};

extern D3DXMATRIX matView, matProj;

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

void CNewPlayerTags::Draw(D3DXVECTOR3* pPlayerPos, float fHealth, float fArmor, float fDistanceFromLocalPlayer)
{
	D3DXVECTOR3 TagPos = *pPlayerPos;
	TagPos.z += 0.2f + (fDistanceFromLocalPlayer * 0.0475f);

	D3DVIEWPORT9 Viewport;
	m_pDevice->GetViewport(&Viewport);

	D3DXVECTOR3 Out;
	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);
	D3DXVec3Project(&Out, &TagPos, &Viewport, &matProj, &matView, &matIdent);

	if (Out.z > 1.0f || fHealth < 0.0f)
		return;

	Out.x = (float)((int)Out.x);
	Out.y = (float)((int)Out.y);

	HealthBarVertices1[0].c = HealthBarVertices1[1].c =
	HealthBarVertices1[2].c = HealthBarVertices1[3].c = D3DCOLOR_XRGB(185, 34, 40);

	HealthBarBGVertices1[0].c = HealthBarBGVertices1[1].c =
	HealthBarBGVertices1[2].c = HealthBarBGVertices1[3].c = D3DCOLOR_XRGB(75, 11, 20);

	HealthBarBDRVertices1[0].x = Out.x - 20.0f; // Top left
	HealthBarBDRVertices1[0].y = Out.y + 18.0f;
	HealthBarBDRVertices1[1].x = Out.x - 20.0f; // Bottom left
	HealthBarBDRVertices1[1].y = Out.y + 24.0f;
	HealthBarBDRVertices1[2].x = Out.x + 21.0f; // Bottom right
	HealthBarBDRVertices1[2].y = Out.y + 24.0f;
	HealthBarBDRVertices1[3].x = Out.x + 21.0f; // Top Right
	HealthBarBDRVertices1[3].y = Out.y + 18.0f;

	HealthBarBGVertices1[0].x = HealthBarVertices1[0].x = Out.x - 19.0f; // Top left
	HealthBarBGVertices1[0].y = HealthBarVertices1[0].y = Out.y + 19.0f;
	HealthBarBGVertices1[1].x = HealthBarVertices1[1].x = Out.x - 19.0f; // Bottom left
	HealthBarBGVertices1[1].y = HealthBarVertices1[1].y = Out.y + 23.0f;
	HealthBarBGVertices1[2].x = Out.x + 19.0f; // Bottom right
	HealthBarBGVertices1[2].y = HealthBarVertices1[2].y = Out.y + 23.0f;
	HealthBarBGVertices1[3].x = Out.x + 19.0f; // Top Right
	HealthBarBGVertices1[3].y = HealthBarVertices1[3].y = Out.y + 19.0f;

	if (fHealth > 100.0f)
		fHealth = 100.0f;
	fHealth /= 2.6f;
	fHealth -= 19.0f;

	HealthBarVertices1[2].x = Out.x + fHealth; // Bottom right
	HealthBarVertices1[3].x = Out.x + fHealth; // Top Right

	if (fArmor > 0.0f)
	{
		for (int i=0; i<4; i++)
		{
			HealthBarBDRVertices1[i].y += 8.0f;
			HealthBarBGVertices1[i].y += 8.0f;
			HealthBarVertices1[i].y += 8.0f;
		}
	}

	m_pDevice->SetFVF(HealthBar1FVF);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBDRVertices1, sizeof(HealthBarVertices1_s));
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBGVertices1,  sizeof(HealthBarVertices1_s));
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarVertices1,    sizeof(HealthBarVertices1_s));

	// ARMOUR BAR
	if (fArmor > 0.0f)
	{
		HealthBarVertices1[0].c = HealthBarVertices1[1].c =
		HealthBarVertices1[2].c = HealthBarVertices1[3].c = D3DCOLOR_XRGB(200, 200, 200);

		HealthBarBGVertices1[0].c = HealthBarBGVertices1[1].c =
		HealthBarBGVertices1[2].c = HealthBarBGVertices1[3].c = D3DCOLOR_XRGB(40, 40, 40);

		for (int i=0; i<4; i++)
		{
			HealthBarBDRVertices1[i].y -= 8.0f;
			HealthBarBGVertices1[i].y -= 8.0f;
			HealthBarVertices1[i].y -= 8.0f;
		}

		if(fArmor > 100.0f)
			fArmor = 100.0f;
		fArmor /= 2.6f;
		fArmor -= 19.0f;

		HealthBarVertices1[2].x = Out.x + fArmor; // Bottom right
		HealthBarVertices1[3].x = Out.x + fArmor; // Top Right

		m_pDevice->SetFVF(HealthBar1FVF);

		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBDRVertices1, sizeof(HealthBarVertices1_s));
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBGVertices1,  sizeof(HealthBarVertices1_s));
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarVertices1,    sizeof(HealthBarVertices1_s));
	}
}

