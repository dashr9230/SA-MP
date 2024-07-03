
#ifndef SAMPNPC_GAMEMODES_H
#define SAMPNPC_GAMEMODES_H

extern char szGameModeFileName[256];

//----------------------------------------------------------------------------------

class CGameMode
{
private:
	AMX m_amx;
	bool m_bInitialised;
	bool m_bSleeping;
	float m_fSleepTime;

public:
	CGameMode();
	~CGameMode();

	char* GetFileName() { return &szGameModeFileName[0]; };

	bool Load(char* pFileName);
	void Unload();
	void Frame(float fElapsedTime);

	int CallPublic(char* szFuncName);

	int OnNPCConnect(cell myplayerid);
	int OnNPCDisconnect(char *szReason);
	int OnNPCSpawn();
	int OnNPCEnterVehicle(cell vehicleid, cell seatid);
	int OnNPCExitVehicle();
	int OnClientMessage(cell color, unsigned char * szText);
	int OnPlayerDeath(cell playerid);
	int OnPlayerText(cell playerid, unsigned char * szText);
	int OnPlayerStreamIn(cell playerid);
	int OnPlayerStreamOut(cell playerid);
	int OnVehicleStreamIn(cell vehicleid);
	int OnVehicleStreamOut(cell vehicleid);
	int OnRecordingPlaybackEnd();
};

//----------------------------------------------------------------------------------

#endif
