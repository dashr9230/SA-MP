
#include "ArchiveBuilder.h"

#include "../saco/archive/CryptoContext.h"
#include "../saco/archive/KeyPair.h"
#include "../saco/archive/Signer.h"
#include "../saco/archive/Hasher.h"
#include "../saco/archive/TinyEncrypt.h"
#include "../saco/archive/Obfuscator.h"

#include <time.h>

#define DO_ENCRYPT

//------------------------------------

DWORD CArchiveBuilder::ms_dwHashInit = OBFUSCATE_DATA(0x9E3779B9);

//------------------------------------

CArchiveBuilder::CArchiveBuilder(void)
{
	m_dwNumEntries = SAA_MAX_ENTRIES;
	m_dwFDSize = SAA_MAX_FAKEDATA;
	m_bProperHeader = TRUE;
}

//------------------------------------

CArchiveBuilder::CArchiveBuilder(DWORD dwNumEntries, DWORD dwFDSize, BOOL bProperHeader)
{
	m_dwNumEntries = dwNumEntries;
	m_dwFDSize = dwFDSize;
	m_bProperHeader = bProperHeader;
}

//------------------------------------

CArchiveBuilder::~CArchiveBuilder(void)
{
	if (!m_vecEntries.empty()) {
		EntryMemStateVector_t::iterator it = m_vecEntries.begin();
		while(it != m_vecEntries.end()) {
			AB_ENTRY_MEMSTATE* pEntry = (*it);
			fclose(pEntry->fiFile);
			delete pEntry;
			it++;
		}
	}
}

//------------------------------------

DWORD CArchiveBuilder::HashString(PCHAR szString)
{
	// This is an implementation of the Jenkins hash

#	define mix(a,b,c) \
	{ \
		a -= b; a -= c; a ^= (c>>13); \
		b -= c; b -= a; b ^= (a<<8); \
		c -= a; c -= b; c ^= (b>>13); \
		a -= b; a -= c; a ^= (c>>12);  \
		b -= c; b -= a; b ^= (a<<16); \
		c -= a; c -= b; c ^= (b>>5); \
		a -= b; a -= c; a ^= (c>>3);  \
		b -= c; b -= a; b ^= (a<<10); \
		c -= a; c -= b; c ^= (b>>15); \
	}

	register BYTE* k = (BYTE*)szString;
	register DWORD initval = 0x12345678;
	register DWORD length;

	length = (DWORD)strlen(szString);

	register DWORD a,b,c,len;

	/* Set up the internal state */
	len = length;
	a = b = UNOBFUSCATE_DATA(ms_dwHashInit); /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */

	/*---------------------------------------- handle most of the key */
	while (len >= 12)
	{
	a += (k[0] +((DWORD)k[1]<<8) +((DWORD)k[2]<<16) +((DWORD)k[3]<<24));
	b += (k[4] +((DWORD)k[5]<<8) +((DWORD)k[6]<<16) +((DWORD)k[7]<<24));
	c += (k[8] +((DWORD)k[9]<<8) +((DWORD)k[10]<<16)+((DWORD)k[11]<<24));
	mix(a,b,c);
	k += 12; len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += length;
	switch(len)              /* all the case statements fall through */
	{
		case 11: c+=((DWORD)k[10]<<24);
		case 10: c+=((DWORD)k[9]<<16);
		case 9 : c+=((DWORD)k[8]<<8);
		  /* the first byte of c is reserved for the length */
		case 8 : b+=((DWORD)k[7]<<24);
		case 7 : b+=((DWORD)k[6]<<16);
		case 6 : b+=((DWORD)k[5]<<8);
		case 5 : b+=k[4];
		case 4 : a+=((DWORD)k[3]<<24);
		case 3 : a+=((DWORD)k[2]<<16);
		case 2 : a+=((DWORD)k[1]<<8);
		case 1 : a+=k[0];
		 /* case 0: nothing left to add */
	}
	mix(a,b,c);
	/*-------------------------------------------- report the result */
	return c;

}

//------------------------------------

PCHAR CArchiveBuilder::ExtractFileName(PCHAR szString)
{
	DWORD dwOffset = (DWORD)strlen(szString);
	while((dwOffset > 0) && (szString[dwOffset] != '\\'))
		dwOffset--;
	if (dwOffset == 0)
		return szString;
	else
		return szString+dwOffset+1;
}

//------------------------------------

DWORD CArchiveBuilder::AddFile(PCHAR szFileName)
{

	AB_ENTRY_MEMSTATE* pEntry = new AB_ENTRY_MEMSTATE();

	PCHAR szNameOnly = ExtractFileName(szFileName);
	if ((strlen(szNameOnly)+1) > sizeof(pEntry->szFileName))
		throw "Internal Error: Not enough memory allocated for the length of the filename.";

	strcpy(pEntry->szFileName, szNameOnly);
	_strlwr(pEntry->szFileName);
	pEntry->dwFileNameHash = HashString(pEntry->szFileName);

	FILE* fiFile;
	fiFile = fopen(szFileName, "rb");
	if (!fiFile)
		throw "Could not load archive data file.";

	pEntry->fiFile = fiFile;

	fseek(fiFile, 0, SEEK_END);
	pEntry->dwFileSize = (DWORD)ftell(fiFile);

	fseek(fiFile, 0, SEEK_SET);

	if (pEntry->dwFileSize > 16*1024*1024)
		throw "Archive data file is greater than 16MB.";

	m_vecEntries.push_back(pEntry);

	return pEntry->dwFileNameHash;

}

//------------------------------------

void CArchiveBuilder::BuildEntryTable(SAA_ENTRY *pEntryTable, DWORD dwInvalidIndex)
{
	CCryptoContext context;
	BOOL* pbEntryUsed;

	EntryMemStateVector_t::iterator it;
	DWORD dwIndex;
	AB_ENTRY_MEMSTATE* pEntry;

	// Create some stuff to make things simple
	pbEntryUsed = new BOOL[m_dwNumEntries];
	memset(pbEntryUsed, 0, sizeof(BOOL)*m_dwNumEntries);

	// Make the entire table's data random
	context.GenerateRandom(m_dwNumEntries*sizeof(SAA_ENTRY), reinterpret_cast<BYTE*>(pEntryTable));

	// Ensure that there are no hash collisions between whats in the entry table and our files
	BOOL bCollision = FALSE;
	for(DWORD i=0; i<m_dwNumEntries; i++) {

		it = m_vecEntries.begin();
		while(it != m_vecEntries.end()) {
			pEntry = (*it);

			if (pEntryTable[i].dwFileNameHash == pEntry->dwFileNameHash) {
				pEntryTable[i].dwFileNameHash = GetRandom();
				bCollision = TRUE;
				break;
			}

			it++;
		}

		if (bCollision) {
			bCollision = FALSE;
			i--;
		}
	}

	/*
	// Ver1: Set all the invalid entries to the invalid index
	for(DWORD i=0; i<m_dwNumEntries; i++) {
		pEntryTable[i].dwPrevEntry = dwInvalidIndex;
	}
	*/

	// Ver2: Set all the invalid entries to itself, use the invalid index for the first entry instead
	for(DWORD i=0; i<m_dwNumEntries; i++) {
		pEntryTable[i].dwPrevEntry = i;
	}

	// Randomly place our loaded entries into the entry table
	it = m_vecEntries.begin();
	while(it != m_vecEntries.end()) {
		pEntry = (*it);

		// Generate a random value that works for us
		while(1) {
			dwIndex = (GetRandom() >> 5) & 0xFF;	// Take it from the middle!
			dwIndex %= m_dwNumEntries;

			// Check if it matches our reserved index for invalid files
			if (dwIndex == dwInvalidIndex)
				continue;

			// Check to see if any existing entries have this index
			if (pbEntryUsed[dwIndex])
				continue;

			break;
		}

		pbEntryUsed[dwIndex] = true;

		pEntry->pEntry = &(pEntryTable[dwIndex]);
		pEntry->dwEntryTableOffset = dwIndex;
		pEntry->pbTEAKey = reinterpret_cast<BYTE*>(pEntryTable) +
			(pEntry->dwFileNameHash % (sizeof(SAA_ENTRY)*m_dwNumEntries-TEA_KEY_SIZE));

		pEntry->pEntry->dwFileNameHash = pEntry->dwFileNameHash;
		pEntry->pEntry->dwFileSize = pEntry->dwFileSize;

		if (it == m_vecEntries.begin()) {
			// Ver2: Set the first index to the invalid index
			pEntry->pEntry->dwPrevEntry = dwInvalidIndex;
		} else {
			pEntry->pEntry->dwPrevEntry = (*(it-1))->dwEntryTableOffset;
		}

		it++;
	}

	/*
	// Ver1:
	// Process the first entry finally and set its previous index to
	// an invalid entry, but not to our invalid index
	it = m_vecEntries.begin();
	while(1) {
		dwIndex = (GetRandom() >> 5) & 0xFF;	// Take it from the middle!

		// Check if it matches our reserved index for invalid files
		if (dwIndex == dwInvalidIndex)
			continue;

		// Check to see if any existing entries have this index
		if (pbEntryUsed[dwIndex])
			continue;

		break;
	}
	(*it)->pEntry->dwPrevEntry = dwIndex;
	*/

	// Now obfuscate the non-hash part of all our entries
	// XOR it with the hash for good measure.
	for(DWORD i=0; i<m_dwNumEntries; i++) {
		pEntryTable[i].dwDataBlock ^= pEntryTable[i].dwFileNameHash;
		pEntryTable[i].dwDataBlock = OBFUSCATE_DATA(pEntryTable[i].dwDataBlock);
	}

}

//------------------------------------

DWORD CArchiveBuilder::GetRandom()
{
	static BOOL bSeeded = FALSE;
	if (!bSeeded) {
		srand((DWORD)time(NULL));
		bSeeded = true;
	}

	DWORD dwRandom;

	dwRandom = rand();
	dwRandom <<= 15;
	dwRandom ^= rand();
	dwRandom <<= 15;
	dwRandom ^= rand();

	return dwRandom;
}

//------------------------------------

void CArchiveBuilder::BuildHeader(SAA_FILE_HEADER* pFileHeader)
{
	CCryptoContext context;

	context.GenerateRandom(pFileHeader->SizeOf(), reinterpret_cast<BYTE*>(pFileHeader));

	pFileHeader->dwFakeDataSize = this->m_dwFDSize;

	if (this->m_bProperHeader)
		pFileHeader->InitializeDataV1();

	pFileHeader->InitializeDataV2();

	pFileHeader->headerV2.dwInvalidIndex %= m_dwNumEntries;

}

//------------------------------------

void CArchiveBuilder::BuildEntryData(FILE *fiArchive)
{
	CCryptoContext context;
	EntryMemStateVector_t::iterator it;
	const DWORD dwReadBlockSize = SAA_BLOCK_SIZE;
	CTinyEncrypt tinyEncrypt;

	BYTE *pbReadData = new BYTE[dwReadBlockSize];

	it = m_vecEntries.begin();
	while(it != m_vecEntries.end()) {

		AB_ENTRY_MEMSTATE* pEntry = (*it);

		tinyEncrypt.SetKey(pEntry->pbTEAKey, 0);

		DWORD dwReadSize;
		for(DWORD i=0; i<pEntry->dwFileSize; i+=dwReadBlockSize) {
			dwReadSize = (DWORD)fread(pbReadData, 1, dwReadBlockSize, pEntry->fiFile);
			if (dwReadSize < dwReadBlockSize) {
				context.GenerateRandom(dwReadBlockSize-dwReadSize, pbReadData+dwReadSize);
			}
#ifdef DO_ENCRYPT
			tinyEncrypt.EncryptData(dwReadBlockSize, pbReadData);
#endif
			fwrite(pbReadData, 1, dwReadBlockSize, fiArchive);
		}

		it++;

	}

	delete[] pbReadData;

}

//------------------------------------

void CArchiveBuilder::SignArchive(FILE *fiArchive, DWORD dwSignatureOffset, SAA_FILE_HEADER *pHeader)
{

	// Sign the archive file

	CCryptoContext context;
	CKeyPair keyPair(&context);
	CHasher hasher(&context);
	CSigner signer;

	// 1. Hash the stuff
	fseek(fiArchive, pHeader->SizeOf(), SEEK_SET);	  // start from the actual data section
	const DWORD dwReadBlockSize = 10 * 1024;	// 10kb
	BYTE *pbReadData = new BYTE[dwReadBlockSize];
	DWORD dwReadSize;
	DWORD dwPos = ftell(fiArchive);
	while(!feof(fiArchive)) {
		dwReadSize = (DWORD)fread(pbReadData, 1, dwReadBlockSize, fiArchive);
		hasher.AddData(dwReadSize, pbReadData);
		dwPos = ftell(fiArchive);
	}
	delete[] pbReadData;

	// 2. Load the key, and sign the hash
	keyPair.LoadFromFile("pkey.bin");
	signer.SignHash(&hasher);
	keyPair.ReleaseKey();

	// 3. Write the signature
	pHeader->headerV2.dwSignSize = signer.GetSignatureLength();
	fseek(fiArchive, dwSignatureOffset, SEEK_SET);
	fwrite(signer.GetSignature(), 1, pHeader->headerV2.dwSignSize, fiArchive);

}

//------------------------------------

void CArchiveBuilder::WriteArchive(PCHAR szFileName)
{
	SAA_FILE_HEADER header;
	SAA_ENTRY *pEntryTable;
	FILE* fiArchive;

	fiArchive = fopen(szFileName, "w+b");

	pEntryTable = new SAA_ENTRY[m_dwNumEntries];

	// 1. Build the header
	BuildHeader(&header);

	// 2. Build the entry table
	BuildEntryTable(pEntryTable, header.headerV2.dwInvalidIndex);

	// 3. Encode and write the file data
	fseek(fiArchive, header.SizeOf(), SEEK_SET);
	BuildEntryData(fiArchive);

	// 4. Encrypt and write the entry table
	//    (Note that the file data decryption must be done with decrypted entry table used for keys)
	CTinyEncrypt tinyEnc;
	tinyEnc.LoadKey("skey.bin");
#ifdef DO_ENCRYPT
	tinyEnc.EncryptData(sizeof(SAA_ENTRY)*m_dwNumEntries, reinterpret_cast<BYTE*>(pEntryTable));
#endif
	fwrite(pEntryTable, sizeof(SAA_ENTRY), m_dwNumEntries, fiArchive);

	// 5. Sign the data (excluding the header)
	fflush(fiArchive);
	SignArchive(fiArchive, (DWORD)ftell(fiArchive), &header);

	// 6. Finish the header and write it
#ifdef DO_ENCRYPT
	header.XorV2Identifier();
#endif
	header.Write(fiArchive);

	// And we're done.
	fclose(fiArchive);

	delete[] pEntryTable;

}

//------------------------------------
