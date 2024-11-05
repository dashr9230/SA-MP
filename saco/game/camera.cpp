
#include "game.h"
#include "util.h"
#include <math.h>

bool GLOBAL_1014FD8C;

//-----------------------------------------------------------

void CCamera::SetBehindPlayer()
{
	ScriptCommand(&lock_camera_position, 0);
	ScriptCommand(&restore_camera_to_user_defined);
	field_0 = NULL;
	ScriptCommand(&set_camera_behind_player);
	ScriptCommand(&restore_camera_jumpcut);
}

//-----------------------------------------------------------

void CCamera::SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ)
{
	ScriptCommand(&restore_camera_to_user_defined);
	field_0 = NULL;
	ScriptCommand(&set_camera_position,fX,fY,fZ,fRotationX,fRotationY,fRotationZ);
}

//-----------------------------------------------------------

void CCamera::LookAtPoint(float fX, float fY, float fZ, int iType)
{
	ScriptCommand(&restore_camera_to_user_defined);
	field_0 = NULL;
	ScriptCommand(&point_camera,fX,fY,fZ,iType);
}

//-----------------------------------------------------------

void CCamera::Restore()
{
	field_0 = NULL;
	ScriptCommand(&restore_camera_jumpcut);
}

//-----------------------------------------------------------

void CCamera::Fade(int iInOut)
{
	ScriptCommand(&fade,500,iInOut);
}

//-----------------------------------------------------------

void CCamera::GetMatrix(PMATRIX4X4 Matrix)
{
	Matrix->right.X = m_matPos->right.X;
	Matrix->right.Y = m_matPos->right.Y;
	Matrix->right.Z = m_matPos->right.Z;

	Matrix->up.X = m_matPos->up.X;
	Matrix->up.Y = m_matPos->up.Y;
	Matrix->up.Z = m_matPos->up.Z;

	Matrix->at.X = m_matPos->at.X;
	Matrix->at.Y = m_matPos->at.Y;
	Matrix->at.Z = m_matPos->at.Z;

	Matrix->pos.X = m_matPos->pos.X;
	Matrix->pos.Y = m_matPos->pos.Y;
	Matrix->pos.Z = m_matPos->pos.Z;
}

//-----------------------------------------------------------

void CCamera::SetMatrix(MATRIX4X4 Matrix)
{
	m_matPos->right.X = Matrix.right.X;
	m_matPos->right.Y = Matrix.right.Y;
	m_matPos->right.Z = Matrix.right.Z;

	m_matPos->up.X = Matrix.up.X;
	m_matPos->up.Y = Matrix.up.Y;
	m_matPos->up.Z = Matrix.up.Z;

	m_matPos->at.X = Matrix.at.X;
	m_matPos->at.Y = Matrix.at.Y;
	m_matPos->at.Z = Matrix.at.Z;

	m_matPos->pos.X = Matrix.pos.X;
	m_matPos->pos.Y = Matrix.pos.Y;
	m_matPos->pos.Z = Matrix.pos.Z;
}

//-----------------------------------------------------------

// unused, also probably not part of CCamera
void __stdcall FUNC_1009D560(bool unk)
{
	GLOBAL_1014FD8C = unk;
}

//-----------------------------------------------------------

void CCamera::Attach(int unk1, int unk2, int unk3)
{
	_asm push 1
	_asm push unk3
	_asm push unk2
	_asm push unk1
	_asm mov ecx, 0xB6F028
	_asm mov edx, 0x50C7C0
	_asm call edx
}

//-----------------------------------------------------------

void CCamera::InterpolatePosition(VECTOR *vecFrom, VECTOR *vecTo, int iTime, BYTE byteCut)
{
	ScriptCommand(&restore_camera_to_user_defined);
	ScriptCommand(&lock_camera_position,1);
	ScriptCommand(&set_camera_position_to,vecFrom->X,vecFrom->Y,vecFrom->Z,vecTo->X,vecTo->Y,vecTo->Z,iTime,byteCut);
}

//-----------------------------------------------------------

void CCamera::InterpolateLookAt(VECTOR *vecFrom, VECTOR *vecTo, int iTime, BYTE byteCut)
{
	ScriptCommand(&lock_camera_target_point,1);
	ScriptCommand(&point_camera_transverse_to,vecFrom->X,vecFrom->Y,vecFrom->Z,vecTo->X,vecTo->Y,vecTo->Z,iTime,byteCut);
}

//-----------------------------------------------------------

void CCamera::FUNC_1009D660(CEntity *pEntity)
{
	field_0 = pEntity;

	if(pEntity) {
		MATRIX4X4 mat;
		pEntity->GetMatrix(&mat);
		if(FUNC_100B4B50(&mat.pos)) {
			InterpolatePosition(&mat.pos, &mat.pos, 100, 1);
		}
	}
}

//-----------------------------------------------------------

void CCamera::FUNC_1009D6B0()
{
	if(!field_0 || !field_0->m_pEntity) return;

	MATRIX4X4 mat;
	field_0->GetMatrix(&mat);
	if(FUNC_100B4B50(&mat.pos)) {
		InterpolatePosition(&mat.pos, &mat.pos, 100, 1);
	}
}

//-----------------------------------------------------------

float CCamera::GetDistanceFromPoint(VECTOR *vecPos)
{
	float fSX,fSY,fSZ;

	fSX = (vecPos->X - *(float*)0xB6F9CC) * (vecPos->X - *(float*)0xB6F9CC);
	fSY = (vecPos->Y - *(float*)0xB6F9D0) * (vecPos->Y - *(float*)0xB6F9D0);
	fSZ = (vecPos->Z - *(float*)0xB6F9D4) * (vecPos->Z - *(float*)0xB6F9D4);

	return (float)sqrt(fSX + fSY + fSZ);
}

//-----------------------------------------------------------
