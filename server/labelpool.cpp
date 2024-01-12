
#include "main.h"

CLabelPool::CLabelPool()
{
	for(WORD wLabel = 0; wLabel < MAX_LABELS; wLabel++)
	{
		memset(&field_1000[wLabel], 0, sizeof(struc_24));
		field_0[wLabel] = 0;
	}
}

CLabelPool::~CLabelPool()
{
	for (WORD wLabel = 0; wLabel < MAX_LABELS; wLabel++)
	{
		if(field_0[wLabel])
		{
			Delete(wLabel);
		}
	}
}

BOOL CLabelPool::Delete(WORD wLabel)
{
	// TODO: CLabelPool::Delete W .text:00464210 L .text:0804C150
	return TRUE;
}
