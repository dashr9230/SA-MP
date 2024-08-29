
#include "main.h"

extern CNetGame* pNetGame;

//----------------------------------------------------------------------------------

CScriptTimers::CScriptTimers()
{
	m_dwTimerCount = 0;
}

//----------------------------------------------------------------------------------

CScriptTimers::~CScriptTimers()
{
	DwordTimerMap::iterator itor;
	for (itor = m_Timers.begin(); itor != m_Timers.end(); itor++)
	{
		FreeMem(itor->second);
		SAFE_DELETE(itor->second);
	}
	m_Timers.clear();
}

//----------------------------------------------------------------------------------

void CScriptTimers::FreeMem(ScriptTimer_s* Timer)
{
	if (Timer->cellParams != NULL)
	{
		free(Timer->cellParams);
		Timer->cellParams = NULL;
	}
}

//----------------------------------------------------------------------------------

// Kills only the timers found in one mode

void CScriptTimers::DeleteForMode(AMX* pEndedAMX)
{
	bool bRestart = false;
	DwordTimerMap::iterator itor;
	for (itor = m_Timers.begin(); itor != m_Timers.end(); bRestart?(itor=m_Timers.begin()):(itor++))
	{
		bRestart = false;
		if (itor->second->pAMX == pEndedAMX)
		{
			FreeMem(itor->second);
			SAFE_DELETE(itor->second);
			m_Timers.erase(itor);
			
			// Can't continue iteration if a node is deleted, start iteration from start again.
			bRestart = true;
		}
	}
}

//----------------------------------------------------------------------------------

DWORD CScriptTimers::New(char* szScriptFunc, int iInterval, BOOL bRepeating, AMX* pAMX)
{
	m_dwTimerCount++;

	ScriptTimer_s* NewTimer = new ScriptTimer_s;

	strncpy(NewTimer->szScriptFunc, szScriptFunc, 255);
	NewTimer->iTotalTime = iInterval;
	NewTimer->iRemainingTime = iInterval;
	NewTimer->bRepeating = bRepeating;
	NewTimer->iParamCount = 0;
	NewTimer->bKilled = false;
	NewTimer->pAMX = pAMX;
	NewTimer->cellParams = NULL;

	m_Timers.insert(DwordTimerMap::value_type(m_dwTimerCount, NewTimer));
	return m_dwTimerCount;
}

//----------------------------------------------------------------------------------

void CScriptTimers::Delete(DWORD dwTimerId)
{
	DwordTimerMap::iterator itor;
	itor = m_Timers.find(dwTimerId);
	if (itor != m_Timers.end())
	{
		FreeMem(itor->second);
		SAFE_DELETE(itor->second);
		m_Timers.erase(itor);
	}
}

//----------------------------------------------------------------------------------

void CScriptTimers::Kill(DWORD dwTimerId)
{
	DwordTimerMap::iterator itor;
	itor = m_Timers.find(dwTimerId);
	if (itor != m_Timers.end())
	{
		itor->second->iRemainingTime = 0;
		itor->second->bKilled = true;
		itor->second->bRepeating = false;
	}
}

//-----------------------------------------------------------

void CScriptTimers::Process(int iElapsedTime)
{
	DwordTimerMap::iterator itor;
	CGameMode *pGameMode;
	for (itor = m_Timers.begin(); itor != m_Timers.end(); itor++)
	{
		itor->second->iRemainingTime -= iElapsedTime;
		if (itor->second->iRemainingTime <= 0)
		{
			DwordTimerMap::iterator itor_tmp = ++itor; itor--;
			if (!itor->second->bKilled)
			{
				pGameMode = pNetGame->GetBotMode();
				if (pGameMode)
				{
					int idx;
					AMX* amx = itor->second->pAMX;
					if (amx && !amx_FindPublic(amx, itor->second->szScriptFunc, &idx))
					{
						cell ret;
						int count = itor->second->iParamCount;
						int i = 0;
						if (count > 0)
						{
							cell* pars = (cell*)itor->second->cellParams;
							while (i < count)
							{
								amx_Push(amx, pars[i]);
								i++; // Go forwards to maintain push order
							}
						}
						amx_Exec(amx, &ret, idx);
					}
				}
			}
	
			if (itor->second->bRepeating)
			{
				itor->second->iRemainingTime = itor->second->iTotalTime;
			}
			else
			{
				// Release parameter memory
				FreeMem(itor->second);
				Delete(itor->first);
			}
			itor = itor_tmp;
		}
		if (itor == m_Timers.end()) break;
	}
}

//----------------------------------------------------------------------------------
// EOF
