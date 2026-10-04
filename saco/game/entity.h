
#pragma once

#include "game.h"

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
	int   SetModelIndex(int iModel); // .text:1009EF50
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
	void  SetWaitingForCollision(int iState);
	void  DisableStreaming();
	void  EnableTunnelTransition();
	void  SetApplySpeed(int iState);
	void  MakeNonCollidable();
	void  SetClumpAlpha(int iAlpha);
	DWORD GetWorldBoundRadius();
	void  SetEulerAngles(float fX, float fY, float fZ);

	void  FUNC_100A7E20(); // .text:100A7E20

	virtual void  Add();
	virtual void  Remove();
	BOOL  IsAdded();

	float FUNC_1009F0C0(); // .text:1009F0C0
	BOOL  FUNC_1009F420(float a1, float a2, float a3, float a4); // .text:1009F420
	float FUNC_1009F190(); // .text:1009F190
	char  FUNC_1009FC00(VECTOR *vecStart, VECTOR *vecEnd, char a3); // .text:1009FC00
	char  FUNC_1009FCE0(); // .text:1009FCE0
	BYTE  FUNC_100B8340(); // .text:100B8340
	float FUNC_100B72A0(); // .text:100B72A0
	int   FUNC_100B7390(); // .text:100B7390
	DWORD * FUNC_1009FA60(); // .text:1009FA60

	VECTOR		field_4[5];
	ENTITY_TYPE *m_pEntity;
	DWORD		m_dwGTAId;
};

//----------------------------------------------------------
