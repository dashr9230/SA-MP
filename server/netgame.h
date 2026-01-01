
#ifndef SAMPSRV_NETGAME_H
#define SAMPSRV_NETGAME_H

#define MAX_SPAWNS 319

#define GAMESTATE_STOPPED	 0
#define GAMESTATE_RUNNING	 1
#define GAMESTATE_RESTARTING 2

#define INVALID_ID			0xFFFF

class CNetGame // size: W: 14808 L: 14816
{
private:

	CGameMode					*m_pGameMode;
	CFilterScripts				*m_pFilterScripts;
	CPlayerPool					*m_pPlayerPool;
	CVehiclePool				*m_pVehiclePool;
	CPickupPool					*m_pPickupPool;
	CObjectPool					*m_pObjectPool;
	CMenuPool					*m_pMenuPool;
	CTextDrawPool				*m_pTextPool;
	CLabelPool					*m_pLabelPool;
	CGangZonePool				*m_pGangZonePool;
	CActorPool					*m_pActorPool;
	
	int							m_iCurrentGameModeIndex;
	int							m_iCurrentGameModeRepeat;
	BOOL						m_bFirstGameModeLoaded;
	
	CScriptHttps				*m_pScriptHttps;
	CScriptTimers				*m_pScriptTimers;
	
	RakServerInterface			*m_pRak;

	int							m_iLastServerTickUpdate;
	int							m_iServerTickCount;
	int							m_iServerTickRate;

	BOOL m_bLanMode;

	void UpdateNetwork();

public:

	int m_iShowPlayerMarkers;
	bool m_bShowNameTags;
	BYTE m_byteWorldTime;
	bool m_bAllowWeapons; // Allow weapons in interiors
	bool m_bStuntBonus; // Insane stunt bonusses enabled?
	char field_5C;
	BYTE m_byteWeather;
	int	 m_iGameState;
	float m_fGravity;
	int  m_iDeathDropMoney;
	char field_6A;
	char field_6B;
	bool m_bLimitGlobalChatRadius; // limit global player chat to other players within a certain radius
	bool m_bUseCJWalk;
	float m_fGlobalChatRadius; // limit global chat radius
	float m_fNameTagDrawDistance; // The distance which players will start rendering nametags
	bool m_bDisableEnterExits; // Interior enter/exits disabled?
	bool m_bNameTagLOS;
	bool m_bManualVehicleEngineAndLights;
	bool m_bLimitPlayerMarkerRadius;
	float m_fPlayerMarkerRadius;
	BOOL m_bVehicleFriendlyFire;

#ifndef WIN32
	double m_dElapsedTime;
#endif

	CScriptTimers* GetTimers() { return m_pScriptTimers; };

	CNetGame();
	~CNetGame();

	void Init(BOOL bFirst);
	void ShutdownForGameModeRestart();
	BOOL SetNextScriptFile(char *szFile);


	CPlayerPool * GetPlayerPool() { return m_pPlayerPool; };
	CVehiclePool * GetVehiclePool() { return m_pVehiclePool; };
	RakServerInterface * GetRakServer() { return m_pRak; };
	CGameMode * GetGameMode() { return m_pGameMode; };
	CFilterScripts * GetFilterScripts() { return m_pFilterScripts; };
	CGangZonePool * GetGangZonePool() { return m_pGangZonePool; };
	CActorPool * GetActorPool() { return m_pActorPool; };

	void MasterServerAnnounce(float fElapsedTime);
	char *GetNextScriptFile();
	void LoadAllFilterscripts();

	void Process();

	void BroadcastData( char *szUniqueID,
						RakNet::BitStream *bitStream,
						PLAYERID excludedPlayer,
						char orderingStream );

	void SendToPlayer( char *szUniqueID,
					   RakNet::BitStream *bitStream,
					   PLAYERID playerId,
					   char orderingChannel );

	void LoadBanList();

	// CLASS SYSTEM
	int					m_iSpawnsAvailable;
	PLAYER_SPAWN_INFO	m_AvailableSpawns[MAX_SPAWNS];
	int AddSpawn(PLAYER_SPAWN_INFO *pSpawnInfo);

	const PCHAR GetWeaponName(int iWeaponID);

	DWORD GetTime();
};

//----------------------------------------------------

#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA					43
#define WEAPON_NIGHTVISION				44
#define WEAPON_INFRARED					45
#define WEAPON_PARACHUTE				46
#define WEAPON_VEHICLE					49
#define WEAPON_DROWN					53
#define WEAPON_COLLISION				54

//----------------------------------------------------

#endif