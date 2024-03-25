
#include "main.h"

CUnkClass3::CUnkClass3(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_1C = 0;
	field_28 = 0;
	field_34 = 0;
	field_30 = 0;
	field_2C = 0;
	field_C = 600;
	field_10 = 300;
	field_14 = 100;
	field_18 = 30;

	memset(field_89, 0, sizeof(field_89));
}
