
#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "RakServer.h"
#include "RakClient.h"
#include "RakPeerInterface.h"
#include "RakPeer.h"

RakClientInterface* RakNetworkFactory::GetRakClientInterface( void )
{
	return new RakClient;
}
RakServerInterface* RakNetworkFactory::GetRakServerInterface( void )
{
	return new RakServer;
}
RakPeerInterface* RakNetworkFactory::GetRakPeerInterface( void )
{
	return new RakPeer;
}




void RakNetworkFactory::DestroyRakClientInterface( RakClientInterface* i )
{
	delete ( RakClient* ) i;
}
void RakNetworkFactory::DestroyRakServerInterface( RakServerInterface* i )
{
	delete ( RakServer* ) i;
}
void RakNetworkFactory::DestroyRakPeerInterface( RakPeerInterface* i )
{
	delete ( RakPeer* ) i;
}
