
#pragma once

#include <string>

//----------------------------------------------------

class CNetPlayer
{
public:
	//char			_gap0[48];
	char			_gap0[4];
	int				field_4;
	BOOL			m_bIsNPC;
	int				field_C;
	CRemotePlayer	*m_pRemotePlayer;
	std::string		m_PlayerName;

	CNetPlayer(PCHAR szPlayerName, BOOL bIsNPC);
};

//----------------------------------------------------
