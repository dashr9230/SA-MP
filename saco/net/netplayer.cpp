
#include "../main.h"

//----------------------------------------------------

CNetPlayer::CNetPlayer(PCHAR szPlayerName, BOOL bIsNPC)
{
	m_bIsNPC = bIsNPC;
	field_C = 0;
	field_4 = 0;
	m_PlayerName = szPlayerName;
	m_pRemotePlayer = new CRemotePlayer();
}

//----------------------------------------------------

CNetPlayer::~CNetPlayer()
{
	SAFE_DELETE(m_pRemotePlayer);
}

//----------------------------------------------------
