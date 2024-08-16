#include "RakNetTransport.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "PacketEnumerations.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if (defined(__GNUC__)  || defined(__GCCXML__))
#define _vsnprintf vsnprintf
#endif

#ifdef _MSC_VER
#pragma warning( push )
#endif

RakNetTransportCommandParser::RakNetTransportCommandParser()
{
	RegisterCommand(1, "SetPassword","Changes the console password to whatever.");
	RegisterCommand(0, "ClearPassword","Removes the console passwords.");
	RegisterCommand(0, "GetPassword","Gets the console password.");
}
RakNetTransportCommandParser::~RakNetTransportCommandParser()
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
bool RakNetTransportCommandParser::OnCommand(const char *command, unsigned numParameters, char **parameterList, TransportInterface *transport, PlayerID playerId, const char *originalString)
{
	RakNetTransport *rnt = (RakNetTransport*) transport;
	if (strcmp(command, "SetPassword")==0)
	{
		rnt->SetIncomingPassword(parameterList[0]);
		rnt->Send(playerId, "Password changed to %s\r\n", parameterList[0]);
	}
	else if (strcmp(command, "ClearPassword")==0)
	{
		rnt->SetIncomingPassword(0);
		rnt->Send(playerId, "Password cleared\r\n");
	}
	else if (strcmp(command, "GetPassword")==0)
	{
		char *password;
		password=rnt->GetIncomingPassword();
		if (password[0])
			rnt->Send(playerId, "Password is %s\r\n",password);
		else
			rnt->Send(playerId, "No password is set.\r\n");
	}
	return true;
}
char *RakNetTransportCommandParser::GetName(void) const
{
	return "RakNetTransport";
}
void RakNetTransportCommandParser::SendHelp(TransportInterface *transport, PlayerID playerId)
{
	transport->Send(playerId, "Provides a secure connection between your console\r\n");
	transport->Send(playerId, "and the console server.  Used to modify the console password.\r\n");
}
RakNetTransport::RakNetTransport()
{
	rakPeer=0;
}
RakNetTransport::~RakNetTransport()
{
	if (rakPeer)
		RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}
bool RakNetTransport::Start(unsigned short port, bool serverMode)
{
	AutoAllocate();
	rakPeer->InitializeSecurity(0,0,0,0);

	if (serverMode)
	{
		// Allow up to 8 remote systems to login
		rakPeer->SetMaximumIncomingConnections(8);
	}

	return rakPeer->Initialize(8, port, 250, 0);
}
void RakNetTransport::Stop(void)
{
	if (rakPeer==0) return;
	rakPeer->Disconnect(1000, 0);
	newConnections.Clear();
	lostConnections.Clear();
}
void RakNetTransport::Send( PlayerID playerId, const char *data, ... )
{
	if (rakPeer==0) return;
	if (data==0 || data[0]==0) return;

	char text[REMOTE_MAX_TEXT_INPUT];
	va_list ap;
	va_start(ap, data);
	_vsnprintf(text, REMOTE_MAX_TEXT_INPUT, data, ap);
	va_end(ap);
	text[REMOTE_MAX_TEXT_INPUT-1]=0;

	RakNet::BitStream str;
	str.Write((unsigned char) ID_TRANSPORT_STRING);
	str.Write(text, (int) strlen(text));
	str.Write((unsigned char) 0); // Null terminate the string
	rakPeer->Send(&str, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, playerId, (playerId==UNASSIGNED_PLAYER_ID)!=0);
}
void RakNetTransport::CloseConnection( PlayerID playerId )
{
	rakPeer->CloseConnection(playerId, true, 0);
}
Packet* RakNetTransport::Receive( void )
{
	if (rakPeer==0) return 0;
	Packet *p;
	p=rakPeer->Receive();
	if (p==0)
		return 0;
	if (p->data[0]==ID_TRANSPORT_STRING)
	{
		p->data++; // Go past ID_TRANSPORT_STRING, since the transport protocol is only supposed to send strings.
		return p;
	}
	if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
	{
		newConnections.Push(p->playerId);
	}
	else if (p->data[0]==ID_DISCONNECTION_NOTIFICATION || p->data[0]==ID_CONNECTION_LOST)
	{
		lostConnections.Push(p->playerId);
	}
	rakPeer->DeallocatePacket(p);

	return 0;
}
PlayerID RakNetTransport::HasNewConnection(void)
{
	if (newConnections.Size())
		return newConnections.Pop();
	return UNASSIGNED_PLAYER_ID;
}
PlayerID RakNetTransport::HasLostConnection(void)
{
	if (lostConnections.Size())
		return lostConnections.Pop();
	return UNASSIGNED_PLAYER_ID;
}
void RakNetTransport::SetIncomingPassword(const char *password)
{
	if (password)
		rakPeer->SetIncomingPassword(password, (int) strlen(password)+1);
	else
		rakPeer->SetIncomingPassword(0, 0);
}
char * RakNetTransport::GetIncomingPassword(void)
{
	static char password[256];
	int passwordLength=255;
	rakPeer->GetIncomingPassword((char*)password, &passwordLength);
	password[passwordLength]=0;
	return (char*) password;
}
void RakNetTransport::DeallocatePacket( Packet *packet )
{
	if (rakPeer==0) return;
	packet->data--; // Go back to ID_TRANSPORT_STRING, which we passed up in Receive()
	rakPeer->DeallocatePacket(packet);
}
void RakNetTransport::AutoAllocate(void)
{
	if (rakPeer==0)
		rakPeer=RakNetworkFactory::GetRakPeerInterface();
}
CommandParserInterface* RakNetTransport::GetCommandParser(void)
{
    return &rakNetTransportCommandParser;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
