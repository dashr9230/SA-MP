
#pragma once

//----------------------------------------------------------

class CEntity
{
public:
	CEntity() {
		memset(field_4, 0, sizeof(field_4));
	};
	virtual ~CEntity() {};

	char		field_4[60];
	int			field_40;
	int 		field_44;
};

//----------------------------------------------------------
