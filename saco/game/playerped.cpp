

#include <windows.h>
#include <assert.h>
#define _ASSERT assert

#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"
#include "task.h"

extern CGame *pGame;

extern BYTE	*pbyteCurrentPlayer;

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
	field_2D2 = 0;
	field_2DE = 0;
	field_2E2 = 0;
	field_48 = 0;
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
	 wKeys >>= 1; // 1

	 // CROUCHING TOGGLE (18)
	 if(IsInVehicle() && wKeys & 1) {
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
	 wKeys >>= 1; // 2

	  // FIRING (17)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 3

	  // SPRINT (16)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 4


	 // SECONDARY ONFOOT ATTACK (15)
	 if( (wKeys & 1) && !IsInJetpackMode() ) {
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
	 wKeys >>= 1; // 5

	 // JUMP (14)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 6

	 // INCAR LOOK RIGHT (7)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 7

	 // INCAR HANDBRAKE / ONFOOT TARGET (6)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 8

 	 // INCAR LOOK LEFT (5)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 9

	 // SUBMISSION (19)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 10

	 // WALKING (21)
	 if(wKeys & 1) {
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
	 wKeys >>= 1; // 11

	 // ANALOG2 D/U
	 BYTE byteVal = wKeys & 3;

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
	 wKeys >>= 2; // 12-13

	 // ANALOG2 L/R
	 byteVal = wKeys & 3;

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

	if(IsInVehicle() && pInternalKeys->wKeys1[5]) wRet |= 1; // incar look left
	wRet <<= 1;

	if(pInternalKeys->wKeys1[6]) wRet |= 1; // incar handbrake / target
	wRet <<= 1;

	if(IsInVehicle() && pInternalKeys->wKeys1[7]) wRet |= 1; // incar look right
	wRet <<= 1;

	if(pInternalKeys->wKeys1[14]) wRet |= 1; // jump
	wRet <<= 1;

	if(!IsInJetpackMode() && pInternalKeys->wKeys1[15]) wRet |= 1; // secondary onfoot attack
	wRet <<= 1;

	if(pInternalKeys->wKeys1[16]) wRet |= 1; // sprint
	wRet <<= 1;

	if(HasAmmoForCurrentWeapon() && pInternalKeys->wKeys1[17]) wRet |= 1; // fire
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

int CPlayerPed::GetCurrentVehicleID()
{
	if(!m_pPed) return 0;

	VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
	return GamePool_Vehicle_GetIndex(pVehicle);
}

//-----------------------------------------------------------
// Shows the normal marker

void CPlayerPed::ShowMarker(int iMarkerColorID)
{
	if (m_dwArrow) HideMarker();
	ScriptCommand(&create_arrow_above_actor, m_dwGTAId, &m_dwArrow);
	ScriptCommand(&set_marker_color, m_dwArrow, iMarkerColorID);
	ScriptCommand(&show_on_radar2, m_dwArrow, 2);
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

	m_pPed->fRotation1 = DegToRad(fRotation);
	m_pPed->fRotation2 = DegToRad(fRotation);

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
		WEAPON_SLOT_TYPE * WeaponSlot = GetCurrentWeaponSlot();

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

	if( GetActionTrigger() == ACTION_INCAR && (pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle) != 0 ) {
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
		SetArmedWeapon(0);
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
		VEHICLEID TrainVehicleId = pVehiclePool->FindIDFromGtaPtr(pVehicle);
		if(TrainVehicleId == INVALID_VEHICLE_ID || TrainVehicleId > MAX_VEHICLES) return;

		CVehicle* pTrain = pVehiclePool->GetAt(TrainVehicleId);
		if ( pTrain && pTrain->IsATrainPart() && m_pPed == GamePool_FindPlayerPed() ) {
			ScriptCommand(&camera_on_vehicle, pTrain->m_dwGTAId, 3, 2);
		}
	}
}

//-----------------------------------------------------------

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
		if(!IsInVehicle()) {
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

WORD CPlayerPed::GetAmmo()
{
	if(m_pPed) {
		WEAPON_SLOT_TYPE * WeaponSlot = GetCurrentWeaponSlot();

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
	if (HasGoggles()) return;
	if (FindWeaponSlot( 44 ) == NULL && FindWeaponSlot( 45 ) == NULL)
		GiveWeapon( 44, 1 ); // Prevents crashing due to lack of animations.

	CTaskGoggles* pGoggles = new CTaskGoggles();
	pGoggles->ApplyToPed( this );
	m_bGoggleState = TRUE;
}

//-----------------------------------------------------------

void CPlayerPed::StopGoggles()
{
	if (!m_pPed || !HasGoggles()) return;

	m_bGoggleState = FALSE;
	DWORD dwPedPointer = (DWORD)m_pPed;
	_asm mov ecx, dwPedPointer
	_asm mov eax, 0x5E6010
	_asm call eax
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasGoggles()
{
	if (!m_pPed) return FALSE;
	return (BOOL)(m_pPed->dwActiveVision != 0 || m_bGoggleState);
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

void CPlayerPed::SetWeaponSkillLevel(int iSkill, float fLevel)
{
	if(m_bytePlayerNumber == 0)
		GameSetLocalPlayerWeaponSkillLevel(iSkill, fLevel);
	else
		GameSetPlayerWeaponSkillLevel(m_bytePlayerNumber, iSkill, fLevel);
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
