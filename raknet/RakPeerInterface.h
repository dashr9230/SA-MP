// TODO: Implement RakPeerInterface.h

#ifndef __RAK_PEER_INTERFACE_H
#define __RAK_PEER_INTERFACE_H

#include "NetworkTypes.h"
#include "Export.h"

class RAK_DLL_EXPORT RakPeerInterface
{
public:

	virtual void vftable_0()=0;
	virtual void vftable_4()=0;
	virtual void vftable_8()=0;
	virtual void vftable_C()=0;

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

	/// Gets the password passed to SetIncomingPassword
	/// \param[out] passwordData  Should point to a block large enough to hold the password data you passed to SetIncomingPassword()
	/// \param[in,out] passwordDataLength Maximum size of the array passwordData.  Modified to hold the number of bytes actually written
	virtual void GetIncomingPassword( char* passwordData, int *passwordDataLength  )=0;

	/// \brief Connect to the specified host (ip or domain name) and server port.
	/// Calling Connect and not calling SetMaximumIncomingConnections acts as a dedicated client.
	/// Calling both acts as a true peer. This is a non-blocking connection.
	/// You know the connection is successful when IsConnected() returns true or Receive() gets a message with the type identifier ID_CONNECTION_ACCEPTED.
	/// If the connection is not successful, such as a rejected connection or no response then neither of these things will happen.
	/// \pre Requires that you first call Initialize
	/// \param[in] host Either a dotted IP address or a domain name
	/// \param[in] remotePort Which port to connect to on the remote machine.
	/// \param[in] passwordData A data block that must match the data block on the server passed to SetIncomingPassword.  This can be a string or can be a stream of data.  Use 0 for no password.
	/// \param[in] passwordDataLength The length in bytes of passwordData
	/// \return True on successful initiation. False on incorrect parameters, internal error, or too many existing peers.  Returning true does not mean you connected!
	virtual bool Connect( const char* host, unsigned short remotePort, char* passwordData, int passwordDataLength )=0;

	/// \brief Stops the network threads and closes all connections.
	/// \param[in] blockDuration How long you should wait for all remaining messages to go out, including ID_DISCONNECTION_NOTIFICATION.  If 0, it doesn't wait at all.
	/// \param[in] orderingChannel If blockDuration > 0, ID_DISCONNECTION_NOTIFICATION will be sent on this channel
	/// If you set it to 0 then the disconnection notification won't be sent
	virtual void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 )=0;

	/// Returns if the network thread is running
	/// \return true if the network thread is running, false otherwise
	virtual bool IsActive( void ) const=0;

	virtual void vftable_2C()=0;
	virtual void vftable_30()=0;
	virtual void vftable_34()=0;
	virtual void vftable_38()=0;

	/// Call this to deallocate a message returned by Receive() when you are done handling it.
	/// \param[in] packet The message to deallocate.	
	virtual void DeallocatePacket( Packet *packet )=0;

	/// Return the total number of connections we are allowed
	// TODO - rename for RakNet 3.0
	virtual unsigned short GetMaximumNumberOfPeers( void ) const=0;

	// --------------------------------------------------------------------------------------------Remote Procedure Call Functions - Functions to initialize and perform RPC--------------------------------------------------------------------------------------------
	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID A null-terminated unique string to identify this procedure.  See RegisterClassMemberRPC() for class member functions.
	/// \param[in] functionPointer(...) The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	virtual void RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )=0;

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID A null terminated string to identify this procedure. Recommended you use the macro REGISTER_CLASS_MEMBER_RPC to create the string.  Use RegisterAsRemoteProcedureCall() for static functions.
	/// \param[in] functionPointer The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa The sample ObjectMemberRPC.cpp
	virtual void RegisterClassMemberRPC( char* uniqueID, void *functionPointer )=0;

	/// \ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registered with RegisterAsRemoteProcedureCall. Only call offline.
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.
	virtual void UnregisterAsRemoteProcedureCall( char* uniqueID )=0;

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
