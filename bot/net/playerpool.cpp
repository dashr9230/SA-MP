
#include "../main.h"

extern CNetGame *pNetGame;

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	field_7163 = 0;
	field_8107 = 0;

	// loop through and initialize all net players to null and slot states to false
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		m_bPlayerSlotState[playerId] = FALSE;
		field_7167[playerId] = 0;
		field_810B[playerId] = 0;
	}
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		Delete(playerId,0);
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(PLAYERID playerId, PCHAR szPlayerName)
{
	m_bPlayerSlotState[playerId] = TRUE;
	strcpy(m_szPlayerNames[playerId], szPlayerName);
	pNetGame->SetPlayerAdded(playerId, FALSE);
	return TRUE;
}

//----------------------------------------------------

BOOL CPlayerPool::Delete(PLAYERID playerId, BYTE byteReason)
{
	m_bPlayerSlotState[playerId] = FALSE;
	pNetGame->SetPlayerAdded(playerId, FALSE);
	return TRUE;
}

//----------------------------------------------------

BOOL CPlayerPool::Process()
{
	return TRUE;
}

//----------------------------------------------------
