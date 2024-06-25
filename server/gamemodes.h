
#ifndef SAMPSRV_GAMEMODES_H
#define SAMPSRV_GAMEMODES_H

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

	int OnPlayerConnect(cell playerid);
	int OnPlayerDisconnect(cell playerid, cell reason);
	int OnPlayerSpawn(cell playerid);
	int OnPlayerDeath(cell playerid, cell killerid, cell reason);
	int OnVehicleSpawn(cell vehicleid);
	int OnVehicleDeath(cell vehicleid, cell killerid);
	int OnPlayerText(cell playerid, unsigned char * szText);
	int OnPlayerCommandText(cell playerid, unsigned char * szCommandText);
	int OnPlayerInfoChange(cell playerid);
	int OnPlayerRequestClass(cell playerid, cell classid);
	int OnPlayerRequestSpawn(cell playerid);
	int OnPlayerEnterVehicle(cell playerid, cell vehicleid, cell ispassenger);
	int OnPlayerExitVehicle(cell playerid, cell vehicleid);
	int OnPlayerStateChange(cell playerid, cell newstate, cell oldstate);
	int OnPlayerEnterCheckpoint(cell playerid);
	int OnPlayerLeaveCheckpoint(cell playerid);
	int OnPlayerEnterRaceCheckpoint(cell playerid);
	int OnPlayerLeaveRaceCheckpoint(cell playerid);
	int OnRconCommand(char* szCommand);
	int OnObjectMoved(cell objectid);
	int OnPlayerObjectMoved(cell playerid, cell objectid);
	int OnPlayerPickedUpPickup(cell playerid, cell pickupid);
	int OnPlayerExitedMenu(cell playerid);
	int OnPlayerSelectedMenuRow(cell playerid, cell row);
	int OnVehicleRespray(cell playerid, cell vehicleid, cell color1, cell color2);
	int OnVehicleMod(cell playerid, cell vehicleid, cell componentid);
	int OnEnterExitModShop(cell playerid, cell enterexit, cell interiorid);
	int OnVehiclePaintjob(cell playerid, cell vehicleid, cell paintjobid);
	int OnPlayerInteriorChange(cell playerid, cell newid, cell oldid);
	int OnPlayerKeyStateChange(cell playerid, cell newkeys, cell oldkeys);
	int OnScriptCash(cell playerid, cell amount, cell type);
	int OnRconLoginAttempt(char *szIP, char *szPassword, cell success);
	int OnPlayerUpdate(cell playerid);
	int OnPlayerStreamIn(cell playerid, cell forplayerid);
	int OnPlayerStreamOut(cell playerid, cell forplayerid);
	int OnVehicleStreamIn(cell vehicleid, cell forplayerid);
	int OnVehicleStreamOut(cell vehicleid, cell forplayerid);
	int OnActorStreamIn(cell actorid, cell forplayerid);
	int OnActorStreamOut(cell actorid, cell forplayerid);
	int OnDialogResponse(cell playerid, cell dialogid, cell response, cell listitem, char *szInputText);
	int OnPlayerClickPlayer(cell playerid, cell clickedplayerid, cell source);
	int OnPlayerTakeDamage(cell playerid, cell issuerid, float amount, cell weaponid, cell bodypart);
	int OnPlayerGiveDamage(cell playerid, cell damagedid, float amount, cell weaponid, cell bodypart);
	int OnPlayerGiveDamageActor(cell playerid, cell damaged_actorid, float amount, cell weaponid, cell bodypart);
	int OnVehicleDamageStatusUpdate(cell vehicleid, cell playerid);
	int OnUnoccupiedVehicleUpdate(cell vehicleid, cell playerid, cell passenger_seat, int a5, PVECTOR vecNew, PVECTOR vecVel);
	int OnPlayerClickMap(cell playerid, float fX, float fY, float fZ);
	int OnPlayerEditAttachedObject(cell playerid, cell index, cell response, struc_64 *pInfo);
	int OnPlayerEditObject(cell playerid, cell playerobject, cell objectid, cell response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
	int OnPlayerSelectObject(cell playerid, cell type, cell objectid, cell modelid, float fX, float fY, float fZ);
	int OnPlayerClickTextDraw(cell playerid, cell clickedid);
	int OnPlayerClickPlayerTextDraw(cell playerid, cell playertextid);
	int OnClientCheckResponse(cell a2, cell a3, cell a4, cell a5); // unused
	int OnPlayerWeaponShot(cell playerid, cell weaponid, cell hittype, cell hitid, PVECTOR vecPos);
	int OnIncomingConnection(cell playerid, char *ip_address, cell port);
	int OnTrailerUpdate(cell playerid, cell vehicleid);
	int OnVehicleSirenStateChange(cell playerid, cell vehicleid, cell newstate);
};

//----------------------------------------------------------------------------------

#endif
