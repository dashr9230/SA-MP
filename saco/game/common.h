
#pragma once

#include <windows.h>
#include <assert.h>

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
typedef struct _WEAPON_SLOT_TYPE
{
	DWORD dwType;

	char _gap4[24];

} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes

typedef struct _PED_TASKS_TYPE
{
	char _gap0[16];

	DWORD * pdwJumpJetPack;
} PED_TASKS_TYPE;

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _ENTITY_TYPE
{
	// ENTITY STUFF
	DWORD vtable; // 0-4

	char _pad4[16];

	MATRIX4X4 *mat; // 20-24
	DWORD *pdwRenderWare; // 24-28

	char _gap1C[6];

	WORD nModelIndex; // 34-36

	char _gap24[32];

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

	char _gapB8[964];

	PED_TASKS_TYPE *Tasks; // 1148-1152

	char _gap480[128];

	DWORD dwActiveVision; // 1280-1284

	char _gap504[156];

	WEAPON_SLOT_TYPE WeaponSlots[13]; // 1440-1804

} PED_TYPE;

//-----------------------------------------------------------
typedef struct _VEHICLE_TYPE
{
	char _gap0;
} VEHICLE_TYPE;

//-----------------------------------------------------------

#define TRAIN_PASSENGER_LOCO			537
#define TRAIN_FREIGHT_LOCO				538
#define TRAIN_PASSENGER					569
#define TRAIN_FREIGHT					570
#define TRAIN_TRAM						449
#define HYDRA							520

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

//---- weapon model defines ----
#define WEAPON_MODEL_BRASSKNUCKLE		331 // was 332
#define WEAPON_MODEL_GOLFCLUB			333
#define WEAPON_MODEL_NITESTICK			334
#define WEAPON_MODEL_KNIFE				335
#define WEAPON_MODEL_BAT				336
#define WEAPON_MODEL_SHOVEL				337
#define WEAPON_MODEL_POOLSTICK			338
#define WEAPON_MODEL_KATANA				339
#define WEAPON_MODEL_CHAINSAW			341
#define WEAPON_MODEL_DILDO				321
#define WEAPON_MODEL_DILDO2				322
#define WEAPON_MODEL_VIBRATOR			323
#define WEAPON_MODEL_VIBRATOR2			324
#define WEAPON_MODEL_FLOWER				325
#define WEAPON_MODEL_CANE				326
#define WEAPON_MODEL_GRENADE			342 // was 327
#define WEAPON_MODEL_TEARGAS			343 // was 328
#define WEAPON_MODEL_MOLTOV				344 // was 329
#define WEAPON_MODEL_COLT45				346
#define WEAPON_MODEL_SILENCED			347
#define WEAPON_MODEL_DEAGLE				348
#define WEAPON_MODEL_SHOTGUN			349
#define WEAPON_MODEL_SAWEDOFF			350
#define WEAPON_MODEL_SHOTGSPA			351
#define WEAPON_MODEL_UZI				352
#define WEAPON_MODEL_MP5				353
#define WEAPON_MODEL_AK47				355
#define WEAPON_MODEL_M4					356
#define WEAPON_MODEL_TEC9				372
#define WEAPON_MODEL_RIFLE				357
#define WEAPON_MODEL_SNIPER				358
#define WEAPON_MODEL_ROCKETLAUNCHER		359
#define WEAPON_MODEL_HEATSEEKER			360
#define WEAPON_MODEL_FLAMETHROWER		361
#define WEAPON_MODEL_MINIGUN			362
#define WEAPON_MODEL_SATCHEL			363
#define WEAPON_MODEL_BOMB				364
#define WEAPON_MODEL_SPRAYCAN			365
#define WEAPON_MODEL_FIREEXTINGUISHER	366
#define WEAPON_MODEL_CAMERA				367
#define WEAPON_MODEL_NIGHTVISION		368	// newly added
#define WEAPON_MODEL_INFRARED			369	// newly added
#define WEAPON_MODEL_JETPACK			370	// newly added
#define WEAPON_MODEL_PARACHUTE			371

//-----------------------------------------------------------

#define MAPICON_LOCAL					0
#define MAPICON_GLOBAL					1
#define MAPICON_LOCAL_CHECKPOINT		2
#define MAPICON_GLOBAL_CHECKPOINT		3
