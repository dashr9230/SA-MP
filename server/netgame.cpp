
#include "main.h"

char szGameModeFile[256];

CNetGame::CNetGame()
{
	m_bAllowWeapons = FALSE;
	m_byteWorldTime = 12;
	m_byteWeather	= 1;
	m_bStuntBonus   = true;
	m_fGravity		= 0.008f;
	m_iDeathDropMoney = 0;
	field_6A		= false;
	m_bVehicleFriendlyFire = FALSE;

	// Init member variables
	m_pPlayerPool = NULL;
	m_pVehiclePool = NULL;
	m_pGameMode = NULL;
	m_pPickupPool = NULL;
	m_pObjectPool = NULL;
	m_pMenuPool = NULL;
	m_pTextPool = NULL;
	m_bNameTagLOS = true;
	m_pGangZonePool = NULL;
	m_pLabelPool = NULL;
	m_pActorPool = NULL;
	m_bLanMode = FALSE;
	field_6B = 1;

	m_bLimitGlobalChatRadius = FALSE;
	m_fGlobalChatRadius = 10000.0f;
	m_bLimitPlayerMarkerRadius = FALSE;
	m_fPlayerMarkerRadius = 10000.0f;
	m_fNameTagDrawDistance = 70.0f;
	m_bDisableEnterExits = false;
	field_5C = 0;

	m_iCurrentGameModeIndex = 0;
	m_iCurrentGameModeRepeat = 0;
	m_bFirstGameModeLoaded = FALSE;

	m_pScriptTimers = new CScriptTimers;
	m_pScriptHttps = new CScriptHttps;

#ifndef WIN32
	m_dElapsedTime = 0.0;
#endif

	if(pConsole->GetIntVariable("maxplayers") > MAX_PLAYERS) {
		pConsole->SetIntVariable("maxplayers", MAX_PLAYERS);
	}

	CHAR *szBindAddress = pConsole->GetStringVariable("bind");
	if (szBindAddress && szBindAddress[0] == 0)
		szBindAddress = NULL;

	DWORD dwPort = pConsole->GetIntVariable("port");
	DWORD dwMaxPlayers = pConsole->GetIntVariable("maxplayers");
	BOOL bLanMode = pConsole->GetBoolVariable("lanmode");

	// Setup RakNet
	m_pRak = RakNetworkFactory::GetRakServerInterface();





	// TODO: CNetGame::CNetGame W: 0048EEE0 L: 080AF360

/*
  sub_496D10(v9);
  dword_515CFC = sub_44E9D0();
  _this->field_44 = sub_48DBD0(_this);
  _this->field_48 = 0;
  _this->field_4C = 0;
  sub_46A7B0(v8);
  if ( (unsigned __int8)(*(int (__stdcall **)(int, _DWORD, int, int, char *))(*(_DWORD *)_this->field_40 + 4))(
                          v19,
                          0,
                          iSleep,
                          v8,
                          v7) )
  {
    sub_48EAE0(_this);
    if ( !sub_48DD50(0) )
    {
      logprintf("I couldn't load any gamemode scripts. Please verify your server.cfg");
      logprintf("It needs a gamemode0 line at the very least.");
      _fcloseall();
      exit(1);
    }
    (*(void (**)(void))(*(_DWORD *)_this->field_40 + 84))();
    v10 = CConsole::GetStringVariable(pConsole, "password");
    if ( v10 && *v10 )
      (*(void (__stdcall **)(char *))(*(_DWORD *)_this->field_40 + 16))(v10);
    sprintf(&pInitStr, "%02d:%02d", _this->field_59, 0);
    CConsole::AddStringVariable(pConsole, "worldtime", 4u, &pInitStr, 0);
    if ( v20 )
      _this->field_50 = 1;
    GetCurrentDirectoryA(0x200u, &Value);
    v11 = strlen(&Value);
    if ( *((_BYTE *)&v22 + v11 + 3) != 92 )
    {
      *(&Value + v11) = 92;
      v24[v11] = 0;
    }
    v12 = (char *)&v22 + 3;
    do
      v13 = (v12++)[1];
    while ( v13 );
    *(_DWORD *)v12 = 1769104243;
    *((_DWORD *)v12 + 1) = 1768322160;
    *((_DWORD *)v12 + 2) = 1551066476;
    v12[12] = 0;
    SetEnvironmentVariableA("AMXFILE", &Value);
    v14 = operator new(0x1034u);
    v22 = v14;
    v26 = 2;
    if ( v14 )
      v15 = sub_46A7C0(v14);
    else
      v15 = 0;
    _this->field_4 = v15;
    if ( v21 )
    {
      v16 = (int (__stdcall **)(int, int, int))"ON";
      if ( !v20 )
        v16 = &off_4B9C04;
      printf(
        "\nStarted server on %s:%d, with maxplayers: %d lanmode is %s.\n\n",
        v21,
        (unsigned __int16)v8,
        (unsigned __int16)v19,
        v16);
    }
    else
    {
      v17 = (int (__stdcall **)(int, int, int))"ON";
      if ( !v20 )
        v17 = &off_4B9C04;
      printf(
        "\nStarted server on port: %d, with maxplayers: %d lanmode is %s.\n\n",
        (unsigned __int16)v8,
        (unsigned __int16)v19,
        v17);
    }
    _this->field_5E = 0;
  }
  else if ( v7 )
  {
    logprintf("Unable to start server on %s:%d. Port in use?", v7, (unsigned __int16)v8);
  }
  else
  {
    logprintf("Unable to start server on port: %d. Port in use?", (unsigned __int16)v8);
  }*/
}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	logprintf("--- Server Shutting Down.");

	SAFE_DELETE(m_pGameMode);
	SAFE_DELETE(m_pFilterScripts);
	SAFE_DELETE(m_pScriptTimers);
	SAFE_DELETE(m_pScriptHttps);
	SAFE_DELETE(m_pLabelPool);
	SAFE_DELETE(m_pVehiclePool);
	SAFE_DELETE(m_pPlayerPool);
	SAFE_DELETE(m_pObjectPool);
	SAFE_DELETE(m_pPickupPool);
	SAFE_DELETE(m_pMenuPool);
	SAFE_DELETE(m_pTextPool);
	SAFE_DELETE(m_pGangZonePool);
	SAFE_DELETE(m_pActorPool);

	m_pRak->Disconnect(100);
	UnRegisterRPCs(m_pRak);

	RakNetworkFactory::DestroyRakServerInterface(m_pRak);
}

//----------------------------------------------------
// Loads filterscripts after pools initialisation

void CNetGame::LoadAllFilterscripts()
{
	logprintf("");
	logprintf("Filterscripts");
	logprintf("---------------");
	int iScriptCount = 0;
	char* szFilterScript = strtok(pConsole->GetStringVariable("filterscripts"), " ");
	while (szFilterScript)
	{
		logprintf("  Loading filterscript '%s.amx'...", szFilterScript);
		if (m_pFilterScripts->LoadFilterScript(szFilterScript))
		{
			iScriptCount++;
		} else {
			logprintf("  Unable to load filterscript '%s.amx'.", szFilterScript);
		}
		szFilterScript = strtok(NULL, " ");
	}
	logprintf("  Loaded %d filterscripts.\n", iScriptCount);
}





void CNetGame::Init(BOOL bFirst)
{
	m_iSpawnsAvailable = 0;

	// Setup player pool
	if(!m_pPlayerPool) {
		m_pPlayerPool = new CPlayerPool();
	} else {
		m_pPlayerPool->ResetPlayerScoresAndMoney();
	}

	// Setup vehicle pool
	if(!m_pVehiclePool) {
		m_pVehiclePool = new CVehiclePool();
	}

	// Setup pickup pool
	if(!m_pPickupPool) {
		m_pPickupPool = new CPickupPool();
	}

	// Setup Object pool
	if (!m_pObjectPool) {
		m_pObjectPool = new CObjectPool();
	}

	// Setup Menu pool
	if (!m_pMenuPool) {
		m_pMenuPool = new CMenuPool();
	}

	// Setup Text pool
	if (!m_pTextPool) {
		m_pTextPool = new CTextDrawPool();
	}

	// Setup Gang Zone pool
	if (!m_pGangZonePool) {
		m_pGangZonePool = new CGangZonePool();
	}

	// Setup Label pool
	if (!m_pLabelPool) {
		m_pLabelPool = new CLabelPool();
	}

	// Setup Actor pool
	if (!m_pActorPool) {
		m_pActorPool = new CActorPool();
	}

	// Setup gamemode
	if(!m_pGameMode) {
		m_pGameMode = new CGameMode();
	}

	// Default tags/markers
	m_bShowNameTags = TRUE;
	m_iShowPlayerMarkers = 1;
	m_bNameTagLOS = true;
	m_bUseCJWalk = FALSE;

	// Set the default world time for clients.
	m_byteWorldTime = 12; // 12:00

	// Set the default weather
	m_byteWeather   = 1;

	m_bLimitGlobalChatRadius = FALSE;
	m_bLimitPlayerMarkerRadius = FALSE;
	m_fGlobalChatRadius = 10000.0f;
	m_fPlayerMarkerRadius = 10000.0f;
	m_fNameTagDrawDistance = 70.0f;
	m_bDisableEnterExits = false;
	m_bManualVehicleEngineAndLights = false;

	if (bFirst) LoadAllFilterscripts();
	// Has to be done here as it need to be AFTER the pools but BEFORE the gamemode

	// Start the gamemode script.
	m_pGameMode->Load(szGameModeFile);

	logprintf("Number of vehicle models: %d", m_pVehiclePool->GetModelCount());

	// Flag we're in a running state.
	m_iGameState = GAMESTATE_RUNNING;
}

void CNetGame::ShutdownForGameModeRestart()
{
	// TODO: CNetGame::ShutdownForGameModeRestart W: 0048F430 L: 080ABBE0

/*
  sub_804C8A0(&v8);
  sub_80ABAD0(this, "(", &v8, -1, 2);
  sub_80D1690(this->field_8);
  v1 = (void *)this->field_0;
  if ( this->field_0 )
  {
    sub_80A5060(this->field_0);
    operator delete(v1);
    this->field_0 = 0;
  }
  v2 = (void *)this->field_C;
  if ( v2 )
  {
    sub_814CCA0(this->field_C);
    operator delete(v2);
    this->field_C = 0;
  }
  if ( this->field_10 )
  {
    operator delete((void *)this->field_10);
    this->field_10 = 0;
  }
  v3 = (void *)this->field_14;
  if ( v3 )
  {
    sub_80C8A20(this->field_14);
    operator delete(v3);
    this->field_14 = 0;
  }
  v4 = (void *)this->field_18;
  if ( v4 )
  {
    sub_80AADA0(this->field_18);
    operator delete(v4);
    this->field_18 = 0;
  }
  v5 = (void *)this->field_1C;
  if ( v5 )
  {
    sub_814A2D0(this->field_1C);
    operator delete(v5);
    this->field_1C = 0;
  }
  if ( this->field_24 )
  {
    operator delete((void *)this->field_24);
    this->field_24 = 0;
  }
  v6 = (void *)this->field_20;
  if ( v6 )
  {
    sub_804C1E0(this->field_20);
    operator delete(v6);
    this->field_20 = 0;
  }
  v7 = (void *)this->field_28;
  if ( v7 )
  {
    sub_8094C00(this->field_28);
    operator delete(v7);
    this->field_28 = 0;
  }
  this->field_5A = 0;
  dword_81CA600 = 0;
  this->field_59 = 12;
  this->field_5D = 1;
  LODWORD(this->field_62) = 1006834287;
  this->field_66 = 0;
  this->field_6A = 0;
  this->gap78[0] = 0;
  this->field_5E = 2;
  sub_804CCB0(&v8);
*/
}

void CNetGame::Process()
{
	// TODO: CNetGame::Process W: 00491240 L: 080AEEE0

/*
  v4 = sub_80AA3D0();
  sub_80ABF00(this);
  sub_80AECE0((int)this);
  v1 = this->field_5E;
  if ( v1 == 1 )
  {
    if ( this->field_8 )
      sub_80D0DF0(this->field_8, v4);
    if ( this->field_C )
      sub_814CD00(this->field_C, v4);
    if ( this->field_14 )
      sub_80C8760(this->field_14, v4);
    if ( this->field_0 )
      sub_80A5080(this->field_0, v4);
    v2 = this->field_3C;
    if ( v2 )
      sub_80EBAF0(v2, (signed __int64)(v4 * 1000.0));
    if ( this->field_38 )
      sub_80EA2E0(this->field_38);
  }
  else if ( v1 == 2 )
  {
    v3 = v4 + *(float *)&dword_81CA600;
    *(float *)&dword_81CA600 = v3;
    if ( v3 > 12.0 )
      sub_80AE530(this);
  }
  if ( CConsole::GetBoolVariable(pConsole, "announce") )
  {
    sub_80ABDE0((int)this, v4);
    sub_80D1CA0(pPlugins);
    this->field_82 = this->field_82 + v4;
  }
  else
  {
    sub_80D1CA0(pPlugins);
    this->field_82 = this->field_82 + v4;
  }
*/
}

void CNetGame::LoadBanList()
{
	// TODO: CNetGame::LoadBanList W: 48EAE0 L: 80AF1A0
}

DWORD CNetGame::GetTime()
{
	return (DWORD)RakNet::GetTime();
}
