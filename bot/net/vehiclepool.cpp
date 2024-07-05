
#include "../main.h"
extern CNetGame *pNetGame;

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	// loop through and initialize all net players to null and slot states to false
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		m_bVehicleSlotState[VehicleID] = FALSE;
	}
}

//----------------------------------------------------

CVehiclePool::~CVehiclePool()
{
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		Delete(VehicleID);
	}
}

//----------------------------------------------------

BOOL CVehiclePool::New(NEW_VEHICLE *pNewVehicle)
{
	if(pNewVehicle->VehicleId < MAX_VEHICLES)
	{
		memset(&m_SpawnInfo[pNewVehicle->VehicleId],0,sizeof(VEHICLE_SPAWN_INFO));

		// Setup the spawninfo for the next respawn.
		m_SpawnInfo[pNewVehicle->VehicleId].field_0 = pNewVehicle->field_2;
		m_SpawnInfo[pNewVehicle->VehicleId].field_4 = pNewVehicle->field_6;
		m_SpawnInfo[pNewVehicle->VehicleId].field_8 = pNewVehicle->field_6;
		m_SpawnInfo[pNewVehicle->VehicleId].field_C = pNewVehicle->field_6;
		m_SpawnInfo[pNewVehicle->VehicleId].field_10 = pNewVehicle->field_12;
		m_SpawnInfo[pNewVehicle->VehicleId].field_14 = pNewVehicle->field_16;
		m_SpawnInfo[pNewVehicle->VehicleId].field_18 = pNewVehicle->field_17;

		pNetGame->SetVehicleAdded(pNewVehicle->VehicleId, TRUE);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CVehiclePool::Delete(VEHICLEID VehicleID)
{
	pNetGame->SetVehicleAdded(VehicleID, FALSE);
	return TRUE;
}

//----------------------------------------------------
