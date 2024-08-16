#include "ReplicaManager.h"
#include "RakPeerInterface.h"
#include "GetTime.h"
#include "PacketEnumerations.h"
#include "BitStream.h"
#include "Replica.h"
#include <memory.h>
#include <assert.h>
#include <stdio.h> // For my debug printfs

#ifdef _MSC_VER
#pragma warning( push )
#endif

int ReplicaManager::CommandStructComp( Replica* const &key, const ReplicaManager::CommandStruct &data )
{
	if (key < data.replica)
		return -1;
	if (key==data.replica)
		return 0;
	return 1;
}

int ReplicaManager::RegisteredReplicaComp( Replica* const &key, const ReplicaManager::RegisteredReplica &data )
{
	if (key < data.replica)
		return -1;
	if (key==data.replica)
		return 0;
	return 1;
}

int ReplicaManager::RemoteObjectComp( Replica* const &key, const ReplicaManager::RemoteObject &data )
{
	if (key < data.replica)
		return -1;
	if (key==data.replica)
		return 0;
	return 1;
}

int ReplicaManager::ParticipantStructComp( const PlayerID &key, ReplicaManager::ParticipantStruct * const &data )
{
	if (key < data->playerId)
		return -1;
	if (key==data->playerId)
		return 0;
	return 1;
}

ReplicaManager::ReplicaManager()
{
	_constructionCB=0;
	_sendDownloadCompleteCB=0;
	_receiveDownloadCompleteCB=0;
	rakPeer=0;
	sendChannel=0;
	autoParticipateNewConnections=false;
	defaultScope=false;
	autoConstructToNewParticipants=false;
}
ReplicaManager::~ReplicaManager()
{
	Clear();
}
void ReplicaManager::SetAutoParticipateNewConnections(bool autoAdd)
{
	autoParticipateNewConnections=autoAdd;
}
void ReplicaManager::AddParticipant(PlayerID playerId)
{
	assert(playerId!=UNASSIGNED_PLAYER_ID);

	// If this player is already in the list of participants, just return.
	ParticipantStruct *participantStruct;
	participantStruct=GetParticipantByPlayerID(playerId);
	if (participantStruct)
		return;

	// Create a new participant with this playerID
	participantStruct = new ParticipantStruct;
	participantStruct->playerId=playerId;

	// Signal that when done sending SendConstruction for each existing object, we call sendDownloadCompleteCB
	participantStruct->callDownloadCompleteCB=true;

    // Add the new participant to the list of participants
	participantList.Insert(playerId,participantStruct);

	if (autoConstructToNewParticipants)
	{
		// Signal that we need to call SendConstruction for each existing object to this participant
		unsigned i;
		CommandStruct replicaAndCommand;
		replicaAndCommand.command=REPLICA_EXPLICIT_CONSTRUCTION;
		for (i=0; i < replicatedObjects.Size(); i++)
		{
			replicaAndCommand.replica=replicatedObjects[i].replica;
			participantStruct->commandList.Insert(replicaAndCommand.replica,replicaAndCommand);
		}
	}
}
void ReplicaManager::RemoveParticipant(PlayerID playerId)
{
	assert(playerId!=UNASSIGNED_PLAYER_ID);

	// Find this participant by playerId
	ParticipantStruct *participantStruct;
	participantStruct=GetParticipantByPlayerID(playerId);

	// If found, remove and free this participant structure
	if (participantStruct)
	{
		participantList.Remove(playerId);
		delete participantStruct;
	}
}

void ReplicaManager::Construct(Replica *replica, bool isCopy, PlayerID playerId, bool broadcast)
{
	assert(replica);

	unsigned i;
	ParticipantStruct *participantStruct;
	CommandStruct replicaAndCommand;
	unsigned index;
	bool objectExists;
	replicaAndCommand.replica=replica;

	ReferencePointer(replica);

	for (i=0; i < participantList.Size(); i++)
	{
		participantStruct=participantList[i];
		if ((broadcast==true && playerId!=participantStruct->playerId) || 
			(broadcast==false && playerId==participantStruct->playerId))
		{
			if (participantStruct->remoteObjectList.HasData(replica)==false)
			{
				index = participantStruct->commandList.GetIndexFromKey(replica, &objectExists);
				if (objectExists)
				{
#ifdef _DEBUG
					// Implicit is only used for objects that were not already registered.
					assert(isCopy==false);
#endif
					participantStruct->commandList[index].command|=REPLICA_EXPLICIT_CONSTRUCTION; // Set this bit
					participantStruct->commandList[index].command&=0xFF ^ REPLICA_IMPLICIT_CONSTRUCTION; // Unset this bit
				}
				else
				{
					if (isCopy)
						replicaAndCommand.command=REPLICA_IMPLICIT_CONSTRUCTION; // Set this bit
					else
						replicaAndCommand.command=REPLICA_EXPLICIT_CONSTRUCTION; // Set this bit

					participantStruct->commandList.Insert(replicaAndCommand.replica,replicaAndCommand);
				}
			}
		}
	}
}
void ReplicaManager::Destruct(Replica *replica, PlayerID playerId, bool broadcast)
{
	assert(replica);

	bool objectExists;
	unsigned replicatedObjectsIndex;
	replicatedObjectsIndex = replicatedObjects.GetIndexFromKey(replica, &objectExists);
	if (objectExists==false)
		return;

	// For each existing participant, send a packet telling them of this object destruction
	RakNet::BitStream outBitstream;
	unsigned i,tempIndex;
	bool replicaReferenced;
	ParticipantStruct *participantStruct;
	replicaReferenced=false;
	for (i=0; i < participantList.Size(); i++)
	{
		participantStruct=participantList[i];

		if ((broadcast==true && playerId!=participantStruct->playerId) || 
			(broadcast==false && playerId==participantStruct->playerId))
		{
			// Send the destruction packet immediately
			if (replica->GetNetworkID()!=UNASSIGNED_NETWORK_ID && (replicatedObjects[replicatedObjectsIndex].allowedInterfaces & REPLICA_SEND_DESTRUCTION))
			{
				outBitstream.Reset();
				outBitstream.Write((unsigned char)ID_REPLICA_MANAGER_DESTRUCTION);
				outBitstream.Write(replica->GetNetworkID());
				replica->SendDestruction(&outBitstream, participantStruct->playerId);
				if (outBitstream.GetNumberOfBitsUsed()>0)
				{
					rakPeer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, sendChannel, participantStruct->playerId, false);
				}
			}

			// Remove any pending commands that reference this object, for this player
			tempIndex = participantStruct->commandList.GetIndexFromKey(replica, &objectExists);
			if (objectExists)
				participantStruct->commandList.RemoveAtIndex(tempIndex);

			// Remove any remote object state tracking for this object, for this player
			tempIndex = participantStruct->remoteObjectList.GetIndexFromKey(replica, &objectExists);
			if (objectExists)
				participantStruct->remoteObjectList.RemoveAtIndex(tempIndex);
		}
		else if (replicaReferenced==false)
		{
			// See if any commands or objects reference replica
			if (participantStruct->commandList.HasData(replica))
				replicaReferenced=true;
			else if (participantStruct->remoteObjectList.HasData(replica))
				replicaReferenced=true;
		}
	}

	// Remove replica from the list if no commands and no remote objects reference it
	if (replicaReferenced==false)
		replicatedObjects.RemoveAtIndex(replicatedObjectsIndex);
}
void ReplicaManager::ReferencePointer(Replica *replica)
{
	// Start tracking this object, if we are not already
	if (replicatedObjects.HasData(replica)==false)
	{
		RegisteredReplica replicaAndTime;
		replicaAndTime.replica=replica;
		replicaAndTime.lastDeserializeTrue=0;
		replicaAndTime.allowedInterfaces=REPLICA_SET_ALL;
		replicatedObjects.Insert(replica,replicaAndTime);
	}
}
void ReplicaManager::DereferencePointer(Replica *replica)
{
	bool objectExists;
	unsigned replicatedObjectsIndex;
	unsigned tempIndex;
	replicatedObjectsIndex = replicatedObjects.GetIndexFromKey(replica, &objectExists);
	if (objectExists==false)
		return;
	replicatedObjects.RemoveAtIndex(replicatedObjectsIndex);

	ParticipantStruct *participantStruct;
	unsigned i;
	for (i=0; i < participantList.Size(); i++)
	{
		participantStruct=participantList[i];

		// Remove any pending commands that reference this object for any player
		tempIndex = participantStruct->commandList.GetIndexFromKey(replica, &objectExists);
		if (objectExists)
			participantStruct->commandList.RemoveAtIndex(tempIndex);

		// Remove any remote object state tracking for this object for any player
		tempIndex = participantStruct->remoteObjectList.GetIndexFromKey(replica, &objectExists);
		if (objectExists)
			participantStruct->remoteObjectList.RemoveAtIndex(tempIndex);
	}
}
void ReplicaManager::SetScope(Replica *replica, bool inScope, PlayerID playerId, bool broadcast)
{
	assert(replica);

	// Autoreference the pointer if necessary.  This way the user can call functions on an object without having to worry
	// About the order of operations.
	if (replicatedObjects.HasData(replica)==false)
		ReferencePointer(replica);

	// For each player that we want, flag to call SendScopeChange if inScope is different from what they already have
	unsigned i;
	ParticipantStruct *participantStruct;
	bool objectExists;
	unsigned index;
	CommandStruct replicaAndCommand;
	if (inScope)
		replicaAndCommand.command=REPLICA_SCOPE_TRUE;
	else
		replicaAndCommand.command=REPLICA_SCOPE_FALSE;
	replicaAndCommand.replica=replica;
	for (i=0; i < participantList.Size(); i++)
	{
		participantStruct=participantList[i];

		if ((broadcast==true && playerId!=participantStruct->playerId) || 
			(broadcast==false && playerId==participantStruct->playerId))
		{
			// If there is already a pending command for this object, add to it.  Otherwise, add a new pending command
			index = participantStruct->commandList.GetIndexFromKey(replica, &objectExists);
            if (objectExists)
			{
				// Update a pending command
				if (inScope)
				{
					participantStruct->commandList[index].command&=0xFF ^ REPLICA_SCOPE_FALSE; // Unset this bit
					participantStruct->commandList[index].command|=REPLICA_SCOPE_TRUE; // Set this bit
				}
				else
				{
					participantStruct->commandList[index].command&=0xFF ^ REPLICA_SCOPE_TRUE; // Unset this bit
					participantStruct->commandList[index].command|=REPLICA_SCOPE_FALSE; // Set this bit
				}						
			}
			else
			{
				// Add a new command, since there are no pending commands for this object
				participantStruct->commandList.Insert(replicaAndCommand.replica,replicaAndCommand);
			}
		}
	}
}
void ReplicaManager::SignalSerializeNeeded(Replica *replica, PlayerID playerId, bool broadcast)
{
	assert(replica);

	// Autoreference the pointer if necessary.  This way the user can call functions on an object without having to worry
	// About the order of operations.
	if (replicatedObjects.HasData(replica)==false)
		ReferencePointer(replica);

	// For each player that we want, if this object exists on that system, flag to call Serialize
	// (this may not necessarily happen - it depends on if the object is inScope when Update actually processes it.)
	unsigned i;
	ParticipantStruct *participantStruct;
	bool objectExists;
	unsigned index;
	CommandStruct replicaAndCommand;
	replicaAndCommand.command=REPLICA_SERIALIZE;
	replicaAndCommand.replica=replica;
	for (i=0; i < participantList.Size(); i++)
	{
		participantStruct=participantList[i];

		if ((broadcast==true && playerId!=participantStruct->playerId) || 
			(broadcast==false && playerId==participantStruct->playerId))
		{
			// If there is already a pending command for this object, add to it.  Otherwise, add a new pending command
			index = participantStruct->commandList.GetIndexFromKey(replica, &objectExists);
			if (objectExists)
			{
				participantStruct->commandList[index].command|=REPLICA_SERIALIZE; // Set this bit			
			}
			else
			{
				// Add a new command, since there are no pending commands for this object
				participantStruct->commandList.Insert(replicaAndCommand.replica,replicaAndCommand);
			}
		}
	}
}
void ReplicaManager::SetReceiveConstructionCB(ReplicaReturnResult (* constructionCB)(RakNet::BitStream *inBitStream, RakNetTime timestamp, NetworkID networkID, PlayerID senderId, ReplicaManager *caller))
{
	// Just overwrite the construction callback pointer
	_constructionCB=constructionCB;
}
void ReplicaManager::SetDownloadCompleteCB(ReplicaReturnResult (* sendDownloadCompleteCB)(RakNet::BitStream *outBitStream, RakNetTime currentTime, PlayerID senderId, ReplicaManager *caller), ReplicaReturnResult (* receiveDownloadCompleteCB)(RakNet::BitStream *inBitStream, PlayerID senderId, ReplicaManager *caller))
{
	// Just overwrite the send and receive download complete pointers.
	_sendDownloadCompleteCB=sendDownloadCompleteCB;
	_receiveDownloadCompleteCB=receiveDownloadCompleteCB;
}
void ReplicaManager::SetSendChannel(unsigned char channel)
{
	// Change the send channel from the default of 0
	sendChannel=channel;
}
void ReplicaManager::SetAutoConstructToNewParticipants(bool autoConstruct)
{
	autoConstructToNewParticipants=autoConstruct;
}
void ReplicaManager::SetDefaultScope(bool scope)
{
	defaultScope=scope;
}
void ReplicaManager::EnableReplicaInterfaces(Replica *replica, unsigned char interfaceFlags)
{
	bool objectExists;
	unsigned replicatedObjectsIndex;
	replicatedObjectsIndex = replicatedObjects.GetIndexFromKey(replica, &objectExists);
	if (objectExists==false)
	{
		// Autoreference the pointer if necessary.  This way the user can call functions on an object without having to worry
		// About the order of operations.
		ReferencePointer(replica);
		replicatedObjectsIndex = replicatedObjects.GetIndexFromKey(replica, &objectExists);
	}
	replicatedObjects[replicatedObjectsIndex].allowedInterfaces|=interfaceFlags;
}
void ReplicaManager::DisableReplicaInterfaces(Replica *replica, unsigned char interfaceFlags)
{
	bool objectExists;
	unsigned replicatedObjectsIndex;
	replicatedObjectsIndex = replicatedObjects.GetIndexFromKey(replica, &objectExists);
	if (objectExists==false)
	{
		// Autoreference the pointer if necessary.  This way the user can call functions on an object without having to worry
		// About the order of operations.
		ReferencePointer(replica);
		replicatedObjectsIndex = replicatedObjects.GetIndexFromKey(replica, &objectExists);
	}
	replicatedObjects[replicatedObjectsIndex].allowedInterfaces&= 0xFF ^ interfaceFlags;
}
bool ReplicaManager::IsConstructed(Replica *replica, PlayerID playerId)
{
	ParticipantStruct *participantStruct = GetParticipantByPlayerID(playerId);
	if (participantStruct)
	{
		bool objectExists;
		participantStruct->remoteObjectList.GetIndexFromKey(replica, &objectExists);
		return objectExists;
	}
	return false;
}
bool ReplicaManager::IsInScope(Replica *replica, PlayerID playerId)
{
	ParticipantStruct *participantStruct = GetParticipantByPlayerID(playerId);
	if (participantStruct)
	{
		bool objectExists;
		unsigned remoteObjectListIndex = participantStruct->remoteObjectList.GetIndexFromKey(replica, &objectExists);
		if (objectExists)
			return participantStruct->remoteObjectList[remoteObjectListIndex].inScope;
	}
	return false;
}
unsigned ReplicaManager::GetReplicaCount(void) const
{
	return replicatedObjects.Size();
}
Replica *ReplicaManager::GetReplicaAtIndex(unsigned index)
{
	return replicatedObjects[index].replica;
}
void ReplicaManager::Clear(void)
{
	// Free all memory
	unsigned i;
	for (i=0; i < participantList.Size(); i++)
		delete participantList[i];
	participantList.Clear();
	replicatedObjects.Clear();
}
void ReplicaManager::OnAttach(RakPeerInterface *peer)
{
	rakPeer=peer;
}
void ReplicaManager::Update(RakPeerInterface *peer)
{
	if (participantList.Size()==0)
		return;

	unsigned participantIndex, remoteObjectListIndex, replicatedObjectsIndex;
	ReplicaReturnResult res;
	bool sendTimestamp;
	ParticipantStruct *participantStruct;
	unsigned commandListIndex;
	RakNet::BitStream outBitstream, userDataBitstream;
	RakNetTime currentTime;
	bool objectExists;
	PacketPriority priority;
	PacketReliability reliability;
	ReceivedCommand *receivedCommand;
	Replica *replica;
	unsigned char command;
	currentTime=0;

	// For each participant
	for (participantIndex=0; participantIndex < participantList.Size(); participantIndex++)
	{
		participantStruct = participantList[participantIndex];

		// For each CommandStruct to send
		for (commandListIndex=0; commandListIndex < participantStruct->commandList.Size(); commandListIndex++)
		{
			// Only call RakNet::GetTime() once because it's slow
			if (currentTime==0)
				currentTime=RakNet::GetTime();

			replica=participantStruct->commandList[commandListIndex].replica;
			command=participantStruct->commandList[commandListIndex].command;
			replicatedObjectsIndex=replicatedObjects.GetIndexFromKey(replica, &objectExists);
#ifdef _DEBUG
			assert(objectExists);
#endif

			// If construction is set, call SendConstruction.  The only precondition is that the object was not already created,
			// which was checked in ReplicaManager::Replicate
			if (command & REPLICA_EXPLICIT_CONSTRUCTION)
			{
				if (replicatedObjects[replicatedObjectsIndex].allowedInterfaces & REPLICA_SEND_CONSTRUCTION)
				{
					userDataBitstream.Reset();
					sendTimestamp=false;
					res=replica->SendConstruction(currentTime, participantStruct->playerId, &userDataBitstream, &sendTimestamp);

					if (res==REPLICA_PROCESSING_DONE)
					{
						outBitstream.Reset();
						// If SendConstruction returns true and writes to outBitStream, do this send.  Clear the construction command.  Then process the next command for this CommandStruct, if any.
						if (sendTimestamp)
						{
							outBitstream.Write((unsigned char)ID_TIMESTAMP);
							outBitstream.Write(currentTime);
						}
						outBitstream.Write((unsigned char)ID_REPLICA_MANAGER_CONSTRUCTION);
						// It's required to send an NetworkID if available.
						// Problem:
						// A->B->C
						//	|  |
						//	D->E
						//
						// A creates.
						// B->C->E->D->B will cycle forever.
						// Fix is to always include an networkID.  Objects are not created if that object id already is present.
						if (replica->GetNetworkID()!=UNASSIGNED_NETWORK_ID)
						{
							outBitstream.Write(true);
							outBitstream.Write(replica->GetNetworkID());
						}
						else
							outBitstream.Write(false);

						outBitstream.Write(&userDataBitstream, userDataBitstream.GetNumberOfBitsUsed());

						peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, sendChannel, participantStruct->playerId, false);

						// Turn off this bit
						participantStruct->commandList[commandListIndex].command &= 0xFF ^ REPLICA_EXPLICIT_CONSTRUCTION;

						// Add the object to the participant's object list, indicating this object has been remotely created
						RemoteObject remoteObject;
						remoteObject.replica=replica;
						remoteObject.inScope=defaultScope;
						remoteObject.lastSendTime=0;
						// Create an entry for this object.  We do this now, even if the user might refuse the SendConstruction override,
						// because that call may be delayed and other commands sent while that is pending.  We always do the REPLICA_EXPLICIT_CONSTRUCTION call first.
						participantStruct->remoteObjectList.Insert(remoteObject.replica,remoteObject);
					}
					else if (res==REPLICA_PROCESS_LATER)
					{
						continue;
					}
					else // REPLICA_CANCEL_PROCESS
					{
						assert(res==REPLICA_CANCEL_PROCESS);
						participantStruct->commandList[commandListIndex].command=0;
					}
				}
				else
				{
					// Don't allow construction, or anything else for this object, as the construction send call is disallowed
					participantStruct->commandList[commandListIndex].command=0;
				}
			}
			else if (command & REPLICA_IMPLICIT_CONSTRUCTION)
			{
				// Turn off this bit
				participantStruct->commandList[commandListIndex].command &= 0xFF ^ REPLICA_IMPLICIT_CONSTRUCTION;

				// Add the object to the participant's object list, indicating this object is assumed to be remotely created
				RemoteObject remoteObject;
				remoteObject.replica=replica;
				remoteObject.inScope=defaultScope;
				remoteObject.lastSendTime=0;
				// Create an entry for this object.  We do this now, even if the user might refuse the SendConstruction override,
				// because that call may be delayed and other commands sent while that is pending.  We always do the REPLICA_EXPLICIT_CONSTRUCTION call first.
				participantStruct->remoteObjectList.Insert(remoteObject.replica,remoteObject);
			}

			// Sends the download complete packet
			// If callDownloadCompleteCB is true then check all the remaining objects starting at commandListIndex
			// I scan every frame in case the callback returns false to delay, and after that time a new object is Replicated
			if (participantStruct->callDownloadCompleteCB)
			{
				bool anyHasConstruction;
				unsigned j;
				anyHasConstruction=false;
				for (j=0; j < participantStruct->commandList.Size(); j++)
				{
					if (participantStruct->commandList[j].command & REPLICA_EXPLICIT_CONSTRUCTION)
					{
						anyHasConstruction=true;
						break;
					}
				}
				// If none have REPLICA_EXPLICIT_CONSTRUCTION, send ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE and set callDownloadCompleteCB false
				if (anyHasConstruction==false)
				{
					ReplicaReturnResult sendDLComplete;
					userDataBitstream.Reset();
					if (_sendDownloadCompleteCB)
						sendDLComplete=_sendDownloadCompleteCB(&userDataBitstream, currentTime, participantStruct->playerId, this); // If you return false, this will be called again next update
					else
						sendDLComplete=REPLICA_CANCEL_PROCESS;
					if (sendDLComplete==REPLICA_PROCESSING_DONE)
					{
						outBitstream.Reset();
						outBitstream.Write((unsigned char)ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE);
						outBitstream.Write(&userDataBitstream, userDataBitstream.GetNumberOfBitsUsed());
						peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, sendChannel, participantStruct->playerId, false);
						participantStruct->callDownloadCompleteCB=false;
					}
					else if (sendDLComplete==REPLICA_CANCEL_PROCESS)
					{
						participantStruct->callDownloadCompleteCB=false;
					}
					else
					{
						assert(sendDLComplete==REPLICA_PROCESS_LATER);
						// else REPLICA_PROCESS_LATER
					}
					
				}
			}

			// The remaining commands, SendScopeChange and Serialize, require the object the command references exists on the remote system, so check that
			remoteObjectListIndex = participantStruct->remoteObjectList.GetIndexFromKey(replica, &objectExists);
			if (objectExists)
			{
				command = participantStruct->commandList[commandListIndex].command;

				// Process SendScopeChange.
				if ((command & (REPLICA_SCOPE_TRUE | REPLICA_SCOPE_FALSE)))
				{
					if (replica->GetNetworkID()==UNASSIGNED_NETWORK_ID)
						continue; // Not set yet so call this later.

					if (replicatedObjects[replicatedObjectsIndex].allowedInterfaces & REPLICA_SEND_SCOPE_CHANGE)
					{
						bool scopeTrue = (command & REPLICA_SCOPE_TRUE)!=0;

						// Only send scope changes if the requested change is different from what they already have
						if (participantStruct->remoteObjectList[remoteObjectListIndex].inScope!=scopeTrue)
						{
							userDataBitstream.Reset();
							res=replica->SendScopeChange(scopeTrue, &userDataBitstream, currentTime, participantStruct->playerId);

							if (res==REPLICA_PROCESSING_DONE)
							{
								// If the user returns true and does write to outBitstream, do this send.  Clear the scope change command. Then process the next command for this CommandStruct, if any.
								outBitstream.Reset();
								outBitstream.Write((unsigned char)ID_REPLICA_MANAGER_SCOPE_CHANGE);
								outBitstream.Write(replica->GetNetworkID());
								outBitstream.Write(&userDataBitstream, userDataBitstream.GetNumberOfBitsUsed());
								peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, sendChannel, participantStruct->playerId, false);

								// Set the scope for this object and system
								participantStruct->remoteObjectList[remoteObjectListIndex].inScope=scopeTrue;

								// If scope is true, turn on serialize, since you virtually always want to serialize when an object goes in scope
								participantStruct->commandList[commandListIndex].command |= REPLICA_SERIALIZE;

								// Turn off these bits - Call is processed
								participantStruct->commandList[commandListIndex].command &= 0xFF ^ (REPLICA_SCOPE_TRUE | REPLICA_SCOPE_FALSE);
							}
							else if (res==REPLICA_CANCEL_PROCESS)
							{
								// Turn off these bits - Call is canceled
								participantStruct->commandList[commandListIndex].command &= 0xFF ^ (REPLICA_SCOPE_TRUE | REPLICA_SCOPE_FALSE);
							}
							else
							{
								// If the user returns false and the scope is currently set to false, just continue with another CommandStruct.  Don't process serialization until scoping is resolved first.
								if (scopeTrue==false)
									continue;

								// If the user returns false and the scope is currently set to false, process the next command for this CommandStruct, if any.
							}
						}
					}
					else
					{
						// Turn off these bits - Call is disallowed
						participantStruct->commandList[commandListIndex].command &= 0xFF ^ (REPLICA_SCOPE_TRUE | REPLICA_SCOPE_FALSE);

						// Set the scope - even if the actual send is disabled we might still be able to serialize.
						participantStruct->remoteObjectList[remoteObjectListIndex].inScope=(command & REPLICA_SCOPE_TRUE)!=0;
					}
				}

				command = participantStruct->commandList[commandListIndex].command;
				// Process Serialize
				if ((participantStruct->commandList[commandListIndex].command & REPLICA_SERIALIZE))
				{
					if (replica->GetNetworkID()==UNASSIGNED_NETWORK_ID)
						continue; // Not set yet so call this later.

					// If scope is currently false for this object in the RemoteObject list, cancel this serialize as the scope changed before the serialization went out
					if (participantStruct->remoteObjectList[remoteObjectListIndex].inScope && (replicatedObjects[replicatedObjectsIndex].allowedInterfaces & REPLICA_SEND_SERIALIZE))
					{
						do 
						{
							userDataBitstream.Reset();
							priority=HIGH_PRIORITY;
							reliability=RELIABLE_ORDERED;
							sendTimestamp=false;
							res=replica->Serialize(&sendTimestamp, &userDataBitstream, participantStruct->remoteObjectList[remoteObjectListIndex].lastSendTime, &priority, &reliability, currentTime, participantStruct->playerId);

							if (res==REPLICA_PROCESSING_DONE || res==REPLICA_PROCESS_AGAIN)
							{
								participantStruct->remoteObjectList[remoteObjectListIndex].lastSendTime=currentTime;

								outBitstream.Reset();
								if (sendTimestamp)
								{
									outBitstream.Write((unsigned char)ID_TIMESTAMP);
									outBitstream.Write(currentTime);
								}
								outBitstream.Write((unsigned char)ID_REPLICA_MANAGER_SERIALIZE);
								outBitstream.Write(replica->GetNetworkID());
								outBitstream.Write(&userDataBitstream, userDataBitstream.GetNumberOfBitsUsed());
								peer->Send(&outBitstream, priority, reliability, sendChannel, participantStruct->playerId, false);		

								// Clear the serialize bit when done
								if (res==REPLICA_PROCESSING_DONE)
									participantStruct->commandList[commandListIndex].command &= 0xFF ^ REPLICA_SERIALIZE;
								// else res==REPLICA_PROCESS_AGAIN so it will repeat the enclosing do {} while(); loop
							}
							else if (res==REPLICA_CANCEL_PROCESS)
							{
								// Clear the serialize bit
								participantStruct->commandList[commandListIndex].command &= 0xFF ^ REPLICA_SERIALIZE;
							}
							else
							{
								// if the user returns REPLICA_PROCESS_LATER, just continue with another CommandStruct.
								assert(res==REPLICA_PROCESS_LATER);
							}
						} while(res==REPLICA_PROCESS_AGAIN);
					}
					else
					{
						// Cancel this serialize
						participantStruct->commandList[commandListIndex].command &= 0xFF ^ REPLICA_SERIALIZE;
					}
				}
			}
		}

		// Go through the command list and delete all cleared commands, from back to front.  It is more efficient to do this than to delete them as we process
		commandListIndex=participantStruct->commandList.Size();
		if (commandListIndex>0)
		{
#ifdef _MSC_VER
#pragma warning( disable : 4127 ) // warning C4127: conditional expression is constant
#endif
			while (1)
			{
				if (participantStruct->commandList[commandListIndex-1].command==0)
				{
					// If this is the last item in the list, and it probably is, then it just changes a number rather than shifts the entire array
					participantStruct->commandList.RemoveAtIndex(commandListIndex-1);
				}

				if (--commandListIndex==0)
					break;
			}
		}
		
		// Now process queued receives
		while (participantStruct->pendingCommands.Size())
		{
			receivedCommand=participantStruct->pendingCommands.Pop();
			participantStruct=GetParticipantByPlayerID(receivedCommand->playerId);
			if (participantStruct)
			{
				res=ProcessReceivedCommand(participantStruct, receivedCommand);
				// Returning false means process this command again later
				if (res==REPLICA_PROCESS_LATER)
				{
					// Push the command back in the queue
					participantStruct->pendingCommands.PushAtHead(receivedCommand);

					// Stop processing, because all processing is in order
					break;
				}
				else
				{
					assert(res==REPLICA_CANCEL_PROCESS);
				}
			}
			
			// Done with this command, so delete it
			delete receivedCommand->userData;
			delete receivedCommand;
		}
	}
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void ReplicaManager::OnCloseConnection(RakPeerInterface *peer, PlayerID playerId)
{
	RemoveParticipant(playerId);
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void ReplicaManager::OnDisconnect(RakPeerInterface *peer)
{
	Clear();
}
PluginReceiveResult ReplicaManager::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	unsigned char packetIdentifier;
	if ( ( unsigned char ) packet->data[ 0 ] == ID_TIMESTAMP )
	{
		if ( packet->length > sizeof( unsigned char ) + sizeof( unsigned int ) )
			packetIdentifier = ( unsigned char ) packet->data[ sizeof( unsigned char ) + sizeof( unsigned int ) ];
		else
			return RR_STOP_PROCESSING_AND_DEALLOCATE;
	}
	else
		packetIdentifier = ( unsigned char ) packet->data[ 0 ];

	switch (packetIdentifier)
	{
	case ID_NEW_INCOMING_CONNECTION:
	case ID_CONNECTION_REQUEST_ACCEPTED:
		if (autoParticipateNewConnections)
			AddParticipant(packet->playerId);
		return RR_CONTINUE_PROCESSING;
	case ID_DISCONNECTION_NOTIFICATION:
	case ID_CONNECTION_LOST:
		OnCloseConnection(peer, packet->playerId);
		return RR_CONTINUE_PROCESSING;
	case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:
		if (_receiveDownloadCompleteCB==0)
		{
			return RR_STOP_PROCESSING_AND_DEALLOCATE;
		}
	case ID_REPLICA_MANAGER_CONSTRUCTION:
	case ID_REPLICA_MANAGER_DESTRUCTION:
	case ID_REPLICA_MANAGER_SCOPE_CHANGE:
	case ID_REPLICA_MANAGER_SERIALIZE:
		{
			ParticipantStruct *participantStruct;
			bool hasNetworkId;
			ReceivedCommand receivedCommand;
			bool b=true;
			RakNet::BitStream inBitstream(packet->data, packet->length, false);
			// SetWriteOffset is used here to get around a design flaw, where I should have had the bitstream constructor take bits, rather than bytes
			// It sets the actual number of bits in the packet
			inBitstream.SetWriteOffset(packet->bitSize);
			receivedCommand.playerId=packet->playerId;
			receivedCommand.command=packetIdentifier;

			if ( ( unsigned char ) packet->data[ 0 ] == ID_TIMESTAMP )
			{
				inBitstream.IgnoreBits(8);
				b=inBitstream.Read(receivedCommand.u1);
			}
			else
				receivedCommand.u1=0;
			inBitstream.IgnoreBits(8); // Ignore the packet id
			receivedCommand.networkID=UNASSIGNED_NETWORK_ID;
			if (packetIdentifier==ID_REPLICA_MANAGER_CONSTRUCTION) // ID_REPLICA_MANAGER_CONSTRUCTION has an optional networkID
			{
				b=inBitstream.Read(hasNetworkId);
				if (hasNetworkId)
					b=inBitstream.Read(receivedCommand.networkID);
			}
			else if (packetIdentifier!=ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE)
			{
				b=inBitstream.Read(receivedCommand.networkID); // Other packets always have an networkID
			}

			if (b==false)
			{
				// Invalid packet
#ifdef _DEBUG
				assert(0);
#endif
				return RR_STOP_PROCESSING_AND_DEALLOCATE;
			}
			receivedCommand.userData=&inBitstream;
			participantStruct=GetParticipantByPlayerID(receivedCommand.playerId);
			if (participantStruct)
			{
				// .Size()>0 is because commands are always processed in order.  If a command is delayed, no further commands are processed.
				// ProcessReceivedCommand(...)==false means that the use signaled to delay a command
				if (participantStruct->pendingCommands.Size()>0 || ProcessReceivedCommand(participantStruct, &receivedCommand)==REPLICA_PROCESS_LATER)
				{
					// Copy the data and add this to a queue that will call ProcessReceivedCommand again in Update.

					// Allocate and copy structure
					ReceivedCommand *rc = new ReceivedCommand;
					memcpy(rc, &receivedCommand, sizeof(ReceivedCommand));

					// Allocate and copy inBitstream remaining data
					rc->userData = new RakNet::BitStream;
					rc->userData->Write(&inBitstream, inBitstream.GetNumberOfBitsUsed());

					participantStruct->pendingCommands.Push(rc);
				}
			}

			return RR_STOP_PROCESSING_AND_DEALLOCATE;
		}
	}

	return RR_CONTINUE_PROCESSING;
}

ReplicaManager::ParticipantStruct* ReplicaManager::GetParticipantByPlayerID(const PlayerID playerId) const
{
	bool objectExists;
	unsigned index;
	index = participantList.GetIndexFromKey(playerId, &objectExists);
	if (objectExists==false)
		return 0;
	return participantList[index];
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#pragma warning( disable : 4701 ) // warning C4701: local variable <variable name> may be used without having been initialized
#endif
ReplicaReturnResult ReplicaManager::ProcessReceivedCommand(ParticipantStruct *participantStruct, ReceivedCommand *receivedCommand)
{
	Replica *replica = (Replica*) NetworkIDGenerator::GET_BASE_OBJECT_FROM_ID(receivedCommand->networkID);
	
	bool objectExists;
	unsigned index;
	ReplicaReturnResult b;
	if (replica)
	{
		index = replicatedObjects.GetIndexFromKey(replica, &objectExists);
		if (objectExists==false)
		{
			// Two ways to handle this.  One is to autoregister the object - but this is dangerous in that if you have a network object that is not
			// using the replica system it would be of the wrong type, read read invalid memory, and crash.
			// Construct(replica, playerId, broadcast, true);

			// The other way is to assert and do nothing.  The disadvantage is now the user has to register the object in order to receive calls on it.
#ifdef _DEBUG
			// If this assert hits, call ReferencePointer on your replica pointer after it is created.
			assert(0);
#endif
			return REPLICA_CANCEL_PROCESS;
		}
	}

	if (receivedCommand->command==ID_REPLICA_MANAGER_SERIALIZE)
	{
		if (replica && (replicatedObjects[index].allowedInterfaces & REPLICA_RECEIVE_SERIALIZE))
		{
			b=replica->Deserialize(receivedCommand->userData, receivedCommand->u1, replicatedObjects[index].lastDeserializeTrue, receivedCommand->playerId);
			if (b==REPLICA_PROCESSING_DONE)
				replicatedObjects[index].lastDeserializeTrue=RakNet::GetTime();
			return b;
		}
	}
	else if (receivedCommand->command==ID_REPLICA_MANAGER_CONSTRUCTION)
	{
		// If networkID already exists on this system, ignore the packet
		if (replica==0)
		{
#ifdef _DEBUG
			assert(_constructionCB);
#endif
			// Call the registered callback.  If it crashes, you forgot to register the callback in SetReceiveConstructionCB
			return _constructionCB(receivedCommand->userData, receivedCommand->u1, receivedCommand->networkID, receivedCommand->playerId, this);
		}
	}
	else if (receivedCommand->command==ID_REPLICA_MANAGER_SCOPE_CHANGE)
	{
		if (replica && (replicatedObjects[index].allowedInterfaces & REPLICA_RECEIVE_SCOPE_CHANGE))
		{
			return replica->ReceiveScopeChange(receivedCommand->userData, receivedCommand->playerId);
		}
	}
	else if (receivedCommand->command==ID_REPLICA_MANAGER_DESTRUCTION)
	{
		if (replica && (replicatedObjects[index].allowedInterfaces & REPLICA_RECEIVE_DESTRUCTION))
		{
			return replica->ReceiveDestruction(receivedCommand->userData, receivedCommand->playerId);
		}        
	}
	else if (receivedCommand->command==ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE)
	{
		if (_receiveDownloadCompleteCB)
		{
			return _receiveDownloadCompleteCB(receivedCommand->userData, receivedCommand->playerId, this);
		}
	}

	return REPLICA_CANCEL_PROCESS;
}

ReplicaManager::ParticipantStruct::~ParticipantStruct()
{
	ReceivedCommand *receivedCommand;
	while ( pendingCommands.Size() )
	{
		receivedCommand=pendingCommands.Pop();
		delete receivedCommand->userData;
		delete receivedCommand;
	}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
