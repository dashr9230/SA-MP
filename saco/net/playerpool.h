
#pragma once

#include <string>

//----------------------------------------------------

class CPlayerPool
{
private:
	int field_0;
	short field_4;
	std::string	field_6;
	int field_22;
	CLocalPlayer	*m_pLocalPlayer;
	int field_2A[MAX_PLAYERS];

	char _gapFDA[4016];

	int field_1F8A[MAX_PLAYERS];
	int field_2F3A;

public:
	CPlayerPool();
};

//----------------------------------------------------
