
#pragma once

#define INVALID_VEHICLE_ID	0xFFFF

#pragma pack(1)
typedef struct _NEW_VEHICLE {
	VEHICLEID VehicleId;
	int field_2;
	int field_6;

	char _gapA[8];

	int field_12;
	char field_16;
	char field_17;

	char _gap18[16];

} NEW_VEHICLE;

#pragma pack(1)
typedef struct _VEHICLE_SPAWN_INFO
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;

	char _gap1C[8];

} VEHICLE_SPAWN_INFO;

//----------------------------------------------------

class CVehiclePool // size: 114000
{
private:
	char _gap0[34000];
	BOOL				m_bVehicleSlotState[MAX_VEHICLES];
	VEHICLE_SPAWN_INFO	m_SpawnInfo[MAX_VEHICLES];

public:

	CVehiclePool();
	~CVehiclePool();

	BOOL New(NEW_VEHICLE *pNewVehicle);

	BOOL Delete(VEHICLEID VehicleID);

	// Find out if the slot is inuse.
	BOOL GetSlotState(VEHICLEID VehicleID) {
		if(VehicleID >= MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[VehicleID];
	};
};
