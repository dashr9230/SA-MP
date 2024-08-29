
#include "main.h"

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

