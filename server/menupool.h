
#ifndef SAMPSRV_MENUPOOL_H
#define SAMPSRV_MENUPOOL_H

class CMenuPool // size: WL 2024
{
private:
	int* field_0[128];
	int field_200[128];
	char field_400[1000];

public:
	CMenuPool();
	~CMenuPool();
};

#endif
