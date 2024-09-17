
#include "main.h"

#define CHECK_PARAMS(n)

int set_amxstring(AMX *amx,cell amx_addr,const char *source,int max);

extern BOOL bGameModeFinished;

//----------------------------------------------------------------------------------

// native gpci(playerid, const clientid[], len) 
static cell AMX_NATIVE_CALL n_gpci(AMX *amx, cell *params)
{
	PLAYERID playerId = (PLAYERID)params[1];
	if (playerId >= MAX_PLAYERS || !pNetGame->GetPlayerPool()->GetSlotState(playerId)) return 0;
	return set_amxstring(amx, params[2], pNetGame->GetPlayerPool()->
		GetPlayerClientID(playerId), params[3]);
}

//----------------------------------------------------------------------------------
// native GetPlayerPoolSize()
static cell AMX_NATIVE_CALL n_GetPlayerPoolSize(AMX *amx, cell *params)
{
	if(!pNetGame) return -1;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(pPlayerPool) {
		return pPlayerPool->GetPoolSize();
	}
	return -1;
}

//----------------------------------------------------------------------------------
// native GetVehiclePoolSize()
static cell AMX_NATIVE_CALL n_GetVehiclePoolSize(AMX *amx, cell *params)
{
	if(!pNetGame) return -1;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(pVehiclePool) {
		return pVehiclePool->GetPoolSize();
	}
	return -1;
}

//----------------------------------------------------------------------------------
// native GetActorPoolSize()
static cell AMX_NATIVE_CALL n_GetActorPoolSize(AMX *amx, cell *params)
{
	if(!pNetGame) return -1;
	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(pActorPool) {
		return pActorPool->GetPoolSize();
	}
	return -1;
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_print(AMX *amx, cell *params)
{
	// TODO: print
	return 0;
}

static cell AMX_NATIVE_CALL n_printf(AMX *amx, cell *params)
{
	// TODO: printf
	return 0;
}

static cell AMX_NATIVE_CALL n_format(AMX *amx, cell *params)
{
	// TODO: format
	return 0;
}

static cell AMX_NATIVE_CALL n_SetTimer(AMX *amx, cell *params)
{
	char* szFuncName;
	amx_StrParam(amx, params[1], szFuncName);
	return pNetGame->GetTimers()->New(szFuncName, params[2], params[3], amx);
}

static cell AMX_NATIVE_CALL n_KillTimer(AMX *amx, cell *params)
{
	pNetGame->GetTimers()->Kill(params[1]);

	return 1;
}

//----------------------------------------------------------------------------------

// native GetTickCount()
static cell AMX_NATIVE_CALL n_GetTickCount(AMX *amx, cell *params)
{
	CHECK_PARAMS(0);

	return (cell)pNetGame->GetTime();
}

//----------------------------------------------------------------------------------
// native GetMaxPlayers()
static cell AMX_NATIVE_CALL n_GetMaxPlayers(AMX *amx, cell *params)
{
	CHECK_PARAMS(0);

	extern CConsole *pConsole;
	return pConsole->GetIntVariable("maxplayers");
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_SetTimerEx(AMX *amx, cell *params)
{
	if (params[0] < 4 * sizeof (cell))
	{
		logprintf("SCRIPT: Bad parameter count (%d < 4): ", params[0]);
		return 0;
	}
	else if (params[0] > 260 * sizeof (cell))
	{
		logprintf("SCRIPT: Bad parameter count (%d > 260): ", params[0]);
		return 0;
	}

	char* szFuncName;
	amx_StrParam(amx, params[1], szFuncName);
	return pNetGame->GetTimers()->NewEx(szFuncName, params[2], params[3], params, amx);
}

//----------------------------------------------------------------------------------
// native LimitGlobalChatRadius(Float:chat_radius)
static cell AMX_NATIVE_CALL n_LimitGlobalChatRadius(AMX *amx, cell *params)
{
	float fRadius = amx_ctof(params[1]);

	pNetGame->m_bLimitGlobalChatRadius = true;
	pNetGame->m_fGlobalChatRadius = fRadius;

	return 1;
}

//----------------------------------------------------------------------------------
// native LimitPlayerMarkerRadius(Float:marker_radius)
static cell AMX_NATIVE_CALL n_LimitPlayerMarkerRadius(AMX *amx, cell *params)
{
	float fRadius = amx_ctof(params[1]);

	pNetGame->m_bLimitPlayerMarkerRadius = true;
	pNetGame->m_fPlayerMarkerRadius = fRadius;

	return 1;
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_SetWeather(AMX *amx, cell *params)
{
	// TODO: SetWeather
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerWeather(AMX *amx, cell *params)
{
	// TODO: SetPlayerWeather
	return 0;
}

static cell AMX_NATIVE_CALL n_CallRemoteFunction(AMX *amx, cell *params)
{
	// TODO: CallRemoteFunction
	return 0;
}

static cell AMX_NATIVE_CALL n_CallLocalFunction(AMX *amx, cell *params)
{
	// TODO: CallLocalFunction
	return 0;
}

//----------------------------------------------------------------------------------
// native Float:VectorSize(Float:x, Float:y, Float:z)
static cell AMX_NATIVE_CALL n_VectorSize(AMX *amx, cell *params)
{
	VECTOR vec;
	vec.X = amx_ctof(params[1]);
	vec.Y = amx_ctof(params[2]);
	vec.Z = amx_ctof(params[3]);
	float fResult = GetLength(&vec);
	return amx_ftoc(fResult);
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_asin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(asin(amx_ctof(params[1])) * (180.0f / PI));
	return amx_ftoc(fResult);
}

static cell AMX_NATIVE_CALL n_acos(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(acos(amx_ctof(params[1])) * (180.0f / PI));
	return amx_ftoc(fResult);
}

static cell AMX_NATIVE_CALL n_atan(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(atan(amx_ctof(params[1])) * (180.0f / PI));
	return amx_ftoc(fResult);
}

static cell AMX_NATIVE_CALL n_atan2(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	float fResult = (float)(atan2(amx_ctof(params[1]), amx_ctof(params[2])) * (180.0f / PI));
	return amx_ftoc(fResult);
}

//----------------------------------------------------

static cell AMX_NATIVE_CALL n_SHA256_PassHash(AMX *amx, cell *params)
{
	// TODO: SHA256_PassHash
	return 0;
}

static cell AMX_NATIVE_CALL n_SetSVarInt(AMX *amx, cell *params)
{
	// TODO: SetSVarInt
	return 0;
}

static cell AMX_NATIVE_CALL n_SetSVarString(AMX *amx, cell *params)
{
	// TODO: SetSVarString
	return 0;
}

static cell AMX_NATIVE_CALL n_SetSVarFloat(AMX *amx, cell *params)
{
	// TODO: SetSVarFloat
	return 0;
}

static cell AMX_NATIVE_CALL n_GetSVarInt(AMX *amx, cell *params)
{
	// TODO: GetSVarInt
	return 0;
}

static cell AMX_NATIVE_CALL n_GetSVarString(AMX *amx, cell *params)
{
	// TODO: GetSVarString
	return 0;
}

static cell AMX_NATIVE_CALL n_GetSVarFloat(AMX *amx, cell *params)
{
	// TODO: GetSVarFloat
	return 0;
}

static cell AMX_NATIVE_CALL n_DeleteSVar(AMX *amx, cell *params)
{
	// TODO: DeleteSVar
	return 0;
}

static cell AMX_NATIVE_CALL n_GetSVarType(AMX *amx, cell *params)
{
	// TODO: GetSVarType
	return 0;
}

static cell AMX_NATIVE_CALL n_GetSVarNameAtIndex(AMX *amx, cell *params)
{
	// TODO: GetSVarNameAtIndex
	return 0;
}

static cell AMX_NATIVE_CALL n_GetSVarsUpperIndex(AMX *amx, cell *params)
{
	// TODO: GetSVarsUpperIndex
	return 0;
}

// native GameModeExit()
static cell AMX_NATIVE_CALL n_GameModeExit(AMX *amx, cell *params)
{
	if(pNetGame->SetNextScriptFile(NULL)) {
		bGameModeFinished = TRUE;
	} else {
		logprintf("The gamemode finished and I couldn't start another script.");
		fcloseall();
		exit(1);
	}
	return 0;
}

// native SetGameModeText(const string[])
static cell AMX_NATIVE_CALL n_SetGameModeText(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);

	char* szGameModeText;
	amx_StrParam(amx, params[1], szGameModeText);
	pConsole->SetStringVariable("gamemodetext", szGameModeText);

	return 0;
}

// native SetTeamCount(count)
static cell AMX_NATIVE_CALL n_SetTeamCount(AMX *amx, cell *params)
{
	return 0;
}

static cell AMX_NATIVE_CALL n_AddPlayerClass(AMX *amx, cell *params)
{
	// TODO: AddPlayerClass
	return 0;
}

static cell AMX_NATIVE_CALL n_AddPlayerClassEx(AMX *amx, cell *params)
{
	// TODO: AddPlayerClassEx
	return 0;
}

static cell AMX_NATIVE_CALL n_AddStaticVehicle(AMX *amx, cell *params)
{
	// TODO: AddStaticVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_AddStaticVehicleEx(AMX *amx, cell *params)
{
	// TODO: AddStaticVehicleEx
	return 0;
}

static cell AMX_NATIVE_CALL n_AddStaticPickup(AMX *amx, cell *params)
{
	// TODO: AddStaticPickup
	return 0;
}

static cell AMX_NATIVE_CALL n_CreatePickup(AMX *amx, cell *params)
{
	// TODO: CreatePickup
	return 0;
}

static cell AMX_NATIVE_CALL n_DestroyPickup(AMX *amx, cell *params)
{
	// TODO: DestroyPickup
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerWorldBounds(AMX *amx, cell *params)
{
	// TODO: SetPlayerWorldBounds
	return 0;
}

//----------------------------------------------------------------------------------

// native ShowNameTags(show)
static cell AMX_NATIVE_CALL n_ShowNameTags(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	pNetGame->m_bShowNameTags = (bool)params[1];
	return 1;
}

//----------------------------------------------------------------------------------

// native ShowPlayerMarkers(mode)
static cell AMX_NATIVE_CALL n_ShowPlayerMarkers(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	pNetGame->m_iShowPlayerMarkers = params[1];
	return 1;
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_SetWorldTime(AMX *amx, cell *params)
{
	// TODO: SetWorldTime
	return 0;
}

// native GetWeaponName(weaponid, const weapon[], len)
static cell AMX_NATIVE_CALL n_GetWeaponName(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	if(params[1] > WEAPON_COLLISION) return 0;

	return set_amxstring(amx,params[2],pNetGame->GetWeaponName(params[1]),params[3]);
}

//----------------------------------------------------------------------------------

// native EnableTirePopping(enable)
static cell AMX_NATIVE_CALL n_EnableTirePopping(AMX *amx, cell *params)
{
	logprintf("EnableTirePopping() function is removed.");
	return 1;
}

//----------------------------------------------------------------------------------
// native EnableVehicleFriendlyFire()
static cell AMX_NATIVE_CALL n_EnableVehicleFriendlyFire(AMX *amx, cell *params)
{
	pNetGame->m_bVehicleFriendlyFire = TRUE;
	return 1;
}

//----------------------------------------------------------------------------------

// native AllowInteriorWeapons(allow)
static cell AMX_NATIVE_CALL n_AllowInteriorWeapons(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	pNetGame->m_bAllowWeapons = (bool)params[1];
	return 1;
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_SetGravity(AMX *amx, cell *params)
{
	// TODO: SetGravity
	return 0;
}

// native GetGravity()
static cell AMX_NATIVE_CALL n_GetGravity(AMX *amx, cell *params)
{
	return amx_ftoc(pNetGame->m_fGravity);
}

//----------------------------------------------------------------------------------

// native AllowAdminTeleport(allow)
static cell AMX_NATIVE_CALL n_AllowAdminTeleport(AMX *amx, cell *params)
{
	logprintf("AllowAdminTeleport() : function is deprecated. Please see OnPlayerClickMap()");
	return 1;
}

//----------------------------------------------------------------------------------

// native SetDeathDropAmount(amount)
static cell AMX_NATIVE_CALL n_SetDeathDropAmount(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	pNetGame->m_iDeathDropMoney = params[1];
	return 1;
}

//----------------------------------------------------------------------------------

static cell AMX_NATIVE_CALL n_CreateExplosion(AMX *amx, cell *params)
{
	// TODO: CreateExplosion
	return 0;
}

static cell AMX_NATIVE_CALL n_CreateExplosionForPlayer(AMX *amx, cell *params)
{
	// TODO: CreateExplosionForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_SetDisabledWeapons(AMX *amx, cell *params)
{
	logprintf("[warning] SetDisabledWeapons() : function is deprecated. Check weapon in OnPlayerUpdate.");
	return 1;
}

// native UsePlayerPedAnims()
static cell AMX_NATIVE_CALL n_UsePlayerPedAnims(AMX *amx, cell *params)
{
	pNetGame->m_bUseCJWalk = true;
	return 1;
}

// native DisableInteriorEnterExits()
static cell AMX_NATIVE_CALL n_DisableInteriorEnterExits(AMX *amx, cell *params)
{
	pNetGame->m_bDisableEnterExits = true;

	return 1;
}

// native SetNameTagDrawDistance(Float:distance)
static cell AMX_NATIVE_CALL n_SetNameTagDrawDistance(AMX *amx, cell *params)
{
	pNetGame->m_fNameTagDrawDistance = amx_ctof(params[1]);

	return 1;
}

// native DisableNameTagLOS()
static cell AMX_NATIVE_CALL n_DisableNameTagLOS(AMX *amx, cell *params)
{
	pNetGame->m_bNameTagLOS = false;
	return 1;
}

static cell AMX_NATIVE_CALL n_ConnectNPC(AMX *amx, cell *params)
{
	// TODO: ConnectNPC
	return 0;
}

// native IsPlayerNPC(playerid)
static cell AMX_NATIVE_CALL n_IsPlayerNPC(AMX *amx, cell *params)
{
	BOOL bResult = FALSE;

	if(pNetGame)
	{
		CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();

		if(pPlayerPool && pPlayerPool->GetSlotState((PLAYERID)params[1]))
		{
			if(pPlayerPool->IsNPC((PLAYERID)params[1]))
			{
				bResult = TRUE;
			}
		}
	}
	return bResult;
}

static cell AMX_NATIVE_CALL n_Create3DTextLabel(AMX *amx, cell *params)
{
	// TODO: Create3DTextLabel
	return 0;
}

static cell AMX_NATIVE_CALL n_Delete3DTextLabel(AMX *amx, cell *params)
{
	// TODO: Delete3DTextLabel
	return 0;
}

static cell AMX_NATIVE_CALL n_Attach3DTextLabelToPlayer(AMX *amx, cell *params)
{
	// TODO: Attach3DTextLabelToPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_Attach3DTextLabelToVehicle(AMX *amx, cell *params)
{
	// TODO: Attach3DTextLabelToVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_Update3DTextLabelText(AMX *amx, cell *params)
{
	// TODO: Update3DTextLabelText
	return 0;
}

static cell AMX_NATIVE_CALL n_CreatePlayer3DTextLabel(AMX *amx, cell *params)
{
	// TODO: CreatePlayer3DTextLabel
	return 0;
}

static cell AMX_NATIVE_CALL n_DeletePlayer3DTextLabel(AMX *amx, cell *params)
{
	// TODO: DeletePlayer3DTextLabel
	return 0;
}

static cell AMX_NATIVE_CALL n_UpdatePlayer3DTextLabelText(AMX *amx, cell *params)
{
	// TODO: UpdatePlayer3DTextLabelText
	return 0;
}

static cell AMX_NATIVE_CALL n_ShowPlayerDialog(AMX *amx, cell *params)
{
	// TODO: ShowPlayerDialog
	return 0;
}

// native EnableZoneNames(enable)
static cell AMX_NATIVE_CALL n_EnableZoneNames(AMX *amx, cell *params)
{
	logprintf("[warning] EnableZoneNames() is removed since 0.3");
	return 1;
}

static cell AMX_NATIVE_CALL n_GangZoneCreate(AMX *amx, cell *params)
{
	// TODO: GangZoneCreate
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneDestroy(AMX *amx, cell *params)
{
	// TODO: GangZoneDestroy
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneShowForPlayer(AMX *amx, cell *params)
{
	// TODO: GangZoneShowForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneShowForAll(AMX *amx, cell *params)
{
	// TODO: GangZoneShowForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneHideForPlayer(AMX *amx, cell *params)
{
	// TODO: GangZoneHideForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneHideForAll(AMX *amx, cell *params)
{
	// TODO: GangZoneHideForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneFlashForPlayer(AMX *amx, cell *params)
{
	// TODO: GangZoneFlashForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneFlashForAll(AMX *amx, cell *params)
{
	// TODO: GangZoneFlashForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneStopFlashForPlayer(AMX *amx, cell *params)
{
	// TODO: GangZoneStopFlashForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GangZoneStopFlashForAll(AMX *amx, cell *params)
{
	// TODO: GangZoneStopFlashForAll
	return 0;
}

// native IsPlayerAdmin(playerid)
static cell AMX_NATIVE_CALL n_IsPlayerAdmin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();

	if (pPlayerPool->GetSlotState((PLAYERID)params[1]))
	{
		return pPlayerPool->IsAdmin((PLAYERID)params[1]);
	}
	return 0;
}

static cell AMX_NATIVE_CALL n_Kick(AMX *amx, cell *params)
{
	// TODO: Kick
	return 0;
}

static cell AMX_NATIVE_CALL n_Ban(AMX *amx, cell *params)
{
	// TODO: Ban
	return 0;
}

static cell AMX_NATIVE_CALL n_BanEx(AMX *amx, cell *params)
{
	// TODO: BanEx
	return 0;
}

static cell AMX_NATIVE_CALL n_SendRconCommand(AMX *amx, cell *params)
{
	// TODO: SendRconCommand
	return 0;
}

static cell AMX_NATIVE_CALL n_GetServerVarAsString(AMX *amx, cell *params)
{
	// TODO: GetServerVarAsString
	return 0;
}

static cell AMX_NATIVE_CALL n_GetServerVarAsInt(AMX *amx, cell *params)
{
	// TODO: GetServerVarAsInt
	return 0;
}

static cell AMX_NATIVE_CALL n_GetServerVarAsBool(AMX *amx, cell *params)
{
	// TODO: GetServerVarAsBool
	return 0;
}

static cell AMX_NATIVE_CALL n_GetConsoleVarAsString(AMX *amx, cell *params)
{
	// TODO: GetConsoleVarAsString
	return 0;
}

static cell AMX_NATIVE_CALL n_GetConsoleVarAsInt(AMX *amx, cell *params)
{
	// TODO: GetConsoleVarAsInt
	return 0;
}

static cell AMX_NATIVE_CALL n_GetConsoleVarAsBool(AMX *amx, cell *params)
{
	// TODO: GetConsoleVarAsBool
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerNetworkStats(AMX *amx, cell *params)
{
	// TODO: GetPlayerNetworkStats
	return 0;
}

static cell AMX_NATIVE_CALL n_GetNetworkStats(AMX *amx, cell *params)
{
	// TODO: GetNetworkStats
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerVersion(AMX *amx, cell *params)
{
	// TODO: GetPlayerVersion
	return 0;
}

static cell AMX_NATIVE_CALL n_BlockIpAddress(AMX *amx, cell *params)
{
	// TODO: BlockIpAddress
	return 0;
}

static cell AMX_NATIVE_CALL n_UnBlockIpAddress(AMX *amx, cell *params)
{
	// TODO: UnBlockIpAddress
	return 0;
}

static cell AMX_NATIVE_CALL n_GetServerTickRate(AMX *amx, cell *params)
{
	// TODO: GetServerTickRate
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_GetConnectedTime(AMX *amx, cell *params)
{
	// TODO: NetStats_GetConnectedTime
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_MessagesReceived(AMX *amx, cell *params)
{
	// TODO: NetStats_MessagesReceived
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_BytesReceived(AMX *amx, cell *params)
{
	// TODO: NetStats_BytesReceived
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_MessagesSent(AMX *amx, cell *params)
{
	// TODO: NetStats_MessagesSent
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_BytesSent(AMX *amx, cell *params)
{
	// TODO: NetStats_BytesSent
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_MessagesRecvPerSecond(AMX *amx, cell *params)
{
	// TODO: NetStats_MessagesRecvPerSecond
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_PacketLossPercent(AMX *amx, cell *params)
{
	// TODO: NetStats_PacketLossPercent
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_ConnectionStatus(AMX *amx, cell *params)
{
	// TODO: NetStats_ConnectionStatus
	return 0;
}

static cell AMX_NATIVE_CALL n_NetStats_GetIpPort(AMX *amx, cell *params)
{
	// TODO: NetStats_GetIpPort
	return 0;
}

static cell AMX_NATIVE_CALL n_SetSpawnInfo(AMX *amx, cell *params)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(!pPlayerPool) return 0;

	CPlayer *pPlayer = pPlayerPool->GetAt((PLAYERID)params[1]);
	if (pPlayer)
	{
		PLAYER_SPAWN_INFO SpawnInfo;
		SpawnInfo.byteTeam = (BYTE)params[2];
		SpawnInfo.iSkin = (int)params[3];
		SpawnInfo.vecPos.X = amx_ctof(params[4]);
		SpawnInfo.vecPos.Y = amx_ctof(params[5]);
		SpawnInfo.vecPos.Z = amx_ctof(params[6]);
		SpawnInfo.fRotation = amx_ctof(params[7]);
		SpawnInfo.iSpawnWeapons[0] = (int)params[8];
		SpawnInfo.iSpawnWeaponsAmmo[0] = (int)params[9];
		SpawnInfo.iSpawnWeapons[1] = (int)params[10];
		SpawnInfo.iSpawnWeaponsAmmo[1] = (int)params[11];
		SpawnInfo.iSpawnWeapons[2] = (int)params[12];
		SpawnInfo.iSpawnWeaponsAmmo[2] = (int)params[13];

		pPlayer->SetSpawnInfo(&SpawnInfo);
		pPlayer->field_290D = TRUE;
		RakNet::BitStream bsData;
		bsData.Write((PCHAR)&SpawnInfo, sizeof(PLAYER_SPAWN_INFO));
		pNetGame->SendToPlayer(RPC_ScrSetSpawnInfo, &bsData, (PLAYERID)params[1], 2);
		return 1;
	} else {
		return 0;
	}
}

static cell AMX_NATIVE_CALL n_SpawnPlayer(AMX *amx, cell *params)
{
	// TODO: SpawnPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerTeam(AMX *amx, cell *params)
{
	// TODO: SetPlayerTeam
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerTeam(AMX *amx, cell *params)
{
	// TODO: GetPlayerTeam
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerName(AMX *amx, cell *params)
{
	// TODO: SetPlayerName
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerSkin(AMX *amx, cell *params)
{
	// TODO: SetPlayerSkin
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerSkin(AMX *amx, cell *params)
{
	// TODO: GetPlayerSkin
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerPos(AMX *amx, cell *params)
{
	// TODO: GetPlayerPos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerPos(AMX *amx, cell *params)
{
	// TODO: SetPlayerPos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerPosFindZ(AMX *amx, cell *params)
{
	// TODO: SetPlayerPosFindZ
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerLastShotVectors(AMX *amx, cell *params)
{
	// TODO: GetPlayerLastShotVectors
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerHealth(AMX *amx, cell *params)
{
	// TODO: GetPlayerHealth
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerHealth(AMX *amx, cell *params)
{
	// TODO: SetPlayerHealth
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerColor(AMX *amx, cell *params)
{
	// TODO: SetPlayerColor
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerColor(AMX *amx, cell *params)
{
	// TODO: GetPlayerColor
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerVehicleID(AMX *amx, cell *params)
{
	// TODO: GetPlayerVehicleID
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerVehicleSeat(AMX *amx, cell *params)
{
	// TODO: GetPlayerVehicleSeat
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerSurfingVehicleID(AMX *amx, cell *params)
{
	// TODO: GetPlayerSurfingVehicleID
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerSurfingObjectID(AMX *amx, cell *params)
{
	// TODO: GetPlayerSurfingObjectID
	return 0;
}

static cell AMX_NATIVE_CALL n_PutPlayerInVehicle(AMX *amx, cell *params)
{
	// TODO: PutPlayerInVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_RemovePlayerFromVehicle(AMX *amx, cell *params)
{
	// TODO: RemovePlayerFromVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerInVehicle(AMX *amx, cell *params)
{
	// TODO: IsPlayerInVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerInAnyVehicle(AMX *amx, cell *params)
{
	// TODO: IsPlayerInAnyVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerName(AMX *amx, cell *params)
{
	// TODO: GetPlayerName
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerCheckpoint(AMX *amx, cell *params)
{
	// TODO: SetPlayerCheckpoint
	return 0;
}

static cell AMX_NATIVE_CALL n_DisablePlayerCheckpoint(AMX *amx, cell *params)
{
	// TODO: DisablePlayerCheckpoint
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerInCheckpoint(AMX *amx, cell *params)
{
	// TODO: IsPlayerInCheckpoint
	return 0;
}

// native SetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size)
static cell AMX_NATIVE_CALL n_SetPlayerRaceCheckpoint(AMX *amx, cell *params)
{
	CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt((PLAYERID)params[1]);
	if (pPlayer)
	{
		pPlayer->SetRaceCheckpoint(
params[2],
params[3],
params[4],
params[5],
params[6],
params[7],
params[8],
params[9]);

		return 1;
	}
	return 0;
}

static cell AMX_NATIVE_CALL n_DisablePlayerRaceCheckpoint(AMX *amx, cell *params)
{
	// TODO: DisablePlayerRaceCheckpoint
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerInRaceCheckpoint(AMX *amx, cell *params)
{
	CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt((PLAYERID)params[1]);
	if (pPlayer)
	{
		return pPlayer->IsInRaceCheckpoint();
	}
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerInterior(AMX *amx, cell *params)
{
	// TODO: SetPlayerInterior
	return 0;
}

// native GetPlayerInterior(playerid,interiorid)
static cell AMX_NATIVE_CALL n_GetPlayerInterior(AMX *amx, cell *params)
{
	CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt((PLAYERID)params[1]);
	if (pPlayer)
	{
		return pPlayer->m_iInteriorId;
	}
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerAttachedObject(AMX *amx, cell *params)
{
	// TODO: SetPlayerAttachedObject
	return 0;
}

static cell AMX_NATIVE_CALL n_RemovePlayerAttachedObject(AMX *amx, cell *params)
{
	// TODO: RemovePlayerAttachedObject
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerAttachedObjectSlotUsed(AMX *amx, cell *params)
{
	// TODO: IsPlayerAttachedObjectSlotUsed
	return 0;
}

static cell AMX_NATIVE_CALL n_EditAttachedObject(AMX *amx, cell *params)
{
	// TODO: EditAttachedObject
	return 0;
}

static cell AMX_NATIVE_CALL n_SendClientCheck(AMX *amx, cell *params)
{
	// TODO: SendClientCheck
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerCameraLookAt(AMX *amx, cell *params)
{
	// TODO: SetPlayerCameraLookAt
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerCameraPos(AMX *amx, cell *params)
{
	// TODO: SetPlayerCameraPos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetCameraBehindPlayer(AMX *amx, cell *params)
{
	// TODO: SetCameraBehindPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraMode(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraMode
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraAspectRatio(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraAspectRatio
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraZoom(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraZoom
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraPos(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraPos
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraFrontVector(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraFrontVector
	return 0;
}

static cell AMX_NATIVE_CALL n_EnablePlayerCameraTarget(AMX *amx, cell *params)
{
	// TODO: EnablePlayerCameraTarget
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraTargetObject(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraTargetObject
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraTargetVehicle(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraTargetVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraTargetPlayer(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraTargetPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerCameraTargetActor(AMX *amx, cell *params)
{
	// TODO: GetPlayerCameraTargetActor
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachCameraToObject(AMX *amx, cell *params)
{
	// TODO: AttachCameraToObject
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachCameraToPlayerObject(AMX *amx, cell *params)
{
	// TODO: AttachCameraToPlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_InterpolateCameraPos(AMX *amx, cell *params)
{
	// TODO: InterpolateCameraPos
	return 0;
}

static cell AMX_NATIVE_CALL n_InterpolateCameraLookAt(AMX *amx, cell *params)
{
	// TODO: InterpolateCameraLookAt
	return 0;
}

static cell AMX_NATIVE_CALL n_TogglePlayerControllable(AMX *amx, cell *params)
{
	// TODO: TogglePlayerControllable
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerPlaySound(AMX *amx, cell *params)
{
	// TODO: PlayerPlaySound
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerScore(AMX *amx, cell *params)
{
	// TODO: SetPlayerScore
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerScore(AMX *amx, cell *params)
{
	// TODO: GetPlayerScore
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerFacingAngle(AMX *amx, cell *params)
{
	// TODO: SetPlayerFacingAngle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerFacingAngle(AMX *amx, cell *params)
{
	// TODO: GetPlayerFacingAngle
	return 0;
}

static cell AMX_NATIVE_CALL n_GivePlayerMoney(AMX *amx, cell *params)
{
	// TODO: GivePlayerMoney
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerMoney(AMX *amx, cell *params)
{
	// TODO: GetPlayerMoney
	return 0;
}

static cell AMX_NATIVE_CALL n_ResetPlayerMoney(AMX *amx, cell *params)
{
	// TODO: ResetPlayerMoney
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerConnected(AMX *amx, cell *params)
{
	// TODO: IsPlayerConnected
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerState(AMX *amx, cell *params)
{
	// TODO: GetPlayerState
	return 0;
}

static cell AMX_NATIVE_CALL n_ResetPlayerWeapons(AMX *amx, cell *params)
{
	// TODO: ResetPlayerWeapons
	return 0;
}

static cell AMX_NATIVE_CALL n_GivePlayerWeapon(AMX *amx, cell *params)
{
	// TODO: GivePlayerWeapon
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerArmedWeapon(AMX *amx, cell *params)
{
	// TODO: SetPlayerArmedWeapon
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerIp(AMX *amx, cell *params)
{
	// TODO: GetPlayerIp
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerPing(AMX *amx, cell *params)
{
	// TODO: GetPlayerPing
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerWeapon(AMX *amx, cell *params)
{
	// TODO: GetPlayerWeapon
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerArmour(AMX *amx, cell *params)
{
	// TODO: SetPlayerArmour
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerArmour(AMX *amx, cell *params)
{
	// TODO: GetPlayerArmour
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerMapIcon(AMX *amx, cell *params)
{
	// TODO: SetPlayerMapIcon
	return 0;
}

static cell AMX_NATIVE_CALL n_RemovePlayerMapIcon(AMX *amx, cell *params)
{
	// TODO: RemovePlayerMapIcon
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerKeys(AMX *amx, cell *params)
{
	// TODO: GetPlayerKeys
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerMarkerForPlayer(AMX *amx, cell *params)
{
	// TODO: SetPlayerMarkerForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerAmmo(AMX *amx, cell *params)
{
	// TODO: GetPlayerAmmo
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerAmmo(AMX *amx, cell *params)
{
	// TODO: SetPlayerAmmo
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerTargetPlayer(AMX *amx, cell *params)
{
	// TODO: GetPlayerTargetPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerTargetActor(AMX *amx, cell *params)
{
	// TODO: GetPlayerTargetActor
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerWeaponData(AMX *amx, cell *params)
{
	// TODO: GetPlayerWeaponData
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerWeaponState(AMX *amx, cell *params)
{
	// TODO: GetPlayerWeaponState
	return 0;
}

static cell AMX_NATIVE_CALL n_AllowPlayerTeleport(AMX *amx, cell *params)
{
	// TODO: AllowPlayerTeleport
	return 0;
}

static cell AMX_NATIVE_CALL n_ForceClassSelection(AMX *amx, cell *params)
{
	// TODO: ForceClassSelection
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerWantedLevel(AMX *amx, cell *params)
{
	// TODO: SetPlayerWantedLevel
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerWantedLevel(AMX *amx, cell *params)
{
	// TODO: GetPlayerWantedLevel
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerFightingStyle(AMX *amx, cell *params)
{
	// TODO: GetPlayerFightingStyle
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerFightingStyle(AMX *amx, cell *params)
{
	// TODO: SetPlayerFightingStyle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerVelocity(AMX *amx, cell *params)
{
	// TODO: GetPlayerVelocity
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerVelocity(AMX *amx, cell *params)
{
	// TODO: SetPlayerVelocity
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerInRangeOfPoint(AMX *amx, cell *params)
{
	// TODO: IsPlayerInRangeOfPoint
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerDistanceFromPoint(AMX *amx, cell *params)
{
	// TODO: GetPlayerDistanceFromPoint
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerStreamedIn(AMX *amx, cell *params)
{
	// TODO: IsPlayerStreamedIn
	return 0;
}

static cell AMX_NATIVE_CALL n_IsVehicleStreamedIn(AMX *amx, cell *params)
{
	// TODO: IsVehicleStreamedIn
	return 0;
}

static cell AMX_NATIVE_CALL n_IsActorStreamedIn(AMX *amx, cell *params)
{
	// TODO: IsActorStreamedIn
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerVirtualWorld(AMX *amx, cell *params)
{
	// TODO: SetPlayerVirtualWorld
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerVirtualWorld(AMX *amx, cell *params)
{
	// TODO: GetPlayerVirtualWorld
	return 0;
}

static cell AMX_NATIVE_CALL n_ShowPlayerNameTagForPlayer(AMX *amx, cell *params)
{
	// TODO: ShowPlayerNameTagForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_EnableStuntBonusForAll(AMX *amx, cell *params)
{
	// TODO: EnableStuntBonusForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_EnableStuntBonusForPlayer(AMX *amx, cell *params)
{
	// TODO: EnableStuntBonusForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_TogglePlayerSpectating(AMX *amx, cell *params)
{
	// TODO: TogglePlayerSpectating
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerDrunkLevel(AMX *amx, cell *params)
{
	// TODO: GetPlayerDrunkLevel
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerDrunkLevel(AMX *amx, cell *params)
{
	// TODO: SetPlayerDrunkLevel
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerSpectateVehicle(AMX *amx, cell *params)
{
	// TODO: PlayerSpectateVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerSpectatePlayer(AMX *amx, cell *params)
{
	// TODO: PlayerSpectatePlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_ApplyAnimation(AMX *amx, cell *params)
{
	// TODO: ApplyAnimation
	return 0;
}

static cell AMX_NATIVE_CALL n_ClearAnimations(AMX *amx, cell *params)
{
	// TODO: ClearAnimations
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerAnimationIndex(AMX *amx, cell *params)
{
	// TODO: GetPlayerAnimationIndex
	return 0;
}

static cell AMX_NATIVE_CALL n_GetAnimationName(AMX *amx, cell *params)
{
	// TODO: GetAnimationName
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerSpecialAction(AMX *amx, cell *params)
{
	// TODO: SetPlayerSpecialAction
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerSpecialAction(AMX *amx, cell *params)
{
	// TODO: GetPlayerSpecialAction
	return 0;
}

static cell AMX_NATIVE_CALL n_StartRecordingPlayerData(AMX *amx, cell *params)
{
	// TODO: StartRecordingPlayerData
	return 0;
}

static cell AMX_NATIVE_CALL n_StopRecordingPlayerData(AMX *amx, cell *params)
{
	// TODO: StopRecordingPlayerData
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayCrimeReportForPlayer(AMX *amx, cell *params)
{
	// TODO: PlayCrimeReportForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayAudioStreamForPlayer(AMX *amx, cell *params)
{
	// TODO: PlayAudioStreamForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_StopAudioStreamForPlayer(AMX *amx, cell *params)
{
	// TODO: StopAudioStreamForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_RemoveBuildingForPlayer(AMX *amx, cell *params)
{
	// TODO: RemoveBuildingForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerShopName(AMX *amx, cell *params)
{
	// TODO: SetPlayerShopName
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerSkillLevel(AMX *amx, cell *params)
{
	// TODO: SetPlayerSkillLevel
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerChatBubble(AMX *amx, cell *params)
{
	// TODO: SetPlayerChatBubble
	return 0;
}

static cell AMX_NATIVE_CALL n_SelectTextDraw(AMX *amx, cell *params)
{
	// TODO: SelectTextDraw
	return 0;
}

static cell AMX_NATIVE_CALL n_CancelSelectTextDraw(AMX *amx, cell *params)
{
	// TODO: CancelSelectTextDraw
	return 0;
}

static cell AMX_NATIVE_CALL n_DisableRemoteVehicleCollisions(AMX *amx, cell *params)
{
	// TODO: DisableRemoteVehicleCollisions
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPVarInt(AMX *amx, cell *params)
{
	// TODO: SetPVarInt
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPVarString(AMX *amx, cell *params)
{
	// TODO: SetPVarString
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPVarFloat(AMX *amx, cell *params)
{
	// TODO: SetPVarFloat
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPVarInt(AMX *amx, cell *params)
{
	// TODO: GetPVarInt
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPVarString(AMX *amx, cell *params)
{
	// TODO: GetPVarString
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPVarFloat(AMX *amx, cell *params)
{
	// TODO: GetPVarFloat
	return 0;
}

static cell AMX_NATIVE_CALL n_DeletePVar(AMX *amx, cell *params)
{
	// TODO: DeletePVar
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPVarType(AMX *amx, cell *params)
{
	// TODO: GetPVarType
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPVarNameAtIndex(AMX *amx, cell *params)
{
	// TODO: GetPVarNameAtIndex
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPVarsUpperIndex(AMX *amx, cell *params)
{
	// TODO: GetPVarsUpperIndex
	return 0;
}

static cell AMX_NATIVE_CALL n_CreatePlayerTextDraw(AMX *amx, cell *params)
{
	// TODO: CreatePlayerTextDraw
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetString(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetString
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawLetterSize(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawLetterSize
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawTextSize(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawTextSize
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawAlignment(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawAlignment
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawColor(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawColor
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawUseBox(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawUseBox
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawBoxColor(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawBoxColor
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetShadow(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetShadow
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetOutline(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetOutline
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawBackgroundColor(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawBackgroundColor
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawFont(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawFont
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetProportional(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetProportional
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetSelectable(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetSelectable
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetPreviewModel(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetPreviewModel
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetPreviewRot(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetPreviewRot
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawSetPreviewVehCol(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawSetPreviewVehCol
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawShow(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawShow
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawHide(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawHide
	return 0;
}

static cell AMX_NATIVE_CALL n_PlayerTextDrawDestroy(AMX *amx, cell *params)
{
	// TODO: PlayerTextDrawDestroy
	return 0;
}

static cell AMX_NATIVE_CALL n_IsValidVehicle(AMX *amx, cell *params)
{
	// TODO: IsValidVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_CreateVehicle(AMX *amx, cell *params)
{
	// TODO: CreateVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_DestroyVehicle(AMX *amx, cell *params)
{
	// TODO: DestroyVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehiclePos(AMX *amx, cell *params)
{
	// TODO: GetVehiclePos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehiclePos(AMX *amx, cell *params)
{
	// TODO: SetVehiclePos
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleZAngle(AMX *amx, cell *params)
{
	// TODO: GetVehicleZAngle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleRotationQuat(AMX *amx, cell *params)
{
	// TODO: GetVehicleRotationQuat
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleDistanceFromPoint(AMX *amx, cell *params)
{
	// TODO: GetVehicleDistanceFromPoint
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleZAngle(AMX *amx, cell *params)
{
	// TODO: SetVehicleZAngle
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleParamsForPlayer(AMX *amx, cell *params)
{
	// TODO: SetVehicleParamsForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_ManualVehicleEngineAndLights(AMX *amx, cell *params)
{
	// TODO: ManualVehicleEngineAndLights
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleParamsEx(AMX *amx, cell *params)
{
	// TODO: SetVehicleParamsEx
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleParamsEx(AMX *amx, cell *params)
{
	// TODO: GetVehicleParamsEx
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleParamsCarDoors(AMX *amx, cell *params)
{
	// TODO: SetVehicleParamsCarDoors
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleParamsCarDoors(AMX *amx, cell *params)
{
	// TODO: GetVehicleParamsCarDoors
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleParamsCarWindows(AMX *amx, cell *params)
{
	// TODO: SetVehicleParamsCarWindows
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleParamsCarWindows(AMX *amx, cell *params)
{
	// TODO: GetVehicleParamsCarWindows
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleToRespawn(AMX *amx, cell *params)
{
	// TODO: SetVehicleToRespawn
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleModelInfo(AMX *amx, cell *params)
{
	// TODO: GetVehicleModelInfo
	return 0;
}

static cell AMX_NATIVE_CALL n_RepairVehicle(AMX *amx, cell *params)
{
	// TODO: RepairVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_AddVehicleComponent(AMX *amx, cell *params)
{
	// TODO: AddVehicleComponent
	return 0;
}

static cell AMX_NATIVE_CALL n_RemoveVehicleComponent(AMX *amx, cell *params)
{
	// TODO: RemoveVehicleComponent
	return 0;
}

static cell AMX_NATIVE_CALL n_ChangeVehicleColor(AMX *amx, cell *params)
{
	// TODO: ChangeVehicleColor
	return 0;
}

static cell AMX_NATIVE_CALL n_ChangeVehiclePaintjob(AMX *amx, cell *params)
{
	// TODO: ChangeVehiclePaintjob
	return 0;
}

static cell AMX_NATIVE_CALL n_LinkVehicleToInterior(AMX *amx, cell *params)
{
	// TODO: LinkVehicleToInterior
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleHealth(AMX *amx, cell *params)
{
	// TODO: SetVehicleHealth
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleHealth(AMX *amx, cell *params)
{
	// TODO: GetVehicleHealth
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachTrailerToVehicle(AMX *amx, cell *params)
{
	// TODO: AttachTrailerToVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_DetachTrailerFromVehicle(AMX *amx, cell *params)
{
	// TODO: DetachTrailerFromVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_IsTrailerAttachedToVehicle(AMX *amx, cell *params)
{
	// TODO: IsTrailerAttachedToVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleTrailer(AMX *amx, cell *params)
{
	// TODO: GetVehicleTrailer
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleNumberPlate(AMX *amx, cell *params)
{
	// TODO: SetVehicleNumberPlate
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleModel(AMX *amx, cell *params)
{
	// TODO: GetVehicleModel
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleParamsSirenState(AMX *amx, cell *params)
{
	// TODO: GetVehicleParamsSirenState
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleComponentInSlot(AMX *amx, cell *params)
{
	// TODO: GetVehicleComponentInSlot
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleComponentType(AMX *amx, cell *params)
{
	// TODO: GetVehicleComponentType
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleVelocity(AMX *amx, cell *params)
{
	// TODO: GetVehicleVelocity
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleVelocity(AMX *amx, cell *params)
{
	// TODO: SetVehicleVelocity
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleAngularVelocity(AMX *amx, cell *params)
{
	// TODO: SetVehicleAngularVelocity
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleDamageStatus(AMX *amx, cell *params)
{
	// TODO: GetVehicleDamageStatus
	return 0;
}

static cell AMX_NATIVE_CALL n_UpdateVehicleDamageStatus(AMX *amx, cell *params)
{
	// TODO: UpdateVehicleDamageStatus
	return 0;
}

static cell AMX_NATIVE_CALL n_SetVehicleVirtualWorld(AMX *amx, cell *params)
{
	// TODO: SetVehicleVirtualWorld
	return 0;
}

static cell AMX_NATIVE_CALL n_GetVehicleVirtualWorld(AMX *amx, cell *params)
{
	// TODO: GetVehicleVirtualWorld
	return 0;
}

static cell AMX_NATIVE_CALL n_SendClientMessage(AMX *amx, cell *params)
{
	// TODO: SendClientMessage
	return 0;
}

static cell AMX_NATIVE_CALL n_SendClientMessageToAll(AMX *amx, cell *params)
{
	// TODO: SendClientMessageToAll
	return 0;
}

static cell AMX_NATIVE_CALL n_SendDeathMessage(AMX *amx, cell *params)
{
	// TODO: SendDeathMessage
	return 0;
}

static cell AMX_NATIVE_CALL n_SendDeathMessageToPlayer(AMX *amx, cell *params)
{
	// TODO: SendDeathMessageToPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_GameTextForAll(AMX *amx, cell *params)
{
	// TODO: GameTextForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_GameTextForPlayer(AMX *amx, cell *params)
{
	// TODO: GameTextForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_SendPlayerMessageToPlayer(AMX *amx, cell *params)
{
	// TODO: SendPlayerMessageToPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_SendPlayerMessageToAll(AMX *amx, cell *params)
{
	// TODO: SendPlayerMessageToAll
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawCreate(AMX *amx, cell *params)
{
	// TODO: TextDrawCreate
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetString(AMX *amx, cell *params)
{
	// TODO: TextDrawSetString
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawLetterSize(AMX *amx, cell *params)
{
	// TODO: TextDrawLetterSize
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawTextSize(AMX *amx, cell *params)
{
	// TODO: TextDrawTextSize
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawAlignment(AMX *amx, cell *params)
{
	// TODO: TextDrawAlignment
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawColor(AMX *amx, cell *params)
{
	// TODO: TextDrawColor
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawUseBox(AMX *amx, cell *params)
{
	// TODO: TextDrawUseBox
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawBoxColor(AMX *amx, cell *params)
{
	// TODO: TextDrawBoxColor
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetShadow(AMX *amx, cell *params)
{
	// TODO: TextDrawSetShadow
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetOutline(AMX *amx, cell *params)
{
	// TODO: TextDrawSetOutline
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawBackgroundColor(AMX *amx, cell *params)
{
	// TODO: TextDrawBackgroundColor
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawFont(AMX *amx, cell *params)
{
	// TODO: TextDrawFont
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetProportional(AMX *amx, cell *params)
{
	// TODO: TextDrawSetProportional
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetSelectable(AMX *amx, cell *params)
{
	// TODO: TextDrawSetSelectable
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetPreviewModel(AMX *amx, cell *params)
{
	// TODO: TextDrawSetPreviewModel
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetPreviewRot(AMX *amx, cell *params)
{
	// TODO: TextDrawSetPreviewRot
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawSetPreviewVehCol(AMX *amx, cell *params)
{
	// TODO: TextDrawSetPreviewVehCol
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawShowForPlayer(AMX *amx, cell *params)
{
	// TODO: TextDrawShowForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawShowForAll(AMX *amx, cell *params)
{
	// TODO: TextDrawShowForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawHideForPlayer(AMX *amx, cell *params)
{
	// TODO: TextDrawHideForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawHideForAll(AMX *amx, cell *params)
{
	// TODO: TextDrawHideForAll
	return 0;
}

static cell AMX_NATIVE_CALL n_TextDrawDestroy(AMX *amx, cell *params)
{
	// TODO: TextDrawDestroy
	return 0;
}

static cell AMX_NATIVE_CALL n_CreateObject(AMX *amx, cell *params)
{
	// TODO: CreateObject
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachObjectToVehicle(AMX *amx, cell *params)
{
	// TODO: AttachObjectToVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachPlayerObjectToVehicle(AMX *amx, cell *params)
{
	// TODO: AttachPlayerObjectToVehicle
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachObjectToObject(AMX *amx, cell *params)
{
	// TODO: AttachObjectToObject
	return 0;
}

static cell AMX_NATIVE_CALL n_SetObjectPos(AMX *amx, cell *params)
{
	// TODO: SetObjectPos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetObjectRot(AMX *amx, cell *params)
{
	// TODO: SetObjectRot
	return 0;
}

static cell AMX_NATIVE_CALL n_GetObjectPos(AMX *amx, cell *params)
{
	// TODO: GetObjectPos
	return 0;
}

static cell AMX_NATIVE_CALL n_GetObjectRot(AMX *amx, cell *params)
{
	// TODO: GetObjectRot
	return 0;
}

static cell AMX_NATIVE_CALL n_GetObjectModel(AMX *amx, cell *params)
{
	// TODO: GetObjectModel
	return 0;
}

static cell AMX_NATIVE_CALL n_SetObjectNoCameraCol(AMX *amx, cell *params)
{
	// TODO: SetObjectNoCameraCol
	return 0;
}

static cell AMX_NATIVE_CALL n_IsValidObject(AMX *amx, cell *params)
{
	// TODO: IsValidObject
	return 0;
}

static cell AMX_NATIVE_CALL n_DestroyObject(AMX *amx, cell *params)
{
	// TODO: DestroyObject
	return 0;
}

static cell AMX_NATIVE_CALL n_IsObjectMoving(AMX *amx, cell *params)
{
	// TODO: IsObjectMoving
	return 0;
}

static cell AMX_NATIVE_CALL n_MoveObject(AMX *amx, cell *params)
{
	// TODO: MoveObject
	return 0;
}

static cell AMX_NATIVE_CALL n_StopObject(AMX *amx, cell *params)
{
	// TODO: StopObject
	return 0;
}

static cell AMX_NATIVE_CALL n_EditObject(AMX *amx, cell *params)
{
	// TODO: EditObject
	return 0;
}

static cell AMX_NATIVE_CALL n_EditPlayerObject(AMX *amx, cell *params)
{
	// TODO: EditPlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_SelectObject(AMX *amx, cell *params)
{
	// TODO: SelectObject
	return 0;
}

static cell AMX_NATIVE_CALL n_CancelEdit(AMX *amx, cell *params)
{
	// TODO: CancelEdit
	return 0;
}

static cell AMX_NATIVE_CALL n_SetObjectMaterial(AMX *amx, cell *params)
{
	// TODO: SetObjectMaterial
	return 0;
}

static cell AMX_NATIVE_CALL n_SetObjectMaterialText(AMX *amx, cell *params)
{
	// TODO: SetObjectMaterialText
	return 0;
}

static cell AMX_NATIVE_CALL n_CreatePlayerObject(AMX *amx, cell *params)
{
	// TODO: CreatePlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerObjectPos(AMX *amx, cell *params)
{
	// TODO: SetPlayerObjectPos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerObjectRot(AMX *amx, cell *params)
{
	// TODO: SetPlayerObjectRot
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerObjectPos(AMX *amx, cell *params)
{
	// TODO: GetPlayerObjectPos
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerObjectRot(AMX *amx, cell *params)
{
	// TODO: GetPlayerObjectRot
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerObjectModel(AMX *amx, cell *params)
{
	// TODO: GetPlayerObjectModel
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerObjectNoCameraCol(AMX *amx, cell *params)
{
	// TODO: SetPlayerObjectNoCameraCol
	return 0;
}

static cell AMX_NATIVE_CALL n_IsValidPlayerObject(AMX *amx, cell *params)
{
	// TODO: IsValidPlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_DestroyPlayerObject(AMX *amx, cell *params)
{
	// TODO: DestroyPlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_IsPlayerObjectMoving(AMX *amx, cell *params)
{
	// TODO: IsPlayerObjectMoving
	return 0;
}

static cell AMX_NATIVE_CALL n_MovePlayerObject(AMX *amx, cell *params)
{
	// TODO: MovePlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_StopPlayerObject(AMX *amx, cell *params)
{
	// TODO: StopPlayerObject
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerObjectMaterial(AMX *amx, cell *params)
{
	// TODO: SetPlayerObjectMaterial
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerObjectMaterialText(AMX *amx, cell *params)
{
	// TODO: SetPlayerObjectMaterialText
	return 0;
}

static cell AMX_NATIVE_CALL n_AttachObjectToPlayer(AMX *amx, cell *params)
{
	// TODO: AttachObjectToPlayer
	return 0;
}

// native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ)
static cell AMX_NATIVE_CALL n_AttachPlayerObjectToPlayer(AMX *amx, cell *params)
{
	logprintf("AttachPlayerObjectToPlayer : removed in 0.3. I can only attach global objects.");
	return 0;
}

static cell AMX_NATIVE_CALL n_SetObjectsDefaultCameraCol(AMX *amx, cell *params)
{
	// TODO: SetObjectsDefaultCameraCol
	return 0;
}

static cell AMX_NATIVE_CALL n_CreateActor(AMX *amx, cell *params)
{
	// TODO: CreateActor
	return 0;
}

static cell AMX_NATIVE_CALL n_DestroyActor(AMX *amx, cell *params)
{
	// TODO: DestroyActor
	return 0;
}

static cell AMX_NATIVE_CALL n_SetActorPos(AMX *amx, cell *params)
{
	// TODO: SetActorPos
	return 0;
}

static cell AMX_NATIVE_CALL n_GetActorPos(AMX *amx, cell *params)
{
	// TODO: GetActorPos
	return 0;
}

static cell AMX_NATIVE_CALL n_SetActorVirtualWorld(AMX *amx, cell *params)
{
	// TODO: SetActorVirtualWorld
	return 0;
}

static cell AMX_NATIVE_CALL n_GetActorVirtualWorld(AMX *amx, cell *params)
{
	// TODO: GetActorVirtualWorld
	return 0;
}

static cell AMX_NATIVE_CALL n_ApplyActorAnimation(AMX *amx, cell *params)
{
	// TODO: ApplyActorAnimation
	return 0;
}

static cell AMX_NATIVE_CALL n_ClearActorAnimations(AMX *amx, cell *params)
{
	// TODO: ClearActorAnimations
	return 0;
}

static cell AMX_NATIVE_CALL n_SetActorFacingAngle(AMX *amx, cell *params)
{
	// TODO: SetActorFacingAngle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetActorFacingAngle(AMX *amx, cell *params)
{
	// TODO: GetActorFacingAngle
	return 0;
}

static cell AMX_NATIVE_CALL n_GetActorHealth(AMX *amx, cell *params)
{
	// TODO: GetActorHealth
	return 0;
}

static cell AMX_NATIVE_CALL n_SetActorHealth(AMX *amx, cell *params)
{
	// TODO: SetActorHealth
	return 0;
}

static cell AMX_NATIVE_CALL n_IsValidActor(AMX *amx, cell *params)
{
	// TODO: IsValidActor
	return 0;
}

static cell AMX_NATIVE_CALL n_SetActorInvulnerable(AMX *amx, cell *params)
{
	// TODO: SetActorInvulnerable
	return 0;
}

static cell AMX_NATIVE_CALL n_IsActorInvulnerable(AMX *amx, cell *params)
{
	// TODO: IsActorInvulnerable
	return 0;
}

static cell AMX_NATIVE_CALL n_CreateMenu(AMX *amx, cell *params)
{
	// TODO: CreateMenu
	return 0;
}

static cell AMX_NATIVE_CALL n_DestroyMenu(AMX *amx, cell *params)
{
	// TODO: DestroyMenu
	return 0;
}

static cell AMX_NATIVE_CALL n_AddMenuItem(AMX *amx, cell *params)
{
	// TODO: AddMenuItem
	return 0;
}

static cell AMX_NATIVE_CALL n_SetMenuColumnHeader(AMX *amx, cell *params)
{
	// TODO: SetMenuColumnHeader
	return 0;
}

static cell AMX_NATIVE_CALL n_ShowMenuForPlayer(AMX *amx, cell *params)
{
	// TODO: ShowMenuForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_HideMenuForPlayer(AMX *amx, cell *params)
{
	// TODO: HideMenuForPlayer
	return 0;
}

static cell AMX_NATIVE_CALL n_IsValidMenu(AMX *amx, cell *params)
{
	// TODO: IsValidMenu
	return 0;
}

static cell AMX_NATIVE_CALL n_DisableMenu(AMX *amx, cell *params)
{
	// TODO: DisableMenu
	return 0;
}

static cell AMX_NATIVE_CALL n_DisableMenuRow(AMX *amx, cell *params)
{
	// TODO: DisableMenuRow
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerMenu(AMX *amx, cell *params)
{
	// TODO: GetPlayerMenu
	return 0;
}

static cell AMX_NATIVE_CALL n_SetPlayerTime(AMX *amx, cell *params)
{
	// TODO: SetPlayerTime
	return 0;
}

static cell AMX_NATIVE_CALL n_TogglePlayerClock(AMX *amx, cell *params)
{
	// TODO: TogglePlayerClock
	return 0;
}

static cell AMX_NATIVE_CALL n_GetPlayerTime(AMX *amx, cell *params)
{
	// TODO: GetPlayerTime
	return 0;
}

static cell AMX_NATIVE_CALL n_HTTP(AMX *amx, cell *params)
{
	// TODO: HTTP
	return 0;
}

//----------------------------------------------------------------------------------

AMX_NATIVE_INFO custom_Natives[] =
{
	{ "gpci", n_gpci },
	{ "GetPlayerPoolSize", n_GetPlayerPoolSize },
	{ "GetVehiclePoolSize", n_GetVehiclePoolSize },
	{ "GetActorPoolSize", n_GetActorPoolSize },
	{ "print", n_print },
	{ "printf", n_printf },
	{ "format", n_format },
	{ "SetTimer", n_SetTimer },
	{ "KillTimer", n_KillTimer },
	{ "GetTickCount", n_GetTickCount },
	{ "GetMaxPlayers", n_GetMaxPlayers },
	{ "SetTimerEx", n_SetTimerEx },
	{ "LimitGlobalChatRadius", n_LimitGlobalChatRadius },
	{ "LimitPlayerMarkerRadius", n_LimitPlayerMarkerRadius },
	{ "SetWeather", n_SetWeather },
	{ "SetPlayerWeather", n_SetPlayerWeather },
	{ "CallRemoteFunction", n_CallRemoteFunction },
	{ "CallLocalFunction", n_CallLocalFunction },
	{ "VectorSize", n_VectorSize },
	{ "asin", n_asin },
	{ "acos", n_acos },
	{ "atan2", n_atan2 },
	{ "atan", n_atan },
	{ "SHA256_PassHash", n_SHA256_PassHash },
	{ "SetSVarInt", n_SetSVarInt },
	{ "SetSVarString", n_SetSVarString },
	{ "SetSVarFloat", n_SetSVarFloat },
	{ "GetSVarInt", n_GetSVarInt },
	{ "GetSVarString", n_GetSVarString },
	{ "GetSVarFloat", n_GetSVarFloat },
	{ "DeleteSVar", n_DeleteSVar },
	{ "GetSVarType", n_GetSVarType },
	{ "GetSVarNameAtIndex", n_GetSVarNameAtIndex },
	{ "GetSVarsUpperIndex", n_GetSVarsUpperIndex },
	{ "GameModeExit", n_GameModeExit },
	{ "SetGameModeText", n_SetGameModeText },
	{ "SetTeamCount", n_SetTeamCount },
	{ "AddPlayerClass", n_AddPlayerClass },
	{ "AddPlayerClassEx", n_AddPlayerClassEx },
	{ "AddStaticVehicle", n_AddStaticVehicle },
	{ "AddStaticVehicleEx", n_AddStaticVehicleEx },
	{ "AddStaticPickup", n_AddStaticPickup },
	{ "CreatePickup", n_CreatePickup },
	{ "DestroyPickup", n_DestroyPickup },
	{ "SetPlayerWorldBounds", n_SetPlayerWorldBounds },
	{ "ShowNameTags", n_ShowNameTags },
	{ "ShowPlayerMarkers", n_ShowPlayerMarkers },
	{ "SetWorldTime", n_SetWorldTime },
	{ "GetWeaponName", n_GetWeaponName },
	{ "EnableTirePopping", n_EnableTirePopping },
	{ "EnableVehicleFriendlyFire", n_EnableVehicleFriendlyFire },
	{ "AllowInteriorWeapons", n_AllowInteriorWeapons },
	{ "SetGravity", n_SetGravity },
	{ "GetGravity", n_GetGravity },
	{ "AllowAdminTeleport", n_AllowAdminTeleport },
	{ "SetDeathDropAmount", n_SetDeathDropAmount },
	{ "CreateExplosion", n_CreateExplosion },
	{ "CreateExplosionForPlayer", n_CreateExplosionForPlayer },
	{ "SetDisabledWeapons", n_SetDisabledWeapons },
	{ "UsePlayerPedAnims", n_UsePlayerPedAnims },
	{ "DisableInteriorEnterExits", n_DisableInteriorEnterExits },
	{ "SetNameTagDrawDistance", n_SetNameTagDrawDistance },
	{ "DisableNameTagLOS", n_DisableNameTagLOS },
	{ "ConnectNPC", n_ConnectNPC },
	{ "IsPlayerNPC", n_IsPlayerNPC },
	{ "Create3DTextLabel", n_Create3DTextLabel },
	{ "Delete3DTextLabel", n_Delete3DTextLabel },
	{ "Attach3DTextLabelToPlayer", n_Attach3DTextLabelToPlayer },
	{ "Attach3DTextLabelToVehicle", n_Attach3DTextLabelToVehicle },
	{ "Update3DTextLabelText", n_Update3DTextLabelText },
	{ "CreatePlayer3DTextLabel", n_CreatePlayer3DTextLabel },
	{ "DeletePlayer3DTextLabel", n_DeletePlayer3DTextLabel },
	{ "UpdatePlayer3DTextLabelText", n_UpdatePlayer3DTextLabelText },
	{ "ShowPlayerDialog", n_ShowPlayerDialog },
	{ "EnableZoneNames", n_EnableZoneNames },
	{ "GangZoneCreate", n_GangZoneCreate },
	{ "GangZoneDestroy", n_GangZoneDestroy },
	{ "GangZoneShowForPlayer", n_GangZoneShowForPlayer },
	{ "GangZoneShowForAll", n_GangZoneShowForAll },
	{ "GangZoneHideForPlayer", n_GangZoneHideForPlayer },
	{ "GangZoneHideForAll", n_GangZoneHideForAll },
	{ "GangZoneFlashForPlayer", n_GangZoneFlashForPlayer },
	{ "GangZoneFlashForAll", n_GangZoneFlashForAll },
	{ "GangZoneStopFlashForPlayer", n_GangZoneStopFlashForPlayer },
	{ "GangZoneStopFlashForAll", n_GangZoneStopFlashForAll },
	{ "IsPlayerAdmin", n_IsPlayerAdmin },
	{ "Kick", n_Kick },
	{ "Ban", n_Ban },
	{ "BanEx", n_BanEx },
	{ "SendRconCommand", n_SendRconCommand },
	{ "GetServerVarAsString", n_GetServerVarAsString },
	{ "GetServerVarAsInt", n_GetServerVarAsInt },
	{ "GetServerVarAsBool", n_GetServerVarAsBool },
	{ "GetConsoleVarAsString", n_GetConsoleVarAsString },
	{ "GetConsoleVarAsInt", n_GetConsoleVarAsInt },
	{ "GetConsoleVarAsBool", n_GetConsoleVarAsBool },
	{ "GetPlayerNetworkStats", n_GetPlayerNetworkStats },
	{ "GetNetworkStats", n_GetNetworkStats },
	{ "GetPlayerVersion", n_GetPlayerVersion },
	{ "BlockIpAddress", n_BlockIpAddress },
	{ "UnBlockIpAddress", n_UnBlockIpAddress },
	{ "GetServerTickRate", n_GetServerTickRate },
	{ "NetStats_GetConnectedTime", n_NetStats_GetConnectedTime },
	{ "NetStats_MessagesReceived", n_NetStats_MessagesReceived },
	{ "NetStats_BytesReceived", n_NetStats_BytesReceived },
	{ "NetStats_MessagesSent", n_NetStats_MessagesSent },
	{ "NetStats_BytesSent", n_NetStats_BytesSent },
	{ "NetStats_MessagesRecvPerSecond", n_NetStats_MessagesRecvPerSecond },
	{ "NetStats_PacketLossPercent", n_NetStats_PacketLossPercent },
	{ "NetStats_ConnectionStatus", n_NetStats_ConnectionStatus },
	{ "NetStats_GetIpPort", n_NetStats_GetIpPort },
	{ "SetSpawnInfo", n_SetSpawnInfo },
	{ "SpawnPlayer", n_SpawnPlayer },
	{ "SetPlayerTeam", n_SetPlayerTeam },
	{ "GetPlayerTeam", n_GetPlayerTeam },
	{ "SetPlayerName", n_SetPlayerName },
	{ "SetPlayerSkin", n_SetPlayerSkin },
	{ "GetPlayerSkin", n_GetPlayerSkin },
	{ "GetPlayerPos", n_GetPlayerPos },
	{ "SetPlayerPos", n_SetPlayerPos },
	{ "SetPlayerPosFindZ", n_SetPlayerPosFindZ },
	{ "GetPlayerLastShotVectors", n_GetPlayerLastShotVectors },
	{ "GetPlayerHealth", n_GetPlayerHealth },
	{ "SetPlayerHealth", n_SetPlayerHealth },
	{ "SetPlayerColor", n_SetPlayerColor },
	{ "GetPlayerColor", n_GetPlayerColor },
	{ "GetPlayerVehicleID", n_GetPlayerVehicleID },
	{ "GetPlayerVehicleSeat", n_GetPlayerVehicleSeat },
	{ "GetPlayerSurfingVehicleID", n_GetPlayerSurfingVehicleID },
	{ "GetPlayerSurfingObjectID", n_GetPlayerSurfingObjectID },
	{ "PutPlayerInVehicle", n_PutPlayerInVehicle },
	{ "RemovePlayerFromVehicle", n_RemovePlayerFromVehicle },
	{ "IsPlayerInVehicle", n_IsPlayerInVehicle },
	{ "IsPlayerInAnyVehicle", n_IsPlayerInAnyVehicle },
	{ "GetPlayerName", n_GetPlayerName },
	{ "SetPlayerCheckpoint", n_SetPlayerCheckpoint },
	{ "DisablePlayerCheckpoint", n_DisablePlayerCheckpoint },
	{ "IsPlayerInCheckpoint", n_IsPlayerInCheckpoint },
	{ "SetPlayerRaceCheckpoint", n_SetPlayerRaceCheckpoint },
	{ "DisablePlayerRaceCheckpoint", n_DisablePlayerRaceCheckpoint },
	{ "IsPlayerInRaceCheckpoint", n_IsPlayerInRaceCheckpoint },
	{ "SetPlayerInterior", n_SetPlayerInterior },
	{ "GetPlayerInterior", n_GetPlayerInterior },
	{ "SetPlayerAttachedObject", n_SetPlayerAttachedObject },
	{ "RemovePlayerAttachedObject", n_RemovePlayerAttachedObject },
	{ "SetPlayerAttachedObject", n_SetPlayerAttachedObject },
	{ "IsPlayerAttachedObjectSlotUsed", n_IsPlayerAttachedObjectSlotUsed },
	{ "EditAttachedObject", n_EditAttachedObject },
	{ "SendClientCheck", n_SendClientCheck },
	{ "SetPlayerCameraLookAt", n_SetPlayerCameraLookAt },
	{ "SetPlayerCameraPos", n_SetPlayerCameraPos },
	{ "SetCameraBehindPlayer", n_SetCameraBehindPlayer },
	{ "GetPlayerCameraMode", n_GetPlayerCameraMode },
	{ "GetPlayerCameraAspectRatio", n_GetPlayerCameraAspectRatio },
	{ "GetPlayerCameraZoom", n_GetPlayerCameraZoom },
	{ "GetPlayerCameraPos", n_GetPlayerCameraPos },
	{ "GetPlayerCameraFrontVector", n_GetPlayerCameraFrontVector },
	{ "EnablePlayerCameraTarget", n_EnablePlayerCameraTarget },
	{ "GetPlayerCameraTargetObject", n_GetPlayerCameraTargetObject },
	{ "GetPlayerCameraTargetVehicle", n_GetPlayerCameraTargetVehicle },
	{ "GetPlayerCameraTargetPlayer", n_GetPlayerCameraTargetPlayer },
	{ "GetPlayerCameraTargetActor", n_GetPlayerCameraTargetActor },
	{ "AttachCameraToObject", n_AttachCameraToObject },
	{ "AttachCameraToPlayerObject", n_AttachCameraToPlayerObject },
	{ "InterpolateCameraPos", n_InterpolateCameraPos },
	{ "InterpolateCameraLookAt", n_InterpolateCameraLookAt },
	{ "TogglePlayerControllable", n_TogglePlayerControllable },
	{ "PlayerPlaySound", n_PlayerPlaySound },
	{ "SetPlayerScore", n_SetPlayerScore },
	{ "GetPlayerScore", n_GetPlayerScore },
	{ "SetPlayerFacingAngle", n_SetPlayerFacingAngle },
	{ "GetPlayerFacingAngle", n_GetPlayerFacingAngle },
	{ "GivePlayerMoney", n_GivePlayerMoney },
	{ "GetPlayerMoney", n_GetPlayerMoney },
	{ "ResetPlayerMoney", n_ResetPlayerMoney },
	{ "IsPlayerConnected", n_IsPlayerConnected },
	{ "GetPlayerState", n_GetPlayerState },
	{ "ResetPlayerWeapons", n_ResetPlayerWeapons },
	{ "GivePlayerWeapon", n_GivePlayerWeapon },
	{ "SetPlayerArmedWeapon", n_SetPlayerArmedWeapon },
	{ "GetPlayerIp", n_GetPlayerIp },
	{ "GetPlayerPing", n_GetPlayerPing },
	{ "GetPlayerWeapon", n_GetPlayerWeapon },
	{ "SetPlayerArmour", n_SetPlayerArmour },
	{ "GetPlayerArmour", n_GetPlayerArmour },
	{ "SetPlayerMapIcon", n_SetPlayerMapIcon },
	{ "RemovePlayerMapIcon", n_RemovePlayerMapIcon },
	{ "GetPlayerKeys", n_GetPlayerKeys },
	{ "SetPlayerMarkerForPlayer", n_SetPlayerMarkerForPlayer },
	{ "GetPlayerAmmo", n_GetPlayerAmmo },
	{ "SetPlayerAmmo", n_SetPlayerAmmo },
	{ "GetPlayerTargetPlayer", n_GetPlayerTargetPlayer },
	{ "GetPlayerTargetActor", n_GetPlayerTargetActor },
	{ "GetPlayerWeaponData", n_GetPlayerWeaponData },
	{ "GetPlayerWeaponState", n_GetPlayerWeaponState },
	{ "AllowPlayerTeleport", n_AllowPlayerTeleport },
	{ "ForceClassSelection", n_ForceClassSelection },
	{ "SetPlayerWantedLevel", n_SetPlayerWantedLevel },
	{ "GetPlayerWantedLevel", n_GetPlayerWantedLevel },
	{ "GetPlayerFightingStyle", n_GetPlayerFightingStyle },
	{ "SetPlayerFightingStyle", n_SetPlayerFightingStyle },
	{ "GetPlayerVelocity", n_GetPlayerVelocity },
	{ "SetPlayerVelocity", n_SetPlayerVelocity },
	{ "IsPlayerInRangeOfPoint", n_IsPlayerInRangeOfPoint },
	{ "GetPlayerDistanceFromPoint", n_GetPlayerDistanceFromPoint },
	{ "IsPlayerStreamedIn", n_IsPlayerStreamedIn },
	{ "IsVehicleStreamedIn", n_IsVehicleStreamedIn },
	{ "IsActorStreamedIn", n_IsActorStreamedIn },
	{ "SetPlayerVirtualWorld", n_SetPlayerVirtualWorld },
	{ "GetPlayerVirtualWorld", n_GetPlayerVirtualWorld },
	{ "ShowPlayerNameTagForPlayer", n_ShowPlayerNameTagForPlayer },
	{ "EnableStuntBonusForAll", n_EnableStuntBonusForAll },
	{ "EnableStuntBonusForPlayer", n_EnableStuntBonusForPlayer },
	{ "TogglePlayerSpectating", n_TogglePlayerSpectating },
	{ "GetPlayerDrunkLevel", n_GetPlayerDrunkLevel },
	{ "SetPlayerDrunkLevel", n_SetPlayerDrunkLevel },
	{ "PlayerSpectateVehicle", n_PlayerSpectateVehicle },
	{ "PlayerSpectatePlayer", n_PlayerSpectatePlayer },
	{ "ApplyAnimation", n_ApplyAnimation },
	{ "ClearAnimations", n_ClearAnimations },
	{ "GetPlayerAnimationIndex", n_GetPlayerAnimationIndex },
	{ "GetAnimationName", n_GetAnimationName },
	{ "SetPlayerSpecialAction", n_SetPlayerSpecialAction },
	{ "GetPlayerSpecialAction", n_GetPlayerSpecialAction },
	{ "StartRecordingPlayerData", n_StartRecordingPlayerData },
	{ "StopRecordingPlayerData", n_StopRecordingPlayerData },
	{ "PlayCrimeReportForPlayer", n_PlayCrimeReportForPlayer },
	{ "PlayAudioStreamForPlayer", n_PlayAudioStreamForPlayer },
	{ "StopAudioStreamForPlayer", n_StopAudioStreamForPlayer },
	{ "RemoveBuildingForPlayer", n_RemoveBuildingForPlayer },
	{ "SetPlayerShopName", n_SetPlayerShopName },
	{ "SetPlayerSkillLevel", n_SetPlayerSkillLevel },
	{ "SetPlayerChatBubble", n_SetPlayerChatBubble },
	{ "SelectTextDraw", n_SelectTextDraw },
	{ "CancelSelectTextDraw", n_CancelSelectTextDraw },
	{ "DisableRemoteVehicleCollisions", n_DisableRemoteVehicleCollisions },
	{ "SetPVarInt", n_SetPVarInt },
	{ "SetPVarString", n_SetPVarString },
	{ "SetPVarFloat", n_SetPVarFloat },
	{ "GetPVarInt", n_GetPVarInt },
	{ "GetPVarString", n_GetPVarString },
	{ "GetPVarFloat", n_GetPVarFloat },
	{ "DeletePVar", n_DeletePVar },
	{ "GetPVarType", n_GetPVarType },
	{ "GetPVarNameAtIndex", n_GetPVarNameAtIndex },
	{ "GetPVarsUpperIndex", n_GetPVarsUpperIndex },
	{ "CreatePlayerTextDraw", n_CreatePlayerTextDraw },
	{ "PlayerTextDrawSetString", n_PlayerTextDrawSetString },
	{ "PlayerTextDrawLetterSize", n_PlayerTextDrawLetterSize },
	{ "PlayerTextDrawTextSize", n_PlayerTextDrawTextSize },
	{ "PlayerTextDrawAlignment", n_PlayerTextDrawAlignment },
	{ "PlayerTextDrawColor", n_PlayerTextDrawColor },
	{ "PlayerTextDrawUseBox", n_PlayerTextDrawUseBox },
	{ "PlayerTextDrawBoxColor", n_PlayerTextDrawBoxColor },
	{ "PlayerTextDrawSetShadow", n_PlayerTextDrawSetShadow },
	{ "PlayerTextDrawSetOutline", n_PlayerTextDrawSetOutline },
	{ "PlayerTextDrawBackgroundColor", n_PlayerTextDrawBackgroundColor },
	{ "PlayerTextDrawFont", n_PlayerTextDrawFont },
	{ "PlayerTextDrawSetProportional", n_PlayerTextDrawSetProportional },
	{ "PlayerTextDrawSetSelectable", n_PlayerTextDrawSetSelectable },
	{ "PlayerTextDrawSetPreviewModel", n_PlayerTextDrawSetPreviewModel },
	{ "PlayerTextDrawSetPreviewRot", n_PlayerTextDrawSetPreviewRot },
	{ "PlayerTextDrawSetPreviewVehCol", n_PlayerTextDrawSetPreviewVehCol },
	{ "PlayerTextDrawShow", n_PlayerTextDrawShow },
	{ "PlayerTextDrawHide", n_PlayerTextDrawHide },
	{ "PlayerTextDrawDestroy", n_PlayerTextDrawDestroy },
	{ "IsValidVehicle", n_IsValidVehicle },
	{ "CreateVehicle", n_CreateVehicle },
	{ "DestroyVehicle", n_DestroyVehicle },
	{ "GetVehiclePos", n_GetVehiclePos },
	{ "SetVehiclePos", n_SetVehiclePos },
	{ "GetVehicleZAngle", n_GetVehicleZAngle },
	{ "GetVehicleRotationQuat", n_GetVehicleRotationQuat },
	{ "GetVehicleDistanceFromPoint", n_GetVehicleDistanceFromPoint },
	{ "SetVehicleZAngle", n_SetVehicleZAngle },
	{ "SetVehicleParamsForPlayer", n_SetVehicleParamsForPlayer },
	{ "ManualVehicleEngineAndLights", n_ManualVehicleEngineAndLights },
	{ "SetVehicleParamsEx", n_SetVehicleParamsEx },
	{ "GetVehicleParamsEx", n_GetVehicleParamsEx },
	{ "SetVehicleParamsCarDoors", n_SetVehicleParamsCarDoors },
	{ "GetVehicleParamsCarDoors", n_GetVehicleParamsCarDoors },
	{ "SetVehicleParamsCarWindows", n_SetVehicleParamsCarWindows },
	{ "GetVehicleParamsCarWindows", n_GetVehicleParamsCarWindows },
	{ "SetVehicleToRespawn", n_SetVehicleToRespawn },
	{ "GetVehicleModelInfo", n_GetVehicleModelInfo },
	{ "RepairVehicle", n_RepairVehicle },
	{ "AddVehicleComponent", n_AddVehicleComponent },
	{ "RemoveVehicleComponent", n_RemoveVehicleComponent },
	{ "ChangeVehicleColor", n_ChangeVehicleColor },
	{ "ChangeVehiclePaintjob", n_ChangeVehiclePaintjob },
	{ "LinkVehicleToInterior", n_LinkVehicleToInterior },
	{ "SetVehicleHealth", n_SetVehicleHealth },
	{ "GetVehicleHealth", n_GetVehicleHealth },
	{ "AttachTrailerToVehicle", n_AttachTrailerToVehicle },
	{ "DetachTrailerFromVehicle", n_DetachTrailerFromVehicle },
	{ "IsTrailerAttachedToVehicle", n_IsTrailerAttachedToVehicle },
	{ "GetVehicleTrailer", n_GetVehicleTrailer },
	{ "SetVehicleNumberPlate", n_SetVehicleNumberPlate },
	{ "GetVehicleModel", n_GetVehicleModel },
	{ "GetVehicleParamsSirenState", n_GetVehicleParamsSirenState },
	{ "GetVehicleComponentInSlot", n_GetVehicleComponentInSlot },
	{ "GetVehicleComponentType", n_GetVehicleComponentType },
	{ "GetVehicleVelocity", n_GetVehicleVelocity },
	{ "SetVehicleVelocity", n_SetVehicleVelocity },
	{ "SetVehicleAngularVelocity", n_SetVehicleAngularVelocity },
	{ "GetVehicleDamageStatus", n_GetVehicleDamageStatus },
	{ "UpdateVehicleDamageStatus", n_UpdateVehicleDamageStatus },
	{ "SetVehicleVirtualWorld", n_SetVehicleVirtualWorld },
	{ "GetVehicleVirtualWorld", n_GetVehicleVirtualWorld },
	{ "SendClientMessage", n_SendClientMessage },
	{ "SendClientMessageToAll", n_SendClientMessageToAll },
	{ "SendDeathMessage", n_SendDeathMessage },
	{ "SendDeathMessageToPlayer", n_SendDeathMessageToPlayer },
	{ "GameTextForAll", n_GameTextForAll },
	{ "GameTextForPlayer", n_GameTextForPlayer },
	{ "SendPlayerMessageToPlayer", n_SendPlayerMessageToPlayer },
	{ "SendPlayerMessageToAll", n_SendPlayerMessageToAll },
	{ "TextDrawCreate", n_TextDrawCreate },
	{ "TextDrawSetString", n_TextDrawSetString },
	{ "TextDrawLetterSize", n_TextDrawLetterSize },
	{ "TextDrawTextSize", n_TextDrawTextSize },
	{ "TextDrawAlignment", n_TextDrawAlignment },
	{ "TextDrawColor", n_TextDrawColor },
	{ "TextDrawUseBox", n_TextDrawUseBox },
	{ "TextDrawBoxColor", n_TextDrawBoxColor },
	{ "TextDrawSetShadow", n_TextDrawSetShadow },
	{ "TextDrawSetOutline", n_TextDrawSetOutline },
	{ "TextDrawBackgroundColor", n_TextDrawBackgroundColor },
	{ "TextDrawFont", n_TextDrawFont },
	{ "TextDrawSetProportional", n_TextDrawSetProportional },
	{ "TextDrawSetSelectable", n_TextDrawSetSelectable },
	{ "TextDrawSetPreviewModel", n_TextDrawSetPreviewModel },
	{ "TextDrawSetPreviewRot", n_TextDrawSetPreviewRot },
	{ "TextDrawSetPreviewVehCol", n_TextDrawSetPreviewVehCol },
	{ "TextDrawShowForPlayer", n_TextDrawShowForPlayer },
	{ "TextDrawShowForAll", n_TextDrawShowForAll },
	{ "TextDrawHideForPlayer", n_TextDrawHideForPlayer },
	{ "TextDrawHideForAll", n_TextDrawHideForAll },
	{ "TextDrawDestroy", n_TextDrawDestroy },
	{ "CreateObject", n_CreateObject },
	{ "AttachObjectToVehicle", n_AttachObjectToVehicle },
	{ "AttachPlayerObjectToVehicle", n_AttachPlayerObjectToVehicle },
	{ "AttachObjectToObject", n_AttachObjectToObject },
	{ "SetObjectPos", n_SetObjectPos },
	{ "SetObjectRot", n_SetObjectRot },
	{ "GetObjectPos", n_GetObjectPos },
	{ "GetObjectRot", n_GetObjectRot },
	{ "GetObjectModel", n_GetObjectModel },
	{ "SetObjectNoCameraCol", n_SetObjectNoCameraCol },
	{ "IsValidObject", n_IsValidObject },
	{ "DestroyObject", n_DestroyObject },
	{ "IsObjectMoving", n_IsObjectMoving },
	{ "MoveObject", n_MoveObject },
	{ "StopObject", n_StopObject },
	{ "EditObject", n_EditObject },
	{ "EditPlayerObject", n_EditPlayerObject },
	{ "SelectObject", n_SelectObject },
	{ "CancelEdit", n_CancelEdit },
	{ "SetObjectMaterial", n_SetObjectMaterial },
	{ "SetObjectMaterialText", n_SetObjectMaterialText },
	{ "CreatePlayerObject", n_CreatePlayerObject },
	{ "SetPlayerObjectPos", n_SetPlayerObjectPos },
	{ "SetPlayerObjectRot", n_SetPlayerObjectRot },
	{ "GetPlayerObjectPos", n_GetPlayerObjectPos },
	{ "GetPlayerObjectRot", n_GetPlayerObjectRot },
	{ "GetPlayerObjectModel", n_GetPlayerObjectModel },
	{ "SetPlayerObjectNoCameraCol", n_SetPlayerObjectNoCameraCol },
	{ "IsValidPlayerObject", n_IsValidPlayerObject },
	{ "DestroyPlayerObject", n_DestroyPlayerObject },
	{ "IsPlayerObjectMoving", n_IsPlayerObjectMoving },
	{ "MovePlayerObject", n_MovePlayerObject },
	{ "StopPlayerObject", n_StopPlayerObject },
	{ "SetPlayerObjectMaterial", n_SetPlayerObjectMaterial },
	{ "SetPlayerObjectMaterialText", n_SetPlayerObjectMaterialText },
	{ "AttachObjectToPlayer", n_AttachObjectToPlayer },
	{ "AttachPlayerObjectToPlayer", n_AttachPlayerObjectToPlayer },
	{ "SetObjectsDefaultCameraCol", n_SetObjectsDefaultCameraCol },
	{ "CreateActor", n_CreateActor },
	{ "DestroyActor", n_DestroyActor },
	{ "SetActorPos", n_SetActorPos },
	{ "GetActorPos", n_GetActorPos },
	{ "SetActorVirtualWorld", n_SetActorVirtualWorld },
	{ "GetActorVirtualWorld", n_GetActorVirtualWorld },
	{ "ApplyActorAnimation", n_ApplyActorAnimation },
	{ "ClearActorAnimations", n_ClearActorAnimations },
	{ "SetActorFacingAngle", n_SetActorFacingAngle },
	{ "GetActorFacingAngle", n_GetActorFacingAngle },
	{ "GetActorHealth", n_GetActorHealth },
	{ "SetActorHealth", n_SetActorHealth },
	{ "IsValidActor", n_IsValidActor },
	{ "SetActorInvulnerable", n_SetActorInvulnerable },
	{ "IsActorInvulnerable", n_IsActorInvulnerable },
	{ "CreateMenu", n_CreateMenu },
	{ "DestroyMenu", n_DestroyMenu },
	{ "AddMenuItem", n_AddMenuItem },
	{ "SetMenuColumnHeader", n_SetMenuColumnHeader },
	{ "ShowMenuForPlayer", n_ShowMenuForPlayer },
	{ "HideMenuForPlayer", n_HideMenuForPlayer },
	{ "IsValidMenu", n_IsValidMenu },
	{ "DisableMenu", n_DisableMenu },
	{ "DisableMenuRow", n_DisableMenuRow },
	{ "GetPlayerMenu", n_GetPlayerMenu },
	{ "SetPlayerTime", n_SetPlayerTime },
	{ "TogglePlayerClock", n_TogglePlayerClock },
	{ "GetPlayerTime", n_GetPlayerTime },
	{ "HTTP", n_HTTP },

	{ NULL, NULL }
};

//----------------------------------------------------------------------------------

int amx_CustomInit(AMX *amx)
{
  return amx_Register(amx, custom_Natives, -1);
}

//----------------------------------------------------------------------------------
