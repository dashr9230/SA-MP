
#pragma once

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

	CGame();
};

//-----------------------------------------------------------
