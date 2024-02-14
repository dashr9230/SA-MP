// TODO: Implement RakServerInterface.h

#ifndef __RAK_SERVER_INTERFACE_H
#define __RAK_SERVER_INTERFACE_H

#include "NetworkTypes.h"
#include "Export.h"

/// This is a user-interface class to act as a game server.  All it does is implement some functionality on top of RakPeer.
/// See the individual functions for what the class can do.
/// \brief Defines the functions used by a game server
class RAK_DLL_EXPORT RakServerInterface
{

public:
	virtual void vftable_0()=0;
	virtual void vftable_4()=0;
	virtual void vftable_8()=0;
	virtual void vftable_C()=0;

	/// Set the password clients have to use to connect to this server. The password persists between connections.
	/// Pass 0 for no password.  You can call this anytime
	/// \param[in] _password The password string, or 0 for none.
	virtual void SetPassword( const char *_password )=0;

	virtual void vftable_14()=0;

	/// Stops the server, stops synchronized data, and resets all internal data.  This will drop all players currently connected, howeversince the server is stopped packet reliability is not enforced so the Kick network message may not actuallyarrive. Those players will disconnect due to timeout. If you want to end the server more gracefully, youcan manually Kick each player first. Does nothing if the server is not running to begin with
	/// \param[in] blockDuration The maximum amount of time to wait for all remaining packets to go out, including the disconnection notification. If you set it to 0 then the disconnection notifications probably won't arrive
	/// \param[in] orderingChannel If blockDuration > 0, the disconnect packet will be sent on this channel
	virtual void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 )=0;

	virtual void vftable_1C()=0;
	virtual void vftable_20()=0;
	virtual void vftable_24()=0;
	virtual void vftable_28()=0;
	virtual void vftable_2C()=0;
	virtual void vftable_30()=0;

	/// Set how many players are allowed on the server.
	/// If more players are currently connected then are allowed then no more players will be allowed to join until the number of players is less than the number of allowed players.
	/// \pre The server must be active for this to have meaning
	/// \param[in] AllowedPlayers The number of players to allow
	virtual void SetAllowedPlayers( unsigned short AllowedPlayers )=0;

	/// Return how many players are allowed to connect. This value was set either from Start or from SetAllowedPlayers.
	/// \pre The server must be active for this to have meaning
	/// \return The number of allowed players
	virtual unsigned short GetAllowedPlayers( void ) const=0;

	virtual void vftable_3C()=0;
	virtual void vftable_40()=0;
	virtual void vftable_44()=0;
	virtual void vftable_48()=0;
	virtual void vftable_4C()=0;
	virtual void vftable_50()=0;
	virtual void vftable_54()=0;
	virtual void vftable_58()=0;
	virtual void vftable_5C()=0;
	virtual void vftable_60()=0;
	virtual void vftable_64()=0;
	virtual void vftable_68()=0;
	virtual void vftable_6C()=0;
	virtual void vftable_70()=0;

	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID: A null-terminated unique string to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions
	/// \param[in] functionPointer  The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
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
	virtual void vftable_114()=0;
	virtual void vftable_118()=0;
	virtual void vftable_11C()=0;
	virtual void vftable_120()=0;
	virtual void vftable_124()=0;
};

#endif
