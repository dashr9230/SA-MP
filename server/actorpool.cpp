
#include "main.h"

CActorPool::CActorPool()
{
	for(ACTORID ActorID = 0; ActorID != MAX_ACTORS; ActorID++)
	{
		field_0[ActorID] = 0;
		field_FA0[ActorID] = 0;
		field_1F40[ActorID] = 0;
	}
	field_2EE0 = 0;
}

CActorPool::~CActorPool()
{
	for(ACTORID ActorID = 0; ActorID != MAX_ACTORS; ActorID++) {
		Delete(ActorID);
	}
}

BOOL CActorPool::Delete(ACTORID ActorID)
{
	// TODO: CActorPool::Delete W .text:00464800 L .text:08094B80
	return FALSE;
}

