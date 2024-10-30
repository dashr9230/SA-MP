
#pragma once

//-----------------------------------------------------------

class CActorPed : public CEntity
{
public:

	// Constructor/Destructor.
	CActorPed(int iSkin, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f);
	virtual ~CActorPed();

	void  Destroy();

	void  SetTargetRotation(float fRotation);
	float GetHealth();
	void  SetHealth(float fHealth);
	float GetArmour();
	void  SetArmour(float fArmour);
	DWORD GetStateFlags();
	void  SetStateFlags(DWORD dwStateFlags);
	BOOL  IsDead();
	BYTE  GetActionTrigger();

	void ApplyAnimation(char *szAnimName, char *szAnimFile, float fT,
						int opt1, int opt2, int opt3, int opt4, int iUnk);
	void ClearAnimations();


	PED_TYPE    *m_pPed;
	DWORD		m_dwMarkerID;
	DWORD 		m_dwArrow;
	BYTE		m_byteCreateMarker;
	BYTE		m_byteImmune;
};

//-----------------------------------------------------------
