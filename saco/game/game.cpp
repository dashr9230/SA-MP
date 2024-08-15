
#include "../main.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

#include <dinput.h>

extern int iGtaVersion;
extern IDirect3DDevice9 *pD3DDevice;

void GameInstallHooks();
BOOL ApplyPreGamePatches();
void ApplyInGamePatches();
void InitAnimNameHashes();

DWORD dwDummyActiveMouseState;
unsigned char *szGameTextMessage;
HWND hWindowHandle;

int unnamed_10150340[210];

IDirectInputDevice8 *pDirectInputMouse;
BYTE unnamed_10150688;
int unnamed_1015068C;

float unnamed_10116718 = 2.0f;

BOOL ApplyPreGamePatches();
bool GLOBAL_101506A4;

typedef void (*DrawZone_t)(float *fPos, DWORD *dwColor, BYTE byteMenu);

//-----------------------------------------------------------

CGame::CGame()
{
	m_pGameAudio = new CAudio();
	m_pGameCamera = new CCamera();
	m_pGamePlayer = NULL;
	field_4D = 0;
	m_bRaceCheckpointsEnabled = FALSE;
	m_dwRaceCheckpointHandle = NULL;
	field_61 = 0;
	field_65 = 0;
	field_69 = FALSE;
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

int CGame::GetWeaponModelIDFromWeapon(int iWeaponID)
{
	return GameGetWeaponModelIDFromWeaponID(iWeaponID);
}

//-----------------------------------------------------------

BOOL CGame::IsKeyPressed(int iKeyIdentifier)
{
	GTA_CONTROLSET * pControlSet = GameGetInternalKeys();

	if(pControlSet->wKeys1[iKeyIdentifier]) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

float CGame::FindGroundZForCoord(float x, float y, float z)
{
	float fGroundZ;
	ScriptCommand(&get_ground_z, x, y, z, &fGroundZ);
	return fGroundZ;
}

//-----------------------------------------------------------

void DIReleaseMouse()
{
	pDirectInputMouse = *(IDirectInputDevice8**)0xC8CFA4;
	//SAFE_RELEASE(pDirectInputMouse);
	if(pDirectInputMouse) {
		pDirectInputMouse->Release();
		*(IDirectInputDevice8**)0xC8CFA4 = NULL;
	}
}

void DIInitMouse()
{
	pDirectInputMouse = *(IDirectInputDevice8**)0xC8CFA4;
	hWindowHandle = *(HWND*)0xC97C1C;

	if(!pDirectInputMouse || !hWindowHandle) return;

	pDirectInputMouse->Unacquire();
	pDirectInputMouse->SetCooperativeLevel(hWindowHandle,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	pDirectInputMouse->Acquire();
}

void DIResetMouse()
{
	*(DWORD*)0xB73424 = 0;
	*(DWORD*)0xB73428 = 0;

	_asm mov edx, 0x541BD0
	_asm call edx
}

void UpdatePads()
{
	_asm mov edx, 0x541DD0
	_asm call edx
}

void DisableMousePositionUpdate()
{
	//*(DWORD*)0xB7340C = 0;
	//*(DWORD*)0xB73410 = 0;
	//*(DWORD*)0xB73414 = 0;
	memset((PVOID)0xB7340C,0,12);

	UnFuck(0x53F47A,4);
	UnFuck(0x53F49A,4);
	UnFuck(0x53F4B3,4);
	*(DWORD*)0x53F47A = (DWORD)&dwDummyActiveMouseState;
	*(DWORD*)0x53F49A = (DWORD)&dwDummyActiveMouseState;
	*(DWORD*)0x53F4B3 = (DWORD)&dwDummyActiveMouseState;
}

void RestoreMousePositionUpdate()
{
	UnFuck(0x53F47A,4);
	UnFuck(0x53F49A,4);
	UnFuck(0x53F4B3,4);
	*(DWORD*)0x53F47A = 0xB73410;
	*(DWORD*)0x53F49A = 0xB73414;
	*(DWORD*)0x53F4B3 = 0xB7340C;
}

void DisableMouseProcess()
{
	UnFuck(0x53F417,5);
	memset((PVOID)0x53F417,0x90,5);

	UnFuck(0x53F41F,4);
	*(BYTE*)0x53F41F = 0x33;
	*(BYTE*)0x53F420 = 0xC0;
	*(BYTE*)0x53F421 = 0x0F;
	*(BYTE*)0x53F422 = 0x84;
}

//-----------------------------------------------------------

BYTE byteGetKeyStateFunc[] = { 0xE8,0x46,0xF3,0xFE,0xFF };
BYTE byteGetMouseStateCallEU10[] = { 0xE8,0x04,0x7B,0x20,0x00 };
BYTE byteGetMouseStateCallUSA10[] = { 0xE8,0xB4,0x7A,0x20,0x00 };

void CGame::ProcessInputDisabling()
{
	if(field_61 != 0) return;

	if(!field_65) {
		UnFuck(0x541DF5,5);
		memcpy((PVOID)0x541DF5,byteGetKeyStateFunc,5);

		if(iGtaVersion == GTASA_VERSION_USA10) {
			UnFuck(0x53F417,5);
			memcpy((PVOID)0x53F417,byteGetMouseStateCallUSA10,5);
		} else {
			UnFuck(0x53F417,5);
			memcpy((PVOID)0x53F417,byteGetMouseStateCallEU10,5);
		}

		RestoreMousePositionUpdate();

		UnFuck(0x53F421,4);
		*(BYTE*)0x53F41F = 0x85;
		*(BYTE*)0x53F420 = 0xC0;
		*(BYTE*)0x53F421 = 0x0F;
		*(BYTE*)0x53F422 = 0x8C;

		DIResetMouse();
		UpdatePads();
		DIResetMouse();

		UnFuck(0x6194A0,1);
		*(BYTE*)0x6194A0 = 0xE9;

		pD3DDevice->ShowCursor(FALSE);
		field_65--;
	} else {
		if(field_65 <= 0)
			return;
		field_65 -= 1;
	}
}

//-----------------------------------------------------------

void CGame::ToggleKeyInputsDisabled(int a2, BOOL a3)
{
	if(a2 == 2)
	{
		UnFuck(0x541DF5,5);
		memset((PVOID)0x541DF5,0x90,5);	// disable call

		DisableMouseProcess();
		DIResetMouse();
		UpdatePads();

		UnFuck(0x6194A0u,1);
		*(BYTE*)0x6194A0 = 0xC3;

		pD3DDevice->ShowCursor(TRUE);
		field_61 = 2;
	}
	else if(a2 == 1)
	{
		if(field_61 != 1)
		{
			UnFuck(0x541DF5,5);
			memset((PVOID)0x541DF5,0x90,5);	// disable call

			field_61 = 1;
		}
	}
	else if(a2 == 3)
	{
		if(field_61 != 3)
		{
			DisableMouseProcess();
			DIResetMouse();
			UpdatePads();

			UnFuck(0x6194A0,1);
			*(BYTE*)0x6194A0 = 0xC3;

			pD3DDevice->ShowCursor(TRUE);
			field_61 = 3;
		}
	}
	else if(a2 == 4)
	{
		if(field_61 != 4)
		{
			DisableMousePositionUpdate();
			DIResetMouse();
			UpdatePads();

			UnFuck(0x6194A0,1);
			*(BYTE*)0x6194A0 = 0xC3;

			field_61 = 4;
		}
	}
	else
	{
		if(!a2 && field_61)
		{
			field_65 = a3 != 0 ? 0 : 10;
			pD3DDevice->ShowCursor(FALSE);
			field_61 = 0;
		}
	}
}

//-----------------------------------------------------------

void CGame::InitGame()
{
	// Create a buffer for game text.
	szGameTextMessage = (unsigned char*)calloc(1,513);

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
	ApplyInGamePatches();

	// Install all hooks
	GameInstallHooks();

	// Setup scripting
	InitScripting();

	InitAnimNameHashes();

	*(PDWORD)ADDR_ENTRY = 8;
	*(PBYTE)ADDR_GAME_STARTED = 1;
	*(PBYTE)ADDR_MENU = 0;
	*(PBYTE)ADDR_STARTGAME = 0;
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
























void CGame::sub_100A1C10()
{
	// TODO: CGame::sub_100A1C10() .text:100A1C10
}




//-----------------------------------------------------------
// Return TRUE if the world has been loaded.

BOOL CGame::IsGameLoaded()
{
	if(!(*(PBYTE)ADDR_GAME_STARTED)) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------

void CGame::RequestModel(int iModelID, int iLoadingStream)
{
	/*
	_asm push iLoadingStream
	_asm push iModelID
	_asm mov edx, 0x4087E0
	_asm call edx
	_asm pop edx
	_asm pop edx*/

	ScriptCommand(&request_model,iModelID);
}

//-----------------------------------------------------------

void CGame::LoadRequestedModels()
{
	/*
	_asm push 0
	_asm mov edx, 0x40EA10
	_asm call edx
	_asm add esp, 4*/

	ScriptCommand(&load_requested_models);
}

//-----------------------------------------------------------

BOOL CGame::IsModelLoaded(int iModelID)
{
	if(iModelID > 20000 || iModelID < 0) return TRUE;

	return ScriptCommand(&is_model_available,iModelID);
}

//-----------------------------------------------------------

BOOL CGame::SetModelDeletable(int iModelID)
{
	BYTE * pStreamingModelInfo = (BYTE*)(iModelID * 20);

	if(IsModelLoaded(iModelID))
		return TRUE;

	_asm push 2
	_asm push iModelID
	_asm mov edx, 0x4087E0 ; CStreaming__RequestModel
	_asm call edx
	_asm pop edx
	_asm pop edx

	_asm push 0
	_asm mov edx, 0x40EA10 ; CStreaming__LoadAllRequestedModels
	_asm call edx
	_asm pop edx

	if(*(pStreamingModelInfo + 0x8E4CD0) == 1)
	{
		if(*(pStreamingModelInfo + 0x8E4CC6) & 2)
		{
			_asm push iModelID
			_asm mov edx, 0x409C10 ; CStreaming__SetModelIsDeletable
			_asm call edx
			_asm pop edx

			_asm push iModelID
			_asm mov edx, 0x409C70 ; CStreaming__SetModelTxdIsDeletable
			_asm call edx
			_asm pop edx
		}
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CGame::SetWorldTime(int iHour, int iMinute)
{
	*(PBYTE)0xB70152 = (BYTE)iMinute;
	*(PBYTE)0xB70153 = (BYTE)iHour;

	ScriptCommand(&set_current_time, iHour, iMinute);
}

//-----------------------------------------------------------

void CGame::GetWorldTime(BYTE* byteHour, BYTE* byteMinute)
{
	*byteMinute = *(PBYTE)0xB70152;
	*byteHour = *(PBYTE)0xB70153;
}

//-----------------------------------------------------------

void CGame::ToggleThePassingOfTime(BYTE byteOnOff)
{
	UnFuck(0x52CF10,1);

	if(byteOnOff) {
		*(PBYTE)0x52CF10 = 0x56; // push esi
		field_69 = TRUE;
	}
	else {
		*(PBYTE)0x52CF10 = 0xC3; // ret
		field_69 = FALSE;
	}
}

//-----------------------------------------------------------

void CGame::SetWorldWeather(int iWeatherID)
{
	if(field_69) {
		*(DWORD*)(0xC81318) = iWeatherID;
	} else {
		*(DWORD*)(0xC81318) = iWeatherID;
		*(DWORD*)(0xC8131C) = iWeatherID;
		*(DWORD*)(0xC81320) = iWeatherID;
	}
}

//-----------------------------------------------------------

int CGame::GetWorldWeather()
{
	return *(int*)0xC81318;
}

//-----------------------------------------------------------

BYTE CGame::IsHudEnabled()
{
	return *(BYTE*)ADDR_ENABLE_HUD;
}

//-----------------------------------------------------------

void CGame::SetFrameLimiterOn(BOOL bLimiter)
{

}

//-----------------------------------------------------------

BOOL CGame::IsFrameLimiterEnabled()
{
	if(*(PBYTE)0xBA6794) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------

void CGame::EnableFrameLimiter()
{
	*(BYTE*)0xBA6794 = 1;
}

//-----------------------------------------------------------

void CGame::SetFrameLimit(DWORD dwLimit)
{
	field_5D = dwLimit;

	UnFuck(0xC1704C,4);
	*(DWORD*)0xC1704C = 200;
}

//-----------------------------------------------------------

void CGame::SetMaxStats()
{
	// driving stat
	_asm mov eax, 0x4399D0
	_asm call eax

	// weapon stats
	_asm mov eax, 0x439940
	_asm call eax

	// Disable CStats::SetStatValue
	UnFuck(0x55A070,1);
	*(BYTE*)0x55A070 = 0xC3;
}

//-----------------------------------------------------------

void CGame::DisableTrainTraffic()
{
	ScriptCommand(&enable_train_traffic,0);
}

//-----------------------------------------------------------

void CGame::RefreshStreamingAt(float x, float y)
{
	ScriptCommand(&refresh_streaming_at,x,y);
}

//-----------------------------------------------------------

void CGame::RequestAnimation(char *szAnimFile)
{
	ScriptCommand(&request_animation, szAnimFile);
}

//-----------------------------------------------------------

int CGame::IsAnimationLoaded(char *szAnimFile)
{
	return ScriptCommand(&is_animation_loaded,szAnimFile);
}

//-----------------------------------------------------------

void CGame::ReleaseAnimation(char *szAnimFile)
{
	if (IsAnimationLoaded(szAnimFile))
		ScriptCommand(&release_animation,szAnimFile);
}

//-----------------------------------------------------------

void CGame::ToggleRadar(int iToggle)
{
	*(PBYTE)0xBAA3FB = (BYTE)!iToggle;
}

//-----------------------------------------------------------
// MATCH
void CGame::DisplayGameText(char *szStr,int iTime,int iSize)
{
	if(iSize > 200) return;

	ScriptCommand(&text_clear_all);

	memset(szGameTextMessage,0,sizeof(szGameTextMessage)); // not a typo

	strncpy((char*)szGameTextMessage,szStr,512);

	unsigned char *str = szGameTextMessage;
	while(*str != 0) {
		if(*str >= 1 && *str < ' ' && *str != '\n')
			*str = ' ';
		str++;
	}

	_asm push iSize
	_asm push iTime
	_asm push szGameTextMessage
	_asm mov eax, 0x69F2B0
	_asm call eax
	_asm add esp, 12
}

//-----------------------------------------------------------

void CGame::DisableRaceCheckpoint()
{
	if (m_dwRaceCheckpointHandle)
	{
		ScriptCommand(&destroy_racing_checkpoint, m_dwRaceCheckpointHandle);
		m_dwRaceCheckpointHandle = NULL;
	}
	m_bRaceCheckpointsEnabled = false;
}

//-----------------------------------------------------------

DWORD CGame::CreateRadarMarkerIcon(int iMarkerType, float fX, float fY, float fZ, DWORD dwColor, int iStyle)
{
	DWORD dwMarkerID=0;

	if(iStyle == MAPICON_LOCAL) {
		ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);
	} else if(iStyle == MAPICON_GLOBAL) {
		ScriptCommand(&create_marker_at, fX, fY, fZ, iMarkerType, &dwMarkerID);
	} else if(iStyle == MAPICON_LOCAL_CHECKPOINT) {
		ScriptCommand(&create_radar_marker_icon, fX, fY, fZ, iMarkerType, &dwMarkerID);
	} else if(iStyle == MAPICON_GLOBAL_CHECKPOINT) {
		ScriptCommand(&create_icon_marker_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);
	} else {
		ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);
	}

	if(iMarkerType == 0) {
		if(dwColor < 1004) {
			ScriptCommand(&set_marker_color, dwMarkerID, dwColor);
			ScriptCommand(&show_on_radar, dwMarkerID, 2);
		} else {
			ScriptCommand(&set_marker_color, dwMarkerID, dwColor);
			ScriptCommand(&show_on_radar, dwMarkerID, 3);
		}
	}
	return dwMarkerID;
}

//-----------------------------------------------------------

void CGame::DisableMarker(DWORD dwMarkerID)
{
	ScriptCommand(&disable_marker, dwMarkerID);
}

//-----------------------------------------------------------
// Get the current active interior

BYTE CGame::GetActiveInterior()
{
	DWORD dwRet;
	ScriptCommand(&get_active_interior,&dwRet);
	return (BYTE)dwRet;
}

//-----------------------------------------------------------

extern float fFarClip;

void CGame::UpdateFarClippingPlane()
{
	PED_TYPE *pPlayerPed = GamePool_FindPlayerPed();

	if(pPlayerPed) {
		if(GetActiveInterior() == 0) {
			fFarClip = 1250.0f - (pPlayerPed->entity.mat->pos.Z * 2.0f);
			if(fFarClip < 700.0f) {
				fFarClip = 700.0f;
			}
		}
		else {
			fFarClip = 400.0f;
		}
	}
	else {
		fFarClip = 1250.0f;
	}
}

//-----------------------------------------------------------

void CGame::AddToLocalMoney(int iAmount)
{
	ScriptCommand(&add_to_player_money,0,iAmount);
}

//-----------------------------------------------------------

int CGame::GetLocalMoney()
{
	return *(int *)0xB7CE50;
}

//-----------------------------------------------------------

const PCHAR CGame::GetWeaponName(int iWeaponID)
{
	switch(iWeaponID) {
      case WEAPON_BRASSKNUCKLE:
         return "Brass Knuckles";
      case WEAPON_GOLFCLUB:
         return "Golf Club";
      case WEAPON_NITESTICK:
         return "Nite Stick";
      case WEAPON_KNIFE:
         return "Knife";
      case WEAPON_BAT:
         return "Baseball Bat";
      case WEAPON_SHOVEL:
         return "Shovel";
      case WEAPON_POOLSTICK:
         return "Pool Cue";
      case WEAPON_KATANA:
         return "Katana";
      case WEAPON_CHAINSAW:
         return "Chainsaw";
      case WEAPON_DILDO:
         return "Dildo";
      case WEAPON_DILDO2:
         return "Dildo";
      case WEAPON_VIBRATOR:
         return "Vibrator";
      case WEAPON_VIBRATOR2:
         return "Vibrator";
      case WEAPON_FLOWER:
         return "Flowers";
      case WEAPON_CANE:
         return "Cane";
      case WEAPON_GRENADE:
         return "Grenade";
      case WEAPON_TEARGAS:
         return "Teargas";
      case WEAPON_MOLTOV:
         return "Molotov";
      case WEAPON_COLT45:
         return "Colt 45";
      case WEAPON_SILENCED:
         return "Silenced Pistol";
      case WEAPON_DEAGLE:
         return "Desert Eagle";
      case WEAPON_SHOTGUN:
         return "Shotgun";
      case WEAPON_SAWEDOFF:
         return "Sawn-off Shotgun";
      case WEAPON_SHOTGSPA: // wtf?
         return "Combat Shotgun";
      case WEAPON_UZI:
         return "UZI";
      case WEAPON_MP5:
         return "MP5";
      case WEAPON_AK47:
         return "AK47";
      case WEAPON_M4:
         return "M4";
      case WEAPON_TEC9:
         return "TEC9";
      case WEAPON_RIFLE:
         return "Rifle";
      case WEAPON_SNIPER:
         return "Sniper Rifle";
      case WEAPON_ROCKETLAUNCHER:
         return "Rocket Launcher";
      case WEAPON_HEATSEEKER:
         return "Heat Seaker";
      case WEAPON_FLAMETHROWER:
         return "Flamethrower";
      case WEAPON_MINIGUN:
         return "Minigun";
      case WEAPON_SATCHEL:
         return "Satchel Explosives";
      case WEAPON_BOMB:
         return "Bomb";
      case WEAPON_SPRAYCAN:
         return "Spray Can";
      case WEAPON_FIREEXTINGUISHER:
         return "Fire Extinguisher";
      case WEAPON_CAMERA:
         return "Camera";
      case WEAPON_PARACHUTE:
         return "Parachute";
      case WEAPON_VEHICLE:
         return "Vehicle";
      case WEAPON_DROWN:
         return "Drowned";
      case WEAPON_COLLISION:
         return "Splat";
	}

	return "";
}

//-----------------------------------------------------------

DWORD CGame::CreateWeaponPickup(int iModel, DWORD dwAmmo, float fX, float fY, float fZ)
{
	DWORD hnd;

	if(!IsModelLoaded(iModel)) {
		RequestModel(iModel);
		LoadRequestedModels();
		while(!IsModelLoaded(iModel)) Sleep(5);
	}

	ScriptCommand(&create_pickup_with_ammo, iModel, 4, dwAmmo, fX, fY, fZ, &hnd);
	return hnd;
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

//---------------------------------------------------

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

void CGame::EnableZoneNames(BYTE byteEnable)
{
	ScriptCommand(&enable_zone_names, byteEnable);
}

//-----------------------------------------------------------

void CGame::EnableStuntBonus(bool bEnable)
{
	UnFuck(0xA4A474,4);
	*(DWORD*)0xA4A474 = (int)bEnable;
}

//-----------------------------------------------------------

void CGame::DisableEnterExits()
{
	DWORD pEnExPool = *(DWORD *)0x96A7D8;
	DWORD pEnExEntries = *(DWORD *)pEnExPool;

	int iNumEnEx=0;
	int x=0;

	_asm mov ecx, pEnExPool
	_asm mov eax, [ecx+8]
	_asm mov iNumEnEx, eax

	BYTE *pEnExPoolSlot;
	while(x!=iNumEnEx) {
		pEnExPoolSlot = (((BYTE *)pEnExEntries) + (60*x));
		_asm mov eax, pEnExPoolSlot
		_asm and word ptr [eax+48], 0
		x++;
	}   
}

//-----------------------------------------------------------

void CGame::LoadScene(char* szScene)
{
	_asm push szScene
	_asm mov ebx, 0x5B8700
	_asm call ebx
	_asm pop ebx
}

//-----------------------------------------------------------

int CGame::GetMemoryUsedForStreaming()
{
	return *(int*)0x8E4CB4;
}

//-----------------------------------------------------------

int CGame::GetMemoryAvailableForStreaming()
{
	return *(int*)0x8A5A80;
}

//-----------------------------------------------------------

int CGame::GetLoadedVehicleModelCount()
{
	int iCount = 0;

	struct DUMMY {
		PADDING(_pad0,16); // 0-16
		BOOL bLoaded; // 16-20
	};

	DUMMY *pStreamingModelInfo = (DUMMY*)0x8E4CC0;
	int x = 400;
	while(x != 611) {
		if(pStreamingModelInfo[x].bLoaded)
			iCount++;
		x++;
	}
	return iCount;
}

//-----------------------------------------------------------

void CGame::SetTimeInMilliseconds(DWORD dwTimeInMs)
{
	if(!field_69) {
		*(DWORD*)0xB7CB84 = dwTimeInMs & 0x3FFFFFFF;
		GLOBAL_101506A4 = true;
	}
}

//-----------------------------------------------------------

DWORD CGame::GetTimeInMilliseconds()
{
	return *(DWORD*)0xB7CB84;
}

//-----------------------------------------------------------

int CGame::GetRwObjectsCount()
{
	return *(int*)0xB71804;
}

//-----------------------------------------------------------

void CGame::RequestAnimationsAndModels()
{
	if(!IsAnimationLoaded("PARACHUTE"))
		RequestAnimation( "PARACHUTE");
	if(!IsAnimationLoaded("BAR"))
		RequestAnimation( "BAR");
	if(!IsAnimationLoaded("GANGS"))
		RequestAnimation( "GANGS");
	if(!IsAnimationLoaded("PAULNMAC"))
		RequestAnimation( "PAULNMAC");
	if(!IsAnimationLoaded("VENDING"))
		RequestAnimation( "VENDING");
	if(!IsAnimationLoaded("FIGHT_B"))
		RequestAnimation( "FIGHT_B");
	if(!IsAnimationLoaded("FIGHT_C"))
		RequestAnimation( "FIGHT_C");
	if(!IsAnimationLoaded("FIGHT_D"))
		RequestAnimation( "FIGHT_D");
	if(!IsAnimationLoaded("FIGHT_E"))
		RequestAnimation( "FIGHT_E");

	if (!IsModelLoaded(OBJECT_PARACHUTE))
		RequestModel(OBJECT_PARACHUTE);
	if (!IsModelLoaded(1485)) // cigar
		RequestModel(1485);
	if (!IsModelLoaded(1486)) // beer bottle
		RequestModel(1486);
	if (!IsModelLoaded(1543)) // beer bottle
		RequestModel(1543);
	if (!IsModelLoaded(1546)) // sprunk cup
		RequestModel(1546);
	if (!IsModelLoaded(WEAPON_MODEL_TEARGAS))
		RequestModel(WEAPON_MODEL_TEARGAS);
	if (!IsModelLoaded(WEAPON_MODEL_GRENADE))
		RequestModel(WEAPON_MODEL_GRENADE);
	if (!IsModelLoaded(WEAPON_MODEL_MOLTOV))
		RequestModel(WEAPON_MODEL_MOLTOV);
	if (!IsModelLoaded(330)) // cellphone
		RequestModel(330);
}

//-----------------------------------------------------------

void CGame::LoadCollisionFile(char *szFileName)
{
	_asm push 0
	_asm push szFileName
	_asm mov edx, 0x5B4E60
	_asm call edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

void CGame::LoadCullZone(char *szLine)
{
	_asm push szLine
	_asm mov edx, 0x5B4B40
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

BOOL CGame::IsUsingController()
{
	if(*(BYTE*)0xBA6818) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------

void CGame::DisableWeaponLockOnTarget()
{
	// CPlayerPed_FindWeaponLockOnTarget
	UnFuck(0x60DC50,1);
	*(BYTE*)0x60DC50 = 0xC3; // ret

	// CPlayerPed_FindNextWeaponLockOnTarget
	UnFuck(0x60E530,3);
	*(BYTE*)0x60E530 = 0xC2; // retn 8
	*(BYTE*)0x60E531 = 0x08;
	*(BYTE*)0x60E532 = 0x00;
}

//-----------------------------------------------------------

