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

#include "RPCMap.h"
#include <string.h>

RPCMap::RPCMap()
{
	unsigned i;
	for (i=0; i < RPC_SET_SIZE; i++)
		rpcSet[i] = 0;
}
RPCMap::~RPCMap()
{
	Clear();
}
void RPCMap::Clear(void)
{
	unsigned i;
	for (i=0; i < RPC_SET_SIZE; i++)
	{
		if(rpcSet[i])
			delete rpcSet[i];
		rpcSet[i] = 0;
	}
}
RPCNode *RPCMap::GetNodeFromIndex(RPCIndex index)
{
	return rpcSet[(unsigned)index];;
}
RPCNode *RPCMap::GetNodeFromFunctionName(unsigned char uniqueIdentifier)
{
	return rpcSet[uniqueIdentifier];
}
RPCIndex RPCMap::GetIndexFromFunctionName(unsigned char uniqueIdentifier)
{
	return (RPCIndex)uniqueIdentifier;
}

// Called from the user thread for the local system
void RPCMap::AddIdentifierWithFunction(unsigned char uniqueIdentifier, void *functionPointer, bool isPointerToMember)
{
	RPCNode *node;

	node = new RPCNode;
	node->uniqueIdentifier=uniqueIdentifier;
	node->functionPointer=functionPointer;
	node->isPointerToMember=isPointerToMember;

	rpcSet[uniqueIdentifier] = node;
}

