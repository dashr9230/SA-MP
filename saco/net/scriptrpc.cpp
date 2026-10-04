
#include "../main.h"
#include "../game/util.h"

using namespace RakNet;
extern CNetGame*	pNetGame;
extern CGame * pGame;
extern CAudioStream * pAudioStream;
extern CChatWindow * pChatWindow;
extern CDownloadList * dword_1026EB98;

// the three UI dispatch objects the script RPCs poke; net/netrpc.cpp keeps its own
// copy of this shape as CEditDispatch
#pragma pack(1)
class CScrEditDispatch
{
public:
	DWORD field_0;
	char  _gap4[0x7C];
	DWORD field_80;

	void FUNC_1006DB80(BOOL bEnable); // .text:1006DB80
	void FUNC_10071440(int a1); // .text:10071440
	void FUNC_10071520(); // .text:10071520
	void FUNC_10072BC0(BOOL bEnable); // .text:10072BC0
};

extern CScrEditDispatch * dword_1026EB60;
extern CScrEditDispatch * dword_1026EB64;
extern CScrEditDispatch * dword_1026EB68;

char pText[257];

//----------------------------------------------------

// TODO: These script RPCs
void ScrUnkA7(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	bool bValue;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bValue = false;
	bsData.Read(bValue);

	if(pNetGame) pNetGame->SetField232(bValue);
}
void ScrUnk38(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteIndex;
	VECTOR vecPos;
	BYTE byteIcon;
	DWORD dwColor;
	BYTE byteStyle;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(byteIndex);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(byteIcon);
	bsData.Read(dwColor);
	bsData.Read(byteStyle);

	pNetGame->SetMapIcon(byteIndex, vecPos.X, vecPos.Y, vecPos.Z, byteIcon, dwColor, byteStyle);
}
void ScrUnk90(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteIndex;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteIndex);

	pNetGame->DisableMapIcon(byteIndex);
}
void ScrUnk91(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	BYTE byteWeapon;
	WORD wAmmo;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteWeapon);
	bsData.Read(wAmmo);

	pLocalPlayer->GetPlayerPed()->SetWeaponAmmo(byteWeapon, wAmmo);
}
void ScrSetGravity(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	float fGravity;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fGravity);

	pGame->SetGravity(fGravity);
}
void ScrUnk93(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(iUnk);

	if(pNetGame->GetVehiclePool()) {
		if(pNetGame->GetVehiclePool()->GetSlotState(vehicleId)) {
			CVehicle *pVehicle = (CVehicle *)pNetGame->GetVehiclePool()->GetAt(vehicleId);
			if(pVehicle) pVehicle->FUNC_100B72C0(iUnk);
		}
	}
}
void ScrUnk94(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	VEHICLEID trailerId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(!pNetGame->GetVehiclePool()) return;

	bsData.Read(vehicleId);
	bsData.Read(trailerId);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	int iVehicle = pVehiclePool->GetAt(vehicleId);
	CVehicle *pTrailer = (CVehicle *)pVehiclePool->GetAt(trailerId);

	if(iVehicle) {
		if(pTrailer) {
			pTrailer->FUNC_100B7C80(iVehicle);
			pTrailer->FUNC_100B7C10();
		}
	}
}
void ScrUnk95(RPCParameters *rpcParams) {}
// the object at 0x10053C30; only the bitstream string reader this file needs
class CScrStringReader
{
public:
	bool FUNC_100542D0(char *szBuffer, int iMaxLen, RakNet::BitStream *pData, int a4); // .text:100542D0
};

CScrStringReader * FUNC_10053C30(); // .text:10053C30

// walks byteCount material entries off the wire and applies them to pObject
void FUNC_1001B070(BYTE byteCount, CObject *pObject, RakNet::BitStream *pData)
{
	BYTE byteLen;
	BYTE byteIndex;
	BYTE byteType;
	WORD wModel;
	DWORD dwColour;
	int iCount;
	DWORD dwFontColour;
	BYTE byteSize;
	BYTE byteAlign;
	BYTE byteBold;
	BYTE byteUnk;
	DWORD dwBackColour;
	int iTextLen;
	char szTxd[257];
	char szFont[257];
	char szTexture[257];
	char szText[2049];

	int iEntries = byteCount;

	dwColour = 0;
	byteLen = 0;

	if(iEntries) {
		iCount = iEntries;

		do {
			memset(szTxd, 0, sizeof(szTxd));
			memset(szTexture, 0, sizeof(szTexture));
			memset(szFont, 0, sizeof(szFont));
			memset(szText, 0, sizeof(szText));

			pData->Read(byteType);

			if(byteType == 1) {
				pData->Read(byteIndex);
				pData->Read(wModel);
				pData->Read(byteLen);
				pData->Read(szTxd, byteLen);
				pData->Read(byteLen);
				pData->Read(szTexture, byteLen);
				pData->Read(dwColour);

				if(strlen(szTxd) < 32 && strlen(szTexture) < 32) {
					int iModel = wModel;
					if(iModel == 0xFFFF || iModel > 20000) iModel = -1;
					if(pObject) pObject->FUNC_100A8530(iModel, byteIndex, szTxd, szTexture, dwColour);
				}
			} else if(byteType == 2) {
				pData->Read(byteIndex);
				pData->Read(byteSize);
				pData->Read(byteLen);
				pData->Read(szFont, byteLen);
				pData->Read(byteBold);
				pData->Read(byteAlign);
				pData->Read(dwFontColour);
				pData->Read(dwBackColour);
				pData->Read(byteUnk);

				FUNC_10053C30()->FUNC_100542D0(szText, 2048, pData, 0);

				if(strlen(szFont) < 32) {
					iTextLen = strlen(szText);
					if(iTextLen) {
						if(pObject) pObject->FUNC_100A86B0(byteIndex, szText, byteSize, szFont,
							byteBold, byteAlign, dwFontColour, dwBackColour, byteUnk);
					}
				}
			}
		} while(--iCount);
	}
}
void ScrUnk2C(RPCParameters *rpcParams) {}
void ScrUnk2D(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD objectId;
	float fX, fY, fZ;
	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(objectId);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(iUnk);

	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	if(objectId <= MAX_OBJECTS) {
		if(pObjectPool->field_4[objectId]) {
			CEntity *pObject = (CEntity *)pObjectPool->field_FA4[objectId];
			if(pObject) pObject->TeleportTo(fX, fY, fZ);
		}
	}
}
void ScrUnk2E(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD objectId;
	VECTOR vecPos;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(objectId);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);

	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	if(pObjectPool && objectId <= MAX_OBJECTS && pObjectPool->field_4[objectId]
		&& pObjectPool->field_FA4[objectId]) {
		((CObject *)pObjectPool->field_FA4[objectId])->FUNC_100A80A0(&vecPos);
	}
}
void ScrUnk2F(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD objectId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(objectId);

	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	if(objectId <= MAX_OBJECTS && pObjectPool->field_4[objectId]) {
		int iObject = pObjectPool->field_FA4[objectId];
		if(iObject) {
			if(pLocalPlayer && pLocalPlayer->field_2DA == (DWORD)iObject) {
				pLocalPlayer->field_2DA = 0;
				pLocalPlayer->field_2E2 = 0;
			}
			pObjectPool->FUNC_10012850(objectId);
		}
	}
}
void ScrUnk4F(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	float fX, fY, fZ, fUnk;
	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(iUnk);
	bsData.Read(fUnk);

	ScriptCommand(&script_command_0948, fX, fY, fZ, iUnk, fUnk);
}
void ScrUnk50(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(playerId);
	bsData.Read(byteUnk);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerId)) {
		pNetGame->GetPlayerPool()->GetAt(playerId)->FUNC_10017C80(byteUnk);
	}
}
void ScrUnk63(RPCParameters *rpcParams) {}
void ScrUnk7A(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD objectId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(objectId);

	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	if(objectId <= MAX_OBJECTS) {
		if(pObjectPool->field_4[objectId]) {
			CObject *pObject = (CObject *)pObjectPool->field_FA4[objectId];
			if(pObject) pObject->FUNC_100A8030();
		}
	}
}
void ScrUnk7B(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	BYTE byteLen;
	char szText[33];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	byteLen = 0;

	memset(szText, 0, sizeof(szText));

	bsData.Read(vehicleId);
	bsData.Read(byteLen);

	if(byteLen <= 32) {
		bsData.Read(szText, byteLen);

		CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
		if(pVehiclePool) {
			if(pVehiclePool->GetSlotState(vehicleId)) {
				((CVehicle *)pVehiclePool->GetAt(vehicleId))->FUNC_100B8150(szText);
			}
		}
	}
}
void ScrUnk7C(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iUnk;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(iUnk);

	pPlayerPool->GetLocalPlayer()->FUNC_100041C0(iUnk);
}

void ScrUnk7D(RPCParameters *rpcParams)
{
	// nothing
}

void ScrUnk7E(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteMode;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(playerId);
	bsData.Read(byteMode);

	switch(byteMode) {
		case 2: byteMode = 15; break;
		case 3: byteMode = 14; break;
		default: byteMode = 4; break;
	}

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	pLocalPlayer->field_30E = byteMode;
	pLocalPlayer->FUNC_10004280(playerId);
}
void ScrUnk7F(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteMode;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(playerId);
	bsData.Read(byteMode);

	switch(byteMode) {
		case 2: byteMode = 15; break;
		case 3: byteMode = 14; break;
		default: byteMode = 3; break;
	}

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	pLocalPlayer->field_30E = byteMode;
	pLocalPlayer->FUNC_10004230(playerId);
}
void ScrUnk39(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	short sUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(!pVehiclePool) return;

	bsData.Read(vehicleId);
	bsData.Read(sUnk);

	if(pVehiclePool->GetSlotState(vehicleId))
		((CVehicle *)pVehiclePool->GetAt(vehicleId))->FUNC_100B80B0(sUnk);
}
void ScrUnk4B(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD objectId;
	PLAYERID playerId;
	float fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ;
	CPlayerPed *pPlayerPed;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(objectId);
	bsData.Read(playerId);
	bsData.Read(fOffsetX);
	bsData.Read(fOffsetY);
	bsData.Read(fOffsetZ);
	bsData.Read(fRotX);
	bsData.Read(fRotY);
	bsData.Read(fRotZ);

	CObjectPool *pObjectPool = pNetGame->FUNC_10002E10();
	if(objectId > MAX_OBJECTS) return;
	if(!pObjectPool->field_4[objectId]) return;
	CObject *pObject = (CObject *)pObjectPool->field_FA4[objectId];
	if(!pObject) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	if(playerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPed = pPlayerPool->GetLocalPlayer()->GetPlayerPed();
	} else {
		if(playerId > MAX_PLAYERS) return;
		if(!pPlayerPool->m_pPlayers[playerId]) return;
		if(!pPlayerPool->m_pPlayers[playerId]->m_pRemotePlayer) return;
		pPlayerPed = pPlayerPool->m_pPlayers[playerId]->m_pRemotePlayer->m_pPlayerPed;
	}

	ScriptCommand(&attach_object_to_player, pObject->field_44, pPlayerPed->m_dwGTAId,
		fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ);
}
void ScrUnk85(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteLevel;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(!pGame) return;

	bsData.Read(byteLevel);
	pGame->SetWantedLevel(byteLevel);
}
void ScrUnk86(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wTextDrawId;
	WORD wTextLen;
	char szTransmit[64];
	char szText[800];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	wTextLen = 0;

	CTextDrawPool *pTextDrawPool = pNetGame->GetTextDrawPool();
	if(pTextDrawPool) {
		bsData.Read(wTextDrawId);
		bsData.Read(szTransmit, 63);
		bsData.Read(wTextLen);
		if(wTextLen >= 800) {
			if(pChatWindow) pChatWindow->AddDebugMessage("Warning: ignoring large TextDraw size=%u", wTextLen);
		} else {
			bsData.Read(szText, wTextLen);
			szText[wTextLen] = 0;
			pTextDrawPool->FUNC_1001E910(wTextDrawId, szTransmit, szText);
		}
	}
}
void ScrUnk87(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wTextDrawId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CTextDrawPool *pTextDrawPool = pNetGame->GetTextDrawPool();
	if(pTextDrawPool) {
		bsData.Read(wTextDrawId);
		pTextDrawPool->FUNC_1001E7F0(wTextDrawId);
	}
}
void ScrUnk69(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wTextDrawId;
	WORD wTextLen;
	char szText[800];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	wTextLen = 0;

	CTextDrawPool *pTextDrawPool = pNetGame->GetTextDrawPool();
	if(pTextDrawPool) {
		bsData.Read(wTextDrawId);
		bsData.Read(wTextLen);
		if(wTextLen <= 800) {
			bsData.Read(szText, wTextLen);
			szText[wTextLen] = 0;
			CTextDraw *pTextDraw = (CTextDraw *)pTextDrawPool->GetAt(wTextDrawId);
			if(pTextDraw) pTextDraw->FUNC_100B2F60(szText);
		}
	}
}
void ScrAddGangZone(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wZone;
	float fMinX, fMinY, fMaxX, fMaxY;
	DWORD dwColor;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CGangZonePool *pGangZonePool = pNetGame->GetGangZonePool();
	if(pGangZonePool) {
		bsData.Read(wZone);
		bsData.Read(fMinX);
		bsData.Read(fMinY);
		bsData.Read(fMaxX);
		bsData.Read(fMaxY);
		bsData.Read(dwColor);
		pGangZonePool->New(wZone, fMinX, fMinY, fMaxX, fMaxY, dwColor);
	}
}
void ScrRemoveGangZone(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wZone;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CGangZonePool *pGangZonePool = pNetGame->GetGangZonePool();
	if(pGangZonePool) {
		bsData.Read(wZone);
		pGangZonePool->Delete(wZone);
	}
}
void ScrFlashGangZone(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wZone;
	DWORD dwColor;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CGangZonePool *pGangZonePool = pNetGame->GetGangZonePool();
	if(pGangZonePool) {
		bsData.Read(wZone);
		bsData.Read(dwColor);
		pGangZonePool->Flash(wZone, dwColor);
	}
}
void ScrStopFlashGangZone(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wZone;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CGangZonePool *pGangZonePool = pNetGame->GetGangZonePool();
	if(pGangZonePool) {
		bsData.Read(wZone);
		pGangZonePool->StopFlash(wZone);
	}
}
void ScrUnk56(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteAnimFileLen;
	BYTE byteAnimNameLen;
	char szAnimFile[256];
	char szAnimName[256];
	float fT;
	bool bUnk1, bUnk2, bUnk3, bUnk4;
	int iUnk;
	CPlayerPed *pPlayerPed;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szAnimFile, 0, sizeof(szAnimFile));
	memset(szAnimName, 0, sizeof(szAnimName));

	bsData.Read(playerId);
	bsData.Read(byteAnimFileLen);
	bsData.Read(szAnimFile, byteAnimFileLen);
	bsData.Read(byteAnimNameLen);
	bsData.Read(szAnimName, byteAnimNameLen);
	bsData.Read(fT);
	bsData.Read(bUnk1);
	bsData.Read(bUnk2);
	bsData.Read(bUnk4);
	bsData.Read(bUnk3);
	bsData.Read(iUnk);

	szAnimFile[byteAnimFileLen] = 0;
	szAnimName[byteAnimNameLen] = 0;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(!pPlayerPool) return;

	if(playerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPed = pPlayerPool->GetLocalPlayer()->GetPlayerPed();
	} else {
		if(playerId >= MAX_PLAYERS) return;
		if(!pPlayerPool->field_2A[playerId]) return;
		CNetPlayer *pNetPlayer = pPlayerPool->m_pPlayers[playerId];
		if(!pNetPlayer) return;
		CRemotePlayer *pRemotePlayer = pNetPlayer->m_pRemotePlayer;
		if(!pRemotePlayer) return;
		pPlayerPed = pRemotePlayer->m_pPlayerPed;
		pRemotePlayer->field_1C1 = 1;
	}

	if(!pPlayerPed) return;

	pPlayerPed->ApplyAnimation(szAnimName, szAnimFile, fT, bUnk1, bUnk2, bUnk4, bUnk3, iUnk);
}
void ScrUnk57(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	MATRIX4X4 matPlayer;
	CPlayerPed *pPlayerPed;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(playerId);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(!pPlayerPool) goto done;

	if(playerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPed = pPlayerPool->GetLocalPlayer()->GetPlayerPed();
	} else {
		if(playerId >= MAX_PLAYERS) goto done;
		if(!pPlayerPool->field_2A[playerId]) goto done;
		CNetPlayer *pNetPlayer = pPlayerPool->m_pPlayers[playerId];
		if(!pNetPlayer) goto done;
		CRemotePlayer *pRemotePlayer = pNetPlayer->m_pRemotePlayer;
		if(!pRemotePlayer) goto done;
		pPlayerPed = pRemotePlayer->m_pPlayerPed;
		pRemotePlayer->field_1C1 = 0;
	}

	if(!pPlayerPed) goto done;

	pPlayerPed->GetMatrix(&matPlayer);
	pPlayerPed->TeleportTo(matPlayer.pos.X, matPlayer.pos.Y, matPlayer.pos.Z);

done:
	;
}
void ScrUnk58(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(byteUnk);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(pPlayerPool) pPlayerPool->GetLocalPlayer()->sub_100030F0(byteUnk);
}
void ScrUnk59(RPCParameters *rpcParams) {}
void ScrUnk5A(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VECTOR vecMoveSpeed;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vecMoveSpeed.X);
	bsData.Read(vecMoveSpeed.Y);
	bsData.Read(vecMoveSpeed.Z);

	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(pPlayerPed) {
		if(pPlayerPed->FUNC_100AD860()) {
			pPlayerPed->FUNC_100ABDF0(pPlayerPed->FUNC_100ABDD0() ^ 3);
		}

		pPlayerPed->SetMoveSpeedVector(vecMoveSpeed);
	}
}
void ScrUnk5B(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteType;
	VECTOR vecVelocity;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	byteType = 0;
	bsData.Read(byteType);
	bsData.Read(vecVelocity.X);
	bsData.Read(vecVelocity.Y);
	bsData.Read(vecVelocity.Z);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

	if(pPlayerPed && pPlayerPed->IsInVehicle()) {
		VEHICLEID vehicleId = pVehiclePool->FUNC_1001EB90((int)pPlayerPed->GetGtaVehicle());
		if(vehicleId != INVALID_VEHICLE_ID) {
			CEntity *pVehicle = (CEntity *)pVehiclePool->GetAt(vehicleId);
			if(pVehicle) {
				if(byteType == 0) pVehicle->SetMoveSpeedVector(vecVelocity);
				else if(byteType == 1) pVehicle->SetTurnSpeedVector(vecVelocity);
			}
		}
	}
}
void ScrUnk6F(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteEnable;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteEnable);

	pChatWindow->AddDebugMessage("Widescreen = %i", byteEnable);
	ScriptCommand(&set_widescreen, byteEnable);
}
void ScrUnk62(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	BYTE byteTyreDamage;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(byteTyreDamage);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle *pVehicle = (CVehicle *)pVehiclePool->GetAt(vehicleId);

	if(pVehiclePool && pVehicle) pVehicle->sub_100B7940(byteTyreDamage);
}
void ScrUnk70(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD wUnk;
	int iUnk1, iUnk2, iUnk3, iUnk4;
	VECTOR vecPos;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	iUnk1 = 0;
	iUnk2 = 0;
	iUnk3 = 0;
	iUnk4 = 0;

	bsData.Read(wUnk);
	bsData.Read(iUnk1);
	bsData.Read(iUnk2);
	bsData.Read(iUnk3);
	bsData.Read(iUnk4);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);

	pGame->PlayCrimeReport(iUnk4, &vecPos, iUnk1, iUnk2, iUnk3);
}
void ScrSetSpawnInfo(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	SPAWN_INFO SpawnInfo;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	bsData.Read((PCHAR)&SpawnInfo, 46);

	pPlayerPool->GetLocalPlayer()->sub_10003BE0(&SpawnInfo);
}
void ScrUnk45(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteTeam;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	bsData.Read(playerId);
	bsData.Read(byteTeam);

	if(playerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPool->GetLocalPlayer()->SetTeam(byteTeam);
	} else {
		CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(playerId);
		if(pRemotePlayer) pRemotePlayer->field_109 = byteTeam;
	}
}
void ScrUnk99(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iPlayerId;
	int iSkin = 0;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(!pPlayerPool) return;

	bsData.Read(iPlayerId);
	bsData.Read(iSkin);

	if(!IsValidPedModel(iSkin)) {
		if(pChatWindow) pChatWindow->AddDebugMessage("Warning: SetPlayerSkin %d isn't a valid ped model.", iSkin);
		return;
	}

	if(iPlayerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPool->GetLocalPlayer()->GetPlayerPed()->SetSkin(iSkin);
	} else if(pPlayerPool->GetSlotState(iPlayerId) && pPlayerPool->GetAt(iPlayerId)->FUNC_10001080()
		&& pPlayerPool->GetAt(iPlayerId)->m_pPlayerPed) {
		pPlayerPool->GetAt(iPlayerId)->m_pPlayerPed->SetSkin(iSkin);
	}
}
void ScrSetPlayerPos(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VECTOR vecPos;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(!pPlayerPool) return;

	CLocalPlayer *pLocalPlayer = pPlayerPool->GetLocalPlayer();
	if(!pLocalPlayer) return;

	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);

	pLocalPlayer->field_2E2 = 0;
	pLocalPlayer->sub_10003710(0, 0);
	pLocalPlayer->GetPlayerPed()->TeleportTo(vecPos.X, vecPos.Y, vecPos.Z);
}
void ScrUnk0D(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VECTOR vecPos;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);

	vecPos.Z = pGame->FindGroundZForCoord(vecPos.X, vecPos.Y, vecPos.Z) + 1.5f;

	pLocalPlayer->field_2E2 = 0;
	pLocalPlayer->sub_10003710(0, 0);
	pLocalPlayer->GetPlayerPed()->TeleportTo(vecPos.X, vecPos.Y, vecPos.Z);
}
void ScrPutPlayerInVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	BYTE byteSeatId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(byteSeatId);

	int iGtaVehicleId = pNetGame->GetVehiclePool()->FindGtaIDFromID(vehicleId);
	int iVehicle = pNetGame->GetVehiclePool()->GetAt(vehicleId);

	if(iGtaVehicleId && iVehicle) {
		pGame->FindPlayerPed()->FUNC_100AC290(iGtaVehicleId, byteSeatId);
	}
}
void ScrRemovePlayerFromVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->ExitCurrentVehicle();
}
void ScrDisplayGameText(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iStyle, iTime, iLength;
	char szText[256];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szText, 0, sizeof(szText));

	bsData.Read(iStyle);
	bsData.Read(iTime);
	bsData.Read(iLength);

	if(iLength >= 1 && iLength <= 200) {
		bsData.Read(szText, iLength);
		szText[iLength] = 0;
		pGame->DisplayGameText(szText, iTime, iStyle);
	}
}
void ScrSetInterior(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteInterior;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteInterior);

	pGame->FindPlayerPed()->SetInterior(byteInterior, TRUE);
}
void ScrUnk9F(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	float fX, fY, fZ;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);

	if(!pNetGame) return;
	if(!pNetGame->GetVehiclePool()) return;

	if(pNetGame->GetVehiclePool()->GetSlotState(vehicleId)) {
		((CEntity *)pNetGame->GetVehiclePool()->GetAt(vehicleId))->TeleportTo(fX, fY, fZ);
	}
}
void ScrUnkA0(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	float fUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(fUnk);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(pVehiclePool && pVehiclePool->GetSlotState(vehicleId))
		((CVehicle *)pVehiclePool->GetAt(vehicleId))->FUNC_100B8170(fUnk);
}
void ScrUnkA1(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	BYTE byteA, byteB;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(byteA);
	bsData.Read(byteB);

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(pVehiclePool) pVehiclePool->FUNC_1001EB30(vehicleId, byteA, byteB);
}
void ScrUnk0F(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteControllable;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(byteControllable);

	pGame->FindPlayerPed()->TogglePlayerControllable(byteControllable);
}
void ScrUnk10(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iSound;
	float fX, fY, fZ;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(iSound);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);

	CAudio *pAudio = pGame->m_pGameAudio;
	pAudio->PlaySound(iSound, fX, fY, fZ);
}
void ScrUnk11(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(pNetGame->GetSettings()->fWorldBoundryPX);
	bsData.Read(pNetGame->GetSettings()->fWorldBoundryZX);
	bsData.Read(pNetGame->GetSettings()->fWorldBoundryPY);
	bsData.Read(pNetGame->GetSettings()->fWorldBoundryNY);
}
void ScrUnk12(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iMoney;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(iMoney);

	pGame->AddToLocalMoney(iMoney);
}
void ScrSetPlayerFacingAngle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	float fAngle;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fAngle);

	pGame->FindPlayerPed()->ForceTargetRotation(fAngle);
}
void ScrUnk16(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iWeapon, iAmmo;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(iWeapon);
	bsData.Read(iAmmo);

	pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->GiveWeapon(iWeapon, iAmmo);
}
void ScrUnk40(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(playerId);
}
void ScrUnk41(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID vehicleId;
	BYTE byteUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(vehicleId);
	bsData.Read(byteUnk);

	pNetGame->GetVehiclePool()->FUNC_1001EB00(vehicleId, byteUnk);
}
void ScrUnk42(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	float fArmour;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fArmour);

	pLocalPlayer->GetPlayerPed()->SetArmour(fArmour);
}
void ScrUnk37(RPCParameters *rpcParams) {}
void ScrUnk21(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iLen;
	char szName[33];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szName, 0, sizeof(szName));

	bsData.Read(szName, 32);

	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(pPlayerPed) {
		iLen = strlen(szName);
		if(!iLen) {
			pPlayerPed->SetShopName(NULL);
			pPlayerPed->LoadShoppingDataSubsection("");
		} else {
			pPlayerPed->SetShopName(szName);
			pPlayerPed->LoadShoppingDataSubsection(szName);
		}
	}
}
void ScrUnk23(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iLevel;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(iLevel);

	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(pPlayerPed) pPlayerPed->FUNC_100ADFB0(iLevel);
}
void ScrUnk43(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iWeapon;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(!pPlayerPed) return;

	bsData.Read(iWeapon);

	if(iWeapon >= 0 && iWeapon <= 46) pPlayerPed->SetArmedWeapon(iWeapon, false);
}
void ScrUnk71(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	bool bAttach;
	int iIndex;
	PLAYERID playerId;
	char szAttach[52];
	CPlayerPed *pPlayerPed;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szAttach, 0, sizeof(szAttach));

	bsData.Read(playerId);
	bsData.Read(iIndex);
	bsData.Read(bAttach);

	if(bAttach) bsData.Read(szAttach, 52);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if(pPlayerPool) {
		if(playerId == pPlayerPool->GetLocalPlayerID()) {
			pPlayerPed = pPlayerPool->GetLocalPlayer()->GetPlayerPed();
		} else {
			if(playerId >= MAX_PLAYERS) return;
			if(!pPlayerPool->field_2A[playerId]) return;
			CNetPlayer *pNetPlayer = pPlayerPool->m_pPlayers[playerId];
			pPlayerPed = (pNetPlayer ? pNetPlayer->m_pRemotePlayer : NULL)->m_pPlayerPed;
		}

		if(pPlayerPed) {
			if(!bAttach) pPlayerPed->RemoveAttachedObject(iIndex);
			else pPlayerPed->FUNC_100B0B10(iIndex, szAttach);
		}
	}
}
void ScrUnk29(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteLen;
	VECTOR vecPos;
	float fRadius;
	BYTE byteUsePos;
	char szURL[257];

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	memset(szURL, 0, sizeof(szURL));

	byteLen = 0;
	vecPos.X = 0.0f;
	vecPos.Y = 0.0f;
	vecPos.Z = 0.0f;
	fRadius = 0.0f;
	byteUsePos = 0;

	bsData.Read(byteLen);
	bsData.Read(szURL, byteLen);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRadius);
	bsData.Read(byteUsePos);

	if(pAudioStream) pAudioStream->FUNC_10066960(szURL, vecPos.X, vecPos.Y, vecPos.Z, fRadius, byteUsePos);
}
void ScrUnk2A(RPCParameters *rpcParams)
{
	if(pAudioStream) pAudioStream->Stop(0);
}
void ScrUnk2B(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iModel;
	float fX, fY, fZ, fRadius;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	iModel = 0;
	fX = 0.0f;
	fY = 0.0f;
	fZ = 0.0f;
	fRadius = 0.0f;

	bsData.Read(iModel);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(fRadius);

	FUNC_1009D3D0(iModel, fX, fY, fZ, fRadius);
}
void ScrUnk51(RPCParameters *rpcParams) {}
void ScrUnk52(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	bool bPosition;
	BYTE byteCut;
	int iTime;
	VECTOR vecFrom, vecTo;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bPosition = true;
	bsData.Read(bPosition);
	bsData.Read(vecFrom.X);
	bsData.Read(vecFrom.Y);
	bsData.Read(vecFrom.Z);
	bsData.Read(vecTo.X);
	bsData.Read(vecTo.Y);
	bsData.Read(vecTo.Z);
	bsData.Read(iTime);
	bsData.Read(byteCut);

	if(byteCut < 1 || byteCut > 2) byteCut = 2;

	if(iTime > 0) {
		pNetGame->GetPlayerPool()->GetLocalPlayer()->field_314 = 1;
		if(bPosition) pGame->GetCamera()->InterpolatePosition(&vecFrom, &vecTo, iTime, byteCut);
		else pGame->GetCamera()->InterpolateLookAt(&vecFrom, &vecTo, iTime, byteCut);
	}
}
void ScrUnk53(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	bool bUnk;
	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bUnk = false;
	iUnk = 0;

	bsData.Read(bUnk);
	bsData.Read(iUnk);

	if(dword_1026EB64 && dword_1026EB64->field_0) dword_1026EB64->FUNC_1006DB80(FALSE);
	if(dword_1026EB60 && dword_1026EB60->field_80) dword_1026EB60->FUNC_10072BC0(FALSE);

	if(bUnk) {
		if(dword_1026EB68) dword_1026EB68->FUNC_10071440(iUnk);
	} else if(dword_1026EB68) {
		dword_1026EB68->FUNC_10071520();
	}
}
void ScrUnk54(RPCParameters *rpcParams) {}
void ScrUnkA9(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD objectId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(objectId);

	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	if(objectId <= MAX_OBJECTS) {
		if(pObjectPool->field_4[objectId]) {
			CObject *pObject = (CObject *)pObjectPool->field_FA4[objectId];
			if(pObject) pObject->field_53 = 1;
		}
	}
}
void ScrUnkAD(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	ACTORID actorId;
	BYTE byteAnimFileLen;
	BYTE byteAnimNameLen;
	char szAnimFile[256];
	char szAnimName[256];
	float fT;
	bool bUnk1, bUnk2, bUnk3, bUnk4;
	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(pActorPool) {
		memset(szAnimFile, 0, sizeof(szAnimFile));
		memset(szAnimName, 0, sizeof(szAnimName));

		bsData.Read(actorId);
		bsData.Read(byteAnimFileLen);
		bsData.Read(szAnimFile, byteAnimFileLen);
		bsData.Read(byteAnimNameLen);
		bsData.Read(szAnimName, byteAnimNameLen);
		bsData.Read(fT);
		bsData.Read(bUnk1);
		bsData.Read(bUnk2);
		bsData.Read(bUnk4);
		bsData.Read(bUnk3);
		bsData.Read(iUnk);

		szAnimFile[byteAnimFileLen] = 0;
		szAnimName[byteAnimNameLen] = 0;

		CActorPed *pActor = (CActorPed *)pActorPool->GetAt(actorId);
		if(pActor) pActor->ApplyAnimation(szAnimName, szAnimFile, fT, bUnk1, bUnk2, bUnk4, bUnk3, iUnk);
	}
}
void ScrUnkAE(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	ACTORID actorId;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(actorId);

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(pActorPool) {
		CActorPed *pActor = (CActorPed *)pActorPool->GetAt(actorId);
		if(pActor) pActor->FUNC_1009C550();
	}
}
void ScrUnkAF(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	ACTORID actorId;
	float fUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(actorId);
	bsData.Read(fUnk);

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(pActorPool) {
		CActorPed *pActor = (CActorPed *)pActorPool->GetAt(actorId);
		if(pActor) pActor->FUNC_1009C570(fUnk);
	}
}
void ScrUnkB0(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	ACTORID actorId;
	VECTOR vecPos;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(actorId);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(pActorPool) {
		if(actorId < MAX_ACTORS) {
			if(pActorPool->field_FA4[actorId]) {
				CEntity *pActor = (CEntity *)pActorPool->field_4[actorId];
				if(pActor) pActor->TeleportTo(vecPos.X, vecPos.Y, vecPos.Z);
			}
		}
	}
}
void ScrUnkB2(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	ACTORID actorId;
	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bsData.Read(actorId);
	bsData.Read(iUnk);

	CActorPool *pActorPool = pNetGame->GetActorPool();
	if(pActorPool) {
		CActorPed *pActor = (CActorPed *)pActorPool->GetAt(actorId);
		if(pActor) pActor->FUNC_1009C5D0(iUnk);
	}
}
void ScrUnk30(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iUnk;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	iUnk = 0;
	bsData.Read(iUnk);

	if(dword_1026EB98) dword_1026EB98->FUNC_10017D10(iUnk);
}
void ScrInitMenu(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteMenuID;
	int iColumns;
	float fX, fY, fCol1Width, fCol2Width;
	MENU_INT MenuInteraction;
	BYTE byteRows, byteRow;
	int i;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(!pNetGame) return;

	CMenuPool *pMenuPool = pNetGame->GetMenuPool();

	fCol2Width = 0.0f;

	bsData.Read(byteMenuID);
	bsData.Read(iColumns);

	memset(pText, 0, sizeof(pText));
	bsData.Read(pText, 32);

	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fCol1Width);
	if(iColumns) bsData.Read(fCol2Width);

	bsData.Read(MenuInteraction.bMenu);
	for(i = 0; i < MAX_MENU_ITEMS; i++) bsData.Read(MenuInteraction.bRow[i]);

	if(pMenuPool->GetSlotState(byteMenuID)) pMenuPool->Delete(byteMenuID);

	CMenu *pMenu = pMenuPool->New(byteMenuID, fX, fY, iColumns + 1, fCol1Width, fCol2Width, &MenuInteraction);
	if(!pMenu) return;

	pMenu->SetTitle(pText);

	memset(pText, 0, sizeof(pText));
	bsData.Read(pText, 32);
	pMenu->SetColumnTitle(0, pText);

	bsData.Read(byteRows);
	if(byteRows > MAX_MENU_ITEMS) return;

	for(byteRow = 0; byteRow < byteRows; byteRow++) {
		memset(pText, 0, sizeof(pText));
		bsData.Read(pText, 32);
		pMenu->AddMenuItem(0, byteRow, pText);
	}

	if(!iColumns) return;

	memset(pText, 0, sizeof(pText));
	bsData.Read(pText, 32);
	pMenu->SetColumnTitle(1, pText);

	bsData.Read(byteRows);
	if(byteRows > MAX_MENU_ITEMS) return;

	for(byteRow = 0; byteRow < byteRows; byteRow++) {
		memset(pText, 0, sizeof(pText));
		bsData.Read(pText, 32);
		pMenu->AddMenuItem(1, byteRow, pText);
	}
}
void ScrShowMenu(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteMenuID;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(!pNetGame) return;

	bsData.Read(byteMenuID);
	pNetGame->GetMenuPool()->ShowMenu(byteMenuID);
}
void ScrHideMenu(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	BYTE byteMenuID;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	if(!pNetGame) return;

	bsData.Read(byteMenuID);
	pNetGame->GetMenuPool()->HideMenu(byteMenuID);
}

//----------------------------------------------------

void ScrSetPlayerName(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteNickLen;
	char szNewName[MAX_PLAYER_NAME+1];
	BYTE byteSuccess;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	bsData.Read(playerId);
	bsData.Read(byteNickLen);

	if(byteNickLen > MAX_PLAYER_NAME) return;

	bsData.Read(szNewName, byteNickLen);
	bsData.Read(byteSuccess);

	szNewName[byteNickLen] = '\0';

	if (byteSuccess == 1) pPlayerPool->SetPlayerName(playerId, szNewName);

	// Extra addition which we need to do if this is the local player;
	if( pPlayerPool->GetLocalPlayerID() == playerId )
		pPlayerPool->SetLocalPlayerName( szNewName );
}

//----------------------------------------------------

void ScrSetPlayerHealth(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	float fHealth;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fHealth);

	pLocalPlayer->GetPlayerPed()->SetHealth(fHealth);
}

//----------------------------------------------------

void ScrSetPlayerColor(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	PLAYERID playerId;
	DWORD dwColor;

	bsData.Read(playerId);
	bsData.Read(dwColor);

	if(playerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPool->GetLocalPlayer()->SetPlayerColor(dwColor);
	} else {
		CRemotePlayer *pPlayer = pPlayerPool->GetAt(playerId);
		if(pPlayer)	pPlayer->SetPlayerColor(dwColor);
	}
}


//----------------------------------------------------

void ScrSetCameraPos(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VECTOR vecPos;
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	pGame->GetCamera()->SetPosition(vecPos.X,vecPos.Y,vecPos.Z,0.0f,0.0f,0.0f);
}

//----------------------------------------------------

void ScrSetCameraLookAt(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VECTOR vecPos;
	BYTE byteType=0;
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(byteType);
	if(byteType < 1 || byteType > 2) {
		byteType = 2;
	}
	pGame->GetCamera()->LookAtPoint(vecPos.X,vecPos.Y,vecPos.Z,byteType);
}

//----------------------------------------------------

void ScrSetCameraBehindPlayer(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	pGame->GetCamera()->SetBehindPlayer();
}

//----------------------------------------------------

void ScrResetMoney(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	pGame->ResetLocalMoney();
}

//----------------------------------------------------

void ScrResetPlayerWeapons(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	CPlayerPed *pPlayerPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
	pPlayerPed->ClearAllWeapons();
}

//----------------------------------------------------

void ScrForceSpawnSelection(RPCParameters *rpcParams)
{
	pNetGame->GetPlayerPool()->GetLocalPlayer()->ReturnToClassSelection();
}

//----------------------------------------------------

void ScrEnableStuntBonus(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bool bStuntBonusEnabled;
	bsData.Read(bStuntBonusEnabled);
	pGame->EnableStuntBonus(bStuntBonusEnabled);
}

//----------------------------------------------------

void ScrSetPlayerDrunkVisuals(RPCParameters* rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	int iVisuals;
	bsData.Read(iVisuals);

	CPlayerPed* pPlayer = pGame->FindPlayerPed();
	if (pPlayer) {
		ScriptCommand(&set_player_drunk_visuals, 0, iVisuals);
	}
}

//----------------------------------------------------

void ScrSetPlayerDrunkHandling(RPCParameters* rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	int iVisuals;
	bsData.Read(iVisuals);

	CPlayerPed* pPlayer = pGame->FindPlayerPed();
	if (pPlayer) {
		ScriptCommand(&handling_responsiveness, 0, iVisuals);
	}
}

//----------------------------------------------------

void RegisterScriptRPCs(RakClientInterface* pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA7);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	REGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk93);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	REGISTER_STATIC_RPC(pRakClient, ScrForceSpawnSelection);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk69);
	REGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk56);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk57);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk58);
	REGISTER_STATIC_RPC(pRakClient, ScrEnableStuntBonus);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk59);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk6F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk62);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkVisuals);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkHandling);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk70);
	REGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerHealth);
	REGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	REGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	REGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk9F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk10);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrResetMoney);
	REGISTER_STATIC_RPC(pRakClient, ScrResetPlayerWeapons);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk42);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk21);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk23);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk43);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk71);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk29);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk51);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk52);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk53);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk54);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAD);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAE);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAF);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkB0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkB2);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	REGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------

void UnRegisterScriptRPCs(RakClientInterface* pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA7);
	UNREGISTER_STATIC_RPC(pRakClient, ScrForceSpawnSelection);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk69);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk56);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk57);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk58);
	UNREGISTER_STATIC_RPC(pRakClient, ScrEnableStuntBonus);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk59);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk6F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk62);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkVisuals);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkHandling);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk70);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk93);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk21);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk23);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk43);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk71);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk29);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerHealth);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk9F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk10);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrResetMoney);
	UNREGISTER_STATIC_RPC(pRakClient, ScrResetPlayerWeapons);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk51);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk52);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk53);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk54);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAD);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAE);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAF);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkB0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkB2);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	UNREGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------
