
#pragma once

//----------------------------------------------------

class CMenuPool
{
private:

	CMenu *m_pMenus[MAX_MENUS];
	BOOL m_bMenuSlotState[MAX_MENUS];
	BYTE m_byteCurrentMenu;
	BYTE m_byteExited;

public:
	CMenuPool();
	~CMenuPool();

	CMenu* New(BYTE byteMenuID, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction);

	BOOL Delete(BYTE byteMenuID);

	void ShowMenu(BYTE byteMenuID);
};

//----------------------------------------------------
