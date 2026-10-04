
#pragma once

#define MAX_PICKUPS 4096

#pragma pack(1)
typedef struct _PICKUP {
	int iModel;
	int iType;
	float fX;
	float fY;
	float fZ;
} PICKUP, *PPICKUP;

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

	// MATCH
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

	void sub_10013440(int iPickup); // .text:10013440
	void sub_10013320(int iPickup); // .text:10013320
	void sub_10013270(PICKUP *pPickup, int iPickup); // .text:10013270
	void sub_10013380(WORD wPickup); // .text:10013380

};

//----------------------------------------------------