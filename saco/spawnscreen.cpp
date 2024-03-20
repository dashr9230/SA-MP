
#include "main.h"
#include "buildinfo.h"

//----------------------------------------------------

const char szVersionInfo[] = "SA-MP " SAMP_VERSION;

//----------------------------------------------------

extern CGame *pGame;
extern CFileSystem *pFileSystem;
extern CFontRender *pDefaultFont;

//----------------------------------------------------

typedef struct _D3DTLVERTEX
{
	float sx, sy, sz;
	float rhw;
	D3DCOLOR color;
	float tu, tv;

	_D3DTLVERTEX()
	{
	}

	_D3DTLVERTEX(const D3DVECTOR& v, float rhw, D3DCOLOR color, float tu, float tv)
	{
		this->sx = v.x;
		this->sy = v.y;
		this->sz = v.z;
		this->rhw = rhw;
		this->color = color;
		this->tu = tu;
		this->tv = tv;
	}

} D3DTLVERTEX;

//----------------------------------------------------

CSpawnScreen::CSpawnScreen(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_szSpawnText = NULL;
	m_pTexture = NULL;
	m_pStateBlockDraw = NULL;
	m_pStateBlockSaved = NULL;

	if (m_pD3DDevice)
	{
		// Just use the default font object
		//m_pFont = new CFontRender(m_pD3DDevice, "Arial", TRUE, TRUE, 0);
		m_pFont = pDefaultFont;
	}

	RestoreDeviceObjects();
}

//----------------------------------------------------

CSpawnScreen::~CSpawnScreen()
{
	if (m_szSpawnText)
		delete m_szSpawnText;

	if (m_pTexture || m_pStateBlockDraw || m_pStateBlockSaved)
		DeleteDeviceObjects();

	if (m_pFont != pDefaultFont)
		SAFE_DELETE(m_pFont);
}

//----------------------------------------------------

void CSpawnScreen::SetSpawnText(CHAR* szSpawnText)
{
	if (m_szSpawnText)
		delete m_szSpawnText;
	m_szSpawnText = NULL;
	if (szSpawnText)
	{
		m_szSpawnText = new CHAR[strlen(szSpawnText)+1];
		strcpy(m_szSpawnText, szSpawnText);
	}
}

//----------------------------------------------------

void CSpawnScreen::Draw()
{
	if (m_pFont && m_szSpawnText)
	{
		SIZE size = m_pFont->MeasureText(m_szSpawnText);

		RECT rect;
		rect.bottom	= pGame->GetScreenHeight() - 30;
		rect.top = rect.bottom - size.cy;
		rect.left = 20;
		rect.right = rect.left + size.cx;

		m_pFont->RenderText(0,m_szSpawnText,rect,0xFFFFFFFF);

		const int iTexWidth = 185;
		const int iTexHeight = 73;

		rect.top -= iTexHeight + 30;
		rect.bottom = rect.top + iTexHeight;
		rect.left = 20;
		rect.right = rect.left + iTexWidth;

		if (m_pTexture)
		{
			D3DTLVERTEX v[4];
			float l=rect.left-0.5f;
			float r=rect.right+0.5f;
			float t=rect.top-0.5f;
			float b=rect.bottom+0.5f;
			v[0]=D3DTLVERTEX(D3DXVECTOR3(l,b,0.1f),1.0f,0xFFFFFFFF,0.0f,1.0f);
			v[1]=D3DTLVERTEX(D3DXVECTOR3(l,t,0.1f),1.0f,0xFFFFFFFF,0.0f,0.0f);
			v[2]=D3DTLVERTEX(D3DXVECTOR3(r,b,0.1f),1.0f,0xFFFFFFFF,1.0f,1.0f);
			v[3]=D3DTLVERTEX(D3DXVECTOR3(r,t,0.1f),1.0f,0xFFFFFFFF,1.0f,0.0f);

			m_pStateBlockSaved->Capture();
			m_pStateBlockDraw->Apply();

	        m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	        m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

			m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
			m_pD3DDevice->SetPixelShader( NULL );
			m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(D3DTLVERTEX));

			m_pStateBlockSaved->Apply();
		}

		char *szText = (char *)szVersionInfo;
		size = m_pFont->MeasureText(szText);
		rect.top = rect.bottom;
		rect.bottom = rect.top + size.cy;
		rect.left = 20 + (iTexWidth - size.cx) / 2;
		rect.right = rect.left + size.cx;

		m_pFont->RenderText(0,szText,rect,0xFF27ABFA);
	}
}

//----------------------------------------------------

void CSpawnScreen::RestoreDeviceObjects()
{
	if(m_pD3DDevice)
	{
		if (m_pTexture)
			m_pTexture->Release();

		//D3DXCreateTextureFromFile(m_pD3DDevice, "logo.png", &m_pTexture);
		//DWORD dwIndex = pFileSystem->GetFileIndex("logo.png");
		DWORD dwIndex = ((CArchiveFS*)pFileSystem)->GetFileIndex(0xbaddea6b);

		if (dwIndex != FS_INVALID_FILE)
		{
			BYTE *pbData = pFileSystem->GetFileData(dwIndex);
			DWORD dwSize = pFileSystem->GetFileSize(dwIndex);
			D3DXCreateTextureFromFileInMemory(m_pD3DDevice, pbData, dwSize, &m_pTexture);
		}

		for(int i=0; i<2; i++)
		{
			m_pD3DDevice->BeginStateBlock();
			m_pD3DDevice->SetTexture( 0, m_pTexture );

			m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
			m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
			m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
			m_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
			m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
			m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
			m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
			m_pD3DDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
			m_pD3DDevice->SetRenderState( D3DRS_VERTEXBLEND,      D3DVBF_DISABLE );
			m_pD3DDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
			m_pD3DDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
			m_pD3DDevice->SetRenderState( D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_RED  | D3DCOLORWRITEENABLE_GREEN |
				D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
			m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
			m_pD3DDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
			m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

			if (i==0)
				m_pD3DDevice->EndStateBlock(&m_pStateBlockSaved);
			else
				m_pD3DDevice->EndStateBlock(&m_pStateBlockDraw);
		}
	}

	if(m_pFont && m_pFont != pDefaultFont)
	{
		m_pFont->RestoreDeviceObjects();
	}
}

//----------------------------------------------------

void CSpawnScreen::DeleteDeviceObjects()
{
	if(m_pTexture)
		m_pTexture->Release();

	if (m_pStateBlockSaved)
		m_pStateBlockSaved->Release();

	if (m_pStateBlockDraw)
		m_pStateBlockDraw->Release();

	m_pTexture = NULL;
	m_pStateBlockSaved = NULL;
	m_pStateBlockDraw = NULL;

	if(m_pFont && m_pFont != pDefaultFont)
	{
		m_pFont->DeleteDeviceObjects();
		m_pFont = NULL;
	}
}

//----------------------------------------------------
