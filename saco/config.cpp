
#include "main.h"

#include <stdio.h>
#include <string.h>

//----------------------------------------------------------

CConfig::CConfig(char* szFileName)
{
	for(int i = 0; i != MAX_CONFIG_VARIABLES; i++)
	{
		memset(&m_Variables[i], 0, sizeof(ConfigVariable_s));
		m_bVariableSlotState[i] = FALSE;
	}

	m_iUpperIndex = 0;

	if(szFileName && strlen(szFileName))
	{
		strcpy(m_szConfigFileName, szFileName);

		ReadFile();
	}
}

//----------------------------------------------------------

CConfig::~CConfig()
{
	int i = 0;

	while(i != MAX_CONFIG_VARIABLES)
	{
		if(m_bVariableSlotState[i])
		{
			if(m_Variables[i].szVarValue)
				free(m_Variables[i].szVarValue);
		}
		m_bVariableSlotState[i] = FALSE;
		i++;
	}
}

//----------------------------------------------------------

void CConfig::RecalcSize()
{
	int iNewIndex = 0;
	int i = 0;

	do
	{
		if(m_bVariableSlotState[i])
			iNewIndex = i;
		if(m_bVariableSlotState[i+1])
			iNewIndex = i+1;
		if(m_bVariableSlotState[i+2])
			iNewIndex = i+2;
		if(m_bVariableSlotState[i+3])
			iNewIndex = i+3;
		i += 4;
	}
	while(i < MAX_CONFIG_VARIABLES);

	m_iUpperIndex = iNewIndex + 1;
}

//----------------------------------------------------------

int CConfig::FindVariableIndex(char *szName)
{
	strlwr(szName);

	int i = 0;
	while(i < m_iUpperIndex)
	{
		if(m_bVariableSlotState[i])
		{
			if(!strcmp(szName, m_Variables[i].szVarName))
			{
				return i;
			}
		}
		i++;
	}
	return -1;
}

//----------------------------------------------------------

bool CConfig::IsVariableExist(char *szName)
{
	if(FindVariableIndex(szName) >= 0) return true;
	return false;
}

//----------------------------------------------------------

int CConfig::AddVariable(char *szName)
{
	int iIndex = 0;
	int i = 0;
	do
	{
		if(!m_bVariableSlotState[i])
		{
			break;
		}
		if(!m_bVariableSlotState[i+1])
		{
			iIndex += 1;
			break;
		}
		if(!m_bVariableSlotState[i+2])
		{
			iIndex += 2;
			break;
		}
		if(!m_bVariableSlotState[i+3])
		{
			iIndex += 3;
			break;
		}
		i += 4;
		iIndex += 4;
	}
	while(i < MAX_CONFIG_VARIABLES);

	if(iIndex == MAX_CONFIG_VARIABLES) return -1;
	if(strlen(szName) > MAX_CONFIG_VARIABLE_NAME) return -1;

	strlwr(szName);
	strcpy(m_Variables[iIndex].szVarName, szName);

	m_Variables[iIndex].bReadOnly = FALSE;
	m_Variables[iIndex].VarType = CONFIG_VARTYPE_NONE;
	m_bVariableSlotState[iIndex] = TRUE;

	RecalcSize();

	return iIndex;
}

//----------------------------------------------------------

int CConfig::GetIntVariable(char *szName)
{
	int iIndex = FindVariableIndex(szName);

	if(iIndex < 0) return 0;
	if(m_Variables[iIndex].VarType != CONFIG_VARTYPE_INT) return 0;

	return m_Variables[iIndex].iVarValue;
}

//----------------------------------------------------------

char *CConfig::GetStringVariable(char *szName)
{
	int iIndex = FindVariableIndex(szName);

	if(iIndex < 0) return NULL;
	if(m_Variables[iIndex].VarType != CONFIG_VARTYPE_STRING) return NULL;

	return m_Variables[iIndex].szVarValue;
}

//----------------------------------------------------------

float CConfig::GetFloatVariable(char *szName)
{
	int iIndex = FindVariableIndex(szName);

	if(iIndex < 0) return 0.0f;
	if(m_Variables[iIndex].VarType != CONFIG_VARTYPE_FLOAT) return 0.0f;

	return m_Variables[iIndex].fVarValue;
}

//----------------------------------------------------------

BOOL CConfig::RemoveVariable(char *szName)
{
	int iIndex = FindVariableIndex(szName);

	if(iIndex < 0) return FALSE;

	if(m_Variables[iIndex].szVarValue )
		free(m_Variables[iIndex].szVarValue);

	memset(&m_Variables[iIndex],0,sizeof(ConfigVariable_s));
	m_bVariableSlotState[iIndex] = FALSE;

	RecalcSize();

	return TRUE;
}

//----------------------------------------------------------

CONFIG_VARTYPE CConfig::GetVariableType(char *szName)
{
	int iIndex = FindVariableIndex(szName);

	if(iIndex < 0) return CONFIG_VARTYPE_NONE;

	return m_Variables[iIndex].VarType;
}

//----------------------------------------------------------

ConfigVariable_s *CConfig::GetVariableAtIndex(int iIndex)
{
	if(iIndex >= 0 && iIndex < MAX_CONFIG_VARIABLES)
	{
		if(m_bVariableSlotState[iIndex])
		{
			return &m_Variables[iIndex];
		}
	}
	return NULL;
}

//----------------------------------------------------------

CONFIG_VARTYPE CConfig::DetermineDataType(char *szData)
{
	if(!szData || !strlen(szData)) return CONFIG_VARTYPE_NONE;

	if(szData[0] == '"' && szData[strlen(szData)-1] == '"')
		return CONFIG_VARTYPE_STRING;
	else if(strchr(szData, '.') != NULL)
		return CONFIG_VARTYPE_FLOAT;
	else
		return CONFIG_VARTYPE_INT;
}

//----------------------------------------------------------

BOOL CConfig::WriteFile()
{
	FILE *fWriteFile = fopen(m_szConfigFileName, "w");
	
	if(!fWriteFile) return FALSE;

	int i = 0;
	do
	{
		if(m_bVariableSlotState[i])
		{
			switch(m_Variables[i].VarType)
			{
			case CONFIG_VARTYPE_INT:
				fprintf(fWriteFile, "%s=%d\n", m_Variables[i].szVarName, m_Variables[i].iVarValue);
				break;
			case CONFIG_VARTYPE_STRING:
				if(m_Variables[i].szVarValue)
					fprintf(fWriteFile, "%s=\"%s\"\n", m_Variables[i].szVarName, m_Variables[i].szVarValue);
				break;
			case CONFIG_VARTYPE_FLOAT:
				fprintf(fWriteFile, "%s=%f\n", m_Variables[i].szVarName, m_Variables[i].fVarValue);
				break;
			}
		}
		i++;
	}
	while(i != MAX_CONFIG_VARIABLES);

	fclose(fWriteFile);
	return TRUE;
}

//----------------------------------------------------------

BOOL CConfig::SetIntVariable(char *szName, int iValue, BOOL bReadOnly)
{
	int iIndex = FindVariableIndex(szName);
	if(iIndex < 0)
	{
		iIndex = AddVariable(szName);
		if (iIndex < 0)
			return FALSE;
	}

	if(!bReadOnly)
	{
		if(m_Variables[iIndex].bReadOnly)
		{
			return FALSE;
		}
	}
	else
		m_Variables[iIndex].bReadOnly = TRUE;

	m_Variables[iIndex].iVarValue = iValue;
	m_Variables[iIndex].VarType = CONFIG_VARTYPE_INT;

	WriteFile();

	return TRUE;
}

//----------------------------------------------------------

BOOL CConfig::SetStringVariable(char *szName, char *szValue, BOOL bReadOnly)
{
	int iIndex = FindVariableIndex(szName);
	if(iIndex < 0)
	{
		iIndex = AddVariable(szName);
		if(iIndex < 0)
			return FALSE;
	}

	if(!bReadOnly)
	{
		if(m_Variables[iIndex].bReadOnly)
		{
			return FALSE;
		}
	}
	else
		m_Variables[iIndex].bReadOnly = TRUE;

	m_Variables[iIndex].VarType = CONFIG_VARTYPE_STRING;
	if(m_Variables[iIndex].szVarValue)
		free(m_Variables[iIndex].szVarValue);
	m_Variables[iIndex].szVarValue = (char *)calloc(1, strlen(szValue) + 1);
	strcpy(m_Variables[iIndex].szVarValue, szValue);

	WriteFile();

	return TRUE;
}

//----------------------------------------------------------

BOOL CConfig::SetFloatVariable(char *szName, float fValue, BOOL bReadOnly)
{
	int iIndex = FindVariableIndex(szName);
	if(iIndex < 0)
	{
		iIndex = AddVariable(szName);
		if (iIndex < 0)
			return FALSE;
	}

	if(!bReadOnly)
	{
		if(m_Variables[iIndex].bReadOnly)
		{
			return FALSE;
		}
	}
	else
		m_Variables[iIndex].bReadOnly = TRUE;

	m_Variables[iIndex].fVarValue = fValue;
	m_Variables[iIndex].VarType = CONFIG_VARTYPE_FLOAT;

	WriteFile();

	return TRUE;
}

//----------------------------------------------------------

void CConfig::AddConfigEntry(char *szName, char *szData)
{
	size_t len;

	switch(DetermineDataType(szData))
	{
		case CONFIG_VARTYPE_INT:
			SetIntVariable(szName, atoi(szData));
			break;
		case CONFIG_VARTYPE_STRING:
			len = strlen(szData);
			strncpy(szData, szData + 1, len - 1);
			szData[len - 2] = 0;
			SetStringVariable(szName, szData);
			break;
		case CONFIG_VARTYPE_FLOAT:
			SetFloatVariable(szName, atof(szData));
			break;
	}
}

//----------------------------------------------------------

BOOL CConfig::ReadFile()
{
	char	szReadBuffer[MAX_CONFIG_STRSIZE+1];
	char	szDirectiveName[MAX_CONFIG_STRSIZE+1];
	char	szDirectiveData[MAX_CONFIG_STRSIZE+1];

	char	*szReadPtr;
	int		iDirectiveLength;
	int		iDirectiveDataLength;

	FILE	*fReadFile = fopen(m_szConfigFileName,"r");

	if(!fReadFile) return FALSE;

	while(!feof(fReadFile)) {
		fgets(szReadBuffer,MAX_CONFIG_STRSIZE,fReadFile);
		szReadPtr = szReadBuffer;
		iDirectiveLength = 0;
		iDirectiveDataLength = 0;

		// Skip any leading whitespace
		while(*szReadPtr == ' ' || *szReadPtr == '\t') szReadPtr++;

		// Check for comment char, blank line or a key name. Key names
		// are currently resevered for future use.
		if( *szReadPtr == '\0' || *szReadPtr == ';' ||
			*szReadPtr == '\n' || *szReadPtr == '[' ) {

			continue;
		}

		// Parse out the directive name
		while( *szReadPtr != '\0' &&
			 *szReadPtr != ' ' &&
			 *szReadPtr != '=' &&
			 *szReadPtr != '\n' &&
			 *szReadPtr != '\t' &&
			 *szReadPtr != ';' ) {
			szDirectiveName[iDirectiveLength] = toupper(*szReadPtr);
			iDirectiveLength++;
			szReadPtr++;
		}

		if(iDirectiveLength == 0) {
			continue;
		}

		szDirectiveName[iDirectiveLength] = '\0';

		// Skip any whitespace
		while(*szReadPtr == ' ' || *szReadPtr == '\t') szReadPtr++;

		// The config entry is delimited by '='
		if(*szReadPtr != '=') {
			continue;
		}

		// The rest is the directive data
		*szReadPtr++;

		// Skip any whitespace
		while(*szReadPtr == ' ' || *szReadPtr == '\t') szReadPtr++;

		if( *szReadPtr == '\0' ) {
			continue;
		}

		while( *szReadPtr != '\0' && 
			   *szReadPtr != '\n' ) {
			szDirectiveData[iDirectiveDataLength] = *szReadPtr;
			iDirectiveDataLength++;
			szReadPtr++;
		}

		if(iDirectiveDataLength == 0) {
			continue;
		}

		szDirectiveData[iDirectiveDataLength] = '\0';

		// cleanup any trailing whitespace on the directive data.
		iDirectiveDataLength--;
		while(szDirectiveData[iDirectiveDataLength] == ' '  ||
			  szDirectiveData[iDirectiveDataLength] == '\t' ||
			  szDirectiveData[iDirectiveDataLength] == '\r')
		{
			szDirectiveData[iDirectiveDataLength] = '\0';
			iDirectiveDataLength--;
		}

		AddConfigEntry(szDirectiveName,szDirectiveData);
	}

	fclose(fReadFile);
	return TRUE;
}

//----------------------------------------------------------
// EOF
