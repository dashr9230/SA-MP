
#pragma once

#include "game.h"
#include "entity.h"

//-----------------------------------------------------------

class CVehicle : public CEntity
{
public:

	char _gap48[4];

	VEHICLE_TYPE	*m_pVehicle;

	char _gap50[104];

	CVehicle( int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, BOOL bKeepModelLoaded = FALSE, int a8 = 0);

	void  ResetPointers();

	void  SetLockedState(int iLocked);

	float GetHealth();

	BOOL  HasADriver();

	void    SetEngineState(BOOL bState);

	void	LinkToInterior(int iInterior);

	BOOL	IsOccupied();


};

//-----------------------------------------------------------
