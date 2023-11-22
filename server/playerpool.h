
#ifndef SAMPSRV_PLAYERPOOL_H
#define SAMPSRV_PLAYERPOOL_H

// TODO: PLAYER_SPAWN_INFO
typedef struct _PLAYER_SPAWN_INFO // size: 46
{
	char _gap0[46];
} PLAYER_SPAWN_INFO;

class CPlayerPool // size: WL 199024
{
private:

	int m_iVirtualWorld[MAX_PLAYERS];
	int field_FA0;
	int field_FA4;
	int field_FA8;
	int m_iPlayerScore[MAX_PLAYERS];
	int m_iPlayerMoney[MAX_PLAYERS];
	char gap2EEC[4000];
	DWORD field_3E8C[1000];
	int field_23A5C[1000];
	BOOL m_bPlayerSlotState[MAX_PLAYERS];
	int field_2599C[1000];
	char gap2693C[41000];
	int field_30964;
	int field_30968;
	int field_3096C;

public:

	CPlayerPool();

};

#endif
