
#include "ArchiveFS.h"

#include "CryptoContext.h"
#include "KeyPair.h"
#include "Signer.h"
#include "Hasher.h"
#include "TinyEncrypt.h"

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

