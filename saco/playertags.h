
#pragma once
#ifndef PLAYERTAGS_H
#define PLAYERTAGS_H

#include <d3d9.h>
#include <d3dx9.h>

#define MAX_NAME_LENGTH 80
#define NameTextFVF D3DFVF_XYZ|D3DFVF_TEX1
#define HealthBarFVF D3DFVF_XYZ|D3DFVF_DIFFUSE

struct HealthBarVertices_s
{
	float x, y, z;
	D3DCOLOR c;
};

class CPlayerTags
{
private:
	struct NameTextVertex_s
	{
		float x, y, z, tu, tv;
		void Initialise(float fX, float fY, float fTu, float fTv)
		{
			x	= fX;
			y	= fY;
			z	= 0.0f;
			tu	= fTu;
			tv	= fTv;
		}
	};

	IDirect3DDevice9* m_pDevice;
	IDirect3DTexture9* m_pTexture;
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DStateBlock9* m_pOldStates;
	IDirect3DStateBlock9* m_pNewStates;

	float m_fCharWidth;
	float m_fCharHeight;
	float m_fScale;
public:
	CPlayerTags(IDirect3DDevice9* pDevice);
	~CPlayerTags();

	void Begin();
	void End();
	void Draw(D3DXMATRIX* pWorldTransform, char* pNameText, DWORD dwColor, float fHealth);
	void InitQuad(char c, float x, float y2, NameTextVertex_s* pVB);
	void DeleteDeviceObjects();
	void RestoreDeviceObjects();
};

#endif
