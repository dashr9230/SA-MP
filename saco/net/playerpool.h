
#pragma once

#include <string>

#define INVALID_PLAYER_ID 0xFFFF
#define NO_TEAM 255

//----------------------------------------------------

class CPlayerPool
{
public:

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
		pPlayer->SetPlayerName(szName);
	}

	PCHAR GetPlayerName(PLAYERID playerId) { // .text:100175C0
		if(playerId == m_LocalPlayerID) return (PCHAR)field_6.c_str();
		CNetPlayer *pPlayer = playerId > MAX_PLAYERS ? NULL : m_pPlayers[playerId];
		if(!pPlayer) return NULL;
		return pPlayer->GetName();
	};

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };

	void FUNC_10013960(); // .text:10013960

	BOOL sub_10014090(PLAYERID playerId, BYTE byteReason); // .text:10014090

	BOOL sub_10014250(PLAYERID playerId, PCHAR szPlayerName, BOOL bIsNPC); // .text:10014250

	PLAYERID FUNC_100138C0(DWORD dwPedPtr); // .text:100138C0

	CRemotePlayer* GetAt(PLAYERID playerId) {
		if(playerId > MAX_PLAYERS) { return NULL; }
		CNetPlayer *pNetPlayer = m_pPlayers[playerId];
		if(pNetPlayer) return pNetPlayer->m_pRemotePlayer;
		return NULL;
	};

	PLAYERID GetLocalPlayerID() { return m_LocalPlayerID; };

	int GetSlotState(PLAYERID playerId) { if(playerId >= MAX_PLAYERS) return 0; return field_2A[playerId]; }

	PCHAR FUNC_100175C0(int iPlayerId); // .text:100175C0

	BOOL IsPlayerNPC(PLAYERID playerId) {
		if(playerId > MAX_PLAYERS) return FALSE;
		CNetPlayer *pPlayer = m_pPlayers[playerId];
		if(!pPlayer) return FALSE;
		return pPlayer->m_bIsNPC;
	}
	int GetPlayerScore(PLAYERID playerId) {
		if(playerId > MAX_PLAYERS) return 0;
		CNetPlayer *pPlayer = m_pPlayers[playerId];
		if(!pPlayer) return 0;
		return pPlayer->GetScore();
	}
	int GetPlayerPing(PLAYERID playerId) {
		if(playerId > MAX_PLAYERS) return 0;
		CNetPlayer *pPlayer = m_pPlayers[playerId];
		if(!pPlayer) return 0;
		return pPlayer->GetPing();
	}
	int GetPlayerCount(BOOL bIncludeNPCs); // .text:100139F0, todo: implement

	class CObject *FUNC_10013B70(ENTITY_TYPE *pEntity); // todo: implement

	void sub_10013C90(); // todo: implement
	void sub_10013D10(); // todo: implement

	CPlayerPool();
	~CPlayerPool(); // .text:10014120
};

//----------------------------------------------------
