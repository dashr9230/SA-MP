
#pragma once

#define SPECIAL_ENTRY_CONNECT 200
#define SPECIAL_ENTRY_DISCONNECT 201

//----------------------------------------------------

class CDeathWindow
{
private:
	char _gap0[343];

public:
	void AddMessage(CHAR *a1, CHAR *a2, DWORD a3, DWORD a4, BYTE a5);

	PCHAR SpriteIDForWeapon(BYTE byteWeaponID);

	CDeathWindow(IDirect3DDevice9 *pD3DDevice);
};

//----------------------------------------------------
