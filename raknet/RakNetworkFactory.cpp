// TODO: Implement RakNetworkFactory.cpp

#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "RakServer.h"

RakServerInterface* RakNetworkFactory::GetRakServerInterface( void )
{
	return new RakServer;
}