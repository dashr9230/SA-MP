
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

//----------------------------------------------------

CObjectPool::~CObjectPool()
{
	for(int i = 0; i < MAX_OBJECTS; i++)
		FUNC_10012850(i);
}

//----------------------------------------------------

void CObjectPool::FUNC_10012850(WORD wObject)
{
	// TODO: CObjectPool::FUNC_10012850() .text:10012850
}
