// TODO: Implement NetworkTypes.h

#ifndef __NETWORK_TYPES_H
#define __NETWORK_TYPES_H

// Define __GET_TIME_64BIT if you want to use large types for GetTime (takes more bandwidth when you transmit time though!)
// You would want to do this if your system is going to run long enough to overflow the millisecond counter (over a month)
#ifdef __GET_TIME_64BIT
typedef long long RakNetTime;
typedef long long RakNetTimeNS;
#else
typedef unsigned int RakNetTime;
typedef long long RakNetTimeNS;
#endif

#endif
