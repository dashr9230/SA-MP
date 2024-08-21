
#pragma once

#define MAX_PICKUPS 4096

//----------------------------------------------------

class CPickupPool
{
private:

	int field_0;
	DWORD	m_dwHnd[MAX_PICKUPS];
	int field_4004[MAX_PICKUPS];
	int field_8004[MAX_PICKUPS];

	char _gapC004[12288];

	char field_F004[81920];

public:

	CPickupPool() {
		memset(field_F004, 0, sizeof(field_F004));
		field_0 = 0;
		for (int i = 0; i < MAX_PICKUPS; i++)
		{
			m_dwHnd[i] = NULL;
			field_8004[i] = 0;
			field_4004[i] = -1;
		}
	}

	~CPickupPool();

};

//----------------------------------------------------