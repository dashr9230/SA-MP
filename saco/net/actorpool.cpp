
#include "../main.h"

//----------------------------------------------------

CActorPool::CActorPool()
{
	for(ACTORID ActorID = 0; ActorID < MAX_ACTORS; ActorID++) {
		field_FA4[ActorID] = 0;
		field_4[ActorID] = 0;
		field_1F44[ActorID] = 0;
	}
	field_0 = 0;
}

//----------------------------------------------------
