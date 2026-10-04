

#include <windows.h>
#include <assert.h>
#define _ASSERT assert

#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "task.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern BOOL	bIgnoreNextEntry;
extern BOOL	bIgnoreNextExit;

extern BYTE	*pbyteCurrentPlayer;
extern int	iGtaVersion;

// all three live in the translation unit at 0x100A4xxx, which also reads them
extern int   VAR_10151658;
extern int   VAR_1015165C;
extern float VAR_10151660;

// .text:1009CFD0, lives in another translation unit; it returns -1 when the lookup
// fails, so the return has to be signed
short FUNC_1009CFD0(int a1);

typedef char CPlayerPed_size_probe[sizeof(CPlayerPed) == 0x32D ? 1 : -1];
typedef char CPlayerPed_2BD_probe[offsetof(CPlayerPed, field_2BD) == 0x2BD ? 1 : -1];

//-----------------------------------------------------------
// Used for instancing the local player.

CPlayerPed::CPlayerPed()
{
	m_dwGTAId = 1; // 0x001
	m_pPed = GamePool_FindPlayerPed();
	m_pEntity = (ENTITY_TYPE *)GamePool_FindPlayerPed();

	m_bytePlayerNumber = 0;
	SetPlayerPedPtrRecord(m_bytePlayerNumber,(DWORD)m_pPed);
	ScriptCommand(&set_actor_weapon_droppable,m_dwGTAId,1);
	ScriptCommand(&set_actor_can_be_decapitated,m_dwGTAId,0);

	field_2A8 = 1;
	field_2AC = 1;
	m_dwArrow = 0;
	field_2B9 = 0;
	field_2F2 = 0;
	m_iDanceState = 0;
	field_2DE = 0;
	field_2E2 = 0;
	m_iCellPhoneEnabled = 0;
	m_bGoggleState = FALSE;
	field_2C1 = 0;
	field_2C5 = 0;
	field_2C9 = 0;
	field_2F6 = 0;
	field_2F7 = 0;
	field_2F8 = 0;

	int x=0;
	while(x!=10) {
		field_4C[x] = 0;
		field_27C[x] = 0;
		memset(&field_74[x], 0, sizeof(struc_97));
		x++;
	}

	field_2F9 = 0;
}

//-----------------------------------------------------------



//-----------------------------------------------------------
// If the game has internally destroyed the ped
// during this frame, the ped pointer should become 0

void CPlayerPed::ResetPointers()
{
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pPed;
}

//-----------------------------------------------------------

void CPlayerPed::SetInitialState()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm push 0
	_asm mov ecx, dwPedPtr
	_asm mov edx, 0x60CD20 ; internal_CPlayerPed_SetInitialState
	_asm call edx
}

//-----------------------------------------------------------

void CPlayerPed::SetKeys(WORD wKeys, WORD lrAnalog, WORD udAnalog)
{
	GTA_CONTROLSET *pPlayerKeys = GameGetPlayerKeys(m_bytePlayerNumber);

	 // LEFT/RIGHT
	 if(pPlayerKeys->wKeys1[0] && lrAnalog) {
		pPlayerKeys->wKeys1[0] = lrAnalog;
		pPlayerKeys->wKeys2[0] = lrAnalog;
	 }
	 else if(lrAnalog && !pPlayerKeys->wKeys1[0]) {
		pPlayerKeys->wKeys1[0] = lrAnalog;
		pPlayerKeys->wKeys2[0] = 0;
	 }
	 else if(!lrAnalog) {
		pPlayerKeys->wKeys1[0] = 0;
		pPlayerKeys->wKeys2[0] = 0;
	 }

	 // UP/DOWN
	 if(pPlayerKeys->wKeys1[1] && udAnalog) {
		pPlayerKeys->wKeys1[1] = udAnalog;
		pPlayerKeys->wKeys2[1] = udAnalog;
	 }
	 else if(udAnalog && !pPlayerKeys->wKeys1[1]) {
		pPlayerKeys->wKeys1[1] = udAnalog;
		pPlayerKeys->wKeys2[1] = 0;
	 }
	 else if(!udAnalog) {
		pPlayerKeys->wKeys1[1] = 0;
		pPlayerKeys->wKeys2[1] = 0;
	 }

	 // SECONDARY FIRE (4)
	 if(wKeys & 1) {
		if(pPlayerKeys->wKeys1[4]) {
			pPlayerKeys->wKeys1[4] = 0xFF;
			pPlayerKeys->wKeys2[4] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[4] = 0xFF;
			pPlayerKeys->wKeys2[4] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[4] = 0x00;
		pPlayerKeys->wKeys2[4] = 0x00;
	 }

	 // CROUCHING TOGGLE (18)
	 if((m_pPed && IN_VEHICLE(m_pPed)) && (wKeys & 2)) {
		if(pPlayerKeys->wKeys1[18]) {
			pPlayerKeys->wKeys1[18] = 0xFF;
			pPlayerKeys->wKeys2[18] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[18] = 0xFF;
			pPlayerKeys->wKeys2[18] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[18] = 0x00;
		pPlayerKeys->wKeys2[18] = 0x00;
	 }

	  // FIRING (17)
	 if(wKeys & 4) {
		if(pPlayerKeys->wKeys1[17]) {
			pPlayerKeys->wKeys1[17] = 0xFF;
			pPlayerKeys->wKeys2[17] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[17] = 0xFF;
			pPlayerKeys->wKeys2[17] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[17] = 0x00;
		pPlayerKeys->wKeys2[17] = 0x00;
	 }

	  // SPRINT (16)
	 if(wKeys & 8) {
		if(pPlayerKeys->wKeys1[16]) {
			pPlayerKeys->wKeys1[16] = 0xFF;
			pPlayerKeys->wKeys2[16] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[16] = 0xFF;
			pPlayerKeys->wKeys2[16] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[16] = 0x00;
		pPlayerKeys->wKeys2[16] = 0x00;
	 }


	 // SECONDARY ONFOOT ATTACK (15)
	 if( (wKeys & 0x10) &&
		 (!m_pPed || IN_VEHICLE(m_pPed) || m_pPed->Tasks->pdwJumpJetPack == NULL ||
		  m_pPed->Tasks->pdwJumpJetPack[0] != 0x8705C4) ) {
		if(pPlayerKeys->wKeys1[15]) {
			pPlayerKeys->wKeys1[15] = 0xFF;
			pPlayerKeys->wKeys2[15] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[15] = 0xFF;
			pPlayerKeys->wKeys2[15] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[15] = 0x00;
		pPlayerKeys->wKeys2[15] = 0x00;
	 }

	 // JUMP (14)
	 if(wKeys & 0x20) {
		if(pPlayerKeys->wKeys1[14]) {
			pPlayerKeys->wKeys1[14] = 0xFF;
			pPlayerKeys->wKeys2[14] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[14] = 0xFF;
			pPlayerKeys->wKeys2[14] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[14] = 0x00;
		pPlayerKeys->wKeys2[14] = 0x00;
	 }

	 // INCAR LOOK RIGHT (7)
	 if(wKeys & 0x40) {
		if(pPlayerKeys->wKeys1[7]) {
			pPlayerKeys->wKeys1[7] = 0xFF;
			pPlayerKeys->wKeys2[7] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[7] = 0xFF;
			pPlayerKeys->wKeys2[7] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[7] = 0x00;
		pPlayerKeys->wKeys2[7] = 0x00;
	 }

	 // INCAR HANDBRAKE / ONFOOT TARGET (6)
	 if(wKeys & 0x80) {
		if(pPlayerKeys->wKeys1[6]) {
			pPlayerKeys->wKeys1[6] = 0xFF;
			pPlayerKeys->wKeys2[6] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[6] = 0xFF;
			pPlayerKeys->wKeys2[6] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[6] = 0x00;
		pPlayerKeys->wKeys2[6] = 0x00;
	 }

 	 // INCAR LOOK LEFT (5)
	 if(wKeys & 0x100) {
		if(pPlayerKeys->wKeys1[5]) {
			pPlayerKeys->wKeys1[5] = 0xFF;
			pPlayerKeys->wKeys2[5] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[5] = 0xFF;
			pPlayerKeys->wKeys2[5] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[5] = 0x00;
		pPlayerKeys->wKeys2[5] = 0x00;
	 }

	 // SUBMISSION (19)
	 if(wKeys & 0x200) {
		if(pPlayerKeys->wKeys1[19] == 0xFF) {
			pPlayerKeys->wKeys1[19] = 0xFF;
			pPlayerKeys->wKeys2[19] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[19] = 0xFF;
			pPlayerKeys->wKeys2[19] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[19] = 0x00;
		pPlayerKeys->wKeys2[19] = 0x00;
	 }

	 // WALKING (21)
	 if(wKeys & 0x400) {
		if(pPlayerKeys->wKeys1[21] == 0xFF) {
			pPlayerKeys->wKeys1[21] = 0xFF;
			pPlayerKeys->wKeys2[21] = 0xFF;
		} else {
			pPlayerKeys->wKeys1[21] = 0xFF;
			pPlayerKeys->wKeys2[21] = 0x00;
		}
	 } else {
		pPlayerKeys->wKeys1[21] = 0x00;
		pPlayerKeys->wKeys2[21] = 0x00;
	 }

	 // ANALOG2 D/U
	 BYTE byteVal = (wKeys >> 11) & 3;

	 if(byteVal==2) {
		pPlayerKeys->wKeys1[3] = (WORD)(128);
		pPlayerKeys->wKeys2[3] = (WORD)(128);
	 } else if(byteVal==1) {
		pPlayerKeys->wKeys1[3] = (WORD)(-128);
		pPlayerKeys->wKeys2[3] = (WORD)(-128);
	 } else {
		pPlayerKeys->wKeys1[3] = 0;
		pPlayerKeys->wKeys2[3] = 0;
	 }
	 // ANALOG2 L/R
	 byteVal = (BYTE)((wKeys >> 11) >> 2) & 3;

	 if(byteVal==2) {
		pPlayerKeys->wKeys1[2] = (WORD)(128);
		pPlayerKeys->wKeys2[2] = (WORD)(128);
	 } else if(byteVal==1) {
		pPlayerKeys->wKeys1[2] = (WORD)(-128);
		pPlayerKeys->wKeys2[2] = (WORD)(-128);
	 } else {
		pPlayerKeys->wKeys1[2] = 0;
		pPlayerKeys->wKeys2[2] = 0;
	 }
	 // 14-15

	 GameStoreRemotePlayerKeys(m_bytePlayerNumber,pPlayerKeys);
}

//-----------------------------------------------------------

WORD CPlayerPed::GetKeys(WORD * lrAnalog, WORD * udAnalog)
{
	WORD wRet=0;
	GTA_CONTROLSET *pInternalKeys = GameGetInternalKeys();

	*lrAnalog = pInternalKeys->wKeys1[0]; // left/right analog
	*udAnalog = pInternalKeys->wKeys1[1]; // up/down analog

	if(((short)pInternalKeys->wKeys1[2]) > 0) wRet |= 1; // analog2 L
	wRet <<= 1;

	if(((short)pInternalKeys->wKeys1[2]) < 0) wRet |= 1; // analog2 R
	wRet <<= 1;

	if(((short)pInternalKeys->wKeys1[3]) > 0) wRet |= 1; // analog2 D
	wRet <<= 1;

	if(((short)pInternalKeys->wKeys1[3]) < 0) wRet |= 1; // analog2 U
	wRet <<= 1;

	if(pInternalKeys->wKeys1[21]) wRet |= 1; // walking
	wRet <<= 1;

	if(pInternalKeys->wKeys1[19]) wRet |= 1; // submission
	wRet <<= 1;

	if((m_pPed && IN_VEHICLE(m_pPed)) && pInternalKeys->wKeys1[5]) wRet |= 1; // incar look left
	wRet <<= 1;

	if(pInternalKeys->wKeys1[6]) wRet |= 1; // incar handbrake / target
	wRet <<= 1;

	if((m_pPed && IN_VEHICLE(m_pPed)) && pInternalKeys->wKeys1[7]) wRet |= 1; // incar look right
	wRet <<= 1;

	if(pInternalKeys->wKeys1[14]) wRet |= 1; // jump
	wRet <<= 1;

	if((!m_pPed || IN_VEHICLE(m_pPed) || m_pPed->Tasks->pdwJumpJetPack == NULL ||
		m_pPed->Tasks->pdwJumpJetPack[0] != 0x8705C4) && pInternalKeys->wKeys1[15]) wRet |= 1; // secondary onfoot attack
	wRet <<= 1;

	if(pInternalKeys->wKeys1[16]) wRet |= 1; // sprint
	wRet <<= 1;

	WEAPON_SLOT_TYPE *pFireSlot;
	if((!m_pPed || (pFireSlot = &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot]) == NULL ||
		pFireSlot->dwType <= WEAPON_CANE ||
		pFireSlot->dwType == WEAPON_PARACHUTE ||
		pFireSlot->dwAmmo) && pInternalKeys->wKeys1[17]) wRet |= 1; // fire
	wRet <<= 1;

	if(pInternalKeys->wKeys1[18]) wRet |= 1; // crouch
	wRet <<= 1;

	if(pInternalKeys->wKeys1[4]) wRet |= 1; // secondary fire

	return wRet;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetSpecialKey()
{
	GTA_CONTROLSET *pInternalKeys = GameGetInternalKeys();

	if(pInternalKeys->wKeys1[11]) // yes
		return 1;
	else if(pInternalKeys->wKeys1[10]) // no
		return 2;
	else if(pInternalKeys->wKeys1[9]) // honk
		return 3;

	return 0;
}

//-----------------------------------------------------------

CAMERA_AIM * CPlayerPed::GetCurrentAim()
{
	return GameGetInternalAim();
}

//-----------------------------------------------------------

void CPlayerPed::SetCurrentAim(CAMERA_AIM *pAim)
{
	GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetCurrentWeapon()
{
	if(!m_pPed) return 0;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0;

	DWORD dwRetVal;
	ScriptCommand(&get_actor_armed_weapon,m_dwGTAId,&dwRetVal);
	return (BYTE)dwRetVal;
}

//-----------------------------------------------------------


//-----------------------------------------------------------
// Shows the normal marker

void CPlayerPed::ShowMarker(int iMarkerColorID)
{
	if (m_dwArrow) {
		ScriptCommand(&disable_marker, m_dwArrow);
		m_dwArrow = 0;
	}
	ScriptCommand(&create_arrow_above_actor, m_dwGTAId, &m_dwArrow);
	ScriptCommand(&set_marker_color, m_dwArrow, iMarkerColorID);
	ScriptCommand(&show_on_radar2, m_dwArrow, 2);
}

//-----------------------------------------------------------

void CPlayerPed::SetInterior(BYTE byteInterior, BOOL bRefresh)
{
	MATRIX4X4 mat;

	if(!m_pPed) return;

	if(!m_bytePlayerNumber) {
		ScriptCommand(&select_interior, byteInterior);
		ScriptCommand(&link_actor_to_interior, m_dwGTAId, byteInterior);
		if(bRefresh) {
			GetMatrix(&mat);
			ScriptCommand(&refresh_streaming_at, mat.pos.X, mat.pos.Y);
		}
	} else {
		ScriptCommand(&link_actor_to_interior, m_dwGTAId, byteInterior);
	}
}

//-----------------------------------------------------------

void CPlayerPed::HideMarker()
{
	if (m_dwArrow) ScriptCommand(&disable_marker, m_dwArrow);
	m_dwArrow = NULL; // Just make sure
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsOnScreen()
{
	if(m_pPed) return GameIsEntityOnScreen((DWORD *)m_pPed);
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::SetImmunities(BOOL bBullet, BOOL bFire, BOOL bExplosion, BOOL bDamage, BOOL bUnknown)
{
	if(!m_pPed) return;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return;

	ScriptCommand(&set_actor_immunities, m_dwGTAId, bBullet, bFire, bExplosion, bDamage, bUnknown);
}

//-----------------------------------------------------------

float CPlayerPed::GetHealth()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fHealth;
}

//-----------------------------------------------------------

void CPlayerPed::SetHealth(float fHealth)
{
	if(!m_pPed) return;
	m_pPed->fHealth = fHealth;
}

//-----------------------------------------------------------

float CPlayerPed::GetArmour()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fArmour;
}

//-----------------------------------------------------------

void CPlayerPed::SetArmour(float fArmour)
{
	if(!m_pPed) return;
	m_pPed->fArmour = fArmour;
}

//-----------------------------------------------------------

DWORD CPlayerPed::GetStateFlags()
{
	if(!m_pPed) return 0;
	return m_pPed->dwStateFlags;
}

//-----------------------------------------------------------

void CPlayerPed::SetStateFlags(DWORD dwState)
{
	if(!m_pPed) return;
	m_pPed->dwStateFlags = dwState;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsDead()
{
	if(!m_pPed) return TRUE;
	if(m_pPed->fHealth > 0.0f) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetActionTrigger()
{
	return (BYTE)m_pPed->dwAction;
}

//-----------------------------------------------------------

void CPlayerPed::SetActionTrigger(BYTE byteTrigger)
{
	if(!m_pPed) return;

	m_pPed->dwAction = byteTrigger;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsInVehicle()
{
	if(!m_pPed) return FALSE;

	if(IN_VEHICLE(m_pPed)) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CPlayerPed::GetTargetRotation()
{
	if(!m_pPed) return 0.0f;
	if(GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0.0f;

	MATRIX4X4 mat;
	GetMatrix(&mat);

	float fZAngle = atan2(-mat.up.X, mat.up.Y) * 180.0f / PI;
	// Bound it to [0, 360)
	if ( fZAngle < 0.0f )
		fZAngle += 360.0f;
	else if ( fZAngle >= 360.0f )
		fZAngle -= 360.0f;
	return fZAngle;
}

//-----------------------------------------------------------

void CPlayerPed::SetTargetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->fRotation2 = DegToRad(fRotation);
	m_pPed->fRotation1 = DegToRad(fRotation);
}

//-----------------------------------------------------------

void CPlayerPed::ForceTargetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->fRotation2 = DegToRad(fRotation);
	m_pPed->fRotation1 = DegToRad(fRotation);

	ScriptCommand(&set_actor_z_angle,m_dwGTAId,fRotation);   
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsAPassenger()
{
	if( m_pPed->pVehicle && IN_VEHICLE(m_pPed) )
	{
		VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

		if( pVehicle->pDriver != m_pPed || 
			pVehicle->entity.nModelIndex == TRAIN_PASSENGER ||
			pVehicle->entity.nModelIndex == TRAIN_FREIGHT ) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

VEHICLE_TYPE * CPlayerPed::GetGtaVehicle()
{
	return (VEHICLE_TYPE *)m_pPed->pVehicle;
}

//-----------------------------------------------------------

void CPlayerPed::GiveWeapon(int iWeaponID, int iAmmo)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	int iModelID = 0;
	iModelID = GameGetWeaponModelIDFromWeaponID(iWeaponID);

	if(iModelID == -1) return;

	if(!pGame->IsModelLoaded(iModelID)) {
		pGame->RequestModel(iModelID);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iModelID)) Sleep(1);
	}

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	GameStoreLocalPlayerWeaponSkills();
	GameSetRemotePlayerWeaponSkills(m_bytePlayerNumber);

	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm mov ecx, dwPedPtr
	_asm push 1
	_asm push iAmmo
	_asm push iWeaponID
	_asm mov edx, 0x5E6080
	_asm call edx

	GameSetLocalPlayerWeaponSkills();

	SetArmedWeapon(iWeaponID, false);

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::ClearAllWeapons()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	if(dwPedPtr) {
		_asm mov ecx, dwPedPtr
		_asm mov eax, 0x5E6320
		_asm call eax
	}

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::SetArmedWeapon(int iWeaponType, bool bUnk)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	GameStoreLocalPlayerWeaponSkills();
	GameSetRemotePlayerWeaponSkills(m_bytePlayerNumber);

	if((!m_pPed || !IN_VEHICLE(m_pPed)) && !bUnk)
		ScriptCommand(&set_actor_armed_weapon,m_dwGTAId,iWeaponType);
	else
	{
		DWORD dwPedPtr = (DWORD)m_pPed;
		if(dwPedPtr) {
			_asm mov ecx, dwPedPtr
			_asm push iWeaponType
			_asm mov edx, 0x5E6280
			_asm call edx
		}
	}

	GameSetLocalPlayerWeaponSkills();

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::RemoveWeaponWhenEnteringVehicle()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	if(dwPedPtr) {
		_asm mov ecx, dwPedPtr
		_asm push 0
		_asm mov edx, 0x5E6370
		_asm call edx
	}
}

//-----------------------------------------------------------

WEAPON_SLOT_TYPE * CPlayerPed::GetCurrentWeaponSlot()
{
	if(m_pPed) {
		return &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot];
	}
	return NULL;
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasAmmoForCurrentWeapon()
{
	if(m_pPed) {
		WEAPON_SLOT_TYPE * WeaponSlot = &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot];

		if(!WeaponSlot) return TRUE;

		// Melee types always have ammo.
		if( WeaponSlot->dwType <= WEAPON_CANE ||
			WeaponSlot->dwType == WEAPON_PARACHUTE ) return TRUE;

		if(!WeaponSlot->dwAmmo) return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------

float CPlayerPed::GetDistanceFromVehicle(CVehicle *pVehicle)
{
	MATRIX4X4	matFromPlayer;
	MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	pVehicle->GetMatrix(&matFromPlayer);

	fSX = (matThis.pos.X - matFromPlayer.pos.X) * (matThis.pos.X - matFromPlayer.pos.X);
	fSY = (matThis.pos.Y - matFromPlayer.pos.Y) * (matThis.pos.Y - matFromPlayer.pos.Y);
	fSZ = (matThis.pos.Z - matFromPlayer.pos.Z) * (matThis.pos.Z - matFromPlayer.pos.Z);

	return (float)sqrt(fSX + fSY + fSZ);
}

//-----------------------------------------------------------

int CPlayerPed::GetVehicleSeatID()
{
	VEHICLE_TYPE *pVehicle;

	if( (BYTE)m_pPed->dwAction == ACTION_INCAR && (pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle) != 0 ) {
		if(pVehicle->pDriver == m_pPed) return 0;
		if(pVehicle->pPassengers[0] == m_pPed) return 1;
		if(pVehicle->pPassengers[1] == m_pPed) return 2;
		if(pVehicle->pPassengers[2] == m_pPed) return 3;
		if(pVehicle->pPassengers[3] == m_pPed) return 4;
		if(pVehicle->pPassengers[4] == m_pPed) return 5;
		if(pVehicle->pPassengers[5] == m_pPed) return 6;
		if(pVehicle->pPassengers[6] == m_pPed) return 7;
	}

	return (-1);
}

//-----------------------------------------------------------

void CPlayerPed::PutDirectlyInVehicle(int iVehicleID, int iSeat)
{
	if(!m_pPed) return;
	if(!GamePool_Vehicle_GetAt(iVehicleID)) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(GetCurrentWeapon() == WEAPON_PARACHUTE) {
		SetArmedWeapon(0, false);
	}

	VEHICLE_TYPE *pVehicle = GamePool_Vehicle_GetAt(iVehicleID);

	if(pVehicle->fHealth == 0.0f) return;

	// Check to make sure internal data structure of the vehicle hasn't been deleted
	// by checking if the vtbl points to CPlaceable_vtbl
	if (pVehicle->entity.vtable == 0x863C40) return;

	if ((GetVehicleSubtypeFromVehiclePtr(pVehicle) == VEHICLE_SUBTYPE_CAR ||
		GetVehicleSubtypeFromVehiclePtr(pVehicle) == VEHICLE_SUBTYPE_BIKE) &&
		iSeat > pVehicle->byteMaxPassengers)
	{
		return;
	}

	if(iSeat==0) {
		if(pVehicle->pDriver && IN_VEHICLE(pVehicle->pDriver)) return;
		ScriptCommand(&put_actor_in_car,m_dwGTAId,iVehicleID);
	} else {
		iSeat--;
		ScriptCommand(&put_actor_in_car2,m_dwGTAId,iVehicleID,iSeat);
	}
	if(m_pPed == GamePool_FindPlayerPed() && IN_VEHICLE(m_pPed)) {
		pGame->GetCamera()->SetBehindPlayer();
	}

	if(pNetGame) {
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		VEHICLEID TrainVehicleId = pVehiclePool->FUNC_1001EB90((int)pVehicle);
		// 0xFFFF is the vehicle pool not-found sentinel
		if(TrainVehicleId == 0xFFFF || TrainVehicleId > MAX_VEHICLES) return;

		CVehicle* pTrain = (CVehicle *)pVehiclePool->FUNC_10001120(TrainVehicleId);
		if ( pTrain && pTrain->IsATrainPart() && m_pPed == GamePool_FindPlayerPed() ) {
			ScriptCommand(&camera_on_vehicle, pTrain->m_dwGTAId, 3, 2);
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::EnterVehicle(int iVehicleID, BOOL bPassenger)
{
	if(!m_pPed) return;
	VEHICLE_TYPE *ThisVehicleType;
	if((ThisVehicleType = GamePool_Vehicle_GetAt(iVehicleID)) == NULL) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	bIgnoreNextEntry = TRUE;

	if(GetCurrentWeapon() == WEAPON_PARACHUTE) {
		SetArmedWeapon(0, false);
	}

	if(bPassenger) {
		if(ThisVehicleType->entity.nModelIndex == TRAIN_PASSENGER && (m_pPed == GamePool_FindPlayerPed())) {
			ScriptCommand(&put_actor_in_car2,m_dwGTAId,iVehicleID,-1);
		} else {
			ScriptCommand(&send_actor_to_car_passenger,m_dwGTAId,iVehicleID,3000,-1);
		}
	} else {
		ScriptCommand(&send_actor_to_car_driverseat,m_dwGTAId,iVehicleID,3000);
	}
}

//-----------------------------------------------------------
// Graceful vehicle exit.

void CPlayerPed::ExitCurrentVehicle()
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(IN_VEHICLE(m_pPed)) {
		if(GetCurrentVehicleID()) {
			VEHICLE_TYPE *pVehicle = GamePool_Vehicle_GetAt(GetCurrentVehicleID());
			if(pVehicle) {
				if( pVehicle->entity.nModelIndex != TRAIN_PASSENGER &&
					pVehicle->entity.nModelIndex != TRAIN_PASSENGER_LOCO ) {
					ScriptCommand(&make_actor_leave_car,m_dwGTAId,GetCurrentVehicleID());
				}
			}
		}
	}
}

//-----------------------------------------------------------
// Forceful removal

void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(m_pPed && IN_VEHICLE(m_pPed)) {
		ScriptCommand(&remove_actor_from_car_and_put_at,m_dwGTAId,fX,fY,fZ);
	}
}

//-----------------------------------------------------------

void CPlayerPed::TogglePlayerControllable(int iControllable)
{
	MATRIX4X4 mat;

	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(!iControllable) {
		ScriptCommand(&toggle_player_controllable,m_bytePlayerNumber,0);
		ScriptCommand(&lock_actor,m_dwGTAId,1);
	} else {
		ScriptCommand(&toggle_player_controllable,m_bytePlayerNumber,1);
		ScriptCommand(&lock_actor,m_dwGTAId,0);
		// retail writes the IsInVehicle test out inline
		if(!(m_pPed && IN_VEHICLE(m_pPed))) {
			GetMatrix(&mat);
			TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::HandsUp()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return;
	if(!IsAdded()) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	ScriptCommand(&actor_task_handsup,m_dwGTAId,-2);
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasHandsUp()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return FALSE;
	if(!IsAdded()) return FALSE;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return FALSE;
	if(m_pPed->Tasks->pdwJumpJetPack == NULL) return FALSE;
	DWORD dwJmpVtbl = m_pPed->Tasks->pdwJumpJetPack[0];
	if(dwJmpVtbl == 0x85A29C) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::HoldItem(int iObject)
{
	if(!m_pPed) return;
	if(!IsAdded()) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	DWORD dwPed = (DWORD)m_pPed;
	_asm push 1
	_asm push iObject
	_asm mov ecx, dwPed
	_asm mov ebx, 0x5E4390
	_asm call ebx
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsJumping()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwJumpJetPack)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwJumpJetPack) == 211;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsFighting()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwFighting)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwFighting) == 1016;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsFalling()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwDamage)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwDamage) == 208;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsSwimming()
{
	if(m_pPed && !IN_VEHICLE(m_pPed) && m_pPed->Tasks->pdwSwimWasted)
	{
		return GetTaskTypeFromTask(m_pPed->Tasks->pdwSwimWasted) == 268;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::StartJetpack()
{
	if(!m_pPed) return;

	*pbyteCurrentPlayer = m_bytePlayerNumber;

	// reset CTasks so the CJetPack task priority can be enforced
	TeleportTo(m_pPed->entity.mat->pos.X, m_pPed->entity.mat->pos.Y, m_pPed->entity.mat->pos.Z);

	_asm mov eax, 0x439600
	_asm call eax

	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------

void CPlayerPed::StopJetpack()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return;

	if(m_pPed->Tasks->pdwJumpJetPack == NULL) return;

	DWORD dwJmpVtbl = m_pPed->Tasks->pdwJumpJetPack[0];

	if(dwJmpVtbl == 0x8705C4) {
		DWORD dwJetPackTask = (DWORD)m_pPed->Tasks->pdwJumpJetPack;
		_asm mov ecx, dwJetPackTask
		_asm mov edx, 0x6801D0
		_asm push 1
		_asm call edx
		m_pPed->Tasks->pdwJumpJetPack = 0;
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsInJetpackMode()
{
	if(!m_pPed || IN_VEHICLE(m_pPed)) return FALSE;
	if(m_pPed->Tasks->pdwJumpJetPack == NULL) return FALSE;

	DWORD dwJmpVtbl = m_pPed->Tasks->pdwJumpJetPack[0];

	if(dwJmpVtbl == 0x8705C4) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::StartGoggles()
{
	// retail writes the HasGoggles test out inline
	if (m_pPed && (m_pPed->dwActiveVision != 0 || m_bGoggleState)) return;
	if (FindWeaponSlot( 44 ) == NULL && FindWeaponSlot( 45 ) == NULL)
		GiveWeapon( 44, 1 ); // Prevents crashing due to lack of animations.

	CTaskGoggles* pGoggles = new CTaskGoggles();
	pGoggles->ApplyToPed( this );
	m_bGoggleState = TRUE;
}

//-----------------------------------------------------------

void CPlayerPed::StopGoggles()
{
	if (!m_pPed) return;

	// retail writes the HasGoggles test out inline
	if (m_pPed->dwActiveVision != 0 || m_bGoggleState) {
		m_bGoggleState = FALSE;
		DWORD dwPedPointer = (DWORD)m_pPed;
		_asm mov ecx, dwPedPointer
		_asm mov eax, 0x5E6010
		_asm call eax
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasGoggles()
{
	if (!m_pPed) return FALSE;
	return (BOOL)(m_pPed->dwActiveVision != 0 || m_bGoggleState);
}

//-----------------------------------------------------------

WORD CPlayerPed::GetAmmo()
{
	if(m_pPed) {
		WEAPON_SLOT_TYPE * WeaponSlot = &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot];

		if(!WeaponSlot) return -1;

		// Melee types always have ammo.
		if( WeaponSlot->dwType <= WEAPON_CANE ||
			WeaponSlot->dwType == WEAPON_PARACHUTE ) return -1;

		return (WORD)WeaponSlot->dwAmmo;
	}
	return 0;
}

//-----------------------------------------------------------

WEAPON_SLOT_TYPE * CPlayerPed::FindWeaponSlot(DWORD dwWeapon)
{
	if (m_pPed)
	{
		BYTE i;
		for (i = 0; i < 13; i++)
		{
			if (m_pPed->WeaponSlots[i].dwType == dwWeapon) return &m_pPed->WeaponSlots[i];
		}
	}
	return NULL;
}

//-----------------------------------------------------------

void CPlayerPed::SetAnimationSet(PCHAR szAnim)
{
	if(m_pPed) {
		ScriptCommand(&set_actor_animation_set,m_dwGTAId,szAnim);
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetMoney(int iAmount)
{
	ScriptCommand(&set_actor_money,m_dwGTAId,0);
	ScriptCommand(&set_actor_money,m_dwGTAId,iAmount);
}

//-----------------------------------------------------------

void CPlayerPed::ApplyAnimation( char *szAnimName, char *szAnimFile, float fT,
								 int opt1, int opt2, int opt3, int opt4, int iUnk )
{
	int iWaitAnimLoad=0;

	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	// Can't allow 'naughty' anims!
	if( !stricmp(szAnimFile,"SEX") )
		return;

	if (!pGame->IsAnimationLoaded(szAnimFile)) {
		pGame->RequestAnimation(szAnimFile);
		while(!pGame->IsAnimationLoaded(szAnimFile)) {
			Sleep(1);
			iWaitAnimLoad++;
			if(iWaitAnimLoad == 15) return; // we can't wait forever
		}		
	}

	ScriptCommand(&apply_animation,m_dwGTAId,szAnimName,szAnimFile,fT,opt1,opt2,opt3,opt4,iUnk);
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsPerformingAnimation(char *szAnimName)
{
	if(m_pPed && ScriptCommand(&is_actor_performing_anim,m_dwGTAId,szAnimName)) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CPlayerPed::GetAimZ()
{
	if(m_pPed) {
		DWORD dwPlayerInfo = m_pPed->dwPlayerInfoOffset;

		float fAimZ;
		_asm mov eax, dwPlayerInfo
		_asm mov ebx, [eax+84]
		_asm mov fAimZ, ebx

		return fAimZ;
	}
	return 0.0f;
}

//-----------------------------------------------------------

void CPlayerPed::SetAimZ(float fAimZ)
{
	if(_isnan(fAimZ) || (fAimZ > 100.0f || fAimZ < -100.0f)) return;

	if(m_pPed) {
		DWORD dwPlayerInfo = m_pPed->dwPlayerInfoOffset;
		_asm mov eax, dwPlayerInfo
		_asm mov ebx, fAimZ
		_asm mov [eax+84], ebx
	}
}

//-----------------------------------------------------------

ENTITY_TYPE* CPlayerPed::GetGtaContactEntity()
{
	return (ENTITY_TYPE*)m_pPed->pContactEntity;
}

//-----------------------------------------------------------

VEHICLE_TYPE* CPlayerPed::GetGtaContactVehicle()
{
	return (VEHICLE_TYPE*)m_pPed->pContactVehicle;
}

//-----------------------------------------------------------

char DanceStyleLibs[4][16] = {"WOP","GFUNK","RUNNINGMAN","STRIP"};
char DanceIdleLoops[4][16] = {"DANCE_LOOP","DANCE_LOOP","DANCE_LOOP","STR_Loop_B"};

void CPlayerPed::StartDancing(int iStyle)
{
	if(iStyle < 0 || iStyle > 3) return;

	m_iDanceState = 1;
	m_iDanceStyle = iStyle;

	if(m_bytePlayerNumber == 0) {
		ApplyAnimation(DanceIdleLoops[m_iDanceStyle],DanceStyleLibs[m_iDanceStyle],16.0,1,0,0,0,-1);
	}
}

//-----------------------------------------------------------

void CPlayerPed::StopDancing()
{
	m_iDanceState = 0;
	MATRIX4X4 mat;
	GetMatrix(&mat);
	TeleportTo(mat.pos.X,mat.pos.Y,mat.pos.Z);
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsDancing()
{
	if(m_iDanceState) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::SetSkin(int iSkin)
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	if(!IsValidPedModel(iSkin)) iSkin = 0;

	if(dwPedPtr) {
		// the skin swap trips an assert inside the streamer, so ret it out first
		UnFuck(0x5A82C0, 1);
		*(BYTE *)0x5A82C0 = 0xC3;

		DestroyFollowPedTask();
		SetModelIndex(iSkin);

		_asm mov eax, dwPedPtr
		_asm lea ecx, [eax+0x294]
		_asm push eax
		_asm mov ebx, 0x4E68D0
		_asm call ebx
	}
}

//-----------------------------------------------------------

CPlayerPed::~CPlayerPed()
{
	Destroy();
}

//-----------------------------------------------------------

// kills the ped after unwinding the jetpack and the attach slots
void CPlayerPed::FUNC_100AFFD0()
{
	MATRIX4X4 mat;

	if(!m_dwGTAId) return;
	if(!m_pPed) return;

	if(!IN_VEHICLE(m_pPed)) {
		DWORD *pTask = m_pPed->Tasks->pdwJumpJetPack;
		if(pTask && pTask[0] == 0x8705C4) StopJetpack();
	}

	FUNC_100AD0F0();

	GetMatrix(&mat);
	TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);

	*(DWORD *)((DWORD)m_pPed + 0x540) = 0;

	*pbyteCurrentPlayer = m_bytePlayerNumber;
	ScriptCommand(&kill_actor, m_dwGTAId);
	*pbyteCurrentPlayer = 0;
}

//-----------------------------------------------------------
// only referenced by ProcessDrunkCamera; rate limits the steering drift
DWORD dwLastDrunkTick;

void CPlayerPed::ProcessDrunkCamera()
{
	float fDrift = 0.0f;

	if(field_2C9 > 2000 && !m_bytePlayerNumber) {
		int iSeverity = (int)(field_2C9 * 0.02f);
		if(iSeverity > 250) iSeverity = 250;
		else if(iSeverity < 5) iSeverity = 0;

		ScriptCommand(&set_player_drunk_visuals, 0, iSeverity);

		if(field_2C9 > 2000 && m_pPed && IN_VEHICLE(m_pPed) && !IsAPassenger()) {
			VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
			if(pVehicle) {
				if(!dwLastDrunkTick || (GetTickCount() - dwLastDrunkTick) > 200) {
					int iRand = rand() % 40;
					if(iRand >= 20) {
						if(iRand > 30) {
							if(field_2C9 < 5000) fDrift = 0.012f;
							else fDrift = 0.015f;
						} else {
							fDrift = -0.012f;
							if(field_2C9 >= 5000) fDrift = -0.015f;
						}
					}

					if(FloatOffset(pVehicle->entity.vecMoveSpeed.X, 0.0f) > 0.05f ||
						FloatOffset(pVehicle->entity.vecMoveSpeed.Y, 0.0f) > 0.05f)
					{
						pVehicle->entity.vecTurnSpeed.Z += fDrift;
					}

					dwLastDrunkTick = GetTickCount();
				}
			}
		}

		field_2C9--;
		return;
	}

	if(field_2C9 <= 2000 && !m_bytePlayerNumber) {
		if(field_2C9 > 0) field_2C9--;
		ScriptCommand(&set_player_drunk_visuals, 0, 0);
	}
}

//-----------------------------------------------------------

void CPlayerPed::FUNC_100AD0F0()
{
	if(!m_pPed) return;

	DWORD dwPtr = *(DWORD *)((DWORD)m_pPed + 0x730);
	if(dwPtr) {
		_asm mov ecx, dwPtr
		_asm mov eax, 0x5393F0
		_asm call eax
	}

	if(IN_VEHICLE(m_pPed)) {
		VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
		if(pVehicle) {
			dwPtr = *(DWORD *)((DWORD)pVehicle + 0x490);
			if(dwPtr) {
				_asm mov ecx, dwPtr
				_asm mov eax, 0x5393F0
				_asm call eax
			}
		}
	}
}

//-----------------------------------------------------------

// 2 = entering as driver, 1 = entering as passenger, 0 = not entering
BOOL CPlayerPed::FUNC_100AC5D0()
{
	if(GamePool_Ped_GetAt(m_dwGTAId) && m_pPed && m_pPed->Tasks &&
		m_pPed->Tasks->pdwJumpJetPack)
	{
		int iTask = GetTaskTypeFromTask(m_pPed->Tasks->pdwJumpJetPack);
		if(iTask == 700 || iTask == 712) return 2;
		if(iTask == 701 || iTask == 713) return 1;
	}
	return 0;
}

//-----------------------------------------------------------

// TRUE while the ped is running the leave-car task
BOOL CPlayerPed::FUNC_100AC640()
{
	if(GamePool_Ped_GetAt(m_dwGTAId) && m_pPed && m_pPed->Tasks &&
		m_pPed->Tasks->pdwJumpJetPack &&
		GetTaskTypeFromTask(m_pPed->Tasks->pdwJumpJetPack) == 704) return TRUE;

	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::SetWeaponAmmo(BYTE byteWeapon, WORD wAmmo)
{
	if(!m_pPed) return;

	WEAPON_SLOT_TYPE *pSlot = FindWeaponSlot(byteWeapon);
	if(pSlot) pSlot->dwAmmo = wAmmo;
}

//-----------------------------------------------------------

void CPlayerPed::Destroy()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	GameResetPlayerKeys(m_bytePlayerNumber);
	SetPlayerPedPtrRecord(m_bytePlayerNumber, 0);

	if(m_pPed && GamePool_Ped_GetAt(m_dwGTAId) && m_pPed->entity.vtable != 0x863C40) {
		if(field_2B9) {
			ScriptCommand(&disassociate_object, field_2B9, 0.0f, 0.0f, 0.0f, 0);
			ScriptCommand(&destroy_object_with_fade, field_2B9);
			field_2B9 = 0;
		}

		if(HasObjectAttached()) RemoveAllAttachedObjects();
		if(field_2C1) StopCarrying();

		if(IN_VEHICLE(m_pPed)) RemoveFromVehicleAndPutAt(100.0f, 100.0f, 10.0f);

		*(DWORD *)(*(DWORD *)(dwPedPtr + 1152) + 76) = 0;

		_asm mov ecx, dwPedPtr
		_asm push 1
		_asm mov eax, [ecx]
		_asm call dword ptr [eax]

		m_pPed = NULL;
		m_pEntity = NULL;
	} else {
		m_pPed = NULL;
		m_pEntity = NULL;
		m_dwGTAId = 0;
	}
}

//-----------------------------------------------------------

void CPlayerPed::RemoveAllAttachedObjects()
{
	// retail spells the RemoveAttachedObject body out here
	for(int x = 0; x < 10; x++) {
		if(x >= 0 && x < 10 && field_4C[x] == 1) {
			CEntity *pObject = (CEntity *)field_27C[x];
			if(pObject) {
				delete pObject;
				field_27C[x] = 0;
			}
			memset(&field_74[x], 0, sizeof(struc_97));
			field_4C[x] = 0;
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::StopCarrying()
{
	MATRIX4X4 mat;

	if(field_2C5) {
		ScriptCommand(&task_pick_up_object, m_dwGTAId, field_2C5, 0.0f, 0.0f, 0.0f, 6, 16, "NULL", "NULL", 0);
		field_2C5 = 0;
	}

	GetMatrix(&mat);
	TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);
	field_2C1 = 0;
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasObjectAttached()
{
	for(int x = 0; x < 10; x++) {
		if(field_4C[x] == 1) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::RemoveAttachedObject(int iIndex)
{
	if(iIndex >= 0 && iIndex < 10 && field_4C[iIndex] == 1) {
		CEntity *pObject = (CEntity *)field_27C[iIndex];
		if(pObject) {
			delete pObject;
			field_27C[iIndex] = 0;
		}
		memset(&field_74[iIndex], 0, sizeof(struc_97));
		field_4C[iIndex] = 0;
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::GetAttachedObjectSlotState(int iIndex)
{
	if(iIndex < 0 || iIndex >= 10) return FALSE;
	return (field_4C[iIndex] == 1);
}

//-----------------------------------------------------------
// drunk level, driven from the net layer and read by the blur overlay

int CPlayerPed::sub_100ADFA0()
{
	return field_2C9;
}

//-----------------------------------------------------------

void CPlayerPed::FUNC_100ADFB0(int iLevel)
{
	field_2C9 = iLevel;
}

//-----------------------------------------------------------

int CPlayerPed::FUNC_100ADC90()
{
	return field_2C1;
}

//-----------------------------------------------------------

void CPlayerPed::ProcessMarkers(BOOL bMarkerStreamingEnabled, float fMarkerStreamRadius, BOOL bVisible)
{
	if(!m_pPed) return;
}

//-----------------------------------------------------------

void CPlayerPed::ApplyCommandTask(char *szTaskName, int p1, int p2, int p3,
								  VECTOR *p4, int p5, float p6, int p7, int p8, int p9)
{
	DWORD dwPed = (DWORD)m_pPed;
	if(!dwPed) return;

	_asm push p9
	_asm push p8
	_asm push p7
	_asm push p6
	_asm push p5
	_asm push p4
	_asm push p3
	_asm push p2
	_asm push p1
	_asm push dwPed
	_asm push szTaskName
	_asm mov ecx, 0xC15448
	_asm mov edx, 0x618970
	_asm call edx
}

//-----------------------------------------------------------

DWORD dwExt4;

void CPlayerPed::DestroyFollowPedTask()
{
	if(!m_pPed) return;
	dwExt4 = (DWORD)m_pPed->Tasks->pdwIK;
	if(!dwExt4) return;

	_asm mov ecx, dwExt4
	_asm mov edx, 0x639330
	_asm push 1
	_asm call edx

	m_pPed->Tasks->pdwIK = 0;
}

//-----------------------------------------------------------

void CPlayerPed::ToggleCellphone(int iOn)
{
	if(!m_pPed) return;
	m_iCellPhoneEnabled = iOn;
	ScriptCommand(&toggle_actor_cellphone,m_dwGTAId,iOn);
}

//-----------------------------------------------------------

int CPlayerPed::IsCellphoneEnabled()
{
    return m_iCellPhoneEnabled;
}

//-----------------------------------------------------------

int CPlayerPed::GetFightingStyle()
{
	if (!m_pPed) return 0;
	return m_pPed->byteFightingStyle;
}

//-----------------------------------------------------------

void CPlayerPed::SetFightingStyle(int iStyle)
{
	if (!m_pPed) return;

	ScriptCommand( &set_fighting_style, m_dwGTAId, iStyle, 6 );
}

//-----------------------------------------------------------

void CPlayerPed::ProcessVehicleHorn()
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	GTA_CONTROLSET *pPlayerControls;

	if(!m_bytePlayerNumber) {
		pPlayerControls = GameGetInternalKeys();
	} else {
		pPlayerControls = GameGetPlayerKeys(m_bytePlayerNumber);
	}

	VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;

	if(pGtaVehicle) {
		if(IN_VEHICLE(m_pPed) && (pGtaVehicle->pDriver == m_pPed)) {
			if(pPlayerControls->wKeys1[18]) {
				pGtaVehicle->byteHorn2 = 1;
			} else {
				pGtaVehicle->byteHorn2 = 0;
			}
		}
	}
}

//-----------------------------------------------------------

PCHAR CPlayerPed::GetLoadedShopName()
{
	if(!m_pPed) return NULL;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return NULL;

	return (PCHAR)0xA9A7D8;
}

//-----------------------------------------------------------

void CPlayerPed::LoadShoppingDataSubsection(PCHAR szSectionName)
{
	if(strlen(szSectionName) > 8) return;

	_asm push szSectionName
	_asm mov edx, 0x49BBE0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

PCHAR CPlayerPed::GetShopName()
{
	if(!m_pPed) return NULL;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return NULL;

	DWORD dwPedPtr = (DWORD)m_pPed;
	PCHAR szResult = NULL;

	_asm mov eax, dwPedPtr
	_asm mov ebx, [eax+1932]
	_asm mov szResult, ebx

	return szResult;
}

//-----------------------------------------------------------

CHAR szShopName[32];

void CPlayerPed::SetShopName(char *szNewShopName)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	DWORD dwPedPtr = (DWORD)m_pPed;

	if(szNewShopName && strlen(szNewShopName))
	{
		if(strlen(szNewShopName) <= 31)
		{
			memset(szShopName, 0, sizeof(szShopName));
			strncpy(szShopName, szNewShopName, sizeof(szShopName));

			_asm mov eax, dwPedPtr
			_asm mov ebx, offset szShopName
			_asm mov [eax+1932], ebx
		}
	}
	else
	{
		_asm mov eax, dwPedPtr
		_asm mov byte ptr [eax+1932], 0
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetWeaponSkillLevel(int iSkill, WORD wLevel)
{
	if(m_bytePlayerNumber == 0)
		GameSetLocalPlayerWeaponSkillLevel(iSkill, wLevel);
	else
		GameSetPlayerWeaponSkillLevel(m_bytePlayerNumber, iSkill, wLevel);
}

//-----------------------------------------------------------

VECTOR* CPlayerPed::GetBonePosition(int iBone, VECTOR *vecPos)
{
	if(!m_pPed) return NULL;
	if(m_pPed->entity.vtable == 0x863C40) return NULL;

	DWORD dwPedPtr = (DWORD)m_pPed;
	VECTOR* vecResult;

	_asm push 0
	_asm push iBone
	_asm push vecPos
	_asm mov ecx, dwPedPtr
	_asm mov edx, 0x5E4280 ; CPed__GetBonePosition
	_asm call edx
	_asm mov vecResult, eax

	return vecResult;
}

//-----------------------------------------------------------

VECTOR* CPlayerPed::GetTransformedBonePosition(int iBone, VECTOR *vecOffset)
{
	if(!m_pPed) return NULL;
	if(m_pPed->entity.vtable == 0x863C40) return NULL;

	DWORD dwPedPtr = (DWORD)m_pPed;
	VECTOR* vecResult;

	_asm push 0
	_asm push iBone
	_asm push vecOffset
	_asm mov ecx, dwPedPtr
	_asm mov edx, 0x5E01C0 ; CPed__GetTransformedBonePosition
	_asm call edx
	_asm mov vecResult, eax

	return vecResult;
}

//-----------------------------------------------------------

int CPlayerPed::FUNC_100AE260()
{
	return field_2F2;
}

//-----------------------------------------------------------

BOOL CPlayerPed::FUNC_100ACE40()
{
	return field_2E2 != 0;
}

//-----------------------------------------------------------

BOOL CPlayerPed::FUNC_100ACEF0()
{
	return field_2DE != 0;
}

//-----------------------------------------------------------

void CPlayerPed::FUNC_100ACF00(int a1)
{
	field_2DE = a1;
	field_2F8 = (a1 != 0);
}

//-----------------------------------------------------------

BOOL CPlayerPed::FUNC_100AD860()
{
	if(m_pPed && (m_pPed->dwStateFlags & 3)) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::FUNC_100AD880()
{
	if(!m_pPed) return;

	*(DWORD *)((DWORD)m_pPed + 0x764) = 0;
	*(DWORD *)((DWORD)m_pPed + 0x760) = 0xFF;
}

//-----------------------------------------------------------

void CPlayerPed::FUNC_100AC720()
{
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(!m_pPed) return;

	m_pPed->dwStateFlags &= ~0x4000000;
}

//-----------------------------------------------------------

void CPlayerPed::FUNC_100AEC90()
{
	DWORD dwPedPtr = (DWORD)m_pPed;
	if(!dwPedPtr) return;

	_asm mov ecx, dwPedPtr
	_asm mov eax, 0x532B20
	_asm call eax
}

//-----------------------------------------------------------

int CPlayerPed::FUNC_100AEF60()
{
	if(!m_pPed) return 0;
	if(m_pPed->entity.vtable == 0x863C40) return 0;
	if(!m_pPed->entity.pdwRenderWare) return 0;

	return *(int *)((DWORD)m_pPed + 0x79C);
}

//-----------------------------------------------------------

void FUNC_100AE520()
{
	*(WORD *)0xB7CEDC = 0;
}

//-----------------------------------------------------------

int CPlayerPed::FUNC_100AC690()
{
	if(GamePool_Ped_GetAt(m_dwGTAId) && m_pPed)
		return (m_pPed->dwStateFlags >> 26) & 1;

	return 0;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100ACE50(int a1)
{
	field_2E2 = a1;

	if(a1) {
		field_2F7 = 1;
		field_2F6 = 1;
		field_2F8 = 1;
	} else {
		field_2F7 = 0;
		field_2F6 = 0;
		field_2F8 = 0;
	}
}
//-----------------------------------------------------------

BOOL CPlayerPed::FUNC_100ADA00()
{
	if(!m_pPed) {
		return FALSE;
	} else {
		if(!IsAdded()) return FALSE;
		return m_pPed->Tasks->pdwJumpJetPack != 0;
	}
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AD3E0()
{
	if(field_2B9) {
		ScriptCommand(&disassociate_object, field_2B9, 0.0f, 0.0f, 0.0f, 0);
		ScriptCommand(&destroy_object, field_2B9);
		field_2B9 = 0;
	}
}
//-----------------------------------------------------------

BOOL CPlayerPed::FUNC_100AD030()
{
	if(m_pPed) {
		if(GamePool_Ped_GetAt(m_dwGTAId)) {
			if(m_pPed->Tasks->pdwJumpJetPack) {
				if(GetTaskTypeFromTask(m_pPed->Tasks->pdwJumpJetPack) == 1022) return TRUE;
			}
		}
	}
	return FALSE;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AD080()
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	DWORD dwTask = (DWORD)m_pPed->Tasks->pdwJumpJetPack;
	if(!dwTask) return;

	if(GetTaskTypeFromTask((DWORD *)dwTask) != 1022) return;

	_asm mov ecx, dwTask
	_asm mov edx, 0x6275E0
	_asm push 1
	_asm call edx

	m_pPed->Tasks->pdwJumpJetPack = 0;
}
//-----------------------------------------------------------

int CPlayerPed::FUNC_100AE960(int a1)
{
	int iResult;
	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm mov edi, dwPedPtr
	_asm mov eax, a1
	_asm mov edx, [edi+eax*4+0x488]
	_asm mov eax, [edx+0x14]
	_asm mov iResult, eax

	return iResult;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AD8B0(int iWeapon)
{
	if(!m_pPed) return;

	DWORD dwPedPtr = (DWORD)m_pPed;
	iWeapon = (int)pGame->GetWeaponInfo(iWeapon, 1);

	_asm mov ebx, iWeapon
	_asm mov eax, [ebx+0xC]
	_asm push eax
	_asm mov ecx, dwPedPtr
	_asm mov edx, 0x5E3990
	_asm call edx
}
//-----------------------------------------------------------

struc_13 * CPlayerPed::FUNC_100AE5D0()
{
	if(m_bytePlayerNumber) return FUNC_100B43D0(m_bytePlayerNumber);

	return NULL;
}
//-----------------------------------------------------------

DWORD CPlayerPed::FUNC_100AEBB0(int a1)
{
	DWORD dwClump;
	DWORD dwGetClump;
	DWORD dwRenderWare;

	if(!m_pPed) return 0;
	if(m_pPed->entity.vtable == 0x863C40) return 0;
	if(!m_pPed->entity.pdwRenderWare) return 0;

	dwRenderWare = (DWORD)m_pPed->entity.pdwRenderWare;
	dwClump = 0;
	dwGetClump = 0x734A40;

	_asm push dwRenderWare
	_asm mov edx, dwGetClump
	_asm call edx
	_asm pop edx
	_asm mov dwClump, eax

	DWORD dwAnim = *(DWORD *)(dwClump + 0x20);

	return dwAnim + a1 * *(DWORD *)(dwAnim + 0x24) + 0x4C;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AE560(int iType, int a2, int a3, int a4)
{
	if(!m_pPed) return;
	if(m_pPed->entity.vtable == 0x863C40) return;
	if(!m_pPed->entity.pdwRenderWare) return;

	DWORD dwRenderWare = (DWORD)m_pPed->entity.pdwRenderWare;

	if(iType == 1) {
		_asm push a3
		_asm push a2
		_asm push dwRenderWare
		_asm mov edx, 0x4D3AA0
		_asm call edx
		_asm add esp, 0xC
		return;
	}

	if(iType == 2) {
		_asm push a4
		_asm push a3
		_asm push a2
		_asm push dwRenderWare
		_asm mov edx, 0x4D4610
		_asm call edx
		_asm add esp, 0x10
	}
}
//-----------------------------------------------------------

DWORD CPlayerPed::FUNC_100AE990()
{
	DWORD dwGetClump;
	DWORD dwRenderWare;
	DWORD dwResult;

	if(!m_pPed) return 0;
	if(m_pPed->entity.vtable == 0x863C40) return 0;
	if(!m_pPed->entity.pdwRenderWare) return 0;

	dwRenderWare = (DWORD)m_pPed->entity.pdwRenderWare;
	dwGetClump = 0x734A40;

	_asm push dwRenderWare
	_asm mov edx, dwGetClump
	_asm call edx
	_asm pop edx
	_asm mov edx, [eax]
	_asm mov dwResult, edx

	return dwResult;
}
//-----------------------------------------------------------

DWORD CPlayerPed::FUNC_100AE9E0()
{
	DWORD dwGetClump;
	DWORD dwRenderWare;
	DWORD dwResult;

	if(!m_pPed) return 0;
	if(m_pPed->entity.vtable == 0x863C40) return 0;
	if(!m_pPed->entity.pdwRenderWare) return 0;

	dwRenderWare = (DWORD)m_pPed->entity.pdwRenderWare;
	dwGetClump = 0x734A40;

	_asm push dwRenderWare
	_asm mov edx, dwGetClump
	_asm call edx
	_asm pop edx
	_asm mov edx, [eax+4]
	_asm mov dwResult, edx

	return dwResult;
}
//-----------------------------------------------------------

DWORD CPlayerPed::FUNC_100AEA30()
{
	DWORD dwGetClump;
	DWORD dwRenderWare;
	DWORD dwResult;

	if(!m_pPed) return 0;
	if(m_pPed->entity.vtable == 0x863C40) return 0;
	if(!m_pPed->entity.pdwRenderWare) return 0;

	dwRenderWare = (DWORD)m_pPed->entity.pdwRenderWare;
	dwGetClump = 0x734A40;

	_asm push dwRenderWare
	_asm mov edx, dwGetClump
	_asm call edx
	_asm pop edx
	_asm mov edx, [eax+8]
	_asm mov dwResult, edx

	return dwResult;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AC6C0()
{
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(!m_pPed) return;
	if(IN_VEHICLE(m_pPed)) return;
	if(FUNC_100AC690()) return;

	DWORD dwTasks = (DWORD)m_pPed->Tasks;
	if(!dwTasks) return;

	_asm push 0
	_asm mov ecx, dwTasks
	_asm mov edx, 0x601230
	_asm call edx
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AE1E0()
{
	MATRIX4X4 mat;

	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(!field_2F2) return;

	if(field_2BD) {
		ScriptCommand(&script_0650, field_2BD);
		field_2BD = 0;
	}

	GetMatrix(&mat);
	TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);

	field_2F2 = 0;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100ACE90()
{
	int iWeapon;

	if(!field_2E2) return;
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	ScriptCommand(&get_actor_armed_weapon, m_dwGTAId, &iWeapon);

	if((BYTE)iWeapon) SetArmedWeapon(0, 0);
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AEC10(MATRIX4X4 *pMatrix, int iBone)
{
	int iFrame;
	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm mov edi, dwPedPtr
	_asm mov eax, iBone
	_asm mov edx, [edi+eax*4+0x488]
	_asm mov eax, [edx+0x14]
	_asm mov iFrame, eax

	FUNC_100AEA80(pMatrix, iFrame);
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AEC50(MATRIX4X4 *pMatrix, int iBone)
{
	int iFrame;
	DWORD dwPedPtr = (DWORD)m_pPed;

	_asm mov edi, dwPedPtr
	_asm mov eax, iBone
	_asm mov edx, [edi+eax*4+0x488]
	_asm mov eax, [edx+0x14]
	_asm mov iFrame, eax

	FUNC_100AEB10(pMatrix, iFrame);
}
//-----------------------------------------------------------

int CPlayerPed::FUNC_100AD9C0()
{
	if(!m_pPed) return -1;

	return FUNC_100B4100(m_pPed->entity.nModelIndex);
}
//-----------------------------------------------------------

int CPlayerPed::FUNC_100AD9E0()
{
	if(!m_pPed) return -1;

	return FUNC_100B4180(m_pPed->entity.nModelIndex);
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AF1F0()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	if(dwPedPtr) {
		_asm mov ecx, dwPedPtr
		_asm mov eax, 0x532B20
		_asm call eax
	}

	FUNC_100AEF90();
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AE530(int a1, int a2)
{
	if(!m_pPed) return;

	DWORD *pRenderWare = FUNC_1009FA60();
	if(!pRenderWare) return;

	FUNC_100B1DA0(pRenderWare, a1, a2);
}
//-----------------------------------------------------------

float CPlayerPed::FUNC_100AF220()
{
	VECTOR vec;
	int iUnk;

	DWORD dwFrame = FUNC_100AEBB0(0);
	if(!dwFrame) return 0.0f;

	vec.X = 0.0f;
	vec.Y = 0.0f;
	vec.Z = 0.0f;
	iUnk = 0;

	FUNC_100B58A0(dwFrame, &vec, &iUnk);

	return 1.0f - vec.Z;
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AEEE0(MATRIX4X4 *pMatrix)
{
	int iFrame;
	DWORD dwPedPtr;
	int iBone;

	iBone = 2;
	dwPedPtr = (DWORD)m_pPed;

	_asm mov edi, dwPedPtr
	_asm mov eax, iBone
	_asm mov edx, [edi+eax*4+0x488]
	_asm mov eax, [edx+0x14]
	_asm mov iFrame, eax

	FUNC_100AEA80(pMatrix, iFrame);

	CAMERA_AIM *pAim = GameGetInternalAim();

	pMatrix->pos.X = pAim->f1x * 0.04f + pMatrix->pos.X;
	pMatrix->pos.Y = pAim->f1y * 0.04f + pMatrix->pos.Y;
	pMatrix->pos.Z = pMatrix->pos.Z + 0.08f;
}
//-----------------------------------------------------------

DWORD CPlayerPed::FUNC_100AE790()
{
	if(!m_pPed) return 0;
	if(m_pPed->entity.vtable == 0x863C40) return 0;
	if(!m_pPed->entity.pdwRenderWare) return 0;

	DWORD *pTask = FUNC_100AE6E0();
	if(pTask) return FUNC_100AE5F0(pTask);

	return FUNC_100AE6A0();
}
//-----------------------------------------------------------

void CPlayerPed::FUNC_100AD440()
{
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;
	if(!IsAdded()) return;
	if(!field_2B9) return;
	if(FUNC_100ABC50() == 46) return;

	ScriptCommand(&disassociate_object, field_2B9, 0.0f, 0.0f, 0.0f, 0);
	ScriptCommand(&destroy_object_with_fade, field_2B9);
	field_2B9 = 0;
}
//-----------------------------------------------------------

char szDanceG[16][16] = {
	"DANCE_G1", "DANCE_G2", "DANCE_G3", "DANCE_G4",
	"DANCE_G5", "DANCE_G6", "DANCE_G7", "DANCE_G8",
	"DANCE_G9", "DANCE_G10", "DANCE_G11", "DANCE_G12",
	"DANCE_G13", "DANCE_G14", "DANCE_G15", "DANCE_G16"
};

char szDanceB[16][16] = {
	"DANCE_B1", "DANCE_B2", "DANCE_B3", "DANCE_B4",
	"DANCE_B5", "DANCE_B6", "DANCE_B7", "DANCE_B8",
	"DANCE_B9", "DANCE_B10", "DANCE_B11", "DANCE_B12",
	"DANCE_B13", "DANCE_B14", "DANCE_B15", "DANCE_B16"
};

char szStrip[16][16] = {
	"strip_A", "strip_B", "strip_C", "strip_D",
	"strip_E", "strip_F", "strip_G", "STR_A2B",
	"STR_B2A", "STR_B2C", "STR_C1", "STR_C2",
	"STR_C2B", "STR_A2B", "STR_B2C", "STR_C2"
};

char * CPlayerPed::FUNC_100ADB70(int iIndex)
{
	if(!m_iDanceState) return "";

	if(m_iDanceStyle >= 0 && m_iDanceStyle <= 2) {
		if(!m_pPed || FUNC_100B4100(m_pPed->entity.nModelIndex) != 5) {
			if(FUNC_100AD9C0() != 22) return szDanceB[iIndex];
		}

		return szDanceG[iIndex];
	}

	if(m_iDanceStyle == 3) return szStrip[iIndex];

	return "";
}
//-----------------------------------------------------------

DWORD CPlayerPed::FUNC_100AE6A0()
{
	short sIndex = FUNC_1009CFD0(FUNC_100B4430(VAR_10151658, VAR_1015165C));

	return (((BYTE)VAR_10151660 | 0xFFFF8000) << 16) | sIndex;
}
//-----------------------------------------------------------
