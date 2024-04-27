
#ifndef SAMPSRV_MAIN_H
#define SAMPSRV_MAIN_H

// -------
// DEFINES
// -------

#define MAX_PLAYER_NAME			24
#define MAX_PLAYERS				1000
#define MAX_VEHICLES			2000
#define MAX_FILTER_SCRIPTS		16
#define MAX_OBJECTS				1000
#define MAX_MENUS				128
#define MAX_TEXT_DRAWS			2048
#define MAX_GANG_ZONES			1024
#define MAX_LABELS				1024
#define MAX_ACTORS				1000

#define DEFAULT_MAX_PLAYERS		50
#define DEFAULT_LISTEN_PORT		8192

#define PI 3.14159265f

#define ARRAY_SIZE(a)	( sizeof((a)) / sizeof(*(a)) )
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }

// ------------
// VERSION INFO
// ------------

#define SAMP_VERSION "0.3.7-R2"

// ------------
// OS SPECIFICS
// ------------

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define SLEEP(x) { Sleep(x); }

	#include <windows.h>
	#include <mmsystem.h>
	#include <time.h>
#else
	#define SLEEP(x) { usleep(x * 1000); }

	#ifndef stricmp
		#define stricmp strcasecmp
	#endif
#endif

// --------
// INCLUDES
// --------

// Regular crap
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Std
#include <map>
#include <string>
#include <vector>

// Raknet
#include "../raknet/RakServer.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/PacketEnumerations.h"
#include "../raknet/SAMPRPC.h"
#include "../raknet/GetTime.h"

// amx
#include "amx/amx.h"

// SA-MP

typedef unsigned short PLAYERID;
typedef unsigned short ACTORID;
typedef unsigned short VEHICLEID;

#include "system.h"
#include "console.h"
#include "scrhttps.h"
#include "scrtimers.h"
#include "gamemodes.h"
#include "filterscripts.h"
#include "netrpc.h"
#include "player.h"
#include "playerpool.h"
#include "vehiclepool.h"
#include "pickuppool.h"
#include "objectpool.h"
#include "menupool.h"
#include "textdrawpool.h"
#include "labelpool.h"
#include "gangzonepool.h"
#include "actorpool.h"
#include "netgame.h"
#include "plugins.h"
#include "mathutils.h"

// ---------
// EXTERNALS
// ---------

extern CConsole* pConsole;
extern CNetGame* pNetGame;
extern CPlugins* pPlugins;

extern PLAYERID RconUser;

// -------------------
// FUNCTION PROTOTYPES
// -------------------

void logprintf(char* format, ...);
void flogprintf(char* format, ...);
void LoadLogFile();

#ifdef LINUX
char* strlwr(char* str);
#endif

#endif // SAMPSRV_MAIN_H
