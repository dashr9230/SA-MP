#pragma once

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define SLEEP(x) { Sleep(x); }

	#include <windows.h>
	#include <mmsystem.h>
	#include <malloc.h>
#else
	#define SLEEP(x) { usleep(x * 1000); }

	#include <unistd.h>
#endif

#define MAX_PLAYER_NAME		24
#define MAX_PLAYERS			1000
#define MAX_VEHICLES		2000
#define MAX_SETTINGS_STRING 256

#define PI 3.14159265f

#define ARRAY_SIZE(a)	( sizeof((a)) / sizeof(*(a)) )
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }
#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p) = NULL; } }

typedef struct _GAME_SETTINGS {
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
	CHAR szModeName[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>

#include "../raknet/RakClientInterface.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/PacketEnumerations.h"
#include "../raknet/SAMPRPC.h"

typedef unsigned short PLAYERID;
typedef unsigned short VEHICLEID;

#include "../server/amx/amx.h"

#include "../server/system.h"

#include "scrtimers.h"
#include "gamemodes.h"

#include "net/netrpc.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"
#include "net/netgame.h"
#include "net/scriptrpc.h"

void logprintf(char* format, ...);

#ifdef LINUX
void SignalHandler(int sig);
long GetTickCount();
char* strlwr(char* str);
#endif

#define SPECIAL_ACTION_NONE				0

#define PLAYER_STATE_NONE						0
#define PLAYER_STATE_ONFOOT						1
#define PLAYER_STATE_DRIVER						2
#define PLAYER_STATE_PASSENGER					3
#define PLAYER_STATE_EXIT_VEHICLE				4
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER		5
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER	6
#define PLAYER_STATE_WASTED						7
#define PLAYER_STATE_SPAWNED					8
#define PLAYER_STATE_SPECTATING					9

#pragma pack(1)
typedef struct _PLAYER_SPAWN_INFO
{
	char _gap0[6];

	VECTOR vecPos;
	float fRotation;

	char _gap16[24];

} PLAYER_SPAWN_INFO;

#pragma pack(1)
typedef struct _ONFOOT_SYNC_DATA
{
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	VECTOR vecPos;
	QUATERNION quatRotation;
	BYTE byteHealth;
	BYTE byteArmour;
	BYTE byteCurrentWeapon : 6;
	BYTE byteSpecialKey : 2;
	BYTE byteSpecialAction;
	VECTOR vecMoveSpeed;
	VECTOR vecSurfOffsets;
	WORD wSurfInfo;
	DWORD field_40;
} ONFOOT_SYNC_DATA;

#pragma pack(1)
typedef struct _INCAR_SYNC_DATA
{
	VEHICLEID VehicleID;
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	QUATERNION quatRotation;
	VECTOR vecPos;
	VECTOR vecMoveSpeed;
	float fCarHealth;
	BYTE bytePlayerHealth;
	BYTE bytePlayerArmour;
	BYTE byteCurrentWeapon : 6;
	BYTE byteSpecialKey : 2;
	BYTE byteSirenOn;
	BYTE byteLandingGearState;
	VEHICLEID TrailerID;
	union {
		FLOAT fTrainSpeed;
		FLOAT fBikeBankingAngle;
		DWORD dwHydraThrustAngle;
	};
} INCAR_SYNC_DATA;

#pragma pack(1)
typedef struct _PASSENGER_SYNC_DATA
{
	VEHICLEID VehicleID;

	char _gap2;

	BYTE byteCurrentWeapon : 6;
	BYTE byteSpecialKey : 2;
	BYTE bytePlayerHealth;
	BYTE bytePlayerArmour;
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	VECTOR vecPos;	
} PASSENGER_SYNC_DATA;
