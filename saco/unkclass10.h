
#pragma once

class CUnkClass10
{
private:
	int field_0;
	int field_4;
	RECT field_8[7];
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	WORD field_88;
	char _gap8A[20];
	DWORD field_9E;
	char field_A2;
	char field_A3;
	char field_A4;
	char field_A5;
	char field_A6;
	int field_A7;
	int field_AB;
	char _gapAF[84];
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXLine *m_pD3DLine;
	ID3DXFont *m_pD3DFont1;
	ID3DXFont *m_pD3DFont2;
	int field_113;

public:

	int GetField80() { return field_80; }; // .text:10002E70, out of line copy

	CUnkClass10(IDirect3DDevice9 *pD3DDevice);

	float FUNC_10071C20(D3DXVECTOR3 *pPos, D3DXVECTOR2 *pOut);
	PCHAR FUNC_10071F90();
	PCHAR FUNC_10071FA0(int nIndex);
	void FUNC_10072040(RECT *pRect, POINT *pPoint);
	void FUNC_100720A0();
	void FUNC_100721F0(int nStyle);
	BOOL FUNC_100723C0();

};
