
#ifndef SAMPSRV_NETGAME_H
#define SAMPSRV_NETGAME_H

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
	CScriptHttps* m_pScriptHttps;
	CScriptTimers* m_pScriptTimers;
	RakServerInterface			*m_pRak;
	int m_iLastServerTickUpdate;
	int m_iServerTickCount;
	int m_iServerTickRate;
	BOOL m_bLanMode;
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
	int m_iSpawnsAvailable;
	PLAYER_SPAWN_INFO	m_AvailableSpawns[319];

public:
	CNetGame();
	~CNetGame();

	void Init(BOOL bFirst);
	void ShutdownForGameModeRestart();

	RakServerInterface * GetRakServer() { return m_pRak; };

	void LoadAllFilterscripts();

	void Process();

	void LoadBanList();

	DWORD GetTime();
};

#endif