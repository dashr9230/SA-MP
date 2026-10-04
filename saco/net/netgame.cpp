
#include "../main.h"
#include "../../raknet/SocketDataEncryptor.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CConfig       *pConfig;
extern CAudioStream  *pAudioStream;

void FUNC_100AABE0(); // .text:100AABE0

DWORD dword_10118A18;

//----------------------------------------------------

// MATCH
BYTE __stdcall GetPacketID(Packet *p)
{
	if (p==0) return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else {
		return (unsigned char) p->data[0];
	}
}

//----------------------------------------------------

// MATCH
bool __stdcall HasTimestamps(Packet *p)
{
	if (p==0) return 0;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return true;
	return false;
}

//----------------------------------------------------

CNetGame::CNetGame(PCHAR szHostOrIp, int iPort, 
				   PCHAR szPlayerName, PCHAR szPass)
{
	field_3D5 = (struc_41*)calloc(1,sizeof(struc_41));

	memset(m_szHostName,0,256);
	memset(m_szHostOrIp,0,256);

	strcpy(m_szHostName, "SA-MP");
	strncpy(m_szHostOrIp, szHostOrIp, sizeof(m_szHostOrIp));
	m_iPort = iPort;

	m_pRakClient = RakNetworkFactory::GetRakClientInterface();

	InitPools();

	SocketDataEncryptor::SetKey(iPort);

	RegisterRPCs(m_pRakClient);
	RegisterScriptRPCs(m_pRakClient);	// Register server-side scripting RPCs.

	m_iGameState = GAMESTATE_WAIT_CONNECT;

	if(pChatWindow) pChatWindow->AddDebugMessage("{FFFFFF}SA-MP {B9C9BF}0.3.7-R5 {FFFFFF}Started");

	// TODO: CNetGame::CNetGame(...) .text:1000B930
}

//----------------------------------------------------

void CNetGame::Process()
{
	UpdateNetwork();
	// TODO: CNetGame::Process()
}

//----------------------------------------------------
// UPDATE NETWORK
//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
/*	Packet* pkt=NULL;
	unsigned char packetIdentifier;

	while((pkt = m_pRakClient->Receive()))
	{

	}*/
	
	// TODO: CNetGame::UpdateNetwork()
}

// MATCH
void CNetGame::InitPools()
{
	m_pPools = (NETGAME_POOLS *)calloc(1, sizeof(NETGAME_POOLS));

	m_pPools->pObjectPool	= new CObjectPool();
	m_pPools->pMenuPool		= new CMenuPool();
	m_pPools->pLabelPool	= new CLabelPool();
	m_pPools->pTextDrawPool	= new CTextDrawPool();
	m_pPools->pGangZonePool	= new CGangZonePool();
	m_pPools->pPlayerPool	= new CPlayerPool();
	m_pPools->pVehiclePool	= new CVehiclePool();
	m_pPools->pPickupPool	= new CPickupPool();
	m_pPools->pActorPool	= new CActorPool();
}

// MATCH
DWORD CNetGame::GetTime()
{
	return (DWORD)RakNet::GetTime();
}

//-----------------------------------------------------------
// Puts a personal marker using any of the radar icons on the map

// MATCH
void CNetGame::SetMapIcon(BYTE byteIndex, float fX, float fY, float fZ, BYTE byteIcon, DWORD dwColor, int iStyle)
{
	if (byteIndex >= 100) return;
	if (m_dwMapIcon[byteIndex] != NULL) {
		if (byteIndex >= 100) return;
		ScriptCommand(&disable_marker, m_dwMapIcon[byteIndex]);
		m_dwMapIcon[byteIndex] = NULL;
	}
	//ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, byteIcon, &m_dwMapIcon);
	m_dwMapIcon[byteIndex] = pGame->CreateRadarMarkerIcon(byteIcon, fX, fY, fZ, dwColor, iStyle);
}

//-----------------------------------------------------------
// Removes the Map Icon


//----------------------------------------------------

// MATCH
// MATCH
void CNetGame::ResetMapIcons()
{
	BYTE i;
	for (i = 0; i < 100; i++)
	{
		if (m_dwMapIcon[i] != NULL) {
			if (i >= 100) continue;
			ScriptCommand(&disable_marker, m_dwMapIcon[i]);
			m_dwMapIcon[i] = NULL;
		}
	}
}

//----------------------------------------------------

// MATCH
void CNetGame::DisableMapIcon(BYTE byteIndex)
{
	if (byteIndex >= 100) return;
	ScriptCommand(&disable_marker, m_dwMapIcon[byteIndex]);
	m_dwMapIcon[byteIndex] = NULL;
}

//----------------------------------------------------

//----------------------------------------------------

void CNetGame::Packet_RSAPublicKeyMismatch(Packet *p)
{
	pChatWindow->AddDebugMessage("Failed to initialize encryption.");
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionBanned(Packet *p)
{
	pChatWindow->AddDebugMessage("You are banned from this server.");
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionSucceeded(Packet *p)
{
	pChatWindow->AddDebugMessage("Server has accepted the connection.");
}

//----------------------------------------------------

// MATCH
void CNetGame::Packet_NoFreeIncomingConnections(Packet *p)
{
	pChatWindow->AddDebugMessage("The server is full. Retrying...");
	m_iGameState = GAMESTATE_WAIT_CONNECT;
}

//----------------------------------------------------

// MATCH
void CNetGame::Packet_InvalidPassword(Packet *p)
{
	pChatWindow->AddDebugMessage("Wrong server password.");
	GetRakClient()->Disconnect(0,0);
}

//----------------------------------------------------

// MATCH
void CNetGame::Packet_ConnectionAttemptFailed(Packet *p)
{
	pChatWindow->AddDebugMessage("The server didn't respond. Retrying..");
	m_iGameState = GAMESTATE_WAIT_CONNECT;
}

//----------------------------------------------------

// MATCH
void CNetGame::ToggleNameTagStatus()
{
	if(!field_234)
	{
		pChatWindow->AddInfoMessage("NameTag Player Status: ON");
		if(pConfig) pConfig->SetIntVariable("nonametagstatus",0);
		field_234 = 1;
	}
	else
	{
		pChatWindow->AddInfoMessage("NameTag Player Status: OFF");
		if(pConfig) pConfig->SetIntVariable("nonametagstatus",1);
		field_234 = 0;
	}
}

//----------------------------------------------------

// MATCH
void CNetGame::AttemptConnect()
{
	if(GetTickCount() - field_3D1 > 3000)
	{
		if(pChatWindow)
			pChatWindow->AddDebugMessage("Connecting to %s:%d...",m_szHostOrIp,m_iPort);

		m_pRakClient->Connect(m_szHostOrIp,m_iPort,0,0,2);

		field_3D1 = GetTickCount();
		m_iGameState = GAMESTATE_CONNECTING;
	}
}

//----------------------------------------------------

void CNetGame::UpdateScoresPingsIPs()
{
	if(GetTickCount() - dword_10118A18 > 3000)
	{
		dword_10118A18 = GetTickCount();

		RakNet::BitStream bsSend;
		if(GetRakClient())
			GetRakClient()->RPC(RPC_UpdateScoresPingsIPs, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE);
	}
}

//----------------------------------------------------

void CNetGame::ResetWorldBounds()
{
	if(field_3D5->field_2B) FUNC_100AABE0();

	field_3D5->fWorldBoundryPX = 20000.0f;
	field_3D5->fWorldBoundryZX = -20000.0f;
	field_3D5->fWorldBoundryPY = 20000.0f;
	field_3D5->fWorldBoundryNY = -20000.0f;
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetPlayerPool()
{
	delete m_pPools->pPlayerPool;
	m_pPools->pPlayerPool = new CPlayerPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetGangZonePool()
{
	delete m_pPools->pGangZonePool;
	m_pPools->pGangZonePool = new CGangZonePool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetPickupPool()
{
	delete m_pPools->pPickupPool;
	m_pPools->pPickupPool = new CPickupPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetMenuPool()
{
	delete m_pPools->pMenuPool;
	m_pPools->pMenuPool = new CMenuPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetLabelPool()
{
	delete m_pPools->pLabelPool;
	m_pPools->pLabelPool = new CLabelPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetActorPool()
{
	delete m_pPools->pActorPool;
	m_pPools->pActorPool = new CActorPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetVehiclePool()
{
	delete m_pPools->pVehiclePool;
	m_pPools->pVehiclePool = new CVehiclePool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetTextDrawPool()
{
	delete m_pPools->pTextDrawPool;
	m_pPools->pTextDrawPool = new CTextDrawPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::ResetObjectPool()
{
	delete m_pPools->pObjectPool;
	m_pPools->pObjectPool = new CObjectPool();
}

//----------------------------------------------------

// MATCH
void CNetGame::Packet_DisconnectionNotification(Packet *p)
{
	pChatWindow->AddDebugMessage("Server closed the connection.");

	if(pAudioStream) pAudioStream->Stop(1);

	GetRakClient()->Disconnect(2000,0);
}

//----------------------------------------------------

void CNetGame::Packet_AimSync(Packet *p)
{
	RakNet::BitStream bsAimSync((char *)p->data, p->length, false);
	char aimSync[31];
	WORD wPlayerID;
	BYTE bytePacketID = 0;

	if(m_iGameState != GAMESTATE_CONNECTED) return;

	bsAimSync.Read(bytePacketID);
	bsAimSync.Read(wPlayerID);
	bsAimSync.Read(aimSync, sizeof(aimSync));

	CPlayerPool *pPlayerPool = GetPlayerPool();

	if(wPlayerID <= MAX_PLAYERS)
	{
		CNetPlayer *pPlayer = pPlayerPool->m_pPlayers[wPlayerID];
		if(pPlayer && pPlayer->m_pRemotePlayer)
			pPlayer->m_pRemotePlayer->FUNC_10015760(aimSync);
	}
}

//----------------------------------------------------

void CNetGame::Packet_PassengerSync(Packet *p)
{
	RakNet::BitStream bsPassengerSync((char *)p->data, p->length, false);
	char passengerSync[24];
	WORD wPlayerID;
	BYTE bytePacketID = 0;

	if(m_iGameState != GAMESTATE_CONNECTED) return;

	bsPassengerSync.Read(bytePacketID);
	bsPassengerSync.Read(wPlayerID);
	bsPassengerSync.Read(passengerSync, sizeof(passengerSync));

	CPlayerPool *pPlayerPool = GetPlayerPool();

	if(wPlayerID <= MAX_PLAYERS)
	{
		CNetPlayer *pPlayer = pPlayerPool->m_pPlayers[wPlayerID];
		if(pPlayer && pPlayer->m_pRemotePlayer)
			pPlayer->m_pRemotePlayer->FUNC_10017440(passengerSync);
	}
}

//----------------------------------------------------

void CNetGame::Packet_TrailerSync(Packet *p)
{
	RakNet::BitStream bsTrailerSync((char *)p->data, p->length, false);
	char trailerSync[54];
	WORD wPlayerID;
	BYTE bytePacketID;

	if(m_iGameState != GAMESTATE_CONNECTED) return;

	bytePacketID = 0;

	bsTrailerSync.Read(bytePacketID);
	bsTrailerSync.Read(wPlayerID);
	bsTrailerSync.Read(trailerSync, sizeof(trailerSync));

	CPlayerPool *pPlayerPool = GetPlayerPool();

	if(wPlayerID <= MAX_PLAYERS)
	{
		CNetPlayer *pPlayer = pPlayerPool->m_pPlayers[wPlayerID];
		if(pPlayer && pPlayer->m_pRemotePlayer)
			pPlayer->m_pRemotePlayer->FUNC_10015C90(trailerSync);
	}
}
