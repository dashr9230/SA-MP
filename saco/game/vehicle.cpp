
#include "../main.h"
#include "util.h"

//-----------------------------------------------------------
// CONSTRUCTOR

CVehicle::CVehicle(int iType, float fPosX, float fPosY,
				   float fPosZ, float fRotation, BOOL bKeepModelLoaded,
				   int a8)
{
	// TODO: CVehicle::CVehicle .text:100B88F0
}

//-----------------------------------------------------------

void CVehicle::LinkToInterior(int iInterior)
{
	if(GamePool_Vehicle_GetAt(m_dwGTAId)) {
		ScriptCommand(&link_vehicle_to_interior, m_dwGTAId, iInterior);
	}
}

//-----------------------------------------------------------
