/// \file
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

#include "PacketLogger.h"
#include "BitStream.h"
#include "DS_List.h"
#include "InternalPacket.h"
#include "RakPeerInterface.h"
#include "PacketEnumerations.h"
#include "StringCompressor.h"
#include "RPCMap.h"
#include "GetTime.h"
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning( push )
#endif

PacketLogger::PacketLogger()
{
	rakPeer=0;
	printId=true;
	printAcks=true;
}
PacketLogger::~PacketLogger()
{
}
void PacketLogger::OnAttach(RakPeerInterface *peer)
{
	rakPeer=peer;
}
void PacketLogger::OnDirectSocketSend(const char *data, const unsigned bitsUsed, PlayerID remoteSystemID)
{
	char str[256];
	PlayerID localPlayerId;
	localPlayerId = rakPeer->GetInternalID();

	if (printId==false)
	{
		sprintf(str, "Snd,Raw,  NIL,  NIL,%5i,%5i,%i,%u:%i,%u:%i\n", data[0], bitsUsed,RakNet::GetTime(),
			localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
	}
	else
	{
		sprintf(str, "Snd,Raw,NIL,NIL,%s,%i,%i,%u:%i,%u:%i\n",IDTOString(data[0]), bitsUsed,RakNet::GetTime(),
			localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
	}
	WriteLog(str);
}

void PacketLogger::LogHeader(void)
{
	WriteLog("S|R,Typ,Pckt#,Frm #,PktID,BitLn,Time     ,Local IP:Port   ,RemoteIP:Port\n");
}
void PacketLogger::OnDirectSocketReceive(const char *data, const unsigned bitsUsed, PlayerID remoteSystemID)
{
	char str[256];
	PlayerID localPlayerId;
	localPlayerId = rakPeer->GetInternalID();
	if (printId==false)
	{
		sprintf(str, "Rcv,Raw,  NIL,  NIL,%5i,%5i,%i,%u:%i,%u:%i\n", data[0], bitsUsed,RakNet::GetTime(),
			localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
	}
	else
	{
		sprintf(str, "Rcv,Raw,NIL,NIL,%s,%i,%i,%u:%i,%u:%i\n", IDTOString(data[0]), bitsUsed,RakNet::GetTime(),
			localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
	}

	WriteLog(str);
}
void PacketLogger::OnInternalPacket(InternalPacket *internalPacket, unsigned frameNumber, PlayerID remoteSystemID, RakNetTime time, bool isSend)
{
	char str[256];
	char sendType[4];
	PlayerID localPlayerId;
	localPlayerId = rakPeer->GetInternalID();

	if(isSend)
		//strcpy(sendType, "Snd");
		return;
	else
		strcpy(sendType, "Rcv");

	{
		if (internalPacket->data[0]==ID_TIMESTAMP && internalPacket->data[sizeof(unsigned char)+sizeof(RakNetTime)]!=ID_RPC)
		{
			if (printId==false)
			{
				sprintf(str, "%s,Tms,%5i,%5i,%5i,%5i,%i,%u:%i,%u:%i\n",sendType, internalPacket->messageNumber,frameNumber,
					internalPacket->data[1+sizeof(int)], internalPacket->dataBitLength,time,
					localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
			}
			else
			{
				sprintf(str, "%s,Tms,%i,%i,%s,%i,%i,%u:%i,%u:%i\n",sendType, internalPacket->messageNumber,frameNumber,
					IDTOString(internalPacket->data[1+sizeof(int)]), internalPacket->dataBitLength,time,
					localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
			}

		}
		else if (internalPacket->data[0]==ID_RPC || (internalPacket->dataBitLength>(sizeof(unsigned char)+sizeof(RakNetTime))*8 && internalPacket->data[0]==ID_TIMESTAMP && internalPacket->data[sizeof(unsigned char)+sizeof(RakNetTime)]==ID_RPC))
		{
			/*bool hasTimestamp;
			unsigned int bitsOfData;
			bool nameIsEncoded;
			unsigned char uniqueIdentifier[256];
			RPCIndex rpcIndex;
			RPCMap *rpcMap;
			RakNet::BitStream rpcDecode(internalPacket->data, BITS_TO_BYTES(internalPacket->dataBitLength), false);
			rpcDecode.IgnoreBits(8);
			if (internalPacket->data[0]==ID_TIMESTAMP)
				rpcDecode.IgnoreBits(sizeof(unsigned char)+sizeof(RakNetTime));
			rpcDecode.Read(nameIsEncoded);
			if (nameIsEncoded)
			{
				stringCompressor->DecodeString((char*)uniqueIdentifier, 256, &rpcDecode);
			}
			else
			{
				rpcDecode.ReadCompressed( rpcIndex );
				RPCNode *rpcNode;
				rpcMap = rakPeer->GetRPCMap(isSend==true ? remoteSystemID : UNASSIGNED_PLAYER_ID);
				if (rpcMap)
					rpcNode = rpcMap->GetNodeFromIndex(rpcIndex);
				else
					rpcNode=0;

				if (rpcMap && rpcNode)
					strcpy((char*)uniqueIdentifier, rpcNode->uniqueIdentifier);
				else
					strcpy((char*)uniqueIdentifier, "[UNKNOWN]");
			}
			rpcDecode.Read(hasTimestamp);
			rpcDecode.ReadCompressed(bitsOfData);
			
			if (hasTimestamp)
				sprintf(str, "%s,RpT,%5i,%5i,%s,%5i,%i,%u:%i,%u:%i\n",sendType, internalPacket->messageNumber,frameNumber,
				uniqueIdentifier, internalPacket->dataBitLength,time,
				localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
			else
				sprintf(str, "%s,Rpc,%5i,%5i,%s,%5i,%i,%u:%i,%u:%i\n",sendType, internalPacket->messageNumber,frameNumber,
				uniqueIdentifier, internalPacket->dataBitLength,time,
				localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
			*/
		}
		else
		{
			if (printId==false)
			{
				sprintf(str, "%s,Nrm,%5i,%5i,%5i,%5i,%i,%u:%i,%u:%i\n",sendType, internalPacket->messageNumber,frameNumber,
					internalPacket->data[0], internalPacket->dataBitLength,time,
					localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
			}
			else
			{
				sprintf(str, "%s,Nrm,%i,%i,%s,%i,%i,%u:%i,%u:%i\n",sendType, internalPacket->messageNumber,frameNumber,
					IDTOString(internalPacket->data[0]), internalPacket->dataBitLength,time,
					localPlayerId.binaryAddress, localPlayerId.port, remoteSystemID.binaryAddress, remoteSystemID.port);
			}
		}
	}

	WriteLog(str);
}
void PacketLogger::WriteLog(const char *str)
{
	//printf("%s", str);
}
void PacketLogger::SetPrintID(bool print)
{
	printId=print;
}
void PacketLogger::SetPrintAcks(bool print)
{
	printAcks=print;
}
char* PacketLogger::BaseIDTOString(unsigned char Id)
{
	return 0;
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
char* PacketLogger::UserIDTOString(unsigned char Id)
{
	switch ( Id )
	{
	case ID_PLAYER_SYNC:
		return "ID_PLAYER_SYNC";
	case ID_VEHICLE_SYNC:
		return "ID_VEHICLE_SYNC";
	case ID_PASSENGER_SYNC:
		return "ID_PASSENGER_SYNC";
	case ID_AIM_SYNC:
		return "ID_AIM_SYNC";
	case ID_RCON_COMMAND:
		return "ID_RCON_COMMAND";
	case ID_RCON_RESPONCE:
		return "ID_RCON_RESPONCE";
	case ID_STATS_UPDATE:
		return "ID_STATS_UPDATE";
	case ID_WEAPONS_UPDATE:
		return "ID_WEAPONS_UPDATE";
	case ID_SPECTATOR_SYNC:
		return "ID_SPECTATOR_SYNC";
	case ID_TRAILER_SYNC:
		return "ID_TRAILER_SYNC";
	case ID_UNOCCUPIED_SYNC:
		return "ID_UNOCCUPIED_SYNC";
	}
	return "Unknown";
}
char* PacketLogger::IDTOString(unsigned char Id)
{
	char *out;
	out=BaseIDTOString(Id);
	if (out)
		return (char*)out;
	return UserIDTOString(Id);
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
