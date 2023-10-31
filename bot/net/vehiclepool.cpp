
#include "vehiclepool.h"

CVehiclePool::CVehiclePool()
{
	// TODO: CVehiclePool::CVehiclePool W: 0041A890 L: 080B9408
	for(unsigned short i = 0; i < 2000; i++) {
		field_84D0[i] = 0;
	}
}
