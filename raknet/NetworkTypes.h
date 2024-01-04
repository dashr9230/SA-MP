// TODO: Implement NetworkTypes.h

#ifndef __NETWORK_TYPES_H
#define __NETWORK_TYPES_H

// Define __GET_TIME_64BIT if you want to use large types for GetTime (takes more bandwidth when you transmit time though!)
// You would want to do this if your system is going to run long enough to overflow the millisecond counter (over a month)
#ifdef __GET_TIME_64BIT
typedef long long RakNetTime;
typedef long long RakNetTimeNS;
#else
typedef unsigned int RakNetTime;
typedef long long RakNetTimeNS;
#endif

struct RPCParameters
{
	char _gap0; // TODO: RPCParameters
};

/// \def REGISTER_STATIC_RPC
/// \ingroup RAKNET_RPC
/// Register a C function as a Remote procedure.
/// \param[in] networkObject Your instance of RakPeer, RakServer, or RakClient
/// \param[in] functionName The name of the C function to call
/// \attention 12/01/05 REGISTER_AS_REMOTE_PROCEDURE_CALL renamed to REGISTER_STATIC_RPC.  Delete the old name sometime in the future
//#pragma deprecated(REGISTER_AS_REMOTE_PROCEDURE_CALL)
//#define REGISTER_AS_REMOTE_PROCEDURE_CALL(networkObject, functionName) REGISTER_STATIC_RPC(networkObject, functionName)
#define REGISTER_STATIC_RPC(networkObject, functionName) (networkObject)->RegisterAsRemoteProcedureCall((RPC_##functionName),(functionName))

/// \def UNREGISTER_STATIC_RPC
/// \ingroup RAKNET_RPC
/// Unregisters a remote procedure call
/// RPC member Functions MUST be marked __cdecl!  See the ObjectMemberRPC example.
/// \param[in] networkObject The object that manages the function
/// \param[in] functionName The function name
// 12/01/05 UNREGISTER_AS_REMOTE_PROCEDURE_CALL Renamed to UNREGISTER_STATIC_RPC.  Delete the old name sometime in the future
//#pragma deprecated(UNREGISTER_AS_REMOTE_PROCEDURE_CALL)
//#define UNREGISTER_AS_REMOTE_PROCEDURE_CALL(networkObject,functionName) UNREGISTER_STATIC_RPC(networkObject,functionName)
#define UNREGISTER_STATIC_RPC(networkObject,functionName) (networkObject)->UnregisterAsRemoteProcedureCall((RPC_##functionName))

#endif
