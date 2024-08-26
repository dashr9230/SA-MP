
#include "main.h"

CUnkClass13::CUnkClass13(IDirect3DDevice9 *pD3DDevice)
{
	field_20 = 0;
	m_pD3DSurface = NULL;
	m_pD3DTexture = NULL;
	m_pD3DRenderToSurface = NULL;
	m_pD3DDevice = pD3DDevice;

	RestoreDeviceObjects();
}

CUnkClass13::~CUnkClass13()
{
	DeleteDeviceObjects();
}

void CUnkClass13::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pD3DSurface);
	SAFE_RELEASE(m_pD3DTexture);
	SAFE_RELEASE(m_pD3DRenderToSurface);
	SAFE_RELEASE(field_20);
}

void CUnkClass13::RestoreDeviceObjects()
{
	m_pD3DDevice->GetDisplayMode(0, &m_DisplayMode);

	HRESULT hr = D3DXCreateTexture(m_pD3DDevice, 128, 32, 1,
		D3DUSAGE_RENDERTARGET, m_DisplayMode.Format, D3DPOOL_DEFAULT, &m_pD3DTexture);
	if(SUCCEEDED(hr))
	{
		D3DSURFACE_DESC desc;

		m_pD3DTexture->GetSurfaceLevel(0, &m_pD3DSurface);
		m_pD3DSurface->GetDesc(&desc);
		
		D3DXCreateRenderToSurface(m_pD3DDevice, desc.Width, desc.Height,
			desc.Format, TRUE, D3DFMT_D16, &m_pD3DRenderToSurface);
	}
}