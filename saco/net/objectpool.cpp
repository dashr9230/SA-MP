
#include "../main.h"

CObjectPool::CObjectPool()
{
	for(WORD wObjectID = 0; wObjectID < MAX_OBJECTS; wObjectID++)
	{
		field_4[wObjectID] = 0;
		field_FA4[wObjectID] = 0;
	}
	field_0 = 0;
};
