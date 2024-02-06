
#include "../main.h"

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	// loop through and initialize all net players to null and slot states to false
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		field_1F8A[playerId] = 0;
		field_2A[playerId] = 0;
	}
	//field_26 = new sub_10004C50(); // TODO: sub_10004C50()
	field_0 = 0;
	field_22 = 0;
	field_4 = 0;
	field_2F3A = 0;
}

//----------------------------------------------------
