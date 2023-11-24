
#ifndef SAMPSRV_GANGZONEPOOL_H
#define SAMPSRV_GANGZONEPOOL_H

class CGangZonePool // size: WL 20480
{
private:
	char gap0[16384];
	int field_4000[1024];
public:
	CGangZonePool();
	~CGangZonePool() {};
};

#endif
