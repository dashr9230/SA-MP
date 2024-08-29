
#ifndef SAMPNPC_TIMERS_H
#define SAMPNPC_TIMERS_H

struct ScriptTimer_s
{
	char szScriptFunc[255];
	int iTotalTime;
	int iRemainingTime;
	BOOL bRepeating;
	BOOL bKilled;
	AMX* pAMX;
	int iParamCount;
	void* cellParams;
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
	~CScriptTimers();

	DWORD New(char* szScriptFunc, int iInterval, BOOL bRepeating, AMX* pAMX);
	void FreeMem(ScriptTimer_s* Timer);
};

#endif
