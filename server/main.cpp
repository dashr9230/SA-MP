
#include "main.h"

CConsole *pConsole = NULL;

FILE *pLogFile;


#ifdef WIN32
extern LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
#endif

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

	// Open the log file
	LoadLogFile();

	// Write welcome message.
	logprintf("");
	logprintf("SA-MP Dedicated Server");
	logprintf("----------------------");
	logprintf("v" SAMP_VERSION ", (C)2005-2015 SA-MP Team\n");

	srand(time(NULL));

	// Create the Console
	pConsole = new CConsole();

#ifdef WIN32
	// Setup the exception handler on windows
	SetUnhandledExceptionFilter(exc_handler);
#endif

	// TODO: main

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
