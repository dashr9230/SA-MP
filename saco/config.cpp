
#include "main.h"

//----------------------------------------------------------

CConfig::CConfig(char* a2)
{
	for(int i = 0; i != MAX_CONFIG_ENTRIES; i++) {
		memset(&field_0[i], 0, sizeof(UNNAMED_STRUCT_1));
		field_7A00[i] = 0;
	}

	field_8305 = 0;

	if(a2 && strlen(a2)) {
		strcpy(field_8200, a2);

		ReadFile();
	}
}

//----------------------------------------------------------

void CConfig::AddConfigEntry(char * szName, char * szData)
{
	// TODO: CConfig::AddConfigEntry 100661E0
}

BOOL CConfig::ReadFile()
{
	char	szReadBuffer[MAX_CONFIG_STRSIZE+1];
	char	szDirectiveName[MAX_CONFIG_STRSIZE+1];
	char	szDirectiveData[MAX_CONFIG_STRSIZE+1];

	char	*szReadPtr;
	int		iDirectiveLength;
	int		iDirectiveDataLength;

	FILE	*fReadFile = fopen(field_8200,"r");

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

void CConfig::sub_10066180()
{
	// TODO: CConfig::sub_10066180 10066180
}

void CConfig::sub_100660E0()
{
	// TODO: CConfig::sub_100660E0 100660E0
}

void CConfig::sub_10065FD0()
{
	// TODO: CConfig::sub_10065FD0 10065FD0
}

void CConfig::sub_10065F30()
{
	// TODO: CConfig::sub_10065F30 10065F30 (unused)
}

void CConfig::sub_10065C90()
{
	// TODO: CConfig::sub_10065C90 10065C90
}

void CConfig::sub_10065D30()
{
	// TODO: CConfig::sub_10065D30 10065D30
}

void CConfig::sub_10065D50()
{
	// TODO: CConfig::sub_10065D50 10065D50
}

void CConfig::sub_10065C40()
{
	// TODO: CConfig::sub_10065C40 10065C40
}

void CConfig::sub_10065E10()
{
	// TODO: CConfig::sub_10065E10 10065E10
}

void CConfig::sub_10065C00()
{
	// TODO: CConfig::sub_10065C00 10065C00
}

void CConfig::sub_10065E40()
{
	// TODO: CConfig::sub_10065E40 10065E40
}

void CConfig::sub_10065E70()
{
	// TODO: CConfig::sub_10065E70 10065E70 (unused)
}

void CConfig::sub_10065EA0()
{
	// TODO: CConfig::sub_10065EA0 10065EA0 (unused)
}

void CConfig::sub_10065F00()
{
	// TODO: CConfig::sub_10065F00 10065F00 (unused)
}

void CConfig::sub_10066080()
{
	// TODO: CConfig::sub_10066080 10066080
}

