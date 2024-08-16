#include "LightweightDatabaseClient.h"
#include "StringCompressor.h"
#include "PacketEnumerations.h"
#include "RakPeerInterface.h"
#include "TableSerializer.h"
#include "BitStream.h"

#ifdef _MSC_VER
#pragma warning( push )
#endif

LightweightDatabaseClient::LightweightDatabaseClient()
{
	rakPeer=0;
}
LightweightDatabaseClient::~LightweightDatabaseClient()
{

}
void LightweightDatabaseClient::QueryTable(const char *tableName, const char *queryPassword, const char **columnSubset, unsigned char numColumnSubset, DatabaseFilter *filter, unsigned char numFilters, unsigned *rowIds, unsigned char numRowIDs, PlayerID playerId, bool broadcast)
{
	if (tableName==0 || tableName[0]==0)
		return;
	if (rakPeer==0)
		return;

	RakNet::BitStream out;
	out.Write((unsigned char) ID_DATABASE_QUERY_REQUEST);
	stringCompressor->EncodeString(tableName, _SIMPLE_DATABASE_TABLE_NAME_LENGTH, &out);
	if (queryPassword && queryPassword[0])
	{
        out.Write(true);
		// This is sent in plain text.  I can do this securely but it's not worth the trouble.
		// Use secure connections if you want security.
		stringCompressor->EncodeString(queryPassword, _SIMPLE_DATABASE_PASSWORD_LENGTH, &out);
	}
	else
		out.Write(false);

	out.Write(numColumnSubset);
	unsigned i;
	for (i=0; i < numColumnSubset; i++)
		out.Write(columnSubset[i]);

	out.Write(numFilters);
	for (i=0; i < numFilters; i++)
		filter[i].Serialize(&out);

	out.Write(numRowIDs);
	for (i=0; i < numRowIDs; i++)
		out.Write(rowIds[i]);

	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED,0,playerId, broadcast);
}
void LightweightDatabaseClient::RemoveRow(const char *tableName, const char *removePassword, unsigned rowId, PlayerID playerId, bool broadcast)
{
	if (tableName==0 || tableName[0]==0)
		return;
	if (rakPeer==0)
		return;

	RakNet::BitStream out;
	out.Write((unsigned char) ID_DATABASE_REMOVE_ROW);
	stringCompressor->EncodeString(tableName, _SIMPLE_DATABASE_TABLE_NAME_LENGTH, &out);
	if (removePassword && removePassword[0])
	{
		out.Write(true);
		// This is sent in plain text.  I can do this securely but it's not worth the trouble.
		// Use secure connections if you want security.
		stringCompressor->EncodeString(removePassword, _SIMPLE_DATABASE_PASSWORD_LENGTH, &out);
	}
	else
		out.Write(false);

	out.Write(rowId);

	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED,0,playerId, broadcast);
}
void LightweightDatabaseClient::UpdateRow(const char *tableName, const char *updatePassword, RowUpdateMode updateMode, bool hasRowId, unsigned rowId, DatabaseCellUpdate *cellUpdates, unsigned char numCellUpdates, PlayerID playerId, bool broadcast)
{
	if (tableName==0 || tableName[0]==0)
		return;
	if (rakPeer==0)
		return;
	if (cellUpdates==0 || numCellUpdates==0)
		return;

	RakNet::BitStream out;
	out.Write((unsigned char) ID_DATABASE_UPDATE_ROW);
	stringCompressor->EncodeString(tableName, _SIMPLE_DATABASE_TABLE_NAME_LENGTH, &out);
	if (updatePassword && updatePassword[0])
	{
		out.Write(true);
		// This is sent in plain text.  I can do this securely but it's not worth the trouble.
		// Use secure connections if you want security.
		stringCompressor->EncodeString(updatePassword, _SIMPLE_DATABASE_PASSWORD_LENGTH, &out);
	}
	else
		out.Write(false);

	out.Write((unsigned char) updateMode);
	out.Write(hasRowId);
	if (hasRowId)
		out.Write(rowId);
	out.Write(numCellUpdates);
	unsigned i;
	for (i=0; i < numCellUpdates; i++)
		cellUpdates[i].Serialize(&out);

	rakPeer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED,0,playerId, broadcast);
}
PluginReceiveResult LightweightDatabaseClient::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	// All these messages are just returned to the user.
//	switch (packet->data[0]) 
//	{
//	case ID_DATABASE_QUERY_REPLY:
//	case ID_DATABASE_UNKNOWN_TABLE:
//	case ID_DATABASE_INCORRECT_PASSWORD:
//	}

	return RR_CONTINUE_PROCESSING;
}
void LightweightDatabaseClient::OnAttach(RakPeerInterface *peer)
{
	rakPeer=peer;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
