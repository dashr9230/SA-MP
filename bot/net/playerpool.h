
#pragma once

class CPlayerPool // size: 41035
{
private:
	BOOL			m_bPlayerSlotState[MAX_PLAYERS];
	char _gapFA0[2];
	CHAR			m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	char gapFBB[25000];
	int field_7163;
	int field_7167[1000];
	int field_8107;
	int field_810B[1000];
	char gap90AB[4000];

public:
	// Process All CPlayers
	BOOL Process();

	void SetLocalPlayerName(PCHAR szName) { strcpy(m_szLocalPlayerName,szName); };

	CPlayerPool();
	~CPlayerPool();

	BOOL Delete(PLAYERID playerId, BYTE byteReason);

};