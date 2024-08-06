
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
	float FUNC_1009EEB0(); // unused
	void  FUNC_1009EEF0(PVECTOR Vector);
	void  FUNC_1009EF20(int a2); // unused
	void  ApplyTurnSpeed(); // unused
	UINT  GetModelIndex();
	void  TeleportTo(float x, float y, float z);

	BOOL  FUNC_1009FDE0();

	VECTOR		field_4[5];
	ENTITY_TYPE *m_pEntity;
	DWORD		m_dwGTAId;
};

//----------------------------------------------------------
