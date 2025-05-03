
#pragma once

#include "address.h"
#include "common.h"
#include "vehicle.h"
#include "playerped.h"
#include "actorped.h"
#include "audio.h"
#include "camera.h"
#include "scripting.h"
#include "menu.h"

#include "hudcolor.h"

//-----------------------------------------------------------

class CGame // size: 322
{
private:

	CAudio			*m_pGameAudio;
	CCamera			*m_pGameCamera;
	CPlayerPed		*m_pGamePlayer;

	VECTOR			m_vecRaceCheckpointPos;
	VECTOR			m_vecRaceCheckpointNext;
	float			m_fRaceCheckpointSize;
	BYTE			m_byteRaceType;
	BOOL			m_bRaceCheckpointsEnabled;
	DWORD			m_dwRaceCheckpointMarker;
	DWORD			m_dwRaceCheckpointHandle;
	VECTOR			m_vecCheckpointPos;
	VECTOR			m_vecCheckpointExtent;
	BOOL			m_bCheckpointsEnabled;
	DWORD			m_dwCheckpointMarker;
	int field_55;
	int field_59;
	DWORD field_5D;
	int field_61;
	int field_65;
	BOOL field_69;
	char field_6D;
	char field_6E[212];

public:

	BYTE FindFirstFreePlayerPedSlot();
	BYTE FUNC_100A00F0();

	int		GetWeaponModelIDFromWeapon(int iWeaponID);
	BOOL	IsKeyPressed(int iKeyIdentifier);
	float	FindGroundZForCoord(float x, float y, float z);
	void	ToggleKeyInputsDisabled(int a2, BOOL a3);
	void	StartGame();
	void	InitGame();
	BOOL	IsGameLoaded();

	void	RequestModel(int iModelID, int iLoadingStream = 2);
	void	LoadRequestedModels();
	BOOL	IsModelLoaded(int iModelID);
	BOOL	SetModelDeletable(int iModelID);

	void	SetWorldTime(int iHour, int iMinute);
	void	GetWorldTime(BYTE* byteHour, BYTE* byteMinute);
	void	ToggleThePassingOfTime(BYTE byteOnOff);
	void	SetWorldWeather(int iWeatherID);
	int		GetWorldWeather();
	void	DisplayHud(BOOL bDisp);
	BYTE	IsHudEnabled();
	void	SetFrameLimiterOn(BOOL bLimiter);
	BOOL	IsFrameLimiterEnabled();
	void	EnableFrameLimiter();
	void	SetFrameLimit(DWORD dwLimit);
	void	SetMaxStats();
	void	DisableTrainTraffic();
	void	RefreshStreamingAt(float x, float y);
	void    RequestAnimation(char *szAnimFile);
	int		IsAnimationLoaded(char *szAnimFile);
	void	ReleaseAnimation(char *szAnimFile);
	void	ToggleRadar(int iToggle);
	void	DisplayGameText(char *szStr,int iTime,int iSize);
	void	SetGravity(float fGravity);
	void	EnableClock(BYTE byteClock);
	void	EnableZoneNames(BYTE byteEnable);
	void	SetWantedLevel(BYTE byteLevel);
	void	SetGameTextCount(WORD wCount);
	void	DrawGangZone(float* fPos, DWORD dwColor);
	void    EnableStuntBonus(bool bEnable);

	void   UpdateCheckpoints();
	void   SetCheckpointInformation(VECTOR *pos, VECTOR *extent);

	void	MakeRaceCheckpoint();
	void	DisableRaceCheckpoint();
	void   SetRaceCheckpointInformation(BYTE byteType, VECTOR *pos, VECTOR *next, float fSize);

	DWORD	CreateRadarMarkerIcon(int iMarkerType, float fX, float fY, float fZ, DWORD dwColor, int iStyle);
	void	DisableMarker(DWORD dwMarkerID);

	void   AddToLocalMoney(int iAmount);
	void   ResetLocalMoney();
	int	   GetLocalMoney();

	BYTE   GetActiveInterior();
	void   UpdateFarClippingPlane();

	DWORD	GetD3DDevice();

	DWORD	GetD3D() { return *(DWORD *)ADDR_ID3D9DEVICE; };

	HWND	GetMainWindowHwnd() { return *(HWND *)ADDR_HWND; };

	void	RestartEverything();
	void	ProcessInputDisabling();

	//-----------------------------------------------------------

	CCamera		*GetCamera() { return m_pGameCamera; };

	void FUNC_10062570() { field_55++; };

	CPlayerPed  *FindPlayerPed() {
		if(m_pGamePlayer==NULL)	m_pGamePlayer = new CPlayerPed();
		return m_pGamePlayer;
	};

	const PCHAR GetWeaponName(int iWeaponID);

	DWORD CreateWeaponPickup(int iModel, DWORD dwAmmo, float fX, float fY, float fZ);

	int GetScreenWidth() { return *(int*)0xC17044; };
	int GetScreenHeight() { return *(int*)0xC17048; };

	DWORD GetWeaponInfo(int iWeapon, int iUnk);
	void DisableEnterExits();

	CGame();

	void sub_100A0010();
	void sub_100A0090(int a1, int a2);
	BYTE sub_100A00C0();
	void sub_100A0110();
	void sub_100A0210();
	void sub_100A0250();
	void sub_100A02E0();
	void sub_100A0310();
	void sub_100A0330();

	void sub_100A1C10();

	void LoadScene(char* szScene);
	int GetMemoryUsedForStreaming();
	int GetMemoryAvailableForStreaming();
	int GetLoadedVehicleModelCount();
	void SetTimeInMilliseconds(DWORD dwTimeInMs);
	DWORD GetTimeInMilliseconds();
	int GetRwObjectsCount();
	void RequestAnimationsAndModels();
	void LoadCollisionFile(char *szFileName);
	void LoadCullZone(char *szLine);
	BOOL IsUsingController();
	void DisableWeaponLockOnTarget();
};

//-----------------------------------------------------------
