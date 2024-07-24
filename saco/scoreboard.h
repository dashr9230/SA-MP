
#pragma once

class CScoreBoard
{
private:
	int field_0;
	int field_4;
	float field_8;
	float field_C;
	float field_10;
	float field_14;
	float field_18;
	float field_1C;
	int field_20;
	float field_24;
	float field_28;
	float field_2C;
	IDirect3DDevice9 *m_pD3DDevice;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;

public:
	CScoreBoard(IDirect3DDevice9 *pD3DDevice);

	void FUNC_1006E930();
};
