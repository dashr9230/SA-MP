
#pragma once

#include "entity.h"

struct struc_97
{
	char _gap0[52];
};

//-----------------------------------------------------------

class CPlayerPed : public CEntity
{
public:
	CPlayerPed();

	int field_48;
	int field_4C[10];
	struc_97 field_74[10];
	int field_27C[10];
	PED_TYPE    *m_pPed;
	int field_2A8;
	int field_2AC;
	BYTE		m_bytePlayerNumber;

	char _gap2B1[8];

	int field_2B9;

	char _gap2BD[4];

	int field_2C1;
	int field_2C5;
	int field_2C9;
	int field_2CD;

	char _gap2D1;

	int field_2D2;

	char _gap2D6[8];

	int field_2DE;
	int field_2E2;
	int field_2E6;

	char _gap2EA[8];

	int field_2F2;
	char field_2F6;
	char field_2F7;
	char field_2F8;
	int field_2F9;
};

//-----------------------------------------------------------
