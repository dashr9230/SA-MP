
#include "main.h"
#include "game/util.h"

int				iGtaVersion=0;

GAME_SETTINGS			tSettings;

CHAR					szArtworkProxy[MAX_PATH+1];
CHAR					szConfigFile[MAX_PATH+1];
CHAR					byte_1026E710[256]; // unused
CHAR					szChatLogFile[MAX_PATH+1];
CHAR					szSAMPDir[MAX_PATH+1];
CHAR					szCacheDir[MAX_PATH+1];

CConfig					*pConfig=0;
CChatWindow				*pChatWindow=0;
CCmdWindow				*pCmdWindow=0;
CDeathWindow			*pDeathWindow=0;
CAudioStream			*pAudioStream=0;
//DWORD					dword_1026EB90=0;
CNetGame				*pNetGame=0;
//DWORD					dword_1026EB98=0;
CFontRender				*pDefaultFont=0;
//DWORD					dword_1026EBA0=0;
//DWORD					dword_1026EBA4=0;
//DWORD					dword_1026EBA8=0;

BOOL					bGameInited=FALSE;

IDirect3D9				*pD3D;
IDirect3DDevice9		*pD3DDevice	= NULL;

HANDLE			hInstance=0;

bool					bShowDebugLabels = false;

CGame					*pGame=0;
DWORD					dwGraphicsLoop=0;

CFileSystem *pFileSystem=NULL;

// forwards

BOOL SubclassGameWindow();
void TheGraphicsLoop();
LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
void sub_1009DD50();

// polls the game until it's able to run.
void LaunchMonitor(PVOID v)
{
	if(pGame)
		pGame->InitGame();

	while(1) {
		if(*(PDWORD)ADDR_ENTRY == 7) {
			sub_1009DD50();
			pGame->StartGame();
			break;
		}
		else {
			Sleep(5);
		}
	}

	ExitThread(0);
}

//----------------------------------------------------

#define ARCHIVE_FILE	"samp.saa"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
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

			InstallFileSystemHooks();
			InstallShowCursorHook();

			pGame = new CGame();

			_beginthread(LaunchMonitor,0,NULL);
		}
	}
	else if(DLL_PROCESS_DETACH==fdwReason)
	{
		if(tSettings.bDebug || tSettings.bPlayOnline) {
			UninstallFileSystemHooks();
		}
	}

	return TRUE;
}

//----------------------------------------------------

PCHAR GetSAMPPath()
{
	return szSAMPDir;
}

//----------------------------------------------------

PCHAR GetCachePath()
{
	return szCacheDir;
}

//----------------------------------------------------

void SetupCacheDirectories()
{
	ZeroMemory(szCacheDir, sizeof(szCacheDir));

	CHAR szPath[MAX_PATH+1];
	DWORD cbData = MAX_PATH;
	HKEY hKey;
	DWORD dwType;
	ZeroMemory(szPath, sizeof(szPath));

	sprintf(szCacheDir, "%s\\cache", szSAMPDir);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\SAMP", 0, KEY_READ, &hKey) == ERROR_SUCCESS &&
		RegQueryValueEx(hKey, "model_cache", NULL, &dwType, (LPBYTE) szPath, &cbData) == ERROR_SUCCESS)
	{
		strncpy(szCacheDir, szPath, MAX_PATH+1);
	}
	if(!IsFileOrDirectoryExists(szCacheDir))
		CreateDirectoryA(szCacheDir, NULL);

	CHAR szLocalDir[MAX_PATH+1];
	sprintf(szLocalDir, "%s\\local", szCacheDir);
	if(!IsFileOrDirectoryExists(szLocalDir))
		CreateDirectoryA(szLocalDir, NULL);
}

//----------------------------------------------------

PCHAR GetArtworkProxy()
{
	ZeroMemory(szArtworkProxy, sizeof(szArtworkProxy));

	DWORD cbData = MAX_PATH;
	HKEY hKey;
	DWORD dwType;

	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\SAMP", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return NULL;

	if(RegQueryValueEx(hKey, "artwork_proxy", NULL, &dwType, (LPBYTE) szArtworkProxy, &cbData) != ERROR_SUCCESS)
		return NULL;

	return szArtworkProxy;
}

//----------------------------------------------------

void SetupDirectories()
{
	ZeroMemory(szSAMPDir, sizeof(szSAMPDir));

	if(strlen((char*)0xC92368) == 0) {
		GetCurrentDirectory(MAX_PATH+1, szSAMPDir);
	} else {
		sprintf(szSAMPDir, "%s\\SAMP", (char*)0xC92368);
		if(!IsFileOrDirectoryExists(szSAMPDir))
			CreateDirectoryA(szSAMPDir, NULL);

		CHAR szScreensDir[MAX_PATH+1];
		sprintf(szScreensDir, "%s\\screens", szSAMPDir);
		if(!IsFileOrDirectoryExists(szScreensDir))
			CreateDirectoryA(szScreensDir, NULL);

		SetupCacheDirectories();
	}
}

//----------------------------------------------------

DWORD dwFogEnabled = 0;
DWORD dwFogColor = 0x00FF00FF;
BOOL gDisableAllFog = FALSE;

void SetupD3DFog(BOOL bEnable)
{
	float fFogStart = 500.0f;
	float fFogEnd = 700.0f;

	if(gDisableAllFog) bEnable = FALSE;

	if(pD3DDevice) {
		pD3DDevice->SetRenderState(D3DRS_FOGENABLE, bEnable);
		//pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, dwFogColor);
		pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		//pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fFogStart));
		//pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fFogEnd));
	}
}

//----------------------------------------------------

void CallRwRenderStateSet(int state, int option)
{
	_asm push option
	_asm push state
	_asm mov ebx, 0xC97B24
	_asm mov eax, [ebx]
	_asm call dword ptr [eax+32]
	_asm add esp, 8
}

//----------------------------------------------------

void CallRwRenderStateGet(int state, int *option)
{
	_asm push option
	_asm push state
	_asm mov ebx, 0xC97B24
	_asm mov eax, [ebx]
	_asm call dword ptr [eax+36]
	_asm add esp, 8
}

//----------------------------------------------------

void DoInitStuff()
{
	// GAME INIT
	if(!bGameInited)
	{
		SetupDirectories();

		sprintf(szConfigFile, "%s\\sa-mp.cfg", szSAMPDir);
		sprintf(szChatLogFile, "%s\\chatlog.txt", szSAMPDir);

		pConfig = new CConfig(szConfigFile);

		timeBeginPeriod(1); // increases the accuracy of Sleep()
		sub_1009DD50();
		SubclassGameWindow();

		// Grab the real IDirect3D9 * from the game.
		pD3D = (IDirect3D9 *)pGame->GetD3D();

		// Grab the real IDirect3DDevice9 * from the game.
		pD3DDevice = (IDirect3DDevice9 *)pGame->GetD3DDevice();

		// TODO: DoInitStuff

		bGameInited = TRUE;

		return;
	}

	
	
}

//----------------------------------------------------

void DoProcessStuff()
{
	DoInitStuff();
}

//----------------------------------------------------

void TheGraphicsLoop()
{
	_asm pushad // because we're called from a hook

	DoProcessStuff();

	_asm popad
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
