
#pragma once

#define MAX_MENU_ITEMS 12
#define MAX_MENU_LINE 32
#define MAX_COLUMNS 2

//----------------------------------------------------

struct MENU_INT
{
	BOOL bMenu;
	BOOL bRow[MAX_MENU_ITEMS];
	BOOL bPadding[8 - ((MAX_MENU_ITEMS + 1) % 8)]; 
};

class CMenu
{
private:

	//char _gap0[979];
	BYTE m_byteMenuID;

	CHAR m_charTitle[MAX_MENU_LINE+1];
	CHAR m_charItems[MAX_MENU_ITEMS][MAX_COLUMNS][MAX_MENU_LINE+1];
	CHAR m_charHeader[MAX_COLUMNS][MAX_MENU_LINE+1];

	float m_fXPos;
	float m_fYPos;
	float m_fCol1Width;
	float m_fCol2Width;
	BYTE m_byteColumns;
	MENU_INT m_MenuInteraction;

	BYTE m_byteColCount[MAX_COLUMNS];

	DWORD m_dwPanel;

public:

	CMenu(float fX, float fY, BYTE byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction);

	void Show();
	void Hide();
	PCHAR MS(BYTE byteColumn, BYTE byteRow);
};
