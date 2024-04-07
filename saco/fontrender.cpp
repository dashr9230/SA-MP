
#include "main.h"
#include "fontrender.h"

//----------------------------------------------------

CFontRender::CFontRender(IDirect3DDevice9* pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_0 = NULL;
	field_4 = NULL;
	field_8 = NULL;
	field_C = NULL;
	field_14 = NULL;
	field_10 = NULL;

	CreateFonts();
}

CFontRender::~CFontRender()
{
	SAFE_RELEASE(field_0);
	SAFE_RELEASE(field_4);
	SAFE_RELEASE(field_8);
	SAFE_RELEASE(field_C);
	SAFE_RELEASE(field_14);
	SAFE_RELEASE(field_10);
}

void CFontRender::CreateFonts()
{
	if(!m_pD3DDevice) return;
	SAFE_RELEASE(field_0);
	SAFE_RELEASE(field_4);
	SAFE_RELEASE(field_8);
	SAFE_RELEASE(field_C);
	SAFE_RELEASE(field_14);
	SAFE_RELEASE(field_10);

	// TODO: CFontRender::CreateFonts

	//D3DXCreateFont();
}

void CFontRender::DeleteDeviceObjects()
{
	field_0->OnLostDevice();
	field_4->OnLostDevice();
	field_8->OnLostDevice();
	field_C->OnLostDevice();
	field_14->OnLostDevice();
	field_10->OnLostDevice();
}

void CFontRender::RestoreDeviceObjects()
{
	field_0->OnResetDevice();
	field_4->OnResetDevice();
	field_8->OnResetDevice();
	field_C->OnResetDevice();
	field_14->OnResetDevice();
	field_10->OnResetDevice();
}

SIZE CFontRender::MeasureText(char * szString, DWORD dwFormat)
{
	SIZE size = {0,0};

	// TODO: CFontRender::MeasureText .text:1006B200
	
	return size;
}

void CFontRender::RenderText(ID3DXSprite * pSprite, char * sz, RECT rect, DWORD dwColor, BOOL bShadowed)
{
	// TODO: CFontRender::RenderText .text:1006B360
}
