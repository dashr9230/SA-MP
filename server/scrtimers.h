
#ifndef SAMPSRV_TIMERS_H
#define SAMPSRV_TIMERS_H

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
	DWORD NewEx(char* szScriptFunc, int iInterval, BOOL bRepeating, cell *params, AMX* pAMX);
	void Kill(DWORD dwTimerId);
	void FreeMem(ScriptTimer_s* Timer);
};

//----------------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------------
// EOF
