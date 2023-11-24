
#include "main.h"

CGangZonePool::CGangZonePool()
{
	for (WORD wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		field_4000[wZone] = 0;
	}
}



