
#pragma once

#include "game.h"
#include "entity.h"

//-----------------------------------------------------------

class CVehicle : public CEntity
{
public:

	char _gap48[112];

	CVehicle( int iType, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f, BOOL bKeepModelLoaded = FALSE, int a8 = 0);

	void	LinkToInterior(int iInterior);

};

//-----------------------------------------------------------
