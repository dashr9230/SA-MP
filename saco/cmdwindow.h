
#pragma once

#define MAX_CMDS		144
#define MAX_CMD_STRLEN  32

typedef void (__cdecl *CMDPROC)(PCHAR);

class CCmdWindow // size: 6908
{
private:

	IDirect3DDevice9 *m_pD3DDevice;
	int field_4;
	int field_8;

public:

	CMDPROC		m_pCmds[MAX_CMDS];
	CHAR        m_szCmdNames[MAX_CMDS][MAX_CMD_STRLEN+1];
	int			m_iCmdCount;

	int field_14E0;
	char field_14E4[129];
	char field_1565[1290];
	char field_1A6F[129];
	int field_1AF0;
	int field_1AF4;

	CMDPROC		m_pDefaultCmd;	 // used when no command specifier was
								 // used (ie. a normal chat message)


	CCmdWindow(IDirect3DDevice9 *pD3DDevice);

	void AddDefaultCmdProc(CMDPROC cmdDefault);
	void AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler);
};
