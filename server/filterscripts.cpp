
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

int AMXAPI aux_LoadProgram(AMX* amx, char* filename);
int AMXAPI aux_LoadProgramFromMemory(AMX* amx, char* filedata);
int AMXAPI aux_FreeProgram(AMX *amx);
int amx_CustomInit(AMX *amx);
int amx_sampDbInit(AMX *amx);
int amx_sampDbCleanup(AMX *amx);

//----------------------------------------------------------------------------------

CFilterScripts::CFilterScripts()
{
	m_iFilterScriptCount = 0;
	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
		m_pFilterScripts[i] = NULL;
}

//----------------------------------------------------------------------------------

CFilterScripts::~CFilterScripts()
{
	UnloadFilterScripts();
}

//----------------------------------------------------------------------------------

bool CFilterScripts::LoadFilterScript(char* pFileName)
{
	char szFilterScriptFile[255];
	sprintf(szFilterScriptFile, "filterscripts/%s.amx", pFileName);
	if (m_iFilterScriptCount >= MAX_FILTER_SCRIPTS)
		return false;

	FILE* f = fopen(&szFilterScriptFile[0], "rb");
	if (!f) return false;
	fclose(f);

	// Find a spare slot to load the script into
	int iSlot;
	for (iSlot = 0; iSlot < MAX_FILTER_SCRIPTS; iSlot++)
	{
		if (m_pFilterScripts[iSlot] == NULL) break;
		if (strcmp(pFileName, m_szFilterScriptName[iSlot]) == 0) return false;
	}
	if (iSlot == MAX_FILTER_SCRIPTS) return false;

	m_pFilterScripts[iSlot] = new AMX;
	AMX* amx = m_pFilterScripts[iSlot];

	memset((void*)amx, 0, sizeof(AMX));
	int err = aux_LoadProgram(amx, &szFilterScriptFile[0]);
	if (err != AMX_ERR_NONE)
	{
		logprintf("Failed to load '%s.amx' filterscript.", szFilterScriptFile);
		return false;
	}

	amx_CoreInit(amx);
	amx_FloatInit(amx);
	amx_StringInit(amx);
	amx_FileInit(amx);
	amx_TimeInit(amx);
	amx_CustomInit(amx);
	amx_sampDbInit(amx);

	int tmp;
	if (!amx_FindPublic(amx, "OnFilterScriptInit", &tmp))
		amx_Exec(amx, (cell*)&tmp, tmp);

	strcpy(m_szFilterScriptName[iSlot], pFileName);

	m_iFilterScriptCount++;

	return true;
}

//----------------------------------------------------------------------------------

bool CFilterScripts::LoadFilterScriptFromMemory(char* pFileName, char* pFileData)
{
	if (m_iFilterScriptCount >= MAX_FILTER_SCRIPTS)
		return false;

	// Find a spare slot to load the script into
	int iSlot;
	for (iSlot = 0; iSlot < MAX_FILTER_SCRIPTS; iSlot++)
	{
		if (m_pFilterScripts[iSlot] == NULL) break;
		if (strcmp(pFileName, m_szFilterScriptName[iSlot]) == 0) return false;
	}
	if (iSlot == MAX_FILTER_SCRIPTS) return false;

	m_pFilterScripts[iSlot] = new AMX;
	AMX* amx = m_pFilterScripts[iSlot];

	memset((void*)amx, 0, sizeof(AMX));
	int err = aux_LoadProgramFromMemory(amx, pFileData);
	if (err != AMX_ERR_NONE)
	{
		return false;
	}

	amx_CoreInit(amx);
	amx_FloatInit(amx);
	amx_StringInit(amx);
	amx_FileInit(amx);
	amx_TimeInit(amx);
	amx_CustomInit(amx);
	amx_sampDbInit(amx);

	int tmp;
	if (!amx_FindPublic(amx, "OnFilterScriptInit", &tmp))
		amx_Exec(amx, (cell*)&tmp, tmp);

	strcpy(m_szFilterScriptName[iSlot], pFileName);

	m_iFilterScriptCount++;

	return true;
}

//----------------------------------------------------------------------------------

void CFilterScripts::UnloadFilterScripts()
{
	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			RemoveFilterScript(i);
		}
	}

	m_iFilterScriptCount = 0;
}

//----------------------------------------------------------------------------------
// Finds and unloads one filterscript

bool CFilterScripts::UnloadOneFilterScript(char* pFilterScript)
{
	int i;
	for (i = 0; i < MAX_FILTER_SCRIPTS; i++)
	{
		if (strcmp(pFilterScript, m_szFilterScriptName[i]) == 0) break;
	}
	if (i == MAX_FILTER_SCRIPTS) return false;
	if (m_pFilterScripts[i])
	{
		RemoveFilterScript(i);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------
// Unloads the individual filterscript

void CFilterScripts::RemoveFilterScript(int iIndex)
{
	int tmp;
	if (!amx_FindPublic(m_pFilterScripts[iIndex], "OnFilterScriptExit", &tmp))
		amx_Exec(m_pFilterScripts[iIndex], (cell*)&tmp, tmp);

	// Kill the timers
	if(pNetGame->GetTimers())
		pNetGame->GetTimers()->DeleteForMode(m_pFilterScripts[iIndex]);

	// Do the other stuff from before
	aux_FreeProgram(m_pFilterScripts[iIndex]);

	amx_TimeCleanup(m_pFilterScripts[iIndex]);
	amx_FileCleanup(m_pFilterScripts[iIndex]);
	amx_StringCleanup(m_pFilterScripts[iIndex]);
	amx_FloatCleanup(m_pFilterScripts[iIndex]);
	amx_CoreCleanup(m_pFilterScripts[iIndex]);
	amx_sampDbCleanup(m_pFilterScripts[iIndex]);
	SAFE_DELETE(m_pFilterScripts[iIndex]);
	m_szFilterScriptName[iIndex][0] = '\0';
	m_iFilterScriptCount--;
}

//----------------------------------------------------------------------------------

void CFilterScripts::Frame(float fElapsedTime)
{

}

//----------------------------------------------------------------------------------

int CFilterScripts::CallPublic(char* szFuncName)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], szFuncName, &idx))
			{
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerConnect(playerid);
int CFilterScripts::OnPlayerConnect(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerConnect", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerDisconnect(playerid, reason);
int CFilterScripts::OnPlayerDisconnect(cell playerid, cell reason)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts && m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerDisconnect", &idx))
			{
				amx_Push(m_pFilterScripts[i], reason);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

int CFilterScripts::OnGameModeInit()
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnGameModeInit", &idx))
			{
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

int CFilterScripts::OnGameModeExit()
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnGameModeExit", &idx))
			{
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerSpawn(playerid);
int CFilterScripts::OnPlayerSpawn(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerSpawn", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerDeath(playerid, killerid, reason);
int CFilterScripts::OnPlayerDeath(cell playerid, cell killerid, cell reason)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerDeath", &idx))
			{
				amx_Push(m_pFilterScripts[i], reason);
				amx_Push(m_pFilterScripts[i], killerid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleSpawn(vehicleid);
int CFilterScripts::OnVehicleSpawn(cell vehicleid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleSpawn", &idx))
			{
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleDeath(vehicleid, killerid);
int CFilterScripts::OnVehicleDeath(cell vehicleid, cell killerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleDeath", &idx))
			{
				amx_Push(m_pFilterScripts[i], killerid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerText(playerid, text[]);
int CFilterScripts::OnPlayerText(cell playerid, unsigned char* szText)
{
	int idx;
	cell ret = 1;	// DEFAULT TO 1!

	int orig_strlen = strlen((char*)szText) + 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerText", &idx))
			{
				cell amx_addr, *phys_addr;
				amx_PushString(m_pFilterScripts[i], &amx_addr, &phys_addr, (char*)szText, 0, 0);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				amx_GetString((char*)szText, phys_addr, 0, orig_strlen);
				amx_Release(m_pFilterScripts[i], amx_addr);
				if (!ret) return 0; // Callback returned 0, so exit and don't display the text.
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerCommandText(playerid, cmdtext[]);
int CFilterScripts::OnPlayerCommandText(cell playerid, unsigned char* szCommandText)
{
	int idx;
	cell ret = 0;

	//int orig_strlen = strlen((char*)szCommandText);

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerCommandText", &idx))
			{
				cell amx_addr, *phys_addr;
				amx_PushString(m_pFilterScripts[i], &amx_addr, &phys_addr, (char*)szCommandText, 0, 0);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				amx_Release(m_pFilterScripts[i], amx_addr);
				if (ret) return 1; // Callback returned 1, so the command was accepted!
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerInfoChange(playerid);
int CFilterScripts::OnPlayerInfoChange(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerInfoChange", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerRequestClass(playerid, classid);
int CFilterScripts::OnPlayerRequestClass(cell playerid, cell classid)
{
	int idx;
	cell ret = 1;	// DEFAULT TO 1!

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerRequestClass", &idx))
			{
				amx_Push(m_pFilterScripts[i], classid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerRequestSpawn(playerid);
int CFilterScripts::OnPlayerRequestSpawn(cell playerid)
{
	int idx;
	cell ret = 1;	// DEFAULT TO 1!

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerRequestSpawn", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
int CFilterScripts::OnPlayerEnterVehicle(cell playerid, cell vehicleid, cell ispassenger)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerEnterVehicle", &idx))
			{
				amx_Push(m_pFilterScripts[i], ispassenger);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerExitVehicle(playerid, vehicleid);
int CFilterScripts::OnPlayerExitVehicle(cell playerid, cell vehicleid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerExitVehicle", &idx))
			{
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStateChange(playerid, newstate, oldstate);
int CFilterScripts::OnPlayerStateChange(cell playerid, cell newstate, cell oldstate)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerStateChange", &idx))
			{
				amx_Push(m_pFilterScripts[i], oldstate);
				amx_Push(m_pFilterScripts[i], newstate);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
int CFilterScripts::OnPlayerInteriorChange(cell playerid, cell newid, cell oldid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerInteriorChange", &idx))
			{
				amx_Push(m_pFilterScripts[i], oldid);
				amx_Push(m_pFilterScripts[i], newid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEnterCheckpoint(playerid);
int CFilterScripts::OnPlayerEnterCheckpoint(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerEnterCheckpoint", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerLeaveCheckpoint(playerid);
int CFilterScripts::OnPlayerLeaveCheckpoint(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerLeaveCheckpoint", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEnterRaceCheckpoint(playerid);
int CFilterScripts::OnPlayerEnterRaceCheckpoint(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerEnterRaceCheckpoint", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerLeaveRaceCheckpoint(playerid);
int CFilterScripts::OnPlayerLeaveRaceCheckpoint(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerLeaveRaceCheckpoint", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
int CFilterScripts::OnPlayerKeyStateChange(cell playerid, cell newkeys, cell oldkeys)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerKeyStateChange", &idx))
			{
				amx_Push(m_pFilterScripts[i], oldkeys);
				amx_Push(m_pFilterScripts[i], newkeys);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerRequestClass(playerid, classid);
int CFilterScripts::OnRconCommand(char* szCommand)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnRconCommand", &idx))
			{
				cell amx_addr, *phys_addr;
				amx_PushString(m_pFilterScripts[i], &amx_addr, &phys_addr, szCommand, 0, 0);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				//if (!ret) return ret;
			} 
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnObjectMoved(objectid);
int CFilterScripts::OnObjectMoved(cell objectid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnObjectMoved", &idx))
			{
				amx_Push(m_pFilterScripts[i], objectid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerObjectMoved(playerid, objectid);
int CFilterScripts::OnPlayerObjectMoved(cell playerid, cell objectid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerObjectMoved", &idx))
			{
				amx_Push(m_pFilterScripts[i], objectid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerPickedUpPickup(playerid, pickupid);
int CFilterScripts::OnPlayerPickedUpPickup(cell playerid, cell pickupid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerPickUpPickup", &idx))
			{
				amx_Push(m_pFilterScripts[i], pickupid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerExitedMenu(playerid);
int CFilterScripts::OnPlayerExitedMenu(cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerExitedMenu", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerSelectedMenuRow(playerid, row);
int CFilterScripts::OnPlayerSelectedMenuRow(cell playerid, cell row)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerSelectedMenuRow", &idx))
			{
				amx_Push(m_pFilterScripts[i], row);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleRespray(playerid, vehicleid, color1, color2);
int CFilterScripts::OnVehicleRespray(cell playerid, cell vehicleid, cell color1, cell color2)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleRespray", &idx))
			{
				amx_Push(m_pFilterScripts[i], color2);
				amx_Push(m_pFilterScripts[i], color1);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleMod(playerid, vehicleid, componentid);
int CFilterScripts::OnVehicleMod(cell playerid, cell vehicleid, cell componentid)
{
	int idx;
	cell ret = 1;
	int retval = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleMod", &idx))
			{
				amx_Push(m_pFilterScripts[i], componentid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) retval = 0;
			}
		}
	}
	return retval;
}

//----------------------------------------------------------------------------------

// forward OnEnterExitModShop(playerid, enterexit, interiorid);
int CFilterScripts::OnEnterExitModShop(cell playerid, cell enterexit, cell interiorid)
{
	int idx;
	cell ret = 1;
	int retval = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnEnterExitModShop", &idx))
			{
				amx_Push(m_pFilterScripts[i], interiorid);
				amx_Push(m_pFilterScripts[i], enterexit);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) retval = 0;
			}
		}
	}
	return retval;
}

//----------------------------------------------------------------------------------

// forward OnVehiclePaintjob(playerid, vehicleid, paintjobid);
int CFilterScripts::OnVehiclePaintjob(cell playerid, cell vehicleid, cell paintjobid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehiclePaintjob", &idx))
			{
				amx_Push(m_pFilterScripts[i], paintjobid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnScriptCash(playerid, amount, increasetype);
int CFilterScripts::OnScriptCash(cell playerid, cell amount, cell type)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnScriptCash", &idx))
			{
				amx_Push(m_pFilterScripts[i], type);
				amx_Push(m_pFilterScripts[i], amount);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnRconLoginAttempt( ip[], password[], success );
int CFilterScripts::OnRconLoginAttempt(char *szIP, char *szPassword, cell success)
{
	int idx;
	cell ret;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnRconLoginAttempt", &idx))
			{
				cell amx_addr, amx_addr2, *phys_addr;
				amx_Push(m_pFilterScripts[i], success);
				amx_PushString(m_pFilterScripts[i], &amx_addr, &phys_addr, szPassword, 0, 0);
				amx_PushString(m_pFilterScripts[i], &amx_addr2, &phys_addr, szIP, 0, 0);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				amx_Release(m_pFilterScripts[i], amx_addr);
				amx_Release(m_pFilterScripts[i], amx_addr2);
			}
		}
	}
	return 1;
}

//----------------------------------------------------------------------------------

// forward OnPlayerUpdate(playerid)
int CFilterScripts::OnPlayerUpdate(cell playerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i]) 
		{
			if(!amx_FindPublic(m_pFilterScripts[i], "OnPlayerUpdate", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStreamIn(playerid, forplayerid);
int CFilterScripts::OnPlayerStreamIn(cell playerid, cell forplayerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerStreamIn", &idx))
			{
				amx_Push(m_pFilterScripts[i], forplayerid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerStreamOut(playerid, forplayerid);
int CFilterScripts::OnPlayerStreamOut(cell playerid, cell forplayerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerStreamOut", &idx))
			{
				amx_Push(m_pFilterScripts[i], forplayerid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleStreamIn(vehicleid, forplayerid);
int CFilterScripts::OnVehicleStreamIn(cell vehicleid, cell forplayerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleStreamIn", &idx))
			{
				amx_Push(m_pFilterScripts[i], forplayerid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleStreamOut(vehicleid, forplayerid);
int CFilterScripts::OnVehicleStreamOut(cell vehicleid, cell forplayerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleStreamOut", &idx))
			{
				amx_Push(m_pFilterScripts[i], forplayerid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnActorStreamIn(actorid, forplayerid);
int CFilterScripts::OnActorStreamIn(cell actorid, cell forplayerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnActorStreamIn", &idx))
			{
				amx_Push(m_pFilterScripts[i], forplayerid);
				amx_Push(m_pFilterScripts[i], actorid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnActorStreamOut(actorid, forplayerid);
int CFilterScripts::OnActorStreamOut(cell actorid, cell forplayerid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnActorStreamOut", &idx))
			{
				amx_Push(m_pFilterScripts[i], forplayerid);
				amx_Push(m_pFilterScripts[i], actorid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
int CFilterScripts::OnDialogResponse(cell playerid, cell dialogid, cell response, cell listitem, char *szInputText)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnDialogResponse", &idx))
			{
				cell amx_addr, *phys_addr;
				amx_PushString(m_pFilterScripts[i], &amx_addr, &phys_addr, szInputText, 0, 0);
				amx_Push(m_pFilterScripts[i], listitem);
				amx_Push(m_pFilterScripts[i], response);
				amx_Push(m_pFilterScripts[i], dialogid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				amx_Release(m_pFilterScripts[i], amx_addr);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickPlayer(playerid, clickedplayerid, source);
int CFilterScripts::OnPlayerClickPlayer(cell playerid, cell clickedplayerid, cell source)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerClickPlayer", &idx))
			{
				amx_Push(m_pFilterScripts[i], source);
				amx_Push(m_pFilterScripts[i], clickedplayerid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
int CFilterScripts::OnPlayerTakeDamage(cell playerid, cell issuerid, float amount, cell weaponid, cell bodypart)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerTakeDamage", &idx))
			{
				amx_Push(m_pFilterScripts[i], bodypart);
				amx_Push(m_pFilterScripts[i], weaponid);
				amx_Push(m_pFilterScripts[i], amx_ftoc(amount));
				amx_Push(m_pFilterScripts[i], issuerid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
int CFilterScripts::OnPlayerGiveDamage(cell playerid, cell damagedid, float amount, cell weaponid, cell bodypart)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerGiveDamage", &idx))
			{
				amx_Push(m_pFilterScripts[i], bodypart);
				amx_Push(m_pFilterScripts[i], weaponid);
				amx_Push(m_pFilterScripts[i], amx_ftoc(amount));
				amx_Push(m_pFilterScripts[i], damagedid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerGiveDamageActor(playerid, damaged_actorid, Float:amount, weaponid, bodypart);
int CFilterScripts::OnPlayerGiveDamageActor(cell playerid, cell damaged_actorid, float amount, cell weaponid, cell bodypart)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerGiveDamageActor", &idx))
			{
				amx_Push(m_pFilterScripts[i], bodypart);
				amx_Push(m_pFilterScripts[i], weaponid);
				amx_Push(m_pFilterScripts[i], amx_ftoc(amount));
				amx_Push(m_pFilterScripts[i], damaged_actorid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleDamageStatusUpdate(vehicleid, playerid);
int CFilterScripts::OnVehicleDamageStatusUpdate(cell vehicleid, cell playerid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleDamageStatusUpdate", &idx))
			{
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
int CFilterScripts::OnUnoccupiedVehicleUpdate(cell vehicleid, cell playerid, cell passenger_seat, int a5, PVECTOR vecPos, PVECTOR vecVelocity)
{
	int idx;
	cell ret = 1;

	cell vel_x = amx_ftoc(vecVelocity->X);
	cell vel_y = amx_ftoc(vecVelocity->Y);
	cell vel_z = amx_ftoc(vecVelocity->Z);
	cell new_x = amx_ftoc(vecPos->X);
	cell new_y = amx_ftoc(vecPos->Y);
	cell new_z = amx_ftoc(vecPos->Z);

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnUnoccupiedVehicleUpdate", &idx))
			{
				amx_Push(m_pFilterScripts[i], vel_z);
				amx_Push(m_pFilterScripts[i], vel_y);
				amx_Push(m_pFilterScripts[i], vel_x);
				amx_Push(m_pFilterScripts[i], new_z);
				amx_Push(m_pFilterScripts[i], new_y);
				amx_Push(m_pFilterScripts[i], new_x);
				amx_Push(m_pFilterScripts[i], passenger_seat);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
int CFilterScripts::OnPlayerClickMap(cell playerid, float fX, float fY, float fZ)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerClickMap", &idx))
			{
				amx_Push(m_pFilterScripts[i], amx_ftoc(fZ));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fY));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fX));
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEditAttachedObject( playerid, response, index, modelid, boneid,
//		Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ,
//		Float:fRotX, Float:fRotY, Float:fRotZ,
//		Float:fScaleX, Float:fScaleY, Float:fScaleZ );
int CFilterScripts::OnPlayerEditAttachedObject(cell playerid, cell index, cell response, struc_64 *pInfo)
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

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerEditAttachedObject", &idx))
			{
				amx_Push(m_pFilterScripts[i], scale_z);
				amx_Push(m_pFilterScripts[i], scale_y);
				amx_Push(m_pFilterScripts[i], scale_x);
				amx_Push(m_pFilterScripts[i], rot_z);
				amx_Push(m_pFilterScripts[i], rot_y);
				amx_Push(m_pFilterScripts[i], rot_x);
				amx_Push(m_pFilterScripts[i], offset_z);
				amx_Push(m_pFilterScripts[i], offset_y);
				amx_Push(m_pFilterScripts[i], offset_x);
				amx_Push(m_pFilterScripts[i], pInfo->iBoneID);
				amx_Push(m_pFilterScripts[i], pInfo->iModelID);
				amx_Push(m_pFilterScripts[i], index);
				amx_Push(m_pFilterScripts[i], response);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if(ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerEditObject( playerid, playerobject, objectid, response, 
//		Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ );
int CFilterScripts::OnPlayerEditObject(cell playerid, cell playerobject, cell objectid, cell response,
	float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerEditObject", &idx))
			{
				amx_Push(m_pFilterScripts[i], amx_ftoc(fRotZ));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fRotY));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fRotX));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fZ));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fY));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fX));
				amx_Push(m_pFilterScripts[i], response);
				amx_Push(m_pFilterScripts[i], objectid);
				amx_Push(m_pFilterScripts[i], playerobject);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if(ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerSelectObject(playerid, type, objectid, modelid, Float:fX, Float:fY, Float:fZ);
int CFilterScripts::OnPlayerSelectObject(cell playerid, cell type, cell objectid, cell modelid, float fX, float fY, float fZ)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerSelectObject", &idx))
			{
				amx_Push(m_pFilterScripts[i], amx_ftoc(fZ));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fY));
				amx_Push(m_pFilterScripts[i], amx_ftoc(fX));
				amx_Push(m_pFilterScripts[i], modelid);
				amx_Push(m_pFilterScripts[i], objectid);
				amx_Push(m_pFilterScripts[i], type);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if(ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickTextDraw(playerid, Text:clickedid);
int CFilterScripts::OnPlayerClickTextDraw(cell playerid, cell clickedid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerClickTextDraw", &idx))
			{
				amx_Push(m_pFilterScripts[i], clickedid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if(ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
int CFilterScripts::OnPlayerClickPlayerTextDraw(cell playerid, cell playertextid)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnPlayerClickPlayerTextDraw", &idx))
			{
				amx_Push(m_pFilterScripts[i], playertextid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if(ret) return 1;
			}
		}
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnClientCheckResponse();
int CFilterScripts::OnClientCheckResponse(cell a2, cell a3, cell a4, cell a5)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnClientCheckResponse", &idx))
			{
				amx_Push(m_pFilterScripts[i], a5);
				amx_Push(m_pFilterScripts[i], a4);
				amx_Push(m_pFilterScripts[i], a3);
				amx_Push(m_pFilterScripts[i], a2);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if(ret) return 1;
			}
		}
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
int CFilterScripts::OnPlayerWeaponShot(cell playerid, cell weaponid, cell hittype, cell hitid, PVECTOR vecPos)
{
	int idx;
	cell ret = 1;

	cell x = amx_ftoc(vecPos->X);
	cell y = amx_ftoc(vecPos->Y);
	cell z = amx_ftoc(vecPos->Z);

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i]) 
		{
			if(!amx_FindPublic(m_pFilterScripts[i], "OnPlayerWeaponShot", &idx))
			{
				amx_Push(m_pFilterScripts[i], z);
				amx_Push(m_pFilterScripts[i], y);
				amx_Push(m_pFilterScripts[i], x);
				amx_Push(m_pFilterScripts[i], hitid);
				amx_Push(m_pFilterScripts[i], hittype);
				amx_Push(m_pFilterScripts[i], weaponid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				if (!ret) return ret;
			}
		}
	}

	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnIncomingConnection(playerid, ip_address[], port);
int CFilterScripts::OnIncomingConnection(cell playerid, char *ip_address, cell port)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnIncomingConnection", &idx))
			{
				cell amx_addr, *phys_addr;
				amx_Push(m_pFilterScripts[i], port);
				amx_PushString(m_pFilterScripts[i], &amx_addr, &phys_addr, ip_address, 0, 0);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
				amx_Release(m_pFilterScripts[i], amx_addr);
				if (ret) return 1;
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnTrailerUpdate(playerid, vehicleid);
int CFilterScripts::OnTrailerUpdate(cell playerid, cell vehicleid)
{
	int idx;
	cell ret = 1;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i]) 
		{
			if(!amx_FindPublic(m_pFilterScripts[i], "OnTrailerUpdate", &idx))
			{
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------

// forward OnVehicleSirenStateChange(playerid, vehicleid, newstate);
int CFilterScripts::OnVehicleSirenStateChange(cell playerid, cell vehicleid, cell newstate)
{
	int idx;
	cell ret = 0;

	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
	{
		if (m_pFilterScripts[i])
		{
			if (!amx_FindPublic(m_pFilterScripts[i], "OnVehicleSirenStateChange", &idx))
			{
				amx_Push(m_pFilterScripts[i], newstate);
				amx_Push(m_pFilterScripts[i], vehicleid);
				amx_Push(m_pFilterScripts[i], playerid);
				amx_Exec(m_pFilterScripts[i], &ret, idx);
			}
		}
	}
	return (int)ret;
}

//----------------------------------------------------------------------------------
