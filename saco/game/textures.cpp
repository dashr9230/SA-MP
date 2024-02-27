
#include <windows.h>
#include "textures.h"

//-----------------------------------------------------------

int FindTextureSlot(char *szName)
{
	int iRetVal;

	_asm push szName
	_asm mov edx, 0x731850
	_asm call edx
	_asm pop edx
	_asm mov iRetVal, eax

	return iRetVal;
}

//-----------------------------------------------------------

int AddTextureSlot(char *szName)
{
	int iResult;

	_asm push szName
	_asm mov edx, 0x731C80
	_asm call edx
	_asm pop edx
	_asm mov iResult, eax

	return iResult;
}

//-----------------------------------------------------------

int LoadTexture(int iIndex, char *szName)
{
	BYTE byteResult = 0;

	_asm xor eax, eax
	_asm push szName
	_asm push iIndex
	_asm mov edx, 0x7320B0
	_asm call edx
	_asm mov byteResult, al
	_asm pop edx
	_asm pop edx

	return byteResult;
}

//-----------------------------------------------------------

void AddTextureRef(int iIndex)
{
	_asm push iIndex
	_asm mov edx, 0x731A00
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

void RemoveTextureRef(int iIndex)
{
	_asm push iIndex
	_asm mov edx, 0x731A30
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

void PushCurrentTexture()
{
	_asm mov edx, 0x7316A0
	_asm call edx
}

//-----------------------------------------------------------

void PopCurrentTexture()
{
	_asm mov edx, 0x7316B0
	_asm call edx
}

//-----------------------------------------------------------

void SetCurrentTexture(int iIndex)
{
	_asm push iIndex
	_asm mov edx, 0x7319C0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

DWORD ReloadTexture(char *szName)
{
	DWORD dwThisTexture = 0;

	_asm push szName
	_asm lea ecx, dwThisTexture
	_asm mov edx, 0x727270
	_asm call edx

	return dwThisTexture;
}

//-----------------------------------------------------------

void DestroyTexture(DWORD dwTexture)
{
	_asm mov eax, dwTexture
	_asm mov dwTexture, eax
	_asm lea ecx, dwTexture
	_asm mov edx, 0x7281E0
	_asm call edx
}

//-----------------------------------------------------------

void RemoveTextureSlotByName(char *szName)
{
	int iIndex = FindTextureSlot(szName);

	if(iIndex == -1) return;

	_asm push iIndex
	_asm mov edx, 0x731CD0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

void RemoveTexture(int iIndex)
{
	if(iIndex == -1) return;

	_asm push iIndex
	_asm mov edx, 0x731E90
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

void RemoveTextureSlot(int iIndex)
{
	if(iIndex == -1) return;

	_asm push iIndex
	_asm mov edx, 0x731CD0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

int GetTextureNumRefs(int iIndex)
{
	int iRetVal = -1;

	if(iIndex == -1) return -1;

	_asm push iIndex
	_asm mov edx, 0x731AA0
	_asm call edx
	_asm mov iRetVal, eax
	_asm pop edx

	return iRetVal;
}

//-----------------------------------------------------------
// EOF
