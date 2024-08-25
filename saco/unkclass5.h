
#pragma once

class CUnkClass5
{
private:
	int field_0;
	int field_4;
	IDirect3DDevice9* m_pD3DDevice;
	char _gapC[8];
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	char _gap24[4];
	CDXUTDialog *m_pDialog;
	CDXUTListBox *m_pListBox;
	int field_30;
	int field_34;
	char field_38[129];
	char field_B9[129];
	char field_13A[129];
	char _gap1BB[129];
	int field_23C;
	int field_240;
	char _gap244[8];
	int field_24C;
	int field_250;
	RakNetTime field_254;
	int field_258;
	int field_25C;

public:
	CUnkClass5(IDirect3DDevice9 *pD3DDevice);

	void ResetDialogControls(CDXUTDialog *pDialog);

};
