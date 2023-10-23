
#pragma once

#include <windows.h>

#define MAX_SETTINGS_STRING		256

typedef struct _GAME_SETTINGS {
	BOOL bDebug;
	BOOL bPlayOnline;
	BOOL bWindowedMode;
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
	CHAR szDebugScript[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

#include "game/game.h"

void SetStringFromCommandLine(char *szCmdLine, char *szString);
void SetStringFromQuotedCommandLine(char *szCmdLine, char *szString);
void InitSettings();
