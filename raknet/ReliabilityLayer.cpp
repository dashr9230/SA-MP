// TODO: Implement ReliabilityLayer.cpp

#include "ReliabilityLayer.h"

static const float PING_MULTIPLIER_TO_RESEND=3.0; // So internet ping variation doesn't cause needless resends
static const RakNetTime MIN_PING_TO_RESEND=30; // So system timer changes and CPU lag don't send needless resends

ReliabilityLayer::ReliabilityLayer()
{
	// TODO: ReliabilityLayer::ReliabilityLayer

	InitializeVariables();
}

//-------------------------------------------------------------------------------------------------------
// Initialize the variables
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::InitializeVariables( void )
{
	// TODO: ReliabilityLayer::InitializeVariables

	memset( &statistics, 0, sizeof( statistics ) );
	statistics.connectionStartTime = RakNet::GetTime();
	statistics.field_110 = RakNet::GetTime();
	statistics.field_114 = 0;
	statistics.field_118 = 0;
	statistics.field_11C = RakNet::GetTime();
	statistics.field_120 = 0;
	statistics.field_124 = 0;
	field_403 = 4;

	SetPing( 1000 );
}

//-------------------------------------------------------------------------------------------------------
// How long to wait between packet resends
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SetPing( RakNetTime i )
{
	//assert(i < (RakNetTimeNS)timeoutTime*1000);
	if (i > timeoutTime)
		ping=500;
	else
		ping = i;
	if (ping < 30)
		ping=30; // Leave a buffer for variations in ping
#ifndef _RELEASE
	if (ping < (RakNetTime)(minExtraPing+extraPingVariance)*2)
		ping=(minExtraPing+extraPingVariance)*2;
#endif

	UpdateNextActionTime();
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
