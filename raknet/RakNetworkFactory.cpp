// TODO: Implement RakNetworkFactory.cpp

#include "RakNetworkFactory.h"
#include "RakServerInterface.h"

#include "RakServer.h"

#include "RakPeerInterface.h"
#include "RakPeer.h"

RakServerInterface* RakNetworkFactory::GetRakServerInterface( void )
{
	return new RakServer;
}
RakPeerInterface* RakNetworkFactory::GetRakPeerInterface( void )
{
	return new RakPeer;
}