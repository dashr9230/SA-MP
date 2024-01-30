
#ifndef SAMPNPC_TIMERS_H
#define SAMPNPC_TIMERS_H

struct ScriptTimer_s
{
	char _gap0[283];
};

typedef std::map<DWORD, ScriptTimer_s*> DwordTimerMap;

//----------------------------------------------------------------------------------

class CScriptTimers
{
private:
	DwordTimerMap m_Timers;
	DWORD m_dwTimerCount;
public:
	CScriptTimers();

};

#endif
