
#include "main.h"

void AMXPrintError(CGameMode* pGameMode, AMX *amx, int error);

char szGameModeFileName[256];

//----------------------------------------------------------------------------------

CGameMode::CGameMode()
{
	m_bInitialised = false;
	m_bSleeping = false;
}

//----------------------------------------------------------------------------------

CGameMode::~CGameMode()
{
	Unload();
}

//----------------------------------------------------------------------------------

void CGameMode::Unload()
{
	// TODO: CGameMode::Unload
}

//----------------------------------------------------------------------------------

void CGameMode::Frame(float fElapsedTime)
{
	if (!m_bInitialised)
		return;

	if (!m_bSleeping)
		return;

	if (m_fSleepTime > 0.0f)
	{
		m_fSleepTime -= fElapsedTime;
	}
	else
	{
		cell ret;
		int err = amx_Exec(&m_amx, &ret, AMX_EXEC_CONT);
		if (err == AMX_ERR_SLEEP)
		{
			m_bSleeping = true;
			m_fSleepTime = ((float)ret / 1000.0f);
		}
		else
		{
			m_bSleeping = false;
			AMXPrintError(this, &m_amx, err);
		}
	}
}

//----------------------------------------------------------------------------------

