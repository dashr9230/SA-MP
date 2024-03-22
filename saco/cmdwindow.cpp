
#include "main.h"

//----------------------------------------------------

CCmdWindow::CCmdWindow(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice			= pD3DDevice;
	field_14E0				= 0;
	m_iCmdCount				= 0;
	field_1AF4				= 0;
	field_8					= 0;
	field_1AF0				= 0;

	memset(&field_1565[0],0,1290);
	memset(&field_14E4[0],0,129);
	memset(&field_1A6F[0],0,129);
}

//----------------------------------------------------

void CCmdWindow::AddDefaultCmdProc(CMDPROC cmdDefault)
{
	m_pDefaultCmd = cmdDefault;	
}

//----------------------------------------------------

void CCmdWindow::AddCmdProc(PCHAR szCmdName, CMDPROC cmdHandler)
{
	if(m_iCmdCount < MAX_CMDS && (strlen(szCmdName) < MAX_CMD_STRLEN)) {
		m_pCmds[m_iCmdCount] = cmdHandler;
		strcpy(m_szCmdNames[m_iCmdCount],szCmdName);
		m_iCmdCount++;
	}
}

//----------------------------------------------------

