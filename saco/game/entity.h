
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
	void  GetTurnSpeedVector(PVECTOR Vector);
	void  SetTurnSpeedVector(VECTOR Vector);
	void  sub_1009EE90(); // unused
	float sub_1009EEB0(); // unused
	void  sub_1009EEF0(PVECTOR Vector);
	void  sub_1009EF20(int a2); // unused
	UINT  GetModelIndex();
	void  TeleportTo(float x, float y, float z);

	BOOL  sub_1009FDE0();

	VECTOR		field_4[5];
	ENTITY_TYPE *m_pEntity;
	DWORD		m_dwGTAId;
};

//----------------------------------------------------------
