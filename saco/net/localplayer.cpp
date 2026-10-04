
#include "../main.h"
#include "../game/util.h"
#include "../game/keystuff.h"
#include "../game/aimstuff.h"

extern CGame		 *pGame;
extern CNetGame		 *pNetGame;
extern CCmdWindow	 *pCmdWindow;
extern CUnkClass3	 *pUnkClass3;
extern CUnkClass10	 *pUnkClass10;
extern CDXUTDialog	 *pClassSelectDialog;

DWORD dword_100FE0A4 = -1;
DWORD dword_100FE0A8 = 30;
DWORD dword_100FE0AC = 30;
DWORD dword_101189A0;
DWORD dword_101189A4;

//----------------------------------------------------------

// layout locks for the fields split out of the pads
typedef char AssertLocalSize[sizeof(CLocalPlayer) == 0x324 ? 1 : -1];
typedef char AssertField14B[offsetof(CLocalPlayer, field_14B) == 0x14B ? 1 : -1];
typedef char AssertSpawnInfo[offsetof(CLocalPlayer, m_SpawnInfo) == 0x14F ? 1 : -1];
typedef char AssertField314[offsetof(CLocalPlayer, field_314) == 0x314 ? 1 : -1];

//----------------------------------------------------------

CLocalPlayer::CLocalPlayer()
{
	field_147 = GetTickCount();
	field_137 = GetTickCount();
	field_13B = GetTickCount();
	m_bInRCMode = FALSE;
	field_2E2 = 0;
	field_2D6 = GetTickCount();
	field_2F6 = 0;
	m_pPlayerPed = pGame->FindPlayerPed();
	field_F0 = 0;
	field_F4 = 0;
	field_10D = 0;
	field_30A = 0;
	field_10F = GetTickCount();
	field_113 = GetTickCount();
	field_13F = GetTickCount();
	field_117 = field_10F;
	field_11B = field_10F;
	m_bIsSpectating = FALSE;
	field_30F = 0;
	field_310 = -1;
	field_306 = 0;

	ResetAllSyncAttributes();

	BYTE i;
	for (i = 0; i < 13; i++)
	{
		m_byteLastWeapon[i] = 0;
		m_dwLastAmmo[i] = 0;
	}

	m_byteTeam = NO_TEAM;
}

//----------------------------------------------------------

void CLocalPlayer::ResetAllSyncAttributes()
{
	field_17D = 0;
	m_bWantsAnotherClass = FALSE;
	field_302 = 0;
	field_2FE = 0;
	field_1CF = 0;
	field_FA = -1;
	m_bInRCMode = FALSE;
	field_318 = -1;
	field_11F = -1;
	field_121 = -1;
	field_123 = -1;
	memset(field_12B, 0, sizeof(field_12B));
	memset(field_3F, 0, sizeof(field_3F));
	memset(field_5E, 0, sizeof(field_5E));
	//field_92 = 0;
	memset(field_94, 0, sizeof(field_94));
	memset(field_0, 0, sizeof(field_0));
	//memset(field_3C, 0, sizeof(field_3C));
	memset(field_D8, 0, sizeof(field_D8));
	field_FC = 0;
	field_100 = 0;
	field_185 = -1;
	field_187 = -1;
	field_181 = GetTickCount();
	field_189 = 0;

	DWORD dwThisTick = GetTickCount();
	field_2DA = 0;
	field_2DE = 0;
	field_127 = dwThisTick;
}

//----------------------------------------------------------

void CLocalPlayer::Say(PCHAR szText)
{
	BYTE byteTextLength = strlen(szText);
	RakNet::BitStream bsSend;

	bsSend.Write(byteTextLength);
	bsSend.Write(szText, byteTextLength);
	pNetGame->GetRakClient()->RPC(RPC_Chat, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE);
}

//----------------------------------------------------------

void CLocalPlayer::SetPlayerColor(DWORD dwColor)
{
	SetRadarColor(pNetGame->GetPlayerPool()->GetLocalPlayerID(),dwColor);
}

//----------------------------------------------------

DWORD CLocalPlayer::GetPlayerColorAsRGBA()
{
	return TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID());
}

//----------------------------------------------------

DWORD CLocalPlayer::GetPlayerColorAsARGB()
{
	return (TranslateColorCodeToRGBA(pNetGame->GetPlayerPool()->GetLocalPlayerID()) >> 8) | 0xFF000000;
}

//----------------------------------------------------

//----------------------------------------------------

// the damage reports scale the amount down to a third before sending
void CLocalPlayer::FUNC_100068A0(WORD a1, float a2, int a3, int a4)
{
	a2 *= 0.33f;

	RakNet::BitStream bsSend;

	bsSend.Write1();
	bsSend.Write(a1);
	bsSend.Write(a2);
	bsSend.Write(a3);
	bsSend.Write(a4);
	pNetGame->GetRakClient()->RPC(RPC_PlayerDamage, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
}

//----------------------------------------------------

void CLocalPlayer::FUNC_100069B0(WORD a1, float a2, int a3, int a4)
{
	a2 *= 0.33f;

	RakNet::BitStream bsSend;

	bsSend.Write0();
	bsSend.Write(a1);
	bsSend.Write(a2);
	bsSend.Write(a3);
	bsSend.Write(a4);
	pNetGame->GetRakClient()->RPC(RPC_PlayerDamage, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
}

//----------------------------------------------------

void CLocalPlayer::FUNC_10006AC0(WORD a1, float a2, int a3, int a4)
{
	RakNet::BitStream bsSend;

	bsSend.Write0();
	bsSend.Write(a1);
	bsSend.Write(a2);
	bsSend.Write(a3);
	bsSend.Write(a4);
	pNetGame->GetRakClient()->RPC(RPC_ActorDamage, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
}

//----------------------------------------------------

void CLocalPlayer::RequestClass(int iClass)
{
	RakNet::BitStream bsSend;

	bsSend.Write(iClass);
	pNetGame->GetRakClient()->RPC(RPC_RequestClass, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE);
}

//----------------------------------------------------

void CLocalPlayer::SendStatsUpdate()
{
	RakNet::BitStream bsSend;

	int iMoney = pGame->FUNC_100A0F90();
	int iDrunkLevel = m_pPlayerPed->sub_100ADFA0();

	bsSend.Write((BYTE)ID_STATS_UPDATE);
	bsSend.Write(iMoney);
	bsSend.Write(iDrunkLevel);
	pNetGame->GetRakClient()->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE, 0);
}

//----------------------------------------------------

// class selection cycles field_2FE and re-requests on every step
void CLocalPlayer::SelectNextClass()
{
	if(!field_306)
		return;

	MATRIX4X4 mat;

	field_143 = 0;
	m_pPlayerPed->GetMatrix(&mat);

	if(field_2FE == pNetGame->GetSettings()->field_23 - 1)
		field_2FE = 0;
	else
		field_2FE++;

	pGame->m_pGameAudio->PlaySound(1052, mat.pos.X, mat.pos.Y, mat.pos.Z);
	RequestClass(field_2FE);
}

//----------------------------------------------------

void CLocalPlayer::SelectPreviousClass()
{
	if(!field_306)
		return;

	MATRIX4X4 mat;

	field_143 = 0;
	m_pPlayerPed->GetMatrix(&mat);

	DWORD dwClass = field_2FE;
	if(!dwClass)
		dwClass = pNetGame->GetSettings()->field_23;

	field_2FE = dwClass - 1;
	pGame->m_pGameAudio->PlaySound(1053, mat.pos.X, mat.pos.Y, mat.pos.Z);
	RequestClass(field_2FE);
}

//----------------------------------------------------

// leaving a vehicle; every guard shares one exit so the nesting has to stay
void CLocalPlayer::FUNC_10005BF0(WORD a1)
{
	RakNet::BitStream bsSend;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(a1 < MAX_VEHICLES && pVehiclePool->field_3074[a1])
	{
		int pVehicle = pVehiclePool->field_1134[a1];
		if(pVehicle)
		{
			if(!m_pPlayerPed->FUNC_100ABFC0())
				field_FA = a1;

			if(((CVehicle *)pVehicle)->FUNC_100B7460())
				pGame->m_pGameCamera->FUNC_1009D740();

			if(!((CVehicle *)pVehicle)->FUNC_100B7E00())
			{
				bsSend.Write(a1);
				pNetGame->GetRakClient()->RPC(RPC_ExitVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE);
			}
		}
	}
}

//----------------------------------------------------

// entering a vehicle; the camera hand-off only runs for the vehicles that ask
void CLocalPlayer::FUNC_10005AD0(WORD a1, int a2)
{
	RakNet::BitStream bsSend;

	BYTE byteFlag = 0;

	if(a2)
		byteFlag = 1;

	bsSend.Write(a1);
	bsSend.Write(byteFlag);
	pNetGame->GetRakClient()->RPC(RPC_EnterVehicle, &bsSend, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(a1 < MAX_VEHICLES && pVehiclePool->field_3074[a1])
	{
		CVehicle *pVehicle = (CVehicle *)pVehiclePool->field_1134[a1];
		if(pVehicle && pVehicle->FUNC_100B7460())
		{
			DWORD dwGtaId = pVehicle->m_dwGTAId;

			ScriptCommand(&camera_on_vehicle, dwGtaId, 3, 2);
			dword_100FE0A4 = GetTickCount();
		}
	}
}

//----------------------------------------------------

// the enter-vehicle key handler; 46 is the parachute, which has to be put away
void CLocalPlayer::FUNC_10006FE0()
{
	GTA_CONTROLSET *pKeys = GameGetInternalKeys();
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pKeys->wKeys1[8] && !pKeys->wKeys2[8])
	{
		WORD wVehicleId = pVehiclePool->FUNC_1001EC00();
		if(wVehicleId < MAX_VEHICLES && pVehiclePool->field_3074[wVehicleId])
		{
			CVehicle *pVehicle = (CVehicle *)pVehiclePool->FUNC_10001120(wVehicleId);
			if(pVehicle->FUNC_1009F0C0() < 8.0f)
			{
				if(m_pPlayerPed->FUNC_100ABC50() == 46)
					m_pPlayerPed->SetArmedWeapon(0, 0);

				m_pPlayerPed->FUNC_100AC410(pVehicle->m_dwGTAId, 1);
				FUNC_10005AD0(wVehicleId, 1);
			}
		}
	}
}

//----------------------------------------------------

void CLocalPlayer::sub_10003710(int a1, int a2)
{
	field_2DA = a1;
	field_2DE = a2;
}

//----------------------------------------------------

void CLocalPlayer::sub_10004060()
{
	if(field_306)
	{
		RequestSpawn();
		field_302 = 1;
	}
}

//----------------------------------------------------

// on foot and in a vehicle report their update rate separately
int CLocalPlayer::FUNC_10003B30()
{
	if(m_pPlayerPed)
	{
		if(m_pPlayerPed->IsInVehicle())
			return FUNC_10003AB0();

		return FUNC_10003AF0();
	}

	return 1000;
}

//----------------------------------------------------

// the three words live inside field_0, so they are read through it
BOOL CLocalPlayer::FUNC_10004340(WORD a1, WORD a2, WORD a3)
{
	if(a1 == *(WORD *)&field_0[6] && a2 == *(WORD *)&field_0[4] && a3 == *(WORD *)&field_0[2])
		return FALSE;

	return TRUE;
}

//----------------------------------------------------

// the sync interval, 15ms while the server asks for it and 30ms otherwise
int CLocalPlayer::FUNC_10003AB0()
{
	if(m_pPlayerPed)
	{
		if(pNetGame->GetField239())
			return 15;

		return dword_100FE0AC + pGame->FUNC_100A00F0();
	}

	return 1000;
}

//----------------------------------------------------

int CLocalPlayer::FUNC_10003AF0()
{
	if(m_pPlayerPed)
	{
		if(pNetGame->GetField239())
			return 15;

		return dword_100FE0A8 + pGame->FUNC_100A00F0();
	}

	return 1000;
}

//----------------------------------------------------

BOOL CLocalPlayer::FUNC_10004300(WORD a1, WORD a2, WORD a3)
{
	if(a1 == *(WORD *)&field_94[4] && a2 == *(WORD *)&field_94[2] && a3 == *(WORD *)&field_94[0])
		return FALSE;

	return TRUE;
}

//----------------------------------------------------

PLAYERID CLocalPlayer::FUNC_10004B70()
{
	if(!m_pPlayerPed)
		return -1;

	int iPed = m_pPlayerPed->FUNC_100AEF60();
	if(!iPed)
		return -1;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(!pPlayerPool)
		return -1;

	return pPlayerPool->FUNC_100138C0(iPed);
}

//----------------------------------------------------

ACTORID CLocalPlayer::FUNC_10004BB0()
{
	if(!m_pPlayerPed)
		return -1;

	int iPed = m_pPlayerPed->FUNC_100AEF60();
	if(!iPed)
		return -1;

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(!pActorPool)
		return -1;

	return pActorPool->FUNC_100018B0(iPed);
}

//----------------------------------------------------

// only accepts a spawn point the game says is inside the world
void CLocalPlayer::sub_10003BE0(SPAWN_INFO *pSpawnInfo)
{
	if(FUNC_100B4B50(&pSpawnInfo->vecPos))
	{
		memcpy(&m_SpawnInfo, pSpawnInfo, sizeof(SPAWN_INFO));
		field_17D = 1;
	}
}

//----------------------------------------------------

void CLocalPlayer::sub_100040E0(BOOL bSpawn)
{
	if(bSpawn)
	{
		if(m_pPlayerPed)
		{
			m_pPlayerPed->FUNC_100AC010();
			m_pPlayerPed->SetSkin(m_SpawnInfo.iSkin);
		}

		field_143 = 1;
	}
}

//----------------------------------------------------

void CLocalPlayer::FUNC_10004080()
{
	field_143 = 0;

	if(m_pPlayerPed)
	{
		m_pPlayerPed->FUNC_100ABBD0();
		m_pPlayerPed->FUNC_100ABD70(100.0f);
		m_pPlayerPed->FUNC_100AC790(0);
	}

	field_14B = GetTickCount();
	field_147 = GetTickCount();
}

//----------------------------------------------------

// resends at most twice a second unless the block actually changed
BOOL CLocalPlayer::FUNC_10003A60(void *a2, void *a3, unsigned int a4)
{
	if(GetTickCount() - field_13F > 500)
	{
		field_13F = GetTickCount();
		return TRUE;
	}

	if(memcmp(a2, a3, a4))
	{
		field_13F = GetTickCount();
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------

void CLocalPlayer::FUNC_10004230(WORD a1)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pVehiclePool && a1 < MAX_VEHICLES && pVehiclePool->field_3074[a1])
	{
		field_30F = 2;
		field_310 = a1;
		field_314 = 0;
	}
}

//----------------------------------------------------

void CLocalPlayer::RequestSpawn()
{
	RakNet::BitStream bsSend;

	pNetGame->GetRakClient()->RPC(RPC_RequestSpawn, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE);
}

//----------------------------------------------------

// straying outside the server boundary disarms and nags
void CLocalPlayer::FUNC_10003F50()
{
	if(!pGame->FUNC_100A0EE0() && m_pPlayerPed->FUNC_1009F420(pNetGame->GetSettings()->fWorldBoundryPX,
		pNetGame->GetSettings()->fWorldBoundryZX, pNetGame->GetSettings()->fWorldBoundryPY,
		pNetGame->GetSettings()->fWorldBoundryNY))
	{
		m_pPlayerPed->SetArmedWeapon(0, 0);
		pGame->DisplayGameText("Stay within the ~r~world boundries", 1000, 5);
	}
}

//----------------------------------------------------

// leaving spectate only resets the spawn state when another class is pending
void CLocalPlayer::FUNC_100041C0(BOOL bSpectating)
{
	if(m_bIsSpectating && !m_bWantsAnotherClass && !bSpectating)
		sub_10003C20();
	else if(m_bWantsAnotherClass && !bSpectating)
	{
		field_F0 = 0;
		field_17D = 0;
	}

	m_bIsSpectating = bSpectating;
	field_30F = 0;
	field_314 = 0;
	field_310 = -1;
}

//----------------------------------------------------

// picks a player to spectate; state 32 is not a spectatable one
void CLocalPlayer::FUNC_10004280(PLAYERID playerId)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if(pPlayerPool && pPlayerPool->GetSlotState(playerId) && pPlayerPool->GetAt(playerId)->field_10A
		&& pPlayerPool->GetAt(playerId)->field_10A != 32)
	{
		field_30F = 1;
		field_310 = playerId;
		field_314 = 0;
	}
}

//----------------------------------------------------

// the in-vehicle half of the boundary check, keyed off the driven vehicle
void CLocalPlayer::FUNC_10003FC0()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	WORD wVehicleId = pVehiclePool->FUNC_1001EB90((int)m_pPlayerPed->GetGtaVehicle());

	if(!pGame->FUNC_100A0EE0() && wVehicleId != INVALID_VEHICLE_ID && wVehicleId < MAX_VEHICLES
		&& pVehiclePool->field_3074[wVehicleId])
	{
		CVehicle *pVehicle = (CVehicle *)pVehiclePool->field_1134[wVehicleId];

		if(pVehicle && pVehicle->FUNC_1009F420(pNetGame->GetSettings()->fWorldBoundryPX,
			pNetGame->GetSettings()->fWorldBoundryZX, pNetGame->GetSettings()->fWorldBoundryPY,
			pNetGame->GetSettings()->fWorldBoundryNY))
			pGame->DisplayGameText("Stay within the ~r~world boundries", 1000, 5);
	}
}

//----------------------------------------------------

// counts the spawned players, recomputed once every 30 calls
int FUNC_10004120()
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	WORD i = 0;

	if(dword_101189A4)
	{
		dword_101189A4--;
		return dword_101189A0;
	}

	int iCount = 0;

	dword_101189A4 = 30;
	dword_101189A0 = 0;

	if(pPlayerPool)
	{
		int iMax = pPlayerPool->field_2F3A;

		for(; i <= iMax; i++)
		{
			if(pPlayerPool->GetSlotState(i))
			{
				CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(i);

				if(pRemotePlayer->m_pPlayerPed && pRemotePlayer->field_10A)
					dword_101189A0 = ++iCount;
			}
		}
	}

	return iCount;
}

//----------------------------------------------------

// reports the death reason and who did it
void CLocalPlayer::SendDeath()
{
	RakNet::BitStream bsSend;
	WORD wKiller;

	BYTE byteReason = m_pPlayerPed->FUNC_100AC850(&wKiller);

	bsSend.Write(byteReason);
	bsSend.Write(wKiller);
	pNetGame->GetRakClient()->RPC(RPC_Death, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
}

//----------------------------------------------------

void CLocalPlayer::SetInteriorId(BYTE byteInteriorId)
{
	field_1CF = byteInteriorId;

	RakNet::BitStream bsSend;

	bsSend.Write(byteInteriorId);
	pNetGame->GetRakClient()->RPC(RPC_SetInteriorId, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE);
}

//----------------------------------------------------

// drives class selection: centres the dialog on entry, then steps on the arrows
void CLocalPlayer::FUNC_10006100()
{
	if(!pGame->sub_100A0920())
	{
		pGame->sub_100A1DB0(0);
		pGame->ToggleKeyInputsDisabled(2, 0);
	}

	if(pUnkClass3 && pUnkClass3->GetField28())
		return;

	if(!field_306)
	{
		RECT rect;

		pClassSelectDialog->SetVisible(true);
		GetClientRect(pGame->GetMainWindowHwnd(), &rect);
		pClassSelectDialog->SetLocation(rect.right / 2 - pClassSelectDialog->GetWidth() / 2,
			rect.bottom - pClassSelectDialog->GetHeight() - 50);

		field_306 = 1;
		RequestClass(field_2FE);
		return;
	}

	if(GetTickCount() - field_14B < 2000)
		return;

	if(!field_302)
	{
		if(!field_143)
			return;

		if((GetAsyncKeyState(VK_SHIFT) & 0x8000) && !pCmdWindow->GetField14E0())
		{
			sub_10004060();
			return;
		}
	}

	if(!field_143)
		return;

	DWORD dwElapsed = GetTickCount() - field_147;

	if((GetAsyncKeyState(VK_LEFT) & 0x8000) && dwElapsed > 250)
	{
		field_147 = GetTickCount();
		SelectPreviousClass();
		return;
	}

	if((GetAsyncKeyState(VK_RIGHT) & 0x8000) && dwElapsed > 250)
	{
		field_147 = GetTickCount();
		SelectNextClass();
	}
}

//----------------------------------------------------

// keeps the FollowPedSA marker on the aim point, refreshed five times a second
void CLocalPlayer::FUNC_10002FA0()
{
	if(m_pPlayerPed && (!pGame || pGame->m_bHeadMove) && !m_pPlayerPed->FUNC_100ADC90()
		&& (!pUnkClass10 || !pUnkClass10->GetField80()))
	{
		MATRIX4X4 mat;

		m_pPlayerPed->GetMatrix(&mat);

		DWORD dwTick = GetTickCount();

		if(dwTick - field_137 > 1000)
		{
			CAMERA_AIM *pAim = GameGetInternalAim();

			float fX = pAim->f1x * 20.0f + mat.pos.X;
			float fY = pAim->f1y * 20.0f + mat.pos.Y;
			float fZ = pAim->f1z * 20.0f + mat.pos.Z;

			field_137 = dwTick;
			*(float *)&field_12B[0] = fX - mat.pos.X;
			*(float *)&field_12B[4] = fY - mat.pos.Y;
			*(float *)&field_12B[8] = fZ - mat.pos.Z;
		}

		if(dwTick - field_13B > 200)
		{
			VECTOR vecTarget;

			vecTarget.X = mat.pos.X + *(float *)&field_12B[0];
			vecTarget.Y = mat.pos.Y + *(float *)&field_12B[4];
			vecTarget.Z = mat.pos.Z + *(float *)&field_12B[8];

			m_pPlayerPed->FUNC_100ADFD0("FollowPedSA", 0, 2000, -1, &vecTarget, 0, 0.1f, 500, 3, 0);
			field_13B = dwTick;
		}
	}
}

//----------------------------------------------------

// the special action code the sync packets carry; 5 to 8 are the dance styles
BYTE CLocalPlayer::GetSpecialAction()
{
	if(m_pPlayerPed->FUNC_100ACDC0())
		return 2;

	if(m_pPlayerPed->FUNC_100ADB60())
	{
		switch(m_pPlayerPed->m_iDanceStyle)
		{
		case 0:
			return 5;
		case 1:
			return 6;
		case 2:
			return 7;
		case 3:
			return 8;
		}
	}

	if(m_pPlayerPed->FUNC_100ACB60())
		return 10;

	if(m_pPlayerPed->FUNC_100AE0A0())
		return 11;

	if(m_pPlayerPed->FUNC_100AE260())
		return 68;

	if(m_pPlayerPed->FUNC_100AC690())
		return 1;

	if(m_pPlayerPed->FUNC_100AC5D0())
		return 3;

	if(m_pPlayerPed->FUNC_100AC640())
		return 4;

	if(m_pPlayerPed->FUNC_100ADC90())
	{
		if(m_pPlayerPed->FUNC_100ADC90() == 1)
			return 20;

		if(m_pPlayerPed->FUNC_100ADC90() == 2)
			return 22;

		if(m_pPlayerPed->FUNC_100ADC90() == 3)
			return 23;

		if(m_pPlayerPed->FUNC_100ADC90() == 4)
			return 21;
	}

	if(m_pPlayerPed->FUNC_100ACEF0())
		return 24;

	BYTE byteAction = m_pPlayerPed->FUNC_100ACE40() ? 25 : 0;

	return byteAction;
}
