
#pragma once

//----------------------------------------------------

class CDeathWindow
{
private:
public:
	void AddMessage(CHAR *a1, CHAR *a2, DWORD a3, DWORD a4, BYTE a5);

	CDeathWindow(IDirect3DDevice9 *pD3DDevice);
};

//----------------------------------------------------
