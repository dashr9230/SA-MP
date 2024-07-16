
#include "../main.h"


CMenu::CMenu(float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction)
{
	for (int i = 0; i < MAX_MENU_ITEMS; i++)
	{
		memset(m_charItems[i][0], 0, sizeof(m_charItems[i][0]));
		memset(m_charItems[i][1], 0, sizeof(m_charItems[i][1]));
	}
	memset(m_charHeader[0], 0, sizeof(m_charHeader[0]));
	memset(m_charHeader[1], 0, sizeof(m_charHeader[1]));
	memset(m_charTitle, 0, sizeof(m_charTitle));

	m_fXPos = fX;
	m_fYPos = fY;
	m_fCol1Width = fCol1Width;
	m_fCol2Width = fCol2Width;

	if (byteColumns == 2) m_byteColumns = 2;
	else m_byteColumns = 1;
	memcpy(&m_MenuInteraction, MenuInteraction, sizeof (MENU_INT));

	m_dwPanel = 0;
}

void CMenu::Hide()
{
	ScriptCommand(&remove_panel, m_dwPanel);
}

