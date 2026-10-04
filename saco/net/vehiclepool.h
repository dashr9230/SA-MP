
#pragma once

#define INVALID_VEHICLE_ID 0xFFFF
#define MAX_WAITING_VEHICLES 100

//----------------------------------------------------

// a vehicle the pool cannot create yet is parked in one of these; the payload layout is unresolved
struct VEHICLE_WAITING
{
	char field_0[40];
};

//----------------------------------------------------

class CVehiclePool
{
public:
	int field_0;

	VEHICLE_WAITING field_4[MAX_WAITING_VEHICLES];

	int field_FA4[MAX_WAITING_VEHICLES];
	int field_1134[MAX_VEHICLES];
	int field_3074[MAX_VEHICLES];
	int field_4FB4[MAX_VEHICLES];

	char _gap6EF4[8000];
	WORD field_8E34[MAX_VEHICLES];

	DWORD field_9DD4[MAX_VEHICLES];

	int field_BD14[MAX_VEHICLES];
	int field_DC54[MAX_VEHICLES];
	int field_FB94[MAX_VEHICLES];
	VECTOR field_11AD4[MAX_VEHICLES];

	int field_17894;

	int GetSlotState(unsigned short VehicleID) { if(VehicleID >= MAX_VEHICLES) return 0; return field_3074[VehicleID]; }
	int GetAt(unsigned short VehicleID)
	{
		if(VehicleID < MAX_VEHICLES && field_3074[VehicleID]) return field_1134[VehicleID];
		return 0;
	}

	void FUNC_1001E9B0(); // .text:1001E9B0

	int FUNC_1001EA80(unsigned short VehicleID); // .text:1001EA80

	~CVehiclePool(); // .text:1001F060

	void FUNC_1001EB00(unsigned short VehicleID, int a2); // .text:1001EB00

	void FUNC_1001EB30(unsigned short VehicleID, char a2, BYTE a3); // .text:1001EB30

	void FUNC_1001EDE0(); // .text:1001EDE0

	short FUNC_1001EB90(int a1); // .text:1001EB90

	int FindGtaIDFromID(int VehicleID); // .text:1001EBC0

	int FUNC_10001120(unsigned short a1); // .text:10001120

	int FUNC_1001EC00(); // .text:1001EC00

	int FUNC_1001ED10(VEHICLE_WAITING *pWaiting); // .text:1001ED10

	int FUNC_1001EC70(float fX, float fY, float fZ); // .text:1001EC70

	short FUNC_1001EF30(float fMaxDistance, VECTOR *vecStart, VECTOR *vecEnd, float fRadius); // .text:1001EF30

	short FUNC_1001EEB0(int a1, VECTOR *vecOrigin, VECTOR *vecLine); // .text:1001EEB0

	void FUNC_1001ED70(); // .text:1001ED70

	char FUNC_1001EE20(VECTOR *vecStart, VECTOR *vecEnd); // .text:1001EE20

	CVehiclePool();
};

//----------------------------------------------------
