
#include <windows.h>
#include "font.h"

#define NUDE void _declspec(naked)

//----------------------------------------------------------

void Font_PrintString(float X, float Y, char *sz)
{
	_asm push sz
	_asm push Y
	_asm push X
	_asm mov eax, 0x71A700
	_asm call eax
	_asm pop eax
	_asm pop eax
	_asm pop eax
}

//----------------------------------------------------------

NUDE Font_SetColor(DWORD dwColor)
{
	_asm mov eax, 0x719430
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetDropColor(DWORD dwColor)
{
	_asm mov eax, 0x719510
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetOutline(int pos)
{
	_asm mov eax, 0x719590
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_Unk12(int unk)
{
	_asm mov eax, 0x719600
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetScale(float X, float Y)
{
	_asm mov eax, 0x719380
    _asm jmp eax
}

//-----------------------------------------------------------
// 0 center 1 left 2 right

NUDE Font_SetJustify(int just)
{
	_asm mov eax, 0x719610
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetFontStyle(int style)
{
	_asm mov eax, 0x719490
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetProportional(int prop)
{
	_asm mov eax, 0x7195B0
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetRightJustifyWrap(float wrap)
{
	_asm mov eax, 0x7194F0
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_UseBox(int use, int unk)
{
	_asm mov eax, 0x7195C0
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_UseBoxColor(DWORD color)
{
	_asm mov eax, 0x7195E0
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetLineWidth(float width)
{
	_asm mov eax, 0x7194D0
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetLineHeight(float height)
{
	_asm mov eax, 0x7194E0
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_SetShadow(int shadow)
{
	_asm mov eax, 0x719570
	_asm jmp eax
}

//-----------------------------------------------------------

NUDE Font_UnkConv(char *sz, int param1, int param2, int param3, int param4, int param5, int param6, char * buf)
{
	_asm mov eax, 0x69DE90
	_asm ret
}

//-----------------------------------------------------------

NUDE Font_UnkConv2(char *sz)
{
	_asm mov eax, 0x69E160
	_asm jmp eax
}

//-----------------------------------------------------------
