
#ifndef SAMPNPC_GAMEMODES_H
#define SAMPNPC_GAMEMODES_H

extern char szGameModeFileName[256];

//----------------------------------------------------------------------------------

class CGameMode
{
private:
	AMX m_amx;
	bool m_bInitialised;
	bool m_bSleeping;
	float m_fSleepTime;

public:
	CGameMode();
	~CGameMode();

	char* GetFileName() { return &szGameModeFileName[0]; };

	void Unload();

	void Frame(float fElapsedTime);

};

//----------------------------------------------------------------------------------

#endif
