
#pragma once

#define GAMESTATE_CONNECTED		5

#define GAMESTATE_WAIT_CONNECT	1

#define GAMESTATE_CONNECTING	2

struct struc_41
{
	bool bUseCJWalk;
	int field_1;
	float fWorldBoundryPX;
	float fWorldBoundryZX;
	float fWorldBoundryPY;
	float fWorldBoundryNY;
	char _gap15;
	float field_16;
	char field_1A;
	char _gap1B[4];
	char field_1F;
	char field_20;
	char field_21;
	char field_22;
	int field_23;
	float field_27;
	char field_2B;
	BYTE field_2C;
	BYTE field_2D;
	char field_2E;
	char field_2F;
	int field_30;
	char _gap34[4];
	BYTE field_38;
	char _gap39;
};

typedef struct _NETGAME_POOLS {
	CVehiclePool	*pVehiclePool;
	CPlayerPool		*pPlayerPool;
	CPickupPool		*pPickupPool;
	CObjectPool		*pObjectPool;
	CActorPool		*pActorPool;
	CGangZonePool	*pGangZonePool;
	CLabelPool		*pLabelPool;
	CTextDrawPool	*pTextDrawPool;
	CMenuPool		*pMenuPool;
} NETGAME_POOLS;

class CNetGame // size: 994
{
private:

	RakClientInterface	*m_pRakClient;

	void UpdateNetwork();

	char _gap4[44];
	char m_szHostOrIp[257];
	char m_szHostName[257];
	char field_232;
public:
	char field_233;
	BYTE field_234;
private:
	int m_iPort;
	int field_239;
	DWORD		m_dwMapIcon[100];
	int m_iGameState;
	int field_3D1;
	struc_41 *field_3D5;
	char field_3D9;
	char _gap3DA[4];
	NETGAME_POOLS* m_pPools;

public:

	CNetGame(PCHAR szHostOrIp,int iPort,PCHAR szPlayerName,PCHAR szPass);

	void Process();

	void InitPools();

	DWORD GetTime();
	int GetGameState() { return m_iGameState; };
	char GetField232() { return field_232; };
	int GetField239() { return field_239; };
	void SetField232(char cValue) { field_232 = cValue; };
	BOOL GetWalkStyle() { return field_3D5->bUseCJWalk; };
	struc_41 * GetSettings() { return field_3D5; };
	void SetWorldTime(BYTE byteHour) { field_3D5->field_2C = byteHour; };
	void SetWorldMinute(BYTE byteMinute) { field_3D5->field_2D = byteMinute; };
	void SetWeather(BYTE byteWeather) { field_3D5->field_2E = byteWeather; };

	CVehiclePool * GetVehiclePool() { return m_pPools->pVehiclePool; };
	CVehiclePool * FUNC_10001180(); // .text:10001180, out of line copy of GetVehiclePool
	CPlayerPool * GetPlayerPool() { return m_pPools->pPlayerPool; };
	CPickupPool * GetPickupPool() { return m_pPools->pPickupPool; };
	CActorPool * GetActorPool() { return m_pPools->pActorPool; };
	CObjectPool * GetObjectPool() { return m_pPools->pObjectPool; };
	CObjectPool * FUNC_10002E10() { return m_pPools->pObjectPool; }; // .text:10002E10, out of line copy of GetObjectPool
	CGangZonePool * GetGangZonePool() { return m_pPools->pGangZonePool; };
	CLabelPool * GetLabelPool() { return m_pPools->pLabelPool; };
	CTextDrawPool * GetTextDrawPool() { return m_pPools->pTextDrawPool; };
	RakClientInterface * GetRakClient() { return m_pRakClient; };

	void  ResetMapIcons();
	void  SetMapIcon(BYTE byteIndex, float fX, float fY, float fZ, BYTE byteIcon, DWORD dwColor, int iStyle);
	// MATCH
	void  DisableMapIcon(BYTE byteIndex); // .text:10008FB0

	CMenuPool * GetMenuPool() { return m_pPools->pMenuPool; }; // .text:100084F0
	void ResetPlayerPool(); // .text:10008FE0
	void ResetVehiclePool(); // .text:10009080
	void ResetTextDrawPool(); // .text:10009110
	void ResetObjectPool(); // .text:100091B0
	void ResetGangZonePool(); // .text:10009250
	void ResetPickupPool(); // .text:100092F0
	void ResetMenuPool(); // .text:10009350
	void ResetLabelPool(); // .text:100093F0
	void ResetActorPool(); // .text:10009490

	void AttemptConnect(); // .text:10008940

	void ResetWorldBounds(); // .text:100088F0

	void Packet_AimSync(Packet *p); // .text:10009C40
	void Packet_PassengerSync(Packet *p); // .text:10009D30
	void Packet_TrailerSync(Packet *p); // .text:10009E20
	void Packet_RSAPublicKeyMismatch(Packet *p); // .text:10008D50
	void Packet_ConnectionBanned(Packet *p); // .text:10008D70
	void Packet_ConnectionSucceeded(Packet *p); // .text:10008D90
	void Packet_NoFreeIncomingConnections(Packet *p); // .text:10008DB0
	void Packet_DisconnectionNotification(Packet *p); // .text:10008DE0
	void Packet_InvalidPassword(Packet *p); // .text:10008E20
	void Packet_ConnectionAttemptFailed(Packet *p); // .text:10008E60

	void UpdateScoresPingsIPs(); // .text:10008F10

	void ToggleNameTagStatus(); // .text:10008E90
	void sub_1000A540(); // .text:1000A540
};
