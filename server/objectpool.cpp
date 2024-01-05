
#include "main.h"

CObjectPool::CObjectPool()
{
	for(WORD wObjectID = 0; wObjectID != MAX_OBJECTS; wObjectID++) {
		field_7A21A0[wObjectID] = 0;
		field_7A3140[wObjectID] = 0;
		field_3D0900[wObjectID] = 0;
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			field_0[i][wObjectID] = 0;
			field_3D18A0[i][wObjectID] = 0;
		}
	}
}

CObjectPool::~CObjectPool()
{
	for(WORD wObjectID = 0; wObjectID != MAX_OBJECTS; wObjectID++) {
		if(!Delete(wObjectID) && field_3D0900[wObjectID])
		{
			// Try delete it for individuals
			for (PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
			{
				DeleteForPlayer(playerId, wObjectID);
			}
		}
	}
}

BOOL CObjectPool::Delete(WORD wObjectID)
{
	// TODO: CObjectPool::Delete W .text:004657A0 L .text:080C89A0
	return FALSE;
}

BOOL CObjectPool::DeleteForPlayer(PLAYERID playerId, WORD wObjectID)
{
	// TODO: CObjectPool::DeleteForPlayer W .text:004656B0 L .text:080C88E0
	return FALSE;
}
