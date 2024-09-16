
#ifndef SAMPSRV_PLAYER_H
#define SAMPSRV_PLAYER_H

#pragma pack(1)
typedef struct _PLAYER_SPAWN_INFO
{
	BYTE byteTeam;
	int iSkin;
	BYTE field_5;
	VECTOR vecPos;
	float fRotation;
	int iSpawnWeapons[3];
	int iSpawnWeaponsAmmo[3];
} PLAYER_SPAWN_INFO;

#pragma pack(1)
typedef struct _ONFOOT_SYNC_DATA
{
	char _pad0[68];
	// TODO: _ONFOOT_SYNC_DATA
} ONFOOT_SYNC_DATA;

#pragma pack(1)
typedef struct _AIM_SYNC_DATA
{
	char _pad0[31];
	// TODO: _AIM_SYNC_DATA
} AIM_SYNC_DATA;

#pragma pack(1)
typedef struct _UNOCCUPIED_SYNC_DATA
{
	char _pad0[67];
	// TODO: _UNOCCUPIED_SYNC_DATA
} UNOCCUPIED_SYNC_DATA;

#pragma pack(1)
typedef struct _INCAR_SYNC_DATA
{
	char _pad0[63];
	// TODO: _INCAR_SYNC_DATA
} INCAR_SYNC_DATA;

#pragma pack(1)
typedef struct _PASSENGER_SYNC_DATA
{
	char _pad0[24];
	// TODO: _PASSENGER_SYNC_DATA
} PASSENGER_SYNC_DATA;

#pragma pack(1)
typedef struct _SPECTATOR_SYNC_DATA
{
	char _pad0[18];
	// TODO: _SPECTATOR_SYNC_DATA
} SPECTATOR_SYNC_DATA;

#pragma pack(1)
typedef struct _TRAILER_SYNC_DATA
{
	char _pad0[54];
	// TODO: _TRAILER_SYNC_DATA
} TRAILER_SYNC_DATA;

#pragma pack(1)
typedef struct _WEAPON_SHOT_SYNC_DATA
{
	char _pad0[40];
	// TODO: _WEAPON_SHOT_SYNC_DATA
} WEAPON_SHOT_SYNC_DATA;

typedef struct struc_92
{
	char _pad0[52];
	// TODO: struc_92
} struc_92;

//----------------------------------------------------

#pragma pack(1)
class CPlayer
{
public:

	AIM_SYNC_DATA			m_aimSync;
	short field_1F;
	short field_21;
	short field_23;
	short field_25;
	INCAR_SYNC_DATA			m_icSync;
	PASSENGER_SYNC_DATA		m_psSync;
	ONFOOT_SYNC_DATA		m_ofSync;
	UNOCCUPIED_SYNC_DATA	m_unocSync;
	SPECTATOR_SYNC_DATA		m_spSync;
	TRAILER_SYNC_DATA		m_trSync;
	int field_14D;
	int field_151;
	char field_155[1000];
	char field_53D[2000];
	char field_D0D[1000];
	char field_10F5[1024];
	char field_14F5[4096];
	char field_24F5[1000];
	int field_28DD;
	int field_28E1;
	int field_28E5;
	int field_28E9;
	int field_28ED;
	int field_28F1;
	int field_28F5;
	int field_28F9;

	char _pad28FD[12];

	int field_2909;
	int field_290D;
	int field_2911;
	int field_2915;
	int field_2919;
	int field_291D;
	int field_2921;
	int field_2925;
	int field_2929;
	int field_292D;
	int field_2931;
	int field_2935;
	int	field_2939;
	int field_293D;
	int field_2941;
	int field_2945;
	short field_2949;
	short field_294B;
	int field_294D;
	int field_2951;
	int field_2955;
	int field_2959;
	short field_295D;
	int field_295F;
	int field_2963;
	PLAYERID				m_PlayerID;
	int field_2969;
	struc_92 field_296D[10];
	BOOL field_2B75[10];
	int field_2B9D;
	int field_2BA1;

	char _pad2BA5[4];

	char field_2BA9;

	VECTOR					m_vecCheckpoint;
	float					m_fCheckpointSize;
	BOOL					m_bInCheckpoint;

	VECTOR					m_vecRaceCheckpoint;
	VECTOR					m_vecRaceNextCheckpoint;
	BYTE					m_byteRaceCheckpointType;
	float					m_fRaceCheckpointSize;
	BOOL					m_bInRaceCheckpoint;

	int field_2BDF;
	short field_2BE3[11];
	RakNetTime field_2BF9;
	PLAYER_SPAWN_INFO		m_SpawnInfo;
	BOOL					m_bHasSpawnInfo;
	char field_2C2F;
	char field_2C30;
	char field_2C31;
	short field_2C32;
	int field_2C34;
	BOOL					m_bCheckpointEnabled;
	BOOL					m_bRaceCheckpointEnabled;
	int field_2C40;

	// Weapon data
	DWORD					m_dwSlotAmmo[13];
	BYTE					m_byteSlotWeapon[13];

	char field_2C85;
	short field_2C86;
	short field_2C88;
	RakNetTime field_2C8A;
	char field_2C8E;
	char field_2C8F[40];
	char field_2CB7;
	float field_2CB8;
	char field_2CBC;
	int field_2CBD;
	RakNetTime field_2CC1;
	int field_2CC5;
	int field_2CC9;

	char _pad2CCD[4];

	char field_2CD1;

	char _pad2CD2[8];

	int field_2CDA;

	void Say(unsigned char * szText, BYTE byteTextLength);

	void SetSpawnInfo(PLAYER_SPAWN_INFO *pSpawn);

	void SetCheckpoint(float fX, float fY, float fZ, float fSize);
	void ToggleCheckpoint(BOOL bEnabled);
	void ToggleRaceCheckpoint(BOOL bEnabled);
};

//----------------------------------------------------

#endif

//----------------------------------------------------
// EOF
