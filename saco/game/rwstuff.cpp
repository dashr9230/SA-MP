
#include "../main.h"
#include "rwstuff.h"

extern int iGtaVersion;

//----------------------------------------------------------

RwRaster* RwRasterCreate(int width, int height, int depth, int flags)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7FB270 : 0x7FB230;
	RwRaster* pRaster = NULL;

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
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F3800 : 0x7F37C0;
	RwTexture* pTexture = NULL;

	_asm push raster
	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pTexture, eax
	_asm pop edx

	return pTexture;
}

RwFrame* RwFrameCreate()
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F0450 : 0x7F0410;
	RwFrame* pFrame = NULL;

	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pFrame, eax

	return pFrame;
}

// 0x100B17D0 - SAMP
void RwFrameDestroy(RwFrame* frame)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F05E0 : 0x7F05A0;

	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

RwCamera* RwCameraCreate()
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE530 : 0x7EE4F0;
	RwCamera* pCamera = NULL;

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

// 0x100B18F0 - SAMP
void RwCameraClear(RwCamera* camera, DWORD* color, int clearMode)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE380 : 0x7EE340;

	_asm push clearMode
	_asm push color
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

// 0x100B1920 - SAMP
void RwCameraBeginUpdate(RwCamera* camera)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE1D0 : 0x7EE190;

	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// 0x100B1950 - SAMP
void RwCameraEndUpdate(RwCamera* camera)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE1C0 : 0x7EE180;

	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// 0x100B1C40 - SAMP
void SetRenderWareCamera(RwCamera* camera)
{
	_asm push camera
	_asm mov edx, 0x7328C0
	_asm call edx
	_asm pop edx
}

// 0x100B1C40 - SAMP
void RpWorldAddLight(RpLight* light)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if (!dwWorld) return;

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x751960 : 0x751910;

	_asm push light
	_asm push dwWorld
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// 0x100B1C40 - SAMP
void RpWorldRemoveLight(RpLight* light)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if (!dwWorld) return;

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7519B0 : 0x751960;

	_asm push light
	_asm push dwWorld
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

RpLight* RpLightCreate(int _type)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x752160 : 0x752110;
	RpLight* pLight = NULL;

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

static VECTOR _axis[3] = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
};

void RwFrameRotate(RwFrame* frame, int axis, float angle)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F1050 : 0x7F1010;
	VECTOR* pAxis = &_axis[axis];

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

// 0x100B5790 - SAMP
void RwMatrixRotate(MATRIX4X4* mat, int axis, float angle)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F2010 : 0x7F1FD0;
	VECTOR* pAxis = &_axis[axis];

	_asm push 1
	_asm push angle
	_asm push pAxis
	_asm push mat
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

void RwMatrixScale(MATRIX4X4* matrix, VECTOR* scale)
{
	matrix->right.X *= scale->X;
	matrix->right.Y *= scale->X;
	matrix->right.Z *= scale->X;

	matrix->up.X *= scale->Y;
	matrix->up.Y *= scale->Y;
	matrix->up.Z *= scale->Y;

	matrix->at.X *= scale->Z;
	matrix->at.Y *= scale->Z;
	matrix->at.Z *= scale->Z;

	matrix->flags &= 0xFFFDFFFC;
}

// 0x100B5790 - SAMP
void RwMatrixInvert(MATRIX4X4* matOut, MATRIX4X4* matIn)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F20B0 : 0x7F2070;

	_asm push matIn
	_asm push matOut
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// 0x100B62F0 - SAMP
void RpAtomicRender(DWORD rpAtomic)
{
	((void(__thiscall*)(DWORD))(*(void***)rpAtomic)[18])(rpAtomic);
}

// 0x100B1800 - SAMP
void RpAtomicDestroy(DWORD atomic)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x749E10 : 0x749DC0;

	_asm push atomic
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// 0x100B19D0 - SAMP
void RpClumpRender(DWORD rpClump)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x749B70 : 0x749B20;

	_asm push rpClump
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// 0x100B1C80 - SAMP
void RpClumpDestroy(DWORD rpClump)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x74A360 : 0x74A310;

	_asm push clump
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// 0x100B6330 - SAMP
void RenderClumpOrAtomic(DWORD rwObject)
{
	if (rwObject)
	{
		if (*(BYTE*)rwObject == 1)
		{
			RpAtomicRender(rwObject);
		}
		else if (*(BYTE*)rwObject == 2)
		{
			RpClumpRender(rwObject);
		}
	}
}

// 0x100B62B0 - SAMP
void DestroyAtomicOrClump(DWORD rwObject)
{
	if (rwObject)
	{
		if (*(BYTE*)rwObject == 1)
		{
			RpAtomicDestroy(rwObject);

			uintptr_t parent = *(uintptr_t*)(rwObject + 4);
			if (parent)
			{
				RwFrameDestroy((RwFrame*)parent);
			}
		}
		else if (*(BYTE*)rwObject == 2)
		{
			RpClumpDestroy(rwObject);
		}
	}
}

// 0x100B1DA0 - SAMP
void RpAnimBlendClumpUpdateAnimations(DWORD rwObject, float step, int flag)
{
	_asm push flag
	_asm push step
	_asm push rwObject
	_asm mov edx, 0x4D34F0
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}