
#include "../main.h"

//----------------------------------------------------

CNetGame::CNetGame()
{

}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	// TODO: ~CNetGame (W: 00416D50 L: 080ACFDE)
	/*
	(*(void (__cdecl **)(int, _DWORD, _DWORD))(*(_DWORD *)this->field_0 + 12))(this->field_0, 0, 0);
	sub_80B1B18(this->field_0);
	sub_80B7942(this->field_0);
	sub_80712C8(this->field_0);
	if ( this->field_386 )
	{
		s = (void *)this->field_386;
		if ( s )
		{
			sub_80A9434(s);
			operator delete(s);
		}
		this->field_386 = 0;
	}
	if ( this->field_38A )
	{
		v4 = (void *)this->field_38A;
		if ( v4 )
		{
			sub_80B8160(v4);
			operator delete(v4);
		}
		this->field_38A = 0;
	}
	if ( this->field_4 )
	{
		v3 = (void *)this->field_4;
		if ( v3 )
		{
			sub_80B1FF8(v3);
			operator delete(v3);
		}
		this->field_4 = 0;
	}
	if ( this->field_8 )
	{
		v2 = (void *)this->field_8;
		if ( v2 )
		{
			sub_80B9470(v2);
			operator delete(v2);
		}
		this->field_8 = 0;
	}*/
}

//----------------------------------------------------

void CNetGame::Init(char *a2, int a3, char *a4, char *a5, char *a6)
{
	// TODO: CNetGame::Init (W: 00416490 L: 080AD126)

	/*
	strcpy(this->field_202, "San Andreas Multiplayer");
	strncpy(this->field_302, src, 0x80u);
	this->field_382 = a3;
	v6 = (void *)operator new(0x6Eu);
	sub_80A940C(v6);
	this->field_386 = (int)v6;
	v7 = (void *)operator new(0x19u);
	sub_80B8036(v7);
	this->field_38A = (int)v7;
	v8 = (void *)operator new(0xA04Bu);
	sub_80B1F4E(v8);
	this->field_4 = (int)v8;
	sub_80AFBA2(this->field_4, a4);
	v9 = (void *)operator new(0x1BD50u);
	sub_80B9408(v9);
	this->field_8 = (int)v9;
	this->field_0 = sub_8070E80();
	sub_8094DD0((unsigned __int16)a3);
	sub_80B1714(this->field_0);
	sub_80B7032(this->field_0);
	sprintf(byte_810A720, "npcmodes/%s.amx", a6);
	if ( !(unsigned __int8)sub_80A9448((void *)this->field_386, byte_810A720) )
		exit(1);
	(*(void (__cdecl **)(int, char *))(*(_DWORD *)this->field_0 + 20))(this->field_0, a5);
	(*(void (__cdecl **)(int, char *, _DWORD, _DWORD, _DWORD, signed int))(*(_DWORD *)this->field_0 + 8))(
		this->field_0,
		src,
		(unsigned __int16)a3,
		0,
		0,
		10);
	this->field_C = 1;
	this->field_14 = 0;
	this->field_1E = 12;
	this->field_1F = 0;
	this->field_20 = 10;
	this->field_34 = 1006834287;
	this->field_38 = 0;
	this->field_10 = 0;
	this->field_3C = 1;
	this->field_40 = 0;
	for ( i = 0; i <= 99; ++i )
		this->field_4A[i] = 0;
	this->field_31 = 1;
	this->field_3E = 0;
	this->field_3D = 0;
	memset(&word_80E1600, 0, 0x44u);
	memset(&word_80E1660, 0, 0x3Fu);
	memset(&word_80E16C0, 0, 0x109A0u);
	memset(&word_80F2060, 0, 0xF618u);
	memset(&word_8101680, 0, 0x5DC0u);
	memset(&dword_8107440, 0, 0xFA0u);
	memset(&dword_81083E0, 0, 0x1F40u);
	memset(&byte_810A320, 0, 0x3E8u);
	this->field_1DE = 0;
	this->field_1E2 = 0;
	this->field_1F2 = sub_80AC0BE();
	byte_80E16B7 = 0;
	this->field_1FA = -1;
	this->field_1FE = -1;
	srand(0);
	*/
}

//----------------------------------------------------

//----------------------------------------------------

void CNetGame::Process()
{
	// TODO: CNetGame::Process (W: 00418370 L: 080AD6A4)
	/*
	v2 = sub_80AC130();
	sub_80AE6A4(this);
	if ( this->field_C == 2 )
	{
		if ( this->field_386 )
			sub_80A969E(this->field_386, v2);
		if ( this->field_38A )
			sub_80B8610(this->field_38A, (signed __int64)(v2 * 1000.0));
		if ( byte_810A708 )
		{
			if ( this->field_1DE )
			{
				sub_80ADFDE(this);
			}
			else
			{
				sub_80AD770(this);
				byte_80E16B7 = 1;
			}
		}
	}*/
}

//----------------------------------------------------
