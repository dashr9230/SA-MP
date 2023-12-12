
#include "main.h"

CPlayerPool::CPlayerPool()
{
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		m_bPlayerSlotState[playerId] = FALSE;
		field_2599C[playerId] = 0;
		field_23A5C[playerId] = 0;
		field_3E8C[playerId] = pNetGame->GetTime();
	}
	field_30964 = 0;
	field_30968 = 0;
	field_FA0 = pNetGame->GetTime();
	field_FA4 = 0;
	field_FA8 = 0;
	field_3096C = 0;
}
