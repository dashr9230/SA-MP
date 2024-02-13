// TODO: Implement NetworkTypes.cpp

#include "NetworkTypes.h"

bool PlayerID::operator==( const PlayerID& right ) const
{
	return binaryAddress == right.binaryAddress && port == right.port;
}
