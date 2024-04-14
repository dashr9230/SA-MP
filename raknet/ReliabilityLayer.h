// TODO: Implement ReliabilityLayer.h

#ifndef __RELIABILITY_LAYER_H
#define __RELIABILITY_LAYER_H

#include "SocketLayer.h"
#include "MTUSize.h"

class ReliabilityLayer
{
public:

	/// Constructor
	ReliabilityLayer();

private:

	// Initialize the variables
	void InitializeVariables( void );

	// Make it so we don't do resends within a minimum threshold of time
	void UpdateNextActionTime(void);

	char _gap0[713];

	RakNetTime ping;

	char _gap[1020];

	RakNetTimeNS ackTimeIncrement;
};

#endif
