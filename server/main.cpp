
#include "main.h"

FILE *pLogFile;

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
