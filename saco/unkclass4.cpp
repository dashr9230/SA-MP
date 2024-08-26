
#include "main.h"

CUnkClass4::CUnkClass4(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pDialog = NULL;
	field_24 = 0;
	field_C = 280;
	field_10 = 150;
	field_14 = 210;
	field_18 = 30;
	field_1C = 38;
}

void CUnkClass4::ResetDialogControls(CDXUTDialog *pDialog)
{
	D3DXCOLOR Color;

	m_pDialog = pDialog;

	if(pDialog) {
		pDialog->AddSlider(30,50,10,field_14,field_18,-100,100,0);
		m_pDialog->GetSlider(30)->SetColor(D3DXCOLOR(0.6f,0.6f,0.80000007f,1.0f));

		int y = field_1C + 10;
		m_pDialog->AddSlider(32,50,y,field_14,field_18,-100,100,0);
		m_pDialog->GetSlider(32)->SetColor(D3DXCOLOR(0.80000007f,0.6f,0.6f,1.0f));

		m_pDialog->AddSlider(34,50,y+field_1C,field_14,field_18,-100,100,0);
		m_pDialog->GetSlider(34)->SetColor(D3DXCOLOR(0.6f,0.80000007f,0.6f,1.0f));
	}
}
