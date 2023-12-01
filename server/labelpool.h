
#ifndef SAMPSRV_LABELPOOL_H
#define SAMPSRV_LABELPOOL_H

struct struc_24
{
	char gap0[33];
};

class CLabelPool // size: WL 37888
{
private:
	int field_0[1024];
	struc_24 field_1000[1024];
public:
	CLabelPool();
	~CLabelPool();

	BOOL Delete(WORD wLabel);
};

#endif
