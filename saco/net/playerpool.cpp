
#include "../main.h"

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	// loop through and initialize all net players to null and slot states to false
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		m_pPlayers[playerId] = NULL;
		field_2A[playerId] = 0;
	}
	m_pLocalPlayer = new CLocalPlayer();
	field_0 = 0;
	field_22 = 0;
	m_LocalPlayerID = 0;
	field_2F3A = 0;
}

//----------------------------------------------------
