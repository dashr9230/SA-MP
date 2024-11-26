
#include "../main.h"

using namespace RakNet;
extern CNetGame*	pNetGame;
extern CGame * pGame;

//----------------------------------------------------

// TODO: These script RPCs
void ScrDisableVehicleCollisions(RPCParameters *rpcParams) {}
void ScrSetMapIcon(RPCParameters *rpcParams) {}
void ScrRemoveMapIcon(RPCParameters *rpcParams) {}
void ScrSetWeaponAmmo(RPCParameters *rpcParams) {}
void ScrSetGravity(RPCParameters *rpcParams) {}
void ScrSetVehicleHealth(RPCParameters *rpcParams) {}
void ScrAttachTrailerToVehicle(RPCParameters *rpcParams) {}
void ScrDetachTrailerFromVehicle(RPCParameters *rpcParams) {}
void ScrCreateObject(RPCParameters *rpcParams) {}
void ScrSetObjectPos(RPCParameters *rpcParams) {}
void ScrSetObjectRotation(RPCParameters *rpcParams) {}
void ScrDestroyObject(RPCParameters *rpcParams) {}
void ScrCreateExplosion(RPCParameters *rpcParams) {}
void ScrShowPlayerNameTag(RPCParameters *rpcParams) {}
void ScrMoveObject(RPCParameters *rpcParams) {}
void ScrStopObject(RPCParameters *rpcParams) {}
void ScrSetVehicleNumberPlate(RPCParameters *rpcParams) {}
void ScrTogglePlayerSpectating(RPCParameters *rpcParams) {}

void ScrUnk7D(RPCParameters *rpcParams)
{
	// nothing
}

void ScrSpectatePlayer(RPCParameters *rpcParams) {}
void ScrSpectateVehicle(RPCParameters *rpcParams) {}
void ScrRemoveVehicleComponent(RPCParameters *rpcParams) {}
void ScrAttachObjectToPlayer(RPCParameters *rpcParams) {}
void ScrSetPlayerWantedLevel(RPCParameters *rpcParams) {}
void ScrShowTextDraw(RPCParameters *rpcParams) {}
void ScrHideTextDraw(RPCParameters *rpcParams) {}
void ScrTextDrawSetString(RPCParameters *rpcParams) {}
void ScrAddGangZone(RPCParameters *rpcParams) {}
void ScrRemoveGangZone(RPCParameters *rpcParams) {}
void ScrFlashGangZone(RPCParameters *rpcParams) {}
void ScrStopFlashGangZone(RPCParameters *rpcParams) {}
void ScrApplyPlayerAnimation(RPCParameters *rpcParams) {}
void ScrClearPlayerAnimation(RPCParameters *rpcParams) {}
void ScrSetPlayerSpecialAction(RPCParameters *rpcParams) {}
void ScrSetPlayerFightingStyle(RPCParameters *rpcParams) {}
void ScrSetPlayerVelocity(RPCParameters *rpcParams) {}
void ScrSetVehicleVelocity(RPCParameters *rpcParams) {}
void ScrToggleWidescreen(RPCParameters *rpcParams) {}
void ScrSetVehicleTireStatus(RPCParameters *rpcParams) {}
void ScrPlayCrimeReport(RPCParameters *rpcParams) {}
void ScrSetSpawnInfo(RPCParameters *rpcParams) {}
void ScrSetPlayerTeam(RPCParameters *rpcParams) {}
void ScrSetPlayerSkin(RPCParameters *rpcParams) {}
void ScrSetPlayerPos(RPCParameters *rpcParams) {}
void ScrSetPlayerPosFindZ(RPCParameters *rpcParams) {}
void ScrPutPlayerInVehicle(RPCParameters *rpcParams) {}
void ScrRemovePlayerFromVehicle(RPCParameters *rpcParams) {}
void ScrDisplayGameText(RPCParameters *rpcParams) {}
void ScrSetInterior(RPCParameters *rpcParams) {}
void ScrSetVehiclePos(RPCParameters *rpcParams) {}
void ScrSetVehicleZAngle(RPCParameters *rpcParams) {}
void ScrSetVehicleParams(RPCParameters *rpcParams) {}
void ScrTogglePlayerControllable(RPCParameters *rpcParams) {}
void ScrPlaySound(RPCParameters *rpcParams) {}
void ScrSetWorldBounds(RPCParameters *rpcParams) {}
void ScrGivePlayerMoney(RPCParameters *rpcParams) {}
void ScrSetPlayerFacingAngle(RPCParameters *rpcParams) {}
void ScrGivePlayerWeapon(RPCParameters *rpcParams) {}
void ScrUnk40(RPCParameters *rpcParams) {}
void ScrLinkVehicleToInterior(RPCParameters *rpcParams) {}
void ScrSetPlayerArmour(RPCParameters *rpcParams) {}
void ScrSendDeathMessage(RPCParameters *rpcParams) {}
void ScrSetShopName(RPCParameters *rpcParams) {}
void ScrSetPlayerDrunkLevel(RPCParameters *rpcParams) {}
void ScrSetArmedWeapon(RPCParameters *rpcParams) {}
void ScrSetPlayerAttachedObject(RPCParameters *rpcParams) {}
void ScrPlayAudioStream(RPCParameters *rpcParams) {}
void ScrStopAudioStream(RPCParameters *rpcParams) {}
void ScrRemoveBuilding(RPCParameters *rpcParams) {}
void ScrAttachCameraToObject(RPCParameters *rpcParams) {}
void ScrInterpolateCamera(RPCParameters *rpcParams) {}
void ScrToggleSelectTextDraw(RPCParameters *rpcParams) {}
void ScrSetPlayerObjectMaterial(RPCParameters *rpcParams) {}
void ScrUnkA9(RPCParameters *rpcParams) {}
void ScrApplyActorAnimation(RPCParameters *rpcParams) {}
void ScrClearActorAnimation(RPCParameters *rpcParams) {}
void ScrSetActorFacingAngle(RPCParameters *rpcParams) {}
void ScrSetActorPos(RPCParameters *rpcParams) {}
void ScrSetActorHealth(RPCParameters *rpcParams) {}
void ScrUnk30(RPCParameters *rpcParams) {}
void ScrInitMenu(RPCParameters *rpcParams) {}
void ScrShowMenu(RPCParameters *rpcParams) {}
void ScrHideMenu(RPCParameters *rpcParams) {}

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
	REGISTER_STATIC_RPC(pRakClient, ScrDisableVehicleCollisions);
	REGISTER_STATIC_RPC(pRakClient, ScrSetMapIcon);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveMapIcon);
	REGISTER_STATIC_RPC(pRakClient, ScrSetWeaponAmmo);
	REGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleHealth);
	REGISTER_STATIC_RPC(pRakClient, ScrAttachTrailerToVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrDetachTrailerFromVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrCreateObject);
	REGISTER_STATIC_RPC(pRakClient, ScrSetObjectPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetObjectRotation);
	REGISTER_STATIC_RPC(pRakClient, ScrDestroyObject);
	REGISTER_STATIC_RPC(pRakClient, ScrCreateExplosion);
	REGISTER_STATIC_RPC(pRakClient, ScrShowPlayerNameTag);
	REGISTER_STATIC_RPC(pRakClient, ScrMoveObject);
	REGISTER_STATIC_RPC(pRakClient, ScrStopObject);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleNumberPlate);
	REGISTER_STATIC_RPC(pRakClient, ScrTogglePlayerSpectating);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	REGISTER_STATIC_RPC(pRakClient, ScrSpectatePlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrSpectateVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveVehicleComponent);
	REGISTER_STATIC_RPC(pRakClient, ScrForceSpawnSelection);
	REGISTER_STATIC_RPC(pRakClient, ScrAttachObjectToPlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerWantedLevel);
	REGISTER_STATIC_RPC(pRakClient, ScrShowTextDraw);
	REGISTER_STATIC_RPC(pRakClient, ScrHideTextDraw);
	REGISTER_STATIC_RPC(pRakClient, ScrTextDrawSetString);
	REGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrApplyPlayerAnimation);
	REGISTER_STATIC_RPC(pRakClient, ScrClearPlayerAnimation);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerSpecialAction);
	REGISTER_STATIC_RPC(pRakClient, ScrEnableStuntBonus);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFightingStyle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerVelocity);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleVelocity);
	REGISTER_STATIC_RPC(pRakClient, ScrToggleWidescreen);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleTireStatus);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkVisuals);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkHandling);
	REGISTER_STATIC_RPC(pRakClient, ScrPlayCrimeReport);
	REGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerTeam);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerSkin);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPosFindZ);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerHealth);
	REGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	REGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	REGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehiclePos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleZAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleParams);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrTogglePlayerControllable);
	REGISTER_STATIC_RPC(pRakClient, ScrPlaySound);
	REGISTER_STATIC_RPC(pRakClient, ScrSetWorldBounds);
	REGISTER_STATIC_RPC(pRakClient, ScrGivePlayerMoney);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrResetMoney);
	REGISTER_STATIC_RPC(pRakClient, ScrResetPlayerWeapons);
	REGISTER_STATIC_RPC(pRakClient, ScrGivePlayerWeapon);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	REGISTER_STATIC_RPC(pRakClient, ScrLinkVehicleToInterior);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerArmour);
	REGISTER_STATIC_RPC(pRakClient, ScrSendDeathMessage);
	REGISTER_STATIC_RPC(pRakClient, ScrSetShopName);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkLevel);
	REGISTER_STATIC_RPC(pRakClient, ScrSetArmedWeapon);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerAttachedObject);
	REGISTER_STATIC_RPC(pRakClient, ScrPlayAudioStream);
	REGISTER_STATIC_RPC(pRakClient, ScrStopAudioStream);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveBuilding);
	REGISTER_STATIC_RPC(pRakClient, ScrAttachCameraToObject);
	REGISTER_STATIC_RPC(pRakClient, ScrInterpolateCamera);
	REGISTER_STATIC_RPC(pRakClient, ScrToggleSelectTextDraw);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerObjectMaterial);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	REGISTER_STATIC_RPC(pRakClient, ScrApplyActorAnimation);
	REGISTER_STATIC_RPC(pRakClient, ScrClearActorAnimation);
	REGISTER_STATIC_RPC(pRakClient, ScrSetActorFacingAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetActorPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetActorHealth);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	REGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------

void UnRegisterScriptRPCs(RakClientInterface* pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisableVehicleCollisions);
	UNREGISTER_STATIC_RPC(pRakClient, ScrForceSpawnSelection);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAttachObjectToPlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerWantedLevel);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowTextDraw);
	UNREGISTER_STATIC_RPC(pRakClient, ScrHideTextDraw);
	UNREGISTER_STATIC_RPC(pRakClient, ScrTextDrawSetString);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrApplyPlayerAnimation);
	UNREGISTER_STATIC_RPC(pRakClient, ScrClearPlayerAnimation);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerSpecialAction);
	UNREGISTER_STATIC_RPC(pRakClient, ScrEnableStuntBonus);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFightingStyle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerVelocity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleVelocity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrToggleWidescreen);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleTireStatus);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkVisuals);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkHandling);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPlayCrimeReport);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetMapIcon);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveMapIcon);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetWeaponAmmo);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleHealth);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAttachTrailerToVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDetachTrailerFromVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrCreateObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetObjectPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetObjectRotation);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDestroyObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrCreateExplosion);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowPlayerNameTag);
	UNREGISTER_STATIC_RPC(pRakClient, ScrMoveObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleNumberPlate);
	UNREGISTER_STATIC_RPC(pRakClient, ScrTogglePlayerSpectating);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSpectatePlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSpectateVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveVehicleComponent);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetShopName);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkLevel);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetArmedWeapon);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerAttachedObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPlayAudioStream);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopAudioStream);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveBuilding);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerTeam);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerSkin);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPosFindZ);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerHealth);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehiclePos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleZAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleParams);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrTogglePlayerControllable);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPlaySound);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetWorldBounds);
	UNREGISTER_STATIC_RPC(pRakClient, ScrGivePlayerMoney);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrResetMoney);
	UNREGISTER_STATIC_RPC(pRakClient, ScrResetPlayerWeapons);
	UNREGISTER_STATIC_RPC(pRakClient, ScrGivePlayerWeapon);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk40);
	UNREGISTER_STATIC_RPC(pRakClient, ScrLinkVehicleToInterior);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSendDeathMessage);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAttachCameraToObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrInterpolateCamera);
	UNREGISTER_STATIC_RPC(pRakClient, ScrToggleSelectTextDraw);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerObjectMaterial);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	UNREGISTER_STATIC_RPC(pRakClient, ScrApplyActorAnimation);
	UNREGISTER_STATIC_RPC(pRakClient, ScrClearActorAnimation);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetActorFacingAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetActorPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetActorHealth);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	UNREGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------
