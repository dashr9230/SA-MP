
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

