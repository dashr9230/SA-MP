
#pragma once

#define MAX_CMD_INPUT   128
#define MAX_CMDS		144
#define MAX_CMD_STRLEN  32

typedef void (__cdecl *CMDPROC)(PCHAR);

//----------------------------------------------------

class CCmdWindow
{
private:

	IDirect3DDevice9 *m_pD3DDevice;
	CDXUTDialog		*m_pGameUI;
	CDXUTIMEEditBox	*m_pEditControl;

public:

	CMDPROC		m_pCmds[MAX_CMDS];
	CHAR        m_szCmdNames[MAX_CMDS][MAX_CMD_STRLEN+1];
	int			m_iCmdCount;

	BOOL		m_bEnabled;

	char		m_szInputBuffer[MAX_CMD_INPUT + 1];
	char field_1565[1290];
	char field_1A6F[129];
	int field_1AF0;
	int field_1AF4;
	CMDPROC		m_pDefaultCmd;	 // used when no command specifier was
								 // used (ie. a normal chat message)

	void GetDialogSize(RECT *pRect);

	void Enable();
	void Disable();

	void ProcessInput();

	CMDPROC GetCmdHandler(PCHAR szCmdName);
	void AddDefaultCmdProc(CMDPROC cmdDefault);
	void AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler);

	void ResetDialogControls(CDXUTDialog *pGameUI);
	CCmdWindow(IDirect3DDevice9 *pD3DDevice);
	~CCmdWindow();
};

//----------------------------------------------------
// EOF
