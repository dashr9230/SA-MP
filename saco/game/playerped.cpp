
#include "../main.h"
#include "util.h"

//-----------------------------------------------------------
// Used for instancing the local player.

CPlayerPed::CPlayerPed()
{
	m_dwGTAId = 1; // 0x001
	m_pPed = GamePool_FindPlayerPed();
	m_pEntity = (ENTITY_TYPE *)GamePool_FindPlayerPed();

	m_bytePlayerNumber = 0;
	SetPlayerPedPtrRecord(m_bytePlayerNumber,(DWORD)m_pPed);
	ScriptCommand(&set_actor_weapon_droppable,m_dwGTAId,1);

	ScriptCommand(&set_actor_can_be_decapitated,m_dwGTAId,0);

	field_2A8 = 1;
	field_2AC = 1;
	field_2CD = 0;
	field_2B9 = 0;
	field_2F2 = 0;
	field_2D2 = 0;
	field_2DE = 0;
	field_2E2 = 0;
	field_48 = 0;
	m_bGoggleState = FALSE;
	field_2C1 = 0;
	field_2C5 = 0;
	field_2C9 = 0;
	field_2F6 = 0;
	field_2F7 = 0;
	field_2F8 = 0;

	int x=0;
	while(x!=10) {
		field_4C[x];
		field_27C[x] = 0;
		memset(&field_74[x], 0, sizeof(struc_97));
		x++;
	}
	field_2F9 = 0;
}

//-----------------------------------------------------------

WEAPON_SLOT_TYPE * CPlayerPed::GetCurrentWeaponSlot()
{
	if(m_pPed) {
		return &m_pPed->WeaponSlots[m_pPed->byteCurWeaponSlot];
	}
	return NULL;
}

//-----------------------------------------------------------

WEAPON_SLOT_TYPE * CPlayerPed::FindWeaponSlot(DWORD dwWeapon)
{
	if (m_pPed)
	{
		BYTE i;
		for (i = 0; i < 13; i++)
		{
			if (m_pPed->WeaponSlots[i].dwType == dwWeapon) return &m_pPed->WeaponSlots[i];
		}
	}
	return NULL;
}

//-----------------------------------------------------------

void CPlayerPed::StopGoggles()
{
	if (!m_pPed || !HasGoggles()) return;

	m_bGoggleState = FALSE;
	DWORD dwPedPointer = (DWORD)m_pPed;
	_asm mov ecx, dwPedPointer
	_asm mov eax, 0x5E6010
	_asm call eax
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasGoggles()
{
	if (!m_pPed) return FALSE;
	return (BOOL)(m_pPed->dwActiveVision != 0 || m_bGoggleState);
}

//-----------------------------------------------------------
