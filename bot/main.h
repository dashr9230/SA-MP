#pragma once

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>
#else
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

