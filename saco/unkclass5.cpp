
#include "main.h"

CUnkClass5::CUnkClass5(IDirect3DDevice9 *pD3DDevice)
{
	field_0 = 0;
	field_4 = 0;
	m_pD3DDevice = pD3DDevice;
	m_pDialog = NULL;
	field_30 = 0;
	field_14 = 640;
	field_18 = 300;
	field_1C = 210;
	field_20 = 30;
	field_34 = 3;

	strcpy(field_38, "Type");
	strcpy(field_B9, "ID");
	strcpy(field_13A, "Progress");

	field_23C = 180;
	field_240 = 220;
	field_24C = 0;
	field_250 = -1;
	field_25C = 0;
	field_258 = 0;
	field_254 = RakNet::GetTime();
}

void CUnkClass5::ResetDialogControls(CDXUTDialog *pDialog)
{
	m_pDialog = pDialog;

	if(pDialog) {
		m_pListBox = new CDXUTListBox(m_pDialog);
		m_pDialog->AddControl(m_pListBox);
		m_pListBox->SetLocation(2,2);
		m_pListBox->SetSize(field_14-4,field_18-35);
		m_pListBox->OnInit();
		m_pListBox->GetElement(0)->TextureColor.Init(D3DCOLOR_ARGB( 200, 255, 255, 255 ));
		m_pListBox->SetEnabled(false);
		m_pListBox->SetVisible(false);
		m_pListBox->field_4D = 2;
		m_pListBox->field_51 = field_23C;
		m_pListBox->field_55 = field_240;
	}
}
