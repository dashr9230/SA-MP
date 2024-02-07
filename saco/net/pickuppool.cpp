
#include "../main.h"

//----------------------------------------------------

CPickupPool::CPickupPool()
{
	 memset(field_F004, 0, sizeof(field_F004));
	 field_0 = 0;
	 for (int i = 0; i < MAX_PICKUPS; i++)
	 {
		field_4[i] = 0;
		field_8004[i] = 0;
		field_4004[i] = 0;
	 }
}

//----------------------------------------------------
