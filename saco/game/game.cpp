
#include "../main.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

char *szGameTextMessage;

int unnamed_10150340[210];

BOOL ApplyPreGamePatches();

CGame::CGame()
{
	// TODO: CGame::CGame()
	m_pGameAudio = new CAudio();
	m_pGameCamera = new CCamera();
	field_8 = 0;
	field_4D = 0;
	field_29 = 0;
	field_31 = 0;
	field_61 = 0;
	field_65 = 0;
	field_69 = 0;
	field_6D = 0;
	memset(unnamed_10150340, 0, sizeof(unnamed_10150340));
	memset(field_6E, 0, sizeof(field_6E));
	field_55 = 0;
	field_59 = 1;
	field_5D = 90;
}

void CGame::sub_100A0010()
{
	int time = (int)RakNet::GetTime();
	if(unnamed_1015068C)
	{
		if((time - unnamed_1015068C) > 30)
		{
			unnamed_10150688++;
			if(unnamed_10150688 == 5)
				unnamed_10150688 = 0;
			unnamed_1015068C = time;
		}
		*(BYTE*)0xB7356E = unnamed_10150688;
	}
	else
	{
		unnamed_1015068C = time;
	}
}

//-----------------------------------------------------------

void CGame::InitGame()
{
	// Create a buffer for game text.
	szGameTextMessage = (char*)calloc(1,513);

	// Init the keystate stuff.
	GameKeyStatesInit();

	// Init the aim stuff.
	GameAimSyncInit();

	// Init radar colors
	GameResetRadarColors();

	if(!ApplyPreGamePatches()) {
		MessageBox(0,
			"I can't determine your GTA version.\r\nSA-MP only supports GTA:SA v1.0 USA/EU",
			"Version Error",MB_OK | MB_ICONEXCLAMATION);
		ExitProcess(1);
	}
}

//-----------------------------------------------------------

void CGame::StartGame()
{
	// TODO: CGame::StartGame() .text:100A08E0
}

//-----------------------------------------------------------

void CGame::sub_100A0060()
{
	// No indication for __thiscall, maybe not part of CGame

	// TODO: CGame::sub_100A0060() .text:100A0060 (unused)
}

void CGame::sub_100A0090()
{
	// TODO: CGame::sub_100A0090() .text:100A0090
}

void CGame::sub_100A00C0()
{
	// TODO: CGame::sub_100A00C0() .text:100A00C0 (unused)
}

void CGame::sub_100A00F0()
{
	// TODO: CGame::sub_100A00F0() .text:100A00F0
}

void CGame::sub_100A0110()
{
	// TODO: CGame::sub_100A0110() .text:100A0110
}

void CGame::sub_100A0210()
{
	// TODO: CGame::sub_100A0210() .text:100A0210
}

void CGame::sub_100A0250()
{
	// TODO: CGame::sub_100A0250() .text:100A0250
}

void CGame::sub_100A02E0()
{
	// TODO: CGame::sub_100A02E0() .text:100A02E0 (unused)
}

void CGame::sub_100A0310()
{
	// TODO: CGame::sub_100A0310() .text:100A0310 (unused)
}

void CGame::sub_100A0330()
{
	// TODO: CGame::sub_100A0330() .text:100A0330
}

void CGame::sub_100A03D0()
{
	// TODO: CGame::sub_100A03D0() .text:100A03D0 (unused)
}

void CGame::sub_100A03E0()
{
	// TODO: CGame::sub_100A03E0() .text:100A03E0 (unused)
}

void CGame::sub_100A0400()
{
	// TODO: CGame::sub_100A0400() .text:100A0400
}










void CGame::sub_100A05D0()
{
	// TODO: CGame::sub_100A05D0() .text:100A05D0
}

void CGame::sub_100A06F0()
{
	// TODO: CGame::sub_100A06F0() .text:100A06F0
}


















void CGame::sub_100A1C10()
{
	// TODO: CGame::sub_100A1C10() .text:100A1C10
}



//-----------------------------------------------------------

DWORD CGame::GetD3DDevice()
{
	DWORD pdwD3DDev=0;

	if(iGtaVersion == GTASA_VERSION_USA10) {
		_asm mov edx, ADDR_RENDERWARE_GETD3D_USA10
		_asm call edx
		_asm mov pdwD3DDev, eax
	}
	else if (iGtaVersion == GTASA_VERSION_EU10) {
		_asm mov edx, ADDR_RENDERWARE_GETD3D_EU10
		_asm call edx
		_asm mov pdwD3DDev, eax
	}

	return pdwD3DDev;
}

//-----------------------------------------------------------

