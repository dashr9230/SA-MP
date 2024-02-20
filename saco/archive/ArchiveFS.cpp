
#include "ArchiveFS.h"

#include "CryptoContext.h"
#include "KeyPair.h"
#include "Signer.h"
#include "Hasher.h"
#include "TinyEncrypt.h"
#include "Obfuscator.h"

//------------------------------------

DWORD CArchiveFS::ms_dwHashInit = OBFUSCATE_DATA(0x9E3779B9);

//------------------------------------

CArchiveFS::CArchiveFS(void)
{
	m_dwNumEntries = SAA_MAX_ENTRIES;
	m_bLoaded = false;
	m_bEntriesLoaded = false;
}

//------------------------------------

CArchiveFS::CArchiveFS(DWORD dwNumEntries, DWORD dwFDSize)
{
	m_dwNumEntries = dwNumEntries;
	m_bLoaded = false;
	m_bEntriesLoaded = false;

	m_Header.dwFakeDataSize = dwFDSize;
}

//------------------------------------

CArchiveFS::~CArchiveFS(void)
{
}

//------------------------------------

DWORD CArchiveFS::HashString(PCHAR szString)
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

void CArchiveFS::LoadEntries()
{
	// Get the file signature, verify it... use the result to decode the entries table

	// Verify the Archive Signature, and decode the Entry block
	CCryptoContext context;
	CKeyPair keyPair(&context);
	CHasher hasher(&context);
	CSigner signer;
	CTinyEncrypt tinyEnc;
	DWORD i;

	// 1. Load the signature from the file
	DWORD dwSignSize = 128;		//m_Header.headerV2.dwSignSize;
	BYTE *pbSignature;
	DWORD dwSignDataEnd;

	pbSignature = new BYTE[dwSignSize];
	m_pStream->Seek(-(INT)dwSignSize, CAbstractStream::SeekEnd);
	dwSignDataEnd = m_pStream->Tell();
	m_pStream->Read(pbSignature, dwSignSize);

	// 2. Hash the stuff (excluding the header and signature!)
	BYTE *pbReadData;
	DWORD dwReadSize;
	const DWORD dwReadBlockSize = 10 * 1024;	// 10kb

	m_pStream->Seek(m_Header.SizeOf());			// start from the actual data section
	pbReadData = new BYTE[dwReadBlockSize];
	for(i=m_Header.SizeOf(); i<dwSignDataEnd; ) {
		dwReadSize = m_pStream->Read(pbReadData, dwReadBlockSize);
		if (i+dwReadSize > dwSignDataEnd)
			hasher.AddData(dwSignDataEnd - i, pbReadData);
		else
			hasher.AddData(dwReadSize, pbReadData);
		i += dwReadSize;
	}
	delete[] pbReadData;

	// 3. Load the key and verify the signature
	BOOL bVerified;

	keyPair.LoadFromMemory(RSA_PUB_KEY_SIZE, (BYTE*)RSA_PUB_KEY, RSA_XOR_KEY);
	signer.SetSignature(dwSignSize, pbSignature);
	bVerified = signer.VerifySignature(&hasher, &keyPair);

	delete[] pbSignature;

	// Set the obfuscation decoding mask based on the bVerified value
	m_dwObfsMask = -((INT)bVerified);		// if its 1 (true), then 0xffffffff, else 0.

	// 4. Decode the TEA encrypted archive entry

	m_pStream->Seek((dwSignDataEnd - m_dwNumEntries*sizeof(SAA_ENTRY)));
	DWORD dwFilePos = m_pStream->Tell();
	m_pStream->Read(m_pEntries, sizeof(SAA_ENTRY), m_dwNumEntries);
	dwFilePos = m_pStream->Tell();

	tinyEnc.SetKey((BYTE*)TEA_KEY, TEA_XOR_KEY);
	tinyEnc.DecryptData(sizeof(SAA_ENTRY)*m_dwNumEntries, reinterpret_cast<BYTE*>(m_pEntries));

	// 5. Build a binary tree of the entries.. it makes searching for files faster (since we have a 
	//    huge index with fake entries)
	for(i=0; i<m_dwNumEntries; i++) {
		m_EntryBTreeRoot.AddEntry(&m_pEntries[i]);
	}

	// Done.

	m_bEntriesLoaded = true;
}

//------------------------------------

bool CArchiveFS::Load(char* szFileName)
{
	if (m_bLoaded)
		Unload();

	m_pStream = new CFileStream(szFileName, CFileStream::TypeBinary, CFileStream::ModeRead);

	m_Header.Read(m_pStream);

	m_Header.XorV2Identifier();

	m_bLoaded = true;

	if (!m_Header.VerifyIdentifier()) {
		Unload();
		return false;
	}

	return true;
}

//------------------------------------

bool CArchiveFS::Load(BYTE* pbData, DWORD nLength)
{
	if (m_bLoaded)
		Unload();

	m_pStream = new CMemoryStream(pbData, nLength);

	m_Header.Read(m_pStream);

	m_Header.XorV2Identifier();

	m_bLoaded = true;

	if (!m_Header.VerifyIdentifier()) {
		Unload();
		return false;
	}

	return true;
}

//------------------------------------

void CArchiveFS::Unload()
{
	if (!m_bLoaded)
		return;

	delete m_pStream;
	m_pStream = NULL;

	m_bLoaded = false;
	m_bEntriesLoaded = false;
}

//------------------------------------

DWORD CArchiveFS::GetFileIndex(DWORD dwFileHash)
{
	if (!m_bEntriesLoaded)
		LoadEntries();

	AFS_ENTRYBT_NODE* node = m_EntryBTreeRoot.FindEntry(dwFileHash);

	if (node == NULL) {
		return FS_INVALID_FILE;
	}

	SAA_ENTRY saaEntry = *(node->pEntry);	// Always make a copy of saaEntry before decrypting it
											// Otherwise, the data decryption will get messed up

	saaEntry.dwDataBlock = UNOBFUSCATE_DATA(saaEntry.dwDataBlock) ^ (saaEntry.dwFileNameHash & this->m_dwObfsMask);
	if (node->pEntry == &(m_pEntries[saaEntry.dwPrevEntry]))
		return FS_INVALID_FILE;

	// Okay, we got a file. 
	// TODO: It might be wise at this point to start a thread to decrypt the data
	// Chances are if the index was requested, data for it will be requested.

	// Painfully evil conversion from SAA_ENTRY* to DWORD
	DWORD* ppEntry = reinterpret_cast<DWORD*>(&node);
	return *ppEntry;
}

//------------------------------------

DWORD CArchiveFS::GetFileIndex(char* szFileName) 
{
	// PRE: szFileName must be the filename only (no paths!)

	if (!m_bEntriesLoaded)
		LoadEntries();

	CHAR szFileNameLC[MAX_PATH];
	strcpy(szFileNameLC, szFileName);
	_strlwr(szFileNameLC);

	DWORD dwHash = this->HashString(szFileNameLC);

	DWORD dwIndex = GetFileIndex(dwHash);

#ifdef _DEBUG
	if (dwIndex != FS_INVALID_FILE)
	{
		CHAR szDebugMsg[1024];
		sprintf(szDebugMsg, "ArchiveFS: Requested file: %s...\n", szFileNameLC);
		OutputDebugString(szDebugMsg);
	}
#endif

	return dwIndex;
}

//------------------------------------

DWORD CArchiveFS::GetFileSize(DWORD dwFileIndex)
{
	AFS_ENTRYBT_NODE* node = *(reinterpret_cast<AFS_ENTRYBT_NODE**>(&dwFileIndex));

	SAA_ENTRY saaEntry = *(node->pEntry);	// Make a copy!
	saaEntry.dwDataBlock = UNOBFUSCATE_DATA(saaEntry.dwDataBlock) ^ (saaEntry.dwFileNameHash & this->m_dwObfsMask);
	return saaEntry.dwFileSize;
}

//------------------------------------

BYTE* CArchiveFS::GetFileData(DWORD dwFileIndex)
{
	CTinyEncrypt tinyEnc;

	AFS_ENTRYBT_NODE* node = *(reinterpret_cast<AFS_ENTRYBT_NODE**>(&dwFileIndex));

	SAA_ENTRY saaEntry = *(node->pEntry);	// Make a copy!
	saaEntry.dwDataBlock = UNOBFUSCATE_DATA(saaEntry.dwDataBlock) ^ (saaEntry.dwFileNameHash & this->m_dwObfsMask);

	DWORD dwFileSize;

	if (node->pbData != NULL) {
		return node->pbData;
	} else {
		// Alloc memory (in blocks!)
		dwFileSize = saaEntry.dwFileSize;
		if (dwFileSize % SAA_BLOCK_SIZE != 0)
			dwFileSize += (SAA_BLOCK_SIZE - (dwFileSize % SAA_BLOCK_SIZE));

		node->pbData = new BYTE[dwFileSize];

		// Determine offset to data
		SAA_ENTRY prevEntry;
		DWORD dwDataOffset = m_Header.SizeOf();

		if (saaEntry.dwPrevEntry != m_Header.headerV2.dwInvalidIndex) {
			prevEntry = saaEntry;
			do {
				prevEntry = m_pEntries[prevEntry.dwPrevEntry];
				prevEntry.dwDataBlock = UNOBFUSCATE_DATA(prevEntry.dwDataBlock) ^ (prevEntry.dwFileNameHash & this->m_dwObfsMask);

				dwFileSize = prevEntry.dwFileSize;
				if (dwFileSize % SAA_BLOCK_SIZE != 0)
					dwFileSize += (SAA_BLOCK_SIZE - (dwFileSize % SAA_BLOCK_SIZE));
				dwDataOffset += dwFileSize;

			} while(prevEntry.dwPrevEntry != m_Header.headerV2.dwInvalidIndex);
		}

		m_pStream->Seek(dwDataOffset);

		// Load the data in blocks and decrypt it
		BYTE* pbTEAKey = reinterpret_cast<BYTE*>(this->m_pEntries) +
								(saaEntry.dwFileNameHash % (sizeof(SAA_ENTRY)*m_dwNumEntries-TEA_KEY_SIZE));

		tinyEnc.SetKey(pbTEAKey, 0);

		for(DWORD i=0; i<saaEntry.dwFileSize; i+=SAA_BLOCK_SIZE) {
			m_pStream->Read(node->pbData+i, SAA_BLOCK_SIZE);
			tinyEnc.DecryptData(SAA_BLOCK_SIZE, node->pbData+i);
		}

		return node->pbData;

	}
}

void CArchiveFS::UnloadData(DWORD dwFileIndex)
{
	AFS_ENTRYBT_NODE* node = *(reinterpret_cast<AFS_ENTRYBT_NODE**>(&dwFileIndex));

	if (node->pbData != NULL)
	{
		delete[] node->pbData;
		node->pbData = NULL;
	}
}
