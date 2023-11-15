
#include "../main.h"

DWORD dwSystemMemory;
DWORD dwStreamingMemory;

extern int iGtaVersion;

//----------------------------------------------------------

void UnFuckAndCheck(DWORD addr, int size, BYTE byteCheck)
{
	DWORD d;
	VirtualProtect((PVOID)addr,size,PAGE_EXECUTE_READWRITE,&d);

	if(byteCheck != *(PBYTE)addr) {
#ifdef _DEBUG
		char s[256];
		sprintf(s,"Failed Check At Addr: 0x%X",addr);
		OutputDebugString(s);
#endif
		while(byteCheck != *(PBYTE)addr) Sleep(1);

		VirtualProtect((PVOID)addr,size,PAGE_EXECUTE_READWRITE,&d);
	}
}

//----------------------------------------------------------

void UnFuck(DWORD addr, int size)
{
	DWORD d;
	VirtualProtect((PVOID)addr,size,PAGE_EXECUTE_READWRITE,&d);
}

//----------------------------------------------------------

BOOL ApplyPreGamePatches()
{
	BYTE * pbyteVersionDetermination = (PBYTE)ADDR_BYPASS_VIDS_USA10;
	int iCounter=0;

	// MAIN VERSION DETERMINING LOGIC
	while( (*pbyteVersionDetermination != 0x89) &&
		   (*pbyteVersionDetermination != 0xC8) )
	{
		if (*(PBYTE)ADDR_GAME_STARTED == 1) {
			return FALSE;
		} else {
			Sleep(10);
			iCounter++;
			if(iCounter>6000) { // 60 seconds have passed
				return FALSE;
			}
		}
	}

	if(*pbyteVersionDetermination == 0x89) {
		iGtaVersion = GTASA_VERSION_USA10;
	} 
	else if(*pbyteVersionDetermination == 0xC8) {
		iGtaVersion = GTASA_VERSION_EU10;
	}

	// (skip to starting screen)
	if(iGtaVersion == GTASA_VERSION_USA10) {
		UnFuck(ADDR_BYPASS_VIDS_USA10,6);
		*(BYTE *)ADDR_ENTRY = 5;
		memset((PVOID)ADDR_BYPASS_VIDS_USA10,0x90,6);
	}
	else if (iGtaVersion == GTASA_VERSION_EU10) {
		UnFuck(ADDR_BYPASS_VIDS_EU10,6);
		*(BYTE *)ADDR_ENTRY = 5;
		memset((PVOID)ADDR_BYPASS_VIDS_EU10,0x90,6);
	}

	// Loading screens
	UnFuck(0x866CD8,10);
	UnFuck(0x866CCC,10);
	strcpy((PCHAR)0x866CD8,"title");
	strcpy((PCHAR)0x866CCC,"title");

	UnFuck(0x745B87,68);
	memset((PVOID)0x745B87,0x90,68);

	UnFuck(0x7459E1,2);
	memset((LPVOID)0x7459E1,0x90,2);

	UnFuckAndCheck(0x561872,30,0x85);
	*(PBYTE)0x561872 = 0x33;
	*(PBYTE)0x561873 = 0xC0;
	memset((LPVOID)0x561874,0x90,27);

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	dwSystemMemory = statex.ullTotalPhys / (1024 * 1024);

	if(dwSystemMemory > 4000)
		dwStreamingMemory = 0x40000000; // 1024MB
	else if(dwSystemMemory > 2000)
		dwStreamingMemory = 0x20000000; // 512MB
	else if(dwSystemMemory > 1000)
		dwStreamingMemory = 0x10000000; // 256MB
	else if(dwSystemMemory > 500)
		dwStreamingMemory = 0x08000000; // 128MB
	else
		dwStreamingMemory = 0x06000000; // 96MB

	// Modify the streaming memory hardcoded values
	UnFuck(0x5B8E6A,4);
	*(DWORD *)0x5B8E6A = dwStreamingMemory;

	UnFuckAndCheck(0x4083C0,1,0xB8);
	*(PBYTE)0x4083C0 = 0xC3;

	UnFuck(0x590099,5);
	memset((LPVOID)0x590099,0x90,5);

	UnFuck(0x53E94C,1);
	*(PBYTE)0x53E94C = 2;

	UnFuck(0x731F60,4);
	*(PDWORD)0x731F60 = 20000;

	return TRUE;
}

//----------------------------------------------------------
