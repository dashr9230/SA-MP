
#pragma once

#include <string>

//----------------------------------------------------

class CPlayerPool
{
private:

	int field_0;
	PLAYERID		m_LocalPlayerID;
	std::string	field_6;
	int field_22;
	CLocalPlayer	*m_pLocalPlayer;
	int field_2A[MAX_PLAYERS];

	char _gapFDA[4016];

	CNetPlayer		*m_pPlayers[MAX_PLAYERS];
	int field_2F3A;

public:

	void SetLocalPlayerName(PCHAR szName) { field_6 = szName; };
	PCHAR GetLocalPlayerName() { return (PCHAR)field_6.c_str(); };
	void SetPlayerName(PLAYERID playerId, PCHAR szName) {
		if(playerId > MAX_PLAYERS) return;
		CNetPlayer* pPlayer = m_pPlayers[playerId];
		if(!pPlayer) return;
		pPlayer->m_PlayerName = szName;
	}

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };

	CRemotePlayer* GetAt(PLAYERID playerId) {
		if(playerId > MAX_PLAYERS) { return NULL; }
		CNetPlayer *pNetPlayer = m_pPlayers[playerId];
		if(pNetPlayer) return pNetPlayer->m_pRemotePlayer;
		return NULL;
	};

	PLAYERID GetLocalPlayerID() { return m_LocalPlayerID; };

	CPlayerPool();
};

//----------------------------------------------------
