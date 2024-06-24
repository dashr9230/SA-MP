
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
// Same as new only with parameters to be passed to the called function
cell* get_amxaddr(AMX *amx, cell amx_addr);
DWORD CScriptTimers::NewEx(char* szScriptFunc, int iInterval, BOOL bRepeating, cell *params, AMX* pAMX)
{
	m_dwTimerCount++;

	ScriptTimer_s* NewTimer = new ScriptTimer_s;

	strncpy(NewTimer->szScriptFunc, szScriptFunc, 255);
	NewTimer->iTotalTime = iInterval;
	NewTimer->iRemainingTime = iInterval;
	NewTimer->bRepeating = bRepeating;
	NewTimer->bKilled = false;
	NewTimer->pAMX = pAMX;

	cell amx_addr[256];

	char* szParamList;
	amx_StrParam(pAMX, params[4], szParamList);
	int j, numstr, iOff = 5; // Count, func, interval, repeat, map
	if (szParamList == NULL) j = 0;
	else j = strlen(szParamList);
	numstr = 0;
	while (j)
	{
		j--;
		cell *paddr = NULL;
		if (*(szParamList + j) == 'a')
		{
			int numcells = *get_amxaddr(pAMX, params[j + iOff + 1]);
			if (amx_Allot(pAMX, numcells, &amx_addr[numstr], &paddr) == AMX_ERR_NONE)
			{
				memcpy(paddr, get_amxaddr(pAMX, params[j + iOff]), numcells * sizeof (cell));
				numstr++;
			}
		}
		else if (*(szParamList + j) == 's')
		{
			char* szParamText;
			amx_StrParam(pAMX, params[j + iOff], szParamText);
			if (szParamText != NULL && strlen(szParamText) > 0)
			{
				int numcells = strlen(szParamText) + 1;
				if (amx_Allot(pAMX, numcells, &amx_addr[numstr], &paddr) == AMX_ERR_NONE)
				{
					amx_SetString(paddr, szParamText, 0, 0, UNLIMITED);
					numstr++;
				}
			}
			else
			{
				*szParamText = 1;
				*(szParamText + 1) = 0;
				if (amx_Allot(pAMX, 1, &amx_addr[numstr], &paddr) == AMX_ERR_NONE)
				{
					amx_SetString(paddr, szParamText, 0, 0, UNLIMITED);
					numstr++;
				}
			}
		}
		else
		{
			amx_addr[numstr] = *get_amxaddr(pAMX, params[j + iOff]);
			numstr++;
		}
	}
	void* mem = NULL;
	if (numstr)
	{
		mem = malloc(numstr * sizeof (cell));
		memcpy(mem, &amx_addr, numstr * sizeof (cell));
		NewTimer->cellParams = mem;
	}
	else
	{
		NewTimer->cellParams = NULL;
	}
	NewTimer->iParamCount = numstr;

	m_Timers.insert(DwordTimerMap::value_type(m_dwTimerCount, NewTimer));
	return m_dwTimerCount;
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

