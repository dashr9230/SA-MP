
#pragma once

#include "game.h"
#include "entity.h"

enum eLandingGearState 
{
	LGS_UP,
	LGS_DOWN,
};

//-----------------------------------------------------------

class CVehicle : public CEntity
{
public:

	char _gap48[4];

	VEHICLE_TYPE	*m_pVehicle;

	char _gap50[20];

	BYTE		field_64;
	int			field_65;

	char _gap69[8];
	
	int			field_71;

	char _gap75[4];

	BYTE		m_byteColor1;
	BYTE		m_byteColor2;
	BOOL		m_bHasNewColor;

	char _gap7F[4];
	int		field_83;
	char _gap87[49];

	CVehicle( int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, BOOL bKeepModelLoaded = FALSE, int a8 = 0);

	void  ResetPointers();

	void  SetLockedState(int iLocked);
	UINT  GetVehicleSubtype();

	float GetHealth();
	void  SetHealth(float fHealth);
	void  SetColor(BYTE byteColor1, BYTE byteColor2);
	void  UpdateColor();

	BOOL  HasSunk();
	BOOL  IsWrecked();
	BOOL  IsDriverLocalPlayer();
	BOOL  IsATrainPart();
	BOOL  HasTurret();
	BOOL  HasADriver();

	void  sub_100B7AC0(int iPanelDamage, int iDoorDamage, BYTE byteLightDamage); // .text:100B7AC0
	void  sub_100B7940(BYTE byteTyreDamage); // .text:100B7940
	void  FUNC_100B72C0(int a1); // .text:100B72C0
	void  FUNC_100B8170(float a1); // .text:100B8170
	void  FUNC_100B80B0(short a1); // .text:100B80B0

	void  SetHydraThrusters(DWORD dwDirection);
	DWORD GetHydraThrusters();


	UINT  GetPassengersMax();

	void    SetSirenOn(BYTE byteState);
	BOOL    IsSirenOn();
	void	SetAlarmState(WORD wState);
	void    SetLandingGearState(eLandingGearState state);
	eLandingGearState	GetLandingGearState();

	void    SetEngineState(BOOL bState);

	void	LinkToInterior(int iInterior);

	void	SetHornState(BYTE byteState);

	BOOL	IsOccupied();

	BOOL	FUNC_100B8240(); // .text:100B8240
	BOOL	FUNC_100B8330(); // .text:100B8330
	BOOL	FUNC_100B7420(); // .text:100B7420

	void	FUNC_100B7090(int a1); // .text:100B7090
	void	FUNC_100B7840(int a1); // .text:100B7840
	void	FUNC_100B7900(float fT); // .text:100B7900
	void	FUNC_100B81A0(); // .text:100B81A0
	void	FUNC_100B81F0(); // .text:100B81F0
	void	FUNC_100B8150(char *szText); // .text:100B8150
	void	FUNC_100B7C10(); // .text:100B7C10
	void	FUNC_100B7C80(int a1); // .text:100B7C80
	CVehicle *FUNC_100B7C90(); // .text:100B7C90
	BOOL	FUNC_100B7D70(); // .text:100B7D70
	BOOL	FUNC_100B7DD0(); // .text:100B7DD0
	BOOL	FUNC_100B70E0(); // .text:100B70E0
	BOOL	FUNC_100B7CF0(); // .text:100B7CF0

	BOOL	FUNC_100B7460(); // .text:100B7460
	BOOL	FUNC_100B7E00(); // .text:100B7E00


};

//-----------------------------------------------------------
