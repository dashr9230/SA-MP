
#pragma once

#include <windows.h>

#define MAX_PLAYERS			1004
#define MAX_ACTORS			1000
#define MAX_VEHICLES		2000
#define MAX_OBJECTS			1000
#define MAX_MENUS			128
#define MAX_TEXT_DRAWS		2304
#define MAX_GANG_ZONES		1024
#define MAX_LABELS			2048
#define PLAYER_PED_SLOTS	210

//-----------------------------------------------------------

typedef unsigned short VEHICLEID;
typedef unsigned short ACTORID;
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
	DWORD *pdwRenderWare; // 24-28

	char _gap1C[40];

	VECTOR vecMoveSpeed; // 68-80
	VECTOR vecTurnSpeed; // 80-92

	char _gap5C[88];

	DWORD dwUnkModelRel; // 180-184
} ENTITY_TYPE;

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _PED_TYPE
{
	ENTITY_TYPE entity;

} PED_TYPE;

//-----------------------------------------------------------
typedef struct _VEHICLE_TYPE
{
	char _gap0;
} VEHICLE_TYPE;

//-----------------------------------------------------------

//-----------------------------------------------------------

// ---- weapon id defines ----
#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_ROCKET					19
#define WEAPON_ROCKET_HS				20
#define WEAPON_FREEFALLBOMB				21
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA					43
#define WEAPON_NIGHTVISION				44
#define WEAPON_INFRARED					45
#define WEAPON_PARACHUTE				46
#define WEAPON_ARMOUR					47
#define WEAPON_VEHICLE					49
#define WEAPON_HELIBLADES				50
#define WEAPON_EXPLOSION				51
#define WEAPON_DROWN					53
#define WEAPON_COLLISION				54

//-----------------------------------------------------------

#define MAPICON_LOCAL					0
#define MAPICON_GLOBAL					1
#define MAPICON_LOCAL_CHECKPOINT		2
#define MAPICON_GLOBAL_CHECKPOINT		3
