
#pragma once

//----------------------------------------------------

struct UNOCCUPIED_SYNC_DATA
{
	VEHICLEID	VehicleID;
	BYTE		byteSeatID;
	VECTOR		vecRoll;
	VECTOR		vecDirection;
	VECTOR		vecPos;
	VECTOR		vecMoveSpeed;
	VECTOR		vecTurnSpeed;
};

struct TRAILER_SYNC_DATA
{
	VEHICLEID	TrailerID;
	VECTOR		vecPos;
	float		quat[4];
	VECTOR		vecMoveSpeed;
	VECTOR		vecTurnSpeed;
};

// the bullet sync payload, 0x28 bytes on the wire
struct BULLET_SYNC_DATA
{
	BYTE		byteHitType;
	WORD		wHitID;
	VECTOR		vecHitOrigin;
	VECTOR		vecHitTarget;
	VECTOR		vecCenterOfHit;
	BYTE		byteWeaponID;
};

#pragma pack(1)
class CRemotePlayer
{
public:
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
	// one 68-byte block: ResetAllSyncAttributes memsets it whole
	struct {
		char _pad0[18];
		float quat[4]; // 0xD7
		char _padE7[16];
		VECTOR vecOffset; // 0xF7
		WORD wVehicleID; // 0x103
		int iAnimation; // 0x105
	} field_C5;
	char field_109;
	BYTE field_10A;
	BYTE field_10B;
	int field_10C;
	int field_110;

	char _gap114[76];
	float field_160;
	float field_164;
	float field_168;
	float quat16C[4];
	VECTOR field_17C;
	VECTOR field_188;
	VECTOR field_194;
	VECTOR field_1A0;

	float field_1AC;
	float field_1B0;
	int field_1B4;
	char field_1B8;
	DWORD field_1B9;
	int field_1BD;
	int field_1C1;
	int field_1C5;
	VECTOR field_1C9;
	DWORD field_1D5;
	DWORD field_1D9;
	CPlayerPed			*m_pPlayerPed;
	CVehicle *field_1E1;
	PLAYERID			m_PlayerID;
	WORD field_1E7;
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

	void FUNC_10017C80(BYTE byteValue) { field_4 = (byteValue != 0); }; // .text:10017C80

	BOOL FUNC_10001080() { return m_pPlayerPed && field_10A; }; // .text:10001080

	char FUNC_10014380() { return field_C; }; // .text:10014380
	int FUNC_10014390() { return field_10C; }; // .text:10014390

	void EnterVehicle(VEHICLEID VehicleID, BOOL bPassenger);
	void ExitVehicle();

	void FUNC_100143A0(); // .text:100143A0
	void FUNC_10014500(int a1); // .text:10014500
	void FUNC_10014540(short a1, short a2, short a3); // .text:10014540
	void FUNC_10014650(); // .text:10014650
	void FUNC_100148F0(); // .text:100148F0
	void FUNC_10014FF0(); // .text:10014FF0
	void FUNC_10017260(BYTE *pSync, int iTime); // .text:10017260
	void FUNC_10014C40(); // .text:10014C40
	void FUNC_10016370(BULLET_SYNC_DATA *pSync); // .text:10016370
	void FUNC_10014950(); // .text:10014950
	void FUNC_10015140(); // .text:10015140
	void FUNC_100158D0(UNOCCUPIED_SYNC_DATA *pSync); // .text:100158D0
	void FUNC_10015C90(TRAILER_SYNC_DATA *pTrailer); // .text:10015C90
	void FUNC_10014E60(); // .text:10014E60
	void FUNC_10015460(); // .text:10015460
	void FUNC_10015760(BYTE *pAim); // .text:10015760
	BOOL FUNC_10017130(int a1, int iSkin, int a3, VECTOR *pPos, float fRotation,
		DWORD dwColor, BYTE byteStyle); // .text:10017130
	void FUNC_10015650(MATRIX4X4 *pMat, VECTOR *pMoveSpeed, float fT); // .text:10015650
	void FUNC_10017340(BYTE *pSync, int iTime); // .text:10017340
	void FUNC_10017440(BYTE *pSync); // .text:10017440
	void FUNC_10014800(); // .text:10014800
	void FUNC_10017610(char *szText); // .text:10017610
	void FUNC_100150D0(VECTOR *pPos, VECTOR *pMoveSpeed); // .text:100150D0
	void FUNC_100155E0(int a1, VECTOR *pPos, VECTOR *pMoveSpeed); // .text:100155E0
	void FUNC_10015760(char *pAimSync); // .text:10015760
	void FUNC_10015C90(char *pTrailerSync); // .text:10015C90
	void FUNC_10017440(char *pPassengerSync); // .text:10017440
	BOOL FUNC_100145F0(); // .text:100145F0
	void FUNC_10016270(BYTE byteNewState, BYTE byteOldState); // .text:10016270
	void FUNC_100165B0(BYTE byteNewState); // .text:100165B0
	BOOL FUNC_10014620(); // .text:10014620
	void FUNC_10016660(); // .text:10016660
	float FUNC_100160A0(CRemotePlayer *pOther); // .text:100160A0
	float FUNC_10016120(); // .text:10016120
	void sub_10017610(PCHAR szText); // .text:10017610
	void FUNC_10017530(); // .text:10017530
	void FUNC_10017570(); // .text:10017570
	int FUNC_10016330(); // .text:10016330
};

//----------------------------------------------------
