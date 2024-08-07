
#pragma once

//----------------------------------------------------------

class CEntity
{
public:
	CEntity() {
		memset(field_4, 0, sizeof(field_4));
	};
	virtual ~CEntity() {};

	void  GetMatrix(PMATRIX4X4 Matrix);
	void  SetMatrix(MATRIX4X4 Matrix);
	void  FUNC_1009EC80();
	void  GetMoveSpeedVector(PVECTOR Vector);
	void  SetMoveSpeedVector(VECTOR Vector);
	void  FUNC_1009ED40(float fX, float fY, float fZ); // unused
	void  GetTurnSpeedVector(PVECTOR Vector);
	void  SetTurnSpeedVector(VECTOR Vector);
	void  ApplyTurnSpeed(); // unused
	float GetDistanceFromCentreOfMassToBaseOfModel(); // unused
	void  GetBoundCentre(PVECTOR Vector);
	void  GetBoundRect(PFRECT Rect); // unused
	UINT  GetModelIndex();
	void  TeleportTo(float x, float y, float z);
	float GetDistanceFromLocalPlayerPed();
	float GetDistanceFromCamera();
	float Get2DDistanceFromLocalPlayerPed(); // unused

	float GetDistanceFromPoint(float X, float Y, float Z);
	BOOL  IsStationary();

	BOOL  EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);
	BOOL  HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY);

	void  SetCollisionChecking(int iCheck);
	BOOL  IsCollisionCheckingEnabled();
	void  SetGravityProcessing(int iState);
	virtual void  Add();
	virtual void  Remove();
	BOOL  IsAdded();

	VECTOR		field_4[5];
	ENTITY_TYPE *m_pEntity;
	DWORD		m_dwGTAId;
};

//----------------------------------------------------------
