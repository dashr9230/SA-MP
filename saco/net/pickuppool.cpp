
#include "../main.h"

//----------------------------------------------------

CPickupPool::CPickupPool()
{
	 memset(field_F004, 0, sizeof(field_F004));
	 field_0 = 0;
	 for (int i = 0; i < MAX_PICKUPS; i++)
	 {
		m_dwHnd[i] = NULL;
		field_8004[i] = 0;
		field_4004[i] = -1;
	 }
}

//----------------------------------------------------

CPickupPool::~CPickupPool()
{
	for(int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != NULL)
		{
			ScriptCommand(&destroy_pickup, m_dwHnd[i]);
		}
	}
}

//----------------------------------------------------

