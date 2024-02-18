
#include "../main.h"

//----------------------------------------------------

CGangZonePool::CGangZonePool()
{
	for (WORD i = 0; i < 1024; i++)
	{
		field_0[i] = 0;
		field_1000[i] = 0;
	}
}

