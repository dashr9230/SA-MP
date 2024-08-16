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

#include "NetworkIDGenerator.h"

#ifdef _COMPATIBILITY_1
#elif defined(_WIN32)
#include <malloc.h> // alloca
#elif defined(_COMPATIBILITY_2)
#include "Compatibility2Includes.h"
#else
#include <stdlib.h> // alloca
#endif

#include <assert.h>

// Note you will need to save and load this if your game supports saving and loading so you start at the same index you left off.
// If you don't do this you can overwrite indices
unsigned short NetworkIDGenerator::staticItemID = 0;
DataStructures::AVLBalancedBinarySearchTree<NetworkIDNode> NetworkIDGenerator::IDTree;
PlayerID NetworkIDGenerator::externalPlayerId=UNASSIGNED_PLAYER_ID;

bool NetworkIDNode::operator==( const NetworkIDNode& right ) const
{
	if ( networkID == right.networkID )
		return !0;

	return 0;
}

bool NetworkIDNode::operator > ( const NetworkIDNode& right ) const
{
	if ( networkID > right.networkID )
		return !0;

	return 0;
}

bool NetworkIDNode::operator < ( const NetworkIDNode& right ) const
{
	if ( networkID < right.networkID )
		return !0;

	return 0;
}

NetworkIDNode::NetworkIDNode()
{
	object = 0;
}

NetworkIDNode::NetworkIDNode( NetworkID _networkID, NetworkIDGenerator *_object )
{
	networkID = _networkID;
	object = _object;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetworkIDGenerator::NetworkIDGenerator()
{
	callGenerationCode=true;
	networkID=UNASSIGNED_NETWORK_ID;
	parent=0;
}

//-------------------------------------------------------------------------------------


NetworkIDGenerator::~NetworkIDGenerator()
{
	if (networkID!=UNASSIGNED_NETWORK_ID)
	{
		NetworkIDNode * object = NetworkIDGenerator::IDTree.GetPointerToNode( NetworkIDNode( ( networkID ), 0 ) );
		if ( object->object == this )
			IDTree.Del( NetworkIDNode( object->networkID, 0 ) );
	}
}

//////////////////////////////////////////////////////////////////////
// Public Methods
//////////////////////////////////////////////////////////////////////

NetworkID NetworkIDGenerator::GetNetworkID( void )
{
	if (callGenerationCode && IsNetworkIDAuthority())
	{
		GenerateID();
		callGenerationCode=false;
	}

	return networkID;
};

//-------------------------------------------------------------------------------------

unsigned short NetworkIDGenerator::GetStaticNetworkID( void )
{
	return staticItemID;
}

//-------------------------------------------------------------------------------------

void NetworkIDGenerator::SetStaticNetworkID( unsigned short i )
{
	staticItemID = i;
}

//-------------------------------------------------------------------------------------
void NetworkIDGenerator::SetExternalPlayerID(PlayerID playerId)
{
	assert(playerId!=UNASSIGNED_PLAYER_ID);
	externalPlayerId=playerId;
}
//-------------------------------------------------------------------------------------
PlayerID NetworkIDGenerator::GetExternalPlayerID(void)
{
	return externalPlayerId;
}
//-------------------------------------------------------------------------------------

bool NetworkIDGenerator::RequiresSetParent(void) const
{
	return false;
}

//-------------------------------------------------------------------------------------

void NetworkIDGenerator::SetNetworkID( NetworkID id )
{
	callGenerationCode=false;

	if ( id == UNASSIGNED_NETWORK_ID )
	{
		// puts("Warning: NetworkIDGenerator passed UNASSIGNED_NETWORK_ID.  SetID ignored");
		return ;
	}

	if ( networkID == id )
	{
		// printf("NetworkIDGenerator passed %i which already exists in the tree.  SetID ignored", id);
		return ;
	}

	NetworkIDNode* collision = NetworkIDGenerator::IDTree.GetPointerToNode( NetworkIDNode( ( id ), 0 ) );

	if ( collision )   // Tree should have only unique values.  The new value is already in use.
	{
		//printf("Warning: NetworkIDGenerator::SetID passed %i, which has an existing node in the tree.  Old node removed, which will cause the item pointed to to be inaccessible to the network", id);
		IDTree.Del( NetworkIDNode( collision->networkID, collision->object ) );
	}

	if ( networkID == UNASSIGNED_NETWORK_ID )   // Object has not had an ID assigned so does not already exist in the tree
	{
		networkID = id;
		IDTree.Add( NetworkIDNode( networkID, this ) );
	}
	else // Object already exists in the tree and has an assigned ID
	{
		IDTree.Del( NetworkIDNode( networkID, this ) ); // Delete the node with whatever ID the existing object is using
		networkID = id;
		IDTree.Add( NetworkIDNode( networkID, this ) );
	}
}

//-------------------------------------------------------------------------------------
void NetworkIDGenerator::SetParent( void *_parent )
{
	parent=_parent;

#ifdef _DEBUG
	// Avoid duplicate parents in the tree
	unsigned size = IDTree.Size();
	NetworkIDNode *nodeArray;

	bool usedAlloca=false;
	#if !defined(_COMPATIBILITY_1)
	if (sizeof(NetworkIDNode) * size < MAX_ALLOCA_STACK_ALLOCATION)
	{
		nodeArray = (NetworkIDNode*) alloca(sizeof(NetworkIDNode) * size);
		usedAlloca=true;
	}
	else
	#endif
		nodeArray = new NetworkIDNode[size];


	IDTree.DisplayBreadthFirstSearch( nodeArray );
	unsigned i;
	for (i=0; i < size; i++)
	{
		// If this assert hits then this _parent is already in the tree.  Classes instance should never contain more than one NetworkIDGenerator
		assert(nodeArray->object->GetParent()!=parent);
	}
	
	if (usedAlloca==false)
		delete [] nodeArray;
	
#endif
}

//-------------------------------------------------------------------------------------
void* NetworkIDGenerator::GetParent( void ) const
{
	return parent;
}
//-------------------------------------------------------------------------------------
void NetworkIDGenerator::GenerateID(void)
{
	assert(IsNetworkIDAuthority());

	NetworkIDNode* collision;
	do
	{
		networkID.localSystemId=staticItemID++;
		if (NetworkID::peerToPeerMode)
		{
			 // If this assert hits you forgot to call SetExternalPlayerID
			assert(externalPlayerId!=UNASSIGNED_PLAYER_ID);
			networkID.playerId=externalPlayerId;
		}
		collision = NetworkIDGenerator::IDTree.GetPointerToNode( NetworkIDNode( ( networkID ), 0 ) );
	}
	while ( collision );

	IDTree.Add( NetworkIDNode( networkID, this ) );
}

//-------------------------------------------------------------------------------------
void* NetworkIDGenerator::GET_BASE_OBJECT_FROM_ID( NetworkID x )
{
	if ( x == UNASSIGNED_NETWORK_ID )
		return 0;

	NetworkIDNode *n = NetworkIDGenerator::IDTree.GetPointerToNode( NetworkIDNode( ( x ), 0 ) );

	if ( n )
	{
		return n->object;
	}

	return 0;
}
//-------------------------------------------------------------------------------------
void* NetworkIDGenerator::GET_OBJECT_FROM_ID( NetworkID x )
{
	NetworkIDGenerator *object = (NetworkIDGenerator *) GET_BASE_OBJECT_FROM_ID( x );
	if (object)
	{
		if (object->GetParent())
		{
			return object->GetParent();
		}
		else
		{
#ifdef _DEBUG
			// If this assert hit then this object requires a call to SetParent and it never got one.
			assert(object->RequiresSetParent()==false);
#endif
			return object;
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------
void* GET_OBJECT_FROM_ID( NetworkID x )
{
	return NetworkIDGenerator::GET_OBJECT_FROM_ID( x );
}


//////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////
