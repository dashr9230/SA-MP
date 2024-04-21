
#pragma once

class CNetStats
{
private:
	int field_0;
	int field_4;
	DWORD field_8;
	int field_C;
	int field_10;
	IDirect3DDevice9* m_pD3DDevice;

public:
	CNetStats(IDirect3DDevice9 *pD3DDevice);

};
