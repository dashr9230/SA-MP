#ifndef __FILE_LIST
#define __FILE_LIST

#include "Export.h"
#include "DS_List.h"

namespace RakNet
{
	class BitStream;
}

struct FileListNode
{
	char *filename;
	char *data;
	unsigned dataLength;
	unsigned fileLength;
	unsigned char context; // User specific data for whatever, describing this file.
};

//int RAK_DLL_EXPORT FileListNodeComp( char * const &key, const FileListNode &data );

class RakPeerInterface;

class RAK_DLL_EXPORT FileList
{
public:
	FileList();
	~FileList();
	void AddFilesFromDirectory(const char *applicationDirectory, const char *subDirectory, bool writeHash, bool writeData, bool recursive, unsigned char context);
	void Clear(void);
	void Serialize(RakNet::BitStream *outBitStream);
	bool Deserialize(RakNet::BitStream *inBitStream);
	void ListMissingOrChangedFiles(const char *applicationDirectory, FileList *missingOrChangedFiles, bool alwaysWriteHash, bool neverWriteHash);
	// Return the files that need to be written to make \a input match this FileList.
	// Specify dirSubset to only consider files in fileList that start with this path
	// specify remoteSubdir to assume that all filenames in input start with this path, so strip it off when comparing filenames.
	void GetDeltaToCurrent(FileList *input, FileList *output, const char *dirSubset, const char *remoteSubdir);
	void PopulateDataFromDisk(const char *applicationDirectory, bool writeFileData, bool writeFileHash, bool removeUnknownFiles);
	void WriteDataToDisk(const char *applicationDirectory);
	void AddFile(const char *filename, const char *data, const unsigned dataLength, const unsigned fileLength, unsigned char context);
	void AddFile(const char *filepath, const char *filename, unsigned char context);
	void DeleteFiles(const char *applicationDirectory);


	// Here so you can read it, but don't modify it
	DataStructures::List<FileListNode> fileList;
protected:
};

#endif
