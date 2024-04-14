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

	/// Sets the ping, which is used by the reliability layer to determine how long to wait for resends.  Mostly for flow control.
	/// \param[in] The ping time.
	void SetPing( RakNetTime i );

private:

	// Initialize the variables
	void InitializeVariables( void );

	// Make it so we don't do resends within a minimum threshold of time
	void UpdateNextActionTime(void);

	char _gap0[713];

	RakNetTime ping;

	char _gap2D1[2];

	RakNetTime timeoutTime; // How long to wait in MS before timing someone out

	char _gap2D7[1014];

	RakNetTimeNS ackTimeIncrement;

	char _gap6D1[25];

	// Internet simulator
	RakNetTime minExtraPing, extraPingVariance;

};

#endif
