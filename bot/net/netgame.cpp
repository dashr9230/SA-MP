
#include "../main.h"
#include "../../raknet/SocketDataEncryptor.h"
#include "../mathutils.h"

#define NETGAME_VERSION 4057

char szGameModeFile[256];

#define PLAYER_STATE_NONE						0
#define PLAYER_STATE_ONFOOT						1
#define PLAYER_STATE_DRIVER						2
#define PLAYER_STATE_PASSENGER					3
#define PLAYER_STATE_WASTED						7
#define PLAYER_STATE_SPAWNED					8
#define PLAYER_STATE_SPECTATING					9

#pragma pack(1)
typedef struct _AIM_SYNC_DATA // size: 31
{
	char _gap0[31];
} AIM_SYNC_DATA;

#pragma pack(1)
typedef struct _TRAILER_SYNC_DATA // size: 54
{
	char _gap0[54];
} TRAILER_SYNC_DATA;

char unnamed_2[63];
PASSENGER_SYNC_DATA unnamed_5[MAX_PLAYERS];
BOOL bPlayerSlotState[MAX_PLAYERS];
BYTE byteState;
ONFOOT_SYNC_DATA ofSync;
ONFOOT_SYNC_DATA unnamed_3[MAX_PLAYERS];
BYTE bytePlayerState[MAX_PLAYERS];
BOOL bVehicleSlotState[MAX_VEHICLES];
INCAR_SYNC_DATA unnamed_4[MAX_PLAYERS];
BYTE byteMySeatID;

bool	bSpawned = false;

void CNetGame::SetPlayerAdded(PLAYERID playerId, BOOL a2)
{
	if(playerId < MAX_PLAYERS)
	{
		bPlayerSlotState[playerId] = a2;
	}
}

void CNetGame::SetVehicleAdded(VEHICLEID VehicleID, BOOL a2)
{
	if(VehicleID < MAX_VEHICLES)
	{
		bVehicleSlotState[VehicleID] = a2;
	}
}

void CNetGame::SetPlayerState(PLAYERID playerId, BYTE byteState)
{
	if(playerId < MAX_PLAYERS)
	{
		bytePlayerState[playerId] = byteState;
	}
}

BYTE CNetGame::GetPlayerState(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return PLAYER_STATE_NONE;
	if(bPlayerSlotState[playerId] == FALSE) return PLAYER_STATE_NONE;

	return bytePlayerState[playerId];
}

BOOL CNetGame::GetPlayerPos(PLAYERID playerId, PVECTOR Vector)
{
	if(playerId >= MAX_PLAYERS || !bPlayerSlotState[playerId])
		return FALSE;

	switch(bytePlayerState[playerId])
	{
	case PLAYER_STATE_ONFOOT:
		Vector->X = unnamed_3[playerId].vecPos.X;
		Vector->Y = unnamed_3[playerId].vecPos.Y;
		Vector->Z = unnamed_3[playerId].vecPos.Z;
		return TRUE;

	case PLAYER_STATE_DRIVER:
		Vector->X = unnamed_4[playerId].vecPos.X;
		Vector->Y = unnamed_4[playerId].vecPos.Y;
		Vector->Z = unnamed_4[playerId].vecPos.Z;
		return TRUE;

	case PLAYER_STATE_PASSENGER:
		Vector->X = unnamed_5[playerId].vecPos.X;
		Vector->Y = unnamed_5[playerId].vecPos.Y;
		Vector->Z = unnamed_5[playerId].vecPos.Z;
		return TRUE;
	}
	return FALSE;
}

VEHICLEID CNetGame::GetPlayerVehicleID(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return INVALID_VEHICLE_ID;
	if(bPlayerSlotState[playerId] == FALSE) return INVALID_VEHICLE_ID;

	if(bytePlayerState[playerId] == PLAYER_STATE_DRIVER)
	{
		return unnamed_4[playerId].VehicleID;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_PASSENGER)
	{
		return unnamed_5[playerId].VehicleID;
	}
	else
	{
		return INVALID_VEHICLE_ID;
	}
}

BYTE CNetGame::GetPlayerArmedWeapon(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return 0;
	if(bPlayerSlotState[playerId] == FALSE) return 0;

	if(bytePlayerState[playerId] == PLAYER_STATE_ONFOOT)
	{
		return unnamed_3[playerId].byteCurrentWeapon;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_DRIVER)
	{
		return unnamed_4[playerId].byteCurrentWeapon;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_PASSENGER)
	{
		return unnamed_5[playerId].byteCurrentWeapon;
	}
	else
	{
		return 0;
	}
}

BYTE CNetGame::GetPlayerHealth(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return 0;
	if(bPlayerSlotState[playerId] == FALSE) return 0;

	if(bytePlayerState[playerId] == PLAYER_STATE_ONFOOT)
	{
		return unnamed_3[playerId].byteHealth;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_DRIVER)
	{
		return unnamed_4[playerId].bytePlayerHealth;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_PASSENGER)
	{
		return unnamed_5[playerId].bytePlayerHealth;
	}
	else
	{
		return 0;
	}
}

BYTE CNetGame::GetPlayerArmour(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return 0;
	if(bPlayerSlotState[playerId] == FALSE) return 0;

	if(bytePlayerState[playerId] == PLAYER_STATE_ONFOOT)
	{
		return unnamed_3[playerId].byteArmour;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_DRIVER)
	{
		return unnamed_4[playerId].bytePlayerArmour;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_PASSENGER)
	{
		return unnamed_5[playerId].bytePlayerArmour;
	}
	else
	{
		return 0;
	}
}

BOOL CNetGame::GetPlayerKeys(PLAYERID playerId, WORD *udAnalog, WORD *lrAnalog, WORD *wKeys)
{
	if(playerId >= MAX_PLAYERS) return FALSE;
	if(bPlayerSlotState[playerId] == FALSE) return FALSE;

	if(bytePlayerState[playerId] == PLAYER_STATE_ONFOOT)
	{
		*udAnalog = unnamed_3[playerId].udAnalog;
		*lrAnalog = unnamed_3[playerId].lrAnalog;
		*wKeys = unnamed_3[playerId].wKeys;
		return TRUE;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_DRIVER)
	{
		*udAnalog = unnamed_4[playerId].udAnalog;
		*lrAnalog = unnamed_4[playerId].lrAnalog;
		*wKeys = unnamed_4[playerId].wKeys;
		return TRUE;
	}
	else if(bytePlayerState[playerId] == PLAYER_STATE_PASSENGER)
	{
		*udAnalog = unnamed_5[playerId].udAnalog;
		*lrAnalog = unnamed_5[playerId].lrAnalog;
		*wKeys = unnamed_5[playerId].wKeys;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BYTE CNetGame::GetPlayerSpecialAction(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return SPECIAL_ACTION_NONE;
	if(bPlayerSlotState[playerId] == FALSE) return SPECIAL_ACTION_NONE;

	if(bytePlayerState[playerId] == PLAYER_STATE_ONFOOT)
	{
		return unnamed_3[playerId].byteSpecialAction;
	}
	return SPECIAL_ACTION_NONE;
}

//----------------------------------------------------
// MATCH
BOOL CNetGame::IsPlayerAdded(PLAYERID playerId)
{
	if(playerId >= MAX_PLAYERS) return FALSE;

	return bPlayerSlotState[playerId] != FALSE;
}

//----------------------------------------------------
BOOL CNetGame::IsVehicleAdded(VEHICLEID VehicleID)
{
	if(VehicleID >= MAX_VEHICLES) return FALSE;

	return bVehicleSlotState[VehicleID] != FALSE;
}

//----------------------------------------------------
float CNetGame::GetDistanceFromMeToPoint(PVECTOR vecPos)
{
	VECTOR vecMyPos;

	if(GetMyPos(&vecMyPos))
	{
	    float fX = vecMyPos.X - vecPos->X;
	    float fY = vecMyPos.Y - vecPos->Y;
	    float fZ = vecMyPos.Z - vecPos->Z;

	    return (float)sqrt(fX * fX + fY * fY + fZ * fZ);
	}
	else
	{
		return 0.0f;
	}
}

//----------------------------------------------------
PVECTOR CNetGame::GetMyPos(PVECTOR Vector)
{
	if(byteState == PLAYER_STATE_ONFOOT)
	{
		Vector->X = ofSync.vecPos.X;
		Vector->Y = ofSync.vecPos.Y;
		Vector->Z = ofSync.vecPos.Z;
		return Vector;
	}
	else if(byteState == PLAYER_STATE_DRIVER)
	{
		Vector->X = icSync.vecPos.X;
		Vector->Y = icSync.vecPos.Y;
		Vector->Z = icSync.vecPos.Z;
		return Vector;
	}
	else
	{
		return NULL;
	}
}

//----------------------------------------------------
void CNetGame::SetMyPos(PVECTOR Vector)
{
	if(byteState == PLAYER_STATE_ONFOOT)
	{
		ofSync.vecPos.X = Vector->X;
		ofSync.vecPos.Y = Vector->Y;
		ofSync.vecPos.Z = Vector->Z;
	}
	else if (byteState == PLAYER_STATE_DRIVER)
	{
		icSync.vecPos.X = Vector->X;
		icSync.vecPos.Y = Vector->Y;
		icSync.vecPos.Z = Vector->Z;
	}
}

//----------------------------------------------------
float CNetGame::GetMyZAngle()
{
	MATRIX4X4 mat;

	if(byteState == PLAYER_STATE_ONFOOT)
	{
		QuaternionToMatrix(&ofSync.quatRotation, &mat);

		float fZAngle = atan2(-mat.up.X, mat.up.Y) * 180.0f / PI;
		// Bound it to [0, 360)
		if ( fZAngle < 0.0f )
			fZAngle += 360.0f;
		else if ( fZAngle >= 360.0f )
			fZAngle -= 360.0f;
		return fZAngle;
	}
	else if(byteState == PLAYER_STATE_DRIVER)
	{
		QuaternionToMatrix(&icSync.quatRotation, &mat);

		float fZAngle = atan2(-mat.up.X, mat.up.Y) * 180.0f/PI;

		// Bound it to [0, 360)
		if ( fZAngle < 0.0f )
			fZAngle += 360.0f;
		else if ( fZAngle >= 360.0f )
			fZAngle -= 360.0f;
		return fZAngle;
	}

	return 0.0f;
}

//----------------------------------------------------
// MATCH
void CNetGame::SetMyZAngle(float fAngle)
{
	//logprintf("CNetGame::SetMyZAngle(%f)", fAngle);

	float fRadians = fAngle * 0.017453292f; // (PI/180.0f)

	MATRIX4X4 mat;
	memset(&mat,0,sizeof(MATRIX4X4));
	mat.right.Z = 0.0f;
	mat.up.Z = 0.0f;
	mat.at.X = 0.0f;
	mat.at.Y = 0.0f;
	mat.at.Z = 1.0f;

	float fCos = cos(fRadians);
	float fSin = sin(fRadians);

	mat.right.X = fCos;
	mat.right.Y = fSin;
	mat.up.X = -fSin;
	mat.up.Y = fCos;

	MatrixToQuaternion(&mat, &ofSync.quatRotation);
}

//----------------------------------------------------

// TODO: length assert has to be at line 397
// what the fuck is on top of the netgame.cpp that makes GetPacketID is at line 397 ????
BYTE GetPacketID(Packet *p)
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

CNetGame::CNetGame()
{
	// nothing
}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	m_pRakClient->Disconnect(0);
	UnRegisterRPCs(m_pRakClient);
	UnRegisterScriptRPCs(m_pRakClient);	// Unregister server-side scripting RPCs.
	RakNetworkFactory::DestroyRakClientInterface(m_pRakClient);
	SAFE_DELETE(m_pGameMode);
	SAFE_DELETE(m_pScriptTimers);
	SAFE_DELETE(m_pPlayerPool);
	SAFE_DELETE(m_pVehiclePool);
}

//----------------------------------------------------

void CNetGame::ShutdownForGameModeRestart()
{
	m_byteWorldTime = 12;
	m_byteWorldMinute = 0;
	m_byteWeather	= 10;
	m_byteHoldTime = 1;
	m_bUseCJWalk = FALSE;
	m_fGravity = (float)0.008000000;
	m_iDeathDropMoney = 0;

	m_iGameState = GAMESTATE_RESTARTING;

	// Process the pool one last time
	m_pPlayerPool->Process();

	ResetVehiclePool();

	StopRecordingPlayback();

	memset(unnamed_2,0,sizeof(unnamed_2));
	memset(unnamed_3,0,sizeof(unnamed_3));
	memset(unnamed_4,0,sizeof(unnamed_4));
	memset(unnamed_5,0,sizeof(unnamed_5));
	memset(&bVehicleSlotState[0],0,sizeof(BOOL)*MAX_VEHICLES);
	memset(&ofSync,0,sizeof(ONFOOT_SYNC_DATA));
	memset(&bPlayerSlotState[0],0,sizeof(BOOL)*MAX_PLAYERS);
	memset(&bytePlayerState[0],0,sizeof(BYTE)*MAX_PLAYERS);

	m_bZoneNames = FALSE;
}

//----------------------------------------------------

void CNetGame::Init(PCHAR szHostOrIp, int iPort,
				    PCHAR szPlayerName, PCHAR szPass,
				    PCHAR szNpcMode)
{
	strcpy(m_szHostName, "San Andreas Multiplayer");
	strncpy(m_szHostOrIp, szHostOrIp, sizeof(m_szHostOrIp));
	m_iPort = iPort;

	m_pGameMode = new CGameMode();
	m_pScriptTimers = new CScriptTimers();

	// Setup player pool
	m_pPlayerPool = new CPlayerPool();
	m_pPlayerPool->SetLocalPlayerName(szPlayerName);
	m_pVehiclePool = new CVehiclePool();

	m_pRakClient = RakNetworkFactory::GetRakClientInterface();

	SocketDataEncryptor::SetKey(iPort);

	RegisterRPCs(m_pRakClient);
	RegisterScriptRPCs(m_pRakClient);	// Register server-side scripting RPCs.
	
	sprintf(szGameModeFile, "npcmodes/%s.amx", szNpcMode);
	if (!m_pGameMode->Load(szGameModeFile))
	{
		//logprintf("NPC: I can't load %s so I'm quiting.", szGameModeFile);
		exit(1);
	}

	m_pRakClient->SetPassword(szPass);
	m_pRakClient->Connect(szHostOrIp,iPort,0,0,10);

	m_iGameState = GAMESTATE_CONNECTING;

	//logprintf("NPC(%s): connecting to %s:%d...",szPlayerName,szHostOrIp,iPort);

	m_iSpawnsAvailable = 0;
	m_byteWorldTime = 12;
	m_byteWorldMinute = 0;
	m_byteWeather	= 10;
	m_fGravity = (float)0.008000000;
	m_iDeathDropMoney = 0;
	m_bLanMode = FALSE;
	m_byteHoldTime = 1;
	m_bUseCJWalk = FALSE;

	int i;
	for (i = 0; i < 100; i++) m_dwMapIcon[i] = NULL;

	m_byteFriendlyFire = 1;
	m_bZoneNames = FALSE;
	m_bInstagib = FALSE;

	memset(unnamed_2,0,sizeof(unnamed_2));
	memset(&ofSync,0,sizeof(ONFOOT_SYNC_DATA));
	memset(unnamed_3,0,sizeof(unnamed_3));
	memset(unnamed_4,0,sizeof(unnamed_4));
	memset(unnamed_5,0,sizeof(unnamed_5));
	memset(&bVehicleSlotState[0],0,sizeof(BOOL)*MAX_VEHICLES);
	memset(&bPlayerSlotState[0],0,sizeof(BOOL)*MAX_PLAYERS);
	memset(&bytePlayerState[0],0,sizeof(BYTE)*MAX_PLAYERS);
	field_1DE = 0;
	field_1E2 = 0;
	field_1F2 = GetTickCount();
	byteState = PLAYER_STATE_NONE;
	field_1FA = -1;
	field_1FE = -1;
	srand(0);
}

//----------------------------------------------------

#ifdef WIN32

#pragma comment(lib, "winmm.lib")
float GetElapsedTime()
{
	static BOOL bTimerInit = false;
	static BOOL bUsingOPF  = false;
	static LONGLONG nTicksPerSec = 0;

	if (!bTimerInit)
	{
		bTimerInit = true;
		LARGE_INTEGER qwTicksPerSec;
		bUsingOPF = QueryPerformanceFrequency(&qwTicksPerSec);
		if (bUsingOPF) nTicksPerSec = qwTicksPerSec.QuadPart;
	}

	if (bUsingOPF)
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter(&qwTime);
		static LONGLONG llLastTime = qwTime.QuadPart;
		double fElapsedTime = (double)(qwTime.QuadPart - llLastTime) / (double) nTicksPerSec;
		llLastTime = qwTime.QuadPart;
		return (float)fElapsedTime;
	} else {
		double fTime = timeGetTime() * 0.001;
		static double fLastTime = fTime;
		double fElapsedTime = (double)(fTime - fLastTime);
		fLastTime = fTime;
		return (float)fElapsedTime;
	}
}

#else

float GetElapsedTime()
{
	static timeval lasttv;
	timeval tv;
	float fRet;

    gettimeofday(&tv, NULL);

	if (!timerisset(&lasttv)) memcpy(&lasttv, &tv, sizeof(timeval));

	fRet = (float)((tv.tv_sec - lasttv.tv_sec) * 1000000) + (tv.tv_usec - lasttv.tv_usec);
	fRet /= 1000000.0f;
	
	memcpy(&lasttv,&tv,sizeof(timeval));

    return fRet;
}

#endif // WIN32

//----------------------------------------------------

void CNetGame::Process()
{
	float fElapsedTime = GetElapsedTime();

	UpdateNetwork();



	// TODO: CNetGame::Process (W: 00418370 L: 080AD6A4)
	/*
	if ( this->field_C == 2 )
	{
		if ( this->field_386 )
			sub_80A969E(this->field_386, v2);
		if ( this->field_38A )
			sub_80B8610(this->field_38A, (signed __int64)(v2 * 1000.0));
		if ( byte_810A708 )
		{
			if ( this->field_1DE )
			{
				sub_80ADFDE(this);
			}
			else
			{
				sub_80AD770(this);
				byte_80E16B7 = 1;
			}
		}
	}*/
}

//----------------------------------------------------
// UPDATE NETWORK
//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
	Packet* pkt=NULL;
	unsigned char packetIdentifier;

	while((pkt = m_pRakClient->Receive()))
	{
		packetIdentifier = GetPacketID(pkt);

		switch(packetIdentifier)
		{
		case ID_RSA_PUBLIC_KEY_MISMATCH:
			Packet_RSAPublicKeyMismatch(pkt);
			break;
		case ID_CONNECTION_BANNED:
			Packet_ConnectionBanned(pkt);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Packet_NoFreeIncomingConnections(pkt);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(pkt);
			break;
		case ID_CONNECTION_LOST:
			Packet_ConnectionLost(pkt);
			break;
		case ID_INVALID_PASSWORD:
			Packet_InvalidPassword(pkt);
			break;
		case ID_MODIFIED_PACKET:
			Packet_ModifiedPacket(pkt);
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			Packet_ConnectAttemptFailed(pkt);
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Packet_ConnectionSucceeded(pkt);
			break;
		case ID_PASSENGER_SYNC:
			Packet_PassengerSync(pkt);
			break;
		case ID_AIM_SYNC:
			Packet_AimSync(pkt);
			break;
		case ID_TRAILER_SYNC:
			Packet_TrailerSync(pkt);
			break;
		}

		m_pRakClient->DeallocatePacket(pkt);
	}
}

//----------------------------------------------------
// PACKET HANDLERS INTERNAL
//----------------------------------------------------

void CNetGame::Packet_AimSync(Packet *p)
{
	RakNet::BitStream bsAimSync((PCHAR)p->data, p->length, false);
	AIM_SYNC_DATA aimSync;
	BYTE bytePacketID=0;
	BYTE bytePlayerID=0;

	if(GetGameState() != GAMESTATE_CONNECTED) return;

	bsAimSync.Read(bytePacketID);
	bsAimSync.Read(bytePlayerID);
	bsAimSync.Read((PCHAR)&aimSync,sizeof(AIM_SYNC_DATA));
}

//----------------------------------------------------

void CNetGame::Packet_PassengerSync(Packet *p)
{
	RakNet::BitStream bsPassengerSync((PCHAR)p->data, p->length, false);
	BYTE		bytePacketID=0;
	PLAYERID	playerId=0;
	PASSENGER_SYNC_DATA psSync;

	if(GetGameState() != GAMESTATE_CONNECTED) return;

	bsPassengerSync.Read(bytePacketID);
	bsPassengerSync.Read(playerId);
	bsPassengerSync.Read((PCHAR)&psSync,sizeof(PASSENGER_SYNC_DATA));

	if (playerId < MAX_PLAYERS)
	{
		memcpy(&unnamed_5[playerId],&psSync,sizeof(PASSENGER_SYNC_DATA));
		bytePlayerState[playerId] = PLAYER_STATE_PASSENGER;
	}
}

//----------------------------------------------------

void CNetGame::Packet_TrailerSync(Packet *p)
{
	RakNet::BitStream bsTrailerSync((PCHAR)p->data, p->length, false);
	if(GetGameState() != GAMESTATE_CONNECTED) return;

	BYTE bytePacketID=0;
	BYTE bytePlayerID=0;
	TRAILER_SYNC_DATA trSync;

	bsTrailerSync.Read(bytePacketID);
	bsTrailerSync.Read(bytePlayerID);
	bsTrailerSync.Read((PCHAR)&trSync, sizeof(TRAILER_SYNC_DATA));
}

//----------------------------------------------------

void CNetGame::Packet_RSAPublicKeyMismatch(Packet* packet)
{
	//logprintf("NPC: Failed to initialize encryption.");
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionBanned(Packet* packet)
{
	//logprintf("NPC: You're banned from the server.");
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionRequestAccepted(Packet* packet)
{
	//logprintf("NPC: Server has accepted the connection.");
}

//----------------------------------------------------

void CNetGame::Packet_NoFreeIncomingConnections(Packet* packet)
{
	//logprintf("NPC: The server is full.");
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_DisconnectionNotification(Packet* packet)
{
	//logprintf("NPC: Disconnected.");
	m_pRakClient->Disconnect(0);
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_ConnectionLost(Packet* packet)
{
	//logprintf("NPC: Lost connection to the server.");
	m_pRakClient->Disconnect(0);
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_InvalidPassword(Packet* packet)
{
	//logprintf("NPC: Wrong server password.");
	m_pRakClient->Disconnect(0);
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_ModifiedPacket(Packet* packet)
{
}

//----------------------------------------------------
// RST

void CNetGame::Packet_ConnectAttemptFailed(Packet* packet)
{
	//logprintf("NPC: Connection attempt failed.");
	exit(1);
}

//----------------------------------------------------
// Connection Success

void CNetGame::Packet_ConnectionSucceeded(Packet *p)
{
	m_iGameState = GAMESTATE_AWAIT_JOIN;

	RakNet::BitStream bsReturnParams((PCHAR)p->data, p->length, true);

	BYTE bytePacketID=0;
	unsigned int binaryAddr=0;
	unsigned short port=0;
	unsigned short playerId=0;
	unsigned int uiChallenge=0;

	bsReturnParams.Read(bytePacketID);
	bsReturnParams.Read(binaryAddr);
	bsReturnParams.Read(port);
	bsReturnParams.Read(playerId);
	bsReturnParams.Read(uiChallenge);

	uiChallenge ^= NETGAME_VERSION;

	//logprintf("NPC: Connection Succeeded");

	int iVersion = NETGAME_VERSION;
	BYTE byteMod = 1;
	BYTE byteNameLen = (BYTE)strlen(m_pPlayerPool->GetLocalPlayerName());

	RakNet::BitStream bsSend;
	bsSend.Write(iVersion);
	bsSend.Write(byteMod);
	bsSend.Write(byteNameLen);
	bsSend.Write(m_pPlayerPool->GetLocalPlayerName(),byteNameLen);
	bsSend.Write(uiChallenge);

	m_pRakClient->RPC(RPC_NPCJoin,&bsSend,HIGH_PRIORITY,RELIABLE,0,FALSE);
}

//----------------------------------------------------

void CNetGame::ResetVehiclePool()
{
	if(m_pVehiclePool) {
		delete m_pVehiclePool;
	}
	m_pVehiclePool = new CVehiclePool();
}

//----------------------------------------------------

void CNetGame::ResetPlayerPool()
{
	if(m_pPlayerPool) {
		delete m_pPlayerPool;
	}
	m_pPlayerPool = new CPlayerPool();
}

//----------------------------------------------------

void CNetGame::SendChat(char *szText)
{
	BYTE byteTextLen = strlen(szText);

	RakNet::BitStream bsSend;
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);

	GetRakClient()->RPC(RPC_Chat,&bsSend,HIGH_PRIORITY,RELIABLE,0,false);
}

void CNetGame::SendCommand(char *szCommand)
{
	RakNet::BitStream bsParams;
	int iStrlen = strlen(szCommand);

	bsParams.Write(iStrlen);
	bsParams.Write(szCommand, iStrlen);
	GetRakClient()->RPC(RPC_ServerCommand,&bsParams,HIGH_PRIORITY,RELIABLE,0,false);
}

DWORD *GLOBAL_7;
ONFOOT_SYNC_DATA *GLOBAL_8;
INCAR_SYNC_DATA *GLOBAL_6;
DWORD GLOBAL_5;

void CNetGame::FUNC_5(FILE *a1, DWORD a2)
{
	DWORD i=0;

	if(0x43 * (a2 / 0x43) != a2)
	{
		//logprintf("NPC: Total failure. File doesn't have correct data alignment for driver recording.");
		exit(1);
	}
	GLOBAL_5 = a2 / 0x43;
	if(GLOBAL_6)
		free(GLOBAL_6);
	if(GLOBAL_7)
		free(GLOBAL_7);
	GLOBAL_6 = (INCAR_SYNC_DATA *)calloc(1, sizeof(INCAR_SYNC_DATA) * GLOBAL_5);
	GLOBAL_7 = (DWORD *)calloc(1, sizeof(DWORD) * GLOBAL_5);
	while(!feof(a1))
	{
		fread(&GLOBAL_7[i], 1, sizeof(DWORD), a1);
		fread(&GLOBAL_6[i], 1, sizeof(INCAR_SYNC_DATA), a1);
		i++;
	}
}

void CNetGame::FUNC_6(FILE *a1, DWORD a2)
{
	DWORD i=0;

	if(0x48 * (a2 / 0x48) != a2)
	{
		//logprintf("NPC: Total failure. File doesn't have correct data alignment for onfoot recording.");
		exit(1);
	}
	GLOBAL_5 = a2 / 0x48;
	if(GLOBAL_8)
		free(GLOBAL_8);
	if(GLOBAL_7)
		free(GLOBAL_7);
	GLOBAL_8 = (ONFOOT_SYNC_DATA *)calloc(1, sizeof(ONFOOT_SYNC_DATA) * GLOBAL_5);
	GLOBAL_7 = (DWORD *)calloc(1, sizeof(DWORD) * GLOBAL_5);
	while(!feof(a1))
	{
		fread(&GLOBAL_7[i], 1, sizeof(DWORD), a1);
		fread(&GLOBAL_8[i], 1, sizeof(ONFOOT_SYNC_DATA), a1);
		i++;
	}
}

void CNetGame::StopRecordingPlayback()
{
	field_1DE = 0;
	field_1DA = 0;
}

void CNetGame::PauseRecordingPlayback()
{
	if(field_1DE == 1 || field_1DE == 2)
		field_1FA = 11;
}

void CNetGame::ResumeRecordingPlayback()
{
	field_1FE = 9;
	field_1DE = 1;
	if(field_1FA == -1)
		field_1EA += GetTickCount() - field_1F6 + 100;
	else
		field_1FA = -1;
}
