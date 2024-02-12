
#include "../main.h"

CLabelPool::CLabelPool()
{
	for(WORD wLabel = 0; wLabel < MAX_LABELS; wLabel++)
	{
		memset(&field_0[wLabel], 0, sizeof(struc_92));
		field_E800[wLabel] = 0;
	}
}
