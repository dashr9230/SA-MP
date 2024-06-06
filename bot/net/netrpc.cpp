
#include "../main.h"
using namespace RakNet;
extern CNetGame* pNetGame;

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2
#define REJECT_REASON_BAD_MOD		3

extern bool	bSpawned;

void EnterVehicle(RPCParameters *rpcParams) {}
void ExitVehicle(RPCParameters *rpcParams) {}
void SetCheckpoint(RPCParameters *rpcParams) {}
void DisableCheckpoint(RPCParameters *rpcParams) {}
void Unk26(RPCParameters *rpcParams) {}
void Unk27(RPCParameters *rpcParams) {}
void UpdateScoresPingsIPs(RPCParameters *rpcParams) {}

RakNetStatisticsStruct RakServerStats;

void SvrStats(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read((char *)&RakServerStats,sizeof(RakNetStatisticsStruct));
}

void GameModeRestart(RPCParameters *rpcParams)
{
	pNetGame->ShutdownForGameModeRestart();
}

void ConnectionRejected(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	BYTE byteRejectReason;

	bsData.Read(byteRejectReason);

	if(byteRejectReason==REJECT_REASON_BAD_VERSION) {
		//logprintf("BOT: CONNECTION REJECTED. INCORRECT SA-MP VERSION!");
	} 
	else if(byteRejectReason==REJECT_REASON_BAD_NICKNAME) {
		//logprintf("BOT: CONNECTION REJECTED. BAD NICKNAME!");
	}

	pNetGame->GetRakClient()->Disconnect(500);
	exit(1);
}

void ClientMessage(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	DWORD dwStrLen;
	DWORD dwColor;

	bsData.Read(dwColor);
	bsData.Read(dwStrLen);
	unsigned char* szMsg = (unsigned char *)malloc(dwStrLen+1);
	bsData.Read((char *)szMsg, dwStrLen);
	szMsg[dwStrLen] = 0;

	if(pNetGame->GetBotMode()) pNetGame->GetBotMode()->OnClientMessage(dwColor, szMsg);

	free(szMsg);
}

void WorldTime(RPCParameters *rpcParams) {}
void Unk5F(RPCParameters *rpcParams) {}
void Unk3F(RPCParameters *rpcParams) {}
void Unk97(RPCParameters *rpcParams) {}
void ScmEvent(RPCParameters *rpcParams) {}
void Weather(RPCParameters *rpcParams) {}
void Unk1D(RPCParameters *rpcParams) {}
void Unk1E(RPCParameters *rpcParams) {}

void Unk89(RPCParameters *rpcParams)
{
	// TODO: Unk89
}

void ServerQuit(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	PLAYERID playerId;
	BYTE byteReason;

	bsData.Read(playerId);
	bsData.Read(byteReason);

	// Delete this client from the player pool.
	pPlayerPool->Delete(playerId,byteReason);
}

int iNetModeNormalOnfootSendRate;
int iNetModeNormalIncarSendRate;
int iNetModeFiringSendRate;
int iNetModeSendMultiplier;
int iNetModeLagCompensation;

void InitGame(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsInitGame(Data,(iBitLength/8)+1,false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	PLAYERID MyPlayerID;
	bool bLanMode, bStuntBonus;
	bool bManualVehicleEngineAndLights;

	bsInitGame.Read(pNetGame->m_bZoneNames);
	bsInitGame.Read(pNetGame->m_bUseCJWalk);
	bsInitGame.Read(pNetGame->m_bAllowWeapons);
	bsInitGame.Read(pNetGame->m_bLimitGlobalChatRadius);
	bsInitGame.Read(pNetGame->m_fGlobalChatRadius);
	bsInitGame.Read(bStuntBonus);
	bsInitGame.Read(pNetGame->m_fNameTagDrawDistance);
	bsInitGame.Read(pNetGame->m_bDisableEnterExits);
	bsInitGame.Read(pNetGame->m_bNameTagLOS);
	pNetGame->m_bNameTagLOS = true;
	bsInitGame.Read(bManualVehicleEngineAndLights);
	bsInitGame.Read(pNetGame->m_iSpawnsAvailable);
	bsInitGame.Read(MyPlayerID);
	bsInitGame.Read(pNetGame->m_iPlayerMarkersMode);
	bsInitGame.Read(pNetGame->m_bShowPlayerMarkers);
	bsInitGame.Read(pNetGame->m_bTirePopping);
	bsInitGame.Read(pNetGame->m_byteWorldTime);
	bsInitGame.Read(pNetGame->m_byteWeather);
	bsInitGame.Read(pNetGame->m_fGravity);
	bsInitGame.Read(bLanMode);
	bsInitGame.Read(pNetGame->m_iDeathDropMoney);
	bsInitGame.Read(pNetGame->m_bInstagib);

	// Server's send rate restrictions
	bsInitGame.Read(iNetModeNormalOnfootSendRate);
	bsInitGame.Read(iNetModeNormalIncarSendRate);
	bsInitGame.Read(iNetModeFiringSendRate);
	bsInitGame.Read(iNetModeSendMultiplier);
	bsInitGame.Read(iNetModeLagCompensation);

	BYTE byteStrLen;
	bsInitGame.Read(byteStrLen);
	if(byteStrLen) {
		memset(pNetGame->m_szHostName,0,sizeof(pNetGame->m_szHostName));
		bsInitGame.Read(pNetGame->m_szHostName, byteStrLen);
	}
	pNetGame->m_szHostName[byteStrLen] = '\0';

	pPlayerPool->SetLocalPlayerID(MyPlayerID);
	if(bLanMode) pNetGame->SetLanMode(TRUE);
	pNetGame->SetGameState(GAMESTATE_CONNECTED);

	//logprintf("NPC: Got InitGame. Sending class request.");

	if(pNetGame->GetBotMode()) {
		pNetGame->GetBotMode()->OnNPCConnect(MyPlayerID);
	}

	RakNet::BitStream bsSpawnRequest;
	bsSpawnRequest.Write((int)0);
	pNetGame->GetRakClient()->RPC(RPC_RequestClass,&bsSpawnRequest,HIGH_PRIORITY,RELIABLE,0,false);
}

void Chat(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	PLAYERID playerId;
	BYTE byteTextLen;

	unsigned char szText[256];
	memset(szText,0,256);

	bsData.Read(playerId);
	bsData.Read(byteTextLen);
	bsData.Read((char*)szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(pNetGame->GetBotMode()) pNetGame->GetBotMode()->OnPlayerText(playerId,szText);
}

void RequestClass(RPCParameters *rpcParams)
{
	// TODO: RequestClass
}

void RequestSpawn(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteRequestOutcome=0;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteRequestOutcome);

	if(byteRequestOutcome) {
		// Let the rest of the network know we're spawning.
		if(pNetGame->GetBotMode()) pNetGame->GetBotMode()->OnNPCSpawn();
		bSpawned = true;
		RakNet::BitStream bsSendSpawn;
		pNetGame->GetRakClient()->RPC(RPC_Spawn,&bsSendSpawn,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
	}
}

void WorldPlayerAdd(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	PLAYERID playerId;
	BYTE byteFightingStyle=4;
	BYTE byteTeam=0;
	int iSkin=0;
	VECTOR vecPos;
	float fRotation=0;
	DWORD dwColor=0;

	bsData.Read(playerId);
	bsData.Read(byteTeam);
	bsData.Read(iSkin);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(dwColor);
	byteFightingStyle = 4;

	if(pNetGame->GetPlayerPool()->GetSlotState(playerId) == FALSE) return;
	pNetGame->SetPlayerAdded(playerId,TRUE);

	if(pNetGame->GetBotMode()) {
		pNetGame->GetBotMode()->OnPlayerStreamIn(playerId);
	}
}

void UnkA6(RPCParameters *rpcParams)
{
	// TODO: UnkA6
}

void UnkA3(RPCParameters *rpcParams)
{
	// TODO: UnkA3
}

void UnkA4(RPCParameters *rpcParams)
{
	// TODO: UnkA4
}

void UnkA5(RPCParameters *rpcParams)
{
	// TODO: UnkA5
}

//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	REGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	REGISTER_STATIC_RPC(pRakClient,Unk26);
	REGISTER_STATIC_RPC(pRakClient,Unk27);
	REGISTER_STATIC_RPC(pRakClient,SetCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	REGISTER_STATIC_RPC(pRakClient,SvrStats);
	REGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	REGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	REGISTER_STATIC_RPC(pRakClient,ClientMessage);
	REGISTER_STATIC_RPC(pRakClient,WorldTime);
	REGISTER_STATIC_RPC(pRakClient,Unk5F);
	REGISTER_STATIC_RPC(pRakClient,Unk3F);
	REGISTER_STATIC_RPC(pRakClient,Unk97);
	REGISTER_STATIC_RPC(pRakClient,ScmEvent);
	REGISTER_STATIC_RPC(pRakClient,Weather);
	REGISTER_STATIC_RPC(pRakClient,Unk1D);
	REGISTER_STATIC_RPC(pRakClient,Unk1E);
	REGISTER_STATIC_RPC(pRakClient,Unk89);
	REGISTER_STATIC_RPC(pRakClient,ServerQuit);
	REGISTER_STATIC_RPC(pRakClient,InitGame);
	REGISTER_STATIC_RPC(pRakClient,Chat);
	REGISTER_STATIC_RPC(pRakClient,RequestClass);
	REGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerAdd);
	REGISTER_STATIC_RPC(pRakClient,UnkA6);
	REGISTER_STATIC_RPC(pRakClient,UnkA3);
	REGISTER_STATIC_RPC(pRakClient,UnkA4);
	REGISTER_STATIC_RPC(pRakClient,UnkA5);
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,Unk26);
	UNREGISTER_STATIC_RPC(pRakClient,Unk27);
	UNREGISTER_STATIC_RPC(pRakClient,SetCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	UNREGISTER_STATIC_RPC(pRakClient,SvrStats);
	UNREGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	UNREGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	UNREGISTER_STATIC_RPC(pRakClient,ClientMessage);
	UNREGISTER_STATIC_RPC(pRakClient,WorldTime);
	UNREGISTER_STATIC_RPC(pRakClient,Unk5F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3F);
	UNREGISTER_STATIC_RPC(pRakClient,Unk97);
	UNREGISTER_STATIC_RPC(pRakClient,ScmEvent);
	UNREGISTER_STATIC_RPC(pRakClient,Weather);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1D);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1E);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerAdd);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA6);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA3);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA4);
	UNREGISTER_STATIC_RPC(pRakClient,UnkA5);
	UNREGISTER_STATIC_RPC(pRakClient,Unk89);
	UNREGISTER_STATIC_RPC(pRakClient,ServerQuit);
	UNREGISTER_STATIC_RPC(pRakClient,InitGame);
	UNREGISTER_STATIC_RPC(pRakClient,Chat);
	UNREGISTER_STATIC_RPC(pRakClient,RequestClass);
	UNREGISTER_STATIC_RPC(pRakClient,RequestSpawn);
}

//----------------------------------------------------
