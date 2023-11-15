
#pragma once

#include <stdio.h>
#include <windows.h>
#include <process.h>

#define MAX_SETTINGS_STRING		256

#define GTASA_VERSION_UNKNOWN	0
#define GTASA_VERSION_USA10		1
#define GTASA_VERSION_EU10		2

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

#include "net/netgame.h"

#include "config.h"
#include "fontrender.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include "audiostream.h"
#include "archive/ArchiveFS.h"

void SetStringFromCommandLine(char *szCmdLine, char *szString);
void SetStringFromQuotedCommandLine(char *szCmdLine, char *szString);
void InitSettings();
