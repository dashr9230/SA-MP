
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

	char		field_4[60];
	ENTITY_TYPE *m_pEntity;
	DWORD		m_dwGTAId;
};

//----------------------------------------------------------
