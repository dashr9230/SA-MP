
#include "main.h"

extern CConfig *pConfig;

CUnkClass3::CUnkClass3(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pDialog = NULL;
	field_28 = 0;
	field_34 = 0;
	field_30 = 0;
	field_2C = 0;
	field_C = 600;
	field_10 = 300;
	field_14 = 100;
	field_18 = 30;
	memset(field_89, 0, sizeof(field_89));
}

void CUnkClass3::ResetDialogControls(CDXUTDialog *pDialog)
{
	m_pDialog = pDialog;

	if(pDialog) {
		m_pListBox = new CDXUTListBox(m_pDialog);
		m_pDialog->AddControl(m_pListBox);
		m_pListBox->SetLocation(10,10);
		m_pListBox->SetSize(field_C,field_10-100);
		m_pListBox->OnInit();
		m_pListBox->GetElement(0)->TextureColor.Init(D3DCOLOR_ARGB( 200, 255, 255, 255 ));
		m_pListBox->field_4D = 0;
		m_pListBox->SetEnabled(false);
		m_pListBox->SetVisible(false);
		m_pDialog->AddButton(20,"BUTTON1",10,5,field_14,field_18);
		m_pDialog->AddButton(21,"BUTTON2",110,5,field_14,field_18);

		m_pDialog->AddIMEEditBox(19,"",10,175,570,40,true,&m_pEditControl);
		if(pConfig->GetIntVariable("ime")) {
			CDXUTIMEEditBox::EnableImeSystem(true);
			CDXUTIMEEditBox::StaticOnCreateDevice();
		}
		m_pEditControl->GetElement(0)->TextureColor.Init(D3DCOLOR_ARGB( 240, 5, 5, 5 ));
		m_pEditControl->SetTextColor(D3DCOLOR_ARGB( 255, 255, 255, 255 ));
		m_pEditControl->SetCaretColor(D3DCOLOR_ARGB( 255, 150, 150, 150 ));
		m_pEditControl->SetSelectedBackColor(D3DCOLOR_ARGB( 255, 185, 34, 40 ));
		m_pEditControl->SetSelectedTextColor(D3DCOLOR_ARGB( 255, 10, 10, 15 ));
		m_pEditControl->SetEnabled(false);
		m_pEditControl->SetVisible(false);
	}
}
