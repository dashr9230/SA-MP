
#include "../main.h"

extern CGame* pGame;

//----------------------------------------------------

CGangZonePool::CGangZonePool()
{
	for (WORD wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		m_pGangZone[wZone] = NULL;
		m_bSlotState[wZone] = FALSE;
	}
}

CGangZonePool::~CGangZonePool()
{
	for (WORD wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		if (m_pGangZone[wZone])
		{
			delete m_pGangZone[wZone];
			m_pGangZone[wZone] = NULL;
		}
	}
}

void CGangZonePool::New(WORD wZone, float fMinX, float fMinY, float fMaxX, float fMaxY, DWORD dwColor)
{
	if (m_pGangZone[wZone])
	{
		delete m_pGangZone[wZone];
		m_pGangZone[wZone] = NULL;
		m_bSlotState[wZone] = FALSE;
	}
	GANG_ZONE* pGangZone = (GANG_ZONE*)malloc(sizeof (GANG_ZONE));
	if (pGangZone)
	{
		// Odd order but it is right
		pGangZone->fPos[0] = fMinX;
		pGangZone->fPos[1] = fMaxY;
		pGangZone->fPos[2] = fMaxX;
		pGangZone->fPos[3] = fMinY;
		pGangZone->dwColor = dwColor;
		pGangZone->dwAltColor = dwColor;
		m_pGangZone[wZone] = pGangZone;
		m_bSlotState[wZone] = TRUE;
	}
}

void CGangZonePool::Flash(WORD wZone, DWORD dwColor)
{
	if (m_pGangZone[wZone])
	{
		m_pGangZone[wZone]->dwAltColor = dwColor;
	}
}

void CGangZonePool::StopFlash(WORD wZone)
{
	if (m_pGangZone[wZone])
	{
		m_pGangZone[wZone]->dwAltColor = m_pGangZone[wZone]->dwColor;
	}
}

void CGangZonePool::Delete(WORD wZone)
{
	if (m_pGangZone[wZone])
	{
		delete m_pGangZone[wZone];
		m_pGangZone[wZone] = NULL;
		m_bSlotState[wZone] = FALSE;
	}
}

void CGangZonePool::Draw()
{
	static int iLastTick = 0;
	int iTick = GetTickCount();
	static BYTE alt = 0;
	if (iTick - iLastTick >= 500)
	{
		// All zones flash, it's easier to have non flashers flash the same color
		alt = ~alt;
		iLastTick = iTick;
	}
	for (WORD wZone = 0; wZone < MAX_GANG_ZONES; wZone++)
	{
		if (m_bSlotState[wZone])
		{
			pGame->DrawGangZone(m_pGangZone[wZone]->fPos, alt ? m_pGangZone[wZone]->dwAltColor : m_pGangZone[wZone]->dwColor);
		}
	}
}
