
#include "../main.h"

CNetGame::CNetGame(PCHAR szHostOrIp, int iPort, 
				   PCHAR szPlayerName, PCHAR szPass)
{
	field_3D5 = (struc_41*)calloc(1,sizeof(struc_41));

	memset(m_szHostName,0,256);
	memset(m_szHostOrIp,0,256);

	strcpy(m_szHostName, "SA-MP");
	strncpy(m_szHostOrIp, szHostOrIp, sizeof(m_szHostOrIp));
	m_iPort = iPort;

	m_iGameState = GAMESTATE_WAIT_CONNECT;

	// TODO: CNetGame::CNetGame(...) .text:1000B930
}
