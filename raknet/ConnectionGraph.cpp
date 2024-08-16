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

#include "ConnectionGraph.h"
#include "RakPeerInterface.h"
#include "PacketEnumerations.h"
#include "BitStream.h"
#include "StringCompressor.h"
#include "GetTime.h"
#include <string.h>
#include "RakAssert.h"
#include "SHA1.h"

#ifdef _MSC_VER
#pragma warning( push )
#endif

extern Packet *AllocPacket(unsigned dataSize);

static const int connectionGraphChannel=31;

ConnectionGraph::PlayerIdAndGroupId::PlayerIdAndGroupId()
{

}
ConnectionGraph::PlayerIdAndGroupId::~PlayerIdAndGroupId()
{

}
ConnectionGraph::PlayerIdAndGroupId::PlayerIdAndGroupId(PlayerID playerId, ConnectionGraphGroupID groupID)
{
	this->playerId=playerId;
	this->groupId=groupId;
}
bool ConnectionGraph::PlayerIdAndGroupId::operator==( const ConnectionGraph::PlayerIdAndGroupId& right ) const
{
	return playerId==right.playerId;
}
bool ConnectionGraph::PlayerIdAndGroupId::operator!=( const ConnectionGraph::PlayerIdAndGroupId& right ) const
{
	return playerId!=right.playerId;
}
bool ConnectionGraph::PlayerIdAndGroupId::operator > ( const ConnectionGraph::PlayerIdAndGroupId& right ) const
{
	return playerId>right.playerId;
}
bool ConnectionGraph::PlayerIdAndGroupId::operator < ( const ConnectionGraph::PlayerIdAndGroupId& right ) const
{
	return playerId<right.playerId;
}
ConnectionGraph::ConnectionGraph()
{
    pw=0;
	myGroupId=0;
	autoAddNewConnections=true;
//	forceBroadcastTime=0;
	DataStructures::WeightedGraph<ConnectionGraph::PlayerIdAndGroupId, unsigned short, false>::IMPLEMENT_DEFAULT_COMPARISON();
	DataStructures::OrderedList<PlayerID, PlayerID>::IMPLEMENT_DEFAULT_COMPARISON();
	DataStructures::OrderedList<ConnectionGraph::PlayerIdAndGroupId, ConnectionGraph::PlayerIdAndGroupId>::IMPLEMENT_DEFAULT_COMPARISON();
	DataStructures::OrderedList<ConnectionGraphGroupID, ConnectionGraphGroupID>::IMPLEMENT_DEFAULT_COMPARISON();

	subscribedGroups.Insert(0,0);
}

ConnectionGraph::~ConnectionGraph()
{
	if (pw)
		delete [] pw;
}

void ConnectionGraph::SetPassword(const char *password)
{
	if (pw)
	{
		delete [] pw;
		pw=0;
	}

	if (password && password[0])
	{
		assert(strlen(password)<256);
		pw=new char [strlen(password)+1];
		strcpy(pw, password);
	}
}
DataStructures::WeightedGraph<ConnectionGraph::PlayerIdAndGroupId, unsigned short, false> *ConnectionGraph::GetGraph(void)
{
	return &graph;
}
void ConnectionGraph::SetAutoAddNewConnections(bool autoAdd)
{
	autoAddNewConnections=autoAdd;
}

#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void ConnectionGraph::OnDisconnect(RakPeerInterface *peer)
{
	graph.Clear();
	participantList.Clear();
//	forceBroadcastTime=0;
}
void ConnectionGraph::Update(RakPeerInterface *peer)
{
//	RakNetTime time = RakNet::GetTime();

	// If the time is past the next weight update time, then refresh all pings of all connected participants and send these out if substantially different.
//	if (forceBroadcastTime && time > forceBroadcastTime)
//	{
//		DataStructures::OrderedList<PlayerID,PlayerID> none;
	//	BroadcastGraphUpdate(none, peer);
//		forceBroadcastTime=0;
//	}
}
PluginReceiveResult ConnectionGraph::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	switch (packet->data[0]) 
	{
	case ID_NEW_INCOMING_CONNECTION:
		OnNewIncomingConnection(peer, packet);
		return RR_CONTINUE_PROCESSING;
	case ID_CONNECTION_REQUEST_ACCEPTED:
		OnConnectionRequestAccepted(peer, packet);
		return RR_CONTINUE_PROCESSING;
	case ID_CONNECTION_GRAPH_REQUEST:
		OnConnectionGraphRequest(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_CONNECTION_GRAPH_REPLY:
		OnConnectionGraphReply(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_CONNECTION_GRAPH_UPDATE:
		OnConnectionGraphUpdate(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_CONNECTION_GRAPH_NEW_CONNECTION:
		OnNewConnection(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
		// Remove connection lost
	case ID_CONNECTION_GRAPH_CONNECTION_LOST:
	case ID_CONNECTION_GRAPH_DISCONNECTION_NOTIFICATION:
		if (OnConnectionLost(peer, packet, packet->data[0]))
		{
			if (packet->data[0]==ID_CONNECTION_GRAPH_CONNECTION_LOST)
				packet->data[0]=ID_REMOTE_CONNECTION_LOST;
			else
				packet->data[0]=ID_REMOTE_DISCONNECTION_NOTIFICATION;
			return RR_CONTINUE_PROCESSING; // Return this packet to the user
		}		
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
		// Local connection lost
	case ID_CONNECTION_LOST:
	case ID_DISCONNECTION_NOTIFICATION:
		{
			unsigned char packetId;
			// Change toe remote connection lost and relay the message
			if (packet->data[0]==ID_CONNECTION_LOST)
				packetId=ID_CONNECTION_GRAPH_CONNECTION_LOST;
			else
				packetId=ID_CONNECTION_GRAPH_DISCONNECTION_NOTIFICATION;
			HandleDroppedConnection(peer, packet->playerId, packetId);
		}
	}

	return RR_CONTINUE_PROCESSING;
}
void ConnectionGraph::OnCloseConnection(RakPeerInterface *peer, PlayerID playerId)
{
	HandleDroppedConnection(peer, playerId, ID_CONNECTION_GRAPH_DISCONNECTION_NOTIFICATION);
}

void ConnectionGraph::HandleDroppedConnection(RakPeerInterface *peer, PlayerID playerId, unsigned char packetId)
{
	assert(peer);
	RemoveParticipant(playerId);
	DataStructures::OrderedList<PlayerID,PlayerID> ignoreList;
	RemoveAndRelayConnection(ignoreList, packetId, playerId, peer->GetExternalID(playerId), peer);
}

void ConnectionGraph::OnNewIncomingConnection(RakPeerInterface *peer, Packet *packet)
{
	if (autoAddNewConnections==false)
		return;

	// 0 is the default groupId
	AddNewConnection(peer, packet->playerId, 0);
}
void ConnectionGraph::OnConnectionRequestAccepted(RakPeerInterface *peer, Packet *packet)
{
	if (autoAddNewConnections==false)
		return;

	RequestConnectionGraph(peer, packet->playerId);

	// 0 is the default groupId
	AddNewConnection(peer, packet->playerId, 0);
}
void ConnectionGraph::SetGroupId(ConnectionGraphGroupID groupId)
{
	myGroupId=groupId;
}
void ConnectionGraph::AddNewConnection(RakPeerInterface *peer, PlayerID playerId, ConnectionGraphGroupID groupId)
{
	if (autoAddNewConnections==false && subscribedGroups.HasData(groupId)==false)
		return;

	DataStructures::OrderedList<PlayerID,PlayerID> ignoreList;
	
	PlayerIdAndGroupId first, second;
	first.playerId=playerId;
	first.groupId=groupId;
	second.playerId=peer->GetExternalID(playerId);
	second.groupId=myGroupId;

	AddAndRelayConnection(ignoreList, first, second, (unsigned short)peer->GetAveragePing(playerId), peer);
}
void ConnectionGraph::SubscribeToGroup(ConnectionGraphGroupID groupId)
{
	subscribedGroups.Insert(groupId, groupId);
}
void ConnectionGraph::UnsubscribeFromGroup(ConnectionGraphGroupID groupId)
{
	subscribedGroups.Remove(groupId);
}
void ConnectionGraph::RequestConnectionGraph(RakPeerInterface *peer, PlayerID playerId)
{
	RakNet::BitStream outBitstream;
	outBitstream.Write((unsigned char) ID_CONNECTION_GRAPH_REQUEST);
	stringCompressor->EncodeString(pw,256,&outBitstream);
	peer->Send(&outBitstream, LOW_PRIORITY, RELIABLE_ORDERED, connectionGraphChannel, playerId, false);

#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
	printf("ID_CONNECTION_GRAPH_REQUEST from %i to %i\n", peer->GetInternalID().port, playerId.port);
#endif
}
void ConnectionGraph::OnConnectionGraphRequest(RakPeerInterface *peer, Packet *packet)
{
	char password[256];
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	inBitstream.IgnoreBits(8);
	stringCompressor->DecodeString(password,256,&inBitstream);
	if (pw && pw[0] && strcmp(pw, password)!=0)
		return;

#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
	printf("ID_CONNECTION_GRAPH_REPLY ");
#endif

	RakNet::BitStream outBitstream;
	outBitstream.Write((unsigned char) ID_CONNECTION_GRAPH_REPLY);
	stringCompressor->EncodeString(pw,256,&outBitstream);
	SerializeWeightedGraph(&outBitstream, graph);
	peer->Send(&outBitstream, LOW_PRIORITY, RELIABLE_ORDERED, connectionGraphChannel, packet->playerId, false);

#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
	printf("from %i to %i\n", peer->GetInternalID().port, packet->playerId.port);
#endif

	// Add packet->playerId to the participant list if it is not already there
	AddParticipant(packet->playerId);
}
void ConnectionGraph::OnConnectionGraphReply(RakPeerInterface *peer, Packet *packet)
{
	unsigned char password[256];
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	inBitstream.IgnoreBits(8);
	stringCompressor->DecodeString((char*)password,256,&inBitstream);
	if (pw && pw[0] && strcmp(pw, (const char*)password)!=0)
		return;

	// Serialize the weighted graph and send it to them
	RakNet::BitStream outBitstream;
	outBitstream.Write((unsigned char) ID_CONNECTION_GRAPH_UPDATE);

#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
	printf("ID_CONNECTION_GRAPH_UPDATE ");
#endif

	// Send our current graph to the sender
	SerializeWeightedGraph(&outBitstream, graph);


	// Write the systems that have processed this graph so we don't resend to these systems
	outBitstream.Write((unsigned short) 1);
	outBitstream.Write(peer->GetExternalID(packet->playerId));

#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
	printf("from %i to %i\n", peer->GetInternalID().port, packet->playerId.port);
#endif

	peer->Send(&outBitstream, LOW_PRIORITY, RELIABLE_ORDERED, connectionGraphChannel, packet->playerId, false);

	// Add packet->playerId to the participant list if it is not already there
	AddParticipant(packet->playerId);

	if (DeserializeWeightedGraph(&inBitstream, peer)==false)
		return;

	// Forward the updated graph to all current participants
	DataStructures::OrderedList<PlayerID,PlayerID> ignoreList;
	ignoreList.Insert(packet->playerId,packet->playerId);
	BroadcastGraphUpdate(ignoreList, peer);
}
void ConnectionGraph::OnConnectionGraphUpdate(RakPeerInterface *peer, Packet *packet)
{
	// Only accept from participants
	if (participantList.HasData(packet->playerId)==false)
		return;

	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	inBitstream.IgnoreBits(8);
	
	if (DeserializeWeightedGraph(&inBitstream, peer)==false)
		return;

	DataStructures::OrderedList<PlayerID,PlayerID> ignoreList;
	DeserializeIgnoreList(ignoreList, &inBitstream);

	// Forward the updated graph to all participants.
	if (ignoreList.HasData(packet->playerId)==false)
		ignoreList.Insert(packet->playerId,packet->playerId);
	BroadcastGraphUpdate(ignoreList, peer);
}
void ConnectionGraph::OnNewConnection(RakPeerInterface *peer, Packet *packet)
{
	// Only accept from participants
	if (participantList.HasData(packet->playerId)==false)
		return;

	PlayerIdAndGroupId node1, node2;
	unsigned short ping;
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	inBitstream.IgnoreBits(8);
	inBitstream.Read(node1.playerId);
	inBitstream.Read(node1.groupId);
	inBitstream.Read(node2.playerId);
	inBitstream.Read(node2.groupId);
	if (inBitstream.Read(ping)==false)
		return;
	DataStructures::OrderedList<PlayerID,PlayerID> ignoreList;
	DeserializeIgnoreList(ignoreList, &inBitstream);
	if (ignoreList.HasData(packet->playerId)==false)
		ignoreList.Insert(packet->playerId,packet->playerId);
	AddAndRelayConnection(ignoreList, node1, node2, ping, peer);	
}
bool ConnectionGraph::OnConnectionLost(RakPeerInterface *peer, Packet *packet, unsigned char packetId)
{
	// Only accept from participants
	if (participantList.HasData(packet->playerId)==false)
		return false;

	PlayerID node1, node2;
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	inBitstream.IgnoreBits(8);
	// This is correct - group IDs are not written for removal, only addition.
	inBitstream.Read(node1);
	if (inBitstream.Read(node2)==false)
		return false;
	DataStructures::OrderedList<PlayerID,PlayerID> ignoreList;
	DeserializeIgnoreList(ignoreList, &inBitstream);
	if (ignoreList.HasData(packet->playerId)==false)
		ignoreList.Insert(packet->playerId, packet->playerId);
	
	return RemoveAndRelayConnection(ignoreList, packetId, node1, node2, peer);
}
bool ConnectionGraph::DeserializeIgnoreList(DataStructures::OrderedList<PlayerID,PlayerID> &ignoreList, RakNet::BitStream *inBitstream )
{
	unsigned short count;
	PlayerID temp;
	unsigned i;
	inBitstream->Read(count);
	for (i=0; i < count; i++)
	{
		if (inBitstream->Read(temp)==false)
		{
			assert(0);
			return false;
		}
		ignoreList.Insert(temp,temp);
	}
	return true;
}
void ConnectionGraph::SerializeWeightedGraph(RakNet::BitStream *out, const DataStructures::WeightedGraph<ConnectionGraph::PlayerIdAndGroupId, unsigned short, false> &g) const
{
	unsigned nodeIndex, connectionIndex;
	unsigned countOffset, oldOffset;
	unsigned short count;
	PlayerIdAndGroupId node1, node2;
	unsigned short weight;
	out->WriteCompressed(g.GetNodeCount());
	for (nodeIndex=0; nodeIndex < g.GetNodeCount(); nodeIndex++)
	{
		// Write the node
		node1=g.GetNodeAtIndex(nodeIndex);
#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
		printf("[%i] ", node1.playerId.port);
#endif
		out->Write(node1.playerId);
		out->Write(node1.groupId);

		// Write the adjacency list count
		count=(unsigned short)g.GetConnectionCount(nodeIndex);
		out->AlignWriteToByteBoundary();
		countOffset=out->GetWriteOffset();
		out->Write(count);
		count=0;
		for (connectionIndex=0; connectionIndex < g.GetConnectionCount(nodeIndex); connectionIndex++)
		{
			g.GetConnectionAtIndex(nodeIndex, connectionIndex, node2, weight);
			// For efficiencies' sake, only serialize the upper half of the connection pairs
			if (node2 > node1)
			{
				count++;
				out->Write(node2.playerId);
				out->Write(node2.groupId);
				out->Write(weight);

#ifdef _CONNECTION_GRAPH_DEBUG_PRINT
				printf("(%i) ", node2.playerId.port);
#endif
			}
		}

		// Go back and change how many elements were written
		oldOffset=out->GetWriteOffset();
		out->SetWriteOffset(countOffset);
		out->Write(count);
		out->SetWriteOffset(oldOffset);
	}
}
bool ConnectionGraph::DeserializeWeightedGraph(RakNet::BitStream *inBitstream, RakPeerInterface *peer)
{
	unsigned nodeCount, nodeIndex, connectionIndex;
	unsigned short connectionCount;
	PlayerIdAndGroupId node, connection;
	bool anyConnectionsNew=false;
	unsigned short weight;
	inBitstream->ReadCompressed(nodeCount);
	for (nodeIndex=0; nodeIndex < nodeCount; nodeIndex++)
	{
		inBitstream->Read(node.playerId);
		inBitstream->Read(node.groupId);

		inBitstream->AlignReadToByteBoundary();
		if (inBitstream->Read(connectionCount)==false)
		{
			assert(0);
			return false;
		}
		for (connectionIndex=0; connectionIndex < connectionCount; connectionIndex++)
		{
			inBitstream->Read(connection.playerId);
			inBitstream->Read(connection.groupId);
			if (inBitstream->Read(weight)==false)
			{
				assert(0);
				return false;
			}
			if (subscribedGroups.HasData(connection.groupId)==false ||
				subscribedGroups.HasData(node.groupId)==false)
				continue;
			RakAssert(node.playerId!=UNASSIGNED_PLAYER_ID);
			RakAssert(connection.playerId!=UNASSIGNED_PLAYER_ID);
			if (IsNewRemoteConnection(node,connection,peer))
				NotifyUserOfRemoteConnection(node,connection,weight,peer);

			if (graph.HasConnection(node,connection)==false)
				anyConnectionsNew=true;

			graph.AddConnection(node,connection,weight);
		}
	}
	return anyConnectionsNew;
}
void ConnectionGraph::RemoveParticipant(PlayerID playerId)
{
	unsigned index;
	bool objectExists;
	index=participantList.GetIndexFromKey(playerId, &objectExists);
	if (objectExists)
		participantList.RemoveAtIndex(index);
}

void ConnectionGraph::AddParticipant(PlayerID playerId)
{
	if (participantList.HasData(playerId)==false)
		participantList.Insert(playerId,playerId);
}

void ConnectionGraph::AddAndRelayConnection(DataStructures::OrderedList<PlayerID,PlayerID> &ignoreList, const PlayerIdAndGroupId &conn1, const PlayerIdAndGroupId &conn2, unsigned short ping, RakPeerInterface *peer)
{
	if (graph.HasConnection(conn1,conn2))
		return;

	if (ping==65535)
		ping=0;
	assert(conn1.playerId!=UNASSIGNED_PLAYER_ID);
	assert(conn2.playerId!=UNASSIGNED_PLAYER_ID);

	graph.AddConnection(conn1,conn2,ping);

	if (IsNewRemoteConnection(conn1,conn2,peer))
	{
		NotifyUserOfRemoteConnection(conn1,conn2,ping,peer);
		return;
	}

	RakNet::BitStream outBitstream;
	outBitstream.Write((unsigned char) ID_CONNECTION_GRAPH_NEW_CONNECTION);
	outBitstream.Write(conn1.playerId);
	outBitstream.Write(conn1.groupId);
	outBitstream.Write(conn2.playerId);
	outBitstream.Write(conn2.groupId);
	outBitstream.Write(ping);
	if (ignoreList.HasData(conn2.playerId)==false)
		ignoreList.Insert(conn2.playerId,conn2.playerId);
	if (ignoreList.HasData(conn1.playerId)==false)
		ignoreList.Insert(conn1.playerId,conn1.playerId);
	SerializeIgnoreListAndBroadcast(&outBitstream, ignoreList, peer);
}
bool ConnectionGraph::RemoveAndRelayConnection(DataStructures::OrderedList<PlayerID,PlayerID> &ignoreList, unsigned char packetId, const PlayerID node1, const PlayerID node2, RakPeerInterface *peer)
{
	PlayerIdAndGroupId n1, n2;
	n1.playerId=node1;
	n2.playerId=node2;
	if (graph.HasConnection(n1,n2)==false)
		return false;
	graph.RemoveConnection(n1,n2);

	RakNet::BitStream outBitstream;
	outBitstream.Write(packetId);
	outBitstream.Write(node1);
	outBitstream.Write(node2);

	if (ignoreList.HasData(node1)==false)
		ignoreList.Insert(node1,node1);
	if (ignoreList.HasData(node2)==false)
		ignoreList.Insert(node2,node2);
	SerializeIgnoreListAndBroadcast(&outBitstream, ignoreList, peer);

	return true;
}

void ConnectionGraph::BroadcastGraphUpdate(DataStructures::OrderedList<PlayerID,PlayerID> &ignoreList, RakPeerInterface *peer)
{
	RakNet::BitStream outBitstream;
	outBitstream.Write((unsigned char) ID_CONNECTION_GRAPH_UPDATE);
	SerializeWeightedGraph(&outBitstream, graph);
	SerializeIgnoreListAndBroadcast(&outBitstream, ignoreList, peer);
}
void ConnectionGraph::SerializeIgnoreListAndBroadcast(RakNet::BitStream *outBitstream, DataStructures::OrderedList<PlayerID,PlayerID> &ignoreList, RakPeerInterface *peer)
{
	DataStructures::List<PlayerID> sendList;
	unsigned i;
	for (i=0; i < participantList.Size(); i++)
	{
		if (ignoreList.HasData(participantList[i])==false)
			sendList.Insert(participantList[i]);
	}
	if (sendList.Size()==0)
		return;

	PlayerID self = peer->GetExternalID(sendList[0]);
	if (ignoreList.HasData(self)==false)
		ignoreList.Insert(self,self);
	outBitstream->Write((unsigned short) (ignoreList.Size()+sendList.Size()));
	for (i=0; i < ignoreList.Size(); i++)
		outBitstream->Write(ignoreList[i]);
	for (i=0; i < sendList.Size(); i++)
		outBitstream->Write(sendList[i]);

	for (i=0; i < sendList.Size(); i++)
	{
		peer->Send(outBitstream, LOW_PRIORITY, RELIABLE_ORDERED, connectionGraphChannel, sendList[i], false);
	}
}
bool ConnectionGraph::IsNewRemoteConnection(const PlayerIdAndGroupId &conn1, const PlayerIdAndGroupId &conn2,RakPeerInterface *peer)
{
	if (graph.HasConnection(conn1,conn2)==false &&
		subscribedGroups.HasData(conn1.groupId) &&
		subscribedGroups.HasData(conn2.groupId) &&
		(peer->GetIndexFromPlayerID(conn1.playerId)==-1 || peer->GetIndexFromPlayerID(conn2.playerId)==-1))
	{
		PlayerID externalId1, externalId2;
		externalId1=peer->GetExternalID(conn1.playerId);
		externalId2=peer->GetExternalID(conn2.playerId);
		return (externalId1!=conn1.playerId && externalId1!=conn2.playerId &&
			externalId2!=conn1.playerId && externalId2!=conn2.playerId);
	}
	return false;
}
void ConnectionGraph::NotifyUserOfRemoteConnection(const PlayerIdAndGroupId &conn1, const PlayerIdAndGroupId &conn2,unsigned short ping, RakPeerInterface *peer)
{
	// Create a packet to tell the user of this event
	static const int length=sizeof(MessageID) + (sizeof(PlayerID) + sizeof(ConnectionGraphGroupID)) * 2 + sizeof(unsigned short);
	Packet *p = AllocPacket(length);
	RakNet::BitStream b(p->data, length, false);
	p->bitSize=p->length*8;
	b.SetWriteOffset(0);
	b.Write((MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
	b.Write(conn1.playerId);
	b.Write(conn1.groupId);
	b.Write(conn2.playerId);
	b.Write(conn2.groupId);
	b.Write(ping);
	peer->PushBackPacket(p, false);
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
