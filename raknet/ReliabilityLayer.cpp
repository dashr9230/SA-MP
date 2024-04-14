// TODO: Implement ReliabilityLayer.cpp

#include "ReliabilityLayer.h"

static const float PING_MULTIPLIER_TO_RESEND=3.0; // So internet ping variation doesn't cause needless resends
static const RakNetTime MIN_PING_TO_RESEND=30; // So system timer changes and CPU lag don't send needless resends

ReliabilityLayer::ReliabilityLayer()
{
	// TODO: ReliabilityLayer::ReliabilityLayer

	InitializeVariables();
}

void ReliabilityLayer::InitializeVariables( void )
{
	// TODO: ReliabilityLayer::InitializeVariables
}

//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::UpdateNextActionTime(void)
{
	//double multiple = log10(currentBandwidth/MINIMUM_SEND_BPS) / 0.30102999566398119521373889472449;
	if (ping*(RakNetTime)PING_MULTIPLIER_TO_RESEND < MIN_PING_TO_RESEND)
		ackTimeIncrement=(RakNetTimeNS)MIN_PING_TO_RESEND*1000;
	else
		ackTimeIncrement=(RakNetTimeNS)(ping*(RakNetTime)PING_MULTIPLIER_TO_RESEND)*1000;
}
