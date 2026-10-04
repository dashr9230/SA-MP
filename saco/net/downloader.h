#pragma once

//----------------------------------------------------

#define MAX_DOWNLOAD_SLOTS 6

//----------------------------------------------------

// the block AddURLRequest copies into a slot, 0x11E bytes
struct DOWNLOAD_REQUEST
{
	char  _gap0[0x11];
	DWORD dwCrc;
	char  _gap15[0x109];
};

//----------------------------------------------------

// one worker slot, 0x230 bytes; offsets pinned by 0x1000C010 and 0x1000C0C0
struct DOWNLOAD_SLOT
{
	DWORD dwSlotId;              // set to the 1-based slot index
	char  field_4;
	char  field_5;
	char  field_6;               // slot live flag
	char  field_7;               // cancel flag
	DWORD dwThread;              // _beginthread handle, -1 when the spawn failed
	char  szPath[257];
	char  field_10D;
	DWORD field_10E;             // initialised to -1
	DOWNLOAD_REQUEST Request;
};

//----------------------------------------------------

// the record the lookups below match on; only the touched fields are known
struct DOWNLOAD_ENTRY
{
	char  field_0;
	char  field_1;
	char  _gap2[5];
	char  field_7;
	char  _gap8[4];
	DWORD field_C;
	DWORD dwCrc;
	char  _gap14[0x32];
	DWORD field_46;
	DWORD field_4A;
	char  _gap4E[0xA];
	char  field_58;
	char  field_59;
	DWORD field_5A;              // retry countdown
};

//----------------------------------------------------

// the list the game side queries; whether it is embedded in CDownloadManager
// at +0 is still open, the offsets are the same either way
class CDownloadList
{
public:

	DOWNLOAD_ENTRY **m_pEntries;
	DWORD m_dwCount;
	char  field_8;
	char  szDir1[0x105];
	char  szDir2[0x105];
	char  _gap213[4];
	DWORD field_217;
	char  _gap21B[5];
	BYTE  field_220;
	BYTE  field_221;

	void FUNC_10017D10(int a1) { // .text:10017D10
		if(field_217 != a1) {
			field_217 = a1;
			field_221 = 0;
			field_220 = 0;
		}
	};

	DOWNLOAD_ENTRY *GetAt(DWORD dwIndex) { if(dwIndex >= m_dwCount) return NULL; return m_pEntries[dwIndex]; }

	BOOL FUNC_1000C590(DWORD dwId); // .text:1000C590
	BOOL FUNC_1000C5D0(DWORD dwId); // .text:1000C5D0
	BOOL FUNC_1000C610(char *szName); // .text:1000C610
	bool FUNC_1000D110(DWORD dwCrc); // .text:1000D110
	bool FUNC_1000D150(DWORD dwCrc); // .text:1000D150
	char FUNC_1000D190(DWORD dwCrc); // .text:1000D190
	void FUNC_1000D1E0(); // .text:1000D1E0
	DWORD FUNC_1000D240(DWORD dwCrc); // .text:1000D240
	DWORD FUNC_1000D280(DWORD dwCrc); // .text:1000D280
	int  FUNC_1000D2C0(DWORD dwCrc); // .text:1000D2C0
	int  FUNC_1000C650(DWORD a1, DWORD dwCrc, DWORD a3, DWORD a4); // .text:1000C650
	int  FUNC_1000C770(DWORD a1, DWORD dwCrc, DWORD a3, DWORD a4); // .text:1000C770
	int  FUNC_1000D320(DWORD dwCrc); // .text:1000D320
};

//----------------------------------------------------

class CDownloadManager
{
public:

	char _gap0[0x1344];
	BYTE m_bSlotsReady;
	DOWNLOAD_SLOT m_Slots[MAX_DOWNLOAD_SLOTS];

	bool FUNC_1000C010(); // .text:1000C010
	void AddURLRequest(DOWNLOAD_REQUEST *pRequest, char *szUrl); // .text:1000C380
	void FUNC_1000C0C0(); // .text:1000C0C0
	bool FUNC_1000C0F0(); // .text:1000C0F0
	void FUNC_1000C140(); // .text:1000C140
	int  FUNC_1000C190(); // .text:1000C190
	int  FUNC_1000C260(); // .text:1000C260
	int  FUNC_1000C2F0(); // .text:1000C2F0
};

//----------------------------------------------------
// EOF
