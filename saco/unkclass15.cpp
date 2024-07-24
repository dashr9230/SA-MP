
#include "main.h"

CUnkClass15::CUnkClass15()
{
	m_pCamera = NULL;
	m_pFrame = NULL;
	m_pTexture = NULL;
	field_18 = 0;
	m_pLight = NULL;

	FUNC_1006BE40();
}

bool CUnkClass15::FUNC_1006BE40()
{
	RwRGBAReal color;
	VECTOR v;
	VECTOR2D viewWindow;

	m_pLight = RpLightCreate(2);

	if(!m_pLight) return false;

	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	RpLightSetColor(m_pLight, &color);

	m_pRaster1 = RwRasterCreate(256, 256, 0, 1285);
	m_pRaster2 = RwRasterCreate(256, 256, 0, 1);

	m_pCamera = RwCameraCreate();
	m_pFrame = RwFrameCreate();

	v.X = 0.0f;
	v.Y = 0.0f;
	v.Z = 50.0f;

	RwFrameTranslate(m_pFrame, &v, 1);
	RwFrameRotate(m_pFrame, 0, 90.0f);

	if(!m_pCamera || !m_pFrame || !m_pRaster1 || !m_pRaster2)
		return false;

	SetCameraFrameAndZBuffer(m_pCamera, m_pRaster1, m_pRaster2);
	RwObjectHasFrameSetFrame(m_pCamera, m_pFrame);
	RwCameraSetClipPlane(m_pCamera, 300.0f, 0.01f);

	viewWindow.X = 0.5f;
	viewWindow.Y = 0.5f;

	RwCameraSetViewWindow(m_pCamera, &viewWindow);
	RwCameraSetProjection(m_pCamera, 1);
	RpWorldAddCamera(m_pCamera);

	m_pTexture = RwTextureCreate(m_pRaster1);

	return true;
}