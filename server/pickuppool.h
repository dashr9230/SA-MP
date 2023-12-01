
#ifndef SAMPSRV_PICKUPPOOL_H
#define SAMPSRV_PICKUPPOOL_H

#define MAX_PICKUPS 4096

class CPickupPool // size: WL 114692
{
private:

	char gap0[81920];
	int field_14000[4096];
	int field_18000[4096];
	int field_1C000;

public:

	CPickupPool() {
		for (int i = 0; i < MAX_PICKUPS; i++)
		{
			field_14000[i] = 0;
			field_18000[i] = 0;
		}
		field_1C000 = 0;
	};

	~CPickupPool() {};

};

#endif
