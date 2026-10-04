
#include "main.h"

extern CGame* pGame;
extern CNetGame *pNetGame;

// retail walks these rows with calloc(n, 45); lock the layout so a field edit cannot drift it
typedef char SCOREBOARD_ROW_size_probe[sizeof(SCOREBOARD_ROW) == 45 ? 1 : -1];
typedef char SCOREBOARD_ROW_name_probe[offsetof(SCOREBOARD_ROW, szName) == 4 ? 1 : -1];
typedef char SCOREBOARD_ROW_id_probe[offsetof(SCOREBOARD_ROW, iPlayerId) == 0x21 ? 1 : -1];
typedef char SCOREBOARD_ROW_score_probe[offsetof(SCOREBOARD_ROW, iScore) == 0x25 ? 1 : -1];
typedef char SCOREBOARD_ROW_ping_probe[offsetof(SCOREBOARD_ROW, iPing) == 0x29 ? 1 : -1];

CScoreBoard::CScoreBoard(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pDialog = NULL;
	m_pListBox = NULL;
	field_3C = 0;
	field_40 = 0;
	field_4 = 0;

	if(pGame && pGame->GetScreenWidth() > 800)
	{
		field_14 = 800.0f;
		field_18 = 600.0f;
	}
	else
	{
		field_14 = 640.0f;
		field_18 = 480.0f;
	}
	field_1C = 60.0f;
	field_20 = 0;
	field_24 = 0.085f;
	field_28 = 0.265625f;
	field_2C = 0.437500f;

	FUNC_1006E930();

	field_0 = 0;
}

void CScoreBoard::FUNC_1006E930()
{
	RECT rect;

	GetClientRect(pGame->GetMainWindowHwnd(),&rect);

	field_10 = 1.0f;
	field_8 = rect.right * 0.5f - field_14 * 0.5f;
	field_C = rect.bottom * 0.5f - field_18 * 0.5f;
}

void CScoreBoard::ResetDialogControls(CDXUTDialog *pDialog)
{
	m_pDialog = pDialog;

	if(pDialog) {
		pDialog->SetSize(field_14, field_18);

		m_pListBox = new CDXUTListBox(m_pDialog);
		m_pDialog->AddControl(m_pListBox);
		m_pListBox->SetLocation(0, field_1C);
		m_pListBox->SetSize(field_14, field_18 - field_1C);
		m_pListBox->OnInit();
		m_pListBox->GetElement(0)->TextureColor.Init(D3DCOLOR_ARGB( 200, 255, 255, 255 ));
		m_pListBox->field_4D = 3;
		m_pListBox->field_51[0] = field_24 * field_14;
		m_pListBox->field_51[2] = field_28 * field_14;
		m_pListBox->field_51[1] = field_2C * field_14;
		m_pListBox->SetEnabled(false);
		m_pListBox->SetVisible(false);

		FUNC_1006E930();
	}
}

void CScoreBoard::GetRect(RECT *pRect)
{
	pRect->left = (int)field_8;
	pRect->right = pRect->left + (int)field_14;
	pRect->top = (int)field_C;
	pRect->bottom = pRect->top + (int)field_18;
}

void CScoreBoard::Hide(bool bResetInput)
{
	if(!field_0) return;
	if(!m_pDialog) return;

	m_pDialog->SetVisible(false);
	m_pListBox->SetEnabled(false);
	m_pListBox->SetVisible(false);

	if(bResetInput)
		pGame->ToggleKeyInputsDisabled(0, FALSE);

	field_0 = 0;
}

// swapping rows through a temp is what retail inlines at both sort sites
inline void SwapScoreboardRows(SCOREBOARD_ROW *pA, SCOREBOARD_ROW *pB)
{
	SCOREBOARD_ROW tmp = *pA;
	*pA = *pB;
	*pB = tmp;
}

void CScoreBoard::FUNC_1006ED30()
{
	if(!pNetGame) return;
	if(!pNetGame->GetPlayerPool()) return;
	if(!field_0) return;
	if(!m_pDialog) return;

	int nSelected = m_pListBox->GetSelectedIndex();
	CDXUTScrollBar *pScrollBar = m_pListBox->GetScrollBar();
	int nTrackPos = pScrollBar->GetTrackPos();
	m_pListBox->RemoveAllItems();

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	int nRows = pPlayerPool->GetPlayerCount(0) + 1;
	int localPlayerId = pPlayerPool->GetLocalPlayerID();
	SCOREBOARD_ROW *pRows = (SCOREBOARD_ROW *)calloc(nRows, sizeof(SCOREBOARD_ROW));

	strcpy(pRows->szName, pPlayerPool->GetLocalPlayerName());
	pRows->Color = pPlayerPool->GetLocalPlayer()->GetPlayerColorAsARGB();
	pRows->iScore = pPlayerPool->field_0;
	pRows->iPing = pPlayerPool->field_22;
	pRows->iPlayerId = pPlayerPool->GetLocalPlayerID();

	SCOREBOARD_ROW *pRow = pRows + 1;
	for(int playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if(pPlayerPool->GetSlotState((PLAYERID)playerId) != 1) continue;
		if(playerId == localPlayerId) continue;
		if(pPlayerPool->IsPlayerNPC((PLAYERID)playerId)) continue;

		pRow->iPing = pPlayerPool->GetPlayerPing((PLAYERID)playerId);
		strcpy(pRow->szName, pPlayerPool->FUNC_100175C0(playerId));
		pRow->iScore = pPlayerPool->GetPlayerScore((PLAYERID)playerId);
		pRow->Color = pPlayerPool->GetAt((PLAYERID)playerId)->GetPlayerColorAsARGB();
		pRow->iPlayerId = playerId;
		pRow++;
	}

	if(field_40 == 1)
	{
		for(int i = nRows - 1; i > 0; i--)
			for(int j = 0; j < i; j++)
				if(strcmp(pRows[j + 1].szName, pRows[j].szName) < 0)
					SwapScoreboardRows(&pRows[j], &pRows[j + 1]);
	}
	else if(field_40 == 2)
	{
		for(int i = nRows - 1; i > 0; i--)
			for(int j = 0; j < i; j++)
				if(pRows[j + 1].iScore > pRows[j].iScore)
					SwapScoreboardRows(&pRows[j], &pRows[j + 1]);
	}

	char szText[256];
	SCOREBOARD_ROW *pFill = pRows;
	for(int i = 0; i < nRows; i++)
	{
		sprintf(szText, "%u", pFill->iPlayerId);
		m_pListBox->AddItem(szText, (void *)pFill->iPlayerId, pFill->Color);

		if(m_pListBox->GetItem(i) && pFill->szName[0])
		{
			m_pListBox->SetItemColumnText(i, 0, pFill->szName);
			sprintf(szText, "%d", pFill->iScore);
			m_pListBox->SetItemColumnText(i, 1, szText);
			sprintf(szText, "%u", pFill->iPing);
			m_pListBox->SetItemColumnText(i, 2, szText);
		}

		pFill++;
	}

	if(nSelected < 0)
		m_pListBox->SelectItem(-1);
	else
		m_pListBox->SelectItem(nSelected);

	free(pRows);

	pScrollBar->SetTrackPos(nTrackPos);
}

void CScoreBoard::Show()
{
	if(field_0) return;
	if(!m_pDialog) return;

	m_pDialog->SetVisible(true);
	m_pListBox->SetEnabled(true);
	m_pListBox->UpdateRects();
	m_pListBox->SetVisible(true);

	FUNC_1006ED30();
	pGame->ToggleKeyInputsDisabled(3, FALSE);

	field_0 = 1;
}

void CScoreBoard::ClickPlayer()
{
	if(!field_0) return;
	if(!m_pDialog) return;

	DXUTListBoxItem *pItem = m_pListBox->GetSelectedItem();
	if(pItem)
	{
		PLAYERID playerId = (PLAYERID)atoi(pItem->strText);
		RakNet::BitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write((BYTE)0);
		pNetGame->GetRakClient()->RPC(RPC_ClickPlayer, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
	}

	if(!field_0) return;
	if(!m_pDialog) return;

	m_pDialog->SetVisible(false);
	m_pListBox->SetEnabled(false);
	m_pListBox->SetVisible(false);
	pGame->ToggleKeyInputsDisabled(0, FALSE);

	field_0 = 0;
}
