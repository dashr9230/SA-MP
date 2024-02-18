
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
	void  sub_1009EC80();
	void  GetMoveSpeedVector(PVECTOR Vector);
	void  SetMoveSpeedVector(VECTOR Vector);
	void  sub_1009ED40(float fX, float fY, float fZ); // unused

	VECTOR		field_4[5];
	ENTITY_TYPE *m_pEntity;
	DWORD		m_dwGTAId;
};

//----------------------------------------------------------
