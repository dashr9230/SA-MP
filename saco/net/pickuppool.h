
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

	CPickupPool();
	~CPickupPool();

};

//----------------------------------------------------