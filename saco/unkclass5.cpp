
#include "main.h"

CUnkClass5::CUnkClass5(IDirect3DDevice9 *pD3DDevice)
{
	field_0 = 0;
	field_4 = 0;
	m_pD3DDevice = pD3DDevice;
	field_28 = 0;
	field_30 = 0;
	field_14 = 640;
	field_18 = 300;
	field_1C = 210;
	field_20 = 30;
	field_34 = 3;

	strcpy(field_38, "Type");
	strcpy(field_B9, "ID");
	strcpy(field_13A, "Progress");

	field_23C = 180;
	field_240 = 220;
	field_24C = 0;
	field_250 = -1;
	field_25C = 0;
	field_258 = 0;
	field_254 = RakNet::GetTime();
}
