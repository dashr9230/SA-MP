// TODO: Implement RakClientInterface.h

#ifndef __RAK_CLIENT_INTERFACE_H
#define __RAK_CLIENT_INTERFACE_H

class RakClientInterface
{

public:

	virtual void vftable_0()=0;
	virtual void vftable_4()=0;

	/// Stops the client, stops synchronized data, and resets all internal data.
	/// Does nothing if the client is not connected to begin wit
	/// \param[in] blockDuration how long you should wait for all remaining packets to go outIf you set it to 0 then the disconnection notification probably won't arrive
	/// \param[in] orderingChannel If blockDuration > 0, the disconnect packet will be sent on this channel
	virtual void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 )=0;

	virtual void vftable_C()=0;
	virtual void vftable_10()=0;
	virtual void vftable_14()=0;
	virtual void vftable_18()=0;
	virtual void vftable_1C()=0;
	virtual void vftable_20()=0;
	virtual void vftable_24()=0;
	virtual void vftable_28()=0;
	virtual void vftable_2C()=0;
	virtual void vftable_30()=0;
	virtual void vftable_34()=0;
	virtual void vftable_38()=0;
	virtual void vftable_3C()=0;
	virtual void vftable_40()=0;
	virtual void vftable_44()=0;
	virtual void vftable_48()=0;
	virtual void vftable_4C()=0;
	virtual void vftable_50()=0;
	virtual void vftable_54()=0;

	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID: A null-terminated unique string to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions
	/// \param[in] functionPointer(...): The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	virtual void RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )=0;

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID: A null terminated string to identify this procedure.Recommended you use the macro REGISTER_CLASS_MEMBER_RPC
	/// \param[in] functionPointer: The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa ObjectMemberRPC.cpp
	virtual void RegisterClassMemberRPC( char* uniqueID, void *functionPointer )=0;

	///\ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registeredwith RegisterAsRemoteProcedureCallOnly call offline
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.  Must match the parameterpassed to RegisterAsRemoteProcedureCall
	virtual void UnregisterAsRemoteProcedureCall( char* uniqueID )=0;

	virtual void vftable_64()=0;
	virtual void vftable_68()=0;
	virtual void vftable_6C()=0;
	virtual void vftable_70()=0;
	virtual void vftable_74()=0;
	virtual void vftable_78()=0;
	virtual void vftable_7C()=0;
	virtual void vftable_80()=0;
	virtual void vftable_84()=0;
	virtual void vftable_88()=0;
	virtual void vftable_8C()=0;
	virtual void vftable_90()=0;
	virtual void vftable_94()=0;
	virtual void vftable_98()=0;
	virtual void vftable_9C()=0;
	virtual void vftable_A0()=0;
	virtual void vftable_A4()=0;
	virtual void vftable_A8()=0;
	virtual void vftable_AC()=0;
	virtual void vftable_B0()=0;
	virtual void vftable_B4()=0;
	virtual void vftable_B8()=0;
	virtual void vftable_BC()=0;
	virtual void vftable_C0()=0;
	virtual void vftable_C4()=0;
	virtual void vftable_C8()=0;
	virtual void vftable_CC()=0;
	virtual void vftable_D0()=0;
	virtual void vftable_D4()=0;
	virtual void vftable_D8()=0;
	virtual void vftable_DC()=0;

};

#endif
