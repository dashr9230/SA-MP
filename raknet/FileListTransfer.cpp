#include "FileListTransfer.h"
#include "DS_HuffmanEncodingTree.h"
#include "FileListTransferCBInterface.h"
#include "StringCompressor.h"
#include "FileList.h"
#include "DS_Queue.h"
#include "PacketEnumerations.h"
#include "NetworkTypes.h"
#include "RakPeerInterface.h"
#include <assert.h>

#ifdef _MSC_VER
#pragma warning( push )
#endif

struct FileListTransfer::FileListReceiver
{
	FileListTransferCBInterface *downloadHandler;
	PlayerID allowedSender;
    HuffmanEncodingTree tree;
	unsigned short setID;
	unsigned setCount;
	unsigned setTotalCompressedTransmissionLength;
	unsigned setTotalFinalLength;
	bool gotSetHeader;
	bool deleteDownloadHandler;
	bool isCompressed;
};

FileListTransfer::FileListTransfer()
{
	rakPeer=0;
	setId=0;
	DataStructures::Map<unsigned short, FileListReceiver*>::IMPLEMENT_DEFAULT_COMPARISON();
}
FileListTransfer::~FileListTransfer()
{
	Clear();
}
unsigned short FileListTransfer::SetupReceive(FileListTransferCBInterface *handler, bool deleteHandler, PlayerID allowedSender)
{
	if (rakPeer->GetIndexFromPlayerID(allowedSender)==-1)
		return (unsigned short)-1;
	FileListReceiver *receiver;

	if (fileListReceivers.Has(setId))
	{
		receiver=fileListReceivers.Get(setId);
		if (receiver->deleteDownloadHandler)
			delete receiver->downloadHandler;
		delete receiver;
		fileListReceivers.Delete(setId);
	}

	unsigned short oldId;
	receiver = new FileListReceiver;
	receiver->downloadHandler=handler;
	receiver->allowedSender=allowedSender;
	receiver->gotSetHeader=false;
	receiver->deleteDownloadHandler=deleteHandler;
	fileListReceivers.Set(setId, receiver);
	oldId=setId;
	if (++setId==(unsigned short)-1)
		setId=0;
	return oldId;
}

void FileListTransfer::Send(FileList *fileList, RakPeerInterface *rakPeer, PlayerID recipient, unsigned short setID, PacketPriority priority, char orderingChannel, bool compressData)
{
	RakNet::BitStream outBitstream, encodedData;
	HuffmanEncodingTree tree;
	unsigned int frequencyTable[ 256 ];
	unsigned int i,j;
	unsigned totalCompressedLength, totalLength;
	DataStructures::Queue<FileListNode> compressedFiles;
	FileListNode n;

	if (compressData)
	{
		memset(frequencyTable,0,256*sizeof(unsigned int));

		for (i=0; i < fileList->fileList.Size(); i++)
		{
			for (j=0; j < fileList->fileList[i].dataLength; j++)
			{
				++frequencyTable[(unsigned char)(fileList->fileList[i].data[j])];
			}
		}

		tree.GenerateFromFrequencyTable(frequencyTable);

		// Compress all the files, so we know the total compressed size to be sent
		totalCompressedLength=totalLength=0;
		for (i=0; i < fileList->fileList.Size(); i++)
		{
			encodedData.Reset();
			// Why send compressed chunks if we are not sending the whole file?
			assert(fileList->fileList[i].fileLength==fileList->fileList[i].fileLength);
			tree.EncodeArray((unsigned char*)fileList->fileList[i].data, fileList->fileList[i].dataLength, &encodedData);
			n.dataLength=encodedData.GetNumberOfBitsUsed();
			totalCompressedLength+=BITS_TO_BYTES(n.dataLength);
			totalLength+=fileList->fileList[i].fileLength;
			n.data = new char[BITS_TO_BYTES(n.dataLength)];
			memcpy(n.data, encodedData.GetData(), BITS_TO_BYTES(n.dataLength));
			compressedFiles.Push(n);
		}
	}

	// Write the chunk header, which contains the frequency table, the total number of files, and the total number of bytes
	bool anythingToWrite;
	outBitstream.Write((unsigned char)ID_FILE_LIST_TRANSFER_HEADER);
	outBitstream.Write(setID);
	anythingToWrite=fileList->fileList.Size()>0;
	outBitstream.Write(anythingToWrite);
	if (anythingToWrite)
	{
		if (compressData)
		{
			outBitstream.Write(true);
			for (i=0; i < 256; i++)
				outBitstream.WriteCompressed(frequencyTable[i]);
			outBitstream.WriteCompressed(fileList->fileList.Size());
			outBitstream.WriteCompressed(totalLength);
			outBitstream.WriteCompressed(totalCompressedLength);
		}
		else
		{
			outBitstream.Write(false);
			outBitstream.WriteCompressed(fileList->fileList.Size());
			outBitstream.WriteCompressed(totalLength);
		}
		
		rakPeer->Send(&outBitstream, priority, RELIABLE_ORDERED, orderingChannel, recipient, false);

		// Send each possibly compressed file
		for (i=0; i < compressedFiles.Size(); i++)
		{
			outBitstream.Reset();
			outBitstream.Write((unsigned char)ID_FILE_LIST_TRANSFER_FILE);
			outBitstream.Write(fileList->fileList[i].context);
			outBitstream.Write(setID);
			outBitstream.WriteCompressed(i);
			outBitstream.WriteCompressed(fileList->fileList[i].dataLength); // Original length
			if (compressData)
				outBitstream.WriteCompressed(compressedFiles[i].dataLength); // Compressed bitlength			}
			stringCompressor->EncodeString(fileList->fileList[i].filename, 512, &outBitstream);
			if (compressData)
			{
				outBitstream.WriteBits((const unsigned char*)compressedFiles[i].data, compressedFiles[i].dataLength);
				delete [] compressedFiles[i].data;
			}
			else
				outBitstream.WriteBits((const unsigned char*)fileList->fileList[i].data, fileList->fileList[i].dataLength);
			
			rakPeer->Send(&outBitstream, priority, RELIABLE_ORDERED, orderingChannel, recipient, false);
		}
	}
	else
		rakPeer->Send(&outBitstream, priority, RELIABLE_ORDERED, orderingChannel, recipient, false);
}

bool FileListTransfer::DecodeSetHeader(Packet *packet)
{
	unsigned i;
	unsigned int frequencyTable[ 256 ];
	bool anythingToWrite;
	unsigned short setID;
	RakNet::BitStream inBitStream(packet->data, packet->length, false);
	inBitStream.IgnoreBits(8);
	inBitStream.Read(setID);
	FileListReceiver *fileListReceiver;
	if (fileListReceivers.Has(setID)==false)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}
	fileListReceiver=fileListReceivers.Get(setID);
	if (fileListReceiver->allowedSender!=packet->playerId)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

#ifdef _DEBUG
	assert(fileListReceiver->gotSetHeader==false);
#endif

	inBitStream.Read(anythingToWrite);
	if (anythingToWrite)
	{
		inBitStream.Read(fileListReceiver->isCompressed);
		if (fileListReceiver->isCompressed)
		{
			for (i=0; i < 256; i++)
				inBitStream.ReadCompressed(frequencyTable[i]);
			fileListReceiver->tree.GenerateFromFrequencyTable(frequencyTable);
			inBitStream.ReadCompressed(fileListReceiver->setCount);
			inBitStream.ReadCompressed(fileListReceiver->setTotalFinalLength);
			if (inBitStream.ReadCompressed(fileListReceiver->setTotalCompressedTransmissionLength))
			{
				fileListReceiver->gotSetHeader=true;
				return true;
			}
		}
		else
		{
			inBitStream.ReadCompressed(fileListReceiver->setCount);
			inBitStream.ReadCompressed(fileListReceiver->setTotalFinalLength);
			fileListReceiver->setTotalCompressedTransmissionLength=fileListReceiver->setTotalFinalLength;
		}
	}
	else
	{
		fileListReceiver->downloadHandler->OnFile(0, 0, 0, 0, 0, setID, 0, 0, 0, 0);
		return true;
	}

	return false;
}

bool FileListTransfer::DecodeFile(Packet *packet, bool fullFile)
{
	unsigned char *decompressedFileData;
	char fileName[512];
	unsigned char context;
	unsigned fileIndex, fileLength, compressedLength;
	unsigned bitLength;
	unsigned len;
	unsigned short setID;
	RakNet::BitStream inBitStream(packet->data, packet->length, false);
	inBitStream.IgnoreBits(8);

	unsigned int partCount;
	unsigned int partTotal;
	unsigned int partLength;
	if (fullFile==false)
	{
		inBitStream.Read(partCount);
		inBitStream.Read(partTotal);
		inBitStream.Read(partLength);
		inBitStream.IgnoreBits(8);
	}
	inBitStream.Read(context);
	inBitStream.Read(setID);
	FileListReceiver *fileListReceiver;
	if (fileListReceivers.Has(setID)==false)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}
	fileListReceiver=fileListReceivers.Get(setID);
	if (fileListReceiver->allowedSender!=packet->playerId)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

#ifdef _DEBUG
	assert(fileListReceiver->gotSetHeader==true);
#endif
	
	inBitStream.ReadCompressed(fileIndex);
	inBitStream.ReadCompressed(fileLength);
	if (fileListReceiver->isCompressed)
	{
		inBitStream.ReadCompressed(bitLength);
		compressedLength=BITS_TO_BYTES(bitLength);
	}

	if (stringCompressor->DecodeString(fileName, 512, &inBitStream)==false)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return false;
	}

	if (fullFile)
	{
	decompressedFileData = new unsigned char [fileLength];

	if (fileListReceiver->isCompressed)
	{
		len=fileListReceiver->tree.DecodeArray(&inBitStream, bitLength, fileLength, decompressedFileData);
		if (len!=fileLength)
		{
#ifdef _DEBUG
			assert(0);
#endif
			delete [] decompressedFileData;
			return false;
		}
	}
	else
		inBitStream.Read((char*)decompressedFileData, fileLength);
	}
	

	// User callback for this file.
	if (fullFile)
	{
	fileListReceiver->downloadHandler->OnFile(fileIndex, fileName, (char*)decompressedFileData, compressedLength, fileLength, setID, fileListReceiver->setCount, fileListReceiver->setTotalCompressedTransmissionLength, fileListReceiver->setTotalFinalLength, context);
	delete [] decompressedFileData;

	// If this set is done, free the memory for it.
	if (fileListReceiver->setCount==fileIndex+1)
	{
		if (fileListReceiver->deleteDownloadHandler)
			delete fileListReceiver->downloadHandler;
		delete fileListReceiver;
		fileListReceivers.Delete(setID);
	}

	}
	else
		fileListReceiver->downloadHandler->OnFileProgress(fileIndex, fileName, compressedLength, fileLength, setID, fileListReceiver->setCount, fileListReceiver->setTotalCompressedTransmissionLength, fileListReceiver->setTotalFinalLength, context, partCount, partTotal, partLength);

	return true;
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
PluginReceiveResult FileListTransfer::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	switch (packet->data[0]) 
	{
	case ID_CONNECTION_LOST:
	case ID_DISCONNECTION_NOTIFICATION:
		RemoveReceiver(packet->playerId);
	break;
	case ID_FILE_LIST_TRANSFER_HEADER:
		DecodeSetHeader(packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_FILE_LIST_TRANSFER_FILE:
		DecodeFile(packet, true);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	case ID_DOWNLOAD_PROGRESS:
		if (packet->length>sizeof(MessageID)+sizeof(unsigned int)*3 &&
			packet->data[sizeof(MessageID)+sizeof(unsigned int)*3]==ID_FILE_LIST_TRANSFER_FILE)
		{
			DecodeFile(packet, false);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	}
		break;
	}

	return RR_CONTINUE_PROCESSING;
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void FileListTransfer::OnDisconnect(RakPeerInterface *peer)
{
	Clear();	
}
void FileListTransfer::Clear(void)
{
	unsigned i;
	for (i=0; i < fileListReceivers.Size(); i++)
	{
		if (fileListReceivers[i]->deleteDownloadHandler)
			delete fileListReceivers[i]->downloadHandler;
		delete fileListReceivers[i];
	}
	fileListReceivers.Clear();
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void FileListTransfer::OnCloseConnection(RakPeerInterface *peer, PlayerID playerId)
{
	RemoveReceiver(playerId);
}

void FileListTransfer::RemoveReceiver(PlayerID playerId)
{
	unsigned i;
	i=0;
	while (i < fileListReceivers.Size())
	{
		if (fileListReceivers[i]->allowedSender==playerId)
		{
			if (fileListReceivers[i]->deleteDownloadHandler)
				delete fileListReceivers[i]->downloadHandler;
			delete fileListReceivers[i];
			fileListReceivers.RemoveAtIndex(i);
		}
		else
			i++;
	}
}
void FileListTransfer::OnAttach(RakPeerInterface *peer)
{
	rakPeer=peer;

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
