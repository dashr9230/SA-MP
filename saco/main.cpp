
#include "main.h"

int				iGtaVersion=0;

GAME_SETTINGS			tSettings;
CConfig					*pConfig=0;
CChatWindow				*pChatWindow=0;
CCmdWindow				*pCmdWindow=0;
//DWORD					dword_1026EB88=0;
CAudioStream			*pAudioStream=0;
//DWORD					dword_1026EB90=0;
CNetGame				*pNetGame=0;
//DWORD					dword_1026EB98=0;
CFontRender				*pDefaultFont=0;
//DWORD					dword_1026EBA0=0;
//DWORD					dword_1026EBA4=0;
//DWORD					dword_1026EBA8=0;

HANDLE			hInstance=0;

CGame					*pGame=0;
DWORD					dwGraphicsLoop=0;

CFileSystem *pFileSystem=NULL;

// forwards

void TheGraphicsLoop();
LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);

//----------------------------------------------------

#define ARCHIVE_FILE	"samp.saa"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	// TODO: DllMain
	if(DLL_PROCESS_ATTACH==fdwReason)
	{
		hInstance = hinstDLL;
		InitSettings();

		if(tSettings.bDebug || tSettings.bPlayOnline)
		{
			SetUnhandledExceptionFilter(exc_handler);
			dwGraphicsLoop = (DWORD)TheGraphicsLoop;

			CHAR szArchiveFile[MAX_PATH];
			GetModuleFileNameA((HMODULE)hInstance, szArchiveFile, MAX_PATH);
			DWORD dwFileNameLen = strlen(szArchiveFile);
			while(szArchiveFile[dwFileNameLen] != '\\')
				dwFileNameLen--;
			strcpy(szArchiveFile+dwFileNameLen+1, ARCHIVE_FILE);

			pFileSystem = new CArchiveFS();
			if(!pFileSystem->Load(ARCHIVE_FILE)) _asm int 3

			AddFontResourceA("gtaweap3.ttf");
			AddFontResourceA("sampaux3.ttf");

			//sub_10062CA0();
			//sub_10062970();

			pGame = new CGame();

			//_beginthread((int)sub_100C3A80, 0, 0);
		}
	}
	else if(DLL_PROCESS_DETACH==fdwReason)
	{
		if(tSettings.bDebug || tSettings.bPlayOnline) {
			//sub_10062D90
		}
	}

	return TRUE;
}

//----------------------------------------------------

void TheGraphicsLoop()
{
}

//----------------------------------------------------

void InitSettings()
{
	PCHAR szCmdLine = GetCommandLineA();

	memset(&tSettings,0,sizeof(GAME_SETTINGS));

	while(*szCmdLine) {

		if(*szCmdLine == '-' || *szCmdLine == '/') {
			szCmdLine++;
			switch(*szCmdLine) {
				case 'd':
					tSettings.bDebug = TRUE;
					tSettings.bPlayOnline = FALSE;
					break;
				case 'c':
					tSettings.bPlayOnline = TRUE;
					tSettings.bDebug = FALSE;
					break;
				case 'z':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPass);
					break;
				/*
				// We'll do this using ALT+ENTER
				case 'w':
					tSettings.bWindowedMode = TRUE;
					break;
				*/
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
				case 'l':
					szCmdLine++;
					SetStringFromQuotedCommandLine(szCmdLine,tSettings.szDebugScript);
					break;
			}
		}

		szCmdLine++;
	}
}

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString)
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

void SetStringFromQuotedCommandLine(char *szCmdLine, char *szString)
{
	while(*szCmdLine != '"') szCmdLine++;
	szCmdLine++;
	while(*szCmdLine &&
		  *szCmdLine != '"')
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}
	*szString = '\0';
}

//----------------------------------------------------
