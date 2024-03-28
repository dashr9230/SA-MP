
#include "main.h"

CUnkClass10::CUnkClass10(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_7C = 0;
	field_113 = -1;
	field_80 = 0;
	field_84 = 0;
	field_78 = 0;
	field_A3 = 0;
	field_A4 = 0;
	field_A5 = 0;
	field_A6 = 0;
	field_A2 = 0;
	field_88 = -1;
	field_9E = GetTickCount();

	D3DXCreateLine(pD3DDevice, &m_pD3DLine);

	D3DXCreateFont(m_pD3DDevice, 22, 0, FW_NORMAL, 1, FALSE,
		SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "SAMPAUX3", &m_pD3DFont1);
	D3DXCreateFont(m_pD3DDevice, 28, 0, FW_NORMAL, 1, FALSE,
		SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "SAMPAUX3", &m_pD3DFont2);
}
