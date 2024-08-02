
#include "../main.h"
#include "rwstuff.h"

extern int iGtaVersion;

//----------------------------------------------------------

RwRaster* RwRasterCreate(int width, int height, int depth, int flags)
{
	RwRaster* pRaster = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7FB270 : 0x7FB230;

	_asm push flags
	_asm push depth
	_asm push height
	_asm push width
	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pRaster, eax
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx

	return pRaster;
}

RwTexture* RwTextureCreate(RwRaster *raster)
{
	RwTexture* pTexture = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F3800 : 0x7F37C0;

	_asm push raster
	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pTexture, eax
	_asm pop edx

	return pTexture;
}

void RwTextureDestroy(RwTexture *texture)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F3860 : 0x7F3820;

	_asm push texture
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

RwFrame* RwFrameCreate()
{
	RwFrame* pFrame = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F0450 : 0x7F0410;

	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pFrame, eax

	return pFrame;
}

void RwFrameDestroy(RwFrame *frame)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F05E0 : 0x7F05A0;

	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

void RpAtomicDestroy(RpAtomic *atomic)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x749E10 : 0x749DC0;

	_asm push atomic
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

RwCamera* RwCameraCreate()
{
	RwCamera* pCamera = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE530 : 0x7EE4F0;

	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pCamera, eax

	return pCamera;
}

void RpWorldAddCamera(RwCamera *camera)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if(!dwWorld) return;

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x750F70 : 0x750F20;

	_asm push camera
	_asm push dwWorld
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

void RwObjectHasFrameSetFrame(RwCamera *camera, RwFrame *frame)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x804F30 : 0x804EF0l;

	_asm push frame
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

void SetCameraFrameAndZBuffer(RwCamera *camera, RwRaster *frameBuffer, RwRaster *zBuffer)
{
	_asm mov ebx, camera
	
	_asm mov edx, frameBuffer
	_asm mov [ebx+0x60], edx

	_asm mov edx, zBuffer
	_asm mov [ebx+0x64], edx
}

void RwCameraSetClipPlane(RwCamera *camera, float farClip, float nearClip)
{
	DWORD dwRwCameraSetFarClipPlane;
	DWORD dwRwCameraSetNearClipPlane;

	if(iGtaVersion == GTASA_VERSION_USA10)
	{
		dwRwCameraSetFarClipPlane = 0x7EE2A0;
		dwRwCameraSetNearClipPlane = 0x7EE1D0;
	}
	else
	{
		dwRwCameraSetFarClipPlane = 0x7EE2E0;
		dwRwCameraSetNearClipPlane = 0x7EE210;
	}

	_asm push farClip
	_asm push camera
	_asm mov edx, dwRwCameraSetFarClipPlane
	_asm call edx
	_asm pop edx
	_asm pop edx

	_asm push nearClip
	_asm push camera
	_asm mov edx, dwRwCameraSetNearClipPlane
	_asm call edx
	_asm pop edx
	_asm pop edx
}

void RwCameraSetViewWindow(RwCamera *camera, VECTOR2D *viewWindow)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE450 : 0x7EE410;

	_asm push viewWindow
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

RpLight* RpLightCreate(int _type)
{
	RpLight* pLight = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x752160 : 0x752110;

	_asm push _type
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm mov pLight, eax

	return pLight;
}

void RpLightSetColor(RpLight *light, RwRGBAReal *color)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x751AE0 : 0x751A90;

	_asm push color
	_asm push light
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

void RwCameraSetProjection(RwCamera *camera, int projection)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE3E0 : 0x7EE3A0;

	_asm push projection
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

void RwFrameTranslate(RwFrame *frame, VECTOR *v, int combine)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F0E70 : 0x7F0E30;

	_asm push combine
	_asm push v
	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

VECTOR stru_10117384[3] = {
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
};

void RwFrameRotate(RwFrame* frame, int axis, float angle)
{
	VECTOR* pAxis = &stru_10117384[axis];
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F1050 : 0x7F1010;

	_asm push 1
	_asm push angle
	_asm push pAxis
	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

