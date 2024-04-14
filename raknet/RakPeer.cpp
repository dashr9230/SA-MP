// TODO: Implement RakPeer.cpp

#include "RakPeer.h"

#include <string.h>
#include <stdlib.h>

RakPeer::RakPeer()
{
	memset( frequencyTable, 0, sizeof( unsigned int ) * 256 );
	rawBytesSent = rawBytesReceived = compressedBytesSent = compressedBytesReceived = 0;
	MTUSize = DEFAULT_MTU_SIZE;
	trackFrequencyTable = false;
	maximumIncomingConnections = 0;
	maximumNumberOfPeers = 0;
	endThreads = true;
	myPlayerId = UNASSIGNED_PLAYER_ID;
	incomingPasswordLength=0;

	// TODO: RakPeer ctor saco .text:1003DE50 server W .text:00455140 L .text:08072970 bot W .text:00408DF0 L .text:08071AB0
}

void RakPeer::vftable_0()
{
	// TODO: RakPeer::vftable_0() (saco W: 10042780) (server W: 459CC0 L: 8072430) (bot W: 40D490 L: 807229E)
}

void RakPeer::vftable_4()
{
	// TODO: RakPeer::vftable_4() (saco W: 100427A0) (server W: 459CE0 L: 8072190) (bot W: 40D4B0 L: 80725E2)
}

void RakPeer::vftable_8()
{
	// TODO: RakPeer::vftable_8() (saco W: 10042A40) (server W: 459F80 L: 806EE80) (bot W: 40D750 L: 8072926)
}

void RakPeer::vftable_C()
{
	// TODO: RakPeer::vftable_C() (saco W: 10038400) (server W: 44FF00 L: 8076430) (bot W: 403EA0 L: 8072E54)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Sets how many incoming connections are allowed.  If this is less than the number of players currently connected, no
// more players will be allowed to connect.  If this is greater than the maximum number of peers allowed, it will be reduced
// to the maximum number of peers allowed.  Defaults to 0.
//
// Parameters:
// numberAllowed - Maximum number of incoming connections allowed.
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::SetMaximumIncomingConnections( unsigned short numberAllowed )
{
	maximumIncomingConnections = numberAllowed;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Returns the maximum number of incoming connections, which is always <= maxConnections
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned short RakPeer::GetMaximumIncomingConnections( void ) const
{
	return maximumIncomingConnections;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Sets the password incoming connections must match in the call to Connect (defaults to none)
// Pass 0 to passwordData to specify no password
//
// Parameters:
// passwordData: A data block that incoming connections must match.  This can be just a password, or can be a stream of data.
// - Specify 0 for no password data
// passwordDataLength: The length in bytes of passwordData
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::SetIncomingPassword( const char* passwordData, int passwordDataLength )
{
	//if (passwordDataLength > MAX_OFFLINE_DATA_LENGTH)
	//	passwordDataLength=MAX_OFFLINE_DATA_LENGTH;

	if (passwordDataLength > 255)
		passwordDataLength=255;

	if (passwordData==0)
		passwordDataLength=0;

	// Not threadsafe but it's not important enough to lock.  Who is going to change the password a lot during runtime?
	// It won't overflow at least because incomingPasswordLength is an unsigned char
	if (passwordDataLength>0)
		memcpy(incomingPassword, passwordData, passwordDataLength);
	incomingPasswordLength=(unsigned char)passwordDataLength;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::GetIncomingPassword( char* passwordData, int *passwordDataLength  )
{
	if (passwordData==0)
	{
		*passwordDataLength=incomingPasswordLength;
		return;
	}

	if (*passwordDataLength > incomingPasswordLength)
		*passwordDataLength=incomingPasswordLength;

	if (*passwordDataLength>0)
		memcpy(passwordData, incomingPassword, *passwordDataLength);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Call this to connect to the specified host (ip or domain name) and server port.
// Calling Connect and not calling SetMaximumIncomingConnections acts as a dedicated client.  Calling both acts as a true peer.
// This is a non-blocking connection.  You know the connection is successful when IsConnected() returns true
// or receive gets a packet with the type identifier ID_CONNECTION_ACCEPTED.  If the connection is not
// successful, such as rejected connection or no response then neither of these things will happen.
// Requires that you first call Initialize
//
// Parameters:
// host: Either a dotted IP address or a domain name
// remotePort: Which port to connect to on the remote machine.
// passwordData: A data block that must match the data block on the server.  This can be just a password, or can be a stream of data
// passwordDataLength: The length in bytes of passwordData
//
// Returns:
// True on successful initiation. False on incorrect parameters, internal error, or too many existing peers
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RakPeer::Connect( const char* host, unsigned short remotePort, char* passwordData, int passwordDataLength )
{
	// TODO: RakPeer::vftable_20() (saco W: 10040550) (server W: 457B00 L: 806D230) (bot W: 40B2C0 L: 807306A)
	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Stops the network threads and close all connections.  Multiple calls are ok.
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::Disconnect( unsigned int blockDuration, unsigned char orderingChannel )
{
	// TODO: RakPeer::vftable_24() (saco W: 10040620) (server W: 457BD0 L: 806FB70) (bot W: 40B390 L: 80730D2)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Returns true if the network threads are running
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool RakPeer::IsActive( void ) const
{
	return endThreads == false;
}

void RakPeer::vftable_2C()
{
	// TODO: RakPeer::vftable_2C() (saco W: 100384D0) (server W: 44FFD0 L: 8076E70) (bot W: 403F70 L: 8080FB0)
}

void RakPeer::vftable_30()
{
	// TODO: RakPeer::vftable_30() (saco W: 1003C3D0) (server W: 4536E0 L: 806D2B0) (bot W: 407380 L: 8073546)
}

void RakPeer::vftable_34()
{
	// TODO: RakPeer::vftable_34() (saco W: 1003C2C0) (server W: 4535D0 L: 806EBA0) (bot W: 407270 L: 8073696)
}

void RakPeer::vftable_38()
{
	// TODO: RakPeer::vftable_38() (saco W: 10040FD0) (server W: 458510 L: 806EA30) (bot W: 40BCE0 L: 8073806)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Call this to deallocate a packet returned by Receive, in the same order returned to you from Receive
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::DeallocatePacket( Packet *packet )
{
	if ( packet == 0 )
		return;

	if (packet->deleteData)
		if (packet->data)
			delete packet->data;

	free(packet);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Return the total number of connections we are allowed
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned short RakPeer::GetMaximumNumberOfPeers( void ) const
{
	return maximumNumberOfPeers;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Register a C function as available for calling as a remote procedure call
//
// Parameters:
// uniqueID: A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions
// functionName(...): The name of the C function or C++ singleton to be used as a function pointer
// This can be called whether the client is active or not, and registered functions stay registered unless unregistered with
// UnregisterAsRemoteProcedureCall
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )
{
	if ( uniqueID == 0 || uniqueID[ 0 ] == 0 || functionPointer == 0 )
		return;

	rpcMap.AddIdentifierWithFunction((unsigned char)*uniqueID, (void*)functionPointer, false);
}

void RakPeer::RegisterClassMemberRPC( char* uniqueID, void *functionPointer )
{
	if ( uniqueID == 0 || uniqueID[ 0 ] == 0 || functionPointer == 0 )
		return;

	rpcMap.AddIdentifierWithFunction((unsigned char)*uniqueID, functionPointer, true);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Unregisters a C function as available for calling as a remote procedure call that was formerly registered
// with RegisterAsRemoteProcedureCall
//
// Parameters:
// uniqueID: A null terminated string to identify this procedure.  Must match the parameter
// passed to RegisterAsRemoteProcedureCall
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::UnregisterAsRemoteProcedureCall( char* uniqueID )
{
	// nothing
}

void RakPeer::vftable_50()
{
	// TODO: RakPeer::vftable_50() (saco W: 1003A720) (server W: 451A60 L: 806D580) (bot W: 405750 L: 8073DAA)
}

void RakPeer::vftable_54()
{
	// TODO: RakPeer::vftable_54() (saco W: 1003E050) (server W: 455340 L: 8071710) (bot W: 408FF0 L: 8073DB6)
}

void RakPeer::vftable_58()
{
	// TODO: RakPeer::vftable_58() (saco W: 10040A00) (server W: 457FB0 L: 806D590) (bot W: 40B770 L: 80741A0)
}

void RakPeer::vftable_5C()
{
	// TODO: RakPeer::vftable_5C() (saco W: 1003E330) (server W: 455620 L: 8074290) (bot W: 4092D0 L: 80742D2)
}

void RakPeer::vftable_60()
{
	// TODO: RakPeer::vftable_60() (saco W: 10038660) (server W: 450160 L: 80716E0) (bot W: 404100 L: 8074318)
}

void RakPeer::vftable_64()
{
	// TODO: RakPeer::vftable_64() (saco W: 1003A7D0) (server W: 451B10 L: 806D6B0) (bot W: 405800 L: 8074342)
}

void RakPeer::vftable_68()
{
	// TODO: RakPeer::vftable_68() (saco W: 1003C4D0) (server W: 4537E0 L: 806D770) (bot W: 407480 L: 807441A)
}

void RakPeer::vftable_6C()
{
	// TODO: RakPeer::vftable_6C() (saco W: 1003A960) (server W: 451CA0 L: 806F440) (bot W: 405990 L: 80745D6)
}

void RakPeer::vftable_70()
{
	// TODO: RakPeer::vftable_70() (saco W: 1003C5E0) (server W: 4538F0 L: 806D8E0) (bot W: 407590 L: 807473C)
}

void RakPeer::vftable_74()
{
	// TODO: RakPeer::vftable_74() (saco W: 1003A9D0) (server W: 451D10 L: 806F300) (bot W: 405A00 L: 80747FC)
}

void RakPeer::vftable_78()
{
	// TODO: RakPeer::vftable_78() (saco W: 10040A30) (server W: 457FE0 L: 8070970) (bot W: 40B7A0 L: 8074A70)
}

void RakPeer::vftable_7C()
{
	// TODO: RakPeer::vftable_7C() (saco W: 1003E350) (server W: 455640 L: 806D9A0) (bot W: 4092F0 L: 8074A9A)
}

void RakPeer::vftable_80()
{
	// TODO: RakPeer::vftable_80() (saco W: 1003E3B0) (server W: 4556A0 L: 8070270) (bot W: 409350 L: 8074CB4)
}

void RakPeer::vftable_84()
{
	// TODO: RakPeer::vftable_84() (saco W: 1003E400) (server W: 4556F0 L: 8070210) (bot W: 4093A0 L: 8074D7A)
}

void RakPeer::vftable_88()
{
	// TODO: RakPeer::vftable_88() (saco W: 10038720) (server W: 450220 L: 80701D0) (bot W: 404160 L: 8074DFE)
}

void RakPeer::vftable_8C()
{
	// TODO: RakPeer::vftable_8C() (saco W: 1003E440) (server W: 455730 L: 806DBB0) (bot W: 4093E0 L: 8074E52)
}

void RakPeer::vftable_90()
{
	// TODO: RakPeer::vftable_90() (saco W: 1003E490) (server W: 455780 L: 8070160) (bot W: 409430 L: 8074E6A)
}

void RakPeer::vftable_94()
{
	// TODO: RakPeer::vftable_94() (saco W: 10040A50) (server W: 458000 L: 80700B0) (bot W: 40B7C0 L: 8074EE2)
}

void RakPeer::vftable_98()
{
	// TODO: RakPeer::vftable_98() (saco W: 10038730) (server W: 450230 L: 8070B60) (bot W: 404170 L: 8074FA8)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Return the unique PlayerID that represents you on the the network
// Note that unlike in previous versions, this is a struct and is not sequential
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PlayerID RakPeer::GetInternalID( void ) const
{
	return myPlayerId;
}

void RakPeer::vftable_A0()
{
	// TODO: RakPeer::vftable_A0() (saco W: 100387A0) (server W: 4502A0 L: 806DC50) (bot W: 4041E0 L: 8075034)
}

void RakPeer::vftable_A4()
{
	// TODO: RakPeer::vftable_A4() (saco W: 1003E520) (server W: 455810 L: 806DC70) (bot W: 4094C0 L: 8075056)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Change the MTU size in order to improve performance when sending large packets
// This can only be called when not connected.
// A too high of value will cause packets not to arrive at worst and be fragmented at best.
// A too low of value will split packets unnecessarily.
//
// Parameters:
// size: Set according to the following table:
// 1500. The largest Ethernet packet size
// This is the typical setting for non-PPPoE, non-VPN connections. The default value for NETGEAR routers, adapters and switches.
// 1492. The size PPPoE prefers.
// 1472. Maximum size to use for pinging. (Bigger packets are fragmented.)
// 1468. The size DHCP prefers.
// 1460. Usable by AOL if you don't have large email attachments, etc.
// 1430. The size VPN and PPTP prefer.
// 1400. Maximum size for AOL DSL.
// 576. Typical value to connect to dial-up ISPs. (Default)
//
// Returns:
// False on failure (we are connected).  True on success.  Maximum allowed size is MAXIMUM_MTU_SIZE
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RakPeer::SetMTUSize( int size )
{
	if ( IsActive() )
		return false;

	if ( size < 512 )
		size = 512;
	else if ( size > MAXIMUM_MTU_SIZE )
		size = MAXIMUM_MTU_SIZE;

	MTUSize = size;

	return true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Returns the current MTU size
//
// Returns:
// The MTU sized specified in SetMTUSize
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int RakPeer::GetMTUSize( void ) const
{
	return MTUSize;
}

void RakPeer::vftable_B0()
{
	// TODO: RakPeer::vftable_B0() (saco W: 100388C0) (server W: 4503C0 L: 806DDB0) (bot W: 404300 L: 80751FE)
}

void RakPeer::vftable_B4()
{
	// TODO: RakPeer::vftable_B4() (saco W: 10038920) (server W: 450420 L: 806DDC0) (bot W: 404360 L: 807520C)
}

void RakPeer::vftable_B8()
{
	// TODO: RakPeer::vftable_B8() (saco W: 10038910) (server W: 450410 L: 806DE40) (bot W: 404350 L: 807529A)
}

void RakPeer::vftable_BC()
{
	// TODO: RakPeer::vftable_BC() (saco W: 10038CB0) (server W: 450790 L: 806DE30) (bot W: 4046D0 L: 8075280)
}

void RakPeer::vftable_C0()
{
	// TODO: RakPeer::vftable_C0() (saco W: 10038960) (server W: 450460 L: 806E690) (bot W: 4043A0 L: 8075F86)
}

void RakPeer::vftable_C4()
{
	// TODO: RakPeer::vftable_C4() (saco W: 1003AB30) (server W: 451E70 L: 806DEA0) (bot W: 405B60 L: 80752EC)
}

void RakPeer::vftable_C8()
{
	// TODO: RakPeer::vftable_C8() (saco W: 10038970) (server W: 450470 L: 806DEC0) (bot W: 4043B0 L: 8075308)
}

void RakPeer::vftable_CC()
{
	// TODO: RakPeer::vftable_CC() (saco W: 100389C0) (server W: 4504C0 L: 806E110) (bot W: 404400 L: 807558A)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Enables or disables our tracking of bytes input to and output from the network.
// This is required to get a frequency table, which is used to generate a new compression layer.
// You can call this at any time - however you SHOULD only call it when disconnected.  Otherwise you will only track
// part of the values sent over the network.
// This value persists between connect calls and defaults to false (no frequency tracking)
//
// Parameters:
// doCompile - true to track bytes.  Defaults to false
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RakPeer::SetCompileFrequencyTable( bool doCompile )
{
	trackFrequencyTable = doCompile;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
// Returns the frequency of outgoing bytes into outputFrequencyTable
// The purpose is to save to file as either a master frequency table from a sample game session for passing to
// GenerateCompressionLayer(false)
// You should only call this when disconnected.
// Requires that you first enable data frequency tracking by calling SetCompileFrequencyTable(true)
//
// Parameters:
// outputFrequencyTable (out): The frequency of each corresponding byte
//
// Returns:
// Ffalse (failure) if connected or if frequency table tracking is not enabled.  Otherwise true (success)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool RakPeer::GetOutgoingFrequencyTable( unsigned int outputFrequencyTable[ 256 ] )
{
	if ( IsActive() )
		return false;

	if ( trackFrequencyTable == false )
		return false;

	memcpy( outputFrequencyTable, frequencyTable, sizeof( unsigned int ) * 256 );

	return true;
}

void RakPeer::vftable_D8()
{
	// TODO: RakPeer::vftable_D8() (saco W: 10038A60) (server W: 450560 L: 806E1F0) (bot W: 4044A0 L: 8075666)
}

void RakPeer::vftable_DC()
{
	// TODO: RakPeer::vftable_DC() (saco W: 1003ACA0) (server W: 451FE0 L: 806E250) (bot W: 405CD0 L: 80756CC)
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns:
// The compression ratio.  A low compression ratio is good.  Compression is for outgoing data
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float RakPeer::GetCompressionRatio( void ) const
{
	if ( rawBytesSent > 0 )
	{
		return ( float ) compressedBytesSent / ( float ) rawBytesSent;
	}

	else
		return 0.0f;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns:
// The decompression ratio.  A high decompression ratio is good.  Decompression is for incoming data
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float RakPeer::GetDecompressionRatio( void ) const
{
	if ( rawBytesReceived > 0 )
	{
		return ( float ) compressedBytesReceived / ( float ) rawBytesReceived;
	}

	else
		return 0.0f;
}

void RakPeer::vftable_E8()
{
	// TODO: RakPeer::vftable_E8() (saco W: 1003AD20) (server W: 452060 L: 806E400) (bot W: 405D50 L: 8075910)
}

void RakPeer::vftable_EC()
{
	// TODO: RakPeer::vftable_EC() (saco W: 1003AD60) (server W: 4520A0 L: 806E440) (bot W: 405D90 L: 8075962)
}

void RakPeer::vftable_F0()
{
	// TODO: RakPeer::vftable_F0() (saco W: 1003ADB0) (server W: 4520F0 L: 806E4A0) (bot W: 405DE0 L: 80759B4)
}

void RakPeer::vftable_F4()
{
	// TODO: RakPeer::vftable_F4() (saco W: 10038BD0) (server W: 4506B0 L: 806E510) (bot W: 4045F0 L: 8075A76)
}

void RakPeer::vftable_F8()
{
	// TODO: RakPeer::vftable_F8() (saco W: 10038BE0) (server W: 4506C0 L: 806E560) (bot W: 404600 L: 8075AC2)
}

void RakPeer::vftable_FC()
{
	// TODO: RakPeer::vftable_FC() (saco W: 10038C00) (server W: 4506E0 L: 806E580) (bot W: 404620 L: 8075AD4)
}

void RakPeer::vftable_100()
{
	// TODO: RakPeer::vftable_100() (saco W: 10038C80) (server W: 450760 L: 806E5A0) (bot W: 4046A0 L: 8075AF4)
}

void RakPeer::vftable_104()
{
	// TODO: RakPeer::vftable_104() (saco W: 1003E5C0) (server W: 4558B0 L: 806E650) (bot W: 409560 L: 8075B92)
}

void RakPeer::vftable_108()
{
	// TODO: RakPeer::vftable_108() (saco W: 1003EC80) (server W: 456030 L: 806FC90) (bot W: 409C20 L: 8075C52)
}

void RakPeer::vftable_10C()
{
	// TODO: RakPeer::vftable_10C() (saco W: 1003E560) (server W: 455850 L: 806F710) (bot W: 409500 L: 8076B8C)
}

void RakPeer::vftable_110()
{
	// TODO: RakPeer::vftable_110() (server L: 806FFE0) (bot L: 8075BDC)
}
