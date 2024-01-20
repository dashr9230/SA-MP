
#include "main.h"

CMenuPool::CMenuPool()
{
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		field_200[byteMenuID] = 0;
		field_0[byteMenuID] = 0;
	}
	for (PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		field_400[playerId] = 255;
	}
}

CMenuPool::~CMenuPool()
{
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		if (field_0[byteMenuID])
		{
			delete field_0[byteMenuID];
			field_0[byteMenuID] = NULL;
		}
	}
}

