
#include "main.h"

//----------------------------------------------------

CDeathWindow::CDeathWindow(IDirect3DDevice9 *pD3DDevice)
{
	// TODO: CDeathWindow::CDeathWindow
}

//----------------------------------------------------

void CDeathWindow::CreateAuxFonts()
{
	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_NORMAL, 1, FALSE,
		SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "SAMPAUX3", &field_14F);

	D3DXCreateFont(m_pD3DDevice, 22, 0, FW_NORMAL, 1, FALSE,
		SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "SAMPAUX3", &field_153);

	field_14B = TRUE;
}

void CDeathWindow::AddMessage(CHAR *a1, CHAR *a2, DWORD a3, DWORD a4, BYTE a5)
{
	// TODO: CDeathWindow::AddMessage .text:1006A6B0
}

//----------------------------------------------------

PCHAR CDeathWindow::SpriteIDForWeapon(BYTE byteWeaponID)
{
	switch (byteWeaponID) {
		case 0:
			return "%";
		case WEAPON_BRASSKNUCKLE:
			return "B";
		case WEAPON_GOLFCLUB:
			return ">";
		case WEAPON_NITESTICK:
			return "(";
		case WEAPON_KNIFE:
			return "C";
		case WEAPON_BAT:
			return "?";
		case WEAPON_SHOVEL:
			return "&";
		case WEAPON_POOLSTICK:
			return "\"";
		case WEAPON_KATANA:
			return "!";
		case WEAPON_CHAINSAW:
			return "1";
		case WEAPON_DILDO:
		case WEAPON_DILDO2:
		case WEAPON_VIBRATOR:
		case WEAPON_VIBRATOR2:
			return "E";
		case WEAPON_FLOWER:
			return "$";
		case WEAPON_CANE:
			return "#";
		case WEAPON_GRENADE:
			return "@";
		case WEAPON_TEARGAS:
			return "D";
		case WEAPON_COLT45:
			return "6";
		case WEAPON_SILENCED:
			return "2";
		case WEAPON_DEAGLE:
			return "3";
		case WEAPON_SHOTGUN:
			return "=";
		case WEAPON_SAWEDOFF:
			return "0";
		case WEAPON_SHOTGSPA:
			return "+";
		case WEAPON_UZI:
			return "I";
		case WEAPON_MP5:
			return "8";
		case WEAPON_AK47:
			return "H";
		case WEAPON_M4:
			return "5";
		case WEAPON_TEC9:
			return "7";
		case WEAPON_RIFLE:
			return ".";
		case WEAPON_SNIPER:
			return "A";
		case WEAPON_ROCKETLAUNCHER:
			return "4";
		case WEAPON_HEATSEEKER:
			return ")";
		case WEAPON_FLAMETHROWER:
			return "P";
		case WEAPON_MINIGUN:
			return "F";
		case WEAPON_SATCHEL:
			return "<";
		case WEAPON_BOMB:
			return ";";
		case WEAPON_SPRAYCAN:
			return "/";
		case WEAPON_FIREEXTINGUISHER:
			return ",";
		case WEAPON_PARACHUTE:
			return ":";
		case WEAPON_VEHICLE:
			return "L";
		case WEAPON_HELIBLADES:
			return "R";
		case WEAPON_EXPLOSION:
			return "Q";
		case WEAPON_COLLISION:
			return "K";
		case SPECIAL_ENTRY_CONNECT:
		case SPECIAL_ENTRY_DISCONNECT:
			return "N";
	}
	return "J";
}

//----------------------------------------------------
// EOF
