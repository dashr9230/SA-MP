
#pragma once

#include "address.h"
#include "audio.h"
#include "camera.h"

//-----------------------------------------------------------

class CGame // size: 322
{
private:
	CAudio *m_pGameAudio;
	CCamera *m_pGameCamera;
	int field_8;
	char gapC[29];
	int field_29;
	char gap2D[4];
	int field_31;
	char gap35[24];
	int field_4D;
	char gap51[4];
	int field_55;
	int field_59;
	int field_5D;
	int field_61;
	int field_65;
	int field_69;
	char field_6D;
	char field_6E[212];

public:

	HWND	GetMainWindowHwnd() { return *(HWND *)ADDR_HWND; };

	void	InitGame();

	CGame();

	void sub_100A0010();
	void sub_100A0060();
	void sub_100A0090();
	void sub_100A00C0();
	void sub_100A00F0();
	void sub_100A0110();
	void sub_100A0210();
	void sub_100A0250();
	void sub_100A02E0();
	void sub_100A0310();
	void sub_100A0330();
	void sub_100A03D0();
	void sub_100A03E0();
	void sub_100A0400();



	void sub_100A05D0();
	void sub_100A06F0();

	void sub_100A1C10();
};

//-----------------------------------------------------------
