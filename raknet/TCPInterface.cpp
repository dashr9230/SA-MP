// TODO: Implement TCPInterface.cpp

#include "TCPInterface.h"

TCPInterface::TCPInterface()
{
}

Packet* TCPInterface::Receive( void )
{
	if (isStarted==false)
		return 0;
	return incomingMessages.ReadLock();
}
PlayerID TCPInterface::HasLostConnection(void)
{
	PlayerID *out;
	out = lostConnections.ReadLock();
	if (out)
	{
		lostConnections.ReadUnlock();
		return *out;
	}
	else
	{
		return UNASSIGNED_PLAYER_ID;
	}
}
