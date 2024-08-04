
#pragma once

struct RwRaster { char _gap0; };
struct RwTexture { char _gap0; };
struct RwFrame { char _gap0; };
struct RwCamera { char _gap0; };
struct RpLight { char _gap0; };
struct RwRGBAReal { float r, g, b, a; };
struct RpAtomic { char _gap0; };
struct RwRGBA { char _gap0; };
struct RpClump { char _gap0; };

RwRaster* RwRasterCreate(int width, int height, int depth, int flags);
RwTexture* RwTextureCreate(RwRaster *raster);
void RwTextureDestroy(RwTexture *texture);
RwFrame* RwFrameCreate();
void RwFrameDestroy(RwFrame *frame);
void RpAtomicDestroy(RpAtomic *atomic);
RwCamera* RwCameraCreate();
void RwRasterDestroy(RwRaster *raster);
void CShadowCamera_Create(int iRasterSize);
void RpWorldAddCamera(RwCamera *camera);
void RwCameraClear(RwCamera *camera, RwRGBA *color, int clearMode);
void RwCameraBeginUpdate(RwCamera *camera);
void RwCameraEndUpdate(RwCamera *camera);
void RwObjectHasFrameSetFrame(RwCamera *camera, RwFrame *frame);
void SetCameraFrameAndZBuffer(RwCamera *camera, RwRaster *frameBuffer, RwRaster *zBuffer);
void RpClumpRender(RpClump *clump);
void RpWorldAddClump(RpClump *clump);
void RpWorldRemoveClump(RpClump *clump);
void RwCameraSetClipPlane(RwCamera *camera, float farClip, float nearClip);
void RwCameraSetViewWindow(RwCamera *camera, VECTOR2D *viewWindow);
RpLight* RpLightCreate(int type);
void RpLightSetColor(RpLight *light, RwRGBAReal *color);
void RwCameraSetProjection(RwCamera *camera, int projection);
void RwFrameTranslate(RwFrame *frame, VECTOR *v, int combine);
void RwFrameRotate(RwFrame* frame, int axis, float angle);
