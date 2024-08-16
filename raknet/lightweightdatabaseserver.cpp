#include "LightweightDatabaseServer.h"
#include "PacketEnumerations.h"
#include "BitStream.h"
#include "StringCompressor.h"
#include "RakPeerInterface.h"
#include "TableSerializer.h"
#include "RakAssert.h"
#include "GetTime.h"
#include "Rand.h"

#define SYSTEM_ID_COLUMN_NAME "__systemId"
#define LAST_PING_RESPONSE_COLUMN_NAME "__lastPingResponseTime"
#define NEXT_PING_SEND_COLUMN_NAME "__nextPingSendTime"
static const int SEND_PING_INTERVAL=15000;
static const int DROP_SERVER_INTERVAL=75000;

#ifdef _MSC_VER
#pragma warning( push )
#endif

int LightweightDatabaseServer::DatabaseTableComp( char* const &key1, char* const &key2 )
{
	return strcmp(key1, key2);
}

LightweightDatabaseServer::LightweightDatabaseServer()
{

}
LightweightDatabaseServer::~LightweightDatabaseServer()
{
	Clear();
}
DataStructures::Table *LightweightDatabaseServer::GetTable(char *tableName)
{
	if (database.Has(tableName))
		return &(database.Get(tableName)->table);
	return 0;
}
DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *LightweightDatabaseServer::GetTableRows(char *tableName)
{
	if (database.Has(tableName))
		database.Get(tableName)->table.GetRows().GetListHead();
	return 0;
}
DataStructures::Table* LightweightDatabaseServer::AddTable(char *tableName,
														   bool allowRemoteQuery,
														   bool allowRemoteUpdate,
														   bool allowRemoteRemove,
														   char queryPassword[_SIMPLE_DATABASE_PASSWORD_LENGTH],
														   char updatePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH],		
														   char removePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH],
														   bool oneRowPerSystemId,
														   bool onlyUpdateOwnRows,
														   bool removeRowOnPingFailure,
														   bool removeRowOnDisconnect,
														   bool autogenerateRowIDs)
{
	if (tableName==0 || tableName[0]==0)
		return 0;
	if (database.Has(tableName))
		return 0;
	DatabaseTable *databaseTable = new DatabaseTable;

	strncpy(databaseTable->tableName, tableName, _SIMPLE_DATABASE_TABLE_NAME_LENGTH);
	databaseTable->tableName[_SIMPLE_DATABASE_TABLE_NAME_LENGTH]=0;

	if (allowRemoteUpdate)
	{
		strncpy(databaseTable->updatePassword, updatePassword, _SIMPLE_DATABASE_PASSWORD_LENGTH);
		databaseTable->updatePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH]=0;
	}
	else
		databaseTable->updatePassword[0]=0;

	if (allowRemoteQuery)
	{
		strncpy(databaseTable->queryPassword, queryPassword, _SIMPLE_DATABASE_PASSWORD_LENGTH);
		databaseTable->queryPassword[_SIMPLE_DATABASE_PASSWORD_LENGTH]=0;
	}
	else
		databaseTable->queryPassword[0]=0;	

	if (allowRemoteRemove)
	{
		strncpy(databaseTable->removePassword, removePassword, _SIMPLE_DATABASE_PASSWORD_LENGTH);
		databaseTable->removePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH]=0;
	}
	else
		databaseTable->removePassword[0]=0;

	if (allowRemoteUpdate)
	{
		databaseTable->allowRemoteUpdate=true;
		databaseTable->oneRowPerSystemId=oneRowPerSystemId;
		databaseTable->onlyUpdateOwnRows=onlyUpdateOwnRows;
		databaseTable->removeRowOnPingFailure=removeRowOnPingFailure;
		databaseTable->removeRowOnDisconnect=removeRowOnDisconnect;
	}
	else
	{
		// All these parameters are related to IP tracking, which is not done if remote updates are not allowed
		databaseTable->allowRemoteUpdate=true;
		databaseTable->oneRowPerSystemId=false;
		databaseTable->onlyUpdateOwnRows=false;
		databaseTable->removeRowOnPingFailure=false;
		databaseTable->removeRowOnDisconnect=false;
	}
	
	databaseTable->nextRowId=0;
	databaseTable->nextRowPingCheck=0;
	
	databaseTable->autogenerateRowIDs=autogenerateRowIDs;
	databaseTable->allowRemoteRemove=allowRemoteRemove;
	databaseTable->allowRemoteQuery=allowRemoteQuery;
	
	database.SetNew(databaseTable->tableName, databaseTable);

	if ( oneRowPerSystemId || onlyUpdateOwnRows || removeRowOnPingFailure || removeRowOnDisconnect)
		databaseTable->systemIdColumnIndex=databaseTable->table.AddColumn(SYSTEM_ID_COLUMN_NAME, DataStructures::Table::BINARY);
	else
		databaseTable->systemIdColumnIndex=(unsigned) -1;
	if (databaseTable->removeRowOnPingFailure)
	{
		databaseTable->lastPingResponseColumnIndex=databaseTable->table.AddColumn(LAST_PING_RESPONSE_COLUMN_NAME, DataStructures::Table::NUMERIC);
		databaseTable->nextPingSendColumnIndex=databaseTable->table.AddColumn(NEXT_PING_SEND_COLUMN_NAME, DataStructures::Table::NUMERIC);
	}
	else
	{
		databaseTable->lastPingResponseColumnIndex=(unsigned) -1;
		databaseTable->nextPingSendColumnIndex=(unsigned) -1;
	}

	return &(databaseTable->table);
}
bool LightweightDatabaseServer::RemoveTable(char *tableName)
{
	LightweightDatabaseServer::DatabaseTable *databaseTable;
	databaseTable = database.Get(tableName);
	if (databaseTable==0)
		return false;
	// Be sure to call Delete on database before I do the actual pointer deletion since the key won't be valid after that time.
	database.Delete(tableName);
	databaseTable->table.Clear();
	delete databaseTable;
	return true;
}
void LightweightDatabaseServer::Clear(void)
{
	unsigned i;

	for (i=0; i < database.Size(); i++)
	{
		database[i]->table.Clear();
		delete database[i];
	}

	database.Clear();
}
unsigned LightweightDatabaseServer::GetAndIncrementRowID(char *tableName)
{
	LightweightDatabaseServer::DatabaseTable *databaseTable;
	databaseTable = database.Get(tableName);
	RakAssert(databaseTable);
	RakAssert(databaseTable->autogenerateRowIDs==true);
	return ++(databaseTable->nextRowId) - 1;
}
void LightweightDatabaseServer::OnAttach(RakPeerInterface *peer)
{

}
void LightweightDatabaseServer::Update(RakPeerInterface *peer)
{
	RakNetTime time=0;
	DatabaseTable *databaseTable;
	DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur;
	unsigned i,j;
	DataStructures::Table::Row* row;
	DataStructures::List<unsigned> removeList;
	PlayerID playerId;

	// periodic ping if removing system that do not respond to pings.
	for (i=0; i < database.Size(); i++)
	{
		databaseTable=database[i];

		if (databaseTable->removeRowOnPingFailure)
		{
			// Reading the time is slow - only do it once if necessary.
			if (time==0)
				 time = RakNet::GetTime();

			if (databaseTable->nextRowPingCheck < time)
			{
				databaseTable->nextRowPingCheck=time+1000+(randomMT()%1000);
				DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> &rows = databaseTable->table.GetRows();
				cur = rows.GetListHead();
				while (cur)
				{
					// Mark dropped entities
					for (j=0; j < (unsigned)cur->size; j++)
					{
						row = cur->data[j];
						if (time - row->cells[databaseTable->lastPingResponseColumnIndex]->i > DROP_SERVER_INTERVAL)
							removeList.Insert(cur->keys[j]);
					}
					cur=cur->next;
				}

				// Remove dropped entities
				for (j=0; j < removeList.Size(); j++)
					databaseTable->table.RemoveRow(removeList[i]);
				removeList.Clear(true);

				cur = rows.GetListHead();
				// Ping remaining entities if they are not connected.  If they are connected just increase the ping interval.
				while (cur)
				{
					for (j=0; j < (unsigned)cur->size; j++)
					{
						row = cur->data[j];
						if (row->cells[databaseTable->nextPingSendColumnIndex]->i < (int) time)
						{
							row->cells[databaseTable->systemIdColumnIndex]->Get((char*)&playerId, 0);
							if (peer->GetIndexFromPlayerID(playerId)==-1)
							{
								peer->Ping(playerId.ToString(false), playerId.port, false);
							}
							else
							{
								// Consider the fact that they are connected to be a ping response
								row->cells[databaseTable->lastPingResponseColumnIndex]->i=time;
							}
							
							row->cells[databaseTable->nextPingSendColumnIndex]->i=time+SEND_PING_INTERVAL+(randomMT()%1000);
						}
					}
					cur=cur->next;
				}
			}
		}
	}
}
PluginReceiveResult LightweightDatabaseServer::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	switch (packet->data[0]) 
	{
	case ID_DATABASE_QUERY_REQUEST:
		OnQueryRequest(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_DATABASE_UPDATE_ROW:
		OnUpdateRow(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_DATABASE_REMOVE_ROW:
		OnRemoveRow(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_DISCONNECTION_NOTIFICATION:
	case ID_CONNECTION_LOST:
		RemoveRowsFromIP(packet->playerId);
		return RR_CONTINUE_PROCESSING;
	case ID_PONG:
		OnPong(peer, packet);
		return RR_CONTINUE_PROCESSING;
	}
	return RR_CONTINUE_PROCESSING;
}
void LightweightDatabaseServer::OnDisconnect(RakPeerInterface *peer)
{

}
void LightweightDatabaseServer::OnCloseConnection(RakPeerInterface *peer, PlayerID playerId)
{
	RemoveRowsFromIP(playerId);
}
void LightweightDatabaseServer::OnQueryRequest(RakPeerInterface *peer, Packet *packet)
{
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	LightweightDatabaseServer::DatabaseTable *databaseTable = DeserializeClientHeader(&inBitstream, peer, packet, 0);
	if (databaseTable==0)
		return;
	if (databaseTable->allowRemoteQuery==false)
		return;
	unsigned char numColumnSubset;
	RakNet::BitStream outBitstream;
	unsigned i;
	if (inBitstream.Read(numColumnSubset)==false)
		return;
	unsigned columnSubset[256];
	for (i=0; i < numColumnSubset; i++)
		inBitstream.Read(columnSubset[i]);
	unsigned char numNetworkedFilters;
	if (inBitstream.Read(numNetworkedFilters)==false)
		return;
	DatabaseFilter networkedFilters[256];
	for (i=0; i < numNetworkedFilters; i++)
	{
		if (networkedFilters[i].Deserialize(&inBitstream)==false)
			return;
	}

	unsigned rowIds[256];
	unsigned char numRowIDs;
	if (inBitstream.Read(numRowIDs)==false)
		return;
	for (i=0; i < numRowIDs; i++)
		inBitstream.Read(rowIds[i]);

	// Convert the safer and more robust networked database filter to the more efficient form the table actually uses.
	DataStructures::Table::FilterQuery tableFilters[256];
	unsigned numTableFilters=0;
	for (i=0; i < numNetworkedFilters; i++)
	{	
		tableFilters[numTableFilters].columnIndex=databaseTable->table.ColumnIndex(networkedFilters[i].columnName);
		if (tableFilters[numTableFilters].columnIndex==(unsigned)-1)
			continue;
		if (networkedFilters[i].columnType!=databaseTable->table.GetColumns()[tableFilters[numTableFilters].columnIndex].columnType)
			continue;
		tableFilters[numTableFilters].operation=networkedFilters[i].operation;
		// It's important that I store a pointer to the class here or the destructor of the class will deallocate the cell twice
		tableFilters[numTableFilters++].cellValue=&(networkedFilters[i].cellValue);
	}
    
	DataStructures::Table queryResult;
	databaseTable->table.QueryTable(columnSubset, numColumnSubset, tableFilters, numTableFilters, rowIds, numRowIDs, &queryResult);
	outBitstream.Write((unsigned char)ID_DATABASE_QUERY_REPLY);
	TableSerializer::SerializeTable(&queryResult, &outBitstream);
	peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->playerId, false);			

}
void LightweightDatabaseServer::OnUpdateRow(RakPeerInterface *peer, Packet *packet)
{
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	LightweightDatabaseServer::DatabaseTable *databaseTable = DeserializeClientHeader(&inBitstream, peer, packet, 1);
	if (databaseTable==0)
		return;
	if (databaseTable->allowRemoteUpdate==false)
		return;
	unsigned char updateMode;
	bool hasRowId;
	unsigned rowId;
	unsigned i;
	DataStructures::Table::Row *row;
	inBitstream.Read(updateMode);
	inBitstream.Read(hasRowId);
	if (hasRowId)
		inBitstream.Read(rowId);
	else
		rowId=(unsigned) -1; // Not used here but remove the debugging check
	unsigned char numCellUpdates;
	if (inBitstream.Read(numCellUpdates)==false)
		return;
	// Read the updates for the row
	DatabaseCellUpdate cellUpdates[256];
	for (i=0; i < numCellUpdates; i++)
	{
		if (cellUpdates[i].Deserialize(&inBitstream)==false)
			return;
	}

	if ((RowUpdateMode)updateMode==RUM_UPDATE_EXISTING_ROW)
	{
		if (hasRowId==false)
			return;

		row = databaseTable->table.GetRowByID(rowId);
		if (row==0 || databaseTable->onlyUpdateOwnRows && RowHasIP(row, packet->playerId, databaseTable->systemIdColumnIndex)==false)
			return; // You can't update some other system's row
	}
	else if ((RowUpdateMode)updateMode==RUM_UPDATE_OR_ADD_ROW)
	{
		if (hasRowId)
			row = databaseTable->table.GetRowByID(rowId);
		else
			row=0;

		if (row==0)
		{
			row=AddRow(databaseTable, packet->playerId, hasRowId, rowId);
			if (row==0)
				return;
		}
		else
		{
			// Existing row
			if (databaseTable->onlyUpdateOwnRows && RowHasIP(row, packet->playerId, databaseTable->systemIdColumnIndex)==false)
				return; // You can't update some other system's row
		}	
	}
	else
	{
		RakAssert((RowUpdateMode)updateMode==RUM_ADD_NEW_ROW);

		row=AddRow(databaseTable, packet->playerId, hasRowId, rowId);
		if (row==0)
			return;
	}

	unsigned columnIndex;
	for (i=0; i < numCellUpdates; i++)
	{
		columnIndex=databaseTable->table.ColumnIndex(cellUpdates[i].columnName);
		if (columnIndex!=(unsigned)-1 && columnIndex!=databaseTable->lastPingResponseColumnIndex && columnIndex!=databaseTable->nextPingSendColumnIndex && columnIndex!=databaseTable->systemIdColumnIndex)
		{
			if (cellUpdates[i].cellValue.isEmpty)
				row->cells[columnIndex]->Clear();
			else if (cellUpdates[i].columnType==databaseTable->table.GetColumnType(columnIndex))
			{
				if (cellUpdates[i].columnType==DataStructures::Table::NUMERIC)
				{
					row->UpdateCell(columnIndex, cellUpdates[i].cellValue.i);
				}
				else if (cellUpdates[i].columnType==DataStructures::Table::BINARY)
				{
					row->UpdateCell(columnIndex, cellUpdates[i].cellValue.i, cellUpdates[i].cellValue.c);
				}
				else
				{
					RakAssert(cellUpdates[i].columnType==DataStructures::Table::STRING);
					row->UpdateCell(columnIndex, cellUpdates[i].cellValue.c);
				}
			}
		}
	}
}
void LightweightDatabaseServer::OnRemoveRow(RakPeerInterface *peer, Packet *packet)
{
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	LightweightDatabaseServer::DatabaseTable *databaseTable = DeserializeClientHeader(&inBitstream, peer, packet, 0);
	if (databaseTable==0)
		return;
	if (databaseTable->allowRemoteRemove==false)
		return;
	unsigned rowId;
	inBitstream.Read(rowId);
	databaseTable->table.RemoveRow(rowId);
}

void LightweightDatabaseServer::OnPong(RakPeerInterface *peer, Packet *packet)
{
	unsigned databaseIndex;
	DatabaseTable *databaseTable;
	unsigned curIndex;
	PlayerID playerId;
	RakNetTime time=0;
	for (databaseIndex=0; databaseIndex < database.Size(); databaseIndex++)
	{
		databaseTable=database[databaseIndex];
		if (databaseTable->removeRowOnPingFailure)
		{
			if (time==0)
				time=RakNet::GetTime();

			DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> &rows = databaseTable->table.GetRows();
			DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur = rows.GetListHead();
			
			while (cur)
			{
				for (curIndex=0; curIndex < (unsigned) cur->size; curIndex++)
				{
					cur->data[curIndex]->cells[databaseTable->systemIdColumnIndex]->Get((char*)&playerId,0);
					if (playerId==packet->playerId)
					{
						cur->data[curIndex]->cells[databaseTable->lastPingResponseColumnIndex]->i=time;
					}
				}
				cur=cur->next;
			}
		}
	}
}

LightweightDatabaseServer::DatabaseTable * LightweightDatabaseServer::DeserializeClientHeader(RakNet::BitStream *inBitstream, RakPeerInterface *peer, Packet *packet, int mode)
{
	RakNet::BitStream outBitstream;
	bool hasPassword;
	char password[_SIMPLE_DATABASE_PASSWORD_LENGTH];
	inBitstream->IgnoreBits(8);
	char tableName[_SIMPLE_DATABASE_TABLE_NAME_LENGTH];
	stringCompressor->DecodeString(tableName, _SIMPLE_DATABASE_TABLE_NAME_LENGTH, inBitstream);
	DatabaseTable *databaseTable = database.Get(tableName);
	if (databaseTable==0)
	{
		outBitstream.Write((unsigned char)ID_DATABASE_UNKNOWN_TABLE);
		peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->playerId, false);
		return 0;
	}
	const char *dbPass;
	if (mode==0)
		dbPass=databaseTable->queryPassword;
	else if (mode==1)
		dbPass=databaseTable->updatePassword;
	else
		dbPass=databaseTable->removePassword;

	inBitstream->Read(hasPassword);
	if (hasPassword)
	{
		if (stringCompressor->DecodeString(password, _SIMPLE_DATABASE_PASSWORD_LENGTH, inBitstream)==false)
			return 0;
		if (databaseTable->queryPassword[0] && strcmp(password, dbPass)!=0)
		{
			outBitstream.Write((unsigned char)ID_DATABASE_INCORRECT_PASSWORD);
			peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->playerId, false);
			// Short ban to prevent brute force password attempts
			peer->AddToBanList(packet->playerId.ToString(false), 1000);
			// Don't send a disconnection notification so it closes the connection right away.
			peer->CloseConnection(packet->playerId, false, 0);			
			return 0;
		}
	}
	else if (dbPass[0])
	{
		outBitstream.Write((unsigned char)ID_DATABASE_INCORRECT_PASSWORD);
		peer->Send(&outBitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->playerId, false);
		return 0;
	}

	return databaseTable;
}

DataStructures::Table::Row * LightweightDatabaseServer::GetRowFromIP(DatabaseTable *databaseTable, PlayerID playerId, unsigned *rowKey)
{
	DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> &rows = databaseTable->table.GetRows();
	DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur = rows.GetListHead();
	DataStructures::Table::Row* row;
	unsigned i;
	while (cur)
	{
		for (i=0; i < (unsigned)cur->size; i++)
		{
			row = cur->data[i];
			if (RowHasIP(row, playerId, databaseTable->systemIdColumnIndex ))
			{
				if (rowKey)
					*rowKey=cur->keys[i];
				return row;
			}
		}
		cur=cur->next;
	}
	return 0;
}
bool LightweightDatabaseServer::RowHasIP(DataStructures::Table::Row *row, PlayerID playerId, unsigned systemIdColumnIndex)
{
	RakAssert(row->cells[systemIdColumnIndex]->isEmpty==false);
	if (memcmp(row->cells[systemIdColumnIndex]->c, &playerId, sizeof(PlayerID))==0)
		return true;
	return false;
}
DataStructures::Table::Row * LightweightDatabaseServer::AddRow(LightweightDatabaseServer::DatabaseTable *databaseTable, PlayerID playerId, bool hasRowId, unsigned rowId)
{
	DataStructures::Table::Row *row;
	if (databaseTable->oneRowPerSystemId && GetRowFromIP(databaseTable, playerId, 0))
		return 0; // This system already has a row.

	if (databaseTable->autogenerateRowIDs==false)
	{
		// For a new row:
		// rowID required but not specified OR
		// rowId specified but already in the table
		// Then exit
		if (hasRowId==false || databaseTable->table.GetRowByID(rowId))
			return 0; 
	}
	else
		rowId=databaseTable->nextRowId++;

	// Add new row
	row = databaseTable->table.AddRow(rowId);

	// Set IP and last update time
	if ( databaseTable->oneRowPerSystemId || databaseTable->onlyUpdateOwnRows || databaseTable->removeRowOnPingFailure || databaseTable->removeRowOnDisconnect)
		row->cells[databaseTable->systemIdColumnIndex]->Set((char*)&playerId, sizeof(PlayerID));
	if (databaseTable->removeRowOnPingFailure)
	{
		RakNetTime time = RakNet::GetTime();
		row->cells[databaseTable->lastPingResponseColumnIndex]->Set(time);
		row->cells[databaseTable->nextPingSendColumnIndex]->Set(time+SEND_PING_INTERVAL);
	}

	return row;
}
void LightweightDatabaseServer::RemoveRowsFromIP(PlayerID playerId)
{
	// Remove rows for tables that do so on a system disconnect
	DatabaseTable *databaseTable;
	DataStructures::List<unsigned> removeList;
	DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur;
	unsigned i,j;
	for (i=0; i < database.Size(); i++)
	{
		databaseTable=database[i];
		if (databaseTable->removeRowOnDisconnect)
		{
			DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> &rows = databaseTable->table.GetRows();
			cur = rows.GetListHead();
			while (cur)
			{
				// Mark dropped entities
				for (j=0; j < (unsigned)cur->size; j++)
				{
					if (RowHasIP(cur->data[j], playerId, databaseTable->systemIdColumnIndex))
						removeList.Insert(cur->keys[j]);
				}
				cur=cur->next;
			}

			for (j=0; j < removeList.Size(); j++)
				databaseTable->table.RemoveRow(removeList[i]);
			removeList.Clear(true);
		}
	}	
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
