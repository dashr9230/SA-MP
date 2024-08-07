
#pragma once

//----------------------------------------------------------

class CLocalPlayer
{
public:

	char field_0[60];
	char field_3C[3];
	char field_3F[31];
	char field_5E[52];
	short field_92;
	char field_94[68];
	char field_D8[24];
	int field_F0;
	int field_F4;

	char _gapF8[2];

	short field_FA;
	int field_FC;
	int field_100;
	CPlayerPed				*m_pPlayerPed;
	BOOL					m_bIsSpectating;
	char field_10C;
	short field_10D;
	DWORD field_10F;
	DWORD field_113;
	DWORD field_117;
	DWORD field_11B;
	short field_11F;
	short field_121;
	short field_123;

	char _gap125[2];

	DWORD field_127;
	char field_12B[12];
	DWORD field_137;
	DWORD field_13B;
	DWORD field_13F;

	char _gap143[4];

	DWORD field_147;

	char _gap14B[50];

	int field_17D;
	DWORD field_181;
	short field_185;
	short field_187;
	char field_189;
	char field_18A[13];
	int field_197[13];

	char _gap1CB[4];

	char field_1CF;
	BOOL					m_bInRCMode;

	char _gap1D4[258];

	DWORD field_2D6;
	int field_2DA;
	int field_2DE;
	int field_2E2;

	char _gap2E6[16];

	int field_2F6;
	int field_2FA;
	int field_2FE;
	int field_302;
	int field_306;
	int field_30A;

	char _gap30E;

	char field_30F;
	int field_310;

	char _gap314[4];

	short field_318;

	char _gap31A[10];

	CLocalPlayer();

	void ResetAllSyncAttributes();

	void Say(PCHAR szText);

	BOOL IsSpectating() { return m_bIsSpectating; };

	BOOL IsInRCMode() { return m_bInRCMode; };
};

//----------------------------------------------------------
