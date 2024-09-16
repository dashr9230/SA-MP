
#include "main.h"

extern CNetGame *pNetGame;

//----------------------------------------------------

void CPlayer::Say(unsigned char * szText, BYTE byteTextLen)
{
	// TODO: CPlayer::Say
}

//----------------------------------------------------

void CPlayer::SetSpawnInfo(PLAYER_SPAWN_INFO *pSpawn)
{
	memcpy(&m_SpawnInfo,pSpawn,sizeof(PLAYER_SPAWN_INFO));
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------






//----------------------------------------------------

void CPlayer::SetCheckpoint(float fX, float fY, float fZ, float fSize)
{
	m_vecCheckpoint.X = fX;
	m_vecCheckpoint.Y = fY;
	m_vecCheckpoint.Z = fZ;
	m_fCheckpointSize = fSize;
	ToggleCheckpoint(TRUE);
}

//----------------------------------------------------

void CPlayer::ToggleCheckpoint(BOOL bEnabled)
{
	m_bCheckpointEnabled = bEnabled;
	m_bInCheckpoint = FALSE;

	RakNet::BitStream bsParams;

	if (bEnabled)
	{
		bsParams.Write(m_vecCheckpoint.X);
		bsParams.Write(m_vecCheckpoint.Y);
		bsParams.Write(m_vecCheckpoint.Z);
		bsParams.Write(m_fCheckpointSize);
		pNetGame->SendToPlayer(RPC_SetCheckpoint, &bsParams, m_PlayerID, 2);
	} else {
		pNetGame->SendToPlayer(RPC_DisableCheckpoint, &bsParams, m_PlayerID, 2);
	}
}

//----------------------------------------------------

void CPlayer::ToggleRaceCheckpoint(BOOL bEnabled)
{
	m_bRaceCheckpointEnabled = bEnabled;
	m_bInRaceCheckpoint = FALSE;

	RakNet::BitStream bsParams;

	if (bEnabled)
	{
		bsParams.Write(m_byteRaceCheckpointType);
		bsParams.Write(m_vecRaceCheckpoint.X);
		bsParams.Write(m_vecRaceCheckpoint.Y);
		bsParams.Write(m_vecRaceCheckpoint.Z);
		bsParams.Write(m_vecRaceNextCheckpoint.X);
		bsParams.Write(m_vecRaceNextCheckpoint.Y);
		bsParams.Write(m_vecRaceNextCheckpoint.Z);
		bsParams.Write(m_fRaceCheckpointSize);
		pNetGame->SendToPlayer(RPC_SetRaceCheckpoint, &bsParams, m_PlayerID, 2);
	} else {
		pNetGame->SendToPlayer(RPC_DisableRaceCheckpoint, &bsParams, m_PlayerID, 2);
	}
}

//----------------------------------------------------
