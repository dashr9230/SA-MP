
#include "../main.h"
#include "../game/util.h"
#include "../../raknet/StringCompressor.h"

extern CNetGame*	pNetGame;
extern CGame * pGame;
extern CChatWindow *pChatWindow;
extern RakNetStatisticsStruct RakServerStats;
extern CScoreBoard *pScoreBoard;
extern HANDLE hInstance;

// the download list global lives in main.cpp's data; only the touched fields are known
// the 94-byte record FUNC_10011EB0 reads off the wire
typedef struct _DOWNLOAD_INFO
{
	char  _gap0[7];
	BYTE  field_7;
	DWORD field_8;
	DWORD field_C;
	DWORD dwCrc;
	char  _gap14[0x32];
	DWORD field_46;
	DWORD field_4A;
	DWORD field_4E;
	DWORD field_52;
	BYTE  field_56;
	BYTE  field_57;
	char  _gap58[6];
} DOWNLOAD_INFO;

class CDownloadListDispatch
{
public:

	char _gap0[0x221];
	char field_221;
	// the size is pinned by the `new` in FUNC_10011EB0
	char _gap222[0x1E4B];

	CDownloadListDispatch(); // .text:1000DE20

	void FUNC_1000BD60(); // .text:1000BD60
	void FUNC_1000D4C0(DWORD dwUnk); // .text:1000D4C0
	void FUNC_1000DAE0(DWORD dwUnk, DOWNLOAD_INFO *pInfo); // .text:1000DAE0
	void FUNC_1000DB30(DWORD dwIndex); // .text:1000DB30
	void FUNC_1000C0C0(); // .text:1000C0C0
	void FUNC_1000C140(); // .text:1000C140
	void FUNC_1000CBF0(int a1, int a2, int a3); // .text:1000CBF0
	void FUNC_1000D500(BYTE byteUnk, DWORD dwUnk, PCHAR pData, int iLength); // .text:1000D500
	void FUNC_1000D650(BYTE byteUnk, DWORD dwUnk, PCHAR szName); // .text:1000D650
	void FUNC_1000D750(BYTE byteUnk, DWORD dwUnk); // .text:1000D750
	void FUNC_1000E160(int a1); // .text:1000E160
};

extern CDownloadListDispatch *dword_1026EB98;

// dispatch helpers invoked on the editor globals (thiscall); only the touched fields are known
class CEditDispatch
{
public:

	DWORD field_0;
	char  _gap4[0x7C];
	DWORD field_80;

	void FUNC_10071520(); // .text:10071520
	void FUNC_1006DB80(BOOL bEnable); // .text:1006DB80
	void FUNC_10072BC0(BOOL bEnable); // .text:10072BC0
	void FUNC_100724E0(DWORD dwIndex); // .text:100724E0
	void FUNC_10072420(WORD wObjectID, BOOL bPlayerObject); // .text:10072420
};

extern int dword_1026EB60;
extern int dword_1026EB64;
extern int dword_1026EB68;

// the chat-bubble overlay global also lives in IDirect3DDevice9Hook.cpp
class CChatBubbleDispatch
{
public:

	void FUNC_10066E10(PLAYERID playerId, PCHAR szText, DWORD dwColor, float fDrawDistance, DWORD dwExpireTime); // .text:10066E10
};

extern int dword_1026EB78;

// the dialog global, same story
class CDialogDispatch
{
public:

	void FUNC_1006FFB0(int iDialogID, int iStyle, PCHAR szCaption, PCHAR szInfo,
		PCHAR szButton1, PCHAR szButton2, int iUnk); // .text:1006FFB0
};

extern int dword_1026EB50;

// MATCH
BYTE Checksum(BYTE *pData, WORD wLen)
{
	BYTE byteSum = 0;

	for(DWORD i = 0; i != wLen; i++)
		byteSum ^= pData[i] & 0xCC;

	return byteSum;
}

// MATCH
void LoadModelsThread(void *pParam)
{
	pGame->LoadRequestedModels();
	_endthread();
}

// TODO: these RPCs
void Unk22(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	int iSkill;
	WORD wLevel;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerId);
	bsData.Read(iSkill);
	bsData.Read(wLevel);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if(pPlayerPool)
	{
		if(playerId == pPlayerPool->GetLocalPlayerID())
		{
			CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
			if(pPlayerPed)
				pPlayerPed->SetWeaponSkillLevel(iSkill,wLevel);
		}
		else if(playerId < MAX_PLAYERS)
		{
			if(pPlayerPool->field_2A[playerId])
			{
				CNetPlayer *pPlayer = pPlayerPool->m_pPlayers[playerId];
				if(pPlayer)
				{
					CRemotePlayer *pRemotePlayer = pPlayer->m_pRemotePlayer;
					if(pRemotePlayer)
					{
						if(pRemotePlayer->FUNC_10001080() && pRemotePlayer->m_pPlayerPed)
							pRemotePlayer->m_pPlayerPed->SetWeaponSkillLevel(iSkill,wLevel);
					}
				}
			}
		}
	}
}
void Unk24(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	WORD wLabelID;
	DWORD dwColor;
	float fX, fY, fZ;
	float fDrawDistance;
	WORD wAttachedPlayer;
	WORD wAttachedVehicle;
	char szText[2049];

	CLabelPool *pLabelPool = pNetGame->GetLabelPool();

	if(pLabelPool)
	{
		BYTE byteShowBehindWalls = 0;

		memset(szText,0,sizeof(szText));

		bsData.Read(wLabelID);
		bsData.Read(dwColor);
		bsData.Read(fX);
		bsData.Read(fY);
		bsData.Read(fZ);
		bsData.Read(fDrawDistance);
		bsData.Read(byteShowBehindWalls);
		bsData.Read(wAttachedPlayer);
		bsData.Read(wAttachedVehicle);

		stringCompressor->DecodeString(szText,2048,&bsData);

		if(wLabelID < MAX_LABELS)
			pLabelPool->sub_100011D0(wLabelID,szText,dwColor,fX,fY,fZ,fDrawDistance,
				byteShowBehindWalls,wAttachedPlayer,wAttachedVehicle);
	}
}
// MATCH
void Unk3A(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	WORD wLabelID;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CLabelPool *pLabelPool = pNetGame->GetLabelPool();
	if(!pLabelPool) return;

	bsData.Read(wLabelID);

	if(wLabelID < MAX_LABELS && pLabelPool->m_bLabelSlotState[wLabelID])
		pLabelPool->Delete(wLabelID);
}
void Unk3B(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	DWORD dwColor;
	float fDrawDistance;
	DWORD dwExpireTime;
	BYTE byteTextLen;
	char szText[257];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(pNetGame)
	{
		CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
		if(pPlayerPool)
		{
			memset(szText,0,sizeof(szText));

			bsData.Read(playerId);
			bsData.Read(dwColor);
			bsData.Read(fDrawDistance);
			bsData.Read(dwExpireTime);
			bsData.Read(byteTextLen);

			if(byteTextLen <= 144)
			{
				bsData.Read(szText,byteTextLen);
				szText[byteTextLen] = '\0';

				if(pPlayerPool->GetSlotState(playerId))
					((CChatBubbleDispatch *)dword_1026EB78)->FUNC_10066E10(playerId,szText,dwColor,fDrawDistance,dwExpireTime);
			}
		}
	}
}
void Unk3D(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	short sDialogID;
	BYTE byteStyle;
	BYTE byteLen;
	DWORD dwLen;
	char szCaption[257];
	char szButton2[257];
	char szButton1[257];
	char szInfo[4097];

	memset(szCaption,0,sizeof(szCaption));
	memset(szButton1,0,sizeof(szButton1));
	memset(szButton2,0,sizeof(szButton2));
	memset(szInfo,0,sizeof(szInfo));

	bsData.Read(sDialogID);
	bsData.Read(byteStyle);

	bsData.Read(byteLen);
	bsData.Read(szCaption,byteLen);

	bsData.Read(byteLen);
	bsData.Read(szButton1,byteLen);

	bsData.Read(byteLen);
	bsData.Read(szButton2,byteLen);

	stringCompressor->DecodeString(szInfo,4096,&bsData);

	dwLen = strlen(szCaption);
	if(dwLen)
	{
		dwLen = strlen(szInfo);
		if(dwLen)
			((CDialogDispatch *)dword_1026EB50)->FUNC_1006FFB0(sDialogID,byteStyle,
				szCaption,szInfo,szButton1,szButton2,1);
	}
}
// MATCH
void SetCheckpoint(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	float fX, fY, fZ, fSize;
	VECTOR vecPos, vecExtent;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(fSize);

	vecPos.X = fX;
	vecPos.Y = fY;
	vecPos.Z = fZ;

	vecExtent.X = fSize;
	vecExtent.Y = fSize;
	vecExtent.Z = fSize;

	pGame->SetCheckpointInformation(&vecPos,&vecExtent);
	pGame->m_bCheckpointsEnabled = TRUE;
}
// MATCH
void DisableCheckpoint(RPCParameters *rpcParams)
{
	pGame->m_bCheckpointsEnabled = FALSE;
}
// MATCH
void SetRaceCheckpoint(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE byteType;
	float fX, fY, fZ;
	VECTOR vecPos, vecNext;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteType);

	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);

	vecPos.X = fX;
	vecPos.Y = fY;
	vecPos.Z = fZ;

	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);

	vecNext.X = fX;
	vecNext.Y = fY;
	vecNext.Z = fZ;

	bsData.Read(fX);

	pGame->SetRaceCheckpointInformation(byteType,&vecPos,&vecNext,fX);
	pGame->m_bRaceCheckpointsEnabled = TRUE;
}
// MATCH
void DisableRaceCheckpoint(RPCParameters *rpcParams)
{
	pGame->m_bRaceCheckpointsEnabled = FALSE;
}
void UpdateScoresPingsIPs(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	int iScore;
	int iPing;

	DWORD dwBytes = iBitLength/8;

	RakNet::BitStream bsData(Data,dwBytes+1,false);

	if(dwBytes)
	{
		DWORD dwCount = dwBytes/10;
		CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

		for(WORD i = 0; i < dwCount; i++)
		{
			bsData.Read(playerId);
			bsData.Read(iScore);
			bsData.Read(iPing);

			if(playerId < MAX_PLAYERS)
			{
				if(playerId == pPlayerPool->GetLocalPlayerID())
					pPlayerPool->field_0 = iScore;
				else
				{
					CNetPlayer *pPlayer = pPlayerPool->m_pPlayers[playerId];
					if(pPlayer) pPlayer->field_4 = iScore;
				}

				if(playerId == pPlayerPool->GetLocalPlayerID())
					pPlayerPool->field_22 = iPing;
				else if(playerId <= MAX_PLAYERS)
				{
					CNetPlayer *pPlayer = pPlayerPool->m_pPlayers[playerId];
					if(pPlayer) pPlayer->field_C = iPing;
				}
			}
		}

		if(pScoreBoard)
			pScoreBoard->FUNC_1006ED30();
	}
}
// MATCH
void SvrStats(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read((PCHAR)&RakServerStats,sizeof(RakNetStatisticsStruct));
}
// MATCH
void GameModeRestart(RPCParameters *rpcParams)
{
	pNetGame->sub_1000A540();
}
// MATCH
void ConnectionRejected(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE byteRejectReason;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteRejectReason);

	if(byteRejectReason == 1)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Incorrect Version.");
	}
	else if(byteRejectReason == 2)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Unacceptable NickName");
		pChatWindow->AddInfoMessage("Please choose another nick between and 3-20 characters");
		pChatWindow->AddInfoMessage("Please use only a-z, A-Z, 0-9");
		pChatWindow->AddInfoMessage("Use /quit to exit or press ESC and select Quit Game");
	}
	else if(byteRejectReason == 3)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Bad mod version.");
	}
	else if(byteRejectReason == 4)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Unable to allocate a player slot.");
	}

	pNetGame->GetRakClient()->Disconnect(500,0);
}
// MATCH
void ClientMessage(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	DWORD dwColor;
	DWORD dwStrLen;
	char szMessage[256];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szMessage,0,sizeof(szMessage));

	bsData.Read(dwColor);
	bsData.Read(dwStrLen);

	if(dwStrLen > 255) return;

	bsData.Read(szMessage,dwStrLen);
	szMessage[dwStrLen] = '\0';

	pChatWindow->AddClientMessage(dwColor,szMessage);
}
// MATCH
void WorldTime(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE byteWorldTime;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteWorldTime);

	pNetGame->SetWorldTime(byteWorldTime);
}
// MATCH
void Pickup(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	int iPickup;
	PICKUP Pickup;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(iPickup);
	bsData.Read((PCHAR)&Pickup,sizeof(PICKUP));

	CPickupPool *pPickupPool = pNetGame->GetPickupPool();
	if(pPickupPool)
		pPickupPool->sub_10013270(&Pickup,iPickup);
}
// MATCH
void DestroyPickup(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	int iPickup;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(iPickup);

	CPickupPool *pPickupPool = pNetGame->GetPickupPool();
	if(pPickupPool)
		pPickupPool->sub_10013320(iPickup);
}
// MATCH
void DestroyWeaponPickup(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE bytePickupIndex;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(bytePickupIndex);

	pNetGame->GetPickupPool()->sub_10013380(bytePickupIndex);
}
// MATCH
void ScmEvent(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	int iEvent;
	int iVehicle;
	int iArg1;
	int iArg2;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerId);
	bsData.Read(iEvent);
	bsData.Read(iVehicle);
	bsData.Read(iArg1);
	bsData.Read(iArg2);

	sub_10001A60(playerId,iEvent,iVehicle,iArg1,iArg2);
}
// MATCH
void Weather(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE byteWeather;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteWeather);

	pNetGame->SetWeather(byteWeather);
	pGame->SetWorldWeather(byteWeather);
}
// MATCH
void SetTimeEx(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE byteHour;
	BYTE byteMinute;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteHour);
	bsData.Read(byteMinute);

	pGame->SetWorldTime(byteHour,byteMinute);
	pNetGame->SetWorldTime(byteHour);
	pNetGame->SetWorldMinute(byteMinute);
}
void ToggleClock(RPCParameters *rpcParams) {}
// MATCH
void Unk3C(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	DWORD dwTime = 0;
	bsData.Read(dwTime);

	pGame->SetTimeInMilliseconds(dwTime);
}
void WorldPlayerAdd(RPCParameters *rpcParams) {}
// MATCH
void WorldPlayerDeath(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	bsData.Read(playerId);

	if(pPlayerPool)
	{
		CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(playerId);
		if(pRemotePlayer)
			pRemotePlayer->FUNC_10017570();
	}
}
// MATCH
void WorldPlayerRemove(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	PLAYERID playerId = 0;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	bsData.Read(playerId);

	if(pPlayerPool)
	{
		CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(playerId);
		if(pRemotePlayer)
			pRemotePlayer->FUNC_10017530();
	}
}
void WorldVehicleAdd(RPCParameters *rpcParams) {}
void WorldVehicleRemove(RPCParameters *rpcParams) {}
// MATCH
void DamageVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	VEHICLEID VehicleID;
	int iPanelDamage;
	int iDoorDamage;
	BYTE byteLightDamage;
	BYTE byteTyreDamage;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(VehicleID);
	bsData.Read(iPanelDamage);
	bsData.Read(iDoorDamage);
	bsData.Read(byteLightDamage);
	bsData.Read(byteTyreDamage);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pVehiclePool->GetSlotState(VehicleID))
	{
		CVehicle *pVehicle = (CVehicle *)pVehiclePool->GetAt(VehicleID);
		if(pVehicle)
		{
			pVehicle->sub_100B7AC0(iPanelDamage,iDoorDamage,byteLightDamage);
			pVehicle->sub_100B7940(byteTyreDamage);
		}
	}
}
void Unk18(RPCParameters *rpcParams) {}
// MATCH
void EnterVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	PLAYERID playerId;
	VEHICLEID VehicleID = 0;
	BYTE bytePassenger = 0;
	BOOL bPassenger = FALSE;

	bsData.Read(playerId);
	bsData.Read(VehicleID);
	bsData.Read(bytePassenger);

	if(bytePassenger) bPassenger = TRUE;

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(playerId);
	if(pRemotePlayer)
	{
		if(pRemotePlayer->FUNC_10016120() < 200.0f)
			pRemotePlayer->EnterVehicle(VehicleID,bPassenger);
	}
}
// MATCH
void ExitVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	PLAYERID playerId;
	VEHICLEID VehicleID = 0;

	bsData.Read(playerId);
	bsData.Read(VehicleID);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(playerId);
	if(pRemotePlayer)
	{
		if(pRemotePlayer->FUNC_10016120() < 200.0f)
			pRemotePlayer->ExitVehicle();
	}
}
void ServerJoin(RPCParameters *rpcParams) {}
// MATCH
void ServerQuit(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	BYTE byteReason;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	bsData.Read(playerId);
	bsData.Read(byteReason);

	pPlayerPool->sub_10014090(playerId,byteReason);
}
void InitGame(RPCParameters *rpcParams) {}
// MATCH
void Chat(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	BYTE byteTextLen;
	char szText[256];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(pNetGame->GetGameState() != GAMESTATE_CONNECTED) return;

	memset(szText,0,sizeof(szText));

	bsData.Read(playerId);
	bsData.Read(byteTextLen);
	bsData.Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if(playerId == pPlayerPool->GetLocalPlayerID())
	{
		pChatWindow->AddChatMessage(pPlayerPool->GetLocalPlayerName(),
			pPlayerPool->GetLocalPlayer()->GetPlayerColorAsARGB(),szText);
	}
	else
	{
		CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(playerId);
		if(pRemotePlayer)
			pRemotePlayer->sub_10017610(szText);
	}
}
// MATCH
void RequestClass(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	BYTE byteRequestOutcome = 0;
	SPAWN_INFO SpawnInfo;

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	bsData.Read(byteRequestOutcome);
	bsData.Read((PCHAR)&SpawnInfo,sizeof(SPAWN_INFO));

	if(byteRequestOutcome)
	{
		pLocalPlayer->sub_10003BE0(&SpawnInfo);
		pLocalPlayer->sub_100040E0(TRUE);
	}
	else
	{
		pLocalPlayer->sub_100040E0(FALSE);
	}
}
// MATCH
void RequestSpawn(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	BYTE byteRequestOutcome = 0;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteRequestOutcome);

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	if(byteRequestOutcome == 2 || (byteRequestOutcome && pLocalPlayer->field_302))
		pLocalPlayer->sub_10003C20();
	else
		pLocalPlayer->field_302 = 0;
}
void EditAttachedObject(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	DWORD dwIndex = 0;
	bsData.Read(dwIndex);

	if(dword_1026EB64 && ((CEditDispatch *)dword_1026EB64)->field_0)
		((CEditDispatch *)dword_1026EB64)->FUNC_1006DB80(FALSE);

	((CEditDispatch *)dword_1026EB60)->FUNC_100724E0(dwIndex);
}
void EditObject(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bool bPlayerObject = false;
	WORD wObjectID = 0xFFFF;

	bsData.Read(bPlayerObject);
	bsData.Read(wObjectID);

	if(dword_1026EB64 && ((CEditDispatch *)dword_1026EB64)->field_0)
		((CEditDispatch *)dword_1026EB64)->FUNC_1006DB80(FALSE);

	if(dword_1026EB68 && ((CEditDispatch *)dword_1026EB68)->field_0)
		((CEditDispatch *)dword_1026EB68)->FUNC_10071520();

	if(bPlayerObject)
		((CEditDispatch *)dword_1026EB60)->FUNC_10072420(wObjectID,TRUE);
	else
		((CEditDispatch *)dword_1026EB60)->FUNC_10072420(wObjectID,FALSE);
}
void SelectObject(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(dword_1026EB68 && ((CEditDispatch *)dword_1026EB68)->field_0)
		((CEditDispatch *)dword_1026EB68)->FUNC_10071520();

	if(dword_1026EB60 && ((CEditDispatch *)dword_1026EB60)->field_80)
		((CEditDispatch *)dword_1026EB60)->FUNC_10072BC0(FALSE);

	if(dword_1026EB64)
		((CEditDispatch *)dword_1026EB64)->FUNC_1006DB80(TRUE);
}
void Unk1C(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(dword_1026EB60 && ((CEditDispatch *)dword_1026EB60)->field_80)
		((CEditDispatch *)dword_1026EB60)->FUNC_10072BC0(FALSE);

	if(dword_1026EB64 && ((CEditDispatch *)dword_1026EB64)->field_0)
		((CEditDispatch *)dword_1026EB64)->FUNC_1006DB80(FALSE);
}
void UnkAA(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bool bValue = false;
	bsData.Read(bValue);

	if(pNetGame) pNetGame->field_233 = bValue;
}
void FUNC_10011990(BYTE byteType, DWORD dwValue, BYTE byteResult)
{
	RakNet::BitStream bsSend;

	bsSend.Write(byteType);
	bsSend.Write(dwValue);
	bsSend.Write(byteResult);

	pNetGame->GetRakClient()->RPC(RPC_ClientCheck,&bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,FALSE);
}
void ClientCheck(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	BYTE byteType = 0;
	WORD wOffset = 0;
	WORD wLength = 0;
	BYTE byteSum = 0;
	DWORD dwAddress = 0;

	bsData.Read(byteType);
	bsData.Read(dwAddress);
	bsData.Read(wOffset);
	bsData.Read(wLength);

	if(wOffset > 256 || wLength > 256 || wLength < 2) return;

	if(byteType == 0x46)
	{
		if(IsValidModel(dwAddress))
		{
			DWORD *pModelInfo = GetModelInfo(dwAddress);
			if(pModelInfo)
				byteSum = Checksum((BYTE *)pModelInfo + wOffset,wLength);
		}

		FUNC_10011990(0x46,dwAddress,byteSum);
		return;
	}

	if(byteType == 2)
	{
		if(pGame->FindPlayerPed())
		{
			if(pGame->FindPlayerPed()->IsInVehicle() && !pGame->FindPlayerPed()->FUNC_100ABFC0())
			{
				VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)pGame->FindPlayerPed()->m_pPed->pVehicle;
				if(pVehicle)
				{
					dwAddress = pVehicle->entity.dwPhysFlags;
					byteSum = 2;
				}
			}
			else
			{
				PED_TYPE *pPed = pGame->FindPlayerPed()->m_pPed;
				if(pPed)
				{
					dwAddress = pPed->entity.dwPhysFlags;
					byteSum = 1;
				}
			}
		}

		FUNC_10011990(2,dwAddress,byteSum);
		return;
	}

	if(byteType == 0x47)
	{
		if(IsValidModel(dwAddress))
		{
			int iTries = 0;

			if(!pGame->sub_100A0970(dwAddress))
			{
				pGame->sub_100A0940(dwAddress,2);
				pGame->sub_100A0960();

				while(!pGame->sub_100A0970(dwAddress))
				{
					iTries++;
					if(iTries > 1000) break;
					Sleep(1);
				}
			}

			if(pGame->sub_100A0970(dwAddress))
			{
				DWORD *pModelInfo = FUNC_100B46D0(dwAddress);
				if(pModelInfo)
					byteSum = Checksum((BYTE *)pModelInfo + wOffset,wLength);

				if(!FUNC_100B4700(dwAddress))
					pGame->sub_100A09A0(dwAddress,0);
			}
		}

		FUNC_10011990(0x47,dwAddress,byteSum);
		return;
	}

	if(byteType == 5)
	{
		if(dwAddress >= 0x400000 && dwAddress <= 0x856E00)
		{
			BYTE *pAddress = (BYTE *)dwAddress;
			if(pAddress)
				byteSum = Checksum(pAddress + wOffset,wLength);

			FUNC_10011990(5,dwAddress,byteSum);
		}

		return;
	}

	if(byteType == 0x45)
	{
		if(dwAddress <= 0xC3500)
		{
			BYTE *pAddress = (BYTE *)hInstance + dwAddress;
			if(pAddress)
				byteSum = Checksum(pAddress + wOffset,wLength);

			FUNC_10011990(0x45,dwAddress,byteSum);
		}

		return;
	}

	if(byteType == 0x48)
	{
		FUNC_10011990(0x48,(RakNet::GetTime() & 0xFFFFFFF) | 0x30000000,0);
	}
}
void UnkAB(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	ACTOR_INFO ActorInfo;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CActorPool *pActorPool = pNetGame->GetActorPool();

	if(!pActorPool) return;

	memset(&ActorInfo,0,sizeof(ACTOR_INFO));
	bsData.Read((PCHAR)&ActorInfo,sizeof(ACTOR_INFO));

	if(IsValidPedModel(ActorInfo.iModel))
	{
		pActorPool->sub_10001900(&ActorInfo);

		// retail still does the lookup here even though nothing follows it
		if(!pActorPool->GetAt(ActorInfo.ActorID)) return;
	}
}
// not registered, kept alive only by the rest of the section
void FUNC_1000EBA0(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(dword_1026EB98)
	{
		dword_1026EB98->FUNC_1000BD60();
		dword_1026EB98->FUNC_1000DB30(0);
	}
}
// not registered either
void FUNC_10011EB0(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	DOWNLOAD_INFO Info;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(&Info,0,sizeof(DOWNLOAD_INFO));

	DWORD dwUnk1 = 0;
	DWORD dwUnk2 = 0;

	if(!dword_1026EB98)
	{
		dword_1026EB98 = new CDownloadListDispatch;
		dword_1026EB98->FUNC_1000BD60();
	}

	if(dword_1026EB98)
	{
		bsData.Read(dwUnk1);
		bsData.Read(dwUnk2);
		bsData.Read(Info.field_7);
		bsData.Read(Info.field_8);
		bsData.Read(Info.field_C);
		bsData.Read(Info.dwCrc);
		bsData.Read(Info.field_46);
		bsData.Read(Info.field_4A);
		bsData.Read(Info.field_4E);
		bsData.Read(Info.field_52);
		bsData.Read(Info.field_56);
		bsData.Read(Info.field_57);

		dword_1026EB98->FUNC_1000DAE0(dwUnk1,&Info);
		dword_1026EB98->FUNC_1000D4C0(dwUnk2);
	}
}
// not registered either
void FUNC_100120B0(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	char szName[257];
	char szData[4096];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szName,0,sizeof(szName));

	BYTE byteType = 0;
	BYTE byteUnk1 = 0;
	int iLength = 0;
	DWORD dwUnk2 = 0;
	BYTE byteNameLen = 0;
	BYTE byteUnk3 = 0;
	DWORD dwUnk4 = 0;

	bsData.Read(byteType);

	if(byteType == 0)
	{
		dword_1026EB98->FUNC_1000C140();
		dword_1026EB98->FUNC_1000CBF0(0,6001,0);
	}
	else if(byteType == 2)
	{
		dword_1026EB98->FUNC_1000E160(0);
	}
	else if(byteType == 6)
	{
		bsData.Read(byteUnk3);
		bsData.Read(dwUnk4);
		bsData.Read(byteNameLen);
		bsData.Read(szName,byteNameLen);

		dword_1026EB98->FUNC_1000C140();
		dword_1026EB98->FUNC_1000D650(byteUnk3,dwUnk4,szName);
	}
	else if(byteType == 5)
	{
		bsData.Read(byteUnk3);
		bsData.Read(dwUnk4);

		dword_1026EB98->FUNC_1000C140();
		dword_1026EB98->FUNC_1000D750(byteUnk3,dwUnk4);
	}
	else if(byteType == 1)
	{
		bsData.Read(byteUnk1);
		bsData.Read(dwUnk2);
		bsData.Read(iLength);

		if(iLength >= 0 && iLength <= 4096)
		{
			bsData.Read(szData,iLength);
			dword_1026EB98->FUNC_1000D500(byteUnk1,dwUnk2,szData,iLength);
		}
	}
}
// not registered either
void FUNC_1000EC30(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(dword_1026EB98)
	{
		CDownloadListDispatch *pDownloadList = dword_1026EB98;
		pDownloadList->field_221 = 1;
		pDownloadList->FUNC_1000C0C0();
	}
}
// MATCH
void UnkAC(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	ACTORID ActorID;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(!pActorPool) return;

	bsData.Read(ActorID);

	pActorPool->sub_100016F0(ActorID);
}

//----------------------------------------------------

// MATCH
void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient,Unk22);
	REGISTER_STATIC_RPC(pRakClient,Unk24);
	REGISTER_STATIC_RPC(pRakClient,Unk3A);
	REGISTER_STATIC_RPC(pRakClient,Unk3B);
	REGISTER_STATIC_RPC(pRakClient,Unk3D);
	REGISTER_STATIC_RPC(pRakClient,SetCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,SetRaceCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,DisableRaceCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	REGISTER_STATIC_RPC(pRakClient,SvrStats);
	REGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	REGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	REGISTER_STATIC_RPC(pRakClient,ClientMessage);
	REGISTER_STATIC_RPC(pRakClient,WorldTime);
	REGISTER_STATIC_RPC(pRakClient,Pickup);
	REGISTER_STATIC_RPC(pRakClient,DestroyPickup);
	REGISTER_STATIC_RPC(pRakClient,DestroyWeaponPickup);
	REGISTER_STATIC_RPC(pRakClient,ScmEvent);
	REGISTER_STATIC_RPC(pRakClient,Weather);
	REGISTER_STATIC_RPC(pRakClient,SetTimeEx);
	REGISTER_STATIC_RPC(pRakClient,ToggleClock);
	REGISTER_STATIC_RPC(pRakClient,Unk3C);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerAdd);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerDeath);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerRemove);
	REGISTER_STATIC_RPC(pRakClient,WorldVehicleAdd);
	REGISTER_STATIC_RPC(pRakClient,WorldVehicleRemove);
	REGISTER_STATIC_RPC(pRakClient,DamageVehicle);
	REGISTER_STATIC_RPC(pRakClient,Unk18);
	REGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	REGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	REGISTER_STATIC_RPC(pRakClient,ServerJoin);
	REGISTER_STATIC_RPC(pRakClient,ServerQuit);
	REGISTER_STATIC_RPC(pRakClient,InitGame);
	REGISTER_STATIC_RPC(pRakClient,Chat);
	REGISTER_STATIC_RPC(pRakClient,RequestClass);
	REGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	REGISTER_STATIC_RPC(pRakClient,EditAttachedObject);
	REGISTER_STATIC_RPC(pRakClient,EditObject);
	REGISTER_STATIC_RPC(pRakClient,SelectObject);
	REGISTER_STATIC_RPC(pRakClient,Unk1C);
	REGISTER_STATIC_RPC(pRakClient,UnkAA);
	REGISTER_STATIC_RPC(pRakClient,ClientCheck);
	REGISTER_STATIC_RPC(pRakClient,UnkAB);
	REGISTER_STATIC_RPC(pRakClient,UnkAC);
}

//----------------------------------------------------

// MATCH
void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient,Unk3C);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerAdd);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerDeath);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerRemove);
	UNREGISTER_STATIC_RPC(pRakClient,WorldVehicleAdd);
	UNREGISTER_STATIC_RPC(pRakClient,WorldVehicleRemove);
	UNREGISTER_STATIC_RPC(pRakClient,Unk18);
	UNREGISTER_STATIC_RPC(pRakClient,RequestClass);
	UNREGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	UNREGISTER_STATIC_RPC(pRakClient,DamageVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,Unk22);
	UNREGISTER_STATIC_RPC(pRakClient,Unk24);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3D);
	UNREGISTER_STATIC_RPC(pRakClient,ServerJoin);
	UNREGISTER_STATIC_RPC(pRakClient,ServerQuit);
	UNREGISTER_STATIC_RPC(pRakClient,InitGame);
	UNREGISTER_STATIC_RPC(pRakClient,Chat);
	UNREGISTER_STATIC_RPC(pRakClient,SetCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,SetRaceCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,DisableRaceCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	UNREGISTER_STATIC_RPC(pRakClient,SvrStats);
	UNREGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	UNREGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	UNREGISTER_STATIC_RPC(pRakClient,ClientMessage);
	UNREGISTER_STATIC_RPC(pRakClient,WorldTime);
	UNREGISTER_STATIC_RPC(pRakClient,Pickup);
	UNREGISTER_STATIC_RPC(pRakClient,DestroyPickup);
	UNREGISTER_STATIC_RPC(pRakClient,DestroyWeaponPickup);
	UNREGISTER_STATIC_RPC(pRakClient,ScmEvent);
	UNREGISTER_STATIC_RPC(pRakClient,Weather);
	UNREGISTER_STATIC_RPC(pRakClient,SetTimeEx);
	UNREGISTER_STATIC_RPC(pRakClient,ToggleClock);
	UNREGISTER_STATIC_RPC(pRakClient,EditAttachedObject);
	UNREGISTER_STATIC_RPC(pRakClient,EditObject);
	UNREGISTER_STATIC_RPC(pRakClient,SelectObject);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1C);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAA);
	UNREGISTER_STATIC_RPC(pRakClient,ClientCheck);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAB);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAC);
}

//----------------------------------------------------
