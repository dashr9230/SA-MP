// TODO: Implement RakServer.cpp

#include "RakServer.h"

RakServer::RakServer()
{
	// TODO: RakServer ctor saco .text:100436F0 server W .text:0045AC20 L .text:0807B820 bot L .text:08081062
}

void RakServer::vftable_0()
{
	// TODO: RakServer::vftable_0() (server W: 45AC90 L: 807B8C0)
}

bool RakServer::Start( unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress )
{
	bool init;


	init=false;
	
	// TODO: RakServer::vftable_4() (server W: 45A130 L: 807B900)
	
	return init;
}

void RakServer::vftable_8()
{
	// TODO: RakServer::vftable_8() (server W: 45A100 L: 807B990)
}

void RakServer::vftable_C()
{
	// TODO: RakServer::vftable_C() (server W: 45A120 L: 807B950)
}

void RakServer::SetPassword( const char *_password )
{
	// TODO: RakServer::vftable_10() (server W: 45A1B0 L: 807B980)
}

void RakServer::vftable_14()
{
	// TODO: RakServer::vftable_14() (server W: 45A1F0 L: 807BA40)
}

void RakServer::Disconnect( unsigned int blockDuration, unsigned char orderingChannel )
{
	// TODO: RakServer::vftable_18() (server W: 45A210 L: 807BAB0)
}

void RakServer::vftable_1C()
{
	// TODO: RakServer::vftable_1C() (server W: 45A260 L: 807BAE0)
}

void RakServer::vftable_20()
{
	// TODO: RakServer::vftable_20() (server W: 45A220 L: 807BB00)
}

void RakServer::vftable_24()
{
	// TODO: RakServer::vftable_24() (server W: 45ACB0 L: 807BB60)
}

void RakServer::vftable_28()
{
	// TODO: RakServer::vftable_28() (server W: 45AD00 L: 807BBB0)
}

void RakServer::vftable_2C()
{
	// TODO: RakServer::vftable_2C() (server W: 45A2A0 L: 807BC30)
}

void RakServer::vftable_30()
{
	// TODO: RakServer::vftable_30() (server W: 45A2D0 L: 807BE80)
}

void RakServer::SetAllowedPlayers( unsigned short AllowedPlayers )
{
	RakPeer::SetMaximumIncomingConnections( AllowedPlayers );
}

unsigned short RakServer::GetAllowedPlayers( void ) const
{
	return RakPeer::GetMaximumIncomingConnections();
}

void RakServer::vftable_3C()
{
	// TODO: RakServer::vftable_3C() (server W: 45A300 L: 807BF00)
}

void RakServer::vftable_40()
{
	// TODO: RakServer::vftable_40() (server W: 45A320 L: 807BF20)
}

void RakServer::vftable_44()
{
	// TODO: RakServer::vftable_44() (server W: 45A360 L: 807BF50)
}

void RakServer::vftable_48()
{
	// TODO: RakServer::vftable_48() (server W: 45A380 L: 807BF90)
}

void RakServer::vftable_4C()
{
	// TODO: RakServer::vftable_4C() (server W: 45A3A0 L: 807BFA0)
}

void RakServer::vftable_50()
{
	// TODO: RakServer::vftable_50() (server W: 45A3C0 L: 807BFB0)
}

void RakServer::vftable_54()
{
	// TODO: RakServer::vftable_54() (server W: 45A3E0 L: 807BFC0)
}

void RakServer::vftable_58()
{
	// TODO: RakServer::vftable_58() (server W: 45A3F0 L: 807BFD0)
}

void RakServer::vftable_5C()
{
	// TODO: RakServer::vftable_5C() (server W: 45A400 L: 807BFF0)
}

void RakServer::vftable_60()
{
	// TODO: RakServer::vftable_60() (server W: 45A410 L: 807C010)
}

void RakServer::vftable_64()
{
	// TODO: RakServer::vftable_64() (server W: 45A420 L: 807C030)
}

void RakServer::vftable_68()
{
	// TODO: RakServer::vftable_68() (server W: 45A430 L: 807C040)
}

void RakServer::vftable_6C()
{
	// TODO: RakServer::vftable_6C() (server W: 45A440 L: 807C050)
}

void RakServer::vftable_70()
{
	// TODO: RakServer::vftable_70() (server W: 45A450 L: 807C060)
}

void RakServer::RegisterAsRemoteProcedureCall( char* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) )
{
	RakPeer::RegisterAsRemoteProcedureCall( uniqueID, functionPointer );
}

void RakServer::RegisterClassMemberRPC( char* uniqueID, void *functionPointer )
{
	RakPeer::RegisterClassMemberRPC( uniqueID, functionPointer );
}

void RakServer::UnregisterAsRemoteProcedureCall( char* uniqueID )
{
	RakPeer::UnregisterAsRemoteProcedureCall( uniqueID );
}

void RakServer::vftable_80()
{
	// TODO: RakServer::vftable_80() (server W: 45A5B0 L: 807C100)
}

void RakServer::vftable_84()
{
	// TODO: RakServer::vftable_84() (server W: 45A4F0 L: 807C120)
}

void RakServer::vftable_88()
{
	// TODO: RakServer::vftable_88() (server W: 45A490 L: 807C1A0)
}

void RakServer::vftable_8C()
{
	// TODO: RakServer::vftable_8C() (server W: 45A550 L: 807C2A0)
}

void RakServer::vftable_90()
{
	// TODO: RakServer::vftable_90() (server W: 45A610 L: 807C220)
}

void RakServer::vftable_94()
{
	// TODO: RakServer::vftable_94() (server W: 45A640 L: 807C320)
}

void RakServer::vftable_98()
{
	// TODO: RakServer::vftable_98() (server W: 45A650 L: 807C360)
}

void RakServer::vftable_9C()
{
	// TODO: RakServer::vftable_9C() (server W: 45A660 L: 807C380)
}

void RakServer::vftable_A0()
{
	// TODO: RakServer::vftable_A0() (server W: 45A670 L: 807C3A0)
}

void RakServer::vftable_A4()
{
	// TODO: RakServer::vftable_A4() (server W: 45A680 L: 807C3B0)
}

void RakServer::vftable_A8()
{
	// TODO: RakServer::vftable_A8() (server W: 45A690 L: 807C3C0)
}

void RakServer::vftable_AC()
{
	// TODO: RakServer::vftable_AC() (server W: 45A6A0 L: 807C3E0)
}

void RakServer::vftable_B0()
{
	// TODO: RakServer::vftable_B0() (server W: 45A6D0 L: 807C400)
}

void RakServer::vftable_B4()
{
	// TODO: RakServer::vftable_B4() (server W: 45A700 L: 807C430)
}

void RakServer::vftable_B8()
{
	// TODO: RakServer::vftable_B8() (server W: 45A710 L: 807C470)
}

void RakServer::vftable_BC()
{
	// TODO: RakServer::vftable_BC() (server W: 45A730 L: 807C490)
}

void RakServer::vftable_C0()
{
	// TODO: RakServer::vftable_C0() (server W: 45A740 L: 807C4A0)
}

void RakServer::vftable_C4()
{
	// TODO: RakServer::vftable_C4() (server W: 45A760 L: 807C4C0)
}

void RakServer::vftable_C8()
{
	// TODO: RakServer::vftable_C8() (server W: 45AEC0 L: 807C4D0)
}

void RakServer::vftable_CC()
{
	// TODO: RakServer::vftable_CC() (server W: 45A790 L: 807C4F0)
}

void RakServer::vftable_D0()
{
	// TODO: RakServer::vftable_D0() (server W: 45A7F0 L: 807C620)
}

void RakServer::vftable_D4()
{
	// TODO: RakServer::vftable_D4() (server W: 45A7A0 L: 807C6C0)
}

void RakServer::vftable_D8()
{
	// TODO: RakServer::vftable_D8() (server W: 45A7C0 L: 807C630)
}

void RakServer::vftable_DC()
{
	// TODO: RakServer::vftable_DC() (server W: 45A7D0 L: 807C660)
}

void RakServer::vftable_E0()
{
	// TODO: RakServer::vftable_E0() (server W: 45A7E0 L: 807C680)
}

void RakServer::vftable_E4()
{
	// TODO: RakServer::vftable_E4() (server W: 45A800 L: 807C6A0)
}

void RakServer::vftable_E8()
{
	// TODO: RakServer::vftable_E8() (server W: 45A820 L: 807C6E0)
}

void RakServer::vftable_EC()
{
	// TODO: RakServer::vftable_EC() (server W: 45A840 L: 807C6F0)
}

void RakServer::vftable_F0()
{
	// TODO: RakServer::vftable_F0() (server W: 45A860 L: 807C720)
}

void RakServer::vftable_F4()
{
	// TODO: RakServer::vftable_F4() (server W: 45A870 L: 807C750)
}

void RakServer::vftable_F8()
{
	// TODO: RakServer::vftable_F8() (server W: 45A880 L: 807C770)
}

void RakServer::vftable_FC()
{
	// TODO: RakServer::vftable_FC() (server W: 45A890 L: 807C790)
}

void RakServer::vftable_100()
{
	// TODO: RakServer::vftable_100() (server W: 45A8A0 L: 807C7A0)
}

void RakServer::vftable_104()
{
	// TODO: RakServer::vftable_104() (server W: 45A8D0 L: 807C7C0)
}

void RakServer::vftable_108()
{
	// TODO: RakServer::vftable_108() (server W: 45A900 L: 807C800)
}

void RakServer::vftable_10C()
{
	// TODO: RakServer::vftable_10C() (server W: 45A910 L: 807C820)
}

void RakServer::vftable_110()
{
	// TODO: RakServer::vftable_110() (server W: 45A920 L: 807C840)
}

void RakServer::vftable_114()
{
	// TODO: RakServer::vftable_114() (server W: 45A930 L: 807C850)
}

void RakServer::vftable_118()
{
	// TODO: RakServer::vftable_118() (server W: 45A950 L: 807C870)
}

void RakServer::vftable_11C()
{
	// TODO: RakServer::vftable_11C() (server W: 45A980 L: 807C880)
}

void RakServer::vftable_120()
{
	// TODO: RakServer::vftable_120() (server W: 45A9A0 L: 807C8A0)
}

void RakServer::vftable_124()
{
	// TODO: RakServer::vftable_124() (server L: 0807C8C0)
}
