
#include <stdio.h>
#include <windows.h>
#include "common.h"
#include "aimstuff.h"
#include "address.h"

CAMERA_AIM * pcaInternalAim = (CAMERA_AIM *)0xB6F32C;
CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[PLAYER_PED_SLOTS];
extern BYTE * pbyteCameraMode;

float * pfStats = (float*)0xB79380;
float * pfAspectRatio = (float*)0xC3EFA4;
float * pfCameraExtZoom = (float *)0xB6F250;
float fCameraExtZoom[PLAYER_PED_SLOTS];		// stored as a normalized multiplier float
float fLocalCameraExtZoom;

BYTE byteCameraMode[PLAYER_PED_SLOTS];

float fLocalAspectRatio;
float fLocalWeaponSkill[11];
float fAspectRatio[PLAYER_PED_SLOTS];
float fWeaponSkill[PLAYER_PED_SLOTS][11];

//----------------------------------------------------------

void __stdcall GameStoreLocalPlayerCameraExtZoom()
{
	fLocalCameraExtZoom = *pfCameraExtZoom;
	fLocalAspectRatio = *pfAspectRatio;
}

//----------------------------------------------------------

void __stdcall GameSetLocalPlayerCameraExtZoom()
{
	*pfCameraExtZoom = fLocalCameraExtZoom;
	*pfAspectRatio = fLocalAspectRatio;
}

//----------------------------------------------------------

void __stdcall GameSetPlayerCameraExtZoom(BYTE bytePlayerID, float fZoom, float fRatio)
{
	fCameraExtZoom[bytePlayerID] = fZoom;
	fAspectRatio[bytePlayerID] = fRatio;
}

//----------------------------------------------------------

float __stdcall GameGetLocalPlayerAspectRatio()
{
	float value = (*pfAspectRatio) - 1.0f;
	return value;
}

//----------------------------------------------------------

float __stdcall GameGetLocalPlayerCameraExtZoom()
{
	float value = ((*pfCameraExtZoom) - 35.0f) / 35.0f;	// normalize for 35.0 to 70.0
	return value;
}

//----------------------------------------------------------

void __stdcall GameSetRemotePlayerCameraExtZoom(BYTE bytePlayerID)
{
	float fZoom = fCameraExtZoom[bytePlayerID];
	float fValue = (fZoom * 35.0f) + 35.0f; // unnormalize for 35.0 to 70.0
	*pfCameraExtZoom = fValue;

	float fRatio = fAspectRatio[bytePlayerID];
	float fRatioValue = fRatio + 1.0f;
	*pfAspectRatio = fRatioValue;
}

//----------------------------------------------------------

void __stdcall GameSetPlayerCameraMode(BYTE byteMode, BYTE bytePlayerID)
{
	byteCameraMode[bytePlayerID] = byteMode;
}

//----------------------------------------------------------

BYTE __stdcall GameGetPlayerCameraMode(BYTE bytePlayerID)
{
	return byteCameraMode[bytePlayerID];
}

//----------------------------------------------------------

BYTE __stdcall GameGetLocalPlayerCameraMode()
{
	return *pbyteCameraMode;
}

//----------------------------------------------------------

void __stdcall GameAimSyncInit()
{
	memset(&caLocalPlayerAim,0,sizeof(CAMERA_AIM));
	memset(caRemotePlayerAim,0,sizeof(CAMERA_AIM) * PLAYER_PED_SLOTS);
	memset(byteCameraMode,4,PLAYER_PED_SLOTS);
	for(int i=0; i<PLAYER_PED_SLOTS; i++)
		for(int j=0; j<11; j++)
			fWeaponSkill[i][j] = 999.0f;
	for(int i=0; i<PLAYER_PED_SLOTS; i++)
		fAspectRatio[i] = 0.333333f;
	for(int i=0; i<PLAYER_PED_SLOTS; i++)
		fCameraExtZoom[i] = 1.0f;
}

//----------------------------------------------------------

void __stdcall GameStoreLocalPlayerAim()
{
	memcpy(&caLocalPlayerAim,pcaInternalAim,sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void __stdcall GameSetLocalPlayerAim()
{
	memcpy(pcaInternalAim,&caLocalPlayerAim,sizeof(CAMERA_AIM));
	//memcpy(pInternalCamera,&SavedCam,sizeof(MATRIX4X4));
}

//----------------------------------------------------------

CAMERA_AIM * __stdcall GameGetInternalAim()
{
	return pcaInternalAim;
}

//----------------------------------------------------------

void __stdcall GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer],caAim,sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void __stdcall GameSetRemotePlayerAim(int iPlayer)
{
	memcpy(pcaInternalAim,&caRemotePlayerAim[iPlayer],sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

CAMERA_AIM * __stdcall GameGetRemotePlayerAim(int iPlayer)
{
    return &caRemotePlayerAim[iPlayer];
}

//----------------------------------------------------------

void __stdcall GameSetPlayerWeaponSkillLevel(int iPlayer, int iSkill, WORD wLevel)
{
	if(iSkill < 11 && iPlayer < PLAYER_PED_SLOTS) {
		fWeaponSkill[iPlayer][iSkill] = (float)wLevel;
	}
}

//----------------------------------------------------------

void __stdcall GameSetLocalPlayerWeaponSkillLevel(int iSkill, WORD wLevel)
{
	if(iSkill < 11) {
		fLocalWeaponSkill[iSkill] = (float)wLevel;
		pfStats[iSkill + 69] = (float)wLevel;
	}
}

//----------------------------------------------------------

void __stdcall GameStoreLocalPlayerWeaponSkills()
{
	fLocalWeaponSkill[0] = pfStats[69]; // pistol
	fLocalWeaponSkill[1] = pfStats[70]; // silenced pistol
	fLocalWeaponSkill[2] = pfStats[71]; // desert eagle
	fLocalWeaponSkill[3] = pfStats[72]; // shotgun
	fLocalWeaponSkill[4] = pfStats[73]; // sawnoff
	fLocalWeaponSkill[5] = pfStats[74]; // spas12
	fLocalWeaponSkill[6] = pfStats[75]; // micro uzi
	fLocalWeaponSkill[7] = pfStats[76]; // mp5
	fLocalWeaponSkill[8] = pfStats[77]; // ak47
	fLocalWeaponSkill[9] = pfStats[78]; // m4
	fLocalWeaponSkill[10] = pfStats[79]; // sniper
}

//----------------------------------------------------------

void __stdcall GameSetLocalPlayerWeaponSkills()
{
	pfStats[69] = fLocalWeaponSkill[0]; // pistol
	pfStats[70] = fLocalWeaponSkill[1]; // silenced pistol
	pfStats[71] = fLocalWeaponSkill[2]; // desert eagle
	pfStats[72] = fLocalWeaponSkill[3]; // shotgun
	pfStats[73] = fLocalWeaponSkill[4]; // sawnoff
	pfStats[74] = fLocalWeaponSkill[5]; // spas12
	pfStats[75] = fLocalWeaponSkill[6]; // micro uzi
	pfStats[76] = fLocalWeaponSkill[7]; // mp5
	pfStats[77] = fLocalWeaponSkill[8]; // ak47
	pfStats[78] = fLocalWeaponSkill[9]; // m4
	pfStats[79] = fLocalWeaponSkill[10]; // sniper
}

//----------------------------------------------------------

void __stdcall GameResetLocalPlayerWeaponSkills()
{
	pfStats[69] = 999.0f; // pistol
	pfStats[70] = 999.0f; // silenced pistol
	pfStats[71] = 999.0f; // desert eagle
	pfStats[72] = 999.0f; // shotgun
	pfStats[73] = 999.0f; // sawnoff
	pfStats[74] = 999.0f; // spas12
	pfStats[75] = 999.0f; // micro uzi
	pfStats[76] = 999.0f; // mp5
	pfStats[77] = 999.0f; // ak47
	pfStats[78] = 999.0f; // m4
	pfStats[79] = 999.0f; // sniper

	GameStoreLocalPlayerWeaponSkills();
}

//----------------------------------------------------------

void __stdcall GameSetRemotePlayerWeaponSkills(int iPlayer)
{
	pfStats[69] = fWeaponSkill[iPlayer][0]; // pistol
	pfStats[70] = fWeaponSkill[iPlayer][1]; // silenced pistol
	pfStats[71] = fWeaponSkill[iPlayer][2]; // desert eagle
	pfStats[72] = fWeaponSkill[iPlayer][3]; // shotgun
	pfStats[73] = fWeaponSkill[iPlayer][4]; // sawnoff
	pfStats[74] = fWeaponSkill[iPlayer][5]; // spas12
	pfStats[75] = fWeaponSkill[iPlayer][6]; // micro uzi
	pfStats[76] = fWeaponSkill[iPlayer][7]; // mp5
	pfStats[77] = fWeaponSkill[iPlayer][8]; // ak47
	pfStats[78] = fWeaponSkill[iPlayer][9]; // m4
	pfStats[79] = fWeaponSkill[iPlayer][10]; // sniper
}

//----------------------------------------------------------
