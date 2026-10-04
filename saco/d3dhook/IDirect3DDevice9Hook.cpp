
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
extern CNetGame *pNetGame;
D3DXMATRIX matView, matProj, matWorld;

int dword_101A25F8;
int dword_101A25FC;
int dword_10140D60;
int dword_1026EBA0;
int dword_101516D8;

extern CGame *pGame;
extern CChatWindow *pChatWindow;
extern CNewPlayerTags *pNewPlayerTags;
extern GAME_SETTINGS tSettings;

int  dword_1012DE64;
int  dword_1026EB4C;
int  dword_1026EB50;
int  dword_1026EB54;
int  dword_1026EB58;
int  dword_1026EB5C;
int  dword_1026EB60;
int  dword_1026EB64;
int  dword_1026EB68;
int  dword_1026EB6C;
int  dword_1026EB70;
int  dword_1026EB74;
int  dword_1026EB78;
int  dword_1026EB84;
int  dword_1026EB88;
int  dword_1026EB8C;
int  dword_1026EC24;
int  dword_1026EC2C;
char byte_1026EBB8;
MATRIX4X4 stru_10140CF8;
MATRIX4X4 stru_10140AF8;
VECTOR vec_10140D3C;
VECTOR vec_10140D64;
char byte_10140C78[128];

void sub_1009D8B0(); // .text:1009D8B0
void sub_100C3F50(); // .text:100C3F50
void sub_100C41B0(); // .text:100C41B0
int sub_100A4470(); // .text:100A4470
int sub_100C3E20(int a1, int a2); // .text:100C3E20
int sub_10062270(std::string *pstrFileName); // .text:10062270
void sub_10074480(); // .text:10074480
void sub_10074210(); // .text:10074210
int sub_100B7E00(); // .text:100B7E00
void sub_1009E490(); // .text:1009E490
// samp keeps this gta helper in a data slot, so the call goes through a pointer
float (*dword_10102BF4)(float a1, float a2) = (float (*)(float, float))0x568FE0;

// dispatch helpers invoked on render-manager globals (thiscall)
class CD3DHookDispatch
{
public:
	void FUNC_10066AB0(); // .text:10066AB0
	void FUNC_1006BC70(); // .text:1006BC70
	void FUNC_1006BC80(); // .text:1006BC80
	void FUNC_1006BC90(void *a1, char *a2, int a3, int a4, int a5); // .text:1006BC90
	void FUNC_1006F0B0(); // .text:1006F0B0
	void FUNC_10060D70(); // .text:10060D70
	void FUNC_10071260(); // .text:10071260
	void FUNC_1006BB30(); // .text:1006BB30
	void FUNC_10066ED0(); // .text:10066ED0
	void FUNC_100696F0(); // .text:100696F0
	void FUNC_1006A2E0(); // .text:1006A2E0
	void FUNC_1006F890(); // .text:1006F890
	void FUNC_10071AC0(); // .text:10071AC0
	void FUNC_1006A910(); // .text:1006A910
	void FUNC_100908E0(float a1); // .text:100908E0
	void FUNC_10073780(); // .text:10073780
	void FUNC_1006DC00(); // .text:1006DC00
	void FUNC_10071410(); // .text:10071410
};

//-------------------------------------------

// per-frame nametag pass: walks the player pool and draws a tag over every visible remote ped
void sub_10075330()
{
	if(!pNetGame)
		return;
	if(!pNetGame->GetSettings()->field_38)
		return;

	pNewPlayerTags->FUNC_1006CD80();

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	pGame->FindPlayerPed()->GetMatrix(&stru_10140CF8);

	int iHighestId = pPlayerPool->field_2F3A;
	for(int i = 0; i <= iHighestId; i++)
	{
		if(pPlayerPool->GetSlotState((PLAYERID)i) != 1)
			continue;
		CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt((PLAYERID)i);
		if(!pRemotePlayer)
			continue;
		CPlayerPed *pPlayerPed = pRemotePlayer->m_pPlayerPed;
		if(!pPlayerPed)
			continue;
		if(!pRemotePlayer->field_10A)
			continue;
		if(!pRemotePlayer->field_4)
			continue;

		float fDistance = pPlayerPed->FUNC_1009F190();
		if(fDistance <= pNetGame->GetSettings()->field_27)
		{
			if(pRemotePlayer->field_10A == 19 && pRemotePlayer->field_1E1 && sub_100B7E00())
			{
				((CEntity *)pRemotePlayer->field_1E1)->GetMatrix(&stru_10140AF8);
				vec_10140D3C = stru_10140AF8.pos;
			}
			else
			{
				if(!pPlayerPed->IsAdded())
					continue;
				memset(&vec_10140D3C, 0, sizeof(VECTOR));
				pPlayerPed->GetBonePosition(8, &vec_10140D3C);
			}

			vec_10140D64 = vec_10140D3C;

			CAMERA_AIM *pAim = GameGetInternalAim();
			int iLineOfSight = 0;
			if(pNetGame->GetSettings()->field_2F)
				iLineOfSight = ScriptCommand(&is_line_of_sight_clear,
					vec_10140D64.X, vec_10140D64.Y, vec_10140D64.Z,
					pAim->pos1x, pAim->pos1y, pAim->pos1z, 1, 0, 0, 1, 0);
			if(!pNetGame->GetSettings()->field_2F || iLineOfSight)
			{
				sprintf(byte_10140C78, "%s (%d)", pPlayerPool->FUNC_100175C0(i), i);
				if(!pRemotePlayer->field_10C)
				{
					pNewPlayerTags->Draw((D3DXVECTOR3 *)&vec_10140D64, byte_10140C78,
						pRemotePlayer->GetPlayerColorAsARGB(), pPlayerPed->FUNC_1009F190(),
						pNetGame->field_234, pRemotePlayer->FUNC_10016330());
				}
			}
		}
	}

	pNewPlayerTags->FUNC_1006CD90();
}

//-------------------------------------------

// screenshot grab: reached from Present when the F8 handler raises dword_1012DE64
void sub_100755C0()
{
	dword_1012DE64 = 0;

	std::string strFileName;
	int iIndex = sub_10062270(&strFileName);

	IDirect3DSurface9 *pSurface;
	pD3DDevice->CreateOffscreenPlainSurface(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pSurface, NULL);

	if(SUCCEEDED(pD3DDevice->GetFrontBufferData(0, pSurface)))
	{
		POINT pt;
		pt.x = 0;
		pt.y = 0;
		ClientToScreen(*(HWND *)0xC97C1C, &pt);

		RECT rc;
		GetClientRect(*(HWND *)0xC97C1C, &rc);
		rc.left += pt.x;
		rc.right += pt.x;
		rc.top += pt.y;
		rc.bottom += pt.y;

		D3DXSaveSurfaceToFileA(strFileName.c_str(), D3DXIFF_PNG, pSurface, NULL, &rc);
		pChatWindow->AddInfoMessage("Screenshot Taken - sa-mp-%03i.png", iIndex);
	}
	else
	{
		pChatWindow->AddDebugMessage("Unable to save screenshot.");
	}
}

//-------------------------------------------

HRESULT __stdcall IDirect3DDevice9Hook::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if(dword_1012DE64)
		sub_100755C0();

	sub_10074480();

	if(!pGame->sub_100A0920())
	{
		if(pNetGame)
		{
			CVehiclePool *v3 = pNetGame->GetVehiclePool();
			sub_10074210();
			sub_10075330();
			if(dword_1026EB8C)
				((CD3DHookDispatch *)dword_1026EB8C)->FUNC_10066AB0();
			if(byte_1026EBB8)
			{
				if(dword_1026EB5C)
					((CD3DHookDispatch *)dword_1026EB5C)->FUNC_1006BC70();
				float *v4 = (float *)((char *)v3 + 0x11AD8);
				for(unsigned short i = 0; i < MAX_VEHICLES; i++, v4 += 3)
				{
					if(v3->GetSlotState(i) == 1)
					{
						CEntity *v7 = (CEntity *)v3->GetAt(i);
						if(v7)
						{
							if(v7->FUNC_1009F0C0() <= 20.0f)
							{
								MATRIX4X4 a2;
								v7->GetMatrix(&a2);
								int v35[3];
								v35[0] = *(int *)&a2.pos.X;
								v35[1] = *(int *)&a2.pos.Y;
								v35[2] = *(int *)&a2.pos.Z;
								char string[264];
								sprintf(string,
									"[id: %d, type: %d subtype: %d Health: %.1f preloaded: %u]\n"
									"Distance: %.2fm\n"
									"PassengerSeats: %u\n"
									"cPos: %.3f,%.3f,%.3f\n"
									"sPos: %.3f,%.3f,%.3f",
									i,
									v7->GetModelIndex(),
									v7->FUNC_100B7390(),
									v7->FUNC_100B72A0(),
									*((BYTE *)pGame + v7->GetModelIndex() - 0x122),
									v7->FUNC_1009F0C0(),
									v7->FUNC_100B8340(),
									a2.pos.X, a2.pos.Y, a2.pos.Z, v4[-1], v4[0], v4[1]);
								((CD3DHookDispatch *)dword_1026EB5C)->FUNC_1006BC90(v35, string, 0xFF358BD4, 1, 0);
							}
						}
					}
				}
				if(dword_1026EB5C)
					((CD3DHookDispatch *)dword_1026EB5C)->FUNC_1006BC80();
			}
			if(pNetGame)
			{
				if(*(int *)dword_1026EB4C)
				{
					pGame->sub_100A1DB0(0);
					((CD3DHookDispatch *)dword_1026EB4C)->FUNC_1006F0B0();
					goto LABEL_77;
				}
				if(GetAsyncKeyState(0x74) && pNetGame->GetGameState() == 5)
				{
					pGame->sub_100A1DB0(0);
					if(dword_1026EB6C)
						((CD3DHookDispatch *)dword_1026EB6C)->FUNC_10060D70();
					goto LABEL_77;
				}
				if(pNetGame)
				{
					if(GetAsyncKeyState(0x79))
					{
						pGame->sub_100A1DB0(0);
						if(dword_1026EB70)
							((CD3DHookDispatch *)dword_1026EB70)->FUNC_10071260();
						goto LABEL_77;
					}
					if(pNetGame && GetAsyncKeyState(0x70) && dword_1026EB74)
						((CD3DHookDispatch *)dword_1026EB74)->FUNC_1006BB30();
				}
			}
		}

		if(pChatWindow->field_8)
			pGame->sub_100A1DB0(1);
		else
			pGame->sub_100A1DB0(0);
		if(pGame)
		{
			if(pGame->FindPlayerPed())
			{
				CPlayerPed *PlayerPed = pGame->FindPlayerPed();
				if(PlayerPed->sub_100ADFA0() > 5000)
					pGame->sub_100A1DB0(0);
			}
		}
		if(dword_1026EB78)
			((CD3DHookDispatch *)dword_1026EB78)->FUNC_10066ED0();
		if(pNetGame && pNetGame->GetLabelPool())
			pNetGame->GetLabelPool()->FUNC_10001350();
		if(pChatWindow)
			pChatWindow->FUNC_10067E00();
		if(dword_1026EB84)
			((CD3DHookDispatch *)dword_1026EB84)->FUNC_100696F0();
		if(dword_1026EB88)
			((CD3DHookDispatch *)dword_1026EB88)->FUNC_1006A2E0();
		if(pNetGame)
		{
			if(dword_1026EB50)
				((CD3DHookDispatch *)dword_1026EB50)->FUNC_1006F890();
			if(pNetGame)
			{
				if(dword_1026EB54)
					((CD3DHookDispatch *)dword_1026EB54)->FUNC_10071AC0();
				if(pNetGame && dword_1026EB58)
					((CD3DHookDispatch *)dword_1026EB58)->FUNC_1006A910();
			}
		}
		if(dword_1026EC24)
			((CD3DHookDispatch *)dword_1026EC24)->FUNC_100908E0(10.0f);
		if(dword_1026EC2C)
			((CD3DHookDispatch *)dword_1026EC2C)->FUNC_100908E0(10.0f);
		if(dword_1026EB60)
			((CD3DHookDispatch *)dword_1026EB60)->FUNC_10073780();
		if(dword_1026EB64)
			((CD3DHookDispatch *)dword_1026EB64)->FUNC_1006DC00();
		if(dword_1026EB68)
			((CD3DHookDispatch *)dword_1026EB68)->FUNC_10071410();
	}
	else
	{
		if(pGame->sub_100A0920() && pNetGame && *(int *)0xBA6774 && (GetAsyncKeyState(2) & 1) && !*(char *)0xBA8298)
		{
			float v32, v30;
			char *p = (char *)0xBA873D;
			int n = 175;
			do
			{
				if(*(WORD *)p == 0x1203 && *(WORD *)p)
				{
					v32 = *(float *)(p - 29);
					v30 = *(float *)(p - 25);
				}
				p += 40;
			}
			while(--n);

			RakNet::BitStream bs;
			bs.Write(v32);
			bs.Write(v30);
			bs.Write(dword_10102BF4(v32, v30) + 2.0f);
			pNetGame->GetRakClient()->RPC("w", &bs, HIGH_PRIORITY, RELIABLE, 0, 0);
		}
	}

LABEL_77:
	if(tSettings.bDebug)
		sub_1009E490();
	sub_1009D8B0();
	HRESULT hr = pD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	sub_1009D8B0();
	return hr;
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
	sub_100C3F50();
	HRESULT hr = pD3DDevice->Reset(pPresentationParameters);
	if(hr >= 0)
		sub_100C41B0();
	return hr;
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
	sub_1009D8B0();
	HRESULT hr = pD3DDevice->EndScene();
	sub_1009D8B0();
	return hr;
}

HRESULT __stdcall IDirect3DDevice9Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return pD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT __stdcall IDirect3DDevice9Hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat)
{
	switch (State)
	{
	case D3DTS_PROJECTION:
		matProj = *mat;
		break;
	case D3DTS_VIEW:
		matView = *mat;
		break;
	case D3DTS_WORLD:
		matWorld = *mat;
		break;
	}

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
	if(dword_101A25F8 && dword_101A25FC)
	{
		for(dword_10140D60 = 0; dword_10140D60 != 16; dword_10140D60++)
		{
			if(*(DWORD *)(dword_101A25FC + 4 * dword_10140D60 + 0x110B) == (DWORD)pTexture &&
				*(DWORD *)(dword_101A25FC + 4 * dword_10140D60 + 0x114B))
			{
				return pD3DDevice->SetTexture(Stage,
					(IDirect3DBaseTexture9 *)*(DWORD *)(dword_101A25FC + 4 * dword_10140D60 + 0x114B));
			}
		}
	}

	int v4 = sub_100A4470();
	if(v4 && v4 == (int)pTexture && pNetGame && pNetGame->GetVehiclePool())
	{
		CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
		short v5 = pVehiclePool->FUNC_1001EB90(dword_101516D8);
		int v7 = pVehiclePool->FUNC_10001120(v5);
		if(v7 && *(DWORD *)(v7 + 0x8F))
		{
			sub_100C3E20(9, 2);
			return pD3DDevice->SetTexture(Stage, (IDirect3DBaseTexture9 *)*(DWORD *)(v7 + 0x8F));
		}
		if(*(DWORD *)(dword_1026EBA0 + 0x20))
		{
			sub_100C3E20(9, 2);
			return pD3DDevice->SetTexture(Stage, (IDirect3DBaseTexture9 *)*(DWORD *)(dword_1026EBA0 + 0x20));
		}
	}

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
