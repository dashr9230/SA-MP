
#include "../main.h"

DWORD dwSystemMemory;
DWORD dwStreamingMemory;

extern int iGtaVersion;

//----------------------------------------------------------

#define memadd(num,arr) dwAlloc+=num;cBytes=(char*)&dwAlloc;for(int i=0;i<4;i++)arr[i]=cBytes[i]
#define patch(a,b) _patch(a,b,sizeof(b))

//----------------------------------------------------------

void _patch(DWORD dwAddress,BYTE* bData,int iSize)
{
	DWORD dwProtect[2];
	VirtualProtect((PVOID)dwAddress,iSize,PAGE_EXECUTE_READWRITE,&dwProtect[0]);
	memcpy((PVOID)dwAddress,bData,iSize);
	VirtualProtect((PVOID)dwAddress,iSize,dwProtect[0],&dwProtect[1]);
}

//----------------------------------------------------------

void SetIPLs(int iIPLs, int iGtaVersion)
{
	DWORD dwAlloc = (DWORD) malloc(iIPLs+4);
	if(dwAlloc)
	{
		memset((void*)dwAlloc,0x0,iIPLs+4);
		char* cBytes = (char*)&dwAlloc;

		BYTE bIPLs[] = { cBytes[0], cBytes[1], cBytes[2], cBytes[3] };

		if(iGtaVersion == GTASA_VERSION_USA10)
		{
			patch(0x1569777,bIPLs);
			patch(0x15649FA,bIPLs);
			patch(0x1561160,bIPLs);
		}
		else
		{
			patch(0x1569717,bIPLs);
			patch(0x156495A,bIPLs);
			patch(0x156115C,bIPLs);
		}
		patch(0x40619B,bIPLs);
		patch(0x405C3D,bIPLs);
	}
}

//----------------------------------------------------------

void SetTimedObjects(int iTimedObjects)
{
	DWORD dwAlloc = (DWORD) malloc((0x24*iTimedObjects)+4);
	if(dwAlloc)
	{
		memset((LPVOID)dwAlloc,0x00,(0x24*iTimedObjects)+4);
		for(DWORD i=dwAlloc+4;i<(dwAlloc+4+(0x24*iTimedObjects));i+=0x24)
		{
			*(BYTE*) i = 0xB0;
			*(BYTE*) (i+1) = 0xBC;
			*(BYTE*) (i+2) = 0x85;
			*(BYTE*) (i+10) = 0xFF;
			*(BYTE*) (i+11) = 0xFF;
			*(BYTE*) (i+34) = 0xFF;
			*(BYTE*) (i+35) = 0xFF;
		}
		char* cBytes = (char*)&dwAlloc;
		BYTE bTimedObjects[] = { cBytes[0], cBytes[1], cBytes[2], cBytes[3] };
		patch(0x4C66B1,bTimedObjects);
		patch(0x4C66C2,bTimedObjects);
		patch(0x84BC51,bTimedObjects);
		patch(0x856261,bTimedObjects);
		patch(0x4C683B,bTimedObjects);
		memadd(4,bTimedObjects);
		patch(0x4C6464,bTimedObjects);
		patch(0x4C66BD,bTimedObjects);
		cBytes = (char*)&iTimedObjects;
		BYTE pushTimedObjects[] = { 0x68, cBytes[0], cBytes[1], cBytes[2], cBytes[3] };
		patch(0x4C58A5,pushTimedObjects);
	}
}

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

BYTE pbyteVehiclePoolAllocPatch[] = {0x6A,0x00,0x68,0xC6,0x2,0x00,0x00}; // 710
BYTE pbyteCollisionPoolAllocPatch[] = { 0x68,0xFF,0x7E,0x00,0x00 }; // 32511

void ApplyGameLimitPatches()
{
	// Increase the vehicle pool limit (see top of proc for patch)
	UnFuckAndCheck(0x551024,sizeof(pbyteVehiclePoolAllocPatch),0x68);
	memcpy((PVOID)0x551024,pbyteVehiclePoolAllocPatch,sizeof(pbyteVehiclePoolAllocPatch));

	// Increase Buildings
	UnFuck(0x55105F,4);
	*(DWORD *)0x55105F = 20000;

	// Increase Dummys
	UnFuck(0x5510CF,4);
	*(DWORD *)0x5510CF = 4000;

	// Increase Ptr Node Single
	UnFuck(0x550F46,4);
	*(DWORD *)0x550F46 = 100000;

	// Increase Ptr Node Double
	UnFuck(0x550F82,4);
	*(DWORD *)0x550F82 = 8000;

	// Increase EntryInfoNode
	UnFuck(0x550FBA,4);
	*(DWORD *)0x550FBA = 5000;

	// Increase Object Pool
	UnFuck(0x551097,4);
	*(DWORD *)0x551097 = 3000;

	// Increase the ped pool limit (240)
	UnFuck(0x550FF2,1);
	*(PBYTE)0x550FF2 = 240;

	// And we need 240 ped intelligence too plz
	UnFuck(0x551283,1);
	*(PBYTE)0x551283 = 240;

	// And a larger task pool
	UnFuck(0x551140,1);
	*(PBYTE)0x551140 = 0x05; // 1524

	// And a larger event pool
	UnFuck(0x551178,1);
	*(PBYTE)0x551178 = 0x01; // 456

	// Increase CPlaceable matrix array
	UnFuck(0x54F3A1,4);
	*(DWORD *)0x54F3A1 = 6000;

	// Increase the collision model ptr
	UnFuck(0x551106,sizeof(pbyteCollisionPoolAllocPatch));
	memcpy((PVOID)0x551106,pbyteCollisionPoolAllocPatch,sizeof(pbyteCollisionPoolAllocPatch));
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

#pragma pack(1)
typedef struct _PED_MODEL
{
	DWORD func_tbl;
	BYTE  data[64];
} PED_MODEL;

PED_MODEL PedModelsMemory[319];

void RelocatePedsListHack()
{
	BYTE *aPedsListMemory = (BYTE*)&PedModelsMemory[0];

	// Init the mem
	int x=0;
	while(x!=319) {
		PedModelsMemory[x].func_tbl = 0x85BDC0;
		memset(PedModelsMemory[x].data,0,64);
		x++;
	}
	// Patch the GetPedsModelInfo to use us
	// instead of the gta_sa.exe mem.
	UnFuck(0x4C67AD,4);
	*(DWORD *)0x4C67AD = (DWORD)aPedsListMemory;
}

//----------------------------------------------------------

#pragma pack(1)
typedef struct _BASE_MODEL_INFO
{
	DWORD func_tbl;
	BYTE  data[28];
} BASE_MODEL_INFO;

BASE_MODEL_INFO BaseModelInfoMemory[20000];

DWORD dwPatchAddrBaseModelInfoReloc[14] = {
0x4C63F2,0x4C662D,0x4C6822,0x4C6829,0x4C6877,0x4C6881,
0x4C6890,0x4C68A5,0x4C68F3,0x4C6932,0x4C6971,0x4C69B0,
0x4C69EF,0x4C6A2E };

void RelocateBaseModelInfoHack()
{
	DWORD oldProt;
	BYTE *aModelListMemory = (BYTE*)&BaseModelInfoMemory[0];

	// Init the mem
	int x=0;
	while(x!=20000) {
		BaseModelInfoMemory[x].func_tbl = 0x85BBF0;
		memset(PedModelsMemory[x].data,0,28);
		x++;
	}

	x=0;
	while(x!=14) {
		VirtualProtect((LPVOID)dwPatchAddrBaseModelInfoReloc[x],4,PAGE_EXECUTE_READWRITE,&oldProt);
		*(PDWORD)dwPatchAddrBaseModelInfoReloc[x] = (DWORD)aModelListMemory;
		x++;
	}
}

//----------------------------------------------------------
// FOLLOWING IS TO RELOCATE THE SCANLIST MEMORY, A BIG
// HACK THAT ALLOWS US TO HAVE MORE THAN 2 CPlayerInfo STRUCTURES.

#define SCANLIST_SIZE 8*20000

unsigned char ScanListMemory[SCANLIST_SIZE];

// Pointers to actual code addresses to patch. The first list
// has taken into account the instruction bytes, second list
// does not. The second list is guaranteed to have 3 byte
// instructions before the new address.

DWORD dwPatchAddrScanReloc1USA[14] = {
0x5DC7AA,0x41A85D,0x41A864,0x408259,0x711B32,0x699CF8,
0x4092EC,0x40914E,0x408702,0x564220,0x564172,0x563845,
0x84E9C2,0x85652D };

DWORD dwPatchAddrScanReloc1EU[14] = {
0x5DC7AA,0x41A85D,0x41A864,0x408261,0x711B32,0x699CF8,
0x4092EC,0x40914E,0x408702,0x564220,0x564172,0x563845,
0x84EA02,0x85656D };

// Lots of hex.. that's why they call us a "determined group of hackers"

DWORD dwPatchAddrScanReloc2USA[56] = {
0x0040D68C,0x005664D7,0x00566586,0x00408706,0x0056B3B1,0x0056AD91,0x0056A85F,0x005675FA,
0x0056CD84,0x0056CC79,0x0056CB51,0x0056CA4A,0x0056C664,0x0056C569,0x0056C445,0x0056C341,
0x0056BD46,0x0056BC53,0x0056BE56,0x0056A940,0x00567735,0x00546738,0x0054BB23,0x006E31AA,
0x0040DC29,0x00534A09,0x00534D6B,0x00564B59,0x00564DA9,0x0067FF5D,0x00568CB9,0x00568EFB,
0x00569F57,0x00569537,0x00569127,0x0056B4B5,0x0056B594,0x0056B2C3,0x0056AF74,0x0056AE95,
0x0056BF4F,0x0056ACA3,0x0056A766,0x0056A685,0x0070B9BA,0x0056479D,0x0070ACB2,0x006063C7,
0x00699CFE,0x0041A861,0x0040E061,0x0040DF5E,0x0040DDCE,0x0040DB0E,0x0040D98C,0x01566855 };

DWORD dwPatchAddrScanReloc2EU[56] = {
0x0040D68C,0x005664D7,0x00566586,0x00408706,0x0056B3B1,0x0056AD91,0x0056A85F,0x005675FA,
0x0056CD84,0x0056CC79,0x0056CB51,0x0056CA4A,0x0056C664,0x0056C569,0x0056C445,0x0056C341,
0x0056BD46,0x0056BC53,0x0056BE56,0x0056A940,0x00567735,0x00546738,0x0054BB23,0x006E31AA,
0x0040DC29,0x00534A09,0x00534D6B,0x00564B59,0x00564DA9,0x0067FF5D,0x00568CB9,0x00568EFB,
0x00569F57,0x00569537,0x00569127,0x0056B4B5,0x0056B594,0x0056B2C3,0x0056AF74,0x0056AE95,
0x0056BF4F,0x0056ACA3,0x0056A766,0x0056A685,0x0070B9BA,0x0056479D,0x0070ACB2,0x006063C7,
0x00699CFE,0x0041A861,0x0040E061,0x0040DF5E,0x0040DDCE,0x0040DB0E,0x0040D98C,0x01566845 };

DWORD dwPatchAddrScanReloc3[11] = {
0x004091C5,0x00409367,0x0040D9C5,0x0040DB47,0x0040DC61,0x0040DE07,0x0040DF97,
0x0040E09A,0x00534A98,0x00534DFA,0x0071CDB0 };

// For End
// 0xB992B8 is reffed for checking end of scanlist... rewrite this to point to end of new list
DWORD dwPatchAddrScanRelocEnd[4] = { 0x005634A6, 0x005638DF, 0x0056420F, 0x00564283 };

//-----------------------------------------------------------

void RelocateScanListHack()
{
	DWORD oldProt;
	memset(&ScanListMemory[0], 0, SCANLIST_SIZE);
	unsigned char *aScanListMemory = &ScanListMemory[0];

	// FIRST PREPARED LIST OF ACCESSORS
	int x=0;
	while(x!=14) {
		if(iGtaVersion == GTASA_VERSION_USA10) {
			VirtualProtect((PVOID)dwPatchAddrScanReloc1USA[x],4,PAGE_EXECUTE_READWRITE,&oldProt);
			*(PDWORD)dwPatchAddrScanReloc1USA[x] = (DWORD)aScanListMemory;
		}
		else if(iGtaVersion == GTASA_VERSION_EU10) {
			VirtualProtect((PVOID)dwPatchAddrScanReloc1EU[x],4,PAGE_EXECUTE_READWRITE,&oldProt);
			*(PDWORD)dwPatchAddrScanReloc1EU[x] = (DWORD)aScanListMemory;
		}
		x++;
	}

	// SECOND PREPARED LIST OF ACCESSORS <G>
	x=0;
	while(x!=56) {
		if(iGtaVersion == GTASA_VERSION_USA10) {
			VirtualProtect((PVOID)dwPatchAddrScanReloc2USA[x],8,PAGE_EXECUTE_READWRITE,&oldProt);
			*(PDWORD)(dwPatchAddrScanReloc2USA[x] + 3) = (DWORD)aScanListMemory;
		}
		else if(iGtaVersion == GTASA_VERSION_EU10) {
			VirtualProtect((PVOID)dwPatchAddrScanReloc2EU[x],8,PAGE_EXECUTE_READWRITE,&oldProt);
			*(PDWORD)(dwPatchAddrScanReloc2EU[x] + 3) = (DWORD)aScanListMemory;
		}
		x++;
	}

	// THIRD LIST THAT POINTS TO THE BASE SCANLIST MEMORY + 4
	x=0;
	while(x!=11) {
		VirtualProtect((PVOID)dwPatchAddrScanReloc3[x],8,PAGE_EXECUTE_READWRITE,&oldProt);
		*(PDWORD)(dwPatchAddrScanReloc3[x] + 3) = (DWORD)(aScanListMemory+4);
		x++;
	}

	// FOURTH LIST THAT POINTS TO THE END OF THE SCANLIST
	x=0;
	while(x!=4) {
		VirtualProtect((PVOID)dwPatchAddrScanRelocEnd[x],4,PAGE_EXECUTE_READWRITE,&oldProt);
		*(PDWORD)(dwPatchAddrScanRelocEnd[x]) = (DWORD)(aScanListMemory+sizeof(ScanListMemory));
		x++;
	}

	// Others that didn't fit.
	VirtualProtect((PVOID)0x564DC7,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)0x564DC7 = (DWORD)(aScanListMemory+115200);

	VirtualProtect((PVOID)0x40936A,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)0x40936A = (DWORD)(aScanListMemory+4);

	// Reset the exe scanlist mem for playerinfo's
	memset((BYTE*)0xB7D0B8,0,8*14400);
}

//----------------------------------------------------------
