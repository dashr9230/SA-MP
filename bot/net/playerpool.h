
#pragma once

class CPlayerPool // size: 41035
{
private:
	BOOL			m_bPlayerSlotState[MAX_PLAYERS];
	PLAYERID		m_LocalPlayerID;
	CHAR			m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	CHAR			m_szPlayerNames[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int field_7163;
	int field_7167[1000];
	int field_8107;
	int field_810B[1000];
	char gap90AB[4000];

public:
	// Process All CPlayers
	BOOL Process();

	void SetLocalPlayerName(PCHAR szName) { strcpy(m_szLocalPlayerName,szName); };
	PCHAR GetPlayerName(BYTE bytePlayerID) { return m_szPlayerNames[bytePlayerID]; };

	CPlayerPool();
	~CPlayerPool();

	BOOL New(PLAYERID playerId, PCHAR szPlayerName);
	BOOL Delete(PLAYERID playerId, BYTE byteReason);

	// Find out if the slot is inuse.
	BOOL GetSlotState(PLAYERID playerId) {
		if(playerId >= MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[playerId];
	};

	void SetLocalPlayerID(PLAYERID MyPlayerID) {
		strcpy(m_szPlayerNames[MyPlayerID],m_szLocalPlayerName);
		m_LocalPlayerID = MyPlayerID;
	};
};