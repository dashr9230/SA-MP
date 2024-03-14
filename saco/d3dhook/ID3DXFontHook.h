
#pragma once
#ifndef ID3DXFONTHOOK_H
#define ID3DXFONTHOOK_H

#include <d3d9.h>
#include <d3dx9.h>

interface ID3DXFontHook : public ID3DXFont
{
public:
	HRESULT __stdcall QueryInterface(REFIID iid, LPVOID *ppv);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall GetDevice(LPDIRECT3DDEVICE9 *ppDevice);
	HRESULT __stdcall GetDescA(D3DXFONT_DESCA *pDesc);
	HRESULT __stdcall GetDescW(D3DXFONT_DESCW *pDesc);
	BOOL __stdcall GetTextMetricsA(TEXTMETRICA *pTextMetrics);
	BOOL __stdcall GetTextMetricsW(TEXTMETRICW *pTextMetrics);
	HDC __stdcall GetDC();
	HRESULT __stdcall GetGlyphData(UINT Glyph, LPDIRECT3DTEXTURE9 *ppTexture, RECT *pBlackBox, POINT *pCellInc);
	HRESULT __stdcall PreloadCharacters(UINT First, UINT Last);
	HRESULT __stdcall PreloadGlyphs(UINT First, UINT Last);
	HRESULT __stdcall PreloadTextA(LPCSTR pString, INT Count);
	HRESULT __stdcall PreloadTextW(LPCWSTR pString, INT Count);
	INT __stdcall DrawTextA(LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color);
	INT __stdcall DrawTextW(LPD3DXSPRITE pSprite, LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color);
	HRESULT __stdcall OnLostDevice();
	HRESULT __stdcall OnResetDevice();

	ID3DXFont *m_pD3DFont;
};

#endif
