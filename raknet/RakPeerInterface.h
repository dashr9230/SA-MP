// TODO: Implement RakPeerInterface.h

#ifndef __RAK_PEER_INTERFACE_H
#define __RAK_PEER_INTERFACE_H

#include "Export.h"

class RAK_DLL_EXPORT RakPeerInterface
{
public:

	virtual void vftable_0()=0;
	virtual void vftable_4()=0;
	virtual void vftable_8()=0;
	virtual void vftable_C()=0;
	virtual void vftable_1C()=0;
	virtual void vftable_20()=0;
	virtual void vftable_24()=0;
	virtual void vftable_28()=0;

	/// Sets how many incoming connections are allowed. If this is less than the number of players currently connected,
	/// no more players will be allowed to connect.  If this is greater than the maximum number of peers allowed,
	/// it will be reduced to the maximum number of peers allowed.  Defaults to 0.
	/// \param[in] numberAllowed Maximum number of incoming connections allowed.
	virtual void SetMaximumIncomingConnections( unsigned short numberAllowed )=0;

	/// Returns the value passed to SetMaximumIncomingConnections()
	/// \return the maximum number of incoming connections, which is always <= maxConnections
	virtual unsigned short GetMaximumIncomingConnections( void ) const=0;

	/// Sets the password incoming connections must match in the call to Connect (defaults to none). Pass 0 to passwordData to specify no password
	/// This is a way to set a low level password for all incoming connections.  To selectively reject connections, implement your own scheme using CloseConnection() to remove unwanted connections
	/// \param[in] passwordData A data block that incoming connections must match.  This can be just a password, or can be a stream of data. Specify 0 for no password data
	/// \param[in] passwordDataLength The length in bytes of passwordData
	virtual void SetIncomingPassword( const char* passwordData, int passwordDataLength )=0;

	/// Returns if the network thread is running
	/// \return true if the network thread is running, false otherwise
	virtual bool IsActive( void ) const=0;

	virtual void vftable_2C()=0;
	virtual void vftable_30()=0;
	virtual void vftable_34()=0;
	virtual void vftable_38()=0;
	virtual void vftable_3C()=0;
	virtual void vftable_40()=0;

	// --------------------------------------------------------------------------------------------Remote Procedure Call Functions - Functions to initialize and perform RPC--------------------------------------------------------------------------------------------
	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID A null-terminated unique string to identify this procedure.  See RegisterClassMemberRPC() for class member functions.
	/// \param[in] functionPointer(...) The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	virtual void RegisterAsRemoteProcedureCall( unsigned char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )=0;

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID A null terminated string to identify this procedure. Recommended you use the macro REGISTER_CLASS_MEMBER_RPC to create the string.  Use RegisterAsRemoteProcedureCall() for static functions.
	/// \param[in] functionPointer The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa The sample ObjectMemberRPC.cpp
	virtual void RegisterClassMemberRPC( unsigned char* uniqueID, void *functionPointer )=0;

	/// \ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registered with RegisterAsRemoteProcedureCall. Only call offline.
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.
	virtual void UnregisterAsRemoteProcedureCall( unsigned char* uniqueID )=0;

	virtual void vftable_50()=0;
	virtual void vftable_54()=0;
	virtual void vftable_58()=0;
	virtual void vftable_5C()=0;
	virtual void vftable_60()=0;
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
	virtual void vftable_E0()=0;
	virtual void vftable_E4()=0;
	virtual void vftable_E8()=0;
	virtual void vftable_EC()=0;
	virtual void vftable_F0()=0;
	virtual void vftable_F4()=0;
	virtual void vftable_F8()=0;
	virtual void vftable_FC()=0;
	virtual void vftable_100()=0;
	virtual void vftable_104()=0;
	virtual void vftable_108()=0;
	virtual void vftable_10C()=0;
	virtual void vftable_110()=0;

};

#endif
