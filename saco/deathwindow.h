
#pragma once

#define MAX_DISP_DEATH_MESSAGES	5

#pragma pack(1)
typedef struct _DEATH_WINDOW_ENTRY
{
	char _gap0[59];

} DEATH_WINDOW_ENTRY;

#define SPECIAL_ENTRY_CONNECT 200
#define SPECIAL_ENTRY_DISCONNECT 201

//----------------------------------------------------

class CDeathWindow
{
private:
	char _gap0[4];
	DEATH_WINDOW_ENTRY	m_DeathWindowEntries[MAX_DISP_DEATH_MESSAGES];
	char _gap012B[24];
	BOOL				field_14B;
	ID3DXFont			*field_14F;
	ID3DXFont			*field_153;
	
	void PushBack();

public:
	void AddMessage(CHAR *a1, CHAR *a2, DWORD a3, DWORD a4, BYTE a5);

	PCHAR SpriteIDForWeapon(BYTE byteWeaponID);

	CDeathWindow(IDirect3DDevice9 *pD3DDevice);

	void CreateAuxFonts();

};

//----------------------------------------------------
