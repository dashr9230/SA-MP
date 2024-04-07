
#include "playertags.h"
#include <stdio.h>

HealthBarVertices_s HealthBarBDRVertices[4] =
{	//  x     y      z     c
	{-0.3f,  -0.03f, -0.002f, D3DCOLOR_XRGB(0, 0, 0)},  // 1 . . 4
	{-0.3f,   0.03f, -0.002f, D3DCOLOR_XRGB(0, 0, 0)},  // 2 . . 3
	{ 0.3f,   0.03f, -0.002f, D3DCOLOR_XRGB(0, 0, 0)},
	{ 0.3f,  -0.03f, -0.002f, D3DCOLOR_XRGB(0, 0, 0)}
};

HealthBarVertices_s HealthBarBGVertices[4] =
{	//  x     y      z     c
	{-0.29f, -0.02f, -0.001f, D3DCOLOR_XRGB(125, 21, 30)},  // 1 . . 4
	{-0.29f,  0.02f, -0.001f, D3DCOLOR_XRGB(125, 21, 30)},  // 2 . . 3
	{ 0.29f,  0.02f, -0.001f, D3DCOLOR_XRGB(125, 21, 30)},
	{ 0.29f, -0.02f, -0.001f, D3DCOLOR_XRGB(125, 21, 30)}
};

HealthBarVertices_s HealthBarVertices[4] =
{	//  x     y      z     c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(195, 34, 40)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(195, 34, 40)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(195, 34, 40)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(195, 34, 40)}
};

extern BYTE m_Metrics[256];

CPlayerTags::CPlayerTags(IDirect3DDevice9* pDevice)
{
	m_pDevice			= pDevice;
	m_pTexture			= NULL;
	m_pVB				= NULL;
	m_pOldStates		= NULL;
	m_pNewStates		= NULL;

	m_fCharWidth		= 0.0f;
	m_fCharHeight		= 0.0f;
	m_fScale			= 0.0075f;
}

CPlayerTags::~CPlayerTags()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pVB) m_pVB->Release();
	if (m_pOldStates) m_pOldStates->Release();
	if (m_pNewStates) m_pNewStates->Release();
}

void CPlayerTags::Begin()
{
	if ((!m_pOldStates) || (!m_pNewStates))
	{
		RestoreDeviceObjects();
		if ((!m_pOldStates) || (!m_pNewStates))
			return;
	}
	m_pOldStates->Capture();
	m_pNewStates->Apply();
}

void CPlayerTags::End()
{
	if (!m_pOldStates)
	{
		RestoreDeviceObjects();
		if (!m_pOldStates)
			return;
	}
	m_pOldStates->Apply();
}

void CPlayerTags::Draw(D3DXMATRIX* pWorldTransform, char* pNameText, DWORD dwColor, float fHealth)
{
	if ((!m_pVB) || (!m_pTexture))
	{
		RestoreDeviceObjects();
		if ((!m_pVB) || (!m_pTexture))
			return;
	}

	// Set the world transform.
	m_pDevice->SetTransform(D3DTS_WORLD, pWorldTransform);


	// --- begin: NameTag drawing... ---
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(NameTextVertex_s));
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->SetFVF(NameTextFVF);

	int len = (int)strlen(pNameText);
	if (len > MAX_NAME_LENGTH)
		len = MAX_NAME_LENGTH;

	NameTextVertex_s *vert_ptr = 0;
	m_pVB->Lock(0, 0, (void**)&vert_ptr, D3DLOCK_DISCARD);

	float fX = 0.0f;
	for (int i=0; i<len; i++)
	{
		fX += (float)m_Metrics[pNameText[i]] * m_fScale;
	}
	fX = -(fX / 2);

	for (int i=0; i<len; i++)
	{
		InitQuad(pNameText[i], fX, 0.05f, vert_ptr);
		fX += (float)m_Metrics[pNameText[i]] * m_fScale;
		vert_ptr += 6;
	}

	m_pVB->Unlock();

	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, len*2);
	// --- end: NameTag drawing ---


	// --- begin: HealthBar drawing... ---
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetFVF(HealthBarFVF);

	if (fHealth > 100.0f)
		fHealth = 100.0f;
	if (fHealth < 0.0f)
		fHealth = 0.0f;

	HealthBarBGVertices[0].x = HealthBarBGVertices[1].x = (0.0058f * fHealth) - 0.29f;

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBDRVertices, sizeof(HealthBarVertices_s));
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBGVertices,  sizeof(HealthBarVertices_s));
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarVertices,    sizeof(HealthBarVertices_s));
	// --- end: HealthBar drawing ---
}

inline void CPlayerTags::InitQuad(char c, float x, float y2, NameTextVertex_s* pVB)
{
	float x2  = x + ((float)m_Metrics[c] * m_fScale);
	float y  = y2 + (m_fCharHeight * m_fScale);
	float tu  = (c % 16) / 16.0f;
	float tu2 = tu + ((1 / 16.0f) * (m_Metrics[c] / m_fCharWidth));
	float tv  = (c - (c % 16)) / 256.0f;
	float tv2 = tv + 1 / 16.0f;

	pVB[0].Initialise(x,  y2, tu,  tv2);
	pVB[1].Initialise(x,  y,  tu,  tv);
	pVB[2].Initialise(x2, y,  tu2, tv);
	pVB[3] = pVB[0];
	pVB[4] = pVB[2];
	pVB[5].Initialise(x2, y2, tu2, tv2);
}

void CPlayerTags::DeleteDeviceObjects()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	/*
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	*/

	if (m_pOldStates)
	{
		m_pOldStates->Release();
		m_pOldStates = NULL;
	}

	if (m_pNewStates)
	{
		m_pNewStates->Release();
		m_pNewStates = NULL;
	}
}

void CPlayerTags::RestoreDeviceObjects()
{
	if (!m_pTexture)
	{
		if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice, "bland.png", 
			D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
			D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture)))
		{
			m_pTexture = NULL;
		} else {
			D3DSURFACE_DESC desc;
			m_pTexture->GetLevelDesc(0, &desc);
			m_fCharWidth = desc.Width / 16.0f;
			m_fCharHeight = desc.Height / 16.0f;
		}
	}

	if (!m_pVB)
	{
		if (FAILED(m_pDevice->CreateVertexBuffer(((MAX_NAME_LENGTH*6)+1)*sizeof(NameTextVertex_s), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, NameTextFVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			m_pVB = NULL;
		}
	}

	if (!m_pOldStates)
	{
		D3DXMATRIX matTemp;
		D3DXMatrixIdentity(&matTemp);

		m_pDevice->BeginStateBlock();
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
		m_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pDevice->SetTransform(D3DTS_WORLD, &matTemp);
		m_pDevice->SetStreamSource(0, NULL, 0, 0);
		m_pDevice->SetTexture(0, NULL);
		m_pDevice->SetFVF(0);
		m_pDevice->EndStateBlock(&m_pOldStates);
	}

	if (!m_pNewStates)
	{
		m_pDevice->BeginStateBlock();
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
		m_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pDevice->EndStateBlock(&m_pNewStates);
	}
}

BYTE m_Metrics[256] =
{
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x58, 0x00, 0x15, 0x15, 0x00, 0x15, 0x15, 
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 
    0x07, 0x08, 0x0A, 0x11, 0x0D, 0x17, 0x0F, 0x06, 0x0A, 0x0A, 0x0D, 0x11, 0x08, 0x0A, 0x08, 0x0A, 
    0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0A, 0x0A, 0x11, 0x11, 0x11, 0x0B, 
    0x15, 0x0E, 0x0E, 0x0F, 0x10, 0x0D, 0x0C, 0x10, 0x10, 0x08, 0x0A, 0x0F, 0x0C, 0x12, 0x10, 0x11, 
    0x0D, 0x11, 0x0F, 0x0E, 0x0D, 0x0F, 0x0E, 0x15, 0x0E, 0x0D, 0x0E, 0x0A, 0x0A, 0x0A, 0x11, 0x0D, 
    0x0D, 0x0D, 0x0D, 0x0B, 0x0D, 0x0D, 0x07, 0x0D, 0x0D, 0x06, 0x07, 0x0C, 0x06, 0x14, 0x0D, 0x0D, 
    0x0D, 0x0D, 0x09, 0x0B, 0x08, 0x0D, 0x0C, 0x11, 0x0C, 0x0C, 0x0B, 0x0D, 0x0A, 0x0D, 0x11, 0x15, 
    0x0D, 0x15, 0x06, 0x0D, 0x0A, 0x11, 0x0D, 0x0D, 0x0D, 0x20, 0x0E, 0x0A, 0x16, 0x15, 0x0E, 0x15, 
    0x15, 0x06, 0x06, 0x0A, 0x0A, 0x0B, 0x0D, 0x15, 0x0D, 0x15, 0x0B, 0x0A, 0x15, 0x15, 0x0B, 0x0D, 
    0x07, 0x08, 0x0D, 0x0D, 0x0D, 0x0D, 0x0A, 0x0D, 0x0D, 0x15, 0x0B, 0x0E, 0x11, 0x0A, 0x15, 0x0D, 
    0x0B, 0x11, 0x0B, 0x0B, 0x0D, 0x0D, 0x0D, 0x08, 0x0D, 0x0B, 0x0B, 0x0E, 0x15, 0x15, 0x15, 0x0B, 
    0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x14, 0x0F, 0x0D, 0x0D, 0x0D, 0x0D, 0x08, 0x08, 0x08, 0x08, 
    0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0F, 0x0F, 0x0F, 0x0F, 0x0D, 0x0D, 0x0D, 
    0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x14, 0x0B, 0x0D, 0x0D, 0x0D, 0x0D, 0x06, 0x06, 0x06, 0x06, 
    0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x11, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0C, 0x0D, 0x0C,
};
