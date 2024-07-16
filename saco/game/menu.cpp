
#include "../main.h"

extern CHAR g_szMenuItems[MAX_MENU_ITEMS][MAX_COLUMNS][MAX_MENU_LINE+1];

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

void CMenu::SetTitle(PCHAR pTitle)
{
	if(strlen(pTitle) > MAX_MENU_LINE) return;

	memset(m_charTitle, 0, sizeof(m_charTitle));
	strcpy(m_charTitle, pTitle);
}

void CMenu::AddMenuItem(BYTE byteColumn, BYTE byteRow, PCHAR pText)
{
	if(strlen(pText) > MAX_MENU_LINE) return;
	if(byteColumn >= MAX_COLUMNS) return;
	if(byteRow >= MAX_MENU_ITEMS) return;

	memset(m_charItems[byteRow][byteColumn], 0, sizeof(m_charItems[byteRow][byteColumn]));
	strcpy(m_charItems[byteRow][byteColumn], pText);
}

void CMenu::SetColumnTitle(BYTE byteColumn, PCHAR pText)
{
	if(strlen(pText) > MAX_MENU_LINE) return;
	if(byteColumn >= MAX_COLUMNS) return;

	memset(m_charHeader[byteColumn], 0, sizeof(m_charHeader[byteColumn]));
	strcpy(m_charHeader[byteColumn], pText);

}

void CMenu::Show()
{
	ScriptCommand(&create_panel, m_charTitle[0] ? "SAMPHED" : "DUMMY", m_fXPos, m_fYPos, (m_fCol1Width + m_fCol2Width) / m_byteColumns, m_byteColumns, m_MenuInteraction.bMenu, 1, 1, &m_dwPanel);
	ScriptCommand(&set_panel_column_data, m_dwPanel, 0, m_charHeader[0][0] ? "SAMPRW1" : "DUMMY"
					, MS(0,0), MS(1,0), MS(2,0)
					, MS(3,0), MS(4,0), MS(5,0)
					, MS(6,0), MS(7,0), MS(8,0)
					, MS(9,0), MS(10,0), MS(11,0));
	ScriptCommand(&set_panel_column_width, m_dwPanel, 0, (int)m_fCol1Width);
	if (m_byteColumns == 2)
	{
		ScriptCommand(&set_panel_column_data, m_dwPanel, 1, m_charHeader[1][0] ? "SAMPRW2" : "DUMMY"
					, MS(0,1), MS(1,1), MS(2,1)
					, MS(3,1), MS(4,1), MS(5,1)
					, MS(6,1), MS(7,1), MS(8,1)
					, MS(9,1), MS(10,1), MS(11,1));
		ScriptCommand(&set_panel_column_width, m_dwPanel, 1, (int)m_fCol1Width);
	}
	for (BYTE i = 0; i < MAX_MENU_ITEMS; i++)
	{
		ScriptCommand(&set_panel_row_enable, m_dwPanel, i, m_MenuInteraction.bRow[i]);
	}
}

void CMenu::Hide()
{
	ScriptCommand(&remove_panel, m_dwPanel);
}

PCHAR CMenu::GetMenuItem(BYTE byteColumn, BYTE byteRow)
{
	return m_charItems[byteRow][byteColumn];
}

PCHAR CMenu::GetMenuTitle()
{
	return m_charTitle;
}

PCHAR CMenu::GetMenuHeader(BYTE byteColumn)
{
	return m_charHeader[byteColumn];
}

PCHAR CMenu::MS(BYTE byteRow, BYTE byteColumn)
{
	if (m_charItems[byteRow][byteColumn][0]) return g_szMenuItems[byteRow][byteColumn];
	return "DUMMY";
}

BYTE CMenu::GetSelectedRow()
{
	if (!m_MenuInteraction.bMenu) return 0xFF;
	DWORD dwRetVal;
	ScriptCommand(&get_panel_active_row, m_dwPanel, &dwRetVal);
	return (BYTE)dwRetVal;
}
