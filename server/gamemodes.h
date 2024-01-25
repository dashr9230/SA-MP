
#ifndef SAMPSRV_GAMEMODES_H
#define SAMPSRV_GAMEMODES_H

class CGameMode // size: WL 110
{
private:
	char gap0[104];
	char field_68;
	char field_69;
	char gap6A[4];

public:
	CGameMode();
	~CGameMode();

	bool Load(char* pFileName);
	void Unload();
};

#endif
