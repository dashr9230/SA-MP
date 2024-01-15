
#include "../main.h"

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	field_7163 = 0;
	field_8107 = 0;

	// loop through and initialize all net players to null and slot states to false
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		field_0[playerId] = 0;
		field_7167[playerId] = 0;
		field_810B[playerId] = 0;
	}
}

//----------------------------------------------------
