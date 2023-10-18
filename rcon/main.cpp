#include "main.h"

//--------------------------------------------------

bool bQuitApp = false;
HANDLE hConsoleExecuteEvent;
SOCKET s		= INVALID_SOCKET;
char* szHost	= NULL;
WORD wPort		= 0;
char* szPassword= NULL;
sockaddr_in to;
BYTE ipblock[4]	= { 0 };

//--------------------------------------------------

void logprintf(char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char buffer[512];
	vsprintf(buffer, format, ap);
	va_end(ap);
	puts(buffer);
}

//--------------------------------------------------

BOOL WINAPI CtrlHandler(DWORD dwType)
{
	switch (dwType)
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

//--------------------------------------------------

DWORD WINAPI NetworkPumpThread(void* pParam)
{
	int to_size = sizeof(to);
	char buf[1024];
	int len;
	while (true)
	{
		Sleep(100);

		len = recvfrom(s, buf, sizeof(buf), 0, (sockaddr*)&to, &to_size);
		if (len > 13)
		{
			char* buf_ptr = &buf[11];
			WORD wStrLen = *(WORD*)buf_ptr;
			buf_ptr += sizeof(WORD);
			char* szMsg = (char*)malloc(wStrLen + 1);
			memcpy(szMsg, buf_ptr, wStrLen);
			szMsg[wStrLen] = 0;
			logprintf("%s", szMsg);
			free(szMsg);
		}
	}
	return 0;
}

//--------------------------------------------------

void SendCommand(char* szCommand);

//--------------------------------------------------

DWORD WINAPI ConsoleInputThread(void* pParam)
{
	while (true)
	{
		Sleep(100);

		char buf[512];
		DWORD dwRead;
		ReadConsole(GetStdHandle(STD_INPUT_HANDLE), buf, 512, &dwRead, NULL);
		if (dwRead > 2)
		{
			buf[dwRead-2] = 0;
			SendCommand(buf);
		}
	}
	return 0;
}

//--------------------------------------------------

void SendCommand(char* szCommand)
{
	char* newdata = (char*)malloc(11 + strlen(szCommand) + strlen(szPassword) + (sizeof(WORD) * 2));
	char* keep_ptr = newdata;
	*(DWORD*)newdata = 0x504D4153;
	newdata += sizeof(DWORD);
	*(BYTE*)newdata = ipblock[0];
	newdata += sizeof(BYTE);
	*(BYTE*)newdata = ipblock[1];
	newdata += sizeof(BYTE);
	*(BYTE*)newdata = ipblock[2];
	newdata += sizeof(BYTE);
	*(BYTE*)newdata = ipblock[3];
	newdata += sizeof(BYTE);
	*(WORD*)newdata = wPort;
	newdata += sizeof(WORD);
	*newdata = 'x';
	newdata++;

	WORD wStrLen = (WORD)strlen(szPassword);
	*(WORD*)newdata = wStrLen;
	newdata += sizeof(WORD);
	memcpy(newdata, szPassword, wStrLen);
	newdata += wStrLen;

	wStrLen = (WORD)strlen(szCommand);
	*(WORD*)newdata = wStrLen;
	newdata += sizeof(WORD);
	memcpy(newdata, szCommand, wStrLen);
	newdata += wStrLen;

	sendto(s, keep_ptr, (int)(newdata - keep_ptr), 0, (sockaddr*)&to, sizeof(to));
	free(keep_ptr);
}

//--------------------------------------------------

int main (int argc, char** argv)
{
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);

	// Print welcome message.
	puts("\n SA:MP Command Line Remote Console Client");
	puts(" ----------------------------------------");
	puts(" (C) Copyright 2005-2006 SA:MP Team, v1.0\n");

	// If there's not enough command line args, print usage.
	if (argc < 3)
	{
		puts(" Usage:");
		puts("   rcon <ip/host> <port> <password>");
		exit(0);
	}

	char* szHost = argv[1];

	hostent* he = gethostbyname(szHost);
	if (he)
	{
		in_addr in;
		memcpy(&in, he->h_addr, he->h_length);
		szHost = inet_ntoa(in);
	}

	int ip = inet_addr(szHost);
	if (ip == -1)
	{
		logprintf("ERROR: Bad host.");
		return 0;
	}

	sscanf(szHost, "%d.%d.%d.%d", &ipblock[0], &ipblock[1], &ipblock[2], &ipblock[3]);
	wPort = (WORD)atoi(argv[2]);
	szPassword = argv[3];
	logprintf("Remote Console: %d.%d.%d.%d:%d...\n", ipblock[0], ipblock[1], ipblock[2], ipblock[3], wPort);

	to.sin_family = AF_INET;
	to.sin_port = htons(wPort);
	to.sin_addr.s_addr = inet_addr(szHost);

	s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	sockaddr_in s_in;
	s_in.sin_addr.S_un.S_addr = INADDR_ANY;
	s_in.sin_family = AF_INET;
	s_in.sin_port = 0;
	bind(s, (sockaddr*)&s_in, sizeof(s_in));

	// Setup the console
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	DWORD dwThreadId;
	HANDLE hConsoleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsoleInputThread, NULL, 0, &dwThreadId);
	HANDLE hNetworkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NetworkPumpThread, NULL, 0, &dwThreadId);

	SendCommand("echo RCON admin connected to server.");

	// While not wanting to quit...
	while (!bQuitApp)
	{
		Sleep(100);
	}

	return 0;
}

//--------------------------------------------------
// EOF