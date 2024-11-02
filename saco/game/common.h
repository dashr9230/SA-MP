
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

#define PI 3.1415926f

#pragma pack(1)
typedef struct _RGBA {
	unsigned char r,g,b,a;
} RGBA, *PRGBA;

typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;

typedef struct _VECTOR2D {
	float X,Y;
} VECTOR2D, *PVECTOR2D;

typedef struct _FRECT {
	float l,t,r,b;
} FRECT, *PFRECT;

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

#define PADDING(x,y) BYTE x[y]

//-----------------------------------------------------------

#define IN_VEHICLE(x) ((x->dwStateFlags & 256) >> 8)

//-----------------------------------------------------------
typedef struct _WEAPON_SLOT_TYPE
{
	DWORD dwType;

	char _gap4[8];

	DWORD dwAmmo;

	char _gap10[12];

} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes

//-----------------------------------------------------------
#pragma pack(1)
typedef struct _PED_TASKS_TYPE
{
	char _gap0[4];
	
	// Basic Tasks
	DWORD * pdwDamage;

	char _gap8[4];

	DWORD * pdwSwimWasted;
	DWORD * pdwJumpJetPack;

	char _gap14[4];

	// Extended Tasks
	DWORD * pdwFighting;

	char _gap1C[16];

	DWORD * pdwIK;
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
	DWORD dwProcessingFlags; // 28-32
	char _gap20[2];

	WORD nModelIndex; // 34-36
	char _gap24[18];
	BYTE nControlFlags; // 54-55
	char _gap37[11];
	BYTE byteImmunities; // 66-67
	BYTE byteUnkEntFlags; // 67-68
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

	char _gapB8[948];

	DWORD dwStateFlags; // 1132-1136

	char _gap470[12];

	PED_TASKS_TYPE *Tasks; // 1148-1152
	DWORD dwPlayerInfoOffset; // 1152-1156
	char _gap484[124];

	DWORD dwActiveVision; // 1280-1284

	char _gap504[44];

	DWORD dwAction; // 1328-1332

	char _gap534[12];

	float fHealth;		 // 1344-1348

	char _gap544[4];

	float fArmour;		// 1352-1356

	char _gap54C[12];

	float fRotation1;	// 1368-1372
	float fRotation2;	// 1372-1376

	char _gap560[8];

	DWORD pContactVehicle; // 1384 - 1388

	char _gap56C[24];

	DWORD pContactEntity; // 1412 - 1416

	char _gap588[4];

	DWORD pVehicle;	// 1420-1424

	char _gap590[8];

	DWORD dwPedType; // 1432-1436

	char _gap59C[4];

	WEAPON_SLOT_TYPE WeaponSlots[13]; // 1440-1804

	char _gap70C[12];

	BYTE byteCurWeaponSlot; // 1816-1817

} PED_TYPE;

//-----------------------------------------------------------
typedef struct _VEHICLE_TYPE
{
	ENTITY_TYPE entity; // 0-184

	char _gapB8[318];

	BYTE byteHorn;		  // 502-503

	char _gap1F7[561];

	BYTE byteFlags;		  // 1064-1065

	char _gap429[4];

	BYTE _pad211  : 7;   // 1069-1070 (bits 0..6)
	BYTE bSirenOn : 1;   // 1069-1070 (bit 7)

	char _gap42E[6];

	BYTE byteColor1;      // 1076-1077
	BYTE byteColor2;      // 1077-1078

	char _gap436[38];

	WORD wAlarmState; // 1116-1118

	char _gap45E[2];

	PED_TYPE * pDriver;   // 1120-1124
	PED_TYPE * pPassengers[7]; // 1124-1152

	char _gap480[8];

	BYTE byteMaxPassengers; // 1160-1161

	char _gap489[31];

	BYTE byteMoreFlags;	// 1192-1193

	char _gap4A9[23];

	float fHealth;			// 1216-1220
	DWORD dwTractor;		// 1220-1224
	DWORD dwTrailer;		// 1224-1228
	
	char _pad4CC[44];

	DWORD dwDoorsLocked;	// 1272-1276

	char _pad4FC[24];

	BYTE byteHorn2;			// 1300-1301

	char _gap515[139];

	union {
		struct {
			DWORD dwDamageBase;     // 1440-1444
			BYTE byteEngineStatus;  // 1444-1445
			BYTE bCarWheelPopped[4]; // 1445-1449
			DWORD dwDoorStatus1;	// 1449-1453
			WORD wDoorStatus2;		// 1453-1455
			BYTE byteDamageUnk1;	// 1455-1456
			DWORD dwLightStatus;	// 1456-1460
			DWORD dwPanelStatus;	// 1460-1464
		};
		struct {
			DWORD dwTrainUnk;	 // 1440-1444
			float fTrainSpeed;   // 1444-1448
			PADDING(_pad2422,18); // 1448-1464
		};
	};

	char _pad5BA[162];

	BYTE bBikeWheelPopped[2]; // 1628-1630

	char _gap65E[526];

	DWORD dwHydraThrusters; // 2156-2160

	char _gap870[220];

	float fTankRotX;		// 2380-2384
	float fTankRotY;		// 2384-2388

	char _gap954[120];

	float fPlaneLandingGear;// 2508-2512

	char _gap9D0[1517];

} VEHICLE_TYPE;

//-----------------------------------------------------------

typedef struct _BUILDING_TYPE
{
	char _gap0[56];
} BUILDING_TYPE;

//-----------------------------------------------------------

typedef struct _DUMMY_TYPE
{
	char _gap0[56];
} DUMMY_TYPE;

//-----------------------------------------------------------

typedef struct _OBJECT_TYPE
{
	DWORD vtable; // 0-4

	char _gap4[30];

	WORD nModelIndex; // 34-36

	char _gap24[376];

} OBJECT_TYPE;

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5
#define	VEHICLE_SUBTYPE_PUSHBIKE		6
#define	VEHICLE_SUBTYPE_TRAIN			7

//-----------------------------------------------------------

#define ACTION_WASTED					55
#define ACTION_DEATH					54
#define ACTION_INCAR					50
#define ACTION_NORMAL					1
#define ACTION_SCOPE					12
#define ACTION_NONE						0 

//-----------------------------------------------------------

#define TRAIN_PASSENGER_LOCO			538
#define TRAIN_FREIGHT_LOCO				537
#define TRAIN_PASSENGER					570
#define TRAIN_FREIGHT					569
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

#define OBJECT_PARACHUTE				3131

//-----------------------------------------------------------

#define MAPICON_LOCAL					0
#define MAPICON_GLOBAL					1
#define MAPICON_LOCAL_CHECKPOINT		2
#define MAPICON_GLOBAL_CHECKPOINT		3
