
#include "main.h"

GAME_SETTINGS	tSettings;
HANDLE			hInstance=0;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(DLL_PROCESS_ATTACH==fdwReason)
	{
		hInstance = hinstDLL;
		InitSettings();

		if(tSettings.bDebug || tSettings.bPlayOnline)
		{
			// TODO: DllMain
			/*
			SetUnhandledExceptionFilter(TopLevelExceptionFilter);
			dword_1026EB3C = (int)sub_100C4FF0;
			GetModuleFileNameA((HMODULE)hInstance, &Filename, 0x104u);
			v3 = strlen(&Filename);
			if ( *(&Filename + v3) != 92 )
			{
			do
			v4 = *((_BYTE *)&v12 + v3-- + 3);
			while ( v4 != 92 );
			}
			v5 = &v15[v3];
			*(_DWORD *)v5 = 1886216563;
			*((_DWORD *)v5 + 1) = 1633776430;
			v5[8] = 0;
			v6 = operator new(0x935u);
			v12 = v6;
			v16 = 0;
			if ( v6 )
			v7 = (int *)sub_10065350(v6);
			else
			v7 = 0;
			dword_1026EB44 = (int)v7;
			v8 = *v7;
			v16 = -1;
			if ( !(unsigned __int8)(*(int (__thiscall **)(int *, int *))(v8 + 4))(v7, &dword_100EDB18) )
			__debugbreak();*/

			AddFontResourceA("gtaweap3.ttf");
			AddFontResourceA("sampaux3.ttf");

			/*sub_10062CA0();
			sub_10062970();
			v9 = operator new(0x142u);
			v13 = v9;
			v16 = 1;
			if ( v9 )
			v10 = sub_1009FF80(v9);
			else
			v10 = 0;
			dword_1026EBAC = v10;
			_beginthread((int)sub_100C3A80, 0, 0);
			*/
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
