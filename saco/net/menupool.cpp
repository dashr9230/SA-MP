
#include "../main.h"

//----------------------------------------------------

CMenuPool::CMenuPool()
{
	// loop through and initialize all net players to null and slot states to false
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		field_200[byteMenuID] = 0;
		field_0[byteMenuID] = 0;
	}
	field_400 = -128;
}

//----------------------------------------------------
