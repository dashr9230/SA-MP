
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

