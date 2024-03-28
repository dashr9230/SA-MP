
#pragma once

class CUnkClass10
{
private:
	char _gap0[120];
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
	char _gapA7[92];
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXLine *m_pD3DLine;
	ID3DXFont *m_pD3DFont1;
	ID3DXFont *m_pD3DFont2;
	int field_113;

public:
	CUnkClass10(IDirect3DDevice9 *pD3DDevice);

};
