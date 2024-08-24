
#include "../main.h"
#include "../../raknet/SocketDataEncryptor.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;

//----------------------------------------------------

BYTE __stdcall GetPacketID(Packet *p)
{
	if (p==0) return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else {
		return (unsigned char) p->data[0];
	}
}

//----------------------------------------------------

bool __stdcall HasTimestamps(Packet *p)
{
	if (p==0) return 0;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return true;
	return false;
}

//----------------------------------------------------

CNetGame::CNetGame(PCHAR szHostOrIp, int iPort, 
				   PCHAR szPlayerName, PCHAR szPass)
{
	field_3D5 = (struc_41*)calloc(1,sizeof(struc_41));

	memset(m_szHostName,0,256);
	memset(m_szHostOrIp,0,256);

	strcpy(m_szHostName, "SA-MP");
	strncpy(m_szHostOrIp, szHostOrIp, sizeof(m_szHostOrIp));
	m_iPort = iPort;

	m_pRakClient = RakNetworkFactory::GetRakClientInterface();

	InitPools();

	SocketDataEncryptor::SetKey(iPort);

	RegisterRPCs(m_pRakClient);
	RegisterScriptRPCs(m_pRakClient);	// Register server-side scripting RPCs.

	m_iGameState = GAMESTATE_WAIT_CONNECT;

	if(pChatWindow) pChatWindow->AddDebugMessage("{FFFFFF}SA-MP {B9C9BF}0.3.7-R5 {FFFFFF}Started");

	// TODO: CNetGame::CNetGame(...) .text:1000B930
}

//----------------------------------------------------

void CNetGame::Process()
{
	UpdateNetwork();
	// TODO: CNetGame::Process()
}

//----------------------------------------------------
// UPDATE NETWORK
//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
/*	Packet* pkt=NULL;
	unsigned char packetIdentifier;

	while((pkt = m_pRakClient->Receive()))
	{

	}*/
	
	// TODO: CNetGame::UpdateNetwork()
}

void CNetGame::InitPools()
{
	m_pPools = (NETGAME_POOLS *)calloc(1, sizeof(NETGAME_POOLS));

	m_pPools->pObjectPool	= new CObjectPool();
	m_pPools->pMenuPool		= new CMenuPool();
	m_pPools->pLabelPool	= new CLabelPool();
	m_pPools->pTextDrawPool	= new CTextDrawPool();
	m_pPools->pGangZonePool	= new CGangZonePool();
	m_pPools->pPlayerPool	= new CPlayerPool();
	m_pPools->pVehiclePool	= new CVehiclePool();
	m_pPools->pPickupPool	= new CPickupPool();
	m_pPools->pActorPool	= new CActorPool();
}

DWORD CNetGame::GetTime()
{
	return (DWORD)RakNet::GetTime();
}

//-----------------------------------------------------------
// Puts a personal marker using any of the radar icons on the map

void CNetGame::SetMapIcon(BYTE byteIndex, float fX, float fY, float fZ, BYTE byteIcon, DWORD dwColor, int iStyle)
{
	if (byteIndex >= 100) return;
	if (m_dwMapIcon[byteIndex] != NULL) DisableMapIcon(byteIndex);
	//ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, byteIcon, &m_dwMapIcon);
	m_dwMapIcon[byteIndex] = pGame->CreateRadarMarkerIcon(byteIcon, fX, fY, fZ, dwColor, iStyle);
}

//-----------------------------------------------------------
// Removes the Map Icon

void CNetGame::DisableMapIcon(BYTE byteIndex)
{
	if (byteIndex >= 100) return;
	ScriptCommand(&disable_marker, m_dwMapIcon[byteIndex]);
	m_dwMapIcon[byteIndex] = NULL;
}

//----------------------------------------------------

void CNetGame::ResetMapIcons()
{
	BYTE i;
	for (i = 0; i < 100; i++)
	{
		if (m_dwMapIcon[i] != NULL) DisableMapIcon(i);
	}
}

//----------------------------------------------------
