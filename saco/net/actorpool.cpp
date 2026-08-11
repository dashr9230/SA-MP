#include "../main.h"
#include "actorpool.h"

CActorPool::CActorPool()
{
    field_0 = 0;

    for (int i = 0; i < MAX_ACTORS; ++i)
    {
        field_4[i] = -1;
        field_FA4[i] = 0;
        field_1F44[i] = 0;
    }

    memset(_gap2EE4, 0, sizeof(_gap2EE4));
}
