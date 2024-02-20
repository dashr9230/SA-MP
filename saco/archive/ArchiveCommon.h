
#pragma once

#include <windows.h>
#include "Stream.h"

// SAA_FILE_ID := {'S', 'A', 'M', 'P'} ignoring first 3 bits of each char
//	first 3 bits are 010 anyhow :)
#define SAA_FILE_ID			0x83433
#define SAA_FILE_VERSION	2
#define SAA_BLOCK_SIZE		2048

#define SAA_MAX_ENTRIES		256

#define SAA_MAX_FAKEDATA	120

typedef struct _SAA_ENTRY
{
	DWORD dwFileNameHash;
	union
	{
		struct
		{
			DWORD dwPrevEntry	: 8;		// index to previous entry (link to fake entry if none)
			DWORD dwFileSize	: 24;		// 24bits = max filesize of 16mb
		};
		DWORD dwDataBlock;
	};
} SAA_ENTRY;

typedef struct _SAA_FILE_HEADER
{
	// This is a fake header
	struct VER1_HEADER
	{
		DWORD dwSAAV;
		DWORD dwFileCount;
		WORD wFakeData[SAA_MAX_FAKEDATA];
	} headerV1; /* 248 bytes */

	struct VER2_HEADER
	{
		union
		{
			struct
			{
				DWORD dwSAMPID		: 20;
				DWORD dwVersion		: 3;
				DWORD dwSignSize	: 8;
				DWORD dwPadding1	: 1;
			};
			DWORD dwCompleteID;
		};
		union
		{
			struct
			{
				DWORD dwPadding2		: 5;
				DWORD dwInvalidIndex	: 8;
				DWORD dwPadding3		: 19;
			};
			DWORD dwXORKey;
		};
	} headerV2;	/* 8 bytes */

	DWORD dwFakeDataSize;

	_SAA_FILE_HEADER()
	{
		dwFakeDataSize = SAA_MAX_FAKEDATA;
	}

	DWORD SizeOf()
	{
		return(sizeof(DWORD)*2 + sizeof(WORD)*dwFakeDataSize + sizeof(VER2_HEADER));
	}

	bool VerifyIdentifier()
	{
		return ((headerV2.dwSAMPID == SAA_FILE_ID) &&
				(headerV2.dwVersion == SAA_FILE_VERSION));
	}

	void XorV2Identifier() {
		this->headerV2.dwCompleteID ^= this->headerV2.dwXORKey;
	}

	void Read(CAbstractStream *pStream)
	{
		pStream->Read(&headerV1, sizeof(DWORD)*2 + sizeof(WORD)*dwFakeDataSize);
		pStream->Read(&headerV2, sizeof(VER2_HEADER));
	}

} SAA_FILE_HEADER;
