
#include "main.h"

CConsole *pConsole = NULL;

FILE *pLogFile;

bool bLogQueries;
int iMaxNpc = 0;
int iMinConnectionTime;
int iDbLogging;
int iDbLogQueries;

int unnamed_2; // W: 004F5FE8 L: 081AA8A8
int unnamed_3; // W: 004F5FEC L: 081AA8A4
int unnamed_4; // W: 004F5FF0 L: 081AA8A0

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

int sub_44E9D0()
{
	// TODO: sub_44E9D0 W: 0044E9D0 L: 0805FBE0
	return 0;
}

//----------------------------------------------------

void ServerPasswordChanged()
{
	// TODO: ServerPasswordChanged
}

//----------------------------------------------------

void ServerMaxPlayersChanged()
{
	// TODO: ServerMaxPlayersChanged
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
	logprintf("v" SAMP_VERSION ", (C)2005-2015 SA-MP Team\n");

	srand(time(NULL));
	unnamed_2 = rand();
	unnamed_3 = rand();
	unnamed_4 = sub_44E9D0();

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
	pConsole->AddStringVariable("mapname", 4, "San Andreas");
	pConsole->AddStringVariable("weburl", 4, "www.sa-mp.com");
	pConsole->AddStringVariable("rcon_password", 0, "changeme");
	pConsole->AddStringVariable("gravity", 0, "0.008");
	pConsole->AddStringVariable("weather", 4, "10");
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

	if ( !memcmp(pConsole->GetStringVariable("rcon_password"), "changeme", 9 ) )
	{
		logprintf("Error: Your password must be changed from the default password, please change it.");
		return 0;
	}

	// Change some var flags to read-only (can only be accessed from server.cfg).
	pConsole->ModifyVariableFlags("maxplayers", 2);
	pConsole->ModifyVariableFlags("bind", 2);
	pConsole->ModifyVariableFlags("port", 2);
	pConsole->ModifyVariableFlags("rcon_bind", 2);
	pConsole->ModifyVariableFlags("rcon_port", 2);
	pConsole->ModifyVariableFlags("filterscripts", 2);
	pConsole->ModifyVariableFlags("plugins", 2);
	pConsole->ModifyVariableFlags("nosign", 2);
	pConsole->ModifyVariableFlags("onfoot_rate", 2);
	pConsole->ModifyVariableFlags("incar_rate", 2);
	pConsole->ModifyVariableFlags("weapon_rate", 2);
	pConsole->ModifyVariableFlags("logtimeformat", 2);
	pConsole->ModifyVariableFlags("lagcompmode", 2);

	if(pConsole->GetIntVariable("lagcompmode") <= 0)
		pConsole->AddStringVariable("lagcomp", 6, "Off");
	else
		pConsole->AddStringVariable("lagcomp", 6, "On");

	// Add the version as a rule
	pConsole->AddStringVariable("version", 6, SAMP_VERSION);

#ifdef WIN32
	// Setup the exception handler on windows
	SetUnhandledExceptionFilter(exc_handler);
#endif

	// TODO: main

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
#endif

	// TODO: logprintf
}

//----------------------------------------------------
