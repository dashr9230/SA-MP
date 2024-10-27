
#pragma once

//----------------------------------------------------

#pragma pack(1)
class CRemotePlayer
{
private:
	//char _gap0[509];
	int field_0;
	int field_4;
	int field_8;
	char field_C;

	char _gapD[12];

	char field_19[63];

	char _gap58[54];

	char field_8E[31];
	char field_AD[24];
	char field_C5[68];
	char field_109;
	char field_10A;
	char field_10B;
	int field_10C;
	int field_110;

	char _gap114[152];

	int field_1AC;
	int field_1B0;
	int field_1B4;
	char field_1B8;
	DWORD field_1B9;
	DWORD field_1BD;
	int field_1C1;
	int field_1C5;
	char field_1C9[12];
	DWORD field_1D5;
	DWORD field_1D9;
	int field_1DD;
	int field_1E1;
	PLAYERID			m_PlayerID;
	short field_1E7;
	int field_1E9;
	int field_1ED;
	int field_1F1;
	int field_1F5;
	int field_1F9;

public:

	CRemotePlayer();

	void ResetAllSyncAttributes();

	void SetPlayerColor(DWORD dwColor);
	DWORD GetPlayerColorAsRGBA();
	DWORD GetPlayerColorAsARGB();
};

//----------------------------------------------------
