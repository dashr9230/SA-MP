
#pragma once

//----------------------------------------------------

class CVehiclePool
{
public:
	int field_0;

	char _gap4[4000];

	char field_FA4[400];
	int field_1134[MAX_VEHICLES];
	int field_3074[MAX_VEHICLES];
	int field_4FB4[MAX_VEHICLES];

	char _gap6EF4[12000];

	DWORD field_9DD4[MAX_VEHICLES];

	char _gapBD14[48000];

	int field_17894;

	CVehiclePool();
};

//----------------------------------------------------
