
#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

extern int iGtaVersion;
extern CNetGame* pNetGame;
extern CGame* pGame;
extern CChatWindow *pChatWindow;
extern CObjectPool *unnamed_1014FFAC; // 0x1014FFAC, lives in another translation unit

extern DWORD dwGraphicsLoop; // Used for the external dll game loop.

int WINAPI exc_filter(unsigned int code, struct _EXCEPTION_POINTERS *ep, char *what);

#define NUDE void _declspec(naked) 

//-----------------------------------------------------------
// Globals which are used to avoid stack frame alteration
// inside the following hook procedures.

DWORD	dwFarClipHookAddr=0;
DWORD	dwFarClipReturnAddr=0;

// used generically
PED_TYPE	*_pPlayer;

BOOL	bIgnoreNextEntry=FALSE;
BOOL	bIgnoreNextExit=FALSE;

BYTE	byteSavedCameraMode;
DWORD	dwCurPlayerActor=0;
BYTE	*pbyteCameraMode = (BYTE *)0xB6F1A8;
BYTE	*pbyteCurrentPlayer = (BYTE *)0xB7CD74;
WORD	*wCameraMode2 = (WORD *)0xB6F858;

PED_TYPE pedCrimeReportTemp; // pay attention! used in 0x100A1790 ; void __thiscall CGame::PlayCrimeReport as pedCrimeReportTemp 0x10150D00

int		iRadarColor1=0;
DWORD	dwSavedCheatFn=0;

BOOL	bAllowVehicleCreation=FALSE;

float fFarClip=1400.0f;

DWORD dwParam1;
DWORD dwParam2;
DWORD dwParamThis;

DWORD unnamed_10113AD8 = 1;
DWORD unnamed_10113ADC = 0x7FB270; // RwRasterCreate
WORD  unnamed_10113B2C;
BYTE unnamed_10113AE8[3] = {0xFF,0x52,0x20}; // original `call dword ptr [edx+20h]` at 0x4096AA

PCHAR unnamed_10150734;
DWORD unnamed_10150960;
PCHAR unnamed_10151628;	// gxt key, also reused as the ped a use-gun task runs on
DWORD unnamed_101516C8;
DWORD unnamed_101516D0;
DWORD unnamed_101516D4;
DWORD unnamed_101516D8; // vehicle being rendered
DWORD unnamed_10151710;
DWORD unnamed_1015172C;
DWORD unnamed_10151804;
bool  unnamed_10151808;
DWORD unnamed_1015180C;
DWORD unnamed_10151810;
DWORD unnamed_10151814;
DWORD unnamed_10151818;
DWORD unnamed_10151824;

CObjectPool *unnamed_1015177C;

WORD wLastRendObj=0;

BYTE Unk1_JmpCode[] = {0xFF,0x19,0xD1,0xBE,0x35,0x00};
BYTE TaskEnterVehicleDriver_HookJmpCode[] = {0xFF,0x25,0xBB,0x19,0x69,0x00,0x90};
BYTE TaskExitVehicle_HookJmpCode[] = {0xFF,0x25,0xBA,0xB8,0x63,0x00,0x90};
BYTE RadarTranslateColor_HookJmpCode[] = {0xFF,0x25,0x79,0x4A,0x58,0x00,0x90};
BYTE CheatProcessHook_JmpCode[] = {0xFF,0x25,0xAA,0x85,0x43,0x00,0x90};
BYTE Unk2_JmpCode[] = {0xFF,0x25,0x33,0x14,0x42,0x00};
BYTE Unk3_JmpCode[] = {0xFF,0x25,0x61,0x36,0x53,0x00,0x90,0x90,0x90};
BYTE CGameShutdown_HookJmpCode[] = {0xFF,0x25,0xF1,0xC8,0x53,0x00,0x90};
BYTE PedDamage_HookJmpCode[] = {0xFF,0x25,0xBC,0x5A,0x4B,0x00};
BYTE Unk4_JmpCode[] = {0xFF,0x25,0x74,0x22,0x50,0x00,0x90,0x90,0x90,0x90};
BYTE Unk5_JmpCode[] = {0xFF,0x25,0x61,0x38,0x4C,0x00};
BYTE GetText_HookJmpCode[] = {0xFF,0x25,0x43,0x00,0x6A,0x00,0x90,0x90,0x90};
BYTE Unk6_JmpCode[] = {0xFF,0x25,0xD8,0xFF,0x5E,0x00,0x90};
BYTE CProjectileInfo_Update_HookJmpCode[] = {0xFF,0x25,0x1B,0x8B,0x73,0x00};
BYTE CWeapon__Satchel__Activate_HookJmpCode[] = {0xFF,0x25,0x5B,0x88,0x73,0x00};
BYTE Unk7_JmpCode[] = {0xFF,0x25,0x36,0xA0,0x63,0x00,0x90};
BYTE Unk8_JmpCode[] = {0xFF,0x25,0x77,0xAB,0x5E,0x00,0x90};
BYTE Unk9_JmpCode[] = {0xFF,0x25,0x39,0x88,0x4C,0x00,0x90,0x90};
BYTE CAnimManager__AddAnimation_Hook_HookJmpCode[] = {0xFF,0x25,0x34,0x39,0x4D,0x00,0x90,0x90,0x90,0x90};
BYTE CAnimManager__BlendAnimation_HookJmpCode[] = {0xFF,0x25,0x09,0x46,0x4D,0x00,0x90};
BYTE Unk10_JmpCode[] = {0xFF,0x25,0xE5,0x42,0x4D,0x00,0x90};
BYTE Unk11_JmpCode[] = {0xFF,0x25,0x29,0x3B,0x4D,0x00};
BYTE RwRasterDestroy_HookJmpCode[] = {0xFF,0x25,0x21,0xC7,0x59,0x00};
BYTE CFileLoader__LoadObjectInstance_HookJmpCode[] = {0xFF,0x25,0x84,0x80,0x53,0x00,0x90};
BYTE CRenderer__AddEntityToRenderList_HookJmpCode[] = {0xFF,0x25,0xA6,0x34,0x55,0x00,0x90,0x90,0x90};
BYTE CEventDamage__AffectsPed_HookJmpCode[] = {0xFF,0x25,0x33,0x34,0x4B,0x00};
BYTE Unk12_JmpCode[] = {0xFF,0x25,0xC6,0x84,0x6D,0x00,0x90,0x90};
BYTE CStreaming__RequestModel_HookJmpCode[] = {0xFF,0x25,0xD7,0x87,0x40,0x00};
BYTE CWorld__ProcessVerticalLine_HookJmpCode[] = {0xFF,0x25,0xDB,0x74,0x56,0x00};

//-----------------------------------------------------------


// TODO: implement functions

// radar scale/shape fix


bool bHudScaleFix = true;
DWORD dwHudScaleX = 0;
DWORD dwHudScaleY = 0;
float fMaximumWidth;
float fMaximumHeight;
bool bSomeHudScaleFixFlag = false;
float* CRadar__radarHeight = (float*)0x866B74;
float* CRadar__radarWidth = (float*)0x866B78;

void ProcessHudScaleFix()
{
	if ( bHudScaleFix )
	{
		dwHudScaleX = *(DWORD*)0x859520;
		dwHudScaleY = *(DWORD*)0x859524;

		fMaximumWidth = (float)pGame->GetScreenWidth();
		fMaximumHeight = (float)pGame->GetScreenHeight();

		if ( pGame->GetScreenWidth() > 0 && pGame->GetScreenHeight() > 0 )
		{
			if ( fMaximumWidth / fMaximumHeight >= 1.6f )
			{
				*CRadar__radarHeight = 82.0f;
				*CRadar__radarWidth = 96.0f;
				*(float*)0x859524 = 0.00242f;
				*(float*)0x859520 = 0.00222f / (fMaximumWidth / fMaximumHeight);
				bSomeHudScaleFixFlag = true;
			}
			else
			{
				*CRadar__radarHeight = 76.0f; 
				*CRadar__radarWidth = 94.0f; 
			}
		}
	}
}

inline void FUNC_100A24C0()
{
	if(bSomeHudScaleFixFlag == true)
	{
		*(DWORD*)0x859520 = dwHudScaleX;
		*(DWORD*)0x859524 = dwHudScaleY;
		*CRadar__radarHeight = 76.0f;
		*CRadar__radarWidth = 94.0f;
		bSomeHudScaleFixFlag = false;
	}
}

NUDE CHud__DrawRadar_Hook() 
{
	__asm pushad 
	ProcessHudScaleFix();

	__asm 
	{
	    popad

		// call original CHud::DrawRadar
		mov edx, 0x58A330
		call edx

		pushad
	}

	FUNC_100A24C0();

	__asm 
	{
		popad
		retn
	}
}

bool bSomeFlag = false;
NUDE CHud__DrawCrossHairs_Hook() 
{
	__asm pushad

	bSomeFlag = false;
	if ( pGame && pGame->FindPlayerPed() )
	{
		if ( pGame->FindPlayerPed()->GetCameraMode() == 53 )
		{
			ProcessHudScaleFix();
			bSomeFlag = true;
		}
	}

	__asm
	{
		popad

		// call original CHud::DrawCrossHairs
		mov edx, 0x58E020
		call edx

		pushad
	}

	if (bSomeFlag)
	{
		FUNC_100A24C0();
		bSomeFlag = false;
	}

	__asm
	{
		popad
		retn
	}
}

//-----------------------------------------------------------

NUDE CCamera__Process_Hook()
{
	_asm pushad

	if(pGame) {
		pGame->GetCamera()->FUNC_1009D6B0();
	}

	_asm popad
	_asm mov eax, 0x52B730
	_asm jmp eax
}

//-----------------------------------------------------------

void CGame__Process_Hook()
{
	if(pGame) pGame->sub_100A1C10();

	// every pool update gets its own seh block, tagged so exc_filter can name it
	if(pNetGame && pNetGame->GetObjectPool())
	{
		__try
		{
			pNetGame->GetObjectPool()->FUNC_10012A50();
		}
		__except(exc_filter(GetExceptionCode(), GetExceptionInformation(), "s003")) { }
	}

	if(unnamed_1014FFAC)
	{
		__try
		{
			unnamed_1014FFAC->FUNC_10012A50();
		}
		__except(exc_filter(GetExceptionCode(), GetExceptionInformation(), "s003d")) { }
	}

	if(pNetGame && pNetGame->GetVehiclePool()) pNetGame->GetVehiclePool()->FUNC_1001ED70();
	if(pNetGame && pNetGame->GetObjectPool()) pNetGame->GetObjectPool()->FUNC_10012B10();
	if(pChatWindow) pChatWindow->FUNC_10067ED0();

	__try
	{
		_asm mov edx, 0x53BEE0
		_asm call edx
	}
	__except(exc_filter(GetExceptionCode(), GetExceptionInformation(), "s007")) { }

	if(pNetGame && pNetGame->GetTextDrawPool()) pNetGame->GetTextDrawPool()->FUNC_1001E870();
}

//-----------------------------------------------------------

void FUNC_100C3D30(BOOL bFogEnable); // todo: implement `FUNC_100C3D30`, lives in another file
extern DWORD GLOBAL_1026EBDC; // todo: define, lives in another file

NUDE CPed_Render_Hook()
{
	__asm pushad

	FUNC_100C3D30(FALSE);

	__asm
	{
		popad

		// call original CPed::Render
		mov edx, 0x5E7680
		call edx

		pushad
	}

	if (GLOBAL_1026EBDC) FUNC_100C3D30(TRUE);
	else FUNC_100C3D30(FALSE);

	__asm
	{
		popad
		retn
	}
}

//-----------------------------------------------------------

// saved return address, shared by the task and anim hooks below
DWORD dwHookRetnAddr;

// animation the local player got told to play, picked up for sync
DWORD dwAnimClump;
DWORD dwAnimGroup;
DWORD dwAnimID;
DWORD dwAnimBlend;
DWORD dwLocalPlayerClump;

DWORD dwSyncAnimType;
DWORD dwSyncAnimGroup;
DWORD dwSyncAnimID;
DWORD dwSyncAnimBlend;

DWORD dwAddAnimationRetAddr = 0x4D3AAA;
DWORD dwBlendAnimationRetAddr = 0x4D4617;

NUDE CAnimManager__AddAnimation_Hook()
{
	_asm mov edx, [esp]
	_asm mov dwHookRetnAddr, edx
	_asm mov edx, [esp+4]
	_asm mov dwAnimClump, edx
	_asm mov edx, [esp+8]
	_asm mov dwAnimGroup, edx
	_asm mov edx, [esp+12]
	_asm mov dwAnimID, edx
	_asm pushad

	_pPlayer = GamePool_FindPlayerPed();
	if(_pPlayer) dwLocalPlayerClump = (DWORD)_pPlayer->entity.pdwRenderWare;
	else dwLocalPlayerClump = 0;

	if(dwAnimClump == dwLocalPlayerClump && (dwAnimGroup || dwAnimID != 0xA0))
	{
		dwSyncAnimType = 1;
		dwSyncAnimGroup = dwAnimGroup;
		dwSyncAnimID = dwAnimID;
	}

	_asm popad
	_asm mov eax, [esp+12]
	_asm mov edx, ds:0xB4EA34
	_asm jmp dwAddAnimationRetAddr
}

//-----------------------------------------------------------

NUDE CAnimManager__BlendAnimation_Hook()
{
	_asm mov edx, [esp]
	_asm mov dwHookRetnAddr, edx
	_asm mov edx, [esp+4]
	_asm mov dwAnimClump, edx
	_asm mov edx, [esp+8]
	_asm mov dwAnimGroup, edx
	_asm mov edx, [esp+12]
	_asm mov dwAnimID, edx
	_asm mov edx, [esp+16]
	_asm mov dwAnimBlend, edx
	_asm pushad

	_pPlayer = GamePool_FindPlayerPed();
	if(_pPlayer) dwLocalPlayerClump = (DWORD)_pPlayer->entity.pdwRenderWare;
	else dwLocalPlayerClump = 0;

	if(dwAnimClump == dwLocalPlayerClump && (dwAnimGroup || dwAnimID != 0xA0))
	{
		dwSyncAnimType = 2;
		dwSyncAnimGroup = dwAnimGroup;
		dwSyncAnimID = dwAnimID;
		dwSyncAnimBlend = dwAnimBlend;
	}

	_asm popad
	_asm sub esp, 0x14
	_asm mov ecx, [esp+0x18]
	_asm jmp dwBlendAnimationRetAddr
}

//-----------------------------------------------------------

DWORD dwProcessControlPed;
PED_TYPE *_pProcessControlPed;
BYTE byteSavedCurrentPlayer;
BYTE byteProcessControlPlayerID;
WORD wSavedCameraMode2;
DWORD dwSavedPedHealth;
GTA_CONTROLSET *pProcessControlKeys;

// `fst [esi+0x55C]` heading writes at 0x6884C4 and 0x688200, only enabled for the local player
BYTE PedHeadingCode1[] = {0xD9,0x96,0x5C,0x05,0x00,0x00};
BYTE PedHeadingCode2[] = {0xD9,0x96,0x5C,0x05,0x00,0x00};
// `call 0x5E1B10` at 0x5E92F4, nopped out while a remote ped processes
BYTE RemotePedProcessCode[] = {0xE8,0x17,0x88,0xFF,0xFF};

CPlayerPool *pRemotePedPlayerPool;
PLAYERID remotePedPlayerId;
CRemotePlayer *pRemotePedPlayer;

// runs the original CPlayerPed::ProcessControl for a remote ped under SEH; a fault
// there kicks the player rather than taking the whole client down
void ProcessControlRemotePed()
{
	__try
	{
		_asm mov ecx, dwProcessControlPed
		_asm mov edx, 0x60EA90
		_asm call edx
	}
	__except(exc_filter(GetExceptionCode(), GetExceptionInformation(), "playerped"))
	{
		if(*pbyteCurrentPlayer)
		{
			if(pNetGame)
			{
				pRemotePedPlayerPool = pNetGame->GetPlayerPool();

				if(pRemotePedPlayerPool)
				{
					remotePedPlayerId = pRemotePedPlayerPool->FUNC_100138C0(dwProcessControlPed);

					if(remotePedPlayerId != 0xFFFF)
					{
						pRemotePedPlayer = pRemotePedPlayerPool->GetAt(remotePedPlayerId);

						if(pRemotePedPlayer) pRemotePedPlayer->FUNC_10017530();

						if(pChatWindow)
							pChatWindow->AddDebugMessage("Removed player %u due to error.", remotePedPlayerId);
					}
				}
			}
		}
	}
}

// .text:100A28C0 : nop the two ped heading writes back out
void FUNC_100A28C0()
{
	memset((void *)0x6884C4, 0x90, 6);
	memset((void *)0x688200, 0x90, 6);
}

// .text:100A28E0 : put the two ped heading writes back
void FUNC_100A28E0()
{
	memcpy((void *)0x6884C4, PedHeadingCode1, 6);
	memcpy((void *)0x688200, PedHeadingCode2, 6);
}

// .text:100A2920 : nop the remote ped process call out
void FUNC_100A2920()
{
	UnFuck(0x5E92F4, 5);
	memset((void *)0x5E92F4, 0x90, 5);
}

// .text:100A2940 : put the remote ped process call back
void FUNC_100A2940()
{
	memcpy((void *)0x5E92F4, RemotePedProcessCode, 5);
}

NUDE CPlayerPed_ProcessControl_Hook()
{
	_asm mov dwProcessControlPed, ecx
	_asm pushad

	_pProcessControlPed = (PED_TYPE *)dwProcessControlPed;
	byteSavedCurrentPlayer = *pbyteCurrentPlayer;
	byteProcessControlPlayerID = FindPlayerNumFromPedPtr(dwProcessControlPed);

	if(dwProcessControlPed && byteProcessControlPlayerID && !byteSavedCurrentPlayer)
	{
		GameStoreLocalPlayerKeys();
		GameSetRemotePlayerKeys(byteProcessControlPlayerID);

		byteSavedCameraMode = *pbyteCameraMode;
		*pbyteCameraMode = GameGetPlayerCameraMode(byteProcessControlPlayerID);
		wSavedCameraMode2 = *wCameraMode2;
		*wCameraMode2 = GameGetPlayerCameraMode(byteProcessControlPlayerID);
		if(*wCameraMode2 == 4) *wCameraMode2 = 0;

		GameStoreLocalPlayerCameraExtZoom();
		GameSetRemotePlayerCameraExtZoom(byteProcessControlPlayerID);
		GameStoreLocalPlayerAim();
		GameSetRemotePlayerAim(byteProcessControlPlayerID);
		GameStoreLocalPlayerWeaponSkills();
		GameSetRemotePlayerWeaponSkills(byteProcessControlPlayerID);

		*pbyteCurrentPlayer = byteProcessControlPlayerID;
		dwSavedPedHealth = *(DWORD *)&_pProcessControlPed->fHealth;

		UnFuck(0x5E92F4, 5);
		memset((void *)0x5E92F4, 0x90, 5);
		ProcessControlRemotePed();
		memcpy((void *)0x5E92F4, RemotePedProcessCode, 5);

		GameSetLocalPlayerWeaponSkills();
		*pbyteCameraMode = byteSavedCameraMode;
		*wCameraMode2 = wSavedCameraMode2;
		GameSetLocalPlayerCameraExtZoom();
		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerKeys();
		GameSetLocalPlayerAim();
	}
	else
	{
		if(pNetGame && pNetGame->GetPlayerPool()->GetLocalPlayer()->GetField_F0())
		{
			struc_41 *pSettings = pNetGame->GetSettings();

			if(pGame->FindPlayerPed()->HasExceededWorldBoundries(pSettings->fWorldBoundryPX,
				pSettings->fWorldBoundryZX, pSettings->fWorldBoundryPY, pSettings->fWorldBoundryNY))
			{
				if(!pGame->GetActiveInterior())
				{
					DWORD dwStateFlags = _pProcessControlPed->dwStateFlags;

					if(!(dwStateFlags & 0x100) && (dwStateFlags & 3))
					{
						pProcessControlKeys = GameGetInternalKeys();
						pProcessControlKeys->wKeys1[14] = 255;
						pProcessControlKeys->wKeys2[14] = 0;
					}
				}
			}
		}

		if(pGame->FindPlayerPed())
		{
			if(pGame->FindPlayerPed()->field_2F7)
			{
				pProcessControlKeys = GameGetInternalKeys();
				pProcessControlKeys->wKeys1[18] = 0;
				pProcessControlKeys->wKeys2[18] = 0;
			}

			if(pGame->FindPlayerPed()->field_2F6)
			{
				pProcessControlKeys = GameGetInternalKeys();
				pProcessControlKeys->wKeys1[14] = 0;
				pProcessControlKeys->wKeys2[14] = 0;
			}

			if(pGame->FindPlayerPed()->field_2F8)
			{
				pProcessControlKeys = GameGetInternalKeys();
				pProcessControlKeys->wKeys1[16] = 0;
				pProcessControlKeys->wKeys2[16] = 0;
			}
		}

		memcpy((void *)0x6884C4, PedHeadingCode1, 6);
		memcpy((void *)0x688200, PedHeadingCode2, 6);

		_asm mov ecx, dwProcessControlPed
		_asm mov edx, 0x60EA90
		_asm call edx

		memset((void *)0x6884C4, 0x90, 6);
		memset((void *)0x688200, 0x90, 6);
	}

	_asm popad
	_asm retn
}

//-----------------------------------------------------------

MATRIX4X4 unnamed_10150A48;	// ped matrix saved across ProcessControl
float unnamed_10150C1C;
float unnamed_10150C20;
float unnamed_10150C24;

NUDE CCivillianPed__ProcessControl_Hook()
{
	_asm mov dwProcessControlPed, ecx
	_asm pushad

	_pProcessControlPed = (PED_TYPE *)dwProcessControlPed;

	// hold attached/frozen peds still while the game processes them
	if((_pProcessControlPed->entity.dwPhysFlags & 0x40000) &&
		(_pProcessControlPed->entity.dwPhysFlags & 0x800000) &&
		(_pProcessControlPed->entity.dwPhysFlags & 0x200000))
	{
		_pProcessControlPed->entity.vecMoveSpeed.X = 0.0f;
		_pProcessControlPed->entity.vecMoveSpeed.Y = 0.0f;
	}

	if(_pProcessControlPed->entity.mat)
	{
		unnamed_10150A48 = *_pProcessControlPed->entity.mat;
		unnamed_10150C1C = _pProcessControlPed->entity.vecMoveSpeed.X;
		unnamed_10150C20 = _pProcessControlPed->entity.vecMoveSpeed.Y;
		unnamed_10150C24 = _pProcessControlPed->entity.vecMoveSpeed.Z;
	}

	_asm mov ecx, dwProcessControlPed
	_asm mov edx, 0x5DDBF0	;// CCivilianPed::ProcessControl
	_asm call edx

	if((_pProcessControlPed->entity.dwPhysFlags & 0x40000) &&
		(_pProcessControlPed->entity.dwPhysFlags & 0x800000) &&
		(_pProcessControlPed->entity.dwPhysFlags & 0x200000))
	{
		if(_pProcessControlPed->entity.mat)
		{
			_pProcessControlPed->entity.mat->pos.X = unnamed_10150A48.pos.X;
			_pProcessControlPed->entity.mat->pos.Y = unnamed_10150A48.pos.Y;
			_pProcessControlPed->entity.mat->pos.Z = unnamed_10150A48.pos.Z;
		}

		_pProcessControlPed->entity.vecMoveSpeed.X = 0.0f;
		_pProcessControlPed->entity.vecMoveSpeed.Y = 0.0f;
	}

	_asm popad
	_asm retn
}

//-----------------------------------------------------------

WORD unnamed_10150730;
BYTE unnamed_10150BC0;
DWORD unnamed_1015168C;

NUDE TaskUseGun_Hook()
{
	_asm mov unnamed_10151628, ecx
	_asm mov eax, [esp+4]
	_asm mov dwProcessControlPed, eax

	byteSavedCurrentPlayer = *pbyteCurrentPlayer;
	byteProcessControlPlayerID = FindPlayerNumFromPedPtr(dwProcessControlPed);

	if(dwProcessControlPed && byteProcessControlPlayerID && !byteSavedCurrentPlayer)
	{
		// run the task with the remote player's keys, camera and skills in place
		GameStoreLocalPlayerKeys();
		GameSetRemotePlayerKeys(byteProcessControlPlayerID);

		unnamed_10150BC0 = *pbyteCameraMode;
		*pbyteCameraMode = GameGetPlayerCameraMode(byteProcessControlPlayerID);

		unnamed_10150730 = *wCameraMode2;
		*wCameraMode2 = GameGetPlayerCameraMode(byteProcessControlPlayerID);
		if(*wCameraMode2 == 4) *wCameraMode2 = 0;

		GameStoreLocalPlayerCameraExtZoom();
		GameSetRemotePlayerCameraExtZoom(byteProcessControlPlayerID);

		GameStoreLocalPlayerAim();
		GameSetRemotePlayerAim(byteProcessControlPlayerID);

		GameStoreLocalPlayerWeaponSkills();
		GameSetRemotePlayerWeaponSkills(byteProcessControlPlayerID);

		*pbyteCurrentPlayer = byteProcessControlPlayerID;

		_asm mov ecx, unnamed_10151628
		_asm push dwProcessControlPed
		_asm mov eax, 0x624ED0	;// CTaskSimpleUseGun::ProcessPed
		_asm call eax

		*pbyteCameraMode = unnamed_10150BC0;
		*wCameraMode2 = unnamed_10150730;

		GameSetLocalPlayerWeaponSkills();
		GameSetLocalPlayerCameraExtZoom();
		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerAim();
		GameSetLocalPlayerKeys();
	}
	else
	{
		unnamed_1015168C = 1;

		_asm mov ecx, unnamed_10151628
		_asm push dwProcessControlPed
		_asm mov eax, 0x624ED0
		_asm call eax

		unnamed_1015168C = 0;
	}

	_asm retn 4
}

//-----------------------------------------------------------

DWORD unnamed_10151684;
DWORD unnamed_10151688;

NUDE WeaponRender__GetWeaponSkill_Hook()
{
	_asm mov dwProcessControlPed, ecx

	_pProcessControlPed = (PED_TYPE *)dwProcessControlPed;
	byteSavedCurrentPlayer = *pbyteCurrentPlayer;
	byteProcessControlPlayerID = FindPlayerNumFromPedPtr(dwProcessControlPed);

	if(dwProcessControlPed && byteProcessControlPlayerID && !byteSavedCurrentPlayer)
	{
		// render a remote player with his own skill level
		GameStoreLocalPlayerWeaponSkills();
		GameSetRemotePlayerWeaponSkills(byteProcessControlPlayerID);
		unnamed_10151688 = 1;
	}

	_asm mov ecx, dwProcessControlPed
	_asm movsx eax, byte ptr [ecx+0x718]
	_asm imul eax, 0x1C
	_asm mov eax, [eax+ecx+0x5A0]
	_asm push eax
	_asm mov edx, 0x5E3B60	;// CPed::GetWeaponSkill
	_asm call edx
	_asm mov unnamed_10151684, eax

	if(unnamed_10151688)
	{
		GameSetLocalPlayerWeaponSkills();
		unnamed_10151688 = 0;
	}

	_asm mov eax, unnamed_10151684
	_asm retn
}

//-----------------------------------------------------------

DWORD dwAttachedEntitiesPlayer;
CRemotePlayer *pAttachedEntitiesRemotePlayer;

void FUNC_10014650(); // .text:10014650

NUDE CWorld__ProcessAttachedEntities_Hook()
{
	__asm
	{
		// call original CWorld::ProcessAttachedEntities
		mov edx, 0x5647F0
		call edx

		pushad
	}

	if(pNetGame)
	{
		if(pNetGame->GetPlayerPool())
			pNetGame->GetPlayerPool()->m_pLocalPlayer->FUNC_10003570();

		if(pNetGame && pNetGame->GetPlayerPool())
		{
			for(dwAttachedEntitiesPlayer = 0; dwAttachedEntitiesPlayer != MAX_PLAYERS; dwAttachedEntitiesPlayer++)
			{
				// GetAt's bound check and body, written out; the PLAYERID cast is what truncates to a word
				PLAYERID playerId = (PLAYERID)dwAttachedEntitiesPlayer;
				if(playerId < MAX_PLAYERS)
				{
					if(pNetGame->GetPlayerPool()->field_2A[playerId])
					{
						CNetPlayer *pNetPlayer = pNetGame->GetPlayerPool()->m_pPlayers[playerId];
						CRemotePlayer *pRemotePlayer;

						if(pNetPlayer) pRemotePlayer = pNetPlayer->m_pRemotePlayer;
						else pRemotePlayer = NULL;

						pAttachedEntitiesRemotePlayer = pRemotePlayer;

						if(pRemotePlayer && pRemotePlayer->m_pPlayerPed && pRemotePlayer->field_10A)
						{
							FUNC_10014650();
						}
					}
				}
			}
		}
	}

	__asm
	{
		popad
		retn
	}
}

//-----------------------------------------------------------

NUDE CWorld__ProcessPedsAfterPreRender_Hook()
{
	__asm
	{
		// call original CWorld::ProcessPedsAfterPreRender
		mov edx, 0x563430
		call edx

		pushad
	}

	if (pNetGame && pNetGame->GetPlayerPool())
		pNetGame->GetPlayerPool()->FUNC_10013960();

	__asm
	{
		popad
		retn
	}}

//-----------------------------------------------------------

DWORD unnamed_10150978;		// original ProcessControl of the vehicle class
float unnamed_10150A34;
VEHICLE_TYPE *unnamed_10150A38;
BYTE unnamed_10150AA0;
DWORD unnamed_10150BBC;		// vtable of the vehicle being processed
DWORD unnamed_10150C88;
DWORD unnamed_10151690;

NUDE AllVehicles_ProcessControl_Hook()
{
	_asm mov unnamed_10150A38, ecx
	_asm mov eax, [ecx]
	_asm mov unnamed_10150BBC, eax
	_asm pushad

	if(unnamed_10150BBC == 0x871120) unnamed_10150978 = 0x6B1880;		// CAutomobile
	else if(unnamed_10150BBC == 0x8721A0) unnamed_10150978 = 0x6F1770;	// CBoat
	else if(unnamed_10150BBC == 0x871360) unnamed_10150978 = 0x6B9250;	// CBike
	else if(unnamed_10150BBC == 0x871948) unnamed_10150978 = 0x6C9260;	// CPlane
	else if(unnamed_10150BBC == 0x871680) unnamed_10150978 = 0x6C7050;	// CHeli
	else if(unnamed_10150BBC == 0x871528) unnamed_10150978 = 0x6BFA30;	// CBmx
	else if(unnamed_10150BBC == 0x8717D8) unnamed_10150978 = 0x6C8250;	// CMonsterTruck
	else if(unnamed_10150BBC == 0x871AE8) unnamed_10150978 = 0x6CDCC0;	// CQuadBike
	else if(unnamed_10150BBC == 0x872370) unnamed_10150978 = 0x6F86A0;	// CTrain
	else if(unnamed_10150BBC == 0x871C28) unnamed_10150978 = 0x6CED20;	// CTrailer

	byteSavedCurrentPlayer = *(BYTE *)0xB7CD74;

	PED_TYPE *pDriver;
	pDriver = unnamed_10150A38->pDriver;

	// a remote player drives it, run the class ProcessControl with his keys and aim
	if(unnamed_10150A38->pDriver && pDriver->dwPedType == 0 &&
		pDriver != GamePool_FindPlayerPed() && byteSavedCurrentPlayer == 0)
	{
		byteProcessControlPlayerID = FindPlayerNumFromPedPtr((DWORD)pDriver);

		GameStoreLocalPlayerKeys();
		GameSetRemotePlayerKeys(byteProcessControlPlayerID);

		unnamed_10150BC0 = *pbyteCameraMode;
		*pbyteCameraMode = GameGetPlayerCameraMode(byteProcessControlPlayerID);
		unnamed_10150730 = *wCameraMode2;
		*wCameraMode2 = GameGetPlayerCameraMode(byteProcessControlPlayerID);
		if(*wCameraMode2 == 4) *wCameraMode2 = 0;

		GameStoreLocalPlayerAim();
		GameSetRemotePlayerAim(byteProcessControlPlayerID);

		if((unnamed_10150A38->byteFlags & 0x10) == 0)
		{
			if(unnamed_10150BBC == 0x871680) unnamed_10150A38->entity.nControlFlags |= 0x20;

			pProcessControlKeys = GameGetInternalKeys();
			pProcessControlKeys->wKeys1[14] = 0;
			pProcessControlKeys->wKeys2[14] = 0;
			pProcessControlKeys->wKeys1[16] = 0;
			pProcessControlKeys->wKeys2[16] = 0;
		} else {
			unnamed_10150A38->entity.nControlFlags &= 0xDF;
		}

		unnamed_10150C88 = *(DWORD *)((BYTE *)unnamed_10150A38 + 0x648);
		*(DWORD *)&unnamed_10150A34 = *(DWORD *)((BYTE *)unnamed_10150A38 + 0x64C);

		// let the game process it as a civillian driven vehicle
		*pbyteCurrentPlayer = 0;
		unnamed_10150A38->pDriver->dwPedType = 4;
		unnamed_10150AA0 = unnamed_10150A38->entity.nControlFlags;
		unnamed_10150A38->entity.nControlFlags = 0x1A;

		_asm mov edx, unnamed_10150A38
		_asm lea ecx, [edx+0x138]
		_asm mov edx, 0x502280
		_asm call edx

		unnamed_10150A38->entity.nControlFlags = unnamed_10150AA0;
		unnamed_10150A38->pDriver->dwPedType = 0;
		*pbyteCurrentPlayer = byteProcessControlPlayerID;

		MATRIX4X4 *pMatrix = unnamed_10150A38->entity.mat;
		if(pMatrix)
		{
			unnamed_10150A48 = *pMatrix;
			*(DWORD *)&unnamed_10150C1C = *(DWORD *)&unnamed_10150A38->entity.vecMoveSpeed.X;
			*(DWORD *)&unnamed_10150C20 = *(DWORD *)&unnamed_10150A38->entity.vecMoveSpeed.Y;
			*(DWORD *)&unnamed_10150C24 = *(DWORD *)&unnamed_10150A38->entity.vecMoveSpeed.Z;
		}

		_asm mov ecx, unnamed_10150A38
		_asm mov eax, unnamed_10150978
		_asm call eax

		if(unnamed_10150A38->entity.mat && !FUNC_100B6950(unnamed_10150A38) &&
			!IsATrainPart(&unnamed_10150A38->entity))
		{
			// keep the height and vertical speed the server gave us
			*(DWORD *)&unnamed_10150A38->entity.mat->pos.Z = *(DWORD *)&unnamed_10150A48.pos.Z;
			unnamed_10150A38->entity.vecMoveSpeed.Z = unnamed_10150C24;
		}
		else if(unnamed_10150A38->entity.mat && (unnamed_10150978 == 0x6F1770 ||
			unnamed_10150978 == 0x6C9260 || unnamed_10150978 == 0x6C7050))
		{
			*(DWORD *)&unnamed_10150A38->entity.mat->pos.Z = *(DWORD *)&unnamed_10150A48.pos.Z;
			unnamed_10150A38->entity.vecMoveSpeed.Z = unnamed_10150C24;
		}

		if(unnamed_10150978 == 0x6BFA30 || unnamed_10150978 == 0x6B9250)
		{
			// bikes, ease the lean angle half way back to the synced one
			*(DWORD *)((BYTE *)unnamed_10150A38 + 0x648) = unnamed_10150C88;
			*(float *)((BYTE *)unnamed_10150A38 + 0x64C) = unnamed_10150A34;

			if(*(float *)((BYTE *)unnamed_10150A38 + 0x64C) > *(float *)((BYTE *)unnamed_10150A38 + 0x648))
			{
				*(float *)((BYTE *)unnamed_10150A38 + 0x648) = (*(float *)((BYTE *)unnamed_10150A38 + 0x64C) -
					*(float *)((BYTE *)unnamed_10150A38 + 0x648)) * 0.5f + *(float *)((BYTE *)unnamed_10150A38 + 0x648);
			}
			else if(*(float *)((BYTE *)unnamed_10150A38 + 0x64C) < *(float *)((BYTE *)unnamed_10150A38 + 0x648))
			{
				*(float *)((BYTE *)unnamed_10150A38 + 0x648) = *(float *)((BYTE *)unnamed_10150A38 + 0x648) -
					(*(float *)((BYTE *)unnamed_10150A38 + 0x648) - *(float *)((BYTE *)unnamed_10150A38 + 0x64C)) * 0.5f;
			}
		}

		*pbyteCurrentPlayer = 0;
		GameSetLocalPlayerKeys();
		*pbyteCameraMode = unnamed_10150BC0;
		*wCameraMode2 = unnamed_10150730;
		GameSetLocalPlayerAim();

		_asm popad
		_asm retn
	}
	else if(pDriver && pDriver->dwPedType == 0 &&
		unnamed_10150A38->pDriver == GamePool_FindPlayerPed())
	{
		if((unnamed_10150A38->byteFlags & 0x10) == 0)
		{
			if(unnamed_10150BBC == 0x871680) unnamed_10150A38->entity.nControlFlags |= 0x20;

			unnamed_10151690 = 1;
			GameStoreLocalPlayerKeys();

			pProcessControlKeys = GameGetInternalKeys();
			pProcessControlKeys->wKeys1[14] = 0;
			pProcessControlKeys->wKeys2[14] = 0;
			pProcessControlKeys->wKeys1[16] = 0;
			pProcessControlKeys->wKeys2[16] = 0;
		} else {
			unnamed_10150A38->entity.nControlFlags &= 0xDF;
		}

		_asm mov edx, unnamed_10150A38
		_asm lea ecx, [edx+0x138]
		_asm mov edx, 0x502280
		_asm call edx

		_asm mov ecx, unnamed_10150A38
		_asm mov eax, unnamed_10150978
		_asm call eax

		if(unnamed_10151690)
		{
			unnamed_10151690 = 0;
			GameSetLocalPlayerKeys();
		}

		_asm popad
		_asm retn
	}
	else
	{
		if(FUNC_100B4860(unnamed_10150A38) || IsATrainPart(&unnamed_10150A38->entity) ||
			FUNC_100B6950(unnamed_10150A38))
		{
			MATRIX4X4 *pMatrix = unnamed_10150A38->entity.mat;
			if(pMatrix)
			{
				unnamed_10150A48 = *pMatrix;
				*(DWORD *)&unnamed_10150C1C = *(DWORD *)&unnamed_10150A38->entity.vecMoveSpeed.X;
				*(DWORD *)&unnamed_10150C20 = *(DWORD *)&unnamed_10150A38->entity.vecMoveSpeed.Y;
				*(DWORD *)&unnamed_10150C24 = *(DWORD *)&unnamed_10150A38->entity.vecMoveSpeed.Z;
			}

			_asm mov edx, unnamed_10150A38
			_asm lea ecx, [edx+0x138]
			_asm mov edx, 0x502280
			_asm call edx

			_asm mov ecx, unnamed_10150A38
			_asm mov eax, unnamed_10150978
			_asm call eax
		}

		_asm popad
		_asm retn
	}

	_asm popad
	_asm retn
}

//-----------------------------------------------------------
// fix horn processing

VEHICLE_TYPE *_pHornVehicle;
int _iHasSetHornHookFix = 0;
BYTE _byteSavedControlFlags = 0;
DWORD _dwVehicleParams = 0;
DWORD _dwAudioClass = 0;

NUDE VehicleHorn_Hook()
{
	_asm mov _dwAudioClass, ecx

	_asm mov edx, [esp+4]
	_asm mov _dwVehicleParams, edx

	_asm mov eax, [edx+16]	
	_asm mov _pHornVehicle, eax

	_byteSavedControlFlags = _pHornVehicle->entity.nControlFlags;

	if( _pHornVehicle->pDriver && 
		IN_VEHICLE(_pHornVehicle->pDriver) ) 
	{
		_pHornVehicle->entity.nControlFlags = 0x02;
		//_iHasSetHornHookFix = 1;
	} else {
		_pHornVehicle->entity.nControlFlags = 0x22;
		//_iHasSetHornHookFix = 0;
	}

	_asm push _dwVehicleParams
	_asm mov ecx, _dwAudioClass
	_asm mov edx, 0x5002C0
	_asm call edx
	
	_pHornVehicle->entity.nControlFlags = _byteSavedControlFlags;
    
	_asm retn 4
}

//-----------------------------------------------------------

DWORD dwSayParam1;
DWORD dwSayParam2;
float fSayParam3;
DWORD dwSayParam4;
DWORD dwSayParam5;
DWORD dwSayParam6;

NUDE CPed_Say_Hook()
{
    _asm mov eax, [esp+4]
	_asm mov dwSayParam1, eax
	_asm mov eax, [esp+8]
	_asm mov dwSayParam2, eax
	_asm mov eax, [esp+12]
	_asm mov fSayParam3, eax
	_asm mov eax, [esp+16]
	_asm mov dwSayParam4, eax
	_asm mov eax, [esp+20]
	_asm mov dwSayParam5, eax
	_asm mov eax, [esp+24]
	_asm mov dwSayParam6, eax

	_asm pushad

	if(dwSayParam1 != 45) {
		if(pChatWindow) pChatWindow->AddDebugMessage("CPed::Say(%u,%u,%f,%u,%u,%u)",
			dwSayParam1,dwSayParam2,fSayParam3,dwSayParam4,dwSayParam5,dwSayParam6);
	}

	_asm popad

    _asm mov eax, [esp+4]
    _asm test ax, ax
	_asm mov edx, 0x5EFFE7
	_asm jmp edx
}

//-----------------------------------------------------------

NUDE ZoneOverlay_Hook() 
{
	_asm pushad
	if (pNetGame && pNetGame->GetGangZonePool()) pNetGame->GetGangZonePool()->Draw();
	_asm popad
	_asm ret
}

//-----------------------------------------------------------

NUDE PlayerWalk_Hook()
{
	_asm pushad

	if (pNetGame && pNetGame->GetWalkStyle())
	{
		_asm popad
		_asm mov [esi + 0x4D4], eax
	}
	else
	{
		_asm popad
	}
	_asm ret
}

//-----------------------------------------------------------

NUDE PickUpPickup_Hook()
{
	_asm mov unnamed_10150960, esi
	_asm pushad

	if (pNetGame && pNetGame->GetPickupPool())
		pNetGame->GetPickupPool()->sub_10013440((unnamed_10150960 - 0x9788C0) >> 5);

	_asm popad
	_asm mov al, [esi+0x1C]
	_asm cmp al, 6
	_asm push 0x4579CB
	_asm ret
}

//-----------------------------------------------------------

NUDE CWeapon_FireCamera_Hook()
{
	_asm mov ebx, [esp+8]
	_asm mov dwCurPlayerActor, ebx
	_asm pushad

	if (dwCurPlayerActor == (DWORD)GamePool_FindPlayerPed()) {
		*(BYTE*)0xC8A7C0 = 1;
		*(BYTE*)0xC8A7C1 = 1;
	}

	_asm popad
	_asm mov ebx, 0x73C260
	_asm jmp ebx
}

//-----------------------------------------------------------

NUDE CCameraCamShake_Sniper_Hook()
{
	_asm ret
}

//-----------------------------------------------------------

NUDE CTrain_ProcessControl_Derailment()
{
	_asm {
		mov eax, [esi+1444];	// get the speed
		and eax, 0x80000000;	// get the upper bit
		or eax, 0x3F800000;		// or it with 1.0f (it'll be -ve if bit is set)
		mov [esi+1444], eax;	// set the new speed
		ret;
	}
}

//-----------------------------------------------------------

int FUNC_100A36C0(); // todo: implement `FUNC_100A36C0`

DWORD dwEnterExitVehicle;

NUDE TaskEnterVehicleDriver_Hook()
{
	_asm mov eax, [esp]
	_asm mov dwHookRetnAddr, eax
	_asm mov eax, [esp+4]
	_asm mov dwEnterExitVehicle, eax
	_asm pushad

	if((dwHookRetnAddr == 0x570A20 || dwHookRetnAddr == 0x570A99) && !FUNC_100A36C0())
	{
		_asm
		{
			popad

			// kill the task, we're not letting them in
			mov ebx, [ecx]
			test ebx, ebx
			jz noTaskDelete
			push 1
			call dword ptr [ebx]
noTaskDelete:
			pop eax
			pop eax
			mov eax, 0x570A9E
			jmp eax
		}
	}

	_asm popad
	_asm mov eax, [esp+4]
	_asm push esi
	_asm push 0
	_asm mov ebx, 0x6402F7
	_asm jmp ebx
}

//-----------------------------------------------------------

DWORD dwExitTaskThis;
DWORD dwPutOutOfVehicle = 0xFFFFFFFF;
CVehiclePool *pExitVehiclePool;
WORD wExitVehicleID;
CVehicle *pExitVehicle;
CLocalPlayer *pExitLocalPlayer;
MATRIX4X4 matExitVehicle;

NUDE TaskExitVehicle()
{
	_asm mov ebp, esp
	_asm mov dwExitTaskThis, ecx
	_asm mov eax, [ebp]
	_asm mov dwHookRetnAddr, eax
	_asm mov eax, [ebp+4]
	_asm mov dwEnterExitVehicle, eax

	dwPutOutOfVehicle = 0xFFFFFFFF;

	if(!bIgnoreNextExit &&
		(dwHookRetnAddr == 0x5704A1 || dwHookRetnAddr == 0x5703FC))
	{
		if(pNetGame)
		{
			if(GamePool_FindPlayerPed()->pVehicle == dwEnterExitVehicle)
			{
				pExitVehiclePool = pNetGame->GetVehiclePool();
				wExitVehicleID = pExitVehiclePool->FUNC_1001EB90((int)GamePool_FindPlayerPed()->pVehicle);

				if(wExitVehicleID != 0xFFFF)
				{
					pExitVehicle = (CVehicle *)pExitVehiclePool->FUNC_10001120(wExitVehicleID);
					pExitLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

					if(pExitVehicle && pExitLocalPlayer)
					{
						if(pExitVehicle->IsATrainPart())
						{
							// exiting a train part normally doesn't work, dump them next to it
							dwPutOutOfVehicle = 1;
							pExitVehicle->GetMatrix(&matExitVehicle);
							pExitLocalPlayer->GetPlayerPed()->RemoveFromVehicleAndPutAt(
								matExitVehicle.pos.X + 2.5f,
								matExitVehicle.pos.Y + 2.5f,
								matExitVehicle.pos.Z);
						}
						else
						{
							pExitLocalPlayer->FUNC_10005BF0(wExitVehicleID);
						}
					}
				}
			}
		}
	}
	else
	{
		bIgnoreNextExit = FALSE;
	}

	_asm push dwPutOutOfVehicle
	_asm push 0x841618
	_asm mov ecx, dwExitTaskThis
	_asm mov edx, 0x63B8C0
	_asm add edx, 7
	_asm jmp edx
}

//-----------------------------------------------------------

NUDE AddVehicleHook()
{
	_asm pushad

	if(!bAllowVehicleCreation) {
		_asm popad
		_asm xor eax, eax
		_asm ret
	}

	_asm popad
	_asm mov eax, 0x421446
	_asm jmp eax
}

//-----------------------------------------------------------

// disable cheat code processing
NUDE CheatProcessorHook()
{
	__asm
	{
		mov dwSavedCheatFn, eax
		pushad

		// CCheat::m_bHasPlayerCheated = true
		mov edx, 0x96918C  
		mov byte ptr [edx], 1

		popad

		// dont process cheat
		mov edx, 0x438581
		cmp eax, 0
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE RadarTranslateColor() 
{
	_asm mov eax, [esp+4]
	_asm mov iRadarColor1, eax
	TranslateColorCodeToRGBA(iRadarColor1); // return will still be in eax.
	_asm ret
}

//-----------------------------------------------------------

NUDE SetFarClipHook()
{
	_asm fld fFarClip
	_asm push esi
	_asm mov esi, [esp+8]
	_asm mov edx, dwFarClipReturnAddr
	_asm jmp edx
}

//-----------------------------------------------------------
// We use this to trap and exit the game

DWORD dwShutDownTick;
void QuitGame(); // todo: implement `QuitGame`

NUDE CGameShutdownHook()
{
	dwShutDownTick = GetTickCount() + 2000;
	QuitGame();

	while(GetTickCount() < dwShutDownTick) {
		Sleep(100);
	}

	ExitProcess(0);
}

//-----------------------------------------------------------

DWORD unnamed_101516A0;	// CEventDamage *
DWORD unnamed_101516A4;	// CPed * taking the damage

int __stdcall FUNC_100A3BB0(DWORD *pEventDamage, PED_TYPE *pPed); // todo: implement `FUNC_100A3BB0`

// maps a ped pointer to a player id, falling back to the driver of the vehicle it is in
PLAYERID FUNC_100A3AE0(DWORD dwPed)
{
	if(!dwPed) return 0xFFFF;

	PLAYERID playerId;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	playerId = pPlayerPool->FUNC_100138C0(dwPed);

	if(playerId == 0xFFFF)
	{
		VEHICLEID vehicleId = pNetGame->GetVehiclePool()->FUNC_1001EB90(dwPed);

		if(vehicleId != 0xFFFF)
		{
			int iHighestId = pPlayerPool->field_2F3A;

			for(int i = 0; i <= iHighestId; i++)
			{
				PLAYERID id = (PLAYERID)i;

				if(id < MAX_PLAYERS && pPlayerPool->field_2A[id])
				{
					CNetPlayer *pNetPlayer = pPlayerPool->m_pPlayers[id];
					CRemotePlayer *pRemotePlayer;

					if(pNetPlayer) pRemotePlayer = pNetPlayer->m_pRemotePlayer;
					else pRemotePlayer = NULL;

					// no null check, same as retail
					if(pRemotePlayer->field_1E7 == vehicleId)
					{
						playerId = id;
						break;
					}
				}
			}
		}
	}

	return playerId;
}

//-----------------------------------------------------------

// maps a ped pointer to the actor that owns it
ACTORID FUNC_100A3B80(DWORD dwPed)
{
	if(!dwPed) return 0xFFFF;
	return pNetGame->GetActorPool()->FUNC_100018B0(dwPed);
}

//-----------------------------------------------------------

// 0x100A3BB0 : decides whether a ped damage event gets reported to the server
BOOL __stdcall FUNC_100A3BB0(DWORD *pEvent, PED_TYPE *pPed)
{
	PED_TYPE *pPlayerPed = GamePool_FindPlayerPed();
	bool bLocalTarget = false;

	if(!pNetGame) return FALSE;

	if((PED_TYPE *)pEvent[0] != pPlayerPed && pPed && *(DWORD *)pPed == 0x86C0A8) return TRUE;

	if(pPed != pPlayerPed)
	{
		if((PED_TYPE *)pEvent[0] != pPlayerPed) return FALSE;
	}
	else bLocalTarget = true;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	PLAYERID playerId;

	if(bLocalTarget)
	{
		DWORD dwTask = pPlayerPool->m_pLocalPlayer->field_2DA;

		if(dwTask)
		{
			if(*(DWORD *)(dwTask + 0x40) == pEvent[0]) return TRUE;
			if(pEvent[3] == 0x36) return TRUE;
		}

		playerId = FUNC_100A3AE0(pEvent[0]);
		pPlayerPool->m_pLocalPlayer->FUNC_100068A0(playerId, *(float *)&pEvent[1], pEvent[3], pEvent[2]);
	}
	else
	{
		playerId = FUNC_100A3AE0((DWORD)pPed);

		if(playerId != 0xFFFF)
		{
			pPlayerPool->m_pLocalPlayer->FUNC_100069B0(playerId, *(float *)&pEvent[1], pEvent[3], pEvent[2]);

			if(pPlayerPool->GetAt(playerId)->field_10C) return TRUE;
		}
		else
		{
			ACTORID actorId = FUNC_100A3B80((DWORD)pPed);

			if(actorId != 0xFFFF)
			{
				pPlayerPool->m_pLocalPlayer->FUNC_10006AC0(actorId, *(float *)&pEvent[1], pEvent[3], pEvent[2]);
				return TRUE;
			}
		}
	}

	if(pNetGame->GetSettings()->field_22)
	{
		BYTE byteTeam = pPlayerPool->m_pLocalPlayer->m_byteTeam;

		if(byteTeam == 0xFF) return FALSE;
		if(playerId == 0xFFFF) return FALSE;

		if((BYTE)pNetGame->GetPlayerPool()->GetAt(playerId)->field_109 == byteTeam) return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------

NUDE PedDamage_Hook()
{
	__asm
	{
		mov eax, fs:0
		test ecx, ecx
		jz computeResponse
		mov unnamed_101516A0, ecx
		mov ebx, [esp+4]
		test ebx, ebx
		jz computeResponse
		mov unnamed_101516A4, ebx
		pushad

		mov eax, unnamed_101516A4
		push eax
		mov ecx, unnamed_101516A0
		push ecx
		call FUNC_100A3BB0
		test eax, eax
		jz computeResponseAfterSync

		// the netcode dealt with it, don't let the game apply the damage
		popad
		retn 0Ch
	computeResponseAfterSync:
		popad
	computeResponse:
		mov edx, 0x4B5AC6	;// CEventDamage::ComputeDamageResponse + 6
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE AnimCrashFixHook()
{
	__asm
	{
		push edi
		mov edi, [esp+8]	;// arg0
		test edi, edi
		jz exitFn
		mov eax, 0x4D41C5	;// CAnimManager::UncompressAnimation
		jmp eax
exitFn:
		pop edi
		ret
	}
}

//-----------------------------------------------------------

NUDE SetForegroundWindowCrashFixHook()
{
	__asm
	{
		mov eax, ds:0xC17054;
		test eax, eax;
		jz exitFn;
		push 0x746929;	// return address;
		ret;
exitFn:
		xor eax, eax;
		ret;
	}
}

//-----------------------------------------------------------

NUDE GenTaskAlloc_Hook()
{
	_asm mov edi, [esp+4]
	_asm mov dwParam1, edi
	//_asm mov edx, [esp+8]
	//_asm mov dwParam2, edx
	_asm mov dwParamThis, ecx

	_asm pushad
	_asm mov edx, [edi]
	_asm mov ecx, edi
	_asm call dword ptr [edx+4]
	_asm mov dwParam2, eax

	//pChatWindow->AddDebugMessage("TaskAlloc: 0x%X, 0x%X, 0x%X",dwParamThis,dwParam1,dwParam2);
	_asm popad

	_asm mov eax, fs:0
	_asm mov edx, 0x4C3876
	_asm jmp edx
}

//-----------------------------------------------------------
// used for correct report message using 100A1790 ; void __thiscall CGame::PlayCrimeReport

NUDE PoliceScannerAudio_FindPlayerPed_Hook() 
{
	__asm
	{
		mov eax, offset pedCrimeReportTemp
		retn
	}
}

//-----------------------------------------------------------
// delete satchel projectile if the owner is dead (not sure if this code will work properly)

DWORD dwProjectileObject = 0;
DWORD dwProjectileInfo = 0;
NUDE CProjectile_Update_Hook() 
{
	__asm
	{
		mov dwProjectileObject, esi
		mov dwProjectileInfo, ebx

		pushad

		mov eax, dwProjectileInfo
		mov ecx, [eax+4]   ;// dwProjectileInfo->pOwner
		test ecx, ecx 		// 
		jnz dont_delete 	// if the owner == nullptr, delete the projectile

		// delete projectile CProjectileInfo::RemoveIfThisIsAProjectile
		push dwProjectileObject
		mov eax, 0x739A40
		call eax

		// correct stack
		add esp, 4

		popad

		mov dl, 0x0FF
		jmp end_hook

	dont_delete:
		popad 

		// original code
		mov ecx, [ebx+4]
		mov dl, [ecx+36h]

	end_hook:
		push 0x738F40
		retn
	}
}

//-----------------------------------------------------------

NUDE CWeapon__Satchel__Activate_Hook()
{
	__asm
	{
		CMP DWORD PTR DS:[ESI-0x20], 0x27
		MOV EDI, DWORD PTR SS:[EBP]
		JNZ skip
		MOV EAX, [ESP]
		CMP DWORD PTR DS:[ESI-0x1C], EAX
		JNZ skip
		MOV EAX, 0x738880
		JMP EAX

skip:
		MOV EAX, 0x7388DB
		JMP EAX
	}
}

//-----------------------------------------------------------

NUDE PathProcessingCrashFixHook()
{
	__asm
	{
		push esi
		test ecx, ecx;
		jz exitFn
		mov esi,dword ptr ss:[esp+8]
		mov eax,dword ptr ds:[esi]
		mov edx, 0x63A047;
		jmp edx;

exitFn:
		mov esi, 0x63a04d;
		jmp esi;
	}
}

//-----------------------------------------------------------

NUDE SetCarColorCrashFix()
{
	__asm
	{
		test eax, eax;
		jz exitFn;						// CPool_CVehicle_GetAt(scmParam1) == NULL
		mov ecx, 0x47eab8;
		mov cl,byte ptr ds:[0xA43C7C];	// get color1 (was replaced by jmpcode)
		jmp ecx;						// continue function

exitFn:
		mov esi, 0x47eaca;
		jmp esi;
	}
}

//-----------------------------------------------------------

NUDE GetText_Hook()
{
	__asm
	{
		// stolen prologue from CText::Get
		sub esp, 0x20
		push esi
		push edi
		mov edi, [esp+0x2C]
		pushad
		mov unnamed_10151628, edi
	}

	unnamed_10150734 = 0;

	// SAMP prefixed keys are server menu text, not gxt entries
	if(pNetGame && unnamed_10151628[0] == 'S' && unnamed_10151628[1] == 'A' &&
		unnamed_10151628[2] == 'M' && unnamed_10151628[3] == 'P' && pNetGame->GetMenuPool())
	{
		unnamed_10150734 = pNetGame->GetMenuPool()->GetTextPointer(unnamed_10151628 + 4);
	}

	__asm
	{
		popad
		cmp unnamed_10150734, 0
		jz callOriginal
		mov eax, unnamed_10150734
		pop edi
		pop esi
		add esp, 0x20
		retn 4

callOriginal:
		mov eax, 0x6A0059
		jmp eax
	}
}

//-----------------------------------------------------------

DWORD unnamed_101516CC;

// .text:100A4470 : caches the d3d texture the plate raster carries at +0x34
DWORD FUNC_100A4470()
{
	if(unnamed_101516C8)
	{
		_asm mov eax, unnamed_101516C8
		_asm mov edx, 0x34
		_asm lea esi, [edx+eax]
		_asm mov eax, [esi]
		_asm mov unnamed_101516CC, eax

		return unnamed_101516CC;
	}

	return 0;
}

//-----------------------------------------------------------

// .text:100A44A0 : drops the cached plate raster
void FUNC_100A44A0()
{
	DWORD dwRasterDestroy = 0x7FB060;
	if(iGtaVersion == GTASA_VERSION_USA10) dwRasterDestroy = 0x7FB020;

	if(unnamed_101516C8)
	{
		_asm push unnamed_101516C8
		_asm mov edx, dwRasterDestroy
		_asm call edx
		_asm pop edx
	}

	unnamed_101516C8 = 0;
}

//-----------------------------------------------------------

// .text:100A44E0 : creates the plate raster, unconditionally
void FUNC_100A44E0()
{
	if(iGtaVersion == GTASA_VERSION_USA10) unnamed_10113ADC = 0x7FB230;

	_asm push 0x604
	_asm push 0x20
	_asm push 0x10
	_asm push 0x40
	_asm mov ebx, unnamed_10113ADC
	_asm call ebx
	_asm mov unnamed_101516C8, eax
	_asm pop ebx
	_asm pop ebx
	_asm pop ebx
	_asm pop ebx
}

//-----------------------------------------------------------

NUDE CCustomCarPlateMgr__CreatePlateTexture__RwRasterCreate_Hook()
{
	__asm
	{
		// only ever create the plate raster once
		mov eax, unnamed_101516C8
		test eax, eax
		jnz exitFn

		cmp iGtaVersion, GTASA_VERSION_USA10
		jnz createRaster
		mov unnamed_10113ADC, 0x7FB230

createRaster:
		push 0x604
		push 0x20
		push 0x10
		push 0x40
		mov ebx, unnamed_10113ADC
		call ebx
		mov unnamed_101516C8, eax
		pop ebx
		pop ebx
		pop ebx
		pop ebx

exitFn:
		mov eax, unnamed_101516C8
		retn
	}
}

//-----------------------------------------------------------

NUDE RwRasterDestroy_Hook()
{
	__asm
	{
		mov eax, [esp+4]	;// arg0 (RwRaster *)
		mov unnamed_101516D0, eax
		pushad

		// never let the game destroy our cached plate raster
		cmp unnamed_10113AD8, 0
		jz destroyRaster
		cmp unnamed_101516C8, 0
		jz destroyRaster
		mov eax, unnamed_101516D0
		cmp eax, unnamed_101516C8
		jnz destroyRaster

		popad
		mov eax, 1
		retn

destroyRaster:
		popad
		push esi
		mov esi, [esp+8]
		push esi
		mov edx, unnamed_101516D4
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE CVehicle__Render_Hook()
{
	_asm mov unnamed_101516D8, esi
	_asm ret
}

//-----------------------------------------------------------

DWORD dwRenderObjectRet = 0;
ENTITY_TYPE *_pRenderEntity;
CObject *_pRenderNetObject;
CPlayerPool *_pRenderPlayerPool;

NUDE CObject__Render_Hook()
{
	_asm mov eax, [esp]
	_asm mov dwRenderObjectRet, eax
	_asm mov _pRenderEntity, ecx

	_asm pushad

	if (_pRenderEntity) wLastRendObj = _pRenderEntity->nModelIndex;

	_pRenderNetObject = NULL;

	if (pNetGame && _pRenderEntity && (_pRenderEntity->dwProcessingFlags & 0x80000000))
	{
		if ((_pRenderNetObject = pNetGame->GetObjectPool()->FUNC_100129D0(_pRenderEntity)) != NULL ||
			((_pRenderPlayerPool = pNetGame->GetPlayerPool()) != NULL &&
			(_pRenderNetObject = _pRenderPlayerPool->FUNC_10013B70(_pRenderEntity)) != NULL))
		{
			_pRenderNetObject->FUNC_100A9E30();
		}
	}

	if (unnamed_1014FFAC && _pRenderEntity && (_pRenderEntity->dwProcessingFlags & 0x80000000))
	{
		_pRenderNetObject = unnamed_1014FFAC->FUNC_100129D0(_pRenderEntity);
		if (_pRenderNetObject) _pRenderNetObject->FUNC_100A9E30();
	}

	_asm
	{
		popad

		// call original CObject::Render
		mov eax, 0x59F180
		call eax

		pushad
	}

	if (pNetGame && _pRenderNetObject) _pRenderNetObject->FUNC_100A9E70();
	if (unnamed_1014FFAC && _pRenderNetObject) _pRenderNetObject->FUNC_100A9E70();

	_asm popad
	_asm retn
}

//-----------------------------------------------------------

NUDE CRadar__DrawMap__FindPlayerSpeed_Hook()
{
	// keep the map still while the local player is a vehicle passenger
	if(pGame && pGame->FindPlayerPed() && pGame->FindPlayerPed()->IsInVehicle() &&
		pGame->FindPlayerPed()->IsAPassenger())
	{
		_asm lea eax, unnamed_10151710
		_asm ret
	}

	_asm mov edx, 0x56E090	;// FindPlayerSpeed
	_asm jmp edx
}

//-----------------------------------------------------------

DWORD dwObjectInstance = 0;
DWORD dwObjectInstanceUnused = 0;
BOOL bReplaceAllObjects = FALSE;

int FUNC_1009D050(int iModelID, float fX, float fY, float fZ); // todo: implement `IsObjectRemoved`

NUDE CFileLoader__LoadObjectInstance_Hook()
{
	_asm mov eax, [esp+4]
	_asm mov dwObjectInstance, eax
	_asm mov eax, [esp+8]
	_asm mov dwObjectInstanceUnused, eax

	_asm pushad

	// replace removed objects with an invisible model
	if (dwObjectInstance && FUNC_1009D050(*(int *)(dwObjectInstance + 0x1C), *(float *)dwObjectInstance,
		*(float *)(dwObjectInstance + 4), *(float *)(dwObjectInstance + 8)))
	{
		*(int *)(dwObjectInstance + 0x1C) = 19300;
	}

	if (bReplaceAllObjects) *(int *)(dwObjectInstance + 0x1C) = 19300;

	_asm
	{
		popad

		push -1
		push 0x83C931
		mov eax, 0x538097
		jmp eax
	}
}

//-----------------------------------------------------------

float fEscalatorCheck1;
float fEscalatorCheck2;

NUDE CEscalator__Update_Hook() 
{
	__asm pushad

	UnFuck(0x858BA4, 4);
	UnFuck(0x858F84, 4);

	fEscalatorCheck1 = *(float*)0x858BA4;
	fEscalatorCheck2 = *(float*)0x858F84;

	*(float*)0x858BA4 = 40.0f;
	*(float*)0x858F84 = 50.0f;

	__asm
	{
		popad

		mov eax, 0x717D30 // CEscalator::Update
		call eax

		pushad
	}

	*(float*)0x858BA4 = fEscalatorCheck1;
	*(float*)0x858F84 = fEscalatorCheck2;
	
	__asm popad
	__asm ret
}

//-----------------------------------------------------------

NUDE CObject__CreateRwObject_Hook()
{
	_asm mov eax, [esp]
	_asm mov unnamed_1015172C, eax
	_asm pushad

	// stop the original from calling the roadsign/plate atomic setup
	UnFuck(0x4096AA, 3);
	memset((void *)0x4096AA, 0x90, 3);

	__asm
	{
		popad

		mov eax, 0x533D30 // CObject::CreateRwObject
		call eax

		pushad
	}

	memcpy((void *)0x4096AA, unnamed_10113AE8, 3);

	__asm popad
	__asm ret
}

//-----------------------------------------------------------

DWORD dwDeleteRwObjectRet = 0;
ENTITY_TYPE *_pDeleteRwObjectEntity;
BYTE byteSavedRoadsignCode[5] = {0xE8,0x3C,0xFF,0xFF,0xFF};

NUDE CEntity__DeleteRwObject_Hook()
{
	_asm mov eax, [esp]
	_asm mov dwDeleteRwObjectRet, eax
	_asm mov _pDeleteRwObjectEntity, ecx

	_asm pushad

	if (_pDeleteRwObjectEntity->nModelIndex == 3586 || _pDeleteRwObjectEntity->nModelIndex == 3743 ||
		_pDeleteRwObjectEntity->nModelIndex == 8980 || _pDeleteRwObjectEntity->nModelIndex == 8979)
	{
		// nop the CCustomRoadsignMgr call these models crash in
		UnFuck(0x71791F, 5);
		memset((void *)0x71791F, 0x90, 5);

		_asm
		{
			popad

			// call original CEntity::DeleteRwObject
			mov eax, 0x534030
			call eax

			pushad
		}

		memcpy((void *)0x71791F, byteSavedRoadsignCode, 5);

		_asm popad
		_asm retn
	}

	_asm popad
	_asm mov eax, 0x534030
	_asm jmp eax
}

//-----------------------------------------------------------

VEHICLE_TYPE	*_pBreakTowLinkVehicle;
VEHICLEID		_BreakTowLinkTractorId;
CVehiclePool	*_pBreakTowLinkPool;
// set while samp detaches a trailer itself, so gta gets to break the link
BOOL			bIgnoreBreakTowLinkHook;
CVehicle		*_pBreakTowLinkTractor;

NUDE CAutomobile__BreakTowLink_Hook()
{
	__asm
	{
		mov _pBreakTowLinkVehicle, ecx

		pushad
	}

	if (pNetGame && !bIgnoreBreakTowLinkHook && _pBreakTowLinkVehicle &&
		_pBreakTowLinkVehicle->fHealth > 0.0f)
	{
		_pBreakTowLinkPool = pNetGame->FUNC_10001180();
		_BreakTowLinkTractorId = _pBreakTowLinkPool->FUNC_1001EB90(_pBreakTowLinkVehicle->dwTractor);

		if (_BreakTowLinkTractorId != 0xFFFF)
		{
			_pBreakTowLinkTractor = (CVehicle *)_pBreakTowLinkPool->FUNC_10001120(_BreakTowLinkTractorId);

			if (_pBreakTowLinkTractor && !_pBreakTowLinkTractor->IsDriverLocalPlayer() &&
				*(DWORD *)_pBreakTowLinkTractor->_gap48)
			{
				__asm
				{
					popad
					retn
				}
			}
		}
	}

	__asm
	{
		popad

		// call original CAutomobile::BreakTowLink
		mov eax, 0x6A4400
		jmp eax
	}
}

//-----------------------------------------------------------

ENTITY_TYPE	*_pAttachedEntity;
CObject		*_pAttachedObject;
DWORD		_dwAttachedEntityEsp;
DWORD		_dwAttachedEntityResult;

NUDE CWorld__ProcessAttachedEntities__PositionAttachedEntity_Hook()
{
	__asm
	{
		mov _dwAttachedEntityEsp, esp
		mov _pAttachedEntity, ecx

		pushad

		// call original CEntity::PositionAttachedEntity
		mov ecx, _pAttachedEntity
		mov ebx, 0x546FF0
		call ebx

		mov _dwAttachedEntityResult, eax
	}

	if (pNetGame && pNetGame->GetObjectPool())
	{
		_pAttachedObject = pNetGame->GetObjectPool()->FUNC_100129D0(_pAttachedEntity);
		if (_pAttachedObject) _pAttachedObject->FUNC_100A7E20();
	}

	__asm
	{
		popad
		mov eax, _dwAttachedEntityResult
		retn
	}}

//-----------------------------------------------------------

NUDE CRenderer__RenderEverythingBarRoads_Hook()
{
	_asm mov eax, 0x553AA0	;// CRenderer::RenderEverythingBarRoads
	_asm call eax

	if(pNetGame) {
		unnamed_1015177C = pNetGame->GetObjectPool();
		if(unnamed_1015177C) {
			unnamed_1015177C->FUNC_10012B90();
		}
	}

	if(unnamed_1014FFAC) {
		unnamed_1014FFAC->FUNC_10012B90();
	}

	_asm ret
}

//-----------------------------------------------------------

NUDE CRenderer__RenderFadingInEntities_Hook()
{
	_asm mov eax, 0x5531E0	;// CRenderer::RenderFadingInEntities
	_asm call eax

	if(pNetGame) {
		unnamed_1015177C = pNetGame->GetObjectPool();
		if(unnamed_1015177C) {
			unnamed_1015177C->FUNC_10012BD0();
		}
	}

	if(unnamed_1014FFAC) {
		unnamed_1014FFAC->FUNC_10012BD0();
	}

	_asm ret
}

//-----------------------------------------------------------

ENTITY_TYPE *_pRenderListEntity;

NUDE CRenderer__AddEntityToRenderList_Hook()
{
	_asm push esi
	_asm mov esi, [esp+8]
	_asm mov _pRenderListEntity, esi

	_asm pushad

	// keep objects out of the render list once they are past their draw distance
	if (pNetGame && _pRenderListEntity && _pRenderListEntity->vtable == 0x866F60 &&
		(_pRenderListEntity->dwProcessingFlags & 0x80000000))
	{
		_pRenderNetObject = pNetGame->FUNC_10002E10()->FUNC_100129D0(_pRenderListEntity);

		if (_pRenderNetObject && (_pRenderNetObject->field_118B != 0 ||
			_pRenderNetObject->field_54 > 300.0f || _pRenderNetObject->field_1199 != 0))
		{
			_asm popad
			_asm pop esi
			_asm retn
		}
	}

	if (unnamed_1014FFAC && _pRenderListEntity && _pRenderListEntity->vtable == 0x866F60 &&
		(_pRenderListEntity->dwProcessingFlags & 0x80000000))
	{
		_pRenderNetObject = unnamed_1014FFAC->FUNC_100129D0(_pRenderListEntity);

		if (_pRenderNetObject && (_pRenderNetObject->field_118B != 0 ||
			_pRenderNetObject->field_54 > 300.0f))
		{
			_asm popad
			_asm pop esi
			_asm retn
		}
	}

	_asm
	{
		popad

		// original code
		mov esi, _pRenderListEntity
		movsx eax, word ptr [esi+22h]
		mov edx, 0x5534B9
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE CEntity__RenderEffects__RenderRoadsignAtomic_Hook()
{
	__asm
	{
		mov eax, [esp+4]	;// arg0 (RpAtomic *)
		test eax, eax
		jz exitFn
		mov eax, 0x6FF350	;// CCustomRoadsignMgr::RenderRoadsignAtomic
		jmp eax
exitFn:
		ret
	}
}

//-----------------------------------------------------------

DWORD dwEventDamageThis;
DWORD dwEventDamagePed;

NUDE CEventDamage__AffectsPed_Hook()
{
	__asm
	{
		mov dwEventDamageThis, ecx
		mov eax, [esp+4]
		mov dwEventDamagePed, eax
		pushad
		cmp pNetGame, 0
		jz do_affects_ped
		call GamePool_FindPlayerPed
		cmp dwEventDamagePed, eax
		jnz do_affects_ped
		mov eax, pbyteCurrentPlayer
		movzx ecx, byte ptr [eax]
		test ecx, ecx
		jz do_affects_ped
		popad

		// inside a remote player context: always report the event as affecting the ped
		xor eax, eax
		mov al, 1
		retn 4
	do_affects_ped:
		popad
		sub esp, 0xC
		push esi
		mov esi, ecx
		mov eax, 0x4B35A6			// CEventDamage::AffectsPed + 6
		jmp eax
	}
}

//-----------------------------------------------------------

// the game hands two 20 byte blocks (a position plus two more floats) to
// CCollision::BuildCacheOfCameraCollision
typedef struct _CAMCOL_POINT
{
	VECTOR	vecPos;
	float	field_C;
	float	field_10;
} CAMCOL_POINT;

CAMCOL_POINT	_camColStart;
CAMCOL_POINT	_camColEnd;
BYTE			_byteCamColResult;

BYTE CCollision__BuildCacheOfCameraCollision_Hook(CAMCOL_POINT *pStart, CAMCOL_POINT *pEnd)
{
	_camColStart = *pStart;
	_camColEnd = *pEnd;

	_byteCamColResult = 0;

	__asm
	{
		push pEnd
		push pStart

		// call original CCollision::BuildCacheOfCameraCollision
		mov edx, 0x41AC40
		call edx

		mov _byteCamColResult, al

		pop edx
		pop edx
	}

	// gta only caches collision inside the map bounds, so samp entities placed
	// outside of them have to be checked by hand
	if (pEnd->vecPos.X > 3000.0f || pEnd->vecPos.X < -3000.0f ||
		pEnd->vecPos.Y > 3000.0f || pEnd->vecPos.Y < -3000.0f)
	{
		if (pNetGame && !_byteCamColResult &&
			pNetGame->GetObjectPool()->FUNC_10012DE0(&_camColStart.vecPos, &_camColEnd.vecPos))
			_byteCamColResult = 1;

		if (unnamed_1014FFAC && !_byteCamColResult &&
			unnamed_1014FFAC->FUNC_10012DE0(&_camColStart.vecPos, &_camColEnd.vecPos))
			_byteCamColResult = 1;

		if (pNetGame && !pNetGame->GetField232() && !_byteCamColResult &&
			pNetGame->GetVehiclePool()->FUNC_1001EE20(&_camColStart.vecPos, &_camColEnd.vecPos))
			_byteCamColResult = 1;
	}

	return _byteCamColResult;
}

//-----------------------------------------------------------

NUDE CCollision__CheckCameraCollisionVehicles_Hook()
{
	__asm
	{
		pushad
		cmp pNetGame, 0
		jz do_check
		mov eax, pNetGame
		movzx ecx, byte ptr [eax+0x232]		// pNetGame->field_232
		test ecx, ecx
		jz do_check
		popad
		xor eax, eax
		retn
	do_check:
		popad
		mov eax, 0x41A990					// CCollision::CheckCameraCollisionVehicles
		jmp eax
	}
}

//-----------------------------------------------------------

NUDE CWorld__CameraToIgnoreThisObject_Hook()
{
	__asm
	{
		mov eax, [esp+4]			// pEntity
		test eax, eax
		jz ignore_this_object
		mov ecx, [eax+0xFC]
		cmp ecx, 0
		jnz ignore_this_object
		mov edx, 0x563F40			// CWorld::CameraToIgnoreThisObject
		jmp edx
	ignore_this_object:
		mov eax, 1
		retn
	}
}

//-----------------------------------------------------------

// shared by the four ProcessEntityCollision hooks below
DWORD dwCollisionThis;
DWORD dwCollisionEntity;
DWORD dwCollisionRetAddr;

NUDE CAutomobile__ProcessEntityCollision_Hook()
{
	__asm
	{
		mov dwCollisionThis, ecx
		mov edx, [esp]
		mov dwCollisionRetAddr, edx
		mov edx, [esp+4]
		mov dwCollisionEntity, edx
		pushad
		cmp pNetGame, 0
		jz process_collision
		mov eax, pNetGame
		movzx ecx, byte ptr [eax+0x232]			// pNetGame->field_232
		test ecx, ecx
		jz process_collision
		cmp dwCollisionThis, 0
		jz process_collision
		cmp dwCollisionEntity, 0
		jz process_collision
		mov edx, dwCollisionEntity
		movzx eax, word ptr [edx+0x22]			// entity.nModelIndex
		cmp eax, 400
		jl process_collision
		mov ecx, dwCollisionEntity
		movzx edx, word ptr [ecx+0x22]
		cmp edx, 611							// only vehicle models pass
		jg process_collision
		mov eax, dwCollisionThis
		cmp dword ptr [eax+0x460], 0			// pVehicle->pDriver
		jz process_collision
		mov ecx, dwCollisionEntity
		cmp dword ptr [ecx+0x460], 0
		jz process_collision
		popad

		// both vehicles are being driven, so don't collide them
		mov eax, 0
		retn 8
	process_collision:
		popad
		mov edx, 0x6ACE70						// CAutomobile::ProcessEntityCollision
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE CBike__ProcessEntityCollision_Hook()
{
	__asm
	{
		mov dwCollisionThis, ecx
		mov edx, [esp]
		mov dwCollisionRetAddr, edx
		mov edx, [esp+4]
		mov dwCollisionEntity, edx
		pushad
		cmp pNetGame, 0
		jz process_collision
		mov eax, pNetGame
		movzx ecx, byte ptr [eax+0x232]			// pNetGame->field_232
		test ecx, ecx
		jz process_collision
		cmp dwCollisionThis, 0
		jz process_collision
		cmp dwCollisionEntity, 0
		jz process_collision
		mov edx, dwCollisionEntity
		movzx eax, word ptr [edx+0x22]			// entity.nModelIndex
		cmp eax, 400
		jl process_collision
		mov ecx, dwCollisionEntity
		movzx edx, word ptr [ecx+0x22]
		cmp edx, 611							// only vehicle models pass
		jg process_collision
		mov eax, dwCollisionThis
		cmp dword ptr [eax+0x460], 0			// pVehicle->pDriver
		jz process_collision
		mov ecx, dwCollisionEntity
		cmp dword ptr [ecx+0x460], 0
		jz process_collision
		popad

		// both vehicles are being driven, so don't collide them
		mov eax, 0
		retn 8
	process_collision:
		popad
		mov edx, 0x6BDEA0						// CBike::ProcessEntityCollision
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE CMonsterTruck__ProcessEntityCollision_Hook()
{
	__asm
	{
		mov dwCollisionThis, ecx
		mov edx, [esp]
		mov dwCollisionRetAddr, edx
		mov edx, [esp+4]
		mov dwCollisionEntity, edx
		pushad
		cmp pNetGame, 0
		jz process_collision
		mov eax, pNetGame
		movzx ecx, byte ptr [eax+0x232]			// pNetGame->field_232
		test ecx, ecx
		jz process_collision
		cmp dwCollisionThis, 0
		jz process_collision
		cmp dwCollisionEntity, 0
		jz process_collision
		mov edx, dwCollisionEntity
		movzx eax, word ptr [edx+0x22]			// entity.nModelIndex
		cmp eax, 400
		jl process_collision
		mov ecx, dwCollisionEntity
		movzx edx, word ptr [ecx+0x22]
		cmp edx, 611							// only vehicle models pass
		jg process_collision
		mov eax, dwCollisionThis
		cmp dword ptr [eax+0x460], 0			// pVehicle->pDriver
		jz process_collision
		mov ecx, dwCollisionEntity
		cmp dword ptr [ecx+0x460], 0
		jz process_collision
		popad

		// both vehicles are being driven, so don't collide them
		mov eax, 0
		retn 8
	process_collision:
		popad
		mov edx, 0x6C8AE0						// CMonsterTruck::ProcessEntityCollision
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE CPhysical__ProcessEntityCollision_Hook()
{
	__asm
	{
		mov dwCollisionThis, ecx
		mov edx, [esp]
		mov dwCollisionRetAddr, edx
		mov edx, [esp+4]
		mov dwCollisionEntity, edx
		pushad
		cmp pNetGame, 0
		jz process_collision
		mov eax, pNetGame
		movzx ecx, byte ptr [eax+0x232]			// pNetGame->field_232
		test ecx, ecx
		jz process_collision
		cmp dwCollisionThis, 0
		jz process_collision
		cmp dwCollisionEntity, 0
		jz process_collision
		mov edx, dwCollisionEntity
		movzx eax, word ptr [edx+0x22]			// entity.nModelIndex
		cmp eax, 400
		jl process_collision
		mov ecx, dwCollisionEntity
		movzx edx, word ptr [ecx+0x22]
		cmp edx, 611							// only vehicle models pass
		jg process_collision
		mov eax, dwCollisionThis
		cmp dword ptr [eax+0x460], 0			// pVehicle->pDriver
		jz process_collision
		mov ecx, dwCollisionEntity
		cmp dword ptr [ecx+0x460], 0
		jz process_collision
		popad

		// both vehicles are being driven, so don't collide them
		mov eax, 0
		retn 8
	process_collision:
		popad
		mov edx, 0x546D00						// CPhysical::ProcessEntityCollision
		jmp edx
	}
}

//-----------------------------------------------------------

NUDE CVehicle__UsesSiren_Hook()
{
	__asm
	{
		// the streamed in check below already answered for this vehicle
		mov eax, unnamed_1015180C
		test eax, eax
		jz usesSiren
		mov unnamed_1015180C, 0
		mov al, 1
		retn

usesSiren:
		mov unnamed_10151804, ecx
		mov edx, 0x6D8470					// CVehicle::UsesSiren
		call edx
		mov unnamed_10151808, al
	}

	if(!unnamed_10151808)
	{
		_asm pushad

		// the game doesn't know about server sided sirens, ask our own vehicle
		if(pNetGame)
		{
			CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
			if(pVehiclePool)
			{
				unnamed_10113B2C = pVehiclePool->FUNC_1001EB90(unnamed_10151804);
				if(unnamed_10113B2C != 0xFFFF)
				{
					CVehicle *pVehicle = (CVehicle *)pNetGame->GetVehiclePool()->FUNC_10001120(unnamed_10113B2C);
					if(pVehicle->FUNC_100B8330()) unnamed_10151808 = 1;
				}
			}
		}

		_asm popad
	}

	_asm mov al, unnamed_10151808
	_asm retn
}

//-----------------------------------------------------------

NUDE CAEWeatherAudioEntity__UpdateParameters_Hook()
{
	_asm mov edx, [esp+4]
	_asm mov unnamed_10151814, edx
	_asm mov edx, [esp+8]
	_asm mov unnamed_10151818, edx
	unnamed_10151810 = *(DWORD *)0xB72914;

	// mute the rain/wind audio while the outdoor ambience track is disabled
	if(pGame)
	{
		CAudio *pAudio = pGame->m_pGameAudio;
		if(pAudio->field_4) *(DWORD *)0xB72914 = 1;
	}

	_asm push unnamed_10151818
	_asm push unnamed_10151814
	_asm mov edx, 0x505A00
	_asm call edx

	*(DWORD *)0xB72914 = unnamed_10151810;
	_asm ret 8
}

//-----------------------------------------------------------

NUDE CWorld__ProcessVerticalLine_Hook()
{
	__asm
	{
		mov eax, [esp]				;// caller
		mov unnamed_10151824, eax
		cmp eax, 0x59E8F9
		jz callFn
		mov eax, [esp+0x14]
		test eax, eax
		jz callFn
		mov byte ptr [esp+0x20], 1
callFn:
		sub esp, 0x2C
		mov eax, 0x407C37	;// CWorld::ProcessVerticalLine
		jmp eax
	}
}

//-----------------------------------------------------------

NUDE CStreaming__RequestModel_Hook()
{
	__asm
	{
		cmp ebp, 0			;// model id
		jl exitFn
		push esi
		push edi
		lea edi, [ebp+ebp*4]
		mov eax, 0x4087F0	;// CStreaming::RequestModel
		jmp eax
exitFn:
		pop ebp
		pop ebx
		ret
	}
}

//-----------------------------------------------------------

// arguments of the shot being fired, shared with CWeapon__FireSniper_Hook
ENTITY_TYPE* 	pFiringEntity = 0;
VECTOR* 		pPosn = 0;
VECTOR* 		pEffectPosn = 0;
ENTITY_TYPE* 	pTargetEntity = 0;
VECTOR* 		pTarget = 0;
VECTOR* 		pPosnForDriveBy = 0;
DWORD 			dwUnknown8 = 0;
DWORD 			dwAdditionalEffects = 0;

NUDE CWeapon__FireInstantHit_Hook() 
{
	__asm
	{
		mov eax, [esp+0x4]
		mov pFiringEntity, eax
		mov eax, [esp+0x8]
		mov pPosn, eax
		mov eax, [esp+0xC]
		mov pEffectPosn, eax
		mov eax, [esp+0x10]
		mov pTargetEntity, eax
		mov eax, [esp+0x14]
		mov pTarget, eax
		mov eax, [esp+0x18]
		mov pPosnForDriveBy, eax
		mov eax, [esp+0x1C]
		mov dwUnknown8, eax
		mov eax, [esp+0x20]
		mov dwAdditionalEffects, eax

		pushad
	}

	if(pFiringEntity != (ENTITY_TYPE*)GamePool_FindPlayerPed())
	{
		__asm
		{
			popad
			retn 0x20
		}
	}

	if(pNetGame && pNetGame->GetPlayerPool())
		pNetGame->GetPlayerPool()->sub_10013C90();

	if(pGame && pGame->FindPlayerPed())
		pGame->FindPlayerPed()->FUNC_100AFA70();

	if(pNetGame && pNetGame->GetPlayerPool())
		pNetGame->GetPlayerPool()->sub_10013D10();

	_asm popad
	_asm ret 0x20
}

//-----------------------------------------------------------

extern DWORD unnamed_10118990; // 0x10118990, lives in another translation unit

// todo: implement sub_100A5410
void __stdcall sub_100A5410(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint, DWORD *pHitEntity);

VECTOR			vecShotOrigin;				// 0x10150CE4
BOOL			bLineOfSightResult = 0;		// 0x101517B0
CPlayerPed		*pShotSyncPlayer = 0;		// 0x101517CC
SHOT_SYNC_DATA	*pShotSyncData = 0;			// 0x101517D0

BOOL CWorld__ProcessLineOfSight_Hook(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
		DWORD *pHitEntity, int bCheckBuildings, int bCheckVehicles, int bCheckPeds,
		int bCheckObjects, int bCheckDummies, int bSeeThroughStuff,
		int  bIgnoreSomeObjectsForCamera, int bUnk1)
{
	// aim the ray at where the remote player actually shot from
	if(unnamed_10118990 != 2 && pShotSyncPlayer && pShotSyncPlayer != pGame->FindPlayerPed() &&
		pShotSyncData && pShotSyncData->pAttachedTo &&
		pShotSyncData->pAttachedTo->vtable != 0x863C40 &&
		pShotSyncData->pAttachedTo->mat)
	{
		if(unnamed_10118990 == 0)
		{
			FUNC_100B4D10(&vecShotOrigin, pShotSyncData->pAttachedTo->mat, &pShotSyncData->vecOffset);
		}
		else
		{
			vecShotOrigin.X = pShotSyncData->pAttachedTo->mat->pos.X + pShotSyncData->vecOffset.X;
			vecShotOrigin.Y = pShotSyncData->pAttachedTo->mat->pos.Y + pShotSyncData->vecOffset.Y;
			vecShotOrigin.Z = pShotSyncData->pAttachedTo->mat->pos.Z + pShotSyncData->vecOffset.Z;
		}

		vecLine->X = vecShotOrigin.X - vecOrigin->X + vecShotOrigin.X;
		vecLine->Y = vecShotOrigin.Y - vecOrigin->Y + vecShotOrigin.Y;
		vecLine->Z = vecShotOrigin.Z - vecOrigin->Z + vecShotOrigin.Z;
	}

	bLineOfSightResult = ProcessLineOfSight(vecOrigin, vecLine, colPoint, pHitEntity,
		bCheckBuildings, bCheckVehicles, bCheckPeds, bCheckObjects, bCheckDummies,
		bSeeThroughStuff, bIgnoreSomeObjectsForCamera, bUnk1);

	// a remote player's shot must not collide with the local player
	if(unnamed_10118990 != 2 && pShotSyncPlayer && pShotSyncPlayer != pGame->FindPlayerPed() &&
		pShotSyncData && !pShotSyncData->pAttachedTo)
	{
		if(*pHitEntity == (DWORD)GamePool_FindPlayerPed() ||
			(IN_VEHICLE(GamePool_FindPlayerPed()) &&
			*pHitEntity == GamePool_FindPlayerPed()->pVehicle))
		{
			*pHitEntity = 0;
			colPoint->X = 0.0f;
			colPoint->Y = 0.0f;
			colPoint->Z = 0.0f;
			return FALSE;
		}
	}

	if(pShotSyncPlayer && pShotSyncPlayer == pGame->FindPlayerPed())
		sub_100A5410(vecOrigin, vecLine, colPoint, pHitEntity);

	return bLineOfSightResult;
}

//-----------------------------------------------------------

SHOT_SYNC_DATA unnamed_101506F8;
MATRIX4X4 unnamed_10150990;
MATRIX4X4 unnamed_10150BC8;
ENTITY_TYPE *unnamed_101517AC;

void FUNC_100B5910(MATRIX4X4 *pOut, MATRIX4X4 *pMatrix);	// .text:100B5910, lives in another translation unit
void FUNC_100B5740(MATRIX4X4 *pOut, MATRIX4X4 *pMatrix);	// .text:100B5740, lives in another translation unit

// .text:100A5410 : records where the local player's own shot landed, in the hit
// entity's space when there is one
void __stdcall sub_100A5410(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint, DWORD *pHitEntity)
{
	memset(&unnamed_101506F8, 0, sizeof(unnamed_101506F8));

	// element wise, a VECTOR assignment would cost a scratch base register
	*(DWORD *)&unnamed_101506F8.vecOrigin.X = *(DWORD *)&vecOrigin->X;
	*(DWORD *)&unnamed_101506F8.vecOrigin.Y = *(DWORD *)&vecOrigin->Y;
	*(DWORD *)&unnamed_101506F8.vecOrigin.Z = *(DWORD *)&vecOrigin->Z;
	*(DWORD *)&unnamed_101506F8.vecColPoint.X = *(DWORD *)&colPoint->X;
	*(DWORD *)&unnamed_101506F8.vecColPoint.Y = *(DWORD *)&colPoint->Y;
	*(DWORD *)&unnamed_101506F8.vecColPoint.Z = *(DWORD *)&colPoint->Z;

	if(pHitEntity)
	{
		unnamed_101517AC = (ENTITY_TYPE *)*pHitEntity;

		if(unnamed_101517AC)
		{
			if(unnamed_101517AC->mat)
			{
				if(unnamed_10118990 == 0)
				{
					memset(&unnamed_10150990, 0, sizeof(unnamed_10150990));
					memset(&unnamed_10150BC8, 0, sizeof(unnamed_10150BC8));

					FUNC_100B5910(&unnamed_10150BC8, unnamed_101517AC->mat);
					FUNC_100B5740(&unnamed_10150990, &unnamed_10150BC8);
					FUNC_100B4D10(&unnamed_101506F8.vecOffset, &unnamed_10150990, colPoint);
				}
				else
				{
					unnamed_101506F8.vecOffset.X = colPoint->X - unnamed_101517AC->mat->pos.X;
					unnamed_101506F8.vecOffset.Y = colPoint->Y - unnamed_101517AC->mat->pos.Y;
					unnamed_101506F8.vecOffset.Z = colPoint->Z - unnamed_101517AC->mat->pos.Z;
				}

				unnamed_101506F8.pAttachedTo = unnamed_101517AC;
			}
		}
	}

	pGame->FindPlayerPed()->FUNC_100AF280(&unnamed_101506F8);
}

//-----------------------------------------------------------

NUDE CWeapon__FireSniper_Hook()
{
	_asm mov eax, [esp+0x4]
	_asm mov pFiringEntity, eax
	_asm pushad

	if(pFiringEntity != (ENTITY_TYPE*)GamePool_FindPlayerPed())
	{
		__asm
		{
			popad
			retn 0xC
		}
	}

	if(pGame && pGame->FindPlayerPed())
		pGame->FindPlayerPed()->FUNC_100AFA70();

	_asm popad
	_asm ret 0xC
}

//-----------------------------------------------------------

// bullets get a 50x velocity so the shot lands on the frame it was fired
const float fBulletVelocityScale = 50.0f;

NUDE CBulletInfo__AddBullet_Hook()
{
	__asm
	{
		push ebp
		mov ebp, esp

		fld dword ptr [ebp+0x1C]
		fmul fBulletVelocityScale
		fstp dword ptr [ebp+0x1C]
		fld dword ptr [ebp+0x20]
		fmul fBulletVelocityScale
		fstp dword ptr [ebp+0x20]
		fld dword ptr [ebp+0x24]
		fmul fBulletVelocityScale
		fstp dword ptr [ebp+0x24]

		push dword ptr [ebp+0x24]
		push dword ptr [ebp+0x20]
		push dword ptr [ebp+0x1C]
		push dword ptr [ebp+0x18]
		push dword ptr [ebp+0x14]
		push dword ptr [ebp+0x10]
		push dword ptr [ebp+0xC]
		push dword ptr [ebp+0x8]
		mov edx, 0x736010	;// CBulletInfo::AddBullet
		call edx
		add esp, 0x20

		mov edx, 0x7360D0	;// CBulletInfo::Update
		call edx

		pop ebp
		retn
	}
}

//-----------------------------------------------------------

// todo: implement sub_100A5240, it tells whether the damage came from a team mate
BOOL sub_100A5240(DWORD dwVehicle, DWORD dwInflictor, DWORD dwWeapon);

DWORD	dwDamageInflictor = 0;
DWORD	dwDamagedVehicle = 0;
DWORD	dwDamageWeapon = 0;
BOOL	bIgnoreVehicleDamage = 0;
DWORD	dwInflictDamageRetAddr = 0;

PLAYERID wInflictDamageDriver;
CPlayerPool *pInflictDamagePool;

// true when the local player drives dwVehicle and dwPed belongs to a team mate
BOOL FUNC_100A5240(DWORD dwVehicle, DWORD dwPed)
{
	if(!pNetGame || !pGame || !pGame->FindPlayerPed()
		|| !pGame->FindPlayerPed()->IsInVehicle()
		|| pGame->FindPlayerPed()->IsAPassenger()
		|| (int)pGame->FindPlayerPed()->GetGtaVehicle() != (int)dwVehicle)
		return FALSE;

	wInflictDamageDriver = FUNC_100A3AE0(dwPed);
	if(wInflictDamageDriver == 0xFFFF) return FALSE;

	pInflictDamagePool = pNetGame->GetPlayerPool();
	if(!pInflictDamagePool) return FALSE;

	if(pNetGame->GetSettings()->field_22)
	{
		BYTE byteTeam = pInflictDamagePool->m_pLocalPlayer->m_byteTeam;

		if(byteTeam == 0xFF) return FALSE;

		if((BYTE)pNetGame->GetPlayerPool()->GetAt(wInflictDamageDriver)->field_109 == byteTeam)
			return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------

NUDE CVehicle__InflictDamage_Hook()
{
	_asm mov dwDamagedVehicle, ecx
	_asm mov eax, [esp]
	_asm mov dwInflictDamageRetAddr, eax
	_asm mov eax, [esp+0x4]
	_asm mov dwDamageInflictor, eax
	_asm mov eax, [esp+0x8]
	_asm mov dwDamageWeapon, eax
	_asm pushad

	bIgnoreVehicleDamage = sub_100A5240(dwDamagedVehicle, dwDamageInflictor, dwDamageWeapon);

	_asm popad

	if(bIgnoreVehicleDamage)
	{
		_asm xor eax, eax
		_asm retn 0x18
	}

	if(iGtaVersion == 1)
	{
		_asm push 0xFFFFFFFF
		_asm mov eax, 0x404CDC
		_asm jmp eax
	}
	else
	{
		_asm push 0xFFFFFFFF
		_asm mov eax, 0x404CE3
		_asm jmp eax
	}
}

//-----------------------------------------------------------
// fps-related hook
NUDE CTimer__GetCurrentTimeInCycles_Hook()
{
	Sleep(1);

	_asm mov edx, 0x561A80
	_asm jmp edx
}

//-----------------------------------------------------------

// .text:100A23F0 : drops a 0x190 byte game frame and returns
NUDE FUNC_100A23F0()
{
	_asm add esp, 0x190
	_asm retn
}


//-----------------------------------------------------------

DWORD dwRandCaller;

NUDE Rand_Hook()
{
	_asm mov eax, [esp+0]
	_asm mov dwRandCaller, eax

	/*
	if(dwRandCaller > 0x73FB10 && dwRandCaller < 0x74132E) {
		_asm mov eax, iSyncedRandomNumber
		_asm ret
	}*/

	rand();
	_asm ret
}

//-----------------------------------------------------------
// We use a special bit (32) on dwProcFlags (+28) to indicate
// whether we should process gravity/collisions on this PlayerPed.

NUDE CPlayerPed_ProcessCollision_Hook()
{
	_asm test ecx, ecx
	_asm jnz ptr_is_ok
	_asm ret
ptr_is_ok:
	_asm mov eax, [ecx+28]
	_asm shr eax, 31
	_asm cmp eax, 1
	_asm jne do_process_cols
	_asm ret // we set top bit so don't process this
do_process_cols:
    _asm mov edx, 0x54DFB0
	_asm jmp edx
}

//-----------------------------------------------------------
DWORD dwMat;
DWORD dwMatEffects;
DWORD dwDataParam;

NUDE VehicleModel_SetEnvironmentMapHook()
{
	_asm mov eax, [esp+4]
	_asm mov dwMat, eax
	_asm mov eax, [esp+8]
	_asm mov dwDataParam, eax

	UnFuck(0x6D64F0,1);
	*(PBYTE)0x6D64F0 = 0xC3;

	/*
	_asm push dwMat
	_asm mov edx, 0x812140	// _RpMatFXMaterialGetEffects
	_asm call edx
	_asm pop edx
	_asm mov dwMatEffects, eax*/

	_asm push 0
	_asm push dwMat
	_asm mov edx, 0x811C80	// _RpMatFXMaterialSetEffects
	_asm call edx
	_asm pop edx
	_asm pop edx

	//pChatWindow->AddDebugMessage("SetEnvironmentMapCB(0x%X,0x%X,%d)",dwMat,dwDataParam,dwMatEffects);

	_asm mov edx, 0x4C8848
    _asm cmp [esp+8], 0FFFFh
	_asm jmp edx
}

//-----------------------------------------------------------

NUDE FUNC_100A5FF0()
{
	_asm mov edx, 0x550F10
	_asm call edx

	_asm ret
}

//-----------------------------------------------------------

extern char szUnk_10113968[8];	// lives in game.cpp

// .text:100A41A0 : hands the caller a copy of the eight byte version tag
bool FUNC_100A41A0(char *pszTag)
{
	memset(pszTag, 0, 8);
	memcpy(pszTag, szUnk_10113968, 8);
	return true;
}

//-----------------------------------------------------------

DWORD unnamed_1015162C;
DWORD unnamed_101506EC;
DWORD unnamed_10150C84;
DWORD unnamed_101516B0;
DWORD unnamed_101516B4;
DWORD unnamed_101516B8;
DWORD unnamed_101516C0;
DWORD unnamed_101516C4;
DWORD unnamed_10150CF0;
PED_TYPE *unnamed_10150CF8;
DWORD unnamed_10113ACC;
DWORD unnamed_10113AD0;
WORD  unnamed_10113AD4;

WORD FUNC_1009CFD0(int a1); // .text:1009CFD0, lives in another translation unit

// .text:100A4340
NUDE FUNC_100A4340()
{
	_asm mov edx, [esp]
	_asm mov unnamed_1015162C, edx
	_asm mov edx, [esp+4]
	_asm mov unnamed_101506EC, edx
	_asm mov edx, [esp+8]
	_asm mov unnamed_10150C84, edx
	_asm mov edx, [esp+12]
	_asm mov unnamed_101516B4, edx
	_asm mov edx, [esp+16]
	_asm mov unnamed_101516B8, edx
	_asm pushad

	unnamed_10150CF8 = GamePool_FindPlayerPed();
	if(unnamed_10150CF8) unnamed_10150CF0 = (DWORD)unnamed_10150CF8->entity.pdwRenderWare;
	else unnamed_10150CF0 = 0;

	_asm popad
	_asm push 0xFFFFFFFF
	_asm push 0x83BD6B
	_asm jmp unnamed_10113ACC
}

//-----------------------------------------------------------

// .text:100A43B0
NUDE FUNC_100A43B0()
{
	_asm mov edx, [esp+4]
	_asm mov unnamed_101506EC, edx
	_asm mov edx, [esp+8]
	_asm mov unnamed_101516C0, edx
	_asm mov edx, [esp+12]
	_asm mov unnamed_101516B0, edx
	_asm mov edx, [esp+16]
	_asm mov unnamed_101516B4, edx
	_asm pushad
	_asm mov eax, unnamed_101516B4
	_asm push eax
	_asm mov ecx, unnamed_101516B0
	_asm push ecx
	_asm call FUNC_100B4430
	_asm mov unnamed_101516C4, eax
	_asm mov edx, unnamed_101516C4
	_asm push edx
	_asm call FUNC_1009CFD0
	_asm add esp, 4
	_asm mov unnamed_10113AD4, ax
	_asm popad
	_asm jmp unnamed_10113AD0
}

//-----------------------------------------------------------

DWORD unnamed_101506A8;
DWORD unnamed_101514C8;
DWORD unnamed_10150C08;
DWORD unnamed_101509DC;
DWORD unnamed_10150CD8;
DWORD unnamed_10150980;
DWORD unnamed_10150A8C;

// .text:100A4D40
NUDE FUNC_100A4D40()
{
	_asm mov unnamed_101506A8, ecx
	_asm mov eax, [esp+4]
	_asm mov unnamed_101514C8, eax
	_asm mov eax, [esp+8]
	_asm mov unnamed_10150C08, eax
	_asm mov eax, [esp+12]
	_asm mov unnamed_101509DC, eax
	_asm mov eax, [esp+16]
	_asm mov unnamed_10150CD8, eax
	_asm mov eax, [esp+20]
	_asm mov unnamed_10150980, eax
	_asm mov eax, [esp+24]
	_asm mov unnamed_10150A8C, eax
	_asm pushad
	_asm popad
	_asm push unnamed_10150A8C
	_asm push unnamed_10150980
	_asm push unnamed_10150CD8
	_asm push unnamed_101509DC
	_asm push unnamed_10150C08
	_asm push unnamed_101514C8
	_asm mov ecx, unnamed_101506A8
	_asm mov eax, 0x514970
	_asm call eax
	_asm pushad
	_asm popad
	_asm retn 0x18
}

//-----------------------------------------------------------

DWORD unnamed_10151618;
DWORD unnamed_101506F0;
DWORD unnamed_101514E4;

// .text:100A2F60 : runs the call with the 0x858EF0 float forced to 2.0
NUDE FUNC_100A2F60()
{
	_asm mov unnamed_10151628, ecx
	_asm mov eax, [esp+4]
	_asm mov unnamed_10151618, eax

	UnFuck(0x858EF0, 4);
	unnamed_101506F0 = *(DWORD *)0x858EF0;
	*(DWORD *)0x858EF0 = 0x40000000;

	_asm mov ecx, unnamed_10151628
	_asm push unnamed_10151618
	_asm mov eax, 0x62A380
	_asm call eax
	_asm mov unnamed_101514E4, eax

	*(DWORD *)0x858EF0 = unnamed_101506F0;

	_asm mov eax, unnamed_101514E4
	_asm retn 4
}

//-----------------------------------------------------------

DWORD unnamed_101517C0;

// .text:100A5530 : skips the game call once we are connected
NUDE FUNC_100A5530()
{
	_asm mov unnamed_101517C0, ecx
	_asm pushad

	if(pGame && pGame->sub_100A1BC0())
	{
		_asm popad
		_asm mov ecx, unnamed_101517C0
		_asm retn
	}

	_asm mov ecx, unnamed_101517C0
	_asm mov edx, 0x60DC50
	_asm call edx
	_asm popad
	_asm mov ecx, unnamed_101517C0
	_asm retn
}

//-----------------------------------------------------------

DWORD unnamed_1015174C;
DWORD unnamed_101506CC;
DWORD unnamed_1015097C;
VECTOR unnamed_10150968;
VECTOR unnamed_10150BA8;

// .text:100A4DC0
NUDE FUNC_100A4DC0()
{
	_asm mov ebx, [esp+4]
	_asm mov unnamed_1015174C, ebx
	_asm mov ebx, [esp+8]
	_asm mov unnamed_101506CC, ebx
	_asm mov ebx, [esp+12]
	_asm mov unnamed_1015097C, ebx
	_asm pushad

	if(pGame && pGame->FindPlayerPed())
		pGame->FindPlayerPed()->FUNC_100AFC70(&unnamed_10150BA8, &unnamed_10150968);

	_asm popad
	_asm mov ebx, 0x742300
	_asm jmp ebx
}

//-----------------------------------------------------------

VECTOR *unnamed_101517C4;
DWORD unnamed_101517C8;

// .text:100A5570
NUDE FUNC_100A5570()
{
	_asm mov unnamed_101517C8, ecx
	_asm mov eax, [esp+4]
	_asm mov unnamed_101517C4, eax
	_asm pushad

	if(pNetGame && pChatWindow)
	{
		pChatWindow->AddDebugMessage("Joypad: %d LocalContext: %u UpdateCameraAim: %f %f %f",
			pGame->sub_100A1BC0(), unnamed_1015168C,
			unnamed_101517C4->X, unnamed_101517C4->Y, unnamed_101517C4->Z);
	}

	_asm mov ecx, unnamed_101517C8
	_asm push unnamed_101517C4
	_asm mov eax, 0x50CB10
	_asm call eax
	_asm popad
	_asm retn 4
}

//-----------------------------------------------------------

char unnamed_101514F0[256];

// .text:100A5EB0 : the game only has 24 bytes for a frame node name
int FUNC_100A5EB0(DWORD dwFrame, DWORD dwNameLen, DWORD a3)
{
	DWORD dwRet = 0;

	if(dwNameLen >= 0x18)
	{
		if(pGame)
		{
			_snprintf(unnamed_101514F0, 256,
				"SA-MP: FrameNode name was too long (%u). SA-MP must exit.", dwNameLen);
			ShowWindow(pGame->GetMainWindowHwnd(), SW_MINIMIZE);
			MessageBox(NULL, unnamed_101514F0, "SA-MP", MB_ICONWARNING);
		}

		_asm mov esp, 0
		_asm int 3
	}

	_asm push a3
	_asm push dwNameLen
	_asm push dwFrame
	_asm mov edx, 0x72FA50
	_asm call edx
	_asm mov dwRet, eax
	_asm pop edx
	_asm pop edx
	_asm pop edx

	return dwRet;
}

//-----------------------------------------------------------

DWORD unnamed_101515F4;
BYTE unnamed_10151614;
BYTE unnamed_1015161C;

// .text:100A3070 : runs the game call with the remote player's weapon skills in place
NUDE FUNC_100A3070()
{
	_asm mov eax, [esp+4]
	_asm mov unnamed_10151618, eax
	_asm mov unnamed_101515F4, ecx

	unnamed_10151614 = *pbyteCurrentPlayer;
	unnamed_1015161C = FindPlayerNumFromPedPtr(unnamed_10151618);

	if(unnamed_10151618 && unnamed_1015161C && !unnamed_10151614)
	{
		GameStoreLocalPlayerWeaponSkills();
		GameSetRemotePlayerWeaponSkills(unnamed_1015161C);

		_asm push unnamed_10151618
		_asm mov ecx, unnamed_101515F4
		_asm mov edx, 0x688810
		_asm call edx

		GameSetLocalPlayerWeaponSkills();
	}
	else
	{
		_asm push unnamed_10151618
		_asm mov ecx, unnamed_101515F4
		_asm mov edx, 0x688810
		_asm call edx
	}

	_asm retn 4
}

//-----------------------------------------------------------

DWORD unnamed_10150C10;
PED_TYPE *unnamed_101516FC;
DWORD unnamed_10150CE0;
DWORD unnamed_1015072C;
DWORD unnamed_10150740;
DWORD unnamed_101514BC;

// .text:100A4670 : the local player only keeps the game behaviour while the flag is set
NUDE FUNC_100A4670()
{
	_asm mov eax, [esp]
	_asm mov unnamed_10150C10, eax
	_asm mov eax, [esp+4]
	_asm mov unnamed_101516FC, eax
	_asm mov eax, [esp+8]
	_asm mov unnamed_10150CE0, eax
	_asm mov eax, [esp+12]
	_asm mov unnamed_1015072C, eax
	_asm mov eax, [esp+16]
	_asm mov unnamed_10150740, eax
	_asm mov eax, [esp+20]
	_asm mov unnamed_101514BC, eax
	_asm pushad

	if(unnamed_101516FC == GamePool_FindPlayerPed() && pNetGame &&
		pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->FUNC_100ACEF0())
	{
		_asm popad
		_asm retn 0x14
	}

	_asm popad
	_asm mov eax, 0x6A6AE0
	_asm jmp eax
}

//-----------------------------------------------------------

DWORD unnamed_10150A3C;
DWORD unnamed_101514B4;
DWORD unnamed_101509D4;
DWORD unnamed_101514D0;

// .text:100A4E20
NUDE FUNC_100A4E20()
{
	_asm mov ebx, [esp+4]
	_asm mov unnamed_1015174C, ebx
	_asm mov ebx, [esp+8]
	_asm mov unnamed_101506CC, ebx
	_asm mov ebx, [esp+12]
	_asm mov unnamed_1015097C, ebx
	_asm mov ebx, [esp+16]
	_asm mov unnamed_10150A3C, ebx
	_asm mov ebx, [esp+20]
	_asm mov unnamed_101514B4, ebx
	_asm mov ebx, [esp+24]
	_asm mov unnamed_101509D4, ebx
	_asm mov ebx, [esp+28]
	_asm mov unnamed_101514D0, ebx
	_asm pushad

	if(pGame && pGame->FindPlayerPed())
		pGame->FindPlayerPed()->FUNC_100AFC70(&unnamed_10150BA8, &unnamed_10150968);

	_asm popad
	_asm mov ebx, 0x73FB10
	_asm jmp ebx
}

//-----------------------------------------------------------

BYTE unnamed_1015181C;
void *unnamed_10151820;

int FUNC_100B1E40(int a1, void *a2, size_t a3);	// .text:100B1E40, lives in another translation unit
void FUNC_100B6AD0(void *a1, size_t a2, int a3);	// .text:100B6AD0, lives in another translation unit

// .text:100A5F30 : reads the stream into a scratch block first when the flag is set
int FUNC_100A5F30(int a1, size_t a2, int a3)
{
	DWORD dwRet = 0;

	if(unnamed_1015181C)
	{
		unnamed_10151820 = calloc(1, a2);
		FUNC_100B1E40(a1, unnamed_10151820, a2);
		FUNC_100B6AD0(unnamed_10151820, a2, a3);

		if(unnamed_10151820)
		{
			free(unnamed_10151820);
			unnamed_10151820 = NULL;
		}

		return a1;
	}

	_asm push a3
	_asm push a2
	_asm push a1
	_asm mov eax, 0x6F9FD0
	_asm call eax
	_asm mov dwRet, eax
	_asm pop eax
	_asm pop eax
	_asm pop eax

	return dwRet;
}

//-----------------------------------------------------------

DWORD unnamed_101506B0;
DWORD unnamed_101514CC;

// .text:100A2FC0 : runs the game call with the remote player's weapon skills in place
NUDE FUNC_100A2FC0()
{
	_asm mov unnamed_101515F4, ecx
	_asm mov eax, [esp+4]
	_asm mov unnamed_10151618, eax
	_asm mov eax, [esp+8]
	_asm mov unnamed_101506B0, eax
	_asm mov eax, [esp+12]
	_asm mov unnamed_101514CC, eax

	unnamed_10151614 = *pbyteCurrentPlayer;
	unnamed_1015161C = FindPlayerNumFromPedPtr(unnamed_10151618);

	if(unnamed_10151618 && unnamed_1015161C && !unnamed_10151614)
	{
		GameStoreLocalPlayerWeaponSkills();
		GameSetRemotePlayerWeaponSkills(unnamed_1015161C);

		_asm push unnamed_101514CC
		_asm push unnamed_101506B0
		_asm push unnamed_10151618
		_asm mov ecx, unnamed_101515F4
		_asm mov edx, 0x6857E0
		_asm call edx

		GameSetLocalPlayerWeaponSkills();
	}
	else
	{
		_asm push unnamed_101514CC
		_asm push unnamed_101506B0
		_asm push unnamed_10151618
		_asm mov ecx, unnamed_101515F4
		_asm mov edx, 0x6857E0
		_asm call edx
	}

	_asm retn 0xC
}

//-----------------------------------------------------------

DWORD unnamed_10151700;
CVehiclePool *unnamed_10151704;
CVehicle *unnamed_10151708;
WORD unnamed_10113AE4;

// .text:100A46F0 : mirrors our own siren state into the game's vehicle flags
NUDE FUNC_100A46F0()
{
	_asm mov unnamed_10151700, esi
	_asm pushad

	if(pNetGame)
	{
		unnamed_10151704 = pNetGame->GetVehiclePool();
		unnamed_10113AE4 = unnamed_10151704->FUNC_1001EB90(unnamed_10151700);

		if(unnamed_10113AE4 != 0xFFFF)
		{
			unnamed_10151708 = (CVehicle *)unnamed_10151704->FUNC_10001120(unnamed_10113AE4);

			if(unnamed_10151708)
			{
				if(unnamed_10151708->FUNC_100B8240())
					*(BYTE *)(unnamed_10151700 + 0x428) |= 0x40;
				else
					*(BYTE *)(unnamed_10151700 + 0x428) &= 0xBF;
			}
		}
	}

	_asm popad
	_asm mov eax, 0x6D55C0
	_asm jmp eax
}

//-----------------------------------------------------------

//-----------------------------------------------------------

DWORD unnamed_10151770;
DWORD unnamed_10151774;
DWORD unnamed_101514B0;
DWORD unnamed_10151778;
float unnamed_1015094C[3];

// the triple forced into 0xB76870 while the game runs its own check
float unnamed_10113AF4[3] = {0.0f, 0.0f, 15.0f};
// original code at 0x5543C0, and the `jz` at 0x5543DC
BYTE unnamed_10113B00[22] = {0x8B,0xCE,0xE8,0x79,0x01,0xFE,0xFF,0x84,0xC0,0x74,0xB5,
	0x8B,0xCE,0xE8,0x0E,0xB7,0x1C,0x00,0x84,0xC0,0x75,0xAA};
WORD unnamed_10113B18 = 0x4774;

// .text:100A4F60 : only one entity gets the patched check
NUDE FUNC_100A4F60()
{
	__asm
	{
		mov eax, [esp+4]
		mov edx, unnamed_10151770
		cmp eax, edx
		jz runHook
		mov edx, 0x554230
		jmp edx

runHook:
		mov edx, [esp+4]
		mov unnamed_10151774, edx
		mov edx, [esp+8]
		mov unnamed_101514B0, edx
		pushad
	}

	memcpy(unnamed_1015094C, (void *)0xB76870, 12);
	memcpy((void *)0xB76870, unnamed_10113AF4, 12);

	UnFuck(0x5543C0, 0x20);
	memset((void *)0x5543C0, 0x90, 22);
	memset((void *)0x5543DC, 0x90, 2);

	__asm
	{
		push unnamed_101514B0
		push unnamed_10151774
		mov edx, 0x554230
		call edx
		mov unnamed_10151778, eax
		pop edx
		pop edx
	}

	memcpy((void *)0xB76870, unnamed_1015094C, 12);
	memcpy((void *)0x5543C0, unnamed_10113B00, 22);
	*(WORD *)0x5543DC = unnamed_10113B18;

	__asm
	{
		popad
		mov eax, unnamed_10151778
		retn
	}
}

//-----------------------------------------------------------

DWORD unnamed_10150974;
DWORD unnamed_101514C4;
DWORD unnamed_101514C0;
DWORD unnamed_101506B4;
DWORD unnamed_10150A94;
DWORD unnamed_101514A0;
VEHICLE_TYPE *unnamed_101514E8;
MATRIX4X4 unnamed_101509E0;
MATRIX4X4 unnamed_10150C40;
DWORD unnamed_10150CDC;

void FUNC_100B5790(MATRIX4X4 *pMatrix, int iAxis, float fAngle);	// .text:100B5790, lives in another translation unit

// .text:100A47B0 : aims the drive by camera along the vehicle's own axes
NUDE FUNC_100A47B0()
{
	__asm
	{
		mov unnamed_10150974, ecx
		mov eax, [esp]
		mov unnamed_101514C4, eax
		mov eax, [esp+4]
		mov unnamed_101514C0, eax
		mov eax, [esp+8]
		mov unnamed_101506B4, eax
		mov eax, [esp+12]
		mov unnamed_10150A94, eax
		mov eax, [esp+16]
		mov unnamed_101514A0, eax
		pushad

		push unnamed_101514A0
		push unnamed_10150A94
		push unnamed_101506B4
		push unnamed_101514C0
		mov ecx, unnamed_10150974
		mov edx, 0x521500
		call edx
	}

	pGame->FindPlayerPed()->FUNC_100AEEE0(&unnamed_101509E0);

	if(pGame->FindPlayerPed()->IsInVehicle())
	{
		unnamed_101514E8 = pGame->FindPlayerPed()->GetGtaVehicle();

		if(unnamed_101514E8)
		{
			memcpy(&unnamed_10150C40, unnamed_101514E8->entity.mat, sizeof(MATRIX4X4));
			FUNC_100B5790(&unnamed_10150C40, 2, 90.0f);
			FUNC_100B5790(&unnamed_10150C40, 0, 90.0f);

			unnamed_10150CDC = (DWORD)GameGetInternalAim();
			*(DWORD *)unnamed_10150CDC = *(DWORD *)&unnamed_10150C40.right.X;
			*(float *)(unnamed_10150CDC + 4) = unnamed_10150C40.right.Y;
			*(float *)(unnamed_10150CDC + 8) = unnamed_10150C40.right.Z;
			*(float *)(unnamed_10150CDC + 0x24) = unnamed_10150C40.up.X;
			*(float *)(unnamed_10150CDC + 0x28) = unnamed_10150C40.up.Y;
			*(float *)(unnamed_10150CDC + 0x2C) = unnamed_10150C40.up.Z;
		}
	}

	*(float *)(unnamed_10150974 + 0x19C) = unnamed_101509E0.pos.X;
	*(float *)(unnamed_10150974 + 0x1A0) = unnamed_101509E0.pos.Y;
	*(float *)(unnamed_10150974 + 0x1A4) = unnamed_101509E0.pos.Z;

	__asm
	{
		popad
		retn 0x10
	}
}

//-----------------------------------------------------------
// trampolines that only snapshot the caller's arguments before handing the
// call on to GTA

DWORD unnamed_10150A20;
DWORD unnamed_10151730;
DWORD unnamed_10150C14;
DWORD unnamed_101515F8;
DWORD unnamed_101514AC;

// .text:100A4410
NUDE FUNC_100A4410()
{
	_asm mov eax, [esp]
	_asm mov unnamed_10150A20, eax
	_asm pushad
	_asm popad
	_asm mov eax, 0x4C95C0
	_asm jmp eax
}

//-----------------------------------------------------------

// .text:100A4B40
NUDE FUNC_100A4B40()
{
	_asm mov eax, [esp]
	_asm mov unnamed_1015172C, eax
	_asm mov unnamed_10151730, ecx
	_asm pushad
	_asm popad
	_asm mov eax, 0x5A2BD0
	_asm jmp eax
}

//-----------------------------------------------------------

// .text:100A4920
NUDE FUNC_100A4920()
{
	_asm mov eax, [esp+4]
	_asm mov unnamed_10150C14, eax
	_asm mov eax, [esp+8]
	_asm mov unnamed_101515F8, eax
	_asm mov eax, [esp+12]
	_asm mov unnamed_101514AC, eax
	_asm pushad
	_asm popad
	_asm mov eax, 0x40C6B0
	_asm jmp eax
}

//-----------------------------------------------------------

// .text:100A4F50
NUDE FUNC_100A4F50()
{
	_asm pushad
	_asm popad
	_asm mov eax, 0x61E3F0
	_asm jmp eax
}

//-----------------------------------------------------------

// .text:100A4F30 : only let the game run its own handler while we are not connected
NUDE FUNC_100A4F30()
{
	if(pGame->sub_100A0920())
	{
		_asm retn
	}

	_asm mov edx, 0x69EFC0
	_asm jmp edx
}

//-----------------------------------------------------------

// .text:100A4EB0
NUDE FUNC_100A4EB0()
{
	_asm
	{
		mov eax, [esp+4]
		push eax
		mov eax, 0x5618D0
		call eax
		pop edx
		pushad
	}

	if(pGame) pGame->sub_100A1C10();

	_asm
	{
		popad
		retn
	}
}

//-----------------------------------------------------------

// .text:100A5E20
NUDE FUNC_100A5E20()
{
	_asm mov edx, 0x72B850
	_asm call edx
	_asm retn
}

//-----------------------------------------------------------

inline void InstallMethodHook(	DWORD dwInstallAddress,
						DWORD dwHookFunction )
{
	DWORD oldProt, oldProt2;
	VirtualProtect((LPVOID)dwInstallAddress,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwInstallAddress = (DWORD)dwHookFunction;
	VirtualProtect((LPVOID)dwInstallAddress,4,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallHook( DWORD dwInstallAddress,
				  DWORD dwHookFunction,
				  DWORD dwHookStorage,
				  BYTE * pbyteJmpCode,
				  int iJmpCodeSize )
{
	DWORD oldProt, oldProt2;

	// Install the pointer to procaddr.
	VirtualProtect((PVOID)dwHookStorage,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;
	VirtualProtect((PVOID)dwHookStorage,4,oldProt,&oldProt2);

	// Install the Jmp code.
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,PAGE_EXECUTE_READWRITE,&oldProt);
	memcpy((PVOID)dwInstallAddress,pbyteJmpCode,iJmpCodeSize);
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,oldProt,&oldProt2);
}

//-----------------------------------------------------------

inline void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction, BYTE byteJumpCode = 0xE8)
{
	DWORD oldProt, oldProt2;
	DWORD disp = dwHookFunction - (dwInstallAddress + 5);

	VirtualProtect((LPVOID)dwInstallAddress,5,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PBYTE)(dwInstallAddress) = byteJumpCode;
	*(PDWORD)(dwInstallAddress+1) = (DWORD)disp;
	VirtualProtect((LPVOID)dwInstallAddress,5,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallGameAndGraphicsLoopHooks()
{
	UnFuck(0x53EB13,4);
	*(int *)0x53EB13 = dwGraphicsLoop - 0x53EB12 - 5; // relative addr

	InstallCallHook(0x58FC53,(DWORD)CHud__DrawRadar_Hook);

	InstallCallHook(0x58FBBF,(DWORD)CHud__DrawCrossHairs_Hook);

	InstallCallHook(0x53C104,(DWORD)CCamera__Process_Hook);

	InstallCallHook(0x53E981,(DWORD)CGame__Process_Hook);

	// For fixing fogging issues (needed for both debug and net)
	InstallMethodHook(0x86D1B0,(DWORD)CPed_Render_Hook); // This is PlayerPed
	InstallMethodHook(0x86C0F0,(DWORD)CPed_Render_Hook);
	InstallMethodHook(0x86C168,(DWORD)CPed_Render_Hook);
	InstallMethodHook(0x86C248,(DWORD)CPed_Render_Hook);
	InstallMethodHook(0x86C3A0,(DWORD)CPed_Render_Hook);

	InstallCallHook(0x53E930,(DWORD)CTimer__GetCurrentTimeInCycles_Hook);
}

//-----------------------------------------------------------

void GameInstallHooks()
{
	InstallGameAndGraphicsLoopHooks();

	InstallHook(0x4D3AA0,(DWORD)CAnimManager__AddAnimation_Hook,0x4D3934,CAnimManager__AddAnimation_Hook_HookJmpCode,sizeof(CAnimManager__AddAnimation_Hook_HookJmpCode));

	InstallHook(0x4D4610,(DWORD)CAnimManager__BlendAnimation_Hook,0x4D4609,CAnimManager__BlendAnimation_HookJmpCode,sizeof(CAnimManager__BlendAnimation_HookJmpCode));

	InstallMethodHook(0x86D190,(DWORD)CPlayerPed_ProcessControl_Hook);

	InstallMethodHook(0x86C0D0,(DWORD)CCivillianPed__ProcessControl_Hook);

	InstallMethodHook(0x86D744,(DWORD)TaskUseGun_Hook);

	InstallCallHook(0x7330A2,(DWORD)WeaponRender__GetWeaponSkill_Hook);

	InstallMethodHook(0x86D194,(DWORD)CPlayerPed_ProcessCollision_Hook);

	InstallCallHook(0x5689FD,(DWORD)CWorld__ProcessAttachedEntities_Hook);

	InstallCallHook(0x53EA03,(DWORD)CWorld__ProcessPedsAfterPreRender_Hook);

	InstallMethodHook(0x871148,(DWORD)AllVehicles_ProcessControl_Hook); // Automobile
	InstallMethodHook(0x8721C8,(DWORD)AllVehicles_ProcessControl_Hook); // boat
	InstallMethodHook(0x871388,(DWORD)AllVehicles_ProcessControl_Hook); // motorbike1
	InstallMethodHook(0x871970,(DWORD)AllVehicles_ProcessControl_Hook); // plane
	InstallMethodHook(0x8716A8,(DWORD)AllVehicles_ProcessControl_Hook); // heli
	InstallMethodHook(0x871550,(DWORD)AllVehicles_ProcessControl_Hook); // pushbike
	InstallMethodHook(0x871800,(DWORD)AllVehicles_ProcessControl_Hook); // truck
	InstallMethodHook(0x871B10,(DWORD)AllVehicles_ProcessControl_Hook); // quad
	InstallMethodHook(0x872398,(DWORD)AllVehicles_ProcessControl_Hook); // train
	InstallMethodHook(0x871C50,(DWORD)AllVehicles_ProcessControl_Hook);

	InstallCallHook(0x501B1D,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x501B42,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x501FC2,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x502067,(DWORD)VehicleHorn_Hook);
	InstallCallHook(0x5021AE,(DWORD)VehicleHorn_Hook);

	// Radar and map hooks for gang zones
	InstallCallHook(0x5869BF,(DWORD)ZoneOverlay_Hook);
	InstallCallHook(0x5759E4,(DWORD)ZoneOverlay_Hook);

	InstallCallHook(0x609A4E,(DWORD)PlayerWalk_Hook);
	InstallCallHook(0x4579C6,(DWORD)PickUpPickup_Hook, 0xE9);
	InstallCallHook(0x73C252,(DWORD)CWeapon_FireCamera_Hook, 0xE9);

	// Hook the call to CCamera::CamShake when called for sniper fire
	InstallCallHook(0x73ACE2,(DWORD)CCameraCamShake_Sniper_Hook);

	// Hook the train derailment code
	InstallCallHook(0x6F8CF8,(DWORD)CTrain_ProcessControl_Derailment);

	InstallHook(0x6402F0,(DWORD)TaskEnterVehicleDriver_Hook,
		0x6919BB,TaskEnterVehicleDriver_HookJmpCode,sizeof(TaskEnterVehicleDriver_HookJmpCode));

	InstallHook(0x63B8C0,(DWORD)TaskExitVehicle,
		0x63B8BA,TaskExitVehicle_HookJmpCode,sizeof(TaskExitVehicle_HookJmpCode));

	InstallHook(0x438576,(DWORD)CheatProcessorHook,
		0x4385AA,CheatProcessHook_JmpCode,sizeof(CheatProcessHook_JmpCode));

	InstallHook(0x584770,(DWORD)RadarTranslateColor,0x584A79,
		RadarTranslateColor_HookJmpCode,sizeof(RadarTranslateColor_HookJmpCode));

	InstallHook(0x53C900,(DWORD)CGameShutdownHook,0x53C8F1,
		CGameShutdown_HookJmpCode,sizeof(CGameShutdown_HookJmpCode));

	InstallHook(0x4B5AC0,(DWORD)PedDamage_Hook,0x4B5ABC,
		PedDamage_HookJmpCode,sizeof(PedDamage_HookJmpCode));

	// Fix for 0x004D41C5 crash
	InstallCallHook(0x4D41C0, (DWORD)AnimCrashFixHook, 0xE9);

	InstallCallHook(0x4E7427,(DWORD)PoliceScannerAudio_FindPlayerPed_Hook);

	// Fix for crash when the player who threw the satchel died
	InstallHook(0x738F3A, (DWORD)CProjectile_Update_Hook, 0x738B1B, CProjectileInfo_Update_HookJmpCode, sizeof(CProjectileInfo_Update_HookJmpCode));
	// Fix for all satchels blowing up when someone activated their satchel
	InstallHook(0x738877, (DWORD)CWeapon__Satchel__Activate_Hook, 0x73885B, CWeapon__Satchel__Activate_HookJmpCode, sizeof(CWeapon__Satchel__Activate_HookJmpCode));
	
	InstallHook(0x6A0050, (DWORD)GetText_Hook, 0x6A0043, GetText_HookJmpCode, sizeof (GetText_HookJmpCode));

	InstallCallHook(0x6FDED6,(DWORD)CCustomCarPlateMgr__CreatePlateTexture__RwRasterCreate_Hook);

	if(iGtaVersion == GTASA_VERSION_USA10)
	{
		InstallHook(0x7FB020,(DWORD)RwRasterDestroy_Hook,0x59C721,RwRasterDestroy_HookJmpCode,sizeof(RwRasterDestroy_HookJmpCode));
		unnamed_101516D4 = 0x7FB026;
	}
	else
	{
		InstallHook(0x7FB060,(DWORD)RwRasterDestroy_Hook,0x59C721,RwRasterDestroy_HookJmpCode,sizeof(RwRasterDestroy_HookJmpCode));
		unnamed_101516D4 = 0x7FB066;
	}

	InstallCallHook(0x6D0E7E,(DWORD)CVehicle__Render_Hook);

	InstallMethodHook(0x866FA8,(DWORD)CObject__Render_Hook);

	InstallCallHook(0x586C0A,(DWORD)CRadar__DrawMap__FindPlayerSpeed_Hook);

	InstallHook(0x538090,(DWORD)CFileLoader__LoadObjectInstance_Hook,0x538084,CFileLoader__LoadObjectInstance_HookJmpCode,sizeof(CFileLoader__LoadObjectInstance_HookJmpCode));

	InstallCallHook(0x718599,(DWORD)CEscalator__Update_Hook);

	InstallMethodHook(0x866F7C,(DWORD)CObject__CreateRwObject_Hook);

	InstallMethodHook(0x866F80,(DWORD)CEntity__DeleteRwObject_Hook);
	InstallMethodHook(0x8585E8,(DWORD)CEntity__DeleteRwObject_Hook);

	InstallMethodHook(0x871218,(DWORD)CAutomobile__BreakTowLink_Hook);
	InstallMethodHook(0x871778,(DWORD)CAutomobile__BreakTowLink_Hook);
	InstallMethodHook(0x8718D0,(DWORD)CAutomobile__BreakTowLink_Hook);
	InstallMethodHook(0x871A40,(DWORD)CAutomobile__BreakTowLink_Hook);
	InstallMethodHook(0x871BE0,(DWORD)CAutomobile__BreakTowLink_Hook);

	InstallCallHook(0x5648D3,(DWORD)CWorld__ProcessAttachedEntities__PositionAttachedEntity_Hook);

	InstallCallHook(0x53DFDD,(DWORD)CRenderer__RenderEverythingBarRoads_Hook);

	InstallCallHook(0x53E019,(DWORD)CRenderer__RenderFadingInEntities_Hook);

	InstallHook(0x5534B0,(DWORD)CRenderer__AddEntityToRenderList_Hook,0x5534A6,CRenderer__AddEntityToRenderList_HookJmpCode,sizeof(CRenderer__AddEntityToRenderList_HookJmpCode));

	InstallCallHook(0x5342F9,(DWORD)CEntity__RenderEffects__RenderRoadsignAtomic_Hook);

	InstallHook(0x4B35A0,(DWORD)CEventDamage__AffectsPed_Hook,0x4B3433,CEventDamage__AffectsPed_HookJmpCode,sizeof(CEventDamage__AffectsPed_HookJmpCode));

	InstallCallHook(0x41B02E,(DWORD)CCollision__BuildCacheOfCameraCollision_Hook);

	InstallCallHook(0x41AF80,(DWORD)CCollision__CheckCameraCollisionVehicles_Hook);

	InstallCallHook(0x41AB78,(DWORD)CWorld__CameraToIgnoreThisObject_Hook);

	InstallMethodHook(0x871178,(DWORD)CAutomobile__ProcessEntityCollision_Hook);
	InstallMethodHook(0x8716D8,(DWORD)CAutomobile__ProcessEntityCollision_Hook);
	InstallMethodHook(0x8719A0,(DWORD)CAutomobile__ProcessEntityCollision_Hook);
	InstallMethodHook(0x871B40,(DWORD)CAutomobile__ProcessEntityCollision_Hook);

	InstallMethodHook(0x8713B8,(DWORD)CBike__ProcessEntityCollision_Hook);
	InstallMethodHook(0x871580,(DWORD)CBike__ProcessEntityCollision_Hook);

	InstallMethodHook(0x871830,(DWORD)CMonsterTruck__ProcessEntityCollision_Hook);

	InstallMethodHook(0x8721F8,(DWORD)CPhysical__ProcessEntityCollision_Hook);

	InstallCallHook(0x6E0954,(DWORD)CVehicle__UsesSiren_Hook);
	InstallCallHook(0x6B2BCB,(DWORD)CVehicle__UsesSiren_Hook);
	InstallCallHook(0x4F77DA,(DWORD)CVehicle__UsesSiren_Hook);

	InstallMethodHook(0x872A74,(DWORD)CAEWeatherAudioEntity__UpdateParameters_Hook);

	InstallHook(0x5674E0,(DWORD)CWorld__ProcessVerticalLine_Hook,0x5674DB,CWorld__ProcessVerticalLine_HookJmpCode,sizeof(CWorld__ProcessVerticalLine_HookJmpCode));
}

//-----------------------------------------------------------

void InstallModelInfoHackHooks()
{
	InstallHook(0x4087EA,(DWORD)CStreaming__RequestModel_Hook,0x4087D7,CStreaming__RequestModel_HookJmpCode,sizeof(CStreaming__RequestModel_HookJmpCode));
}

//-----------------------------------------------------------

void InstallShotSyncHooks()
{
	InstallCallHook(0x742495,(DWORD)CWeapon__FireInstantHit_Hook);
	InstallCallHook(0x7424EC,(DWORD)CWeapon__FireInstantHit_Hook);
	InstallCallHook(0x742548,(DWORD)CWeapon__FireInstantHit_Hook);

	InstallCallHook(0x740721,(DWORD)CWorld__ProcessLineOfSight_Hook);
	InstallCallHook(0x740B69,(DWORD)CWorld__ProcessLineOfSight_Hook);
	InstallCallHook(0x736247,(DWORD)CWorld__ProcessLineOfSight_Hook);

	InstallCallHook(0x7424CB,(DWORD)CWeapon__FireSniper_Hook);

	InstallCallHook(0x73AC4B,(DWORD)CBulletInfo__AddBullet_Hook);
}

//-----------------------------------------------------------

void InstallVehicleFriendyFireHooks()
{
	InstallCallHook(0x6D7C90,(DWORD)CVehicle__InflictDamage_Hook,0xE9);
}

//-----------------------------------------------------------
