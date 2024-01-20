// TODO: Implement RakServer.h

#ifndef __RAK_SERVER_H
#define __RAK_SERVER_H

#include "RakPeer.h"
#include "RakServerInterface.h"
#include "Export.h"

/// This is a user-interface class to act as a game server.  All it does is implement some functionality on top of RakPeer.
/// See the individual functions for what the class can do.
/// \brief Defines the functions used by a game server
class RAK_DLL_EXPORT RakServer : public RakServerInterface, public RakPeer
{

public:

	void vftable_0();
	void vftable_4();
	void vftable_8();
	void vftable_C();

	/// Set the password clients have to use to connect to this server. The password persists between connections.
	/// Pass 0 for no password.  You can call this anytime
	/// \param[in] _password The password string, or 0 for none.
	void SetPassword( const char *_password );

	void vftable_14();
	void vftable_18();
	void vftable_1C();
	void vftable_20();
	void vftable_24();
	void vftable_28();
	void vftable_2C();
	void vftable_30();

	/// Set how many players are allowed on the server.
	/// If more players are currently connected then are allowed then no more players will be allowed to join until the number of players is less than the number of allowed players.
	/// \pre The server must be active for this to have meaning
	/// \param[in] AllowedPlayers The number of players to allow
	void SetAllowedPlayers( unsigned short AllowedPlayers );

	/// Return how many players are allowed to connect. This value was set either from Start or from SetAllowedPlayers.
	/// \pre The server must be active for this to have meaning
	/// \return The number of allowed players
	unsigned short GetAllowedPlayers( void ) const;

	void vftable_3C();
	void vftable_40();
	void vftable_44();
	void vftable_48();
	void vftable_4C();
	void vftable_50();
	void vftable_54();
	void vftable_58();
	void vftable_5C();
	void vftable_60();
	void vftable_64();
	void vftable_68();
	void vftable_6C();
	void vftable_70();

	/// \ingroup RAKNET_RPC
	/// Register a C or static member function as available for calling as a remote procedure call
	/// \param[in] uniqueID: A null-terminated unique string to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions
	/// \param[in] functionPointer  The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered
	void RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) );

	/// \ingroup RAKNET_RPC
	/// Register a C++ member function as available for calling as a remote procedure call.
	/// \param[in] uniqueID: A null terminated string to identify this procedure.Recommended you use the macro REGISTER_CLASS_MEMBER_RPC
	/// \param[in] functionPointer: The name of the function to be used as a function pointer. This can be called whether active or not, and registered functions stay registered unless unregistered with UnregisterAsRemoteProcedureCall
	/// \sa ObjectMemberRPC.cpp
	void RegisterClassMemberRPC( char* uniqueID, void *functionPointer );

	///\ingroup RAKNET_RPC
	/// Unregisters a C function as available for calling as a remote procedure call that was formerly registeredwith RegisterAsRemoteProcedureCallOnly call offline
	/// \param[in] uniqueID A string of only letters to identify this procedure.  Recommended you use the macro CLASS_MEMBER_ID for class member functions.  Must match the parameterpassed to RegisterAsRemoteProcedureCall
	void UnregisterAsRemoteProcedureCall( char* uniqueID );

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
	void vftable_114();
	void vftable_118();
	void vftable_11C();
	void vftable_120();
	void vftable_124();

};

#endif
