
#pragma once

#define GAMESTATE_NONE			0 // used for debugging modes and such
#define GAMESTATE_CONNECTING	1
#define GAMESTATE_CONNECTED		2
#define GAMESTATE_AWAIT_JOIN	3

#define GAMESTATE_RESTARTING	5

#define PLAYER_RECORDING_TYPE_NONE		0
#define PLAYER_RECORDING_TYPE_DRIVER	1
#define PLAYER_RECORDING_TYPE_ONFOOT	2

//----------------------------------------------------

class CNetGame // size: 910 bytes
{
private:
	RakClientInterface	*m_pRakClient;
	CPlayerPool			*m_pPlayerPool;
	CVehiclePool		*m_pVehiclePool;
	int					m_iGameState;
	BOOL				m_bLanMode;

	void UpdateNetwork();

	// Packet handlers
	void Packet_AimSync(Packet *p);
	void Packet_PlayerSync(Packet *p);
	void Packet_VehicleSync(Packet *p);
	void Packet_PassengerSync(Packet *p);
	void Packet_ConnectionSucceeded(Packet *p);
	void Packet_RSAPublicKeyMismatch(Packet* packet);
	void Packet_ConnectionBanned(Packet* packet);
	void Packet_ConnectionRequestAccepted(Packet* packet);
	void Packet_NoFreeIncomingConnections(Packet* packet);
	void Packet_DisconnectionNotification(Packet* packet);
	void Packet_ConnectionLost(Packet* packet);
	void Packet_InvalidPassword(Packet* packet);
	void Packet_ModifiedPacket(Packet* packet);
	void Packet_ConnectAttemptFailed(Packet* packet);
	void Packet_TrailerSync(Packet *p);
	void Packet_Unk12(Packet *p);

public:

	int			m_iSpawnsAvailable;
	bool		m_bShowPlayerMarkers;
	int			m_iPlayerMarkersMode;
	bool		m_bTirePopping;
	BYTE		m_byteWorldTime;
	BYTE		m_byteWorldMinute;
	BYTE		m_byteWeather;
	char _gap21[16];
	BYTE		m_byteFriendlyFire;
	bool		m_bAllowWeapons;
	bool		m_bNameTagLOS;
	float		m_fGravity;
	int			m_iDeathDropMoney;
	BYTE		m_byteHoldTime;
	bool		m_bInstagib;
	bool		m_bZoneNames;
	bool		m_bLimitGlobalChatRadius;
	bool		m_bUseCJWalk;
	float		m_fGlobalChatRadius;
	float		m_fNameTagDrawDistance;
	bool		m_bDisableEnterExits;
	DWORD		m_dwMapIcon[100];
	int field_1DA;
	int field_1DE;
	FILE *field_1E2;
	int field_1E6;
	int field_1EA;
	char _gap1EE[4];
	int field_1F2;
	int field_1F6;
	int field_1FA;
	int field_1FE;

	char m_szHostName[256];
	char m_szHostOrIp[128];
	int m_iPort;

	CGameMode* m_pGameMode;
	CScriptTimers* m_pScriptTimers;

public:

	CNetGame();
	~CNetGame();

	int GetGameState() { return m_iGameState; };
	void SetGameState(int iGameState) { m_iGameState = iGameState; };
	void SetLanMode(BOOL bMode) { m_bLanMode = bMode; };

	CPlayerPool * GetPlayerPool() { return m_pPlayerPool; };
	CVehiclePool * GetVehiclePool() { return m_pVehiclePool; };
	RakClientInterface * GetRakClient() { return m_pRakClient; };
	CScriptTimers * GetTimers() { return m_pScriptTimers; };
	CGameMode * GetBotMode() { return m_pGameMode; };

	void Init(PCHAR szHostOrIp,int iPort,PCHAR szPlayerName,PCHAR szPass,PCHAR szNpcMode);
	void Process();
	void ResetVehiclePool();
	void ResetPlayerPool();
	void ShutdownForGameModeRestart();

	void SendChat(char *szText);
	void SendCommand(char *szCommand);

	void SetPlayerAdded(PLAYERID playerId, BOOL a2);
	void SetVehicleAdded(VEHICLEID VehicleID, BOOL a2);
	void SetPlayerState(PLAYERID playerId, BYTE byteState);
	BYTE GetPlayerState(PLAYERID playerId);
	BOOL GetPlayerPos(PLAYERID playerId, PVECTOR Vector);
	VEHICLEID GetPlayerVehicleID(PLAYERID playerId);
	BYTE GetPlayerArmedWeapon(PLAYERID playerId);
	BYTE GetPlayerHealth(PLAYERID playerId);
	BYTE GetPlayerArmour(PLAYERID playerId);
	BOOL GetPlayerKeys(PLAYERID playerId, WORD *udAnalog, WORD *lrAnalog, WORD *wKeys);
	float GetPlayerFacingAngle(PLAYERID playerId);
	BYTE GetPlayerSpecialAction(PLAYERID playerId);
	BOOL IsPlayerAdded(PLAYERID playerId);
	BOOL IsVehicleAdded(VEHICLEID VehicleID);
	float GetDistanceFromMeToPoint(PVECTOR vecPos);
	PVECTOR GetMyPos(PVECTOR Vector);
	void SetMyPos(PVECTOR Vector);
	float GetMyZAngle();
	void SetMyZAngle(float fAngle);	

	void FUNC_5(FILE *a1, DWORD a2);
	void FUNC_6(FILE *a1, DWORD a2);

	void StartRecordingPlayback(int iPlaybackType, char *szRecordName);
	void StopRecordingPlayback();
	void PauseRecordingPlayback();
	void ResumeRecordingPlayback();
};

//----------------------------------------------------
