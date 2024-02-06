
#include "../main.h"

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	// loop through and initialize all vehicle properties to 0
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		field_3074[VehicleID] = 0;
		field_1134[VehicleID] = 0;
		field_4FB4[VehicleID] = 0;
		field_9DD4[VehicleID] = GetTickCount();
	}
	memset(field_FA4, 0, sizeof(field_FA4));
	field_0 = 0;
	field_17894 = 1;
}

//----------------------------------------------------
