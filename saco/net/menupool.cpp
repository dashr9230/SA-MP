
#include "../main.h"
#include "../game/keystuff.h"

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

extern CNetGame* pNetGame;

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

PCHAR CMenuPool::GetTextPointer(PCHAR szName)
{
	if (m_byteCurrentMenu == MAX_MENUS) return NULL;
	CMenu* pMenu = m_pMenus[m_byteCurrentMenu];
	if (!pMenu) return NULL;

	if (!strcmp(szName, "HED")) return pMenu->GetMenuTitle();
	if (!strcmp(szName, "RW1")) return pMenu->GetMenuHeader(0);
	if (!strcmp(szName, "RW2")) return pMenu->GetMenuHeader(1);
	for (BYTE i = 0; i < MAX_MENU_ITEMS; i++)
	{
		if (!strcmp(szName, g_szMenuItems[i][0] + 4)) return pMenu->GetMenuItem(0, i);
		if (!strcmp(szName, g_szMenuItems[i][1] + 4)) return pMenu->GetMenuItem(1, i);
	}
	return NULL;
}

void CMenuPool::Process()
{
	if (m_byteCurrentMenu == MAX_MENUS) return;

	GTA_CONTROLSET * pControls = GameGetInternalKeys();
	RakClientInterface* pRak = pNetGame->GetRakClient();
	if (pControls->wKeys1[16] && !pControls->wKeys2[16]) // Selected an item
	{
		BYTE row = m_pMenus[m_byteCurrentMenu]->GetSelectedRow();
		if (row != 0xFF)
		{
			m_byteExited = 1;
			RakNet::BitStream bsSend;
			bsSend.Write(row);
			pRak->RPC(RPC_MenuSelect, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE);
		}
	}
	else if (pControls->wKeys1[15] && !pControls->wKeys2[15]) // Exited
	{
		m_byteExited = 1;
		pRak->RPC(RPC_MenuQuit, NULL, HIGH_PRIORITY, RELIABLE, 0, FALSE);
	}
	else if (m_byteExited)
	{
		// Delay clearing for one frame to allow SA to make the nice noise
		HideMenu(m_byteCurrentMenu);
		m_byteExited = 0;
	}
}
