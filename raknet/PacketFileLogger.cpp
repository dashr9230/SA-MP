#include "PacketFileLogger.h"
#include "GetTime.h"

PacketFileLogger::~PacketFileLogger()
{
	fclose(packetLogFile);
}
void PacketFileLogger::OnAttach(RakPeerInterface *peer)
{
	PacketLogger::OnAttach(peer);

	// Open file for writing
	char filename[256];
	sprintf(filename, "PacketLog%i.csv", RakNet::GetTime());
	packetLogFile = fopen(filename, "wt");
	LogHeader();
}

void PacketFileLogger::WriteLog(const char *str)
{
	fputs(str, packetLogFile);
}
