
#include "../main.h"
#include "../../raknet/SocketDataEncryptor.h"

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
typedef struct _PASSENGER_SYNC_DATA // size: 24
{
	char _gap0[24];
} PASSENGER_SYNC_DATA;

#pragma pack(1)
typedef struct _TRAILER_SYNC_DATA // size: 54
{
	char _gap0[54];
} TRAILER_SYNC_DATA;

char unnamed_2[63];
PASSENGER_SYNC_DATA unnamed_5[MAX_PLAYERS];
BOOL unnamed_6[MAX_PLAYERS];
char unnamed_9;
char unnamed_1[68];
char unnamed_3[1000][68];
BYTE unnamed_8[MAX_PLAYERS];
BOOL unnamed_7[MAX_VEHICLES];
char unnamed_4[1000][63];

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

	memset(unnamed_1,0,sizeof(unnamed_1));
	memset(unnamed_2,0,sizeof(unnamed_2));
	memset(unnamed_3,0,sizeof(unnamed_3));
	memset(unnamed_4,0,sizeof(unnamed_4));
	memset(unnamed_5,0,sizeof(unnamed_5));
	memset(unnamed_6,0,sizeof(unnamed_6));
	memset(unnamed_7,0,sizeof(unnamed_7));
	memset(unnamed_8,0,sizeof(unnamed_8));

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
		exit(1);
	
	m_pRakClient->SetPassword(szPass);
	m_pRakClient->Connect(szHostOrIp,iPort,0,0,10);

	m_iGameState = GAMESTATE_CONNECTING;

	/*
	char s[256];
	sprintf(s,"Bot(%s): connecting to %s:%d...",szPlayerName,szHostOrIp,iPort);
	OutputDebugString(s);*/

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

	memset(unnamed_1,0,sizeof(unnamed_1));
	memset(unnamed_2,0,sizeof(unnamed_2));
	memset(unnamed_3,0,sizeof(unnamed_3));
	memset(unnamed_4,0,sizeof(unnamed_4));
	memset(unnamed_5,0,sizeof(unnamed_5));
	memset(unnamed_6,0,sizeof(unnamed_6));
	memset(unnamed_7,0,sizeof(unnamed_7));
	memset(unnamed_8,0,sizeof(unnamed_8));
	field_1DE = 0;
	field_1E2 = 0;
	field_1F2 = GetTickCount();
	unnamed_9 = 0;
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
		case ID_CONNECTION_BANNED:
			Packet_ConnectionBanned(pkt);
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(pkt);
			break;

		case ID_MODIFIED_PACKET:
			Packet_ModifiedPacket(pkt);
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
		unnamed_8[playerId] = PLAYER_STATE_PASSENGER;
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

void CNetGame::Packet_ConnectionBanned(Packet* packet)
{
	//OutputDebugString("BOT: You're banned from the server.");
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_DisconnectionNotification(Packet* packet)
{
	//OutputDebugString("BOT: Disconnected.");
	m_pRakClient->Disconnect(0);
	exit(1);
}

//----------------------------------------------------

void CNetGame::Packet_ModifiedPacket(Packet* packet)
{
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

void CNetGame::sub_415EA0(PLAYERID playerId, BOOL a2)
{
	if(playerId < MAX_PLAYERS)
	{
		unnamed_6[playerId] = a2;
	}
}

void CNetGame::sub_415EC0(VEHICLEID VehicleID, BOOL a2)
{
	if(VehicleID < MAX_VEHICLES)
	{
		unnamed_7[VehicleID] = a2;
	}
}

void CNetGame::StopRecordingPlayback()
{
	field_1DE = 0;
	field_1DA = 0;
}



