
#include "../main.h"

extern int iGtaVersion;

extern DWORD dwGraphicsLoop; // Used for the external dll game loop.

#define NUDE void _declspec(naked) 

BYTE	*pbyteCameraMode = (BYTE *)0xB6F1A8;
BYTE	*pbyteCurrentPlayer = (BYTE *)0xB7CD74;

float fFarClip=1400.0f;

DWORD unnamed_101516D4;

WORD wLastRendObj=0;

BYTE Unk1_JmpCode[] = {0xFF,25,0xD1,0xBE,53,0x00};
BYTE HOOK_24_JmpCode[] = {0xFF,0x25,0xBB,0x19,0x69,0x00,0x90};
BYTE HOOK_25_JmpCode[] = {0xFF,0x25,0xBA,0xB8,0x63,0x00,0x90};
BYTE HOOK_27_JmpCode[] = {0xFF,0x25,0x79,0x4A,0x58,0x00,0x90};
BYTE HOOK_26_JmpCode[] = {0xFF,0x25,0xAA,0x85,0x43,0x00,0x90};
BYTE Unk2_JmpCode[] = {0xFF,0x25,0x33,0x14,0x42,0x00};
BYTE Unk3_JmpCode[] = {0xFF,0x25,0x61,0x36,0x53,0x00,0x90,0x90,0x90};
BYTE HOOK_28_JmpCode[] = {0xFF,0x25,0xF1,0xC8,0x53,0x00,0x90};
BYTE HOOK_29_JmpCode[] = {0xFF,0x25,0xBC,0x5A,0x4B,0x00};
BYTE Unk4_JmpCode[] = {0xFF,0x25,0x74,0x22,0x50,0x00,0x90,0x90,0x90,0x90};
BYTE Unk5_JmpCode[] = {0xFF,0x25,0x61,0x38,0x4C,0x00};
BYTE HOOK_34_JmpCode[] = {0xFF,0x25,0x43,0x00,0x6A,0x00,0x90,0x90,0x90};
BYTE Unk6_JmpCode[] = {0xFF,0x25,0xD8,0xFF,0x5E,0x00,0x90};
BYTE HOOK_32_JmpCode[] = {0xFF,0x25,0x1B,0x8B,0x73,0x00};
BYTE HOOK_33_JmpCode[] = {0xFF,0x25,0x5B,0x88,0x73,0x00};
BYTE Unk7_JmpCode[] = {0xFF,0x25,0x36,0xA0,0x63,0x00,0x90};
BYTE Unk8_JmpCode[] = {0xFF,0x25,0x77,0xAB,0x5E,0x00,0x90};
BYTE Unk9_JmpCode[] = {0xFF,0x25,0x39,0x88,0x4C,0x00,0x90,0x90};
BYTE HOOK_7_JmpCode[] = {0xFF,0x25,0x34,0x39,0x4D,0x00,0x90,0x90,0x90,0x90};
BYTE HOOK_8_JmpCode[] = {0xFF,0x25,0x09,0x46,0x4D,0x00,0x90};
BYTE Unk10_JmpCode[] = {0xFF,0x25,0xE5,0x42,0x4D,0x00,0x90};
BYTE Unk11_JmpCode[] = {0xFF,0x25,0x29,0x3B,0x4D,0x00};
BYTE HOOK_36_JmpCode[] = {0xFF,0x25,0x21,0xC7,0x59,0x00};
BYTE HOOK_40_JmpCode[] = {0xFF,0x25,0x84,0x80,0x53,0x00,0x90};
BYTE HOOK_48_JmpCode[] = {0xFF,0x25,0xA6,0x34,0x55,0x00,0x90,0x90,0x90};
BYTE HOOK_50_JmpCode[] = {0xFF,0x25,0x33,0x34,0x4B,0x00};
BYTE Unk12_JmpCode[] = {0xFF,0x25,0xC6,0x84,0x6D,0x00,0x90,0x90};
BYTE HOOK_61_JmpCode[] = {0xFF,0x25,0xD7,0x87,0x40,0x00};
BYTE HOOK_60_JmpCode[] = {0xFF,0x25,0xDB,0x74,0x56,0x00};

//-----------------------------------------------------------


// TODO: HOOK_*() functions
NUDE HOOK_1() {}
NUDE HOOK_2() {}
NUDE HOOK_3() {}
NUDE HOOK_4() {}
NUDE HOOK_5() {}
NUDE HOOK_7() {}
NUDE HOOK_8() {}
NUDE HOOK_9() {}
NUDE HOOK_10() {}
NUDE HOOK_11() {}
NUDE HOOK_12() {}
NUDE HOOK_13() {}
NUDE HOOK_14() {}
NUDE HOOK_15() {}
NUDE HOOK_16() {}
NUDE HOOK_17() {}
NUDE HOOK_18() {}
NUDE HOOK_19() {}
NUDE HOOK_20() {}
NUDE HOOK_21() {}
NUDE HOOK_22() {}
NUDE HOOK_23() {}
NUDE HOOK_24() {}
NUDE HOOK_25() {}
NUDE HOOK_26() {}
NUDE HOOK_27() {}
NUDE HOOK_28() {}
NUDE HOOK_29() {}
NUDE HOOK_30() {}
NUDE HOOK_31() {}
NUDE HOOK_32() {}
NUDE HOOK_33() {}
NUDE HOOK_34() {}
NUDE HOOK_35() {}
NUDE HOOK_36() {}
NUDE HOOK_37() {}
NUDE HOOK_38() {}
NUDE HOOK_39() {}
NUDE HOOK_40() {}
NUDE HOOK_41() {}
NUDE HOOK_42() {}
NUDE HOOK_43() {}
NUDE HOOK_44() {}
NUDE HOOK_45() {}
NUDE HOOK_46() {}
NUDE HOOK_47() {}
NUDE HOOK_48() {}
NUDE HOOK_49() {}
NUDE HOOK_50() {}
NUDE HOOK_51() {}
NUDE HOOK_52() {}
NUDE HOOK_53() {}
NUDE HOOK_54() {}
NUDE HOOK_55() {}
NUDE HOOK_56() {}
NUDE HOOK_57() {}
NUDE HOOK_58() {}
NUDE HOOK_59() {}
NUDE HOOK_60() {}
NUDE HOOK_61() {}
NUDE HOOK_62() {}
NUDE HOOK_63() {}
NUDE HOOK_64() {}
NUDE HOOK_65() {}
NUDE HOOK_66() {}



NUDE HOOK_6()
{
	Sleep(1);

	_asm mov edx, 0x561A80
	_asm jmp edx
}


//-----------------------------------------------------------

DWORD dwRandCaller;

NUDE Rand_Hook()
{
	_asm mov eax, [esp+0]
	_asm mov dwRandCaller, eax

	/*
	if(dwRandCaller > 0x73FB10 && dwRandCaller < 0x74132E) {
		_asm mov eax, iSyncedRandomNumber
		_asm ret
	}*/

	rand();
	_asm ret
}

//-----------------------------------------------------------

void InstallMethodHook(	DWORD dwInstallAddress,
						DWORD dwHookFunction )
{
	DWORD oldProt, oldProt2;
	VirtualProtect((LPVOID)dwInstallAddress,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwInstallAddress = (DWORD)dwHookFunction;
	VirtualProtect((LPVOID)dwInstallAddress,4,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallHook( DWORD dwInstallAddress,
				  DWORD dwHookFunction,
				  DWORD dwHookStorage,
				  BYTE * pbyteJmpCode,
				  int iJmpCodeSize )
{
	DWORD oldProt, oldProt2;

	// Install the pointer to procaddr.
	VirtualProtect((PVOID)dwHookStorage,4,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;
	VirtualProtect((PVOID)dwHookStorage,4,oldProt,&oldProt2);

	// Install the Jmp code.
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,PAGE_EXECUTE_READWRITE,&oldProt);
	memcpy((PVOID)dwInstallAddress,pbyteJmpCode,iJmpCodeSize);
	VirtualProtect((PVOID)dwInstallAddress,iJmpCodeSize,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction, BYTE byteJumpCode = 0xE8)
{
	DWORD oldProt, oldProt2;
	DWORD disp = dwHookFunction - (dwInstallAddress + 5);

	VirtualProtect((LPVOID)dwInstallAddress,5,PAGE_EXECUTE_READWRITE,&oldProt);
	*(PBYTE)(dwInstallAddress) = byteJumpCode;
	*(PDWORD)(dwInstallAddress+1) = (DWORD)disp;
	VirtualProtect((LPVOID)dwInstallAddress,5,oldProt,&oldProt2);
}

//-----------------------------------------------------------

void InstallGameAndGraphicsLoopHooks()
{
	UnFuck(0x53EB13,4);
	*(int *)0x53EB13 = dwGraphicsLoop - 0x53EB12 - 5; // relative addr

	InstallCallHook(0x58FC53,(DWORD)HOOK_1);

	InstallCallHook(0x58FBBF,(DWORD)HOOK_2);

	InstallCallHook(0x53C104,(DWORD)HOOK_3);

	InstallCallHook(0x53E981,(DWORD)HOOK_4);

	InstallMethodHook(0x86D1B0,(DWORD)HOOK_5);
	InstallMethodHook(0x86C0F0,(DWORD)HOOK_5);
	InstallMethodHook(0x86C168,(DWORD)HOOK_5);
	InstallMethodHook(0x86C248,(DWORD)HOOK_5);
	InstallMethodHook(0x86C3A0,(DWORD)HOOK_5);

	InstallCallHook(0x53E930,(DWORD)HOOK_6);
}

//-----------------------------------------------------------

void GameInstallHooks()
{
	InstallGameAndGraphicsLoopHooks();

	InstallHook(0x4D3AA0,(DWORD)HOOK_7,0x4D3934,HOOK_7_JmpCode,sizeof(HOOK_7_JmpCode));

	InstallHook(0x4D4610,(DWORD)HOOK_8,0x4D4609,HOOK_8_JmpCode,sizeof(HOOK_8_JmpCode));

	InstallMethodHook(0x86D190,(DWORD)HOOK_9);

	InstallMethodHook(0x86C0D0,(DWORD)HOOK_10);

	InstallMethodHook(0x86D744,(DWORD)HOOK_11);

	InstallCallHook(0x7330A2,(DWORD)HOOK_12);

	InstallMethodHook(0x86D194,(DWORD)HOOK_13);

	InstallCallHook(0x5689FD,(DWORD)HOOK_14);

	InstallCallHook(0x53EA03,(DWORD)HOOK_15);

	InstallMethodHook(0x871148,(DWORD)HOOK_16);
	InstallMethodHook(0x8721C8,(DWORD)HOOK_16);
	InstallMethodHook(0x871388,(DWORD)HOOK_16);
	InstallMethodHook(0x871970,(DWORD)HOOK_16);
	InstallMethodHook(0x8716A8,(DWORD)HOOK_16);
	InstallMethodHook(0x871550,(DWORD)HOOK_16);
	InstallMethodHook(0x871800,(DWORD)HOOK_16);
	InstallMethodHook(0x871B10,(DWORD)HOOK_16);
	InstallMethodHook(0x872398,(DWORD)HOOK_16);
	InstallMethodHook(0x871C50,(DWORD)HOOK_16);

	InstallCallHook(0x501B1D,(DWORD)HOOK_17);
	InstallCallHook(0x501B42,(DWORD)HOOK_17);
	InstallCallHook(0x501FC2,(DWORD)HOOK_17);
	InstallCallHook(0x502067,(DWORD)HOOK_17);
	InstallCallHook(0x5021AE,(DWORD)HOOK_17);

	InstallCallHook(0x5869BF,(DWORD)HOOK_18);
	InstallCallHook(0x5759E4,(DWORD)HOOK_18);

	InstallCallHook(0x609A4E,(DWORD)HOOK_19);

	InstallCallHook(0x4579C6,(DWORD)HOOK_20,0xE9);

	InstallCallHook(0x73C252,(DWORD)HOOK_21,0xE9);

	InstallCallHook(0x73ACE2,(DWORD)HOOK_22);

	InstallCallHook(0x6F8CF8,(DWORD)HOOK_23);

	InstallHook(0x6402F0,(DWORD)HOOK_24,0x6919BB,HOOK_24_JmpCode,sizeof(HOOK_24_JmpCode));
	InstallHook(0x63B8C0,(DWORD)HOOK_25,0x63B8BA,HOOK_25_JmpCode,sizeof(HOOK_25_JmpCode));
	InstallHook(0x438576,(DWORD)HOOK_26,0x4385AA,HOOK_26_JmpCode,sizeof(HOOK_26_JmpCode));
	InstallHook(0x584770,(DWORD)HOOK_27,0x584A79,HOOK_27_JmpCode,sizeof(HOOK_27_JmpCode));
	InstallHook(0x53C900,(DWORD)HOOK_28,0x53C8F1,HOOK_28_JmpCode,sizeof(HOOK_28_JmpCode));
	InstallHook(0x4B5AC0,(DWORD)HOOK_29,0x4B5ABC,HOOK_29_JmpCode,sizeof(HOOK_29_JmpCode));

	InstallCallHook(0x4D41C0,(DWORD)HOOK_30,0xE9);

	InstallCallHook(0x4E7427,(DWORD)HOOK_31);

	InstallHook(0x738F3A,(DWORD)HOOK_32,0x738B1B,HOOK_32_JmpCode,sizeof(HOOK_32_JmpCode));
	InstallHook(0x738877,(DWORD)HOOK_33,0x73885B,HOOK_33_JmpCode,sizeof(HOOK_33_JmpCode));
	InstallHook(0x6A0050,(DWORD)HOOK_34,0x6A0043,HOOK_34_JmpCode,sizeof(HOOK_34_JmpCode));

	InstallCallHook(0x6FDED6,(DWORD)HOOK_35);

	if(iGtaVersion == GTASA_VERSION_USA10)
	{
		InstallHook(0x7FB020,(DWORD)HOOK_36,0x59C721,HOOK_36_JmpCode,sizeof(HOOK_36_JmpCode));
		unnamed_101516D4 = 0x7FB026;
	}
	else
	{
		InstallHook(0x7FB060,(DWORD)HOOK_36,0x59C721,HOOK_36_JmpCode,sizeof(HOOK_36_JmpCode));
		unnamed_101516D4 = 0x7FB066;
	}

	InstallCallHook(0x6D0E7E,(DWORD)HOOK_37);

	InstallMethodHook(0x866FA8,(DWORD)HOOK_38);

	InstallCallHook(0x586C0A,(DWORD)HOOK_39);

	InstallHook(0x538090,(DWORD)HOOK_40,0x538084,HOOK_40_JmpCode,sizeof(HOOK_40_JmpCode));

	InstallCallHook(0x718599,(DWORD)HOOK_41);

	InstallMethodHook(0x866F7C,(DWORD)HOOK_42);

	InstallMethodHook(0x866F80,(DWORD)HOOK_43);
	InstallMethodHook(0x8585E8,(DWORD)HOOK_43);

	InstallMethodHook(0x871218,(DWORD)HOOK_44);
	InstallMethodHook(0x871778,(DWORD)HOOK_44);
	InstallMethodHook(0x8718D0,(DWORD)HOOK_44);
	InstallMethodHook(0x871A40,(DWORD)HOOK_44);
	InstallMethodHook(0x871BE0,(DWORD)HOOK_44);

	InstallCallHook(0x5648D3,(DWORD)HOOK_45);

	InstallCallHook(0x53DFDD,(DWORD)HOOK_46);

	InstallCallHook(0x53E019,(DWORD)HOOK_47);

	InstallHook(0x5534B0,(DWORD)HOOK_48,0x5534A6,HOOK_48_JmpCode,sizeof(HOOK_48_JmpCode));

	InstallCallHook(0x5342F9,(DWORD)HOOK_49);

	InstallHook(0x4B35A0,(DWORD)HOOK_50,0x4B3433,HOOK_50_JmpCode,sizeof(HOOK_50_JmpCode));

	InstallCallHook(0x41B02E,(DWORD)HOOK_51);

	InstallCallHook(0x41AF80,(DWORD)HOOK_52);

	InstallCallHook(0x41AB78,(DWORD)HOOK_53);

	InstallMethodHook(0x871178,(DWORD)HOOK_54);
	InstallMethodHook(0x8716D8,(DWORD)HOOK_54);
	InstallMethodHook(0x8719A0,(DWORD)HOOK_54);
	InstallMethodHook(0x871B40,(DWORD)HOOK_54);

	InstallMethodHook(0x8713B8,(DWORD)HOOK_55);
	InstallMethodHook(0x871580,(DWORD)HOOK_55);

	InstallMethodHook(0x871830,(DWORD)HOOK_56);

	InstallMethodHook(0x8721F8,(DWORD)HOOK_57);

	InstallCallHook(0x6E0954,(DWORD)HOOK_58);
	InstallCallHook(0x6B2BCB,(DWORD)HOOK_58);
	InstallCallHook(0x4F77DA,(DWORD)HOOK_58);

	InstallMethodHook(0x872A74,(DWORD)HOOK_59);

	InstallHook(0x5674E0,(DWORD)HOOK_60,0x5674DB,HOOK_60_JmpCode,sizeof(HOOK_60_JmpCode));
}

//-----------------------------------------------------------

void unnamed_100A6FF0()
{
	InstallHook(0x4087EA,(DWORD)HOOK_61,0x4087D7,HOOK_61_JmpCode,sizeof(HOOK_61_JmpCode));
}

//-----------------------------------------------------------

void unnamed_100A7010()
{
	InstallCallHook(0x742495,(DWORD)HOOK_62);
	InstallCallHook(0x7424EC,(DWORD)HOOK_62);
	InstallCallHook(0x742548,(DWORD)HOOK_62);

	InstallCallHook(0x742548,(DWORD)HOOK_63);
	InstallCallHook(0x740B69,(DWORD)HOOK_63);
	InstallCallHook(0x736247,(DWORD)HOOK_63);

	InstallCallHook(0x7424CB,(DWORD)HOOK_64);

	InstallCallHook(0x73AC4B,(DWORD)HOOK_65);
}

//-----------------------------------------------------------

void unnamed_100A71C0()
{
	InstallCallHook(0x6D7C90,(DWORD)HOOK_66,0xE9);
}

//-----------------------------------------------------------
