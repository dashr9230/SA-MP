
#pragma once

#include "game.h"
#include "entity.h"

//-----------------------------------------------------------

class CVehicle : public CEntity
{
public:

	char _gap48[4];

	VEHICLE_TYPE	*m_pVehicle;

	char _gap50[33];
	
	int			field_71;

	char _gap75[4];

	BYTE		m_byteColor1;
	BYTE		m_byteColor2;
	BOOL		m_bHasNewColor;

	char _gap7F[57];

	CVehicle( int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, BOOL bKeepModelLoaded = FALSE, int a8 = 0);

	void  ResetPointers();

	void  SetLockedState(int iLocked);

	float GetHealth();
	void  SetHealth(float fHealth);
	void  SetColor(BYTE byteColor1, BYTE byteColor2);
	void  UpdateColor();

	BOOL  HasSunk();
	BOOL  IsWrecked();
	BOOL  IsDriverLocalPlayer();
	BOOL  HasADriver();

	void    SetEngineState(BOOL bState);

	void	LinkToInterior(int iInterior);

	BOOL	IsOccupied();


};

//-----------------------------------------------------------
