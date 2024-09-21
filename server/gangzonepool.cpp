
#include "main.h"

#define RGBA_ABGR(n) (((n >> 24) & 0x000000FF) | ((n >> 8) & 0x0000FF00) | ((n << 8) & 0x00FF0000) | ((n << 24) & 0xFF000000))

//----------------------------------------------------

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

void CGangZonePool::Delete(WORD wZone)
{
	m_bSlotState[wZone] = FALSE;
	RakNet::BitStream bsParams;
	bsParams.Write(wZone);
	pNetGame->BroadcastData(RPC_ScrRemoveGangZone, &bsParams, INVALID_PLAYER_ID, 2);
}

void CGangZonePool::ShowForPlayer(PLAYERID playerId, WORD wZone, DWORD dwColor)
{
	RakNet::BitStream bsParams;
	bsParams.Write(wZone);
	bsParams.Write(m_fGangZone[wZone][0]);
	bsParams.Write(m_fGangZone[wZone][1]);
	bsParams.Write(m_fGangZone[wZone][2]);
	bsParams.Write(m_fGangZone[wZone][3]);
	dwColor = RGBA_ABGR(dwColor);
	bsParams.Write(dwColor);
	pNetGame->SendToPlayer(RPC_ScrAddGangZone, &bsParams, playerId, 2);
}

void CGangZonePool::ShowForAll(WORD wZone, DWORD dwColor)
{
	RakNet::BitStream bsParams;
	bsParams.Write(wZone);
	bsParams.Write(m_fGangZone[wZone][0]);
	bsParams.Write(m_fGangZone[wZone][1]);
	bsParams.Write(m_fGangZone[wZone][2]);
	bsParams.Write(m_fGangZone[wZone][3]);
	dwColor = RGBA_ABGR(dwColor);
	bsParams.Write(dwColor);
	pNetGame->BroadcastData(RPC_ScrAddGangZone, &bsParams, INVALID_PLAYER_ID, 2);
}

void CGangZonePool::HideForPlayer(PLAYERID playerId, WORD wZone)
{
	RakNet::BitStream bsParams;
	bsParams.Write(wZone);
	pNetGame->SendToPlayer(RPC_ScrRemoveGangZone, &bsParams, playerId, 2);
}

void CGangZonePool::HideForAll(WORD wZone)
{
	RakNet::BitStream bsParams;
	bsParams.Write(wZone);
	pNetGame->BroadcastData(RPC_ScrRemoveGangZone, &bsParams, INVALID_PLAYER_ID, 2);
}

