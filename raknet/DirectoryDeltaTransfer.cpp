#include "DirectoryDeltaTransfer.h"
#include "FileList.h"
#include "StringCompressor.h"
#include "RakPeerInterface.h"
#include "FileListTransfer.h"
#include "FileListTransferCBInterface.h"
#include "BitStream.h"
#include "PacketEnumerations.h"
#include "FileOperations.h"

#ifdef _MSC_VER
#pragma warning( push )
#endif

class DDTCallback : public FileListTransferCBInterface
{
public:
	unsigned subdirLen;
	char outputSubdir[512];
	FileListTransferCBInterface *onFileCallback;

	virtual void OnFile(
		unsigned fileIndex,
		char *filename,
		char *fileData,
		unsigned compressedTransmissionLength,
		unsigned finalDataLength,
		unsigned short setID,
		unsigned setCount,	
		unsigned setTotalCompressedTransmissionLength,
		unsigned setTotalFinalLength,
		unsigned char context)
	{
		char fullPathToDir[1024];

		if (filename && fileData && subdirLen < strlen(filename))
		{
			strcpy(fullPathToDir, outputSubdir);
			strcat(fullPathToDir, filename+subdirLen);
			WriteFileWithDirectories(fullPathToDir, (char*)fileData, finalDataLength);
		}
		else
			fullPathToDir[0]=0;

		onFileCallback->OnFile(fileIndex, fullPathToDir, fileData, compressedTransmissionLength, finalDataLength, setID, setCount, setTotalCompressedTransmissionLength, setTotalFinalLength, context);
	}

	virtual void OnFileProgress(unsigned fileIndex,
		char *filename,
		unsigned compressedTransmissionLength,
		unsigned finalDataLength,
		unsigned short setID,
		unsigned setCount,	
		unsigned setTotalCompressedTransmissionLength,
		unsigned setTotalFinalLength,
		unsigned char context,
		unsigned int partCount,
		unsigned int partTotal,
		unsigned int partLength)
	{
		char fullPathToDir[1024];

		if (filename && subdirLen < strlen(filename))
		{
			strcpy(fullPathToDir, outputSubdir);
			strcat(fullPathToDir, filename+subdirLen);
		}
		else
			fullPathToDir[0]=0;

		onFileCallback->OnFileProgress(fileIndex, fullPathToDir, compressedTransmissionLength, finalDataLength, setID, setCount, setTotalCompressedTransmissionLength, setTotalFinalLength, context, partCount, partTotal, partLength);
	}
};

DirectoryDeltaTransfer::DirectoryDeltaTransfer()
{
	applicationDirectory[0]=0;
	fileListTransfer=0;
	availableUploads = new FileList;
	rakPeer=0;
	priority=HIGH_PRIORITY;
	orderingChannel=0;
}
DirectoryDeltaTransfer::~DirectoryDeltaTransfer()
{
	delete availableUploads;
}
void DirectoryDeltaTransfer::SetFileListTransferPlugin(FileListTransfer *flt)
{
	fileListTransfer=flt;
}
void DirectoryDeltaTransfer::SetApplicationDirectory(const char *pathToApplication)
{
	if (pathToApplication==0 || pathToApplication[0]==0)
		applicationDirectory[0]=0;
	else
	{
		strncpy(applicationDirectory, pathToApplication, 510);
		if (applicationDirectory[strlen(applicationDirectory)-1]!='/' && applicationDirectory[strlen(applicationDirectory)-1]!='\\')
			strcat(applicationDirectory, "/");
		applicationDirectory[511]=0;
	}
}
void DirectoryDeltaTransfer::SetUploadSendParameters(PacketPriority _priority, char _orderingChannel)
{
	priority=_priority;
	orderingChannel=_orderingChannel;
}
void DirectoryDeltaTransfer::AddUploadsFromSubdirectory(const char *subdir)
{
	availableUploads->AddFilesFromDirectory(applicationDirectory, subdir, true, false, true, 0);
}
unsigned short DirectoryDeltaTransfer::DownloadFromSubdirectory(const char *subdir, const char *outputSubdir, bool prependAppDirToOutputSubdir, PlayerID host, FileListTransferCBInterface *onFileCallback, PacketPriority _priority, char _orderingChannel)
{
	if (rakPeer->GetIndexFromPlayerID(host)==-1)
		return (unsigned short) -1;

	DDTCallback *transferCallback;
	FileList localFiles;
	// Get a hash of all the files that we already have (if any)
	localFiles.AddFilesFromDirectory(prependAppDirToOutputSubdir ? applicationDirectory : 0, outputSubdir, true, false, true, 0);

	// Prepare the callback data
	transferCallback = new DDTCallback;
	if (subdir && subdir[0])
	{
		transferCallback->subdirLen=(unsigned int)strlen(subdir);
		if (subdir[transferCallback->subdirLen-1]!='/' && subdir[transferCallback->subdirLen-1]!='\\')
			transferCallback->subdirLen++;
	}
	else
		transferCallback->subdirLen=0;
	if (prependAppDirToOutputSubdir)
		strcpy(transferCallback->outputSubdir, applicationDirectory);
	else
		transferCallback->outputSubdir[0]=0;
	if (outputSubdir)
		strcat(transferCallback->outputSubdir, outputSubdir);
	if (transferCallback->outputSubdir[strlen(transferCallback->outputSubdir)-1]!='/' && transferCallback->outputSubdir[strlen(transferCallback->outputSubdir)-1]!='\\')
		strcat(transferCallback->outputSubdir, "/");
	transferCallback->onFileCallback=onFileCallback;

	// Setup the transfer plugin to get the response to this download request
	unsigned short setId = fileListTransfer->SetupReceive(transferCallback, true, host);

	// Send to the host, telling it to process this request
	RakNet::BitStream outBitstream;
	outBitstream.Write((unsigned char) ID_DDT_DOWNLOAD_REQUEST);
	outBitstream.Write(setId);
	stringCompressor->EncodeString(subdir, 256, &outBitstream);
	stringCompressor->EncodeString(outputSubdir, 256, &outBitstream);
    localFiles.Serialize(&outBitstream);
	rakPeer->Send(&outBitstream, _priority, RELIABLE_ORDERED, _orderingChannel, host, false);

	return setId;
}
void DirectoryDeltaTransfer::ClearUploads(void)
{
	availableUploads->Clear();
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void DirectoryDeltaTransfer::OnDownloadRequest(RakPeerInterface *peer, Packet *packet)
{
	char subdir[256];
	char remoteSubdir[256];
	RakNet::BitStream inBitstream(packet->data, packet->length, false);
	FileList remoteFileHash;
	FileList delta;
	unsigned short setId;
    inBitstream.IgnoreBits(8);
	inBitstream.Read(setId);
	stringCompressor->DecodeString(subdir, 256, &inBitstream);
	stringCompressor->DecodeString(remoteSubdir, 256, &inBitstream);
	if (remoteFileHash.Deserialize(&inBitstream)==false)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return;
	}

	availableUploads->GetDeltaToCurrent(&remoteFileHash, &delta, subdir, remoteSubdir);
	delta.PopulateDataFromDisk(applicationDirectory, true, false, true);

	// This will call the ddtCallback interface that was passed to FileListTransfer::SetupReceive on the remote system
	fileListTransfer->Send(&delta, rakPeer, packet->playerId, setId, priority, orderingChannel, true);
}
void DirectoryDeltaTransfer::OnAttach(RakPeerInterface *peer)
{
	rakPeer=peer;
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void DirectoryDeltaTransfer::Update(RakPeerInterface *peer)
{

}
PluginReceiveResult DirectoryDeltaTransfer::OnReceive(RakPeerInterface *peer, Packet *packet)
{
	switch (packet->data[0]) 
	{
	case ID_DDT_DOWNLOAD_REQUEST:
		OnDownloadRequest(peer, packet);
		return RR_STOP_PROCESSING_AND_DEALLOCATE;
	}

	return RR_CONTINUE_PROCESSING;
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void DirectoryDeltaTransfer::OnDisconnect(RakPeerInterface *peer)
{

}

unsigned DirectoryDeltaTransfer::GetNumberOfFilesForUpload(void) const
{
	return availableUploads->fileList.Size();
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
