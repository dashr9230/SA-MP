
/*
	Changes found:
		IDirect3DDevice9Hook::Present
		IDirect3DDevice9Hook::Reset
		IDirect3DDevice9Hook::CreateDepthStencilSurface
		IDirect3DDevice9Hook::EndScene
		IDirect3DDevice9Hook::SetTransform
		IDirect3DDevice9Hook::SetTexture
*/

#include "IDirect3DDevice9Hook.h"
#include "../main.h"

extern IDirect3DDevice9 *pD3DDevice;

//-------------------------------------------

HRESULT __stdcall IDirect3DDevice9Hook::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	// TODO: IDirect3DDevice9Hook::Present

	return pD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT __stdcall IDirect3DDevice9Hook::QueryInterface(REFIID riid, void** ppvObj)
{
	return pD3DDevice->QueryInterface(riid, ppvObj);
}

ULONG __stdcall IDirect3DDevice9Hook::AddRef()
{
	return pD3DDevice->AddRef();
}

ULONG __stdcall IDirect3DDevice9Hook::Release()
{
	return pD3DDevice->Release();
}

HRESULT __stdcall IDirect3DDevice9Hook::TestCooperativeLevel()
{
	return pD3DDevice->TestCooperativeLevel();
}

UINT __stdcall IDirect3DDevice9Hook::GetAvailableTextureMem()
{
	return pD3DDevice->GetAvailableTextureMem();
}

HRESULT __stdcall IDirect3DDevice9Hook::EvictManagedResources()
{
	return pD3DDevice->EvictManagedResources();
}

HRESULT __stdcall IDirect3DDevice9Hook::GetDirect3D(IDirect3D9** ppD3D9)
{
	return pD3DDevice->GetDirect3D(ppD3D9);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return pD3DDevice->GetDeviceCaps(pCaps);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	return pD3DDevice->GetDisplayMode(iSwapChain, pMode);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return pD3DDevice->GetCreationParameters(pParameters);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
	return pD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void __stdcall IDirect3DDevice9Hook::SetCursorPosition(int X, int Y, DWORD Flags)
{
	pD3DDevice->SetCursorPosition(X, Y, Flags);
}

BOOL __stdcall IDirect3DDevice9Hook::ShowCursor(BOOL bShow)
{
	return pD3DDevice->ShowCursor(bShow);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	return pD3DDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	return pD3DDevice->GetSwapChain(iSwapChain, pSwapChain);
}

UINT __stdcall IDirect3DDevice9Hook::GetNumberOfSwapChains()
{
	return pD3DDevice->GetNumberOfSwapChains();
}

HRESULT __stdcall IDirect3DDevice9Hook::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	// TODO: IDirect3DDevice9Hook::Reset

	return pD3DDevice->Reset(pPresentationParameters);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return pD3DDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	return pD3DDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return pD3DDevice->SetDialogBoxMode(bEnableDialogs);
}

void __stdcall IDirect3DDevice9Hook::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	pD3DDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void __stdcall IDirect3DDevice9Hook::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	pD3DDevice->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateDepthStencilSurface(Width, Height, Format, D3DMULTISAMPLE_NONE, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
	return pD3DDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT __stdcall IDirect3DDevice9Hook::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	return pD3DDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
	return pD3DDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	return pD3DDevice->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT __stdcall IDirect3DDevice9Hook::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	return pD3DDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT __stdcall IDirect3DDevice9Hook::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	return pD3DDevice->ColorFill(pSurface, pRect, color);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return pD3DDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	return pD3DDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
	return pD3DDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return pD3DDevice->SetDepthStencilSurface(pNewZStencil);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return pD3DDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT __stdcall IDirect3DDevice9Hook::BeginScene()
{
	return pD3DDevice->BeginScene();
}

HRESULT __stdcall IDirect3DDevice9Hook::EndScene()
{
	// TODO: IDirect3DDevice9Hook::EndScene

	return pD3DDevice->EndScene();
}

HRESULT __stdcall IDirect3DDevice9Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return pD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat)
{
	// TODO: IDirect3DDevice9Hook::SetTransform

	return pD3DDevice->SetTransform(State, mat);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* mat)
{
	return pD3DDevice->GetTransform(State, mat);
}

HRESULT __stdcall IDirect3DDevice9Hook::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat)
{
	return pD3DDevice->MultiplyTransform(State, mat);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	return pD3DDevice->SetViewport(pViewport);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetViewport(D3DVIEWPORT9* pViewport)
{
	return pD3DDevice->GetViewport(pViewport);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	return pD3DDevice->SetMaterial(pMaterial);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return pD3DDevice->GetMaterial(pMaterial);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
	return pD3DDevice->SetLight(Index, pLight);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
	return pD3DDevice->GetLight(Index, pLight);
}

HRESULT __stdcall IDirect3DDevice9Hook::LightEnable(DWORD Index, BOOL Enable)
{
	return pD3DDevice->LightEnable(Index, Enable);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return pD3DDevice->GetLightEnable(Index, pEnable);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetClipPlane(DWORD Index, CONST float* pPlane)
{
	return pD3DDevice->SetClipPlane(Index, pPlane);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetClipPlane(DWORD Index, float* pPlane)
{
	return pD3DDevice->GetClipPlane(Index, pPlane);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return pD3DDevice->SetRenderState(State, Value);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return pD3DDevice->GetRenderState(State, pValue);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	return pD3DDevice->CreateStateBlock(Type, ppSB);
}

HRESULT __stdcall IDirect3DDevice9Hook::BeginStateBlock()
{
	return pD3DDevice->BeginStateBlock();
}

HRESULT __stdcall IDirect3DDevice9Hook::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return pD3DDevice->EndStateBlock(ppSB);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return pD3DDevice->SetClipStatus(pClipStatus);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return pD3DDevice->GetClipStatus(pClipStatus);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	return pD3DDevice->GetTexture(Stage, ppTexture);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
	// TODO: IDirect3DDevice9Hook::SetTexture

	return pD3DDevice->SetTexture(Stage, pTexture);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return pD3DDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return pD3DDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	return pD3DDevice->GetSamplerState(Sampler, Type, pValue);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	return pD3DDevice->SetSamplerState(Sampler, Type, Value);
}

HRESULT __stdcall IDirect3DDevice9Hook::ValidateDevice(DWORD* pNumPasses)
{
	return pD3DDevice->ValidateDevice(pNumPasses);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return pD3DDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return pD3DDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return pD3DDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	return pD3DDevice->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetScissorRect(CONST RECT* pRect)
{
	return pD3DDevice->SetScissorRect(pRect);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetScissorRect(RECT* pRect)
{
	return pD3DDevice->GetScissorRect(pRect);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return pD3DDevice->SetSoftwareVertexProcessing(bSoftware);
}

BOOL __stdcall IDirect3DDevice9Hook::GetSoftwareVertexProcessing()
{
	return pD3DDevice->GetSoftwareVertexProcessing();
}

HRESULT __stdcall IDirect3DDevice9Hook::SetNPatchMode(float nSegments)
{
	return pD3DDevice->SetNPatchMode(nSegments);
}

float __stdcall IDirect3DDevice9Hook::GetNPatchMode()
{
	return pD3DDevice->GetNPatchMode();
}

HRESULT __stdcall IDirect3DDevice9Hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return pD3DDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	return pD3DDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return pD3DDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT __stdcall IDirect3DDevice9Hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return pD3DDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT __stdcall IDirect3DDevice9Hook::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	return pD3DDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	return pD3DDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return pD3DDevice->SetVertexDeclaration(pDecl);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return pD3DDevice->GetVertexDeclaration(ppDecl);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetFVF(DWORD FVF)
{
	return pD3DDevice->SetFVF(FVF);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetFVF(DWORD* pFVF)
{
	return pD3DDevice->GetFVF(pFVF);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	return pD3DDevice->CreateVertexShader(pFunction, ppShader);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	return pD3DDevice->SetVertexShader(pShader);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return pD3DDevice->GetVertexShader(ppShader);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return pD3DDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return pD3DDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return pD3DDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return pD3DDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
	return pD3DDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return pD3DDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return pD3DDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride)
{
	return pD3DDevice->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
{
	return pD3DDevice->SetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider)
{
	return pD3DDevice->GetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return pD3DDevice->SetIndices(pIndexData);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return pD3DDevice->GetIndices(ppIndexData);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	return pD3DDevice->CreatePixelShader(pFunction, ppShader);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return pD3DDevice->SetPixelShader(pShader);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return pD3DDevice->GetPixelShader(ppShader);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return pD3DDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return pD3DDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return pD3DDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return pD3DDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
	return pD3DDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return pD3DDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT __stdcall IDirect3DDevice9Hook::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return pD3DDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT __stdcall IDirect3DDevice9Hook::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return pD3DDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT __stdcall IDirect3DDevice9Hook::DeletePatch(UINT Handle)
{
	return pD3DDevice->DeletePatch(Handle);
}

HRESULT __stdcall IDirect3DDevice9Hook::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	return pD3DDevice->CreateQuery(Type, ppQuery);
}
