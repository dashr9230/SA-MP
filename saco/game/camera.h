
#pragma once

#include "game.h" // Main runtime interface base structs.

//-----------------------------------------------------------

class CCamera // size: 8
{
public:
	CEntity *field_0;
	MATRIX4X4 *m_matPos;
public:
	void SetBehindPlayer();
	void SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ);	// tested
	void LookAtPoint(float fX, float fY, float fZ, int iType);
	void Restore();
	void Fade(int iInOut);
	void GetMatrix(PMATRIX4X4 Matrix);
	void SetMatrix(MATRIX4X4 Matrix);
	void Attach(int unk1, int unk2, int unk3);
	void InterpolatePosition(VECTOR *vecFrom, VECTOR *vecTo, int iTime, BYTE byteCut);
	void InterpolateLookAt(VECTOR *vecFrom, VECTOR *vecTo, int iTime, BYTE byteCut);
	void sub_1009D660(CEntity *pEntity);
	void sub_1009D6B0();
	float GetDistanceFromPoint(VECTOR *vecPos);

	CCamera() {
		m_matPos = (MATRIX4X4 *)ADDR_CAMERA;
		field_0 = NULL;
	}
};
