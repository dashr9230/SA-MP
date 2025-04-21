
#pragma once

//----------------------------------------------------------

#pragma pack(1)
class CLocalPlayer
{
private:

	BYTE field_0[63];
	//BYTE field_3C[3];
	BYTE field_3F[31];
	BYTE field_5E[54];
	//WORD field_92;
	BYTE field_94[68];
	BYTE field_D8[24];
	DWORD field_F0;
	DWORD field_F4;

	BYTE _gapF8[2];

	WORD field_FA;
	DWORD field_FC;
	DWORD field_100;
	CPlayerPed				*m_pPlayerPed;
	BOOL					m_bIsSpectating;
	BYTE					m_byteTeam;
	WORD field_10D;
	DWORD field_10F;
	DWORD field_113;
	DWORD field_117;
	DWORD field_11B;
	WORD field_11F;
	WORD field_121;
	WORD field_123;

	BYTE _pad125[2];

	DWORD field_127;
	BYTE field_12B[12];
	DWORD field_137;
	DWORD field_13B;
	DWORD field_13F;

	BYTE _pad143[4];

	DWORD field_147;

	BYTE _pad14B[50];

	DWORD field_17D;
	DWORD field_181;
	WORD field_185;
	WORD field_187;
	BYTE field_189;
	BYTE					m_byteLastWeapon[13];
	DWORD					m_dwLastAmmo[13];

	BYTE _pad1CB[4];

	BYTE field_1CF;
	BOOL					m_bInRCMode;

	BYTE _pad1D4[258];

	DWORD field_2D6;
	DWORD field_2DA;
	DWORD field_2DE;
	DWORD field_2E2;

	BYTE _pad2E6[16];

	DWORD field_2F6;
	BOOL					m_bWantsAnotherClass;
	DWORD field_2FE;
	DWORD field_302;
	DWORD field_306;
	DWORD field_30A;

	BYTE _pad30E;

	BYTE field_30F;
	DWORD field_310;

	BYTE _pad314[4];

	WORD field_318;

	BYTE _pad31A[10];

public:
	CLocalPlayer();

	void ResetAllSyncAttributes();

	CPlayerPed * GetPlayerPed() { return m_pPlayerPed; };

	void Say(PCHAR szText);

	void SetPlayerColor(DWORD dwColor);
	DWORD GetPlayerColorAsRGBA();
	DWORD GetPlayerColorAsARGB();
	BOOL IsSpectating() { return m_bIsSpectating; };

	void ReturnToClassSelection() { m_bWantsAnotherClass = TRUE; };

	BOOL IsInRCMode() { return m_bInRCMode; };
};

//----------------------------------------------------------
