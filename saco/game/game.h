
#pragma once

#include "address.h"
#include "common.h"
#include "playerped.h"
#include "audio.h"
#include "camera.h"
#include "scripting.h"

//-----------------------------------------------------------

class CGame // size: 322
{
private:

	CAudio			*m_pGameAudio;
	CCamera			*m_pGameCamera;
	CPlayerPed		*m_pGamePlayer;

	struct
	{
		VECTOR position;
		VECTOR size;
		bool bEnabled;
		DWORD handle;
	} m_checkpoint;

	struct
	{
		VECTOR currentPosition;
		VECTOR nextPosition;
		float size;
		BYTE type;
		bool bEnabled;
		DWORD marker;
		DWORD handle;
	} m_racingCheckpoint;

	int field_55;
	int field_59;
	DWORD field_5D;
	int field_61;
	int field_65;
	int field_69;
	char field_6D;
	char field_6E[212];

public:

	void	StartGame();
	void	InitGame();
	BOOL	IsMenuActive();
	BOOL	IsGameLoaded();

	void	RequestModel(int iModelID, int iLoadingStream = 2);
	void	LoadRequestedModels();
	BOOL	IsModelLoaded(int iModelID);

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
	void	SetWantedLevel(BYTE byteLevel);
	void	SetGameTextCount(WORD wCount);
	void	DrawGangZone(float* fPos, DWORD dwColor);

	void	UpdateCheckpoints();
	void	ToggleCheckpoints(bool bEnabled) { m_checkpoint.bEnabled = bEnabled; }
	void	SetCheckpointInformation(VECTOR* vecPos, VECTOR* vecSize);

	void	ToggleRaceCheckpoints(bool bEnabled) { m_racingCheckpoint.bEnabled = bEnabled; }
	void	SetRaceCheckpointInformation(BYTE byteType, VECTOR* vecPos, VECTOR* vecNextPos, float fSize);
	void	MakeRaceCheckpoint();
	void	DisableRaceCheckpoint();

	DWORD	CreateRadarMarkerIcon(int iMarkerType, float fX, float fY, float fZ, DWORD dwColor, int iStyle);
	void	DisableMarker(DWORD dwMarkerID);

	void   AddToLocalMoney(int iAmount);
	int	   GetLocalMoney();

	BYTE   GetActiveInterior();
	void   UpdateFarClippingPlane();

	DWORD	GetD3DDevice();

	DWORD	GetD3D() { return *(DWORD *)ADDR_ID3D9DEVICE; };

	HWND	GetMainWindowHwnd() { return *(HWND *)ADDR_HWND; };

	void	RestartEverything();

	//-----------------------------------------------------------

	void sub_10062570() { field_55++; };

	CAudio* GetAudio() { return m_pGameAudio; }

	CPlayerPed  *FindPlayerPed() {
		if(m_pGamePlayer==NULL)	m_pGamePlayer = new CPlayerPed();
		return m_pGamePlayer;
	};

	const PCHAR GetWeaponName(int iWeaponID);

	int GetScreenWidth() { return *(int*)0xC17044; };
	int GetScreenHeight() { return *(int*)0xC17048; };

	DWORD GetWeaponInfo(int iWeapon, int iUnk);

	CGame();

	void sub_100A0010();
	void sub_100A0090(int a1, int a2);
	BYTE sub_100A00C0();
	BYTE sub_100A00F0();
	void sub_100A0110();
	void sub_100A0210();
	void sub_100A0250();
	void sub_100A02E0();
	void sub_100A0310();
	void sub_100A0330();
	void sub_100A03D0();
	void sub_100A03E0();
	void sub_100A0400();

	void sub_100A05D0();
	void sub_100A06F0();

	void sub_100A1C10();
};

//-----------------------------------------------------------
