
#pragma once

//----------------------------------------------------

class CMenuPool
{
private:
	char field_401;

	CMenu *m_pMenus[MAX_MENUS];
	BOOL m_bMenuSlotState[MAX_MENUS];
	BYTE m_byteCurrentMenu;

public:
	CMenuPool();

	CMenu* New(BYTE byteMenuID, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction);

	BOOL Delete(BYTE byteMenuID);

};

//----------------------------------------------------
