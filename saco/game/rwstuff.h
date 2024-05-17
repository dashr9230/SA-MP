
#pragma once

struct RwRaster
{
	char _gap0;
};

struct RwTexture
{
	char _gap0;
};

struct RwFrame
{
	char _gap0;
};

struct RwCamera
{
	char _gap0;
};

struct RpLight
{
	char _gap0;
};

struct RwRGBAReal
{
	float r, g, b, a;
};

RwRaster* RwRasterCreate(int width, int height, int depth, int flags);
RwTexture* RwTextureCreate(RwRaster *raster);
RwFrame* RwFrameCreate();
void RwFrameDestroy(RwFrame* frame);
RwCamera* RwCameraCreate();
void RpWorldAddCamera(RwCamera *camera);
void RwObjectHasFrameSetFrame(RwCamera *camera, RwFrame *frame);
void SetCameraFrameAndZBuffer(RwCamera *camera, RwRaster *frameBuffer, RwRaster *zBuffer);
void RwCameraSetClipPlane(RwCamera *camera, float farClip, float nearClip);
void RwCameraSetViewWindow(RwCamera *camera, VECTOR2D *viewWindow);
void RwCameraClear(RwCamera* camera, DWORD* color, int clearMode);
void RwCameraBeginUpdate(RwCamera* camera);
void RwCameraEndUpdate(RwCamera* camera);
void SetRenderWareCamera(RwCamera* camera);
void RpWorldAddLight(RpLight* light);
void RpWorldRemoveLight(RpLight* light);
RpLight* RpLightCreate(int type);
void RpLightSetColor(RpLight *light, RwRGBAReal *color);
void RwCameraSetProjection(RwCamera *camera, int projection);
void RwFrameTranslate(RwFrame *frame, VECTOR *v, int combine);
void RwFrameRotate(RwFrame* frame, int axis, float angle);
void RwMatrixRotate(MATRIX4X4* mat, int axis, float angle);
void RwMatrixScale(MATRIX4X4* matrix, VECTOR* scale);
void RwMatrixInvert(MATRIX4X4* matOut, MATRIX4X4* matIn);
void RpAtomicRender(DWORD rpAtomic);
void RpAtomicDestroy(DWORD atomic);
void RpClumpRender(DWORD rpClump);
void RpClumpDestroy(DWORD rpClump);
void RenderClumpOrAtomic(DWORD rwObject);
void DestroyAtomicOrClump(DWORD rwObject);
void RpAnimBlendClumpUpdateAnimations(DWORD rwObject, float step, int flag);