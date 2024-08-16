/// \file
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

#include "FullyConnectedMesh.h"
#include "RakPeerInterface.h"
#include "PacketEnumerations.h"
#include "BitStream.h"
#include "ConnectionGraph.h"
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER
#pragma warning( push )
#endif

FullyConnectedMesh::FullyConnectedMesh()
{
	pw=0;
}

FullyConnectedMesh::~FullyConnectedMesh()
{
	if (pw)
		delete [] pw;
}

void FullyConnectedMesh::Initialize(const char *password)
{
	if (pw)
		delete [] pw;
	if (password && password[0])
	{
		pw = new char [strlen(password)+1];
		strcpy(pw, password);
	}
	else
		pw=0;
	
}

#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void FullyConnectedMesh::OnDisconnect(RakPeerInterface *peer)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4701 ) // warning C4701: local variable may be used without having been initialized
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void FullyConnectedMesh::Update(RakPeerInterface *peer)
{
}

PluginReceiveResult FullyConnectedMesh::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	assert(packet);
	assert(peer);

	switch (packet->data[0])
	{
	case ID_REMOTE_NEW_INCOMING_CONNECTION: // This comes from the connection graph plugin
		{
			RakNet::BitStream b(packet->data, packet->length, false);
			b.IgnoreBits(8);
			ConnectionGraphGroupID group1, group2;
			PlayerID node1, node2;
			b.Read(node1);
			b.Read(group1);
			if (peer->GetIndexFromPlayerID(node1)==-1)
				peer->Connect(peer->PlayerIDToDottedIP(node1), node1.port, pw, pw ? (int)strlen(pw) : 0);
			b.Read(node2);
			b.Read(group2);
			if (peer->GetIndexFromPlayerID(node2)==-1)
				peer->Connect(peer->PlayerIDToDottedIP(node2), node2.port, pw, pw ? (int)strlen(pw) : 0);
			break;
		}
	}

	return RR_CONTINUE_PROCESSING;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
