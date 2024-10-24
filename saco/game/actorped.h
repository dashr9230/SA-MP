
#pragma once

//-----------------------------------------------------------

class CActorPed : public CEntity
{
public:

	// Constructor/Destructor.
	CActorPed(int iSkin, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f);

	void  Destroy();


	PED_TYPE    *m_pPed;
	DWORD		m_dwMarkerID;
	DWORD 		m_dwArrow;
	BYTE		m_byteCreateMarker;
	BYTE		m_byteImmune;
};

//-----------------------------------------------------------
