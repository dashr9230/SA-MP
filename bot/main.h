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

#define MAX_SETTINGS_STRING 256

typedef struct _GAME_SETTINGS {
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
	CHAR szModeName[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

#include <stdio.h>
#include <stdlib.h>

#include "scrtimers.h"
#include "npcmode.h"

#include "net/playerpool.h"
#include "net/vehiclepool.h"
#include "net/netgame.h"

void logprintf(char* format, ...);

#ifdef LINUX
void SignalHandler(int sig);
long GetTickCount();
char* strlwr(char* str);
#endif
