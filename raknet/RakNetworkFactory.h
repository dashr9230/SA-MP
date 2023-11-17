
// TODO: Implement RakNetworkFactory.h

#ifndef __RAK_NETWORK_FACTORY_H
#define __RAK_NETWORK_FACTORY_H

#include "Export.h"

class RakServerInterface;

class RAK_DLL_EXPORT RakNetworkFactory
{
public:

	static RakServerInterface* GetRakServerInterface( void );

};

#endif
