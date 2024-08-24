
#include "main.h"

extern CGame* pGame;

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
		m_pListBox->field_51 = field_24 * field_14;
		m_pListBox->field_59 = field_28 * field_14;
		m_pListBox->field_55 = field_2C * field_14;
		m_pListBox->SetEnabled(false);
		m_pListBox->SetVisible(false);

		FUNC_1006E930();
	}
}
