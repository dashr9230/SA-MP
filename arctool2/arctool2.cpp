
#include <windows.h>
#include <stdio.h>
#include "../saco/archive/CryptoContext.h"
#include "../saco/archive/KeyPair.h"
#include "../saco/archive/TinyEncrypt.h"
#include "../saco/archive/ArchiveFS.h"
#include "ArchiveBuilder.h"

void CreateArchive(PCHAR szFileList, PCHAR szArchive)
{

	CArchiveBuilder archiveBuilder;

	// Load the file list
	FILE* fiList;
	fiList = fopen(szFileList, "rb");
	if (!fiList)
		throw "Could not find file list.";
	fseek(fiList, 0, SEEK_END);
	DWORD dwFileListSize = ftell(fiList);
	CHAR* szFiles = new CHAR[dwFileListSize+1];
	fseek(fiList, 0, SEEK_SET);
	fread(szFiles, 1, dwFileListSize, fiList);
	szFiles[dwFileListSize] = 0;
	fclose(fiList);

	// Load the files and stuff
	CHAR* szToken;
	szToken = strtok(szFiles, "\r\n");
	while(szToken != NULL) {
		if (szToken[0] != 0 && szToken[0] != '#') {
			printf("Adding file: %s...\t", szToken);
			DWORD hash = archiveBuilder.AddFile(szToken);
			printf("  Added, file hash=0x%x\n", hash);
		}
		szToken = strtok(NULL, "\r\n");
	}

	// Write the archive
	printf("Writing archive...\n");
	archiveBuilder.WriteArchive(szArchive);

	// Clean up
	delete[] szFiles;

}


void CreateArchiveSingle(PCHAR szFile, PCHAR szArchive)
{

	CArchiveBuilder archiveBuilder(4, 0, FALSE);

	// Load the file
	printf("Adding file: %s...\t", szFile);
	DWORD hash = archiveBuilder.AddFile(szFile);
	printf("  Added, file hash=0x%x\n", hash);

	// Write the archive
	printf("Writing archive...\n");
	archiveBuilder.WriteArchive(szArchive);

}

// Helper for test routine
PCHAR ExtractFileName(PCHAR szString)
{
	DWORD dwOffset = (DWORD)strlen(szString);
	while((dwOffset > 0) && (szString[dwOffset] != '\\'))
		dwOffset--;
	if (dwOffset == 0)
		return szString;
	else
		return szString+dwOffset+1;
}

// Test routine to see if the archive making works properly...
// And for archive verification!
void VerifyArchive(PCHAR szFileList, PCHAR szArchive)
{

	CArchiveFS fs;

	const DWORD BUFFER_SIZE = 1024;
	BYTE* pbBuffer[BUFFER_SIZE];

	// Load the file list
	FILE* fiList;
	fiList = fopen(szFileList, "rb");
	if (!fiList)
		throw "Could not find file list.";
	fseek(fiList, 0, SEEK_END);
	DWORD dwFileListSize = ftell(fiList);
	CHAR* szFiles = new CHAR[dwFileListSize+1];
	fseek(fiList, 0, SEEK_SET);
	fread(szFiles, 1, dwFileListSize, fiList);
	szFiles[dwFileListSize] = 0;
	fclose(fiList);

	// Load the archive
	if (!fs.Load(szArchive))
		throw "Failed to load archive.";

	// Load the files and stuff
	CHAR* szToken;
	FILE* fiData;

	szToken = strtok(szFiles, "\r\n");
	while(szToken != NULL) {
		if (szToken[0] != 0 && szToken[0] != '#') {

			// Get the file data
			DWORD dwFileIndex = fs.GetFileIndex(ExtractFileName(szToken));

			if (dwFileIndex != FS_INVALID_FILE) {

				DWORD dwFileSize = fs.GetFileSize(dwFileIndex);
				BYTE* pbFileData = fs.GetFileData(dwFileIndex);

				printf("Verifying file: %s...\t", szToken);

				// Load the actual file and verify the data
				fiData = fopen(szToken, "rb");

				if (dwFileSize == 0)
					printf("  Warning: 0 byte file.\n");

				for(DWORD i=0; i<dwFileSize; i+=BUFFER_SIZE)
				{
					DWORD dwReadBytes;
					if ((i + BUFFER_SIZE) >= dwFileSize) {
						// The last block, read using our real size
						dwReadBytes = dwFileSize % BUFFER_SIZE;
					} else {
						dwReadBytes = BUFFER_SIZE;
					}
					dwReadBytes = (DWORD)fread(pbBuffer, 1, dwReadBytes, fiData);
					if (memcmp(pbBuffer, pbFileData+i, dwReadBytes) != 0) {
						throw "  Failed to verify file.\n";
					}
				}
				fclose(fiData);

				printf("  Verified.\n");

			} else {
				printf("  Warning: File ""%s"" not found in archive.\n", szToken);
			}

		}
		szToken = strtok(NULL, "\r\n");
	}

	// Close the archive
	fs.Unload();

	// Clean up
	delete[] szFiles;
}

void VerifyArchiveSingle(PCHAR szFile, PCHAR szArchive)
{
	const DWORD BUFFER_SIZE = 1024;
	BYTE* pbBuffer[BUFFER_SIZE];

	CArchiveFS fs(4, 0);

	// Load the archive
	if (!fs.Load(szArchive))
		throw "Failed to load archive.";

	// Load the files and stuff
	FILE* fiData;

	// Get the file data
	DWORD dwFileIndex = fs.GetFileIndex(ExtractFileName(szFile));

	if (dwFileIndex != FS_INVALID_FILE) {

		DWORD dwFileSize = fs.GetFileSize(dwFileIndex);
		BYTE* pbFileData = fs.GetFileData(dwFileIndex);

		printf("Verifying file: %s...\t", szFile);

		// Load the actual file and verify the data
		fiData = fopen(szFile, "rb");

		if (dwFileSize == 0)
			printf("  Warning: 0 byte file.\n");

		for(DWORD i=0; i<dwFileSize; i+=BUFFER_SIZE)
		{
			DWORD dwReadBytes;
			if ((i + BUFFER_SIZE) >= dwFileSize) {
				// The last block, read using our real size
				dwReadBytes = dwFileSize % BUFFER_SIZE;
			} else {
				dwReadBytes = BUFFER_SIZE;
			}
			dwReadBytes = (DWORD)fread(pbBuffer, 1, dwReadBytes, fiData);
			if (memcmp(pbBuffer, pbFileData+i, dwReadBytes) != 0) {
				throw "  Failed to verify file.\n";
			}
		}
		fclose(fiData);

		printf("  Verified.\n");

	} else {
		printf("  Warning: File ""%s"" not found in archive.\n", szFile);
	}


	// Close the archive
	fs.Unload();

}

void GenerateKeys()
{
	CCryptoContext context;
	CKeyPair keyPair(&context);
	CTinyEncrypt tinyEncrypt;
	BYTE pbTEAKey[TEA_KEY_SIZE];

	keyPair.GenerateKey();
	keyPair.WriteToFile("pkey.bin");
	keyPair.WriteCHeaderFile("pkey.h");
	keyPair.ReleaseKey();

	context.GenerateRandom(TEA_KEY_SIZE, pbTEAKey);
	tinyEncrypt.SetKey(pbTEAKey, 0);
	tinyEncrypt.WriteKey("skey.bin");
	tinyEncrypt.WriteCHeaderFile("skey.h");
}

void PrintBanner()
{
	printf("SA:MP Archive 2 Tool\n");
	printf("Copyright(C) 2006, SA:MP Team\n");
	printf("Built on " __DATE__ " at " __TIME__ ".\n\n");
}

void PrintUsage()
{
	printf("Usage:\n");
	printf("  -c [filelist.txt] [samp.saa]\n  \tGenerates samp.saa from filelist.txt\n");
	printf("  -v [filelist.txt] [samp.saa]\n  \tVerifies samp.saa using filelist.txt\n");
	printf("  -sc file.dat file.saa\n  \tGenerates single file SAA\n");
	printf("  -sv file.dat file.saa\n  \tVerifies single file SAA\n");
	printf("  -gk\tGenerates a new set of TEA and RSA keys\n");
}

DWORD main(DWORD argc, CHAR** argv)
{

	PrintBanner();

	if (argc == 1) {
		PrintUsage();
	} else {
		if (strcmp(argv[1], "-c")==0) {
			// generate file
			PCHAR szFileList = "filelist.txt";
			PCHAR szArchive = "samp.saa";
			if (argc > 2)
				szFileList = argv[2];
			if (argc > 3)
				szArchive = argv[3];
			try
			{
				printf("Creating archive...\n");
				CreateArchive(szFileList, szArchive);
				printf("Archive generated successfully.\n");
			}
			catch (PCHAR error)
			{
				printf("Error: %s\n", error);
			}
		} else if (strcmp(argv[1], "-v")==0) {
			// verify file
			PCHAR szFileList = "filelist.txt";
			PCHAR szArchive = "samp.saa";
			if (argc > 2)
				szFileList = argv[2];
			if (argc > 3)
				szArchive = argv[3];
			try
			{
				printf("Verifying archive...\n");
				VerifyArchive(szFileList, szArchive);
				printf("Archive verification complete.\n");
			}
			catch (PCHAR error)
			{
				printf("Error: %s\n", error);
			}
		} else if (strcmp(argv[1], "-gk")==0) {
			GenerateKeys();
			printf("Generated keys successfully.\n");
		} else if (argc > 3 && strcmp(argv[1], "-sc")==0) {
			// generate file
			PCHAR szFile = argv[2];
			PCHAR szArchive = argv[3];
			try
			{
				printf("Creating archive...\n");
				CreateArchiveSingle(szFile, szArchive);
				printf("Archive generated successfully.\n");
			}
			catch (PCHAR error)
			{
				printf("Error: %s\n", error);
			}
		} else if (argc > 3 && strcmp(argv[1], "-sv")==0) {
			// generate file
			PCHAR szFile = argv[2];
			PCHAR szArchive = argv[3];
			try
			{
				printf("Verifying archive...\n");
				VerifyArchiveSingle(szFile, szArchive);
				printf("Archive verification complete.\n");
			}
			catch (PCHAR error)
			{
				printf("Error: %s\n", error);
			}
		} else {
			PrintUsage();
		}
	}

#ifdef _DEBUG
	getchar();
#endif


}
