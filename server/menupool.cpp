
#include "main.h"

CMenuPool::CMenuPool()
{
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		field_200[byteMenuID] = 0;
		field_0[byteMenuID] = 0;
	}
	for (PLAYERID Player = 0; Player < MAX_PLAYERS; Player++)
	{
		field_400[Player] = 255;
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

