
#pragma once

#define MAX_CONFIG_STRSIZE 256
#define MAX_CONFIG_VARIABLES 512
#define MAX_CONFIG_VARIABLE_NAME 40

enum CONFIG_VARTYPE
{
	CONFIG_VARTYPE_NONE = 0,
	CONFIG_VARTYPE_INT,
	CONFIG_VARTYPE_STRING,
	CONFIG_VARTYPE_FLOAT,
};

struct ConfigVariable_s
{
	char szVarName[MAX_CONFIG_VARIABLE_NAME+1];
	BOOL bReadOnly;
	CONFIG_VARTYPE VarType;
	int iVarValue;
	float fVarValue;
	char* szVarValue;
};

class CConfig
{
private:
	ConfigVariable_s	m_Variables[MAX_CONFIG_VARIABLES];
	BOOL				m_bVariableSlotState[MAX_CONFIG_VARIABLES];
	CHAR				m_szConfigFileName[MAX_PATH+1];
	int					m_iUpperIndex;

public:
	~CConfig();
	void RecalcSize();
	int FindVariableIndex(char *szName);
	bool IsVariableExist(char *szName);
	int AddVariable(char *szName);
	int GetIntVariable(char *szName);
	char *GetStringVariable(char *szName);
	float GetFloatVariable(char *szName);
	BOOL RemoveVariable(char *szName);
	CONFIG_VARTYPE GetVariableType(char *szName);
	ConfigVariable_s *GetVariableAtIndex(int iIndex);
	CONFIG_VARTYPE DetermineDataType(char *szData);
	BOOL WriteFile();
	BOOL SetIntVariable(char *szName, int iValue, BOOL bReadOnly = FALSE);
	BOOL SetStringVariable(char *szName, char *szValue, BOOL bReadOnly = FALSE);
	BOOL SetFloatVariable(char *szName, float fValue, BOOL bReadOnly = FALSE);
	void AddConfigEntry(char *szName, char *szData);
	BOOL ReadFile();
	CConfig(char* szFileName);	
};
