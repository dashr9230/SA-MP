// TODO: Implement RakClient.h

#ifndef __RAK_CLIENT_H
#define __RAK_CLIENT_H

#include "RakPeer.h"
#include "RakClientInterface.h"

class RakClient : public RakPeer, public RakClientInterface
{

public:
	///Constructor
	RakClient();

	void vftable_0();
	void vftable_4();

	/// Stops the client, stops synchronized data, and resets all internal data.
	/// Does nothing if the client is not connected to begin wit
	/// \param[in] blockDuration how long you should wait for all remaining packets to go outIf you set it to 0 then the disconnection notification probably won't arrive
	/// \param[in] orderingChannel If blockDuration > 0, the disconnect packet will be sent on this channel
	void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 );

	void vftable_C();
	void vftable_10();
	void vftable_14();
	void vftable_18();
	void vftable_1C();
	void vftable_20();
	void vftable_24();
	void vftable_28();
	void vftable_2C();
	void vftable_30();
	void vftable_34();
	void vftable_38();
	void vftable_3C();
	void vftable_40();
	void vftable_44();

	/// Returns true if the client is connected to a responsive server
	/// \return true if connected to a server 
	bool IsConnected( void ) const;

	void vftable_4C();
	void vftable_50();
	void vftable_54();

	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID: A null-terminated unique string to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions
	/// \param[in] functionPointer(...): The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	void RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) );

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID: A null terminated string to identify this procedure.Recommended you use the macro REGISTER_CLASS_MEMBER_RPC
	/// \param[in] functionPointer: The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa ObjectMemberRPC.cpp
	void RegisterClassMemberRPC( char* uniqueID, void *functionPointer );

	/// \ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registeredwith RegisterAsRemoteProcedureCallOnly call offline
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.  Must match the parameterpassed to RegisterAsRemoteProcedureCall
	void UnregisterAsRemoteProcedureCall( char* uniqueID );

	void vftable_64();
	void vftable_68();
	void vftable_6C();
	void vftable_70();
	void vftable_74();
	void vftable_78();
	void vftable_7C();
	void vftable_80();
	void vftable_84();
	void vftable_88();
	void vftable_8C();
	void vftable_90();
	void vftable_94();
	void vftable_98();
	void vftable_9C();
	void vftable_A0();
	void vftable_A4();
	void vftable_A8();
	void vftable_AC();
	void vftable_B0();
	void vftable_B4();
	void vftable_B8();
	void vftable_BC();
	void vftable_C0();
	void vftable_C4();
	void vftable_C8();
	void vftable_CC();
	void vftable_D0();
	void vftable_D4();
	void vftable_D8();
	void vftable_DC();

private:

};

#endif
