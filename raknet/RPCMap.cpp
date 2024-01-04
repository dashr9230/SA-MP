// TODO: Implement RPCMap.cpp

// Called from the user thread for the local system
void RPCMap::AddIdentifierWithFunction(unsigned char uniqueIdentifier, void *functionPointer, bool isPointerToMember)
{
	RPCNode *node;

	node = new RPCNode;
	node->uniqueIdentifier = uniqueIdentifier;
	node->functionPointer=functionPointer;
	node->isPointerToMember=isPointerToMember;

	rpcSet[uniqueIdentifier] = node;
}
