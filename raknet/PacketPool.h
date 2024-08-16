/// \file
/// \brief \b [Internal] A pool for the Packet class.  I don't believe this is used any longer.
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.rakkarsoft.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

// TODO - Remove this file from the project
/*
#ifndef __PACKET_POOL
#define __PACKET_POOL
#include "SimpleMutex.h"
#include "DS_Queue.h"

struct Packet;

class PacketPool
{

public:
	PacketPool();
	~PacketPool();

	static void AddReference(void);
	static void RemoveReference(void);
	Packet* GetPointer( void );
	void ReleasePointer( Packet *p );
	void ClearPool( void );
	static inline PacketPool* Instance()
	{
		return I;
	}

private:
	DataStructures::Queue<Packet*> pool;
	SimpleMutex poolMutex;
	static PacketPool *I;
	static int referenceCount;
#ifdef _DEBUG
	int packetsReleased;
#endif
};

#endif

*/