
#include "main.h"

CGangZonePool::CGangZonePool()
{
	for (WORD wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		m_bSlotState[wZone] = FALSE;
	}
}

WORD CGangZonePool::New(float fMinX, float fMinY, float fMaxX, float fMaxY)
{
	WORD wZone = 0;
	while (wZone < MAX_GANG_ZONES)
	{
		if (!m_bSlotState[wZone]) break;
		wZone++;
	}
	if (wZone == MAX_GANG_ZONES) return 0xFFFF;
	m_fGangZone[wZone][0] = fMinX;
	m_fGangZone[wZone][1] = fMinY;
	m_fGangZone[wZone][2] = fMaxX;
	m_fGangZone[wZone][3] = fMaxY;
	m_bSlotState[wZone] = TRUE;
	return wZone;
}
