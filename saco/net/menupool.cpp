
#include "../main.h"

CHAR g_szMenuItems[MAX_MENU_ITEMS][MAX_COLUMNS][MAX_MENU_LINE+1] =
{
	{"SAMP000", "SAMP100"},
	{"SAMP001", "SAMP101"},
	{"SAMP002", "SAMP102"},
	{"SAMP003", "SAMP103"},
	{"SAMP004", "SAMP104"},
	{"SAMP005", "SAMP105"},
	{"SAMP006", "SAMP106"},
	{"SAMP007", "SAMP107"},
	{"SAMP008", "SAMP108"},
	{"SAMP009", "SAMP109"},
	{"SAMP010", "SAMP110"},
	{"SAMP011", "SAMP111"},
};

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

CMenuPool::~CMenuPool()
{
	if (m_byteCurrentMenu != MAX_MENUS) m_pMenus[m_byteCurrentMenu]->Hide();
	for (BYTE byteMenuID = 0; byteMenuID < MAX_MENUS; byteMenuID++)
	{
		if (m_pMenus[byteMenuID])
		{
			delete m_pMenus[byteMenuID];
			m_pMenus[byteMenuID] = NULL;
		}
	}
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

void CMenuPool::ShowMenu(BYTE byteMenuID)
{
	if (byteMenuID >= MAX_MENUS) return;
	if (m_bMenuSlotState[byteMenuID] == FALSE || !m_pMenus[byteMenuID]) return;

	if (m_byteCurrentMenu != MAX_MENUS) m_pMenus[m_byteCurrentMenu]->Hide();
	m_pMenus[byteMenuID]->Show();
	m_byteCurrentMenu = byteMenuID;

	m_byteExited = 0;
}

void CMenuPool::HideMenu(BYTE byteMenuID)
{
	if (byteMenuID >= MAX_MENUS || m_byteCurrentMenu == MAX_MENUS) return;
	if (m_bMenuSlotState[byteMenuID] == FALSE || !m_pMenus[byteMenuID]) return;
	m_pMenus[byteMenuID]->Hide();
	m_byteCurrentMenu = MAX_MENUS;
}

