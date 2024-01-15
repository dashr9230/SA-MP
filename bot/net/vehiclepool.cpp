
#include "../main.h"

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	// loop through and initialize all net players to null and slot states to false
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		field_84D0[VehicleID] = 0;
	}
}

//----------------------------------------------------
