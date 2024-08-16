/// \file
/// \brief A simple TCP based server allowing sends and receives.  Can be connected by any TCP client, including telnet.
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.rakkarsoft.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __SIMPLE_TCP_SERVER
#define __SIMPLE_TCP_SERVER

#ifdef _COMPATIBILITY_1
#include "Compatibility1Includes.h"
#elif defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h> // fd_set
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
/// Unix/Linux uses ints for sockets
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#include "DS_List.h"
#include "NetworkTypes.h"
#include "SingleProducerConsumer.h"
#include "Export.h"

struct RemoteClient;

/// \internal
/// \brief As the name says, a simple multithreaded TCP server.  Used by TelnetTransport
class RAK_DLL_EXPORT TCPInterface
{
public:
	TCPInterface();
	~TCPInterface();

	/// Starts the TCP server on the indicated port
	bool Start(unsigned short port, unsigned short maxIncomingConnections);

	/// Stops the TCP server
	void Stop(void);

	/// Connect to the specified host on the specified port
	PlayerID Connect(const char* host, unsigned short remotePort);

	/// Sends a byte stream
	void Send( const char *data, unsigned length, PlayerID playerId );

	/// Returns data received
	Packet* Receive( void );

	/// Disconnects a player/address
	void CloseConnection( PlayerID playerId );

	/// Deallocates a packet returned by Receive
	void DeallocatePacket( Packet *packet );

	/// Queued events of new connections
	PlayerID HasNewConnection(void);

	/// Queued events of lost connections
	PlayerID HasLostConnection(void);
protected:

	bool waitForClient;
	bool isStarted, threadRunning;
	SOCKET listenSocket;

	// Assuming remoteClients is only used by one thread!
	DataStructures::List<RemoteClient*> remoteClients;
	DataStructures::SingleProducerConsumer<Packet> outgoingMessages, incomingMessages;
	DataStructures::SingleProducerConsumer<PlayerID> newConnections, lostConnections, requestedCloseConnections;
	DataStructures::SingleProducerConsumer<RemoteClient*> newRemoteClients;

#ifdef _WIN32
	HANDLE threadHandle;
	friend unsigned __stdcall UpdateTCPInterfaceLoop( LPVOID arguments );
#else
	pthread_t threadHandle;
	friend void* UpdateTCPInterfaceLoop( void* arguments );
#endif

	void DeleteRemoteClient(RemoteClient *remoteClient, fd_set *exceptionFD);
};

/// Stores information about a remote client.  In this case, only the socket used by that client.
struct RemoteClient
{
	SOCKET socket;
	PlayerID playerId;
};

#endif
