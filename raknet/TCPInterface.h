// TODO: Implement TCPInterface.h

#ifndef __SIMPLE_TCP_SERVER
#define __SIMPLE_TCP_SERVER

#include "NetworkTypes.h"
#include "SingleProducerConsumer.h"
#include "Export.h"

/// \internal
/// \brief As the name says, a simple multithreaded TCP server.  Used by TelnetTransport
class RAK_DLL_EXPORT TCPInterface
{
public:
	TCPInterface();

	/// Returns data received
	Packet* Receive( void );

	/// Queued events of lost connections
	PlayerID HasLostConnection(void);

protected:
	bool isStarted;

	DataStructures::SingleProducerConsumer<Packet> incomingMessages;

	DataStructures::SingleProducerConsumer<PlayerID> lostConnections;
};

#endif
