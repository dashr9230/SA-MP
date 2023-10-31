
#include <stdio.h>
#include <windows.h>
#include "common.h"
#include "keystuff.h"
#include "address.h"

//-----------------------------------------------------------

GTA_CONTROLSET *pGcsInternalKeys = (GTA_CONTROLSET *)ADDR_KEYSTATES;
GTA_CONTROLSET GcsLocalPlayerKeys;
GTA_CONTROLSET GcsRemotePlayerKeys[PLAYER_PED_SLOTS];

BYTE	byteSaveDriveByLeft;
BYTE	byteSaveDriveByRight;
BYTE   *pbyteDriveByLeft =  (BYTE *)0xB6F1A4;
BYTE   *pbyteDriveByRight = (BYTE *)0xB6F1A5;

//-----------------------------------------------------------

void GameKeyStatesInit()
{
	int x=0;
	memset(&GcsLocalPlayerKeys,0,sizeof(GTA_CONTROLSET));
	while(x!=PLAYER_PED_SLOTS) {
		memset(&GcsRemotePlayerKeys[x],0,sizeof(GTA_CONTROLSET));
		x++;
	}
}

//-----------------------------------------------------------

void GameStoreLocalPlayerKeys()
{
	memcpy(&GcsLocalPlayerKeys,pGcsInternalKeys,sizeof(GTA_CONTROLSET));

	// save current driveby state
	byteSaveDriveByLeft = *pbyteDriveByLeft;
	byteSaveDriveByRight = *pbyteDriveByRight;
}

//-----------------------------------------------------------

void GameSetLocalPlayerKeys()
{
	memcpy(pGcsInternalKeys,&GcsLocalPlayerKeys,sizeof(GTA_CONTROLSET));

	// restore the driveby state also
	*pbyteDriveByLeft = byteSaveDriveByLeft;
	*pbyteDriveByRight = byteSaveDriveByRight;
}

//-----------------------------------------------------------

void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET * pGcsKeyStates)
{
	memcpy(&GcsRemotePlayerKeys[iPlayer],pGcsKeyStates,sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameSetRemotePlayerKeys(int iPlayer)
{
	// Use this to sync between player and remote player.
	//memcpy(&GcsRemotePlayerKeys[iPlayer],&GcsLocalPlayerKeys,96);

	memcpy(pGcsInternalKeys,&GcsRemotePlayerKeys[iPlayer],sizeof(GTA_CONTROLSET));

	if(GcsRemotePlayerKeys[iPlayer].wKeys1[5]) { // lookl
		*pbyteDriveByLeft = 1;
	} else {
		*pbyteDriveByLeft = 0;
	}

	if(GcsRemotePlayerKeys[iPlayer].wKeys1[7]) { // lookr
		*pbyteDriveByRight = 1;
	} else {
		*pbyteDriveByRight = 0;
	}
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetInternalKeys()
{
	return pGcsInternalKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetLocalPlayerKeys()
{
	return &GcsLocalPlayerKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetPlayerKeys(int iPlayer)
{
	return &GcsRemotePlayerKeys[iPlayer];
}

//-----------------------------------------------------------

void GameResetPlayerKeys(int iPlayer)
{
	memset(&GcsRemotePlayerKeys[iPlayer],0,sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameResetInternalKeys()
{
	GTA_CONTROLSET *pInternals = GameGetInternalKeys();
	memset((PVOID)pInternals,0,48 * sizeof(WORD));
}

//-----------------------------------------------------------
