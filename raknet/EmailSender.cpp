// TODO: Implement EmailSender.cpp

#include "EmailSender.h"
#include "TCPInterface.h"
#include "GetTime.h"
#include <stdio.h>
#include <string.h> // strstr

#include "RakSleep.h"

char *EmailSender::GetResponse(TCPInterface *tcpInterface, const PlayerID &emailServer, bool doPrintf)
{
	Packet *packet;
	RakNetTime timeout;
	timeout=RakNet::GetTime()+5000;
	while (1)
	{
		if (tcpInterface->HasLostConnection()==emailServer)
			return "Connection to server lost.";
		packet = tcpInterface->Receive();
		if (packet)
		{
			if (doPrintf)
				printf("%s", packet->data);
			if (strstr((const char*)packet->data, "250"))
				return 0; // OK
			if (strstr((const char*)packet->data, "354"))
				return 0; // OK
			if (strstr((const char*)packet->data, "550"))
				return "Failed on error code 550";
			if (strstr((const char*)packet->data, "553"))
				return "Failed on error code 553";
		}
		if (RakNet::GetTime() > timeout)
			return "Timed out";
		RakSleep(100);
	}
}
