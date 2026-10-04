
#pragma once

//----------------------------------------------------

class CMenuPool
{
public:

	CMenu *m_pMenus[MAX_MENUS];
	BOOL m_bMenuSlotState[MAX_MENUS];
	BYTE m_byteCurrentMenu;
	BYTE m_byteExited;

public:
	int GetSlotState(BYTE byteMenuID) { if(byteMenuID > MAX_MENUS) return 0; return m_bMenuSlotState[byteMenuID]; }

	CMenuPool();
	~CMenuPool();

	CMenu* New(BYTE byteMenuID, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction);

	BOOL Delete(BYTE byteMenuID);

	void ShowMenu(BYTE byteMenuID);
	void HideMenu(BYTE byteMenuID);

	PCHAR GetTextPointer(PCHAR szName);

	void Process();
};

//----------------------------------------------------
