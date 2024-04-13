// TODO: Implement RakPeer.h

#ifndef __RAK_PEER_H
#define __RAK_PEER_H

#include "Export.h"
#include "RakPeerInterface.h"
#include "RPCNode.h"
#include "RPCMap.h"

class RAK_DLL_EXPORT RakPeer : public RakPeerInterface
{
public:
	///Constructor
	RakPeer();

	void vftable_0();
	void vftable_4();
	void vftable_8();
	void vftable_C();

	/// Sets how many incoming connections are allowed. If this is less than the number of players currently connected,
	/// no more players will be allowed to connect.  If this is greater than the maximum number of peers allowed,
	/// it will be reduced to the maximum number of peers allowed.  Defaults to 0.
	/// \param[in] numberAllowed Maximum number of incoming connections allowed.
	void SetMaximumIncomingConnections( unsigned short numberAllowed );

	/// Returns the value passed to SetMaximumIncomingConnections()
	/// \return the maximum number of incoming connections, which is always <= maxConnections
	unsigned short GetMaximumIncomingConnections( void ) const;

	/// Sets the password incoming connections must match in the call to Connect (defaults to none). Pass 0 to passwordData to specify no password
	/// This is a way to set a low level password for all incoming connections.  To selectively reject connections, implement your own scheme using CloseConnection() to remove unwanted connections
	/// \param[in] passwordData A data block that incoming connections must match.  This can be just a password, or can be a stream of data. Specify 0 for no password data
	/// \param[in] passwordDataLength The length in bytes of passwordData
	void SetIncomingPassword( const char* passwordData, int passwordDataLength );

	/// Gets the password passed to SetIncomingPassword
	/// \param[out] passwordData  Should point to a block large enough to hold the password data you passed to SetIncomingPassword()
	/// \param[in,out] passwordDataLength Maximum size of the array passwordData.  Modified to hold the number of bytes actually written
	void GetIncomingPassword( char* passwordData, int *passwordDataLength  );

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
	bool Connect( const char* host, unsigned short remotePort, char* passwordData, int passwordDataLength );

	/// \brief Stops the network threads and closes all connections.
	/// \param[in] blockDuration How long you should wait for all remaining messages to go out, including ID_DISCONNECTION_NOTIFICATION.  If 0, it doesn't wait at all.
	/// \param[in] orderingChannel If blockDuration > 0, ID_DISCONNECTION_NOTIFICATION will be sent on this channel
	/// If you set it to 0 then the disconnection notification won't be sent
	void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 );

	/// Returns if the network thread is running
	/// \return true if the network thread is running, false otherwise
	bool IsActive( void ) const;

	void vftable_2C();
	void vftable_30();
	void vftable_34();
	void vftable_38();

	/// Call this to deallocate a message returned by Receive() when you are done handling it.
	/// \param[in] packet The message to deallocate.	
	void DeallocatePacket( Packet *packet );

	/// Return the total number of connections we are allowed
	unsigned short GetMaximumNumberOfPeers( void ) const;

	// --------------------------------------------------------------------------------------------Remote Procedure Call Functions - Functions to initialize and perform RPC--------------------------------------------------------------------------------------------
	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID A null-terminated unique string to identify this procedure.  See RegisterClassMemberRPC() for class member functions.
	/// \param[in] functionPointer(...) The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	void RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) );

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID A null terminated string to identify this procedure. Recommended you use the macro REGISTER_CLASS_MEMBER_RPC to create the string.  Use RegisterAsRemoteProcedureCall() for static functions.
	/// \param[in] functionPointer The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa The sample ObjectMemberRPC.cpp
	void RegisterClassMemberRPC( char* uniqueID, void *functionPointer );

	/// \ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registered with RegisterAsRemoteProcedureCall. Only call offline.
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.
	void UnregisterAsRemoteProcedureCall( char* uniqueID );

	void vftable_50();
	void vftable_54();
	void vftable_58();
	void vftable_5C();
	void vftable_60();
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
	void vftable_E0();
	void vftable_E4();
	void vftable_E8();
	void vftable_EC();
	void vftable_F0();
	void vftable_F4();
	void vftable_F8();
	void vftable_FC();
	void vftable_100();
	void vftable_104();
	void vftable_108();
	void vftable_10C();
	void vftable_110();

protected:

	///Set this to true to terminate the Peer thread execution 
	volatile bool endThreads;

	///Store the maximum number of peers allowed to connect
	unsigned short maximumNumberOfPeers;
	///Store the maximum incoming connection allowed 
	unsigned short maximumIncomingConnections;

	char incomingPassword[256];
	unsigned char incomingPasswordLength;

	RPCMap rpcMap; // Can't use StrPtrHash because runtime insertions will screw up the indices

};

#endif
