/// \file
/// \brief Fully connected mesh plugin.  This will connect RakPeer to all connecting peers, and all peers the connecting peer knows about.
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

#ifndef __FULLY_CONNECTED_MESH_H
#define __FULLY_CONNECTED_MESH_H

class RakPeerInterface;
#include "PluginInterface.h"

/// \defgroup FULLY_CONNECTED_MESH_GROUP FullyConnectedMesh
/// \ingroup PLUGINS_GROUP

/// Fully connected mesh plugin.  This will connect RakPeer to all connecting peers, and all peers the connecting peer knows about.
/// \pre You must also install the ConnectionGraph plugin.  If you want a password, set it there.
/// \ingroup FULLY_CONNECTED_MESH_GROUP
class FullyConnectedMesh : public PluginInterface
{
public:
	FullyConnectedMesh();
	virtual ~FullyConnectedMesh();

	// --------------------------------------------------------------------------------------------
	// User functions
	// --------------------------------------------------------------------------------------------
	// Set the password to use to connect to the other systems
 	void Initialize(const char *password);

	// --------------------------------------------------------------------------------------------
	// Packet handling functions
	// --------------------------------------------------------------------------------------------
	virtual void OnDisconnect(RakPeerInterface *peer);
	virtual void Update(RakPeerInterface *peer);
	virtual PluginReceiveResult OnReceive(RakPeerInterface *peer, Packet *packet);
	

protected:
	char *pw;
};

#endif
