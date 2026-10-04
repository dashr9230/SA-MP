
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

	void SetPlayerName(PCHAR szName) { m_PlayerName = szName; };
	int GetScore() { return field_4; };
	int GetPing() { return field_C; };

	PCHAR GetName() { return (PCHAR)m_PlayerName.c_str(); }; // .text:100175B0

	CNetPlayer(PCHAR szPlayerName, BOOL bIsNPC);
	~CNetPlayer();
};

//----------------------------------------------------
