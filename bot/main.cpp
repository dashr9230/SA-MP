
#include "main.h"

GAME_SETTINGS	tSettings;
CNetGame		*pNetGame=0;

void InitSettingsFromCommandLine(char * szCmdLine);

//----------------------------------------------------

int main (int argc, char** argv)
{
	logprintf("----- NPC Starting -----");

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

	pNetGame = new CNetGame();
	pNetGame->Init(tSettings.szConnectHost,atoi(tSettings.szConnectPort),
		tSettings.szNickName,tSettings.szConnectPass,tSettings.szModeName);

#ifdef WIN32
	timeBeginPeriod(5);
#endif

	// TODO: main
	
	while(1) {
		pNetGame->Process();
		SLEEP(5);

		if( pNetGame->GetGameState() == GAMESTATE_CONNECTED &&
			!pNetGame->GetRakClient()->IsConnected() ) {
			break;
		}
	}

	delete pNetGame;

	logprintf("----- NPC: End -----");

	return 0;
}

//----------------------------------------------------

void logprintf(char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	char buffer[2048];
	vsprintf(buffer, format, ap);

#ifdef WIN32
	OutputDebugString(buffer);
#endif

	va_end(ap);
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

#ifdef LINUX

void SignalHandler(int sig)
{
	// nothing
}

//----------------------------------------------------
#include <time.h>

long GetTickCount()
{
	tms tm;
	return (times(&tm) * 10);
}

// strlwr is not included with the GNU C lib it seems.
char* strlwr(char* str)
{
	for (size_t i=0; i<strlen(str); i++)
	{
		if ((str[i] >= 'A') && (str[i] <= 'Z'))
		{
			str[i] -= 32;
		}
	}
	return str;
}

#endif

//----------------------------------------------------
