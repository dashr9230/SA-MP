
#include "main.h"

GAME_SETTINGS	tSettings;

void InitSettingsFromCommandLine(char * szCmdLine);

//----------------------------------------------------

/*
	Argument format:
		"-h %s -p %d -n %s -m %s"

		-h -> either server's bind address or 127.0.0.1
		-p -> server's port number
		-n -> NPC's name
		-m -> script name
*/
int main (int argc, char** argv)
{
	char szCmdLine[1024];
	memset(szCmdLine,0,1024);

	int cmdcnt=1;
	if(argc > 1) {
		while(cmdcnt != argc) {
			strcat(szCmdLine, argv[cmdcnt]);
			strcat(szCmdLine, " ");
			cmdcnt++;
		}
	}
	InitSettingsFromCommandLine(szCmdLine);

	// TODO: main
	// Absolutely no indication it is named npc or bot...

	return 0;
}

//----------------------------------------------------

void SetStringFromCommandLine(char* szCmdLine, char* szString);

void InitSettingsFromCommandLine(char * szCmdLine)
{
	logprintf(szCmdLine);

	memset(&tSettings,0,sizeof(GAME_SETTINGS));

	while(*szCmdLine) {

		if(*szCmdLine == '-' || *szCmdLine == '/') {
			szCmdLine++;
			switch(*szCmdLine) {
				case 'z':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPass);
					break;
				case 'h':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectHost);
					break;
				case 'p':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPort);
					break;
				case 'n':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szNickName);
					break;
				case 'm':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szModeName);
					break;
			}
		}

		szCmdLine++;
	}
}

//----------------------------------------------------

void SetStringFromCommandLine(char* szCmdLine, char* szString)
{
	while(*szCmdLine == ' ') szCmdLine++;
	while(*szCmdLine &&
		  *szCmdLine != ' ' &&
		  *szCmdLine != '-' &&
		  *szCmdLine != '/')
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}
	*szString = '\0';
}

//----------------------------------------------------
