
#include "main.h"

CPlayerPool::CPlayerPool()
{
	for(PLAYERID PlayerID = 0; PlayerID < MAX_PLAYERS; PlayerID++) {
		m_bPlayerSlotState[PlayerID] = FALSE;
		field_2599C[PlayerID] = 0;
		field_23A5C[PlayerID] = 0;
		field_3E8C[PlayerID] = pNetGame->GetTime();
	}
	field_30964 = 0;
	field_30968 = 0;
	field_FA0 = pNetGame->GetTime();
	field_FA4 = 0;
	field_FA8 = 0;
	field_3096C = 0;
}
