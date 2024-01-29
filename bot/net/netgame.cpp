
#include "../main.h"

char unnamed_2[63];
char unnamed_5[1000][24];
int unnamed_6[1000];
char unnamed_9;
char unnamed_1[68];
char unnamed_3[1000][68];
char unnamed_8[1000];
int unnamed_7[2000];
char unnamed_4[1000][63];

//----------------------------------------------------

CNetGame::CNetGame()
{

}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	m_pRakClient->Disconnect(0);
	UnRegisterRPCs(m_pRakClient);
	UnRegisterScriptRPCs(m_pRakClient);	// Unregister server-side scripting RPCs.
	RakNetworkFactory::DestroyRakClientInterface(m_pRakClient);
	SAFE_DELETE(m_pGameMode);

	// TODO: ~CNetGame (W: 00416D50 L: 080ACFDE)
	/*
	if ( this->field_38A )
	{
		v4 = (void *)this->field_38A;
		if ( v4 )
		{
			sub_80B8160(v4);
			operator delete(v4);
		}
		this->field_38A = 0;
	}
	if ( this->field_4 )
	{
		v3 = (void *)this->field_4;
		if ( v3 )
		{
			sub_80B1FF8(v3);
			operator delete(v3);
		}
		this->field_4 = 0;
	}
	if ( this->field_8 )
	{
		v2 = (void *)this->field_8;
		if ( v2 )
		{
			sub_80B9470(v2);
			operator delete(v2);
		}
		this->field_8 = 0;
	}*/
}

//----------------------------------------------------

void CNetGame::Init(PCHAR szHostOrIp, int iPort,
				    PCHAR szPlayerName, PCHAR szPass,
				    PCHAR szNpcMode)
{
	strcpy(m_szHostName, "San Andreas Multiplayer");
	strncpy(m_szHostOrIp, szHostOrIp, sizeof(m_szHostOrIp));
	m_iPort = iPort;

	m_pGameMode = new CGameMode();
	m_pScriptTimers = new CScriptTimers();

	// Setup player pool
	m_pPlayerPool = new CPlayerPool();
	m_pPlayerPool->SetLocalPlayerName(szPlayerName);
	m_pVehiclePool = new CVehiclePool();

	m_pRakClient = RakNetworkFactory::GetRakClientInterface();

	RegisterRPCs(m_pRakClient);
	RegisterScriptRPCs(m_pRakClient);
	
	
	
	

	// TODO: CNetGame::Init (W: 00416490 L: 080AD126)

	/*
	this->field_0 = sub_8070E80();
	sub_8094DD0((unsigned __int16)a3);
	sub_80B1714(this->field_0);
	sub_80B7032(this->field_0);
	sprintf(byte_810A720, "npcmodes/%s.amx", a6);
	if ( !(unsigned __int8)sub_80A9448((void *)this->field_386, byte_810A720) )
		exit(1);
	(*(void (__cdecl **)(int, char *))(*(_DWORD *)this->field_0 + 20))(this->field_0, a5);
	(*(void (__cdecl **)(int, char *, _DWORD, _DWORD, _DWORD, signed int))(*(_DWORD *)this->field_0 + 8))(
		this->field_0,
		src,
		(unsigned __int16)a3,
		0,
		0,
		10);*/
	m_iGameState = 1;
	field_14 = 0;
	field_1E = 12;
	field_1F = 0;
	field_20 = 10;
	field_34 = 1006834287;
	field_38 = 0;
	field_10 = 0;
	field_3C = 1;
	field_40 = 0;
	for (int i = 0; i < 100; ++i )
		field_4A[i] = 0;
	field_31 = 1;
	field_3E = 0;
	field_3D = 0;
	memset(unnamed_1,0,sizeof(unnamed_1));
	memset(unnamed_2,0,sizeof(unnamed_2));
	memset(unnamed_3,0,sizeof(unnamed_3));
	memset(unnamed_4,0,sizeof(unnamed_4));
	memset(unnamed_5,0,sizeof(unnamed_5));
	memset(unnamed_6,0,sizeof(unnamed_6));
	memset(unnamed_7,0,sizeof(unnamed_7));
	memset(unnamed_8,0,sizeof(unnamed_8));
	field_1DE = 0;
	field_1E2 = 0;
	field_1F2 = GetTickCount();
	unnamed_9 = 0;
	field_1FA = -1;
	field_1FE = -1;
	srand(0);
}

//----------------------------------------------------

//----------------------------------------------------

void CNetGame::Process()
{
	// TODO: CNetGame::Process (W: 00418370 L: 080AD6A4)
	/*
	v2 = sub_80AC130();
	sub_80AE6A4(this);
	if ( this->field_C == 2 )
	{
		if ( this->field_386 )
			sub_80A969E(this->field_386, v2);
		if ( this->field_38A )
			sub_80B8610(this->field_38A, (signed __int64)(v2 * 1000.0));
		if ( byte_810A708 )
		{
			if ( this->field_1DE )
			{
				sub_80ADFDE(this);
			}
			else
			{
				sub_80AD770(this);
				byte_80E16B7 = 1;
			}
		}
	}*/
}

//----------------------------------------------------
