/// \file
/// \brief Contains interface Replica used by the ReplicaManager.
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

#ifndef __REPLICA_H
#define __REPLICA_H

#include "NetworkIDGenerator.h"
#include "PacketPriority.h"
#include "ReplicaEnums.h"

/// This is an interface of a replicated object for use in the framework of ReplicaManager
/// You should derive from this class, implementing the functions to provide the behavior you want.
/// If your architecture doesn't allow you to derive from this class, you can store an instance of a derived instance of this class in your base game object.
/// In that case, use GetParent() and SetParent() and propagate the function calls up to your real classes. For an example where I do this, see Monster.h in the ReplicaManagerCS sample.
/// \brief The interface to derive your game's networked classes from
/// \ingroup REPLICA_MANAGER_GROUP
class Replica : public NetworkIDGenerator
{
public:
	/// This function is called in the first update tick after this object is first passed to ReplicaManager::Replicate for each player, and also when a new participant joins
	/// The intent of this function is to tell another system to create an instance of this class.
	/// You can do this however you want - I recommend having each class having a string identifier which is registered with StringTable, and then using EncodeString to write the name of the class.  In the callback passed to SetReceiveConstructionCB create an instance of the object based on that string.
	/// \note If you return true from IsNetworkIDAuthority, which you should do for a server or peer, I recommend encoding the value returned by GetNetworkID() into your bitstream and calling SetNetworkID with that value in your SetReceiveConstructionCB callback.
	/// \note Dereplicate, SetScope, and SignalSerializeNeeded all rely on being able to call GET_OBJECT_FROM_ID which requires that SetNetworkID be called on that object.
	/// \note SendConstruction is called once for every new player that connects and every existing player when an object is passed to Replicate.
	/// \param[in] currentTime The current time that would be returned by RakNet::GetTime().  That's a slow call I do already, so you can use the parameter instead of having to call it yourself.
	/// \param[in] playerId The participant to send to.
	/// \param[out] outBitStream The data you want to write in the message. If you do not write to outBitStream and return true, then no send call will occur and the system will consider this object as not created on that remote system.
	/// \param[out] includeTimestamp Set to true to include a timestamp with the message.  This will be reflected in the timestamp parameter of the callback.  Defaults to false.
	/// \return See ReplicaReturnResult
	virtual ReplicaReturnResult SendConstruction( RakNetTime currentTime, PlayerID playerId, RakNet::BitStream *outBitStream, bool *includeTimestamp )=0;

	/// The purpose of the function is to send a packet containing the data in \a outBitStream to \a playerId telling that system that Dereplicate was called.
	/// In the code, this is called in the update cycle after you call ReplicaManager::Destruct().  Then, if you write to outBitStream, a message is sent to that participant.
	/// This is the one send you cannot delay because objects may be deleted and we can't read into them past this.
	/// \param[out] outBitStream The data to send.  If you do not write to outBitStream, then no send call will occur
	/// \param[in] playerId The participant to send to.
	virtual void SendDestruction(RakNet::BitStream *outBitStream, PlayerID playerId)=0;

	/// This function is called when SendDestruction is sent from another system.  Delete your object if you want.
	/// \param[in] inBitStream What was sent in SendDestruction::outBitStream
	/// \param[in] playerId The participant that sent this message to us.
	/// \return See ReplicaReturnResult
	virtual ReplicaReturnResult ReceiveDestruction(RakNet::BitStream *inBitStream, PlayerID playerId)=0;

	/// Called when ReplicaManager::SetScope is called with a different value than what it currently has.
	/// It is up to you to write \a inScope to \a outBitStream.  Not doing so, and returning true, means you want to cancel the scope change call.
	/// If \a inScope is true, you return true, and data is written to outBitStream, then Serialize will be called automatically
	/// This is a convenience feature, since there's almost no case where an object would go in scope but not be serialized
	/// \param[in] inScope The new scope that will be sent to ReceiveScopeChange that originally came from SetScope
	/// \param[out] outBitStream The data to send.  If you do not write to outBitStream and return true, then no send will occur and the object will keep its existing scope
	/// \param[in] currentTime The current time that would be returned by RakNet::GetTime().  That's a slow call I do already, so you can use the parameter instead of having to call it yourself.
	/// \param[in] playerId The participant to send to.
	/// \return See ReplicaReturnResult
	virtual ReplicaReturnResult SendScopeChange(bool inScope, RakNet::BitStream *outBitStream, RakNetTime currentTime, PlayerID playerId)=0;

	/// Called when when we get the SendScopeChange message.  The new scope should have been encoded (by you) into \a inBitStream
	/// \param[in] inBitStream What was sent in SendScopeChange::outBitStream
	/// \param[in] playerId The participant that sent this message to us.
	/// \return See ReplicaReturnResult
	virtual ReplicaReturnResult ReceiveScopeChange(RakNet::BitStream *inBitStream,PlayerID playerId)=0;

	/// Called when ReplicaManager::SignalSerializeNeeded is called with this object as the parameter.
	/// The system will ensure that Serialize only occurs for participants that have this object constructed and in scope
	/// The intent of this function is to serialize all your class member variables for remote transmission.
	/// \param[out] sendTimestamp Set to true to include a timestamp with the message.  This will be reflected in the timestamp parameter Deserialize.  Defaults to false.
	/// \param[out] outBitStream The data you want to write in the message. If you do not write to outBitStream and return true, then no send will occur for this participant.
	/// \param[in] lastSendTime The last time Serialize returned true and outBitStream was written to.  0 if this is the first time the function has ever been called for this \a playerId
	/// \param[in] priority Passed to RakPeer::Send for the send call.
	/// \param[in] reliability Passed to RakPeer::Send for the send call.
	/// \param[in] currentTime The current time that would be returned by RakNet::GetTime().  That's a slow call I do already, so you can use the parameter instead of having to call it yourself.
	/// \param[in] playerId The participant we are sending to.
	/// \return See ReplicaReturnResult
	virtual ReplicaReturnResult Serialize(bool *sendTimestamp, RakNet::BitStream *outBitStream, RakNetTime lastSendTime, PacketPriority *priority, PacketReliability *reliability, RakNetTime currentTime, PlayerID playerId)=0;

	/// Called when another participant called Serialize with our system as the target
	/// \param[in] inBitStream What was written to Serialize::outBitStream
	/// \param[in] timestamp if Serialize::sendTimestamp was set to true, the time the packet was sent.
	/// \param[in] lastDeserializeTime Last time you returned true from this function for this object, or 0 if never, regardless of \a playerId.
	/// \param[in] playerId The participant that sent this message to us.
	virtual ReplicaReturnResult Deserialize(RakNet::BitStream *inBitStream, RakNetTime timestamp, RakNetTime lastDeserializeTime, PlayerID playerId )=0;
};

#endif
