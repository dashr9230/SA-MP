
#ifndef SAMPSRV_OBJECTPOOL_H
#define SAMPSRV_OBJECTPOOL_H

class CObjectPool // size: WL 8012000
{
private:

	int field_0[1000][1000];
	int field_3D0900[1000];
	int field_3D18A0[1000][1000];
	int field_7A21A0[1000];
	int field_7A3140[1000];

public:
	CObjectPool();
	~CObjectPool();

	BOOL Delete(WORD wObjectID);
	BOOL DeleteForPlayer(PLAYERID playerId, WORD wObjectID);
};

#endif
