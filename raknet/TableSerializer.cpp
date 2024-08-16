#include "TableSerializer.h"
#include "DS_Table.h"
#include "BitStream.h"
#include "StringCompressor.h"

void TableSerializer::SerializeTable(DataStructures::Table *in, RakNet::BitStream *out)
{
	DataStructures::List<DataStructures::Table::ColumnDescriptor> &columns=in->GetColumns();
	DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur = in->GetRows().GetListHead();
	out->Write((unsigned)columns.Size());
	unsigned i;
	for (i=0; i<columns.Size(); i++)
	{
		stringCompressor->EncodeString(columns[i].columnName, _TABLE_MAX_COLUMN_NAME_LENGTH, out);
		out->Write((unsigned char)columns[i].columnType);
	}
	out->Write((unsigned)in->GetRows().Size());
	unsigned rowIndex;
	while (cur)
	{
		for (rowIndex=0; rowIndex < (unsigned)cur->size; rowIndex++)
		{
			SerializeRow(cur->data[rowIndex], cur->keys[rowIndex], columns, out);
		}
		cur=cur->next;
	}
}
bool TableSerializer::DeserializeTable(unsigned char *serializedTable, unsigned int tableLength, DataStructures::Table *out)
{
	RakNet::BitStream in((unsigned char*) serializedTable, tableLength, false);

	unsigned columnSize;
	unsigned char columnType;
	unsigned rowSize;
	char columnName[_TABLE_MAX_COLUMN_NAME_LENGTH];
	if (in.Read(columnSize)==false || columnSize > 10000)
		return false; // Hacker crash prevention

	out->Clear();
	unsigned i;
	for (i=0; i<columnSize; i++)
	{
		stringCompressor->DecodeString(columnName, 32, &in);
		in.Read(columnType);
		out->AddColumn(columnName, (DataStructures::Table::ColumnType)columnType);
	}

	if (in.Read(rowSize)==false || rowSize>100000)
		return false; // Hacker crash prevention

	unsigned rowIndex;
	for (rowIndex=0; rowIndex < rowSize; rowIndex++)
	{
		if (DeserializeRow(&in, out)==false)
			return false;
	}
	return true;
}
void TableSerializer::SerializeRow(DataStructures::Table::Row *in, unsigned keyIn, DataStructures::List<DataStructures::Table::ColumnDescriptor> &columns, RakNet::BitStream *out)
{
	unsigned cellIndex;
	out->Write(keyIn);
	for (cellIndex=0; cellIndex<columns.Size(); cellIndex++)
	{
		SerializeCell(out, in->cells[cellIndex], columns[cellIndex].columnType);
	}
}
bool TableSerializer::DeserializeRow(RakNet::BitStream *in, DataStructures::Table *out)
{
	DataStructures::List<DataStructures::Table::ColumnDescriptor> &columns=out->GetColumns();
	unsigned cellIndex;
	DataStructures::Table::Row *row;
	unsigned key;
	if (in->Read(key)==false)
		return false;
	row=out->AddRow(key);
	for (cellIndex=0; cellIndex<columns.Size(); cellIndex++)
	{
		if (DeserializeCell(in, row->cells[cellIndex], columns[cellIndex].columnType)==false)
		{
			out->RemoveRow(key);
			return false;
		}
	}
	return true;
}
void TableSerializer::SerializeCell(RakNet::BitStream *out, DataStructures::Table::Cell *cell, DataStructures::Table::ColumnType columnType)
{
	out->Write(cell->isEmpty);
	if (cell->isEmpty==false)
	{
		if (columnType==DataStructures::Table::NUMERIC)
		{
			out->Write(cell->i);
		}
		else if (columnType==DataStructures::Table::STRING)
		{
			stringCompressor->EncodeString(cell->c, 65535, out);
		}
		else
		{
			// Binary
			assert(columnType==DataStructures::Table::BINARY);
			out->Write((unsigned)cell->i);
			out->WriteAlignedBytes((const unsigned char *) cell->c, cell->i);
		}
	}
}
bool TableSerializer::DeserializeCell(RakNet::BitStream *in, DataStructures::Table::Cell *cell, DataStructures::Table::ColumnType columnType)
{
	bool isEmpty;
	int value;
	char tempString[65535];
	cell->Clear();

	if (in->Read(isEmpty)==false)
		return false;
	if (isEmpty==false)
	{
		if (columnType==DataStructures::Table::NUMERIC)
		{
			if (in->Read(value)==false)
				return false;
			cell->Set(value);
		}
		else if (columnType==DataStructures::Table::STRING)
		{
			if (stringCompressor->DecodeString(tempString, 65535, in)==false)
				return false;
			cell->Set(tempString);
		}
		else
		{
			// Binary
			assert(columnType==DataStructures::Table::BINARY);
			if (in->Read(value)==false || value > 10000000)
				return false; // Sanity check to max binary cell of 10 megabytes
			in->AlignReadToByteBoundary();
			if (BITS_TO_BYTES(in->GetNumberOfUnreadBits())<value)
				return false;
			cell->Set((char*) in->GetData()+BITS_TO_BYTES(in->GetReadOffset()), value);
			in->IgnoreBits(BYTES_TO_BITS(value));
		}
	}
	return true;
}
