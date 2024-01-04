// TODO: Implement RPCMap.h

#ifndef __RPC_MAP
#define __RPC_MAP

#include "RPCNode.h"
#include "Export.h"

#define RPC_MAP_SIZE 256

struct RAK_DLL_EXPORT RPCMap
{
public:
	void AddIdentifierWithFunction(unsigned char uniqueIdentifier, void *functionPointer, bool isPointerToMember);
protected:
	RPCNode *rpcSet[RPC_MAP_SIZE];
};

#endif
