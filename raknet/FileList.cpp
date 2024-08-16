#include "FileList.h"
#include <assert.h>
#ifndef _COMPATIBILITY_2
	#if defined(_WIN32) || defined(__CYGWIN__)
	#include <io.h>
	#elif !defined ( __APPLE__ ) && !defined ( __APPLE_CC__ )
	#include <sys/io.h>
	#endif
#endif
#include <stdio.h>
#include "DS_Queue.h"
#ifdef _WIN32 
// For mkdir
#include <direct.h>
#else
#include <sys/stat.h>
#include "LinuxStrings.h"
#endif
#include "SHA1.h"
#include "StringCompressor.h"
#include "BitStream.h"
#include "FileOperations.h"

#if !defined(_WIN32)
#define _unlink unlink
#define _mkdir mkdir
#endif

// alloca
#ifdef _COMPATIBILITY_1
#elif defined(_WIN32)
#include <malloc.h>
#elif defined(_COMPATIBILITY_2)
#include "Compatibility2Includes.h"
#else
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "LinuxStrings.h"
#include "_findfirst.h"
#include <stdint.h> //defines intptr_t
#endif

//int RAK_DLL_EXPORT FileListNodeComp( char * const &key, const FileListNode &data )
//{
//	return strcmp(key, data.filename);
//}

FileList::FileList()
{
}
FileList::~FileList()
{
	Clear();
}
void FileList::AddFile(const char *filepath, const char *filename, unsigned char context)
{
	if (filepath==0 || filename==0)
		return;

	char *data;
	FILE *fp = fopen(filepath, "rb");
	if (fp==0)
		return;
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
#if !defined(_COMPATIBILITY_1)
	bool usedAlloca=false;
	if (length < MAX_ALLOCA_STACK_ALLOCATION)
	{
		data = ( char* ) alloca( length );
		usedAlloca=true;
	}
	else
#endif
	{
		data = new char [length];
	}

	AddFile(filename, data, length, length, context);
	fclose(fp);

#if !defined(_COMPATIBILITY_1)
	if (usedAlloca==false)
#endif
		delete [] data;

}
void FileList::AddFile(const char *filename, const char *data, const unsigned dataLength, const unsigned fileLength, unsigned char context)
{
	if (filename==0)
		return;

	// Avoid duplicate insertions unless the data is different, in which case overwrite the old data
	unsigned i;
	for (i=0; i<fileList.Size();i++)
	{
		if (strcmp(fileList[i].filename, filename)==0)
		{
			if (fileList[i].fileLength==fileLength && fileList[i].dataLength==dataLength &&
				(dataLength==0 || memcmp(fileList[i].data, data, dataLength)==0))
				// Exact same file already here
				return;

			// File of the same name, but different contents, so overwrite
			delete [] fileList[i].data;
			delete [] fileList[i].filename;
			fileList.RemoveAtIndex(i);
			break;
		}
	}

	FileListNode n;
	n.filename=new char [strlen(filename)+1];
	if (dataLength)
	{
		n.data=new char [dataLength];
		memcpy(n.data, data, dataLength);
	}
	else
		n.data=0;
	n.dataLength=dataLength;
	n.fileLength=fileLength;
	n.context=context;
	strcpy(n.filename, filename);
		
	fileList.Insert(n);
}
void FileList::AddFilesFromDirectory(const char *applicationDirectory, const char *subDirectory, bool writeHash, bool writeData, bool recursive, unsigned char context)
{
#ifndef _COMPATIBILITY_2
	DataStructures::Queue<char*> dirList;
	char root[260];
	char fullPath[520];
	_finddata_t fileInfo;
	intptr_t dir;
	int file;
	FILE *fp;
	CSHA1 sha1;
	char *dirSoFar, *fileData;
	dirSoFar=new char[520];

	if (applicationDirectory)
		strcpy(root, applicationDirectory);
	else
		root[0]=0;

	int rootLen=(int)strlen(root);
	if (rootLen)
	{
		strcpy(dirSoFar, root);
		if (dirSoFar[strlen(dirSoFar)-1]!='/' && dirSoFar[strlen(dirSoFar)-1]!='\\')
		{
			strcat(dirSoFar, "/");
			rootLen++;
		}
	}
	else
		dirSoFar[0]=0;
	
	if (subDirectory)
	{
		strcat(dirSoFar, subDirectory);
		if (dirSoFar[strlen(dirSoFar)-1]!='/' && dirSoFar[strlen(dirSoFar)-1]!='\\')
		{
			strcat(dirSoFar, "/");
		}
	}
	dirList.Push(dirSoFar);
	while (dirList.Size())
	{
		dirSoFar=dirList.Pop();
		strcpy(fullPath, dirSoFar);
		strcat(fullPath, "*.*");
		dir=_findfirst(fullPath, &fileInfo ); // Read .
		if (dir==-1)
		{
			_findclose(dir);
			delete [] dirSoFar;
			unsigned i;
			for (i=0; i < dirList.Size(); i++)
				delete [] dirList[i];
			return;
		}
		file=_findnext(dir, &fileInfo ); // Read ..
		file=_findnext(dir, &fileInfo ); // Skip ..

		while (file!=-1)
		{
			if ((fileInfo.attrib & (_A_HIDDEN | _A_SUBDIR | _A_SYSTEM))==0)
			{
				strcpy(fullPath, dirSoFar);
				strcat(fullPath, fileInfo.name);
				if (writeData && writeHash)
					fileData= new char [fileInfo.size+SHA1_LENGTH];
				else
					fileData= new char [fileInfo.size];
				fp = fopen(fullPath, "rb");
				if (writeData && writeHash)
					fread(fileData+SHA1_LENGTH, fileInfo.size, 1, fp);
				else
					fread(fileData, fileInfo.size, 1, fp);
				fclose(fp);

				if (writeData && writeHash)
				{
					sha1.Reset();
					sha1.Update( ( unsigned char* ) fileData+SHA1_LENGTH, fileInfo.size );
					sha1.Final();
					memcpy(fileData, sha1.GetHash(), SHA1_LENGTH);
					AddFile((const char*)fullPath+rootLen, fileData, fileInfo.size+SHA1_LENGTH, fileInfo.size, context);
				}
				else if (writeHash)
				{
					sha1.Reset();
					sha1.Update( ( unsigned char* ) fileData, fileInfo.size );
					sha1.Final();
					AddFile((const char*)fullPath+rootLen, (const char*)sha1.GetHash(), SHA1_LENGTH, fileInfo.size, context);
				}
				else if (writeData)
				{
					AddFile(fullPath+rootLen, fileData, fileInfo.size, fileInfo.size, context);
				}
				else
					AddFile(fullPath+rootLen, 0, 0, fileInfo.size, context);

				delete [] fileData;
			}
			else if ((fileInfo.attrib & _A_SUBDIR) && (fileInfo.attrib & (_A_HIDDEN | _A_SYSTEM))==0 && recursive)
			{
				char *newDir=new char[520];
				strcpy(newDir, dirSoFar);
				strcat(newDir, fileInfo.name);
				strcat(newDir, "/");
				dirList.Push(newDir);
			}
			file=_findnext(dir, &fileInfo );
		}

		_findclose(dir);
		delete [] dirSoFar;
	}
#endif
}
void FileList::Clear(void)
{
	unsigned i;
	for (i=0; i<fileList.Size(); i++)
	{
		delete [] fileList[i].data;
		delete [] fileList[i].filename;
	}
	fileList.Clear();
}
void FileList::Serialize(RakNet::BitStream *outBitStream)
{
	outBitStream->WriteCompressed(fileList.Size());
	unsigned i;
	for (i=0; i < fileList.Size(); i++)
	{
		outBitStream->WriteCompressed(fileList[i].context);
		stringCompressor->EncodeString(fileList[i].filename, 512, outBitStream);
		outBitStream->Write((bool)(fileList[i].dataLength>0==true));
		if (fileList[i].dataLength>0)
		{
			outBitStream->WriteCompressed(fileList[i].dataLength);
			outBitStream->Write(fileList[i].data, fileList[i].dataLength);
		}

		outBitStream->Write((bool)(fileList[i].fileLength==fileList[i].dataLength));
		if (fileList[i].fileLength!=fileList[i].dataLength)
			outBitStream->WriteCompressed(fileList[i].fileLength);
	}
}
bool FileList::Deserialize(RakNet::BitStream *inBitStream)
{
	bool b, dataLenNonZero, fileLenMatchesDataLen;
	char filename[512];
	unsigned int fileListSize;
	FileListNode n;
	b=inBitStream->ReadCompressed(fileListSize);
#ifdef _DEBUG
	assert(b);
	assert(fileListSize < 10000);
#endif
	if (b==false || fileListSize > 10000)
		return false; // Sanity check
	Clear();	
	unsigned i;
	for (i=0; i < fileListSize; i++)
	{
		inBitStream->ReadCompressed(n.context);
		stringCompressor->DecodeString((char*)filename, 512, inBitStream);
		inBitStream->Read(dataLenNonZero);
		if (dataLenNonZero)
		{
			inBitStream->ReadCompressed(n.dataLength);
			// sanity check
			if (n.dataLength>200000000)
			{
#ifdef _DEBUG
				assert(n.dataLength<=200000000);
#endif
				return false;
			}
			n.data=new char [n.dataLength];
			inBitStream->Read(n.data, n.dataLength);
		}
		else
		{
			n.dataLength=0;
			n.data=0;
		}
		
		b=inBitStream->Read(fileLenMatchesDataLen);
		if (fileLenMatchesDataLen)
			n.fileLength=n.dataLength;
		else
			b=inBitStream->ReadCompressed(n.fileLength);
#ifdef _DEBUG
		assert(b);
#endif
		if (b==0)
		{
			Clear();
			return false;
		}
		n.filename=new char [strlen(filename)+1];
		strcpy(n.filename, filename);
		fileList.Insert(n);
	}

	return true;
}
void FileList::GetDeltaToCurrent(FileList *input, FileList *output, const char *dirSubset, const char *remoteSubdir)
{
	// For all files in this list that do not match the input list, write them to the output list.
	// dirSubset allows checking only a portion of the files in this list.
	unsigned thisIndex, inputIndex;
	unsigned dirSubsetLen, localPathLen, remoteSubdirLen;
	bool match;
	if (dirSubset)
		dirSubsetLen = (unsigned int) strlen(dirSubset);
	else
		dirSubsetLen = 0;
	if (remoteSubdir && remoteSubdir[0])
	{
		remoteSubdirLen=(unsigned int) strlen(remoteSubdir);
		if (IsSlash(remoteSubdir[remoteSubdirLen-1]))
			remoteSubdirLen--;
	}
	else
		remoteSubdirLen=0;

	for (thisIndex=0; thisIndex < fileList.Size(); thisIndex++)
	{
		localPathLen = (unsigned int) strlen(fileList[thisIndex].filename);
		while (localPathLen>0)
		{
			if (IsSlash(fileList[thisIndex].filename[localPathLen-1]))
			{
				localPathLen--;
				break;
			}
			localPathLen--;
		}

		// fileList[thisIndex].filename has to match dirSubset and be shorter or equal to it in length.
		if (dirSubsetLen>0 &&
			(localPathLen<dirSubsetLen ||
			_strnicmp(fileList[thisIndex].filename, dirSubset, dirSubsetLen)!=0 ||
			(localPathLen>dirSubsetLen && IsSlash(fileList[thisIndex].filename[dirSubsetLen])==false)))
			continue;

		match=false;
		for (inputIndex=0; inputIndex < input->fileList.Size(); inputIndex++)
		{
			// If the filenames, hashes, and lengths match then skip this element in fileList.  Otherwise write it to output
			if (_stricmp(input->fileList[inputIndex].filename+remoteSubdirLen,fileList[thisIndex].filename+dirSubsetLen)==0)
			{
				match=true;
				if (input->fileList[inputIndex].fileLength==fileList[thisIndex].fileLength &&
					input->fileList[inputIndex].dataLength==fileList[thisIndex].dataLength &&
					memcmp(input->fileList[inputIndex].data,fileList[thisIndex].data,fileList[thisIndex].dataLength)==0)
				{
					// File exists on both machines and is the same.
					break;
				}
				else
				{
					// File exists on both machines and is not the same.
					output->AddFile(fileList[inputIndex].filename, 0,0, fileList[inputIndex].fileLength, 0);
					break;
				}
			}
		}
		if (match==false)
		{
            // Other system does not have the file at all
			output->AddFile(fileList[thisIndex].filename, 0,0, fileList[thisIndex].fileLength, 0);
		}
	}
}
void FileList::ListMissingOrChangedFiles(const char *applicationDirectory, FileList *missingOrChangedFiles, bool alwaysWriteHash, bool neverWriteHash)
{
	unsigned fileLength;
	CSHA1 sha1;
	FILE *fp;
	char fullPath[512];
	unsigned i;
	char *fileData;

	for (i=0; i < fileList.Size(); i++)
	{
		strcpy(fullPath, applicationDirectory);
		if (fullPath[strlen(fullPath)-1]!='/' && fullPath[strlen(fullPath)-1]!='\\')
			strcat(fullPath, "/");
		strcat(fullPath,fileList[i].filename);
		fp=fopen(fullPath, "rb");
		if (fp==0)
		{
			missingOrChangedFiles->AddFile(fileList[i].filename, 0, 0, 0, 0);
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			fileLength = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			if (fileLength != fileList[i].fileLength && alwaysWriteHash==false)
			{
				missingOrChangedFiles->AddFile(fileList[i].filename, 0, 0, fileLength, 0);
			}
			else
			{

				fileData= new char [fileLength];
				fread(fileData, fileLength, 1, fp);
				fclose(fp);

				sha1.Reset();
				sha1.Update( ( unsigned char* ) fileData, fileLength );
				sha1.Final();

				delete [] fileData;

				if (fileLength != fileList[i].fileLength || memcmp( sha1.GetHash(), fileList[i].data, 20)!=0)
				{
					if (neverWriteHash==false)
						missingOrChangedFiles->AddFile((const char*)fileList[i].filename, (const char*)sha1.GetHash(), 20, fileLength, 0);
					else
						missingOrChangedFiles->AddFile((const char*)fileList[i].filename, 0, 0, fileLength, 0);
				}
			}
		}
	}
}
void FileList::PopulateDataFromDisk(const char *applicationDirectory, bool writeFileData, bool writeFileHash, bool removeUnknownFiles)
{
	FILE *fp;
	char fullPath[512];
	unsigned i;
	CSHA1 sha1;

	i=0;
	while (i < fileList.Size())
	{
		delete [] fileList[i].data;
		strcpy(fullPath, applicationDirectory);
		if (fullPath[strlen(fullPath)-1]!='/' && fullPath[strlen(fullPath)-1]!='\\')
			strcat(fullPath, "/");
		strcat(fullPath,fileList[i].filename);
		fp=fopen(fullPath, "rb");
		if (fp)
		{
			if (writeFileHash || writeFileData)
			{
				fseek(fp, 0, SEEK_END);
				fileList[i].fileLength = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if (writeFileHash)
				{
					if (writeFileData)
					{
						// Hash + data so offset the data by SHA1_LENGTH
						fileList[i].data=new char[fileList[i].fileLength+SHA1_LENGTH];
						fread(fileList[i].data+SHA1_LENGTH, fileList[i].fileLength, 1, fp);
						sha1.Reset();
						sha1.Update((unsigned char*)fileList[i].data+SHA1_LENGTH, fileList[i].fileLength);
						sha1.Final();
						memcpy(fileList[i].data, sha1.GetHash(), SHA1_LENGTH);
					}
					else
					{
						// Hash only
						fileList[i].dataLength=SHA1_LENGTH;
						if (fileList[i].fileLength < SHA1_LENGTH)
							fileList[i].data=new char[SHA1_LENGTH];
						else
							fileList[i].data=new char[fileList[i].fileLength];
						fread(fileList[i].data, fileList[i].fileLength, 1, fp);
						sha1.Reset();
						sha1.Update((unsigned char*)fileList[i].data, fileList[i].fileLength);
						sha1.Final();
						memcpy(fileList[i].data, sha1.GetHash(), SHA1_LENGTH);
					}
				}
				else
				{
					// Data only
					fileList[i].dataLength=fileList[i].fileLength;
					fileList[i].data=new char[fileList[i].fileLength];
					fread(fileList[i].data, fileList[i].fileLength, 1, fp);
				}

				fclose(fp);
				i++;
			}
			else
			{
				fileList[i].data=0;
				fileList[i].dataLength=0;
			}
		}
		else
		{
			if (removeUnknownFiles)
			{
				delete [] fileList[i].filename;
				fileList.RemoveAtIndex(i);
			}
			else
				i++;			
		}
	}
}
void FileList::WriteDataToDisk(const char *applicationDirectory)
{
	char fullPath[512];
	unsigned i,j;

	for (i=0; i < fileList.Size(); i++)
	{
		strcpy(fullPath, applicationDirectory);
		if (fullPath[strlen(fullPath)-1]!='/' && fullPath[strlen(fullPath)-1]!='\\')
			strcat(fullPath, "/");
		strcat(fullPath,fileList[i].filename);
		
		// Security - Don't allow .. in the filename anywhere so you can't write outside of the root directory
		for (j=1; j < strlen(fileList[i].filename); j++)
		{
			if (fileList[i].filename[j]=='.' && fileList[i].filename[j-1]=='.')
			{
#ifdef _DEBUG
				assert(0);
#endif
				// Just cancel the write entirely
				return;
			}
		}

		WriteFileWithDirectories(fullPath, fileList[i].data, fileList[i].dataLength);
	}
}


void FileList::DeleteFiles(const char *applicationDirectory)
{
	char fullPath[512];
	unsigned i,j;

	for (i=0; i < fileList.Size(); i++)
	{
		// The filename should not have .. in the path - if it does ignore it
		for (j=1; j < strlen(fileList[i].filename); j++)
		{
			if (fileList[i].filename[j]=='.' && fileList[i].filename[j-1]=='.')
			{
#ifdef _DEBUG
				assert(0);
#endif
				// Just cancel the deletion entirely
				return;
			}
		}

		strcpy(fullPath, applicationDirectory);
		strcat(fullPath, fileList[i].filename);
        _unlink(fullPath);
	}
}
