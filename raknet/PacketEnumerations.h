/// \file
/// \brief All the packet identifiers used by RakNet.  Packet identifiers comprise the first byte of any message.
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

#ifndef __PACKET_ENUMERATIONS_H
#define __PACKET_ENUMERATIONS_H 

/// You should not edit the file PacketEnumerations.h as it is a part of RakNet static library
/// To define your own message id, define an enum following the code example that follows. 
///
/// \code
/// enum {
///   ID_MYPROJECT_MSG_1 = ID_USER_PACKET_ENUM
///   ID_MYPROJECT_MSG_2, 
///    ... 
/// };
/// \endcode 
///
/// \note All these enumerations should be casted to (unsigned char) before writing them to RakNet::BitStream
enum
{
	/// 6: Ping from a connected system.  Update timestamps (internal use only)
	ID_INTERNAL_PING = 6,  
	/// 7: Ping from an unconnected system.  Reply but do not update timestamps. (internal use only)
	ID_PING,
	/// 8: Ping from an unconnected system.  Only reply if we have open connections. Do not update timestamps. (internal use only)
	ID_PING_OPEN_CONNECTIONS,
	/// 9: Pong from a connected system.  Update timestamps (internal use only)
	ID_CONNECTED_PONG,

	/// 12: ???
	ID_UNK_12 = 12,

	/// 15: Connecting to a secured server/peer
	ID_SECURED_CONNECTION_RESPONSE = 15,
	/// 16: Connecting to a secured server/peer
	ID_SECURED_CONNECTION_CONFIRMATION,

	/// 18: Server / Client only - The server is broadcasting the pings of all players in the game (internal use only)
	ID_BROADCAST_PINGS = 18,
	/// 19: Server / Client only - The server is broadcasting a random number seed (internal use only)
	ID_SET_RANDOM_NUMBER_SEED,
	/// 20: Remote procedure call (internal use only)
	ID_RPC,
	/// 21: Remote procedure call reply, for RPCs that return data (internal use only)
	ID_RPC_REPLY,

	/// 22: ???
	ID_NEW_INCOMING_CONNECTION_2,

	/// [PEER|SERVER|CLIENT] 29: Sent to the player when a connection request cannot be completed due to inability to connect.
	/// Never transmitted.
	ID_CONNECTION_ATTEMPT_FAILED = 29,

	/// [PEER|SERVER] 30: A remote system has successfully connected.
	ID_NEW_INCOMING_CONNECTION,

	/// [PEER|CLIENT] 31: The system we attempted to connect to is not accepting new connections.
	ID_NO_FREE_INCOMING_CONNECTIONS,

	/// [PEER|SERVER|CLIENT] 32: The system specified in Packet::playerID has disconnected from us.  For the client, this would mean the server has shutdown.
	ID_DISCONNECTION_NOTIFICATION,

	/// [PEER|SERVER|CLIENT] 33: Reliable packets cannot be delivered to the system specifed in Packet::playerID.  The connection to that system has been closed.
	ID_CONNECTION_LOST,

	/// [PEER|CLIENT] 34: In a client/server environment, our connection request to the server has been accepted.
	ID_CONNECTION_REQUEST_ACCEPTED,

	/// [CLIENT|PEER] 35: We preset an RSA public key which does not match what the system we connected to is using.
	ID_RSA_PUBLIC_KEY_MISMATCH,

	/// [PEER|CLIENT] 36: We are banned from the system we attempted to connect to.
	ID_CONNECTION_BANNED,

	/// [PEER|CLIENT] 37: The remote system is using a password and has refused our connection because we did not set the correct password.
	ID_INVALID_PASSWORD,

	/// [PEER|SERVER|CLIENT] 38: A packet has been tampered with in transit.  The sender is contained in Packet::playerID.
	/// Never transmitted.
	ID_MODIFIED_PACKET,

	/// [PEER] 39: Pong from an unconnected system.  First byte is ID_PONG, second sizeof(RakNetTime) bytes is the ping, following bytes is system specific enumeration data.
	ID_PONG,

	/// [PEER|SERVER|CLIENT] 40: The four bytes following this byte represent an unsigned int which is automatically modified by the difference in system times between the sender and the recipient. Requires that you call StartOccasionalPing.
	ID_TIMESTAMP,

	/// [PEER|SERVER|CLIENT] 41: We got a bitstream containing static data.  You can now read this data. This packet is transmitted automatically on connections, and can also be manually sent.
	ID_RECEIVED_STATIC_DATA,

	/// [CLIENT] 42: In a client/server environment, a client other than ourselves has disconnected gracefully.  Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_DISCONNECTION_NOTIFICATION,

	/// [CLIENT] 43: In a client/server environment, a client other than ourselves has been forcefully dropped. Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_CONNECTION_LOST,

	/// [CLIENT] 44: In a client/server environment, a client other than ourselves has connected.  Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_NEW_INCOMING_CONNECTION,

	/// [CLIENT] 45: On our initial connection to the server, we are told of every other client in the game.  Packet::playerID is modified to reflect the playerID of this client.
	ID_REMOTE_EXISTING_CONNECTION,

	/// [CLIENT] - 46: Got the data for another client
	ID_REMOTE_STATIC_DATA,

	/// [FILELIST] 47:
	ID_FILE_LIST_TRANSFER_HEADER,

	/// [FILELIST] 48:
	ID_FILE_LIST_TRANSFER_FILE,

	/// [Delta Directory transfer] 49:
	ID_DDT_DOWNLOAD_REQUEST,

	/// [PEER|SERVER|CLIENT] 55: Inform a remote system of our IP/Port.
	ID_ADVERTISE_SYSTEM = 55,

	/// [RakNetTransport] 56
	ID_TRANSPORT_STRING,

	/// [ConnectionGraph] 57
	ID_CONNECTION_GRAPH_REQUEST,
	/// [ConnectionGraph] 58
	ID_CONNECTION_GRAPH_REPLY,
	/// [ConnectionGraph] 59
	ID_CONNECTION_GRAPH_UPDATE,
	/// [ConnectionGraph] 60
	ID_CONNECTION_GRAPH_NEW_CONNECTION,
	/// [ConnectionGraph] 61
	ID_CONNECTION_GRAPH_CONNECTION_LOST,
	/// [ConnectionGraph] 62
	ID_CONNECTION_GRAPH_DISCONNECTION_NOTIFICATION,

	/// [ReplicaManager] 63
	ID_REPLICA_MANAGER_CONSTRUCTION,
	/// [ReplicaManager] 64
	ID_REPLICA_MANAGER_DESTRUCTION,
	/// [ReplicaManager] 65
	ID_REPLICA_MANAGER_SCOPE_CHANGE,
	/// [ReplicaManager] 66
	ID_REPLICA_MANAGER_SERIALIZE,
	/// [ReplicaManager] 67
	ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE,

	/// [Router] 68
	ID_ROUTE_AND_MULTICAST,

	/// [NAT Punchthrough]
	ID_NAT_PUNCHTHROUGH_REQUEST = 81,

	/// [NAT Punchthrough] Returned to user.  PlayerID binary address / port is written to the stream
	ID_NAT_TARGET_CONNECTION_LOST,

	/// [NAT Punchthrough] Returned to user.  PlayerID binary address / port is written to the stream
	ID_NAT_TARGET_NOT_CONNECTED,

	// [NAT Punchthrough]
	ID_NAT_CONNECT_AT_TIME,

	// [NAT Punchthrough]
	ID_NAT_SEND_OFFLINE_MESSAGE_AT_TIME,

	// [Database] Internal
	ID_DATABASE_QUERY_REQUEST,

	// [Database] Internal
	ID_DATABASE_UPDATE_ROW,

	// [Database] Internal
	ID_DATABASE_REMOVE_ROW,

	// [Database] A serialized table.  Bytes 1+ contain the table.  Pass to TableSerializer::DeserializeTable
	ID_DATABASE_QUERY_REPLY,

	// [Database] Specified table not found
	ID_DATABASE_UNKNOWN_TABLE,

	// [Database] Incorrect password
	ID_DATABASE_INCORRECT_PASSWORD,

	// RakPeer - Downloading a large message. Format is ID_DOWNLOAD_PROGRESS (MessageID), partCount (unsigned int), partTotal (unsigned int), partLength (unsigned int), first part data (length <= MAX_MTU_SIZE)
	ID_DOWNLOAD_PROGRESS,

	///
	/// SA-MP 0.3.7 Packet IDs
	///

	ID_VEHICLE_SYNC = 200,
	ID_RCON_COMMAND,
	ID_RCON_RESPONCE,
	ID_AIM_SYNC,
	ID_WEAPONS_UPDATE,
	ID_STATS_UPDATE,
	ID_WEAPON_SHOT_SYNC,
	ID_PLAYER_SYNC,
	// 208
	ID_UNOCCUPIED_SYNC = 209,
	ID_TRAILER_SYNC,
	ID_PASSENGER_SYNC,
	ID_SPECTATOR_SYNC,

	// TODO: Find out the packet IDs for these.
	ID_OPEN_CONNECTION_REPLY,
	ID_CONNECTION_REQUEST,
	ID_OPEN_CONNECTION_REQUEST,
	ID_DETECT_LOST_CONNECTIONS,
	ID_RPC_MAPPING,
	ID_REQUEST_STATIC_DATA,
};

#endif
