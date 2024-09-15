
#include "main.h"

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
