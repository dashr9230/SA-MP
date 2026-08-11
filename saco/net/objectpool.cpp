#include "../main.h"
#include "objectpool.h"

CObjectPool::CObjectPool()
{
    field_0 = 0;

    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        field_4[i] = -1;
        field_FA4[i] = 0;
    }
}
