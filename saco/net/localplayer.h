
#pragma once

//----------------------------------------------------------

#pragma pack(1)
typedef struct _SPAWN_INFO {
	BYTE		byteTeam;
	int			iSkin;
	BYTE		byteUnk;
	VECTOR		vecPos;
	float		fRotation;
	int			iSpawnWeapons[3];
	int			iSpawnWeaponsAmmo[3];
} SPAWN_INFO, *PSPAWN_INFO;

#pragma pack(1)
class CLocalPlayer
{
public:

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

	DWORD field_143;

	DWORD field_147;

	DWORD field_14B;
	SPAWN_INFO m_SpawnInfo;

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

	BYTE field_30E;

	BYTE field_30F;
	DWORD field_310;

	DWORD field_314;

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
	void SetTeam(BYTE byteTeam) { m_byteTeam = byteTeam; };

	void ReturnToClassSelection() { m_bWantsAnotherClass = TRUE; };

	BOOL IsInRCMode() { return m_bInRCMode; };

	void SelectNextClass(); // .text:10005FF0
	void SelectPreviousClass(); // .text:10006080
	void RequestClass(int iClass); // .text:100058D0
	void SendStatsUpdate(); // .text:10005D00
	void FUNC_10005AD0(WORD a1, int a2); // .text:10005AD0
	void FUNC_10006FE0(); // .text:10006FE0
	void FUNC_10005BF0(WORD a1); // .text:10005BF0
	void FUNC_10003570(); // .text:10003570

	// the three ped-damage reports; all take `this` in ecx but never read it
	void FUNC_100068A0(WORD a1, float a2, int a3, int a4); // .text:100068A0
	void FUNC_100069B0(WORD a1, float a2, int a3, int a4); // .text:100069B0
	void FUNC_10006AC0(WORD a1, float a2, int a3, int a4); // .text:10006AC0
	void sub_10003C20(); // .text:10003C20
	void sub_10003BE0(SPAWN_INFO *pSpawnInfo); // .text:10003BE0
	void sub_100040E0(BOOL bSpawn); // .text:100040E0
	void sub_100030F0(BYTE a1); // .text:100030F0
	void sub_10003710(int a1, int a2); // .text:10003710
	void sub_10004060(); // .text:10004060
	int  FUNC_10003AB0(); // .text:10003AB0
	int  FUNC_10003AF0(); // .text:10003AF0
	int  FUNC_10003B30(); // .text:10003B30
	void RequestSpawn(); // .text:10003B60
	BOOL FUNC_10004340(WORD a1, WORD a2, WORD a3); // .text:10004340
	BOOL FUNC_10004300(WORD a1, WORD a2, WORD a3); // .text:10004300
	PLAYERID FUNC_10004B70(); // .text:10004B70
	ACTORID  FUNC_10004BB0(); // .text:10004BB0
	BOOL FUNC_10003A60(void *a2, void *a3, unsigned int a4); // .text:10003A60
	void FUNC_10004080(); // .text:10004080
	void FUNC_10004230(WORD a1); // .text:10004230
	void FUNC_10004280(PLAYERID playerId); // .text:10004280
	void FUNC_10002FA0(); // .text:10002FA0
	BYTE GetSpecialAction(); // .text:10003410
	void FUNC_10006100(); // .text:10006100
	void SendDeath(); // .text:10005810
	void SetInteriorId(BYTE byteInteriorId); // .text:10005970
	void FUNC_10003F50(); // .text:10003F50
	void FUNC_10003FC0(); // .text:10003FC0
	void FUNC_100041C0(BOOL bSpectating); // .text:100041C0

	DWORD GetField_F0() { return field_F0; };
};

//----------------------------------------------------------
