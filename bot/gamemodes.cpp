
#include "main.h"

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
extern "C" int amx_DGramInit(AMX* amx);
extern "C" int amx_DGramCleanup(AMX* amx);

int AMXAPI aux_LoadProgram(AMX* amx, char* filename);
int AMXAPI aux_FreeProgram(AMX *amx);
void AMXPrintError(CGameMode* pGameMode, AMX *amx, int error);
int amx_CustomInit(AMX *amx);

char szGameModeFileName[256];

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
		return false;
	}

	amx_CoreInit(&m_amx);
	amx_FloatInit(&m_amx);
	amx_StringInit(&m_amx);
	amx_FileInit(&m_amx);
	amx_TimeInit(&m_amx);
	amx_CustomInit(&m_amx);

	// Execute OnGameModeInit callback, if it exists!
	int tmp;
	if (!amx_FindPublic(&m_amx, "OnNPCModeInit", &tmp))
		amx_Exec(&m_amx, (cell*)&tmp, tmp);
	// ----------------------------------------------

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
	// Execute OnNPCModeExit callback, if it exists!
	int tmp;
	if (!amx_FindPublic(&m_amx, "OnNPCModeExit", &tmp))
		amx_Exec(&m_amx, (cell*)&tmp, tmp);
	// ----------------------------------------------

	if (m_bInitialised)
	{
		aux_FreeProgram(&m_amx);
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

void CGameMode::Frame(float fElapsedTime)
{
	if (!m_bInitialised)
		return;

	if (!m_bSleeping)
		return;

	if (m_fSleepTime > 0.0f)
	{
		m_fSleepTime -= fElapsedTime;
	}
	else
	{
		cell ret;
		int err = amx_Exec(&m_amx, &ret, AMX_EXEC_CONT);
		if (err == AMX_ERR_SLEEP)
		{
			m_bSleeping = true;
			m_fSleepTime = ((float)ret / 1000.0f);
		}
		else
		{
			m_bSleeping = false;
			AMXPrintError(this, &m_amx, err);
		}
	}
}

//----------------------------------------------------------------------------------

int CGameMode::CallPublic(char* szFuncName)
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, szFuncName, &idx))
		amx_Exec(&m_amx, &ret, idx);
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnNPCConnect(myplayerid);
int CGameMode::OnNPCConnect(cell myplayerid)
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnNPCConnect", &idx))
	{
		amx_Push(&m_amx, myplayerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnNPCDisconnect(reason[]);
int CGameMode::OnNPCDisconnect(char *szReason)
{
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnNPCDisconnect", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_PushString(&m_amx, &amx_addr, &phys_addr, szReason, 0, 0);
		amx_Exec(&m_amx, &ret, idx);
		amx_Release(&m_amx, amx_addr);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnNPCSpawn();
int CGameMode::OnNPCSpawn()
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnNPCSpawn", &idx))
		amx_Exec(&m_amx, &ret, idx);
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnNPCEnterVehicle(vehicleid, seatid);
int CGameMode::OnNPCEnterVehicle(cell vehicleid, cell seatid)
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnNPCEnterVehicle", &idx))
	{
		amx_Push(&m_amx, seatid);
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnNPCExitVehicle();
int CGameMode::OnNPCExitVehicle()
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnNPCExitVehicle", &idx))
		amx_Exec(&m_amx, &ret, idx);
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnClientMessage(color, text[]);
int CGameMode::OnClientMessage(cell color, char *szText)
{
	int idx;
	cell ret = 1;
	int orig_strlen = strlen(szText) + 1;

	if (!amx_FindPublic(&m_amx, "OnClientMessage", &idx))
	{
		cell amx_addr, *phys_addr;
		amx_PushString(&m_amx, &amx_addr, &phys_addr, szText, 0, 0);
		amx_Push(&m_amx, color);
		amx_Exec(&m_amx, &ret, idx);
		amx_GetString(szText, phys_addr, 0, orig_strlen);
		amx_Release(&m_amx, amx_addr);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerDeath(playerid);
int CGameMode::OnPlayerDeath(cell playerid)
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnPlayerDeath", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerText(playerid, text[]);
int CGameMode::OnPlayerText(cell playerid, unsigned char * szText)
{
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
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStreamIn(playerid);
int CGameMode::OnPlayerStreamIn(cell playerid)
{
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnPlayerStreamIn", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStreamOut(playerid);
int CGameMode::OnPlayerStreamOut(cell playerid)
{
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnPlayerStreamOut", &idx))
	{
		amx_Push(&m_amx, playerid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleStreamIn(vehicleid);
int CGameMode::OnVehicleStreamIn(cell vehicleid)
{
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnVehicleStreamIn", &idx))
	{
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleStreamOut(vehicleid);
int CGameMode::OnVehicleStreamOut(cell vehicleid)
{
	int idx;
	cell ret = 1;

	if (!amx_FindPublic(&m_amx, "OnVehicleStreamOut", &idx))
	{
		amx_Push(&m_amx, vehicleid);
		amx_Exec(&m_amx, &ret, idx);
	}
	return ret;
}

//----------------------------------------------------------------------------------

// forward OnRecordingPlaybackEnd();
int CGameMode::OnRecordingPlaybackEnd()
{
	int idx;
	cell ret = 0;

	if (!amx_FindPublic(&m_amx, "OnRecordingPlaybackEnd", &idx))
		amx_Exec(&m_amx, &ret, idx);
	return (int)ret;
}

//----------------------------------------------------------------------------------
