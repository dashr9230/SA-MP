
#include "main.h"

#define CHECK_INIT() { if (!m_bInitialised) return 0; };

extern "C" int amx_CoreInit(AMX* amx);
extern "C" int amx_CoreCleanup(AMX* amx);
extern "C" int amx_FloatInit(AMX* amx);
extern "C" int amx_FloatCleanup(AMX* amx);
extern "C" int amx_StringInit(AMX* amx);
extern "C" int amx_StringCleanup(AMX* amx);
extern "C" int amx_FileInit(AMX* amx);
extern "C" int amx_FileCleanup(AMX* amx);
extern "C" int amx_TimeInit(AMX* amx);
extern "C" int amx_TimeCleanup(AMX* amx);

int AMXAPI aux_LoadProgram(AMX* amx, char* filename);
int AMXAPI aux_FreeProgram(AMX *amx);
void AMXPrintError(CGameMode* pGameMode, AMX *amx, int error);
int amx_CustomInit(AMX *amx);
int amx_sampDbInit(AMX *amx);
int amx_sampDbCleanup(AMX *amx);

char szGameModeFileName[256];

extern CNetGame* pNetGame;

//----------------------------------------------------------------------------------

CGameMode::CGameMode()
{
	m_bInitialised = false;
	m_bSleeping = false;
}

//----------------------------------------------------------------------------------

CGameMode::~CGameMode()
{
	Unload();
}

//----------------------------------------------------------------------------------

bool CGameMode::Load(char* pFileName)
{
	if (m_bInitialised)
		Unload();

	FILE* f = fopen(pFileName, "rb");
	if (!f) return false;
	fclose(f);

	memset((void*)&m_amx, 0, sizeof(AMX));
	m_fSleepTime = 0.0f;
	strcpy(szGameModeFileName, pFileName);

	int err = aux_LoadProgram(&m_amx, szGameModeFileName);
	if (err != AMX_ERR_NONE)
	{
		AMXPrintError(this, &m_amx, err);
		logprintf("Failed to load '%s' script.", szGameModeFileName);
		return false;
	}

	amx_CoreInit(&m_amx);
	amx_FloatInit(&m_amx);
	amx_StringInit(&m_amx);
	amx_FileInit(&m_amx);
	amx_TimeInit(&m_amx);
	amx_CustomInit(&m_amx);
	amx_sampDbInit(&m_amx);

	m_bInitialised = true;

	// Execute OnGameModeInit callback, if it exists!
	int tmp;
	if (!amx_FindPublic(&m_amx, "OnGameModeInit", &tmp))
		amx_Exec(&m_amx, (cell*)&tmp, tmp);
	pNetGame->GetFilterScripts()->OnGameModeInit();
	// ----------------------------------------------

	// Call in filterscripts
	pNetGame->GetFilterScripts()->OnGameModeInit();

	cell ret = 0;
	err = amx_Exec(&m_amx, &ret, AMX_EXEC_MAIN);
	if (err == AMX_ERR_SLEEP)
	{
		m_bSleeping = true;
		m_fSleepTime = ((float)ret / 1000.0f);
	}
	else if (err != AMX_ERR_NONE)
	{
		m_bSleeping = false;
		AMXPrintError(this, &m_amx, err);
	}

	return true;
}

//----------------------------------------------------------------------------------

void CGameMode::Unload()
{
	// Execute OnGameModeExit callback, if it exists!
	int tmp;
	if (!amx_FindPublic(&m_amx, "OnGameModeExit", &tmp))
		amx_Exec(&m_amx, (cell*)&tmp, tmp);
	// ----------------------------------------------

	// Call in filterscripts
	pNetGame->GetFilterScripts()->OnGameModeExit();
	CScriptTimers* pScriptTimers = pNetGame->GetTimers();
	if(pScriptTimers) pScriptTimers->DeleteForMode(&m_amx);

	if (m_bInitialised)
	{
		aux_FreeProgram(&m_amx);
		amx_sampDbCleanup(&m_amx);
		amx_TimeCleanup(&m_amx);
		amx_FileCleanup(&m_amx);
		amx_StringCleanup(&m_amx);
		amx_FloatCleanup(&m_amx);
		amx_CoreCleanup(&m_amx);
	}
	m_bInitialised = false;
	m_bSleeping = false;
}

//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------

int CGameMode::CallPublic(char* szFuncName)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, szFuncName, &idx))
		amx_Exec(&m_amx, &ret, idx);
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerConnect(playerid);
int CGameMode::OnPlayerConnect(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerConnect", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerDisconnect(playerid, reason);
int CGameMode::OnPlayerDisconnect(cell playerid, cell reason)
{
	//CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerDisconnect", &idx))
	{
		amx_Push(&m_amx, reason);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerSpawn(playerid);
int CGameMode::OnPlayerSpawn(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerSpawn", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerDeath(playerid, killerid, reason);
int CGameMode::OnPlayerDeath(cell playerid, cell killerid, cell reason)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerDeath", &idx))
	{
		amx_Push(&m_amx, reason);
		amx_Push(&m_amx, killerid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleSpawn(vehicleid);
int CGameMode::OnVehicleSpawn(cell vehicleid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnVehicleSpawn", &idx))
	{
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleDeath(vehicleid, killerid);
int CGameMode::OnVehicleDeath(cell vehicleid, cell killerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnVehicleDeath", &idx))
	{
		amx_Push(&m_amx, killerid);
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerText(playerid, text[]);
int CGameMode::OnPlayerText(cell playerid, unsigned char * szText)
{
	CHECK_INIT();

	int idx;
	cell ret = 1;	// DEFAULT TO 1!
	int orig_strlen = strlen((char*)szText) + 1;

	if (!amx_FindPublic(&m_amx, "OnPlayerText", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_PushString(&m_amx, &amx_addr, &phys_addr, (char*)szText, 0, 0);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
		amx_GetString((char*)szText, phys_addr, 0, orig_strlen);
		amx_Release(&m_amx, amx_addr);
	}

	if (ret && pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid)) {
		pNetGame->GetPlayerPool()->GetAt((PLAYERID)playerid)->Say(szText, strlen((char*)szText));
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerCommandText(playerid, cmdtext[]);
int CGameMode::OnPlayerCommandText(cell playerid, unsigned char * szCommandText)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;
	//int orig_strlen = strlen((char*)szCommandText);

	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerCommandText", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_PushString(&m_amx, &amx_addr, &phys_addr, (char*)szCommandText, 0, 0);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
		amx_Release(&m_amx, amx_addr);
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerInfoChange(playerid);
int CGameMode::OnPlayerInfoChange(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerInfoChange", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerRequestClass(playerid, classid);
int CGameMode::OnPlayerRequestClass(cell playerid, cell classid)
{
	CHECK_INIT();

	int idx;
	cell ret = 1;	// DEFAULT TO 1!
	
	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerRequestClass", &idx))
	{
		amx_Push(&m_amx, classid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerRequestSpawn(playerid);
int CGameMode::OnPlayerRequestSpawn(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 1;	// DEFAULT TO 1!

	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerRequestSpawn", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
int CGameMode::OnPlayerEnterVehicle(cell playerid, cell vehicleid, cell ispassenger)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerEnterVehicle", &idx))
	{
		amx_Push(&m_amx, ispassenger);
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerExitVehicle(playerid, vehicleid);
int CGameMode::OnPlayerExitVehicle(cell playerid, cell vehicleid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;
	
	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerExitVehicle", &idx))
	{
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStateChange(playerid, newstate, oldstate);
int CGameMode::OnPlayerStateChange(cell playerid, cell newstate, cell oldstate)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;
	
	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerStateChange", &idx))
	{
		amx_Push(&m_amx, oldstate);
		amx_Push(&m_amx, newstate);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------
// forward OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);

int CGameMode::OnPlayerInteriorChange(cell playerid, cell newid, cell oldid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;
	
	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerInteriorChange", &idx))
	{
		amx_Push(&m_amx, oldid);
		amx_Push(&m_amx, newid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEnterCheckpoint(playerid);
int CGameMode::OnPlayerEnterCheckpoint(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;
	
	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if (!amx_FindPublic(&m_amx, "OnPlayerEnterCheckpoint", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerLeaveCheckpoint(playerid);
int CGameMode::OnPlayerLeaveCheckpoint(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerLeaveCheckpoint", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEnterRaceCheckpoint(playerid);
int CGameMode::OnPlayerEnterRaceCheckpoint(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerEnterRaceCheckpoint", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerLeaveRaceCheckpoint(playerid);
int CGameMode::OnPlayerLeaveRaceCheckpoint(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerLeaveRaceCheckpoint", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
int CGameMode::OnPlayerKeyStateChange(cell playerid, cell newkeys, cell oldkeys)
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerKeyStateChange", &idx))
	{
		amx_Push(&m_amx, oldkeys);
		amx_Push(&m_amx, newkeys);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnRconCommand(cmd[]);
int CGameMode::OnRconCommand(char* szCommand)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;
	//int orig_strlen = strlen(szCommand);

	if (!amx_FindPublic(&m_amx, "OnRconCommand", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_PushString(&m_amx, &amx_addr, &phys_addr, szCommand, 0, 0);
		amx_Exec(&m_amx, &ret, idx);
		amx_Release(&m_amx, amx_addr);
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnObjectMoved(objectid);
int CGameMode::OnObjectMoved(cell objectid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnObjectMoved", &idx))
	{
		amx_Push(&m_amx, objectid);
		amx_Exec(&m_amx, &ret, idx);
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerObjectMoved(playerid, objectid);
int CGameMode::OnPlayerObjectMoved(cell playerid, cell objectid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerObjectMoved", &idx))
	{
		amx_Push(&m_amx, objectid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerPickedUpPickup(playerid, pickupid);
int CGameMode::OnPlayerPickedUpPickup(cell playerid, cell pickupid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerPickUpPickup", &idx))
	{
		amx_Push(&m_amx, pickupid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerExitedMenu(playerid);
int CGameMode::OnPlayerExitedMenu(cell playerid)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerExitedMenu", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerSelectedMenuRow(playerid, row);
int CGameMode::OnPlayerSelectedMenuRow(cell playerid, cell row)
{
	CHECK_INIT();

	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerSelectedMenuRow", &idx))
	{
		amx_Push(&m_amx, row);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleRespray(playerid, vehicleid, color1, color2);
int CGameMode::OnVehicleRespray(cell playerid, cell vehicleid, cell color1, cell color2)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnVehicleRespray", &idx))
	{
		amx_Push(&m_amx, color2);
		amx_Push(&m_amx, color1);
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleMod(playerid, vehicleid, componentid);
int CGameMode::OnVehicleMod(cell playerid, cell vehicleid, cell componentid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnVehicleMod", &idx))
	{
		amx_Push(&m_amx, componentid);
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnEnterExitModShop(playerid, enterexit, interiorid);
int CGameMode::OnEnterExitModShop(cell playerid, cell enterexit, cell interiorid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnEnterExitModShop", &idx))
	{
		amx_Push(&m_amx, interiorid);
		amx_Push(&m_amx, enterexit);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehiclePaintjob(playerid, vehicleid, paintjobid);
int CGameMode::OnVehiclePaintjob(cell playerid, cell vehicleid, cell paintjobid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnVehiclePaintjob", &idx))
	{
		amx_Push(&m_amx, paintjobid);
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnScriptCash(playerid, amount, increasetype);
int CGameMode::OnScriptCash(cell playerid, cell amount, cell type)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnScriptCash", &idx))
	{
		amx_Push(&m_amx, type);
		amx_Push(&m_amx, amount);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnRconLoginAttempt( ip[], password[], success );
int CGameMode::OnRconLoginAttempt(char *szIP, char *szPassword, cell success)
{
	CHECK_INIT();
	int idx;
	cell ret;

	if(!amx_FindPublic(&m_amx, "OnRconLoginAttempt", &idx))
	{
		cell amx_addr, amx_addr2, *phys_addr;
		amx_Push(&m_amx, success);
		amx_PushString(&m_amx, &amx_addr, &phys_addr, szPassword, 0, 0);
		amx_PushString(&m_amx, &amx_addr2, &phys_addr, szIP, 0, 0);
		amx_Exec(&m_amx, &ret, idx);
		amx_Release(&m_amx, amx_addr);
		amx_Release(&m_amx, amx_addr2);
	}
	return 1;
}

//----------------------------------------------------------------------------------
// forward OnPlayerUpdate(playerid)

int CGameMode::OnPlayerUpdate(cell playerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnPlayerUpdate", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStreamIn(playerid, forplayerid);
int CGameMode::OnPlayerStreamIn(cell playerid, cell forplayerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnPlayerStreamIn", &idx))
	{
		amx_Push(&m_amx, forplayerid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStreamOut(playerid, forplayerid);
int CGameMode::OnPlayerStreamOut(cell playerid, cell forplayerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnPlayerStreamOut", &idx))
	{
		amx_Push(&m_amx, forplayerid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleStreamIn(vehicleid, forplayerid);
int CGameMode::OnVehicleStreamIn(cell vehicleid, cell forplayerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnVehicleStreamIn", &idx))
	{
		amx_Push(&m_amx, forplayerid);
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleStreamOut(vehicleid, forplayerid);
int CGameMode::OnVehicleStreamOut(cell vehicleid, cell forplayerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnVehicleStreamOut", &idx))
	{
		amx_Push(&m_amx, forplayerid);
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnActorStreamIn(actorid, forplayerid);
int CGameMode::OnActorStreamIn(cell actorid, cell forplayerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnActorStreamIn", &idx))
	{
		amx_Push(&m_amx, forplayerid);
		amx_Push(&m_amx, actorid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnActorStreamOut(actorid, forplayerid);
int CGameMode::OnActorStreamOut(cell actorid, cell forplayerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnActorStreamOut", &idx))
	{
		amx_Push(&m_amx, forplayerid);
		amx_Push(&m_amx, actorid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
int CGameMode::OnDialogResponse(cell playerid, cell dialogid, cell response, cell listitem, char *szInputText)
{
	CHECK_INIT();
	int idx;
	cell ret = 0;

	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return (int)ret;

	if(!amx_FindPublic(&m_amx, "OnDialogResponse", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_PushString(&m_amx, &amx_addr, &phys_addr, szInputText, 0, 0);
		amx_Push(&m_amx, listitem);
		amx_Push(&m_amx, response);
		amx_Push(&m_amx, dialogid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
		amx_Release(&m_amx, amx_addr);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickPlayer(playerid, clickedplayerid, source);
int CGameMode::OnPlayerClickPlayer(cell playerid, cell clickedplayerid, cell source)
{
	CHECK_INIT();
	int idx;
	cell ret = 0;

	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)playerid))
		return 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerClickPlayer", &idx))
	{
		amx_Push(&m_amx, source);
		amx_Push(&m_amx, clickedplayerid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
int CGameMode::OnPlayerTakeDamage(cell playerid, cell issuerid, float amount, cell weaponid, cell bodypart)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerTakeDamage", &idx))
	{
		amx_Push(&m_amx, bodypart);
		amx_Push(&m_amx, weaponid);
		amx_Push(&m_amx, amx_ftoc(amount));
		amx_Push(&m_amx, issuerid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
int CGameMode::OnPlayerGiveDamage(cell playerid, cell damagedid, float amount, cell weaponid, cell bodypart)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerGiveDamage", &idx))
	{
		amx_Push(&m_amx, bodypart);
		amx_Push(&m_amx, weaponid);
		amx_Push(&m_amx, amx_ftoc(amount));
		amx_Push(&m_amx, damagedid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerGiveDamageActor(playerid, damaged_actorid, Float:amount, weaponid, bodypart);
int CGameMode::OnPlayerGiveDamageActor(cell playerid, cell damaged_actorid, float amount, cell weaponid, cell bodypart)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerGiveDamageActor", &idx))
	{
		amx_Push(&m_amx, bodypart);
		amx_Push(&m_amx, weaponid);
		amx_Push(&m_amx, amx_ftoc(amount));
		amx_Push(&m_amx, damaged_actorid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleDamageStatusUpdate(vehicleid, playerid);
int CGameMode::OnVehicleDamageStatusUpdate(cell vehicleid, cell playerid)
{
	CHECK_INIT();
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnVehicleDamageStatusUpdate", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
int CGameMode::OnUnoccupiedVehicleUpdate(cell vehicleid, cell playerid, cell passenger_seat, int a5, PVECTOR vecNew, PVECTOR vecVel)
{
	CHECK_INIT();
	int idx;
	cell ret = 1;

	cell vel_y = amx_ftoc(vecVel->Y);
	cell vel_z = amx_ftoc(vecVel->Z);
	cell vel_x = amx_ftoc(vecVel->X);
	cell new_x = amx_ftoc(vecNew->X);
	cell new_y = amx_ftoc(vecNew->Y);
	cell new_z = amx_ftoc(vecNew->Z);

	if(!amx_FindPublic(&m_amx, "OnUnoccupiedVehicleUpdate", &idx))
	{
		amx_Push(&m_amx, vel_z);
		amx_Push(&m_amx, vel_y);
		amx_Push(&m_amx, vel_x);
		amx_Push(&m_amx, new_z);
		amx_Push(&m_amx, new_y);
		amx_Push(&m_amx, new_x);
		amx_Push(&m_amx, passenger_seat);
		amx_Push(&m_amx, playerid);
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
int CGameMode::OnPlayerClickMap(cell playerid, float fX, float fY, float fZ)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerClickMap", &idx))
	{
		amx_Push(&m_amx, amx_ftoc(fZ));
		amx_Push(&m_amx, amx_ftoc(fY));
		amx_Push(&m_amx, amx_ftoc(fX));
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid,
//		Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ,
//		Float:fRotX, Float:fRotY, Float:fRotZ,
//		Float:fScaleX, Float:fScaleY, Float:fScaleZ );
int CGameMode::OnPlayerEditAttachedObject(cell playerid, cell index, cell response, struc_64 *pInfo)
{
	int idx;
	cell ret = 0;

	cell offset_x = amx_ftoc(pInfo->vecOffset.X);
	cell offset_y = amx_ftoc(pInfo->vecOffset.Y);
	cell offset_z = amx_ftoc(pInfo->vecOffset.Z);
	cell rot_x = amx_ftoc(pInfo->vecRotation.X);
	cell rot_y = amx_ftoc(pInfo->vecRotation.Y);
	cell rot_z = amx_ftoc(pInfo->vecRotation.Z);
	cell scale_x = amx_ftoc(pInfo->vecScale.X);
	cell scale_y = amx_ftoc(pInfo->vecScale.Y);
	cell scale_z = amx_ftoc(pInfo->vecScale.Z);

	if(!amx_FindPublic(&m_amx, "OnPlayerEditAttachedObject", &idx))
	{
		amx_Push(&m_amx, scale_z);
		amx_Push(&m_amx, scale_y);
		amx_Push(&m_amx, scale_x);
		amx_Push(&m_amx, rot_z);
		amx_Push(&m_amx, rot_y);
		amx_Push(&m_amx, rot_x);
		amx_Push(&m_amx, offset_z);
		amx_Push(&m_amx, offset_y);
		amx_Push(&m_amx, offset_x);
		amx_Push(&m_amx, pInfo->iBoneID);
		amx_Push(&m_amx, pInfo->iModelID);
		amx_Push(&m_amx, index);
		amx_Push(&m_amx, response);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEditObject( playerid, playerobject, objectid, response, 
//		Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ );
int CGameMode::OnPlayerEditObject(cell playerid, cell playerobject, cell objectid, cell response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerEditObject", &idx))
	{
		amx_Push(&m_amx, amx_ftoc(fRotZ));
		amx_Push(&m_amx, amx_ftoc(fRotY));
		amx_Push(&m_amx, amx_ftoc(fRotX));
		amx_Push(&m_amx, amx_ftoc(fZ));
		amx_Push(&m_amx, amx_ftoc(fY));
		amx_Push(&m_amx, amx_ftoc(fX));
		amx_Push(&m_amx, response);
		amx_Push(&m_amx, objectid);
		amx_Push(&m_amx, playerobject);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerSelectObject(playerid, type, objectid, modelid, Float:fX, Float:fY, Float:fZ);
int CGameMode::OnPlayerSelectObject(cell playerid, cell type, cell objectid, cell modelid, float fX, float fY, float fZ)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerSelectObject", &idx))
	{
		amx_Push(&m_amx, amx_ftoc(fZ));
		amx_Push(&m_amx, amx_ftoc(fY));
		amx_Push(&m_amx, amx_ftoc(fX));
		amx_Push(&m_amx, modelid);
		amx_Push(&m_amx, objectid);
		amx_Push(&m_amx, type);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickTextDraw(playerid, Text:clickedid);
int CGameMode::OnPlayerClickTextDraw(cell playerid, cell clickedid)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerClickTextDraw", &idx))
	{
		amx_Push(&m_amx, clickedid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
int CGameMode::OnPlayerClickPlayerTextDraw(cell playerid, cell playertextid)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnPlayerClickPlayerTextDraw", &idx))
	{
		amx_Push(&m_amx, playertextid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

int CGameMode::OnClientCheckResponse(cell a2, cell a3, cell a4, cell a5)
{
	int idx;
	cell ret = 0;

	if(!amx_FindPublic(&m_amx, "OnClientCheckResponse", &idx))
	{
		amx_Push(&m_amx, a5);
		amx_Push(&m_amx, a4);
		amx_Push(&m_amx, a3);
		amx_Push(&m_amx, a2);
		amx_Exec(&m_amx, &ret, idx);
		if(ret) return 1;
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
int CGameMode::OnPlayerWeaponShot(cell playerid, cell weaponid, cell hittype, cell hitid, PVECTOR vecPos)
{
	int idx;
	cell ret = 1;

	cell x = amx_ftoc(vecPos->X);
	cell y = amx_ftoc(vecPos->Y);
	cell z = amx_ftoc(vecPos->Z);

	if(!amx_FindPublic(&m_amx, "OnPlayerWeaponShot", &idx))
	{
		amx_Push(&m_amx, z);
		amx_Push(&m_amx, y);
		amx_Push(&m_amx, x);
		amx_Push(&m_amx, hitid);
		amx_Push(&m_amx, hittype);
		amx_Push(&m_amx, weaponid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnIncomingConnection(playerid, ip_address[], port);
int CGameMode::OnIncomingConnection(cell playerid, char *ip_address, cell port)
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnIncomingConnection", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_Push(&m_amx, port);
		amx_PushString(&m_amx, &amx_addr, &phys_addr, ip_address, 0, 0);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
		amx_Release(&m_amx, amx_addr);
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnTrailerUpdate(playerid, vehicleid);
int CGameMode::OnTrailerUpdate(cell playerid, cell vehicleid)
{
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnTrailerUpdate", &idx))
	{
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleSirenStateChange(playerid, vehicleid, newstate);
int CGameMode::OnVehicleSirenStateChange(cell playerid, cell vehicleid, cell newstate)
{
	int idx;
	cell ret = 1;

	if(!amx_FindPublic(&m_amx, "OnVehicleSirenStateChange", &idx))
	{
		amx_Push(&m_amx, newstate);
		amx_Push(&m_amx, vehicleid);
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------
