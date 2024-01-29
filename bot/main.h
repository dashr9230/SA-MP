#pragma once

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define SLEEP(x) { Sleep(x); }

	#include <windows.h>
	#include <mmsystem.h>
#else
	#define SLEEP(x) { usleep(x * 1000); }

	#include <unistd.h>
#endif

#define MAX_PLAYER_NAME		24
#define MAX_PLAYERS			1000
#define MAX_VEHICLES		2000
#define MAX_SETTINGS_STRING 256

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

#include "../raknet/RakClientInterface.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/PacketEnumerations.h"
#include "../raknet/SAMPRPC.h"

typedef unsigned short PLAYERID;
typedef unsigned short VEHICLEID;

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
