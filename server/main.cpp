
#include "main.h"

CNetGame		*pNetGame	= NULL;
CConsole		*pConsole	= NULL;
CPlugins		*pPlugins	= NULL;

PLAYERID	RconUser=INVALID_ID;

FILE		*pLogFile;
bool		bQuitApp = false;
BOOL		bGameModeFinished=FALSE;

bool bLogQueries;
int iMaxNpc = 0;
int iMinConnectionTime;
int iDbLogging;
int iDbLogQueries;

int unnamed_3; // W: 004F5FEC L: 081AA8A4
int unnamed_4; // W: 004F5FF0 L: 081AA8A0
unsigned int _uiRndSrvChallenge;

int iOnFootRate = 30;
int iInCarRate = 30;
int iWeaponRate = 30;
int unnamed_1 = 2; // TODO: unnamed_1 W: 004E5888 L: 08197DDC
float fStreamDistance = 200.0;
int iStreamRate = 1000;
int iSleep = 5;
int iChatLogging = 1;
int iMessageHoleLimit = 3000;
int iMessagesLimit = 500;
int iAcksLimit = 3000;
int iPlayerTimeout = 10000;
int iLagCompMode = 1;
int iConnSeedTime = 300000;
int iConnCookies = 1;
int iCookieLogging = 1;

#ifdef WIN32
extern LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
#endif

#ifdef WIN32
HANDLE hConsoleExecuteEvent;
DWORD WINAPI ConsoleInputThread(void* pParam)
{
	char buf[512];
	while (true)
	{
		DWORD dwRead;
		ReadConsole(GetStdHandle(STD_INPUT_HANDLE), buf, 255, &dwRead, NULL);
		if (dwRead > 2)
		{
			buf[dwRead-2] = 0;
			WaitForSingleObject(hConsoleExecuteEvent, INFINITE);
			flogprintf("Console input: %s", buf);
			pConsole->Execute(buf);
			SetEvent(hConsoleExecuteEvent);
		}
	}
}

//----------------------------------------------------

BOOL WINAPI CtrlHandler(DWORD type)
{
	switch (type)
	{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			bQuitApp = true;
			return TRUE;
	}
	return FALSE;
}

#endif

//----------------------------------------------------

int GetMessageHoleLimit()
{
	return iMessageHoleLimit;
}

//----------------------------------------------------

int GetMessagesLimit()
{
	return iMessagesLimit;
}

//----------------------------------------------------

int GetAcksLimit()
{
	return iAcksLimit;
}

//----------------------------------------------------

int GetPlayerTimeout()
{
	return iPlayerTimeout;
}

//----------------------------------------------------

int GetMinConnectionTime()
{
	return iMinConnectionTime;
}

//----------------------------------------------------

int GetConnCookies()
{
	return iConnCookies;
}

//----------------------------------------------------

int GetCookieLogging()
{
	return iCookieLogging;
}

//----------------------------------------------------

void ServerPasswordChanged()
{
	if (pNetGame)
	{
		char* szPass = pConsole->GetStringVariable("password");
		if ((szPass) && (szPass[0] != 0) && (szPass[0] != '0'))
		{
			logprintf("Setting server password to: \"%s\"", szPass);
			pNetGame->GetRakServer()->SetPassword(szPass);
		} else {
			logprintf("Server password has been removed.");
			pConsole->SetStringVariable("password", "");
			pNetGame->GetRakServer()->SetPassword(NULL);
		}
	}
}

//----------------------------------------------------

void ServerMaxPlayersChanged()
{
	int maxplayers = pConsole->GetIntVariable("maxplayers");
	if (maxplayers < 0)
		pConsole->SetIntVariable("maxplayers", 0);
	if (maxplayers > MAX_PLAYERS)
		pConsole->SetIntVariable("maxplayers", MAX_PLAYERS);

	if (pNetGame)
	{
		if (pConsole->GetIntVariable("maxplayers") > MAX_PLAYERS) {
			pConsole->SetIntVariable("maxplayers", MAX_PLAYERS);
		}
		pNetGame->GetRakServer()->SetAllowedPlayers((WORD)pConsole->GetIntVariable("maxplayers"));
	}
}

//----------------------------------------------------

void ServerStreamRateChanged()
{
	int stream_rate = pConsole->GetIntVariable("stream_rate");
	if (stream_rate < 500)
		pConsole->SetIntVariable("stream_rate", 500);
	if ( stream_rate > 5000)
		pConsole->SetIntVariable("stream_rate", 5000);
}

//----------------------------------------------------

void ServerStreamDistanceChanged()
{
	float stream_distance = pConsole->GetFloatVariable("stream_distance");
	if (stream_distance < 50.0f)
		pConsole->SetFloatVariable("stream_distance", 50.0f);
	if (stream_distance > 400.0f)
		pConsole->SetFloatVariable("stream_distance", 400.0f);
}

//----------------------------------------------------

void LoadLogFile()
{
	int reload = 0;
	if (pLogFile)
	{
		fclose(pLogFile);
		reload = 1;
	}
	pLogFile = fopen("server_log.txt", "a");
	if (pLogFile)
	{
		logprintf("");
		logprintf("----------");
		if (reload) logprintf("Reloaded log file: \"server_log.txt\".");
		else logprintf("Loaded log file: \"server_log.txt\".");
		logprintf("----------");
	} else {
		logprintf("Failed to load log file: \"server_log.txt\".");
	}
}

#ifdef LINUX

void SignalHandler(int sig)
{
	switch (sig)
	{
		case SIGUSR1:
		{
			LoadLogFile();
			break;
		}
		case SIGUSR2:
		{
			if (pNetGame)
			{
				pNetGame->LoadBanList();
			}
			break;
		}
		case SIGINT:
		case SIGTERM:
		{
			bQuitApp = true;
			break;
		}
	}
}

//----------------------------------------------------

// strlwr is not included with the GNU C lib it seems.
char* strlwr(char* str)
{
	size_t maxlen=strlen(str);
	for (size_t i=0; i<maxlen; i++)
	{
		if ((str[i] >= 'A') && (str[i] <= 'Z'))
		{
			str[i] -= 32;
		}
	}
	return str;
}

#endif	// #ifdef LINUX

//----------------------------------------------------

int main (int argc, char** argv)
{
#ifdef LINUX
	struct sigaction sv;
	sigemptyset(&sv.sa_mask);
	sv.sa_flags = 0;
	sv.sa_handler = SignalHandler;
	sigaction(SIGTERM, &sv, NULL);
	sigaction(SIGQUIT, &sv, NULL);
	sigaction(SIGINT, &sv, NULL);
	sigaction(SIGUSR1, &sv, NULL);
	sigaction(SIGUSR2, &sv, NULL);
	bool bOutputEnable = false;
#endif

	bool bEnableAnnounce = true;
	int iMaxPlayers	= DEFAULT_MAX_PLAYERS;
	int iListenPort	= DEFAULT_LISTEN_PORT;
	bool bLanModeEnable = false;
	bool bEnableTimestamp = true;
	bool bGameMod = false;
	bool bAllowQuery = true;
	bool bAllowRcon = true;

	// Open the log file
	LoadLogFile();

	// Write welcome message.
	logprintf("");
	logprintf("SA-MP Dedicated Server");
	logprintf("----------------------");
	logprintf("v" SAMP_VERSION ", (C)2005-2019 SA-MP Team\n");

	// Create a challenge number for the clients to be able to connect
	srand(time(NULL));
	_uiRndSrvChallenge = (unsigned int)rand();
	unnamed_3 = rand();
	unnamed_4 = RakNet::GetTime();

	// Create the Console
	pConsole = new CConsole();

	pConsole->AddVariable("announce", CON_VARTYPE_BOOL, 0, &bEnableAnnounce);
	pConsole->AddVariable("maxplayers", CON_VARTYPE_INT, 0, &iMaxPlayers, ServerMaxPlayersChanged);
	pConsole->AddVariable("port", CON_VARTYPE_INT, 0, &iListenPort);
	pConsole->AddVariable("lanmode", CON_VARTYPE_BOOL, 0, &bLanModeEnable);
	pConsole->AddVariable("query", CON_VARTYPE_BOOL, 0, &bAllowQuery);
	pConsole->AddVariable("rcon", CON_VARTYPE_BOOL, 0, &bAllowRcon);
	pConsole->AddVariable("logqueries", CON_VARTYPE_BOOL, 0, &bLogQueries);
	pConsole->AddVariable("stream_rate", CON_VARTYPE_INT, 0, &iStreamRate, ServerStreamRateChanged);
	pConsole->AddVariable("stream_distance", CON_VARTYPE_FLOAT, 0, &fStreamDistance, ServerStreamDistanceChanged);
	pConsole->AddVariable("sleep", CON_VARTYPE_INT, 0, &iSleep);
	pConsole->AddVariable("maxnpc", CON_VARTYPE_INT, 0, &iMaxNpc);
	pConsole->AddVariable("onfoot_rate", CON_VARTYPE_INT, 0, &iOnFootRate);
	pConsole->AddVariable("incar_rate", CON_VARTYPE_INT, 0, &iInCarRate);
	pConsole->AddVariable("weapon_rate", CON_VARTYPE_INT, 0, &iWeaponRate);
	pConsole->AddVariable("chatlogging", CON_VARTYPE_INT, 0, &iChatLogging);

#ifdef LINUX
	pConsole->AddVariable("output", CON_VARTYPE_BOOL, 0, &bOutputEnable);
#endif

	pConsole->AddVariable("timestamp", CON_VARTYPE_BOOL, 0, &bEnableTimestamp);
	pConsole->AddStringVariable("bind", 0, NULL);
	pConsole->AddStringVariable("password", 0, NULL, ServerPasswordChanged);
	pConsole->AddStringVariable("hostname", 0, "SA-MP Server");
	pConsole->AddStringVariable("language", 0, NULL);
	pConsole->AddStringVariable("mapname", CON_VARFLAG_RULE, "San Andreas");
	pConsole->AddStringVariable("weburl", CON_VARFLAG_RULE, "www.sa-mp.com");
	pConsole->AddStringVariable("rcon_password", 0, "changeme");
	pConsole->AddStringVariable("gravity", 0, "0.008");
	pConsole->AddStringVariable("weather", CON_VARFLAG_RULE, "10");
	pConsole->AddStringVariable("gamemodetext", 0, "Unknown");
	pConsole->AddStringVariable("filterscripts", 0, "");
	pConsole->AddStringVariable("plugins", 0, "");
	pConsole->AddStringVariable("nosign", 0, "");
	pConsole->AddStringVariable("logtimeformat", 0, "[%H:%M:%S]");
	pConsole->AddVariable("messageholelimit", CON_VARTYPE_INT, 0, &iMessageHoleLimit);
	pConsole->AddVariable("messageslimit", CON_VARTYPE_INT, 0, &iMessagesLimit);
	pConsole->AddVariable("ackslimit", CON_VARTYPE_INT, 0, &iAcksLimit);
	pConsole->AddVariable("playertimeout", CON_VARTYPE_INT, 0, &iPlayerTimeout);
	pConsole->AddVariable("minconnectiontime", CON_VARTYPE_INT, 0, &iMinConnectionTime);
	pConsole->AddVariable("myriad", CON_VARTYPE_BOOL, 0, &bGameMod);
	pConsole->AddVariable("lagcompmode", CON_VARTYPE_INT, 0, &iLagCompMode);
	pConsole->AddVariable("connseedtime", CON_VARTYPE_INT, 0, &iConnSeedTime);
	pConsole->AddVariable("db_logging", CON_VARTYPE_INT, 0, &iDbLogging);
	pConsole->AddVariable("db_log_queries", CON_VARTYPE_INT, 0, &iDbLogQueries);
	pConsole->AddVariable("conncookies", CON_VARTYPE_INT, 0, &iConnCookies);
	pConsole->AddVariable("cookielogging", CON_VARTYPE_INT, 0, &iCookieLogging);

	// Add 16 gamemode variables.
	int x=0;
	char t[64];
	while(x!=16) {
		sprintf(t,"gamemode%u",x);
		pConsole->AddStringVariable(t,0,"");
		x++;
	}

	// Exec the server config!
	pConsole->Execute("exec server");

	if ( !strcmp(pConsole->GetStringVariable("rcon_password"), "changeme" ) )
	{
		logprintf("Error: Your password must be changed from the default password, please change it.");
		return 0;
	}

	// Change some var flags to read-only (can only be accessed from server.cfg).
	pConsole->ModifyVariableFlags("maxplayers", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("bind", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("port", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("rcon_bind", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("rcon_port", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("filterscripts", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("plugins", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("nosign", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("onfoot_rate", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("incar_rate", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("weapon_rate", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("logtimeformat", CON_VARFLAG_READONLY);
	pConsole->ModifyVariableFlags("lagcompmode", CON_VARFLAG_READONLY);

	if(pConsole->GetIntVariable("lagcompmode") > 0)
		pConsole->AddStringVariable("lagcomp", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, "On");
	else
		pConsole->AddStringVariable("lagcomp", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, "Off");

	// Add the version as a rule
	pConsole->AddStringVariable("version", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, SAMP_VERSION);

#ifdef WIN32
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	hConsoleExecuteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsoleInputThread, NULL, 0, &dwThreadId);

	// Setup the exception handler on windows
	SetUnhandledExceptionFilter(exc_handler);
	timeBeginPeriod(1); // increases the accuracy of Sleep()
#endif

	// Load up the plugins
	pPlugins = new CPlugins();
	pPlugins->LoadPlugins("plugins");

	// Create the NetGame.
	pNetGame = new CNetGame();
	pNetGame->Init(true);

	// Start the rcon server
	PCHAR szBindAddress = pConsole->GetStringVariable("rcon_bind");
	if (!szBindAddress || szBindAddress[0] == 0)
		szBindAddress = pConsole->GetStringVariable("bind");
	if (!szBindAddress || szBindAddress[0] == 0)
		szBindAddress = NULL;

	// While the app is running...
	while (!bQuitApp)
	{
		pNetGame->Process();

		if(TRUE == bGameModeFinished) {
			pNetGame->ShutdownForGameModeRestart();
			bGameModeFinished = FALSE;
		}

		#ifdef WIN32
			SetEvent(hConsoleExecuteEvent);
			WaitForSingleObject(hConsoleExecuteEvent, INFINITE);
		#endif

		if(RakNet::GetTime() - unnamed_4 > iConnSeedTime)
		{
			unnamed_3 = rand();
			unnamed_4 = RakNet::GetTime();
		}

		SLEEP(iSleep);
	}

	delete pNetGame;

	delete pPlugins;

	// If WIN32: Kill the input thread.
	#ifdef WIN32
		TerminateThread(hThread, 0);
		CloseHandle(hConsoleExecuteEvent);
	#endif

	delete pConsole;

	fclose(pLogFile);

	return 0;
}

//----------------------------------------------------

void logprintf(char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	char buffer[2048];
	vsprintf(buffer, format, ap);

	va_end(ap);

#ifdef WIN32
	char output[2048];
	CharToOem(buffer, output);
	puts(output);
	fflush(stdout);
#else
	if (pConsole && pConsole->GetBoolVariable("output"))
	{
		puts(buffer);
	}
#endif
	if (pLogFile) {
		if (pConsole) {
			if (pConsole->GetBoolVariable("timestamp"))
			{
				const struct tm *tm;
				time_t now;
				now = time(NULL);
				tm = localtime(&now);
				char s[256];
				strftime(s, 256, pConsole->GetStringVariable("logtimeformat"), tm);
				fprintf(pLogFile, "%s %s\n", s, buffer);
			}
			else fprintf(pLogFile, "%s\n", buffer);
		}
		else fprintf(pLogFile, "%s\n", buffer);
		fflush(pLogFile);
	}

	// TODO: logprintf
}

//----------------------------------------------------

// Print to log file only.
void flogprintf(char* format, ...)
{
	if (!pLogFile) return;
	va_list ap;
	va_start(ap, format);
	char buffer[512];
	vsprintf(buffer, format, ap);
	fprintf(pLogFile, "%s\n", buffer);
	fflush(pLogFile);
	va_end(ap);
}

//----------------------------------------------------
