
#include "../main.h"

//----------------------------------------------------

CMenuPool::CMenuPool()
{
	// loop through and initialize all net players to null and slot states to false
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		m_bMenuSlotState[byteMenuID] = FALSE;
		m_pMenus[byteMenuID] = NULL;
	}
	m_byteCurrentMenu = MAX_MENUS;
}

//----------------------------------------------------

CMenu* CMenuPool::New(BYTE byteMenuID, float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction)
{
	SAFE_DELETE(m_pMenus[byteMenuID]);
	m_bMenuSlotState[byteMenuID] = FALSE;
	CMenu* pMenu = new CMenu(fX, fY, byteColumns, fCol1Width, fCol2Width, MenuInteraction);

	if (pMenu)
	{
		m_bMenuSlotState[byteMenuID] = TRUE;
		m_pMenus[byteMenuID] = pMenu;
		return pMenu;
	}
	return NULL;
}

//----------------------------------------------------

BOOL CMenuPool::Delete(BYTE byteMenuID)
{
	if (m_bMenuSlotState[byteMenuID] == FALSE || !m_pMenus[byteMenuID])
	{
		return FALSE;
	}

	m_bMenuSlotState[byteMenuID] = FALSE;
	delete m_pMenus[byteMenuID];
	m_pMenus[byteMenuID] = NULL;

	return TRUE;
}

//----------------------------------------------------

