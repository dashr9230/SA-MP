
#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define HealthBar1FVF D3DFVF_XYZRHW|D3DFVF_DIFFUSE

struct HealthBarVertices1_s
{
	float x, y, z, rhw;
	D3DCOLOR c;
};

class CNewPlayerTags
{
private:
	IDirect3DDevice9* m_pDevice;
	IDirect3DStateBlock9* m_pStates;
	ID3DXSprite* m_pSprite;

public:
	CNewPlayerTags(IDirect3DDevice9* pDevice);
	~CNewPlayerTags();

	void Begin();
	void End();
	void Draw(D3DXVECTOR3* pPlayerPos, float fHealth, float fArmor, float fDistanceFromLocalPlayer);

	void DeleteDeviceObjects();
	void RestoreDeviceObjects();
};
