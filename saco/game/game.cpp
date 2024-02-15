
#include "../main.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

extern int iGtaVersion;

char *szGameTextMessage;

int unnamed_10150340[210];

BYTE unnamed_10150688;
int unnamed_1015068C;

float unnamed_10116718 = 2.0f;

BOOL ApplyPreGamePatches();

typedef void (*DrawZone_t)(float *fPos, DWORD *dwColor, BYTE byteMenu);

//-----------------------------------------------------------

CGame::CGame()
{
	// TODO: CGame::CGame()
	m_pGameAudio = new CAudio();
	m_pGameCamera = new CCamera();
	m_pGamePlayer = NULL;
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

void unnamed_100A0060(float a1)
{
  *(float*)0xB7CB5C = a1;
  *(float*)0xB7CB58 = a1;
  unnamed_10116718 = a1 * 3.0f;
}

void CGame::sub_100A0090(int a1, int a2)
{
	if(a1 && a2 && a1 < 1000 / a2)
		Sleep(1000 / a2 - a1 - 1);
}

BYTE CGame::sub_100A00C0()
{
	BYTE result = 2;
	while(result != 210)
	{
		if(!unnamed_10150340[result])
			return result;
		result++;
	}
	return 0;
}

BYTE CGame::sub_100A00F0()
{
	BYTE result = 0;
	BYTE v1 = 2;
	while(v1 != 210)
	{
		if(unnamed_10150340[v1] == 1)
		{
			result++;
		}
		v1++;
	}
	return result;
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

// DOESN'T CURRENTLY WORK

void CGame::RestartEverything()
{
	//*(PBYTE)ADDR_MENU = 1;
	*(PBYTE)ADDR_MENU2 = 1;
	*(PBYTE)ADDR_MENU3 = 1;

	//(PBYTE)ADDR_GAME_STARTED = 0;
	//*(PBYTE)ADDR_MENU = 1;

	OutputDebugString("ShutDownForRestart");
	_asm mov edx, 0x53C550 ; internal_CGame_ShutDownForRestart
	_asm call edx

	OutputDebugString("Timers stopped");
	_asm mov edx, 0x561AA0 ; internal_CTimer_Stop
	_asm call edx

	OutputDebugString("ReInitialise");
	_asm mov edx, 0x53C680 ; internal_CGame_InitialiseWhenRestarting
	_asm call edx

	*(PBYTE)ADDR_GAME_STARTED = 1;

}

//-----------------------------------------------------------

DWORD CGame::GetWeaponInfo(int iWeapon, int iUnk)
{
	DWORD dwRet;

	_asm push iUnk
	_asm push iWeapon
	_asm mov edx, 0x743C60
	_asm call edx
	_asm pop ecx
	_asm pop ecx
	_asm mov dwRet, eax

	return dwRet;
}

//----------------------------------------------------

void CGame::SetGravity(float fGravity)
{
	UnFuck(0x863984, 4);
	*(float*)0x863984 = fGravity;
}

// ---------------------------------------------------

void CGame::SetWantedLevel(BYTE byteLevel)
{
	*(BYTE*)0x58DB60 = byteLevel;
}

//-----------------------------------------------------------

void CGame::SetGameTextCount(WORD wCount)
{
	*(WORD*)0xA44B68 = wCount;
}

//-----------------------------------------------------------

void CGame::DrawGangZone(float fPos[], DWORD dwColor)
{
	((DrawZone_t)0x5853D0)(fPos, &dwColor, *(BYTE*)ADDR_MENU);
}

//-----------------------------------------------------------

