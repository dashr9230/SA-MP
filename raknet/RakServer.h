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
	void vftable_10();
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
	void vftable_114();
	void vftable_118();
	void vftable_11C();
	void vftable_120();
	void vftable_124();

};

#endif
