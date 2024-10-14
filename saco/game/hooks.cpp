
#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "aimstuff.h"

extern int iGtaVersion;
extern CNetGame* pNetGame;
extern CGame* pGame;
extern CChatWindow *pChatWindow;

extern DWORD dwGraphicsLoop; // Used for the external dll game loop.

#define NUDE void _declspec(naked) 

//-----------------------------------------------------------
// Globals which are used to avoid stack frame alteration
// inside the following hook procedures.

DWORD	dwFarClipHookAddr=0;
DWORD	dwFarClipReturnAddr=0;

// used generically
PED_TYPE	*_pPlayer;

BYTE	byteSavedCameraMode;
DWORD	dwCurPlayerActor=0;
BYTE	*pbyteCameraMode = (BYTE *)0xB6F1A8;
BYTE	*pbyteCurrentPlayer = (BYTE *)0xB7CD74;

PED_TYPE pedCrimeReportTemp; // pay attention! used in 0x100A1790 ; void __thiscall CGame::PlayCrimeReport as pedCrimeReportTemp 0x10150D00

int		iRadarColor1=0;
DWORD	dwSavedCheatFn=0;

float fFarClip=1400.0f;

DWORD dwParam1;
DWORD dwParam2;
DWORD dwParamThis;

DWORD unnamed_101516D4;

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

void FUNC_100A24C0()
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

NUDE CGame__Process_Hook()
{
	// TODO: CGame__Process_Hook
}

//-----------------------------------------------------------

NUDE CPed_Render_Hook()
{
	// TODO: CPed_Render_Hook
}

//-----------------------------------------------------------

NUDE CAnimManager__AddAnimation_Hook()
{
	// TODO: CAnimManager__AddAnimation_Hook
}

//-----------------------------------------------------------

NUDE CAnimManager__BlendAnimation_Hook()
{
	// TODO: CAnimManager__BlendAnimation_Hook
}

//-----------------------------------------------------------

NUDE CPlayerPed_ProcessControl_Hook()
{
	// TODO: CPlayerPed_ProcessControl_Hook
}

//-----------------------------------------------------------

NUDE CCivillianPed__ProcessControl_Hook()
{
	// TODO: CCivillianPed__ProcessControl_Hook
}

//-----------------------------------------------------------

NUDE TaskUseGun_Hook()
{
	// TODO: TaskUseGun_Hook
}

//-----------------------------------------------------------

NUDE WeaponRender__GetWeaponSkill_Hook()
{
	// TODO: WeaponRender__GetWeaponSkill_Hook
}

//-----------------------------------------------------------

NUDE CWorld__ProcessAttachedEntities_Hook()
{
	// TODO: CWorld__ProcessAttachedEntities_Hook
}

//-----------------------------------------------------------

NUDE CWorld__ProcessPedsAfterPreRender_Hook()
{
	// TODO: CWorld__ProcessPedsAfterPreRender_Hook
}

//-----------------------------------------------------------

NUDE AllVehicles_ProcessControl_Hook()
{
	// TODO: AllVehicles_ProcessControl_Hook
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
	// TODO: PickUpPickup_Hook
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

NUDE TaskEnterVehicleDriver_Hook()
{
	// TODO: TaskEnterVehicleDriver_Hook
}

//-----------------------------------------------------------

NUDE TaskExitVehicle()
{
	// TODO: TaskExitVehicle
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

NUDE PedDamage_Hook() {}

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
	// TODO: CWeapon__Satchel__Activate_Hook
}

//-----------------------------------------------------------

NUDE GetText_Hook()
{
	// TODO: GetText_Hook
}

//-----------------------------------------------------------

NUDE CCustomCarPlateMgr__CreatePlateTexture__RwRasterCreate_Hook()
{
	// TODO: CCustomCarPlateMgr__CreatePlateTexture__RwRasterCreate_Hook
}

//-----------------------------------------------------------

NUDE RwRasterDestroy_Hook()
{
	// TODO: RwRasterDestroy_Hook
}

//-----------------------------------------------------------

NUDE CVehicle__Render_Hook()
{
	// TODO: CVehicle__Render_Hook
}

//-----------------------------------------------------------

NUDE CObject__Render_Hook()
{
	// TODO: CObject__Render_Hook
}

//-----------------------------------------------------------

NUDE CRadar__DrawMap__FindPlayerSpeed_Hook()
{
	// TODO: CRadar__DrawMap__FindPlayerSpeed_Hook
}

//-----------------------------------------------------------

NUDE CFileLoader__LoadObjectInstance_Hook()
{
	// TODO: CFileLoader__LoadObjectInstance_Hook
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
	// TODO: CObject__CreateRwObject_Hook
}

//-----------------------------------------------------------

NUDE CEntity__DeleteRwObject_Hook()
{
	// TODO: CEntity__DeleteRwObject_Hook
}

//-----------------------------------------------------------

NUDE CAutomobile__BreakTowLink_Hook()
{
	// TODO: CAutomobile__BreakTowLink_Hook
}

//-----------------------------------------------------------

NUDE CWorld__ProcessAttachedEntities__PositionAttachedEntity_Hook()
{
	// TODO: CWorld__ProcessAttachedEntities__PositionAttachedEntity_Hook
}

//-----------------------------------------------------------

NUDE CRenderer__RenderEverythingBarRoads_Hook()
{
	// TODO: CRenderer__RenderEverythingBarRoads_Hook
}

//-----------------------------------------------------------

NUDE CRenderer__RenderFadingInEntities_Hook()
{
	// TODO: CRenderer__RenderFadingInEntities_Hook
}

//-----------------------------------------------------------

NUDE CRenderer__AddEntityToRenderList_Hook()
{
	// TODO: CRenderer__AddEntityToRenderList_Hook
}

//-----------------------------------------------------------

NUDE CEntity__RenderEffects__RenderRoadsignAtomic_Hook()
{
	// TODO: CEntity__RenderEffects__RenderRoadsignAtomic_Hook
}

//-----------------------------------------------------------

NUDE CEventDamage__AffectsPed_Hook()
{
	// TODO: CEventDamage__AffectsPed_Hook
}

//-----------------------------------------------------------

NUDE CCollision__BuildCacheOfCameraCollision_Hook()
{
	// TODO: CCollision__BuildCacheOfCameraCollision_Hook
}

//-----------------------------------------------------------

NUDE CCollision__CheckCameraCollisionVehicles_Hook()
{
	// TODO: CCollision__CheckCameraCollisionVehicles_Hook
}

//-----------------------------------------------------------

NUDE CWorld__CameraToIgnoreThisObject_Hook()
{
	// TODO: CWorld__CameraToIgnoreThisObject_Hook
}

//-----------------------------------------------------------

NUDE CAutomobile__ProcessEntityCollision_Hook()
{
	// TODO: CAutomobile__ProcessEntityCollision_Hook
}

//-----------------------------------------------------------

NUDE CBike__ProcessEntityCollision_Hook()
{
	// TODO: CBike__ProcessEntityCollision_Hook
}

//-----------------------------------------------------------

NUDE CMonsterTruck__ProcessEntityCollision_Hook()
{
	// TODO: CMonsterTruck__ProcessEntityCollision_Hook
}

//-----------------------------------------------------------

NUDE CPhysical__ProcessEntityCollision_Hook()
{
	// TODO: CPhysical__ProcessEntityCollision_Hook
}

//-----------------------------------------------------------

NUDE CVehicle__UsesSiren_Hook()
{
	// TODO: CVehicle__UsesSiren_Hook
}

//-----------------------------------------------------------

NUDE CAEWeatherAudioEntity__UpdateParameters_Hook()
{
	// TODO: CAEWeatherAudioEntity__UpdateParameters_Hook
}

//-----------------------------------------------------------

NUDE CWorld__ProcessVerticalLine_Hook()
{
	// TODO: CWorld__ProcessVerticalLine_Hook
}

//-----------------------------------------------------------

NUDE CStreaming__RequestModel_Hook()
{
	// TODO: CStreaming__RequestModel_Hook
}

//-----------------------------------------------------------

// todo: implement fully
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

	// todo: implement sub_10013C90
	/*if ( pNetGame && pNetGame->m_pPools->pPlayerPool )
    	sub_10013C90();*/

	_asm popad
	_asm ret 0x20
}

//-----------------------------------------------------------

NUDE CWorld__ProcessLineOfSight_Hook()
{
	// TODO: CWorld__ProcessLineOfSight_Hook
}

//-----------------------------------------------------------

NUDE CWeapon__FireSniper_Hook()
{
	// TODO: CWeapon__FireSniper_Hook
}

//-----------------------------------------------------------

NUDE CBulletInfo__AddBullet_Hook()
{
	// TODO: CBulletInfo__AddBullet_Hook
}

//-----------------------------------------------------------

NUDE CVehicle__InflictDamage_Hook()
{
	// TODO: CVehicle__InflictDamage_Hook
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

void InstallMethodHook(	DWORD dwInstallAddress,
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

void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction, BYTE byteJumpCode = 0xE8)
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
