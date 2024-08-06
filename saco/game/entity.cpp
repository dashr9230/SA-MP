
#include "../main.h"
#include "entity.h"


//-----------------------------------------------------------

void CEntity::GetMatrix(PMATRIX4X4 Matrix)
{
	if (!m_pEntity || !m_pEntity->mat) return;

	Matrix->right.X = m_pEntity->mat->right.X;
	Matrix->right.Y = m_pEntity->mat->right.Y;
	Matrix->right.Z = m_pEntity->mat->right.Z;

	Matrix->up.X = m_pEntity->mat->up.X;
	Matrix->up.Y = m_pEntity->mat->up.Y;
	Matrix->up.Z = m_pEntity->mat->up.Z;

	Matrix->at.X = m_pEntity->mat->at.X;
	Matrix->at.Y = m_pEntity->mat->at.Y;
	Matrix->at.Z = m_pEntity->mat->at.Z;

	Matrix->pos.X = m_pEntity->mat->pos.X;
	Matrix->pos.Y = m_pEntity->mat->pos.Y;
	Matrix->pos.Z = m_pEntity->mat->pos.Z;
}

//-----------------------------------------------------------

void CEntity::SetMatrix(MATRIX4X4 Matrix)
{
	if (!m_pEntity || !m_pEntity->mat) return;

	m_pEntity->mat->right.X = Matrix.right.X;
	m_pEntity->mat->right.Y = Matrix.right.Y;
	m_pEntity->mat->right.Z = Matrix.right.Z;

	m_pEntity->mat->up.X = Matrix.up.X;
	m_pEntity->mat->up.Y = Matrix.up.Y;
	m_pEntity->mat->up.Z = Matrix.up.Z;

	m_pEntity->mat->at.X = Matrix.at.X;
	m_pEntity->mat->at.Y = Matrix.at.Y;
	m_pEntity->mat->at.Z = Matrix.at.Z;

	m_pEntity->mat->pos.X = Matrix.pos.X;
	m_pEntity->mat->pos.Y = Matrix.pos.Y;
	m_pEntity->mat->pos.Z = Matrix.pos.Z;
}

//-----------------------------------------------------------

void CEntity::FUNC_1009EC80()
{
	if(!m_pEntity || m_pEntity->vtable == 0x863C40) return;

	DWORD dwRenderWare = (DWORD)m_pEntity->pdwRenderWare;
	DWORD dwMatrix = (DWORD)m_pEntity->mat;
	DWORD dwEntity = (DWORD)m_pEntity;

	if(dwEntity && dwRenderWare && dwMatrix)
	{
		_asm mov edx, dwRenderWare
		_asm mov eax, [edx+4]
		_asm add eax, 16
		_asm push eax
		_asm mov ecx, dwMatrix
		_asm mov edx, 0x59AD70
		_asm call edx

		_asm mov ecx, dwEntity
		_asm mov edx, 0x532B00
		_asm call edx
	}
}

//-----------------------------------------------------------

void CEntity::GetMoveSpeedVector(PVECTOR Vector)
{
	Vector->X = m_pEntity->vecMoveSpeed.X;
	Vector->Y = m_pEntity->vecMoveSpeed.Y;
	Vector->Z = m_pEntity->vecMoveSpeed.Z;
}

//-----------------------------------------------------------

void CEntity::SetMoveSpeedVector(VECTOR Vector)
{
	m_pEntity->vecMoveSpeed.X = Vector.X;
	m_pEntity->vecMoveSpeed.Y = Vector.Y;
	m_pEntity->vecMoveSpeed.Z = Vector.Z;
}

//-----------------------------------------------------------

void CEntity::FUNC_1009ED40(float fX, float fY, float fZ)
{
	int x=4;
	while(x) {
		field_4[x-1].X = field_4[x].X;
		field_4[x-1].Y = field_4[x].Y;
		field_4[x-1].Z = field_4[x].Z;
		x--;
	}

	field_4[4].X = fX;
	field_4[4].Y = fY;
	field_4[4].Z = fZ;

	m_pEntity->vecMoveSpeed.X =
		(field_4[0].X + field_4[1].X + field_4[2].X + field_4[3].X + field_4[4].X) * 0.2f;
	m_pEntity->vecMoveSpeed.Y =
		(field_4[0].Y + field_4[1].Y + field_4[2].Y + field_4[3].Y + field_4[4].Y) * 0.2f;
	m_pEntity->vecMoveSpeed.Z =
		(field_4[0].Z + field_4[1].Z + field_4[2].Z + field_4[3].Z + field_4[4].Z) * 0.2f;
}

//-----------------------------------------------------------

void CEntity::GetTurnSpeedVector(PVECTOR Vector)
{
	Vector->X = m_pEntity->vecTurnSpeed.X;
	Vector->Y = m_pEntity->vecTurnSpeed.Y;
	Vector->Z = m_pEntity->vecTurnSpeed.Z;
}

//-----------------------------------------------------------

void CEntity::SetTurnSpeedVector(VECTOR Vector)
{
	m_pEntity->vecTurnSpeed.X = Vector.X;
	m_pEntity->vecTurnSpeed.Y = Vector.Y;
	m_pEntity->vecTurnSpeed.Z = Vector.Z;
}

//-----------------------------------------------------------

void CEntity::ApplyTurnSpeed()
{
	DWORD dwEnt = (DWORD)m_pEntity;
	if(!dwEnt) return;

	_asm mov ecx, dwEnt
	_asm mov eax, 0x542E20
	_asm call eax
}

//-----------------------------------------------------------

float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
	DWORD dwEnt = (DWORD)m_pEntity;
	float fResult = 0.0f;

	if(!dwEnt) return 0.0f;

	_asm mov ecx, dwEnt
	_asm mov edx, 0x536BE0
	_asm call edx
	_asm mov fResult, eax

	return fResult;
}

//-----------------------------------------------------------

void CEntity::FUNC_1009EEF0(PVECTOR Vector)
{
	DWORD dwEnt = (DWORD)m_pEntity;
	if(!dwEnt) return;

	_asm push Vector
	_asm mov ecx, dwEnt
	_asm mov edx, 0x534250
	_asm call edx
}

//-----------------------------------------------------------

void CEntity::FUNC_1009EF20(int a2)
{
	DWORD dwEnt = (DWORD)m_pEntity;
	if(!dwEnt) return;

	_asm push a2
	_asm mov ecx, dwEnt
	_asm mov edx, 0x534120
	_asm call edx
}

//-----------------------------------------------------------

UINT CEntity::GetModelIndex()
{
	return m_pEntity->nModelIndex;
}

//-----------------------------------------------------------

void CEntity::TeleportTo(float x, float y, float z)
{
	DWORD dwThisEntity = (DWORD)m_pEntity;

	if(dwThisEntity && m_pEntity->vtable != 0x863C40) {
		if( GetModelIndex() != TRAIN_PASSENGER_LOCO &&
			GetModelIndex() != TRAIN_FREIGHT_LOCO &&
			GetModelIndex() != TRAIN_TRAM) {
			_asm mov ecx, dwThisEntity
			_asm mov edx, [ecx] ; vtbl
			_asm push 0
			_asm push z
			_asm push y
			_asm push x
			_asm call dword ptr [edx+56] ; method 14
		} else {
			ScriptCommand(&put_train_at,m_dwGTAId,x,y,z);
		}
	}
}

//-----------------------------------------------------------

{
	return m_pEntity
		&& m_pEntity->vtable != 0x863C40
		&& m_pEntity->dwUnkModelRel
		&& m_pEntity->vecMoveSpeed.X == 0.0f
		&& m_pEntity->vecMoveSpeed.Y == 0.0f
		&& m_pEntity->vecMoveSpeed.Z == 0.0f;
}

//-----------------------------------------------------------
