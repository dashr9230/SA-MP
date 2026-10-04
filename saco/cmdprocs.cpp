
#include "main.h"
#include "game/util.h"

extern CChatWindow   *pChatWindow;
extern CGame		 *pGame;
extern CCmdWindow	 *pCmdWindow;
extern CDeathWindow	 *pDeathWindow;
extern CNetGame		 *pNetGame;
extern GAME_SETTINGS tSettings;
extern CConfig		 *pConfig;
extern CFontRender	 *pDefaultFont;

extern bool bShowDebugLabels;
extern bool bHudScaleFix;
extern bool bCameraTargetDebug;
extern BOOL bToggleObjectLight;

void ShutdownGame(); // .text:100C3E80
int VehicleSelect(int a1, int a2, int a3); // .text:1009E0A0
void sub_100C5430(); // .text:100C5430
char *sub_100C3AD0(); // .text:100C3AD0

void cmdDefaultCmdProc(PCHAR szCmd)
{
	if(pNetGame) {
		CLocalPlayer *pLocalPlayer;
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		pLocalPlayer->Say(szCmd);
	}
}

void cmdTestDeathWindow(PCHAR szCmd)
{
	if(pDeathWindow) {
		pDeathWindow->AddMessage("Pooper","Pooper333",0xFFFFFFFF,0xFFFFFFFF,1);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,5);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,15);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,14);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,2);
		pDeathWindow->AddMessage(0,"PooperPooperPooper0001",0xFFFFFFFF,0xFFFFFFFF,5);
		pDeathWindow->AddMessage(0,"Pooper",0xFFFFFFFF,0xFFFFFFFF,-1);
		pDeathWindow->AddMessage("Pooper","PooperPooperPooper0001",0xFFFFFFFF,0xFFFFFFFF,0);
	}
}

void cmdCameraTargetDebug(PCHAR szCmd)
{
	bCameraTargetDebug = !bCameraTargetDebug;
}

void cmdPageSize(PCHAR szCmd)
{
	int iPageSize;
	if(strlen(szCmd) && (iPageSize = atoi(szCmd)) >= 10 && iPageSize <= 20)
	{
		if(pChatWindow) pChatWindow->SetPageSize(iPageSize);
		if(pConfig) pConfig->SetIntVariable("pagesize", iPageSize);
	}
	else
	{
		if(pChatWindow) pChatWindow->AddDebugMessage("pagesize [10-20] (lines)");
	}
}

void cmdFontSize(PCHAR szCmd)
{
	int iSize;
	if(strlen(szCmd) && (iSize = atoi(szCmd)) >= -3 && iSize <= 5)
	{
		if(pConfig) pConfig->SetIntVariable("fontsize", iSize);
		if(pDefaultFont) pDefaultFont->CreateFonts();
		if(pChatWindow)
		{
			pChatWindow->CreateFonts();
			pChatWindow->FUNC_10067200();
		}
		if(pDeathWindow) pDeathWindow->CreateFonts();
		sub_100C5430();
	}
	else
	{
		if(pChatWindow) pChatWindow->AddDebugMessage("Valid fontsize: -3 to 5");
	}
}

void cmdNameTagStatus(PCHAR szCmd)
{
	if(pNetGame) pNetGame->ToggleNameTagStatus();
}

void cmdTimestamp(PCHAR szCmd)
{
	CChatWindow *pChatWindow = ::pChatWindow;
	if(pChatWindow)
	{
		if(pChatWindow->m_bTimestamp)
		{
			pChatWindow->m_bTimestamp = 0;
			pChatWindow->m_bRedraw = 1;
			if(pConfig) pConfig->SetIntVariable("timestamp", 0);
		}
		else
		{
			pChatWindow->m_bTimestamp = 1;
			pChatWindow->m_bRedraw = 1;
			if(pConfig) pConfig->SetIntVariable("timestamp", 1);
		}
	}
}

void cmdAudioMsg(PCHAR szCmd)
{
	if(pConfig->GetIntVariable("audiomsgoff") == 1)
	{
		pConfig->SetIntVariable("audiomsgoff", 0);
		if(pChatWindow) pChatWindow->AddInfoMessage("Audio messages: On");
	}
	else
	{
		pConfig->SetIntVariable("audiomsgoff", 1);
		if(pChatWindow) pChatWindow->AddInfoMessage("Audio messages: Off");
	}
}

void cmdLogUrls(PCHAR szCmd)
{
	if(pConfig->GetIntVariable("logurls") == 1)
	{
		pConfig->SetIntVariable("logurls", 0);
		if(pChatWindow) pChatWindow->AddInfoMessage("URL messages: Off");
	}
	else
	{
		pConfig->SetIntVariable("logurls", 1);
		if(pChatWindow) pChatWindow->AddInfoMessage("URL messages: On");
	}
}

void cmdHudScaleFix(PCHAR szCmd)
{
	if(bHudScaleFix)
	{
		bHudScaleFix = false;
		pConfig->SetIntVariable("nohudscalefix", 1);
	}
	else
	{
		bHudScaleFix = true;
		pConfig->SetIntVariable("nohudscalefix", 0);
	}
}

//----------------------------------------------------

void cmdMem(PCHAR szCmd)
{
	pChatWindow->AddDebugMessage("Memory: %u",*(DWORD *)0x8A5A80);
}

void cmdSetFrameLimit(PCHAR szCmd)
{
	if(strlen(szCmd))
	{
		unsigned int uiLimit = atoi(szCmd);
		if(uiLimit >= 20 && uiLimit <= 90)
		{
			pGame->SetFrameLimit(uiLimit);
			if(pConfig) pConfig->SetIntVariable("fpslimit", uiLimit);
			pChatWindow->AddInfoMessage("-> Frame Limiter: %u", uiLimit);
		}
		else
		{
			pChatWindow->AddInfoMessage("-> Frame Limiter: valid amounts are 20-90", uiLimit);
		}
	}
}

void cmdHeadMove(PCHAR szCmd)
{
	CGame *pGame = ::pGame;
	if(pGame)
	{
		if(pGame->m_bHeadMove)
		{
			pGame->m_bHeadMove = 0;
			if(pConfig) pConfig->SetIntVariable("disableheadmove", 1);
			pChatWindow->AddInfoMessage("-> Head movements disabled");
		}
		else
		{
			pGame->m_bHeadMove = 1;
			if(pConfig) pConfig->SetIntVariable("disableheadmove", 0);
			pChatWindow->AddInfoMessage("-> Head movements enabled");
		}
	}
}

void cmdQuit(PCHAR szCmd)
{
	ShutdownGame();
}

void cmdCmpStat(PCHAR szCmd)
{
}

void cmdSavePos(PCHAR szCmd)
{
	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

	char szString[264];
	sprintf(szString, "%s\\savedpositions.txt", sub_100C3AD0());

	FILE *f = fopen(szString, "a");
	if(!f)
	{
		pChatWindow->AddDebugMessage("I can't open the savepositions.txt file for append.");
		return;
	}

	if(pPlayerPed->IsInVehicle())
	{
		VEHICLE_TYPE *pVehicle = pPlayerPed->GetGtaVehicle();
		int iIndex = GamePool_Vehicle_GetIndex(pVehicle);
		float fAngle;
		ScriptCommand(&get_car_z_angle, iIndex, &fAngle);
		fprintf(f, "AddStaticVehicle(%u,%.4f,%.4f,%.4f,%.4f,%u,%u); // %s\n",
			pVehicle->entity.nModelIndex,
			pVehicle->entity.mat->pos.X,
			pVehicle->entity.mat->pos.Y,
			pVehicle->entity.mat->pos.Z,
			fAngle,
			pVehicle->byteColor1,
			pVehicle->byteColor2,
			szCmd);
		fclose(f);
		pChatWindow->AddInfoMessage("-> InCar position saved");
	}
	else
	{
		PED_TYPE *pPed = pPlayerPed->m_pPed;
		float fAngle;
		ScriptCommand(&get_actor_z_angle, pPlayerPed->m_dwGTAId, &fAngle);
		fprintf(f, "AddPlayerClass(%u,%.4f,%.4f,%.4f,%.4f,0,0,0,0,0,0); // %s\n",
			pPlayerPed->GetModelIndex(),
			pPed->entity.mat->pos.X,
			pPed->entity.mat->pos.Y,
			pPed->entity.mat->pos.Z,
			fAngle,
			szCmd);
		fclose(f);
		pChatWindow->AddInfoMessage("-> OnFoot position saved");
	}
}

void cmdRawSavePos(PCHAR szCmd)
{
	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	float fAngle;
	FILE *f;
	PED_TYPE *pPed;
	char szString[264];

	if(pPlayerPed->IsInVehicle())
	{
		f = fopen("rawvehicles.txt", "a");
		if(!f) goto cant_open;
		VEHICLE_TYPE *pVehicle = pPlayerPed->GetGtaVehicle();
		int iIndex = GamePool_Vehicle_GetIndex(pVehicle);
		ScriptCommand(&get_car_z_angle, iIndex, &fAngle);
		fprintf(f, "%u,%.4f,%.4f,%.4f,%.4f,%u,%u ; %s\n",
			pVehicle->entity.nModelIndex,
			pVehicle->entity.mat->pos.X,
			pVehicle->entity.mat->pos.Y,
			pVehicle->entity.mat->pos.Z,
			fAngle,
			pVehicle->byteColor1,
			pVehicle->byteColor2,
			szCmd);
		fclose(f);
		pChatWindow->AddDebugMessage("-> InCar pos saved");
		return;
	}

	pPed = pPlayerPed->m_pPed;
	ScriptCommand(&get_actor_z_angle, pPlayerPed->m_dwGTAId, &fAngle);
	sprintf(szString, "%s\\rawpositions.txt", sub_100C3AD0());
	f = fopen(szString, "a");
	if(!f)
	{
cant_open:
		pChatWindow->AddDebugMessage("I can't open the rawvehicles.txt file for append.");
		return;
	}
	fprintf(f, "%.4f,%.4f,%.4f,%.4f ; %s\n",
		pPed->entity.mat->pos.X,
		pPed->entity.mat->pos.Y,
		pPed->entity.mat->pos.Z,
		fAngle,
		szCmd);
	fclose(f);
	pChatWindow->AddDebugMessage("-> OnFoot pos saved");
}

void cmdPlayerSkin(PCHAR szCmd)
{
	if(tSettings.bDebug)
	{
		if(!strlen(szCmd))
		{
			pChatWindow->AddDebugMessage("Usage: player_skin (skin number).");
		}
		else
		{
			int iSkin = atoi(szCmd);
			if(pGame->sub_100A0930())
			{
				CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
				if(pPlayerPed)
				{
					pPlayerPed->SetSkin(iSkin);
				}
			}
		}
	}
}

void cmdCreateVehicle(PCHAR szCmd)
{
	if(tSettings.bDebug)
	{
		if(!strlen(szCmd))
		{
			pChatWindow->AddDebugMessage("Usage: /v (vehicle id).");
		}
		else
		{
			int iModel = atoi(szCmd);
			if(pGame->sub_100A0930())
			{
				pGame->sub_100A0940(iModel, 2);
				pGame->sub_100A0960();
				CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
				if(pPlayerPed)
				{
					MATRIX4X4 mat;
					pPlayerPed->GetMatrix(&mat);
					char szName[9] = {0};
					sprintf(szName, "TYPE_%d", iModel);
					pGame->sub_100A0250(iModel, mat.pos.X - 5.0f, mat.pos.Y - 5.0f, mat.pos.Z + 1.0f, 0, 0)->Add();
				}
				else
				{
					pChatWindow->AddDebugMessage("I couldn't find the player actor.");
				}
			}
			else
			{
				pChatWindow->AddDebugMessage("game not loaded.");
			}
		}
	}
}

void cmdSelectVehicle(PCHAR szCmd)
{
	if(tSettings.bDebug)
	{
		VehicleSelect(0, 0, 10);
	}
}

void cmdSetWeather(PCHAR szCmd)
{
	if(!strlen(szCmd))
	{
		pChatWindow->AddDebugMessage("Usage: set_weather (weather number)");
		return;
	}
	pGame->SetWorldWeather(atoi(szCmd));
}

void cmdSetTime(PCHAR szCmd)
{
	if(!strlen(szCmd))
	{
		pChatWindow->AddDebugMessage("Usage: set_time (hour) (minute)");
		return;
	}
	int iHour = 0, iMinute = 0;
	sscanf(szCmd, "%d%d", &iHour, &iMinute);
	if(iHour < 0 || iHour > 23 || iMinute < 0 || iMinute > 59)
	{
		pChatWindow->AddDebugMessage("Invalid Time. Use /set_time (hour 0-23) (minute 0-59)");
	}
	else
	{
		pGame->SetWorldTime(iHour, iMinute);
	}
}

void cmdShowInterior(PCHAR szCmd)
{
	int iInterior;
	ScriptCommand(&get_active_interior, &iInterior);
	pChatWindow->AddDebugMessage("Current Interior: %u", iInterior);
}

void cmdToggleObjectLight(PCHAR szCmd)
{
	bToggleObjectLight = !bToggleObjectLight;
}

void cmdDebugLabels(PCHAR szCmd)
{
	bShowDebugLabels = !bShowDebugLabels;
}

void cmdRcon(PCHAR szCmd)
{
	if(szCmd)
	{
		if(pNetGame)
		{
			RakNet::BitStream bsSend;
			bsSend.Write((BYTE)201);
			int iStrlen = strlen(szCmd);
			bsSend.Write(iStrlen);
			bsSend.Write(szCmd, iStrlen);
			pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}

//----------------------------------------------------

void SetupCommands()
{
	// RELEASE COMMANDS
	pCmdWindow->AddDefaultCmdProc(cmdDefaultCmdProc);
	pCmdWindow->AddCmdProc("quit",cmdQuit);
	pCmdWindow->AddCmdProc("q",cmdQuit);
	pCmdWindow->AddCmdProc("save",cmdSavePos);
	pCmdWindow->AddCmdProc("rs",cmdRawSavePos);
	pCmdWindow->AddCmdProc("rcon",cmdRcon);
	pCmdWindow->AddCmdProc("mem",cmdMem);
	pCmdWindow->AddCmdProc("fpslimit",cmdSetFrameLimit);
	pCmdWindow->AddCmdProc("pagesize",cmdPageSize);
	pCmdWindow->AddCmdProc("fontsize",cmdFontSize);
	pCmdWindow->AddCmdProc("nametagstatus",cmdNameTagStatus);
	pCmdWindow->AddCmdProc("timestamp",cmdTimestamp);
	pCmdWindow->AddCmdProc("headmove",cmdHeadMove);
	pCmdWindow->AddCmdProc("hudscalefix",cmdHudScaleFix);
	pCmdWindow->AddCmdProc("testdw",cmdTestDeathWindow);

#ifndef _DEBUG
	if(tSettings.bDebug)
	{
#endif
	pCmdWindow->AddCmdProc("vsel",cmdSelectVehicle);
	pCmdWindow->AddCmdProc("v",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("vehicle",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("player_skin",cmdPlayerSkin);
	pCmdWindow->AddCmdProc("set_weather",cmdSetWeather);
	pCmdWindow->AddCmdProc("set_time",cmdSetTime);
#ifndef _DEBUG
	}
#endif

	pCmdWindow->AddCmdProc("interior",cmdShowInterior);
	pCmdWindow->AddCmdProc("togobjlight",cmdToggleObjectLight);
	pCmdWindow->AddCmdProc("cmpstat",cmdCmpStat);
	pCmdWindow->AddCmdProc("dl",cmdDebugLabels);
	pCmdWindow->AddCmdProc("ctd",cmdCameraTargetDebug);
	pCmdWindow->AddCmdProc("audiomsg",cmdAudioMsg);
	pCmdWindow->AddCmdProc("logurls",cmdLogUrls);
}

//----------------------------------------------------
