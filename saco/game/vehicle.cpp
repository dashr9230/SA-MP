
#include "../main.h"
#include "util.h"

//-----------------------------------------------------------
// CONSTRUCTOR

CVehicle::CVehicle(int iType, float fPosX, float fPosY,
				   float fPosZ, float fRotation, BOOL bKeepModelLoaded,
				   int a8)
{
	// TODO: CVehicle::CVehicle .text:100B88F0
}

//-----------------------------------------------------------

void CVehicle::LinkToInterior(int iInterior)
{
	if(GamePool_Vehicle_GetAt(m_dwGTAId)) {
		ScriptCommand(&link_vehicle_to_interior, m_dwGTAId, iInterior);
	}
}

//-----------------------------------------------------------
// If the game has internally destroyed the vehicle
// during this frame, the vehicle pointer should become 0

void CVehicle::ResetPointers()
{
	if(!m_pVehicle) return;

	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pVehicle;
}

//-----------------------------------------------------------

BOOL CVehicle::HasADriver()
{
	if(!m_pVehicle) return FALSE;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	if(m_pVehicle) {
		if(m_pVehicle->pDriver && IN_VEHICLE(m_pVehicle->pDriver) && m_pVehicle->pDriver->dwPedType == 0)
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsOccupied()
{
	if(m_pVehicle) {
		if(m_pVehicle->pDriver) return TRUE;
		if(m_pVehicle->pPassengers[0]) return TRUE;
		if(m_pVehicle->pPassengers[1]) return TRUE;
		if(m_pVehicle->pPassengers[2]) return TRUE;
		if(m_pVehicle->pPassengers[3]) return TRUE;
		if(m_pVehicle->pPassengers[4]) return TRUE;
		if(m_pVehicle->pPassengers[5]) return TRUE;
		if(m_pVehicle->pPassengers[6]) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

void CVehicle::SetLockedState(int iLocked)
{
	if(!m_pVehicle) return;

	if(iLocked) {
		ScriptCommand(&lock_car,m_dwGTAId,1);
	} else {
		ScriptCommand(&lock_car,m_dwGTAId,0);
	}
}

//-----------------------------------------------------------

void CVehicle::SetEngineState(BOOL bState)
{
	if(!m_pVehicle) return;

	if(!bState) {
		m_pVehicle->byteFlags &= 0xDF;
	} else {
		m_pVehicle->byteFlags |= 0x20;
	}
}

//-----------------------------------------------------------

float CVehicle::GetHealth()
{
	if(m_pVehicle) return m_pVehicle->fHealth;
	else return 0.0f;
}

//-----------------------------------------------------------

void CVehicle::SetHealth(float fHealth)
{
	if(m_pVehicle) {
		m_pVehicle->fHealth = fHealth;
	}
}

//-----------------------------------------------------------

void CVehicle::SetColor(BYTE byteColor1, BYTE byteColor2)
{
	if(m_pVehicle && GamePool_Vehicle_GetAt(m_dwGTAId))  {
		m_pVehicle->byteColor1 = byteColor1;
		m_pVehicle->byteColor2 = byteColor2;
	}
	m_byteColor2 = byteColor2;
	m_byteColor1 = byteColor1;
	m_bHasNewColor = TRUE;
}

//-----------------------------------------------------------

void CVehicle::UpdateColor()
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if(m_bHasNewColor) {
		if(!field_71) {
			if(m_byteColor1 != m_pVehicle->byteColor1 || m_byteColor2 != m_pVehicle->byteColor2) {
				m_pVehicle->byteColor1 = m_byteColor1;
				m_pVehicle->byteColor2 = m_byteColor2;
			}
		}
	}
}

//-----------------------------------------------------------

BOOL CVehicle::HasSunk()
{
	if(!m_pVehicle) return FALSE;

	return ScriptCommand(&has_car_sunk,m_dwGTAId);
}

//-----------------------------------------------------------

BOOL CVehicle::IsWrecked()
{
	if(!m_pVehicle) return FALSE;

	return ScriptCommand(&is_car_wrecked,m_dwGTAId);
}

//-----------------------------------------------------------

BOOL CVehicle::IsDriverLocalPlayer()
{
	if(m_pVehicle) {
		if((PED_TYPE *)m_pVehicle->pDriver == GamePool_FindPlayerPed()) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsATrainPart()
{
	int nModel;
	if(m_pVehicle) {
		nModel = m_pVehicle->entity.nModelIndex;
		if(nModel == TRAIN_PASSENGER_LOCO) return TRUE;
		if(nModel == TRAIN_PASSENGER) return TRUE;
		if(nModel == TRAIN_FREIGHT_LOCO) return TRUE;
		if(nModel == TRAIN_FREIGHT) return TRUE;
		if(nModel == TRAIN_TRAM) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::HasTurret()
{
	int nModel = GetModelIndex();
	return (nModel == 432 ||	// Tank
			nModel == 564 ||	// RC Tank
			nModel == 407 ||	// Firetruck
			nModel == 601		// Swatvan
			);
}

//-----------------------------------------------------------

void CVehicle::SetSirenOn(BYTE byteState)
{
	m_pVehicle->bSirenOn = byteState;
}

//-----------------------------------------------------------

BOOL CVehicle::IsSirenOn()
{
	return (m_pVehicle->bSirenOn == 1);
}

//-----------------------------------------------------------

void CVehicle::SetAlarmState(WORD wState)
{
	m_pVehicle->wAlarmState = wState;
}

//-----------------------------------------------------------

UINT CVehicle::GetPassengersMax()
{
	return 0;
}

//-----------------------------------------------------------

void CVehicle::SetHydraThrusters(DWORD dwDirection)
{
	if(m_pVehicle) m_pVehicle->dwHydraThrusters = dwDirection; // 0x00 - 0x80 // byte
}

//-----------------------------------------------------------

//-----------------------------------------------------------

