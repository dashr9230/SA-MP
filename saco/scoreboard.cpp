
#include "main.h"

extern CGame* pGame;

CScoreBoard::CScoreBoard(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_34 = 0;
	field_38 = 0;
	field_3C = 0;
	field_40 = 0;
	field_4 = 0;

	if(pGame && pGame->GetScreenWidth() > 800)
	{
		field_14 = 800.0f;
		field_18 = 600.0f;
	}
	else
	{
		field_14 = 640.0f;
		field_18 = 480.0f;
	}
	field_1C = 60.0f;
	field_20 = 0;
	field_24 = 0.085f;
	field_28 = 0.265625f;
	field_2C = 0.437500f;

	sub_1006E930();

	field_0 = 0;
}

void CScoreBoard::sub_1006E930()
{
	RECT rect;

	GetClientRect(pGame->GetMainWindowHwnd(),&rect);

	field_10 = 1.0f;
	field_8 = rect.right * 0.5f - field_14 * 0.5f;
	field_C = rect.bottom * 0.5f - field_18 * 0.5f;
}
