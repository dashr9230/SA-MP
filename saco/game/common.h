
#pragma once

#include <windows.h>

#define MAX_PLAYERS			1004
#define MAX_VEHICLES		2000
#define PLAYER_PED_SLOTS	210

//-----------------------------------------------------------

typedef unsigned short VEHICLEID;
typedef unsigned short PLAYERID;

typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;

typedef struct _MATRIX4X4 {
	VECTOR right;
	DWORD  flags;
	VECTOR up;
	float  pad_u;
	VECTOR at;
	float  pad_a;
	VECTOR pos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _ENTITY_TYPE
{
	// ENTITY STUFF
	DWORD vtable; // 0-4

	char _pad4[16];

	MATRIX4X4 *mat; // 20-24

} ENTITY_TYPE;

//-----------------------------------------------------------
