
#ifndef SAMPSRV_PLAYER_H
#define SAMPSRV_PLAYER_H

#pragma pack(1)
typedef struct _PLAYER_SPAWN_INFO
{
	char _pad0[46];
} PLAYER_SPAWN_INFO;

//----------------------------------------------------

#pragma pack(1)
class CPlayer
{
public:

	// Size: 11486
	char _pad0[11261];

	PLAYER_SPAWN_INFO		m_SpawnInfo;
	BOOL					m_bHasSpawnInfo;

	char _pad2C2F[175];


	void Say(unsigned char * szText, BYTE byteTextLength);

	void SetSpawnInfo(PLAYER_SPAWN_INFO *pSpawn);
};

//----------------------------------------------------

#endif

//----------------------------------------------------
// EOF
