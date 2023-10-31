
#include "playerpool.h"

CPlayerPool::CPlayerPool()
{
	// TODO: CPlayerPool::CPlayerPool W: 004196A0 L: 080B1F4E
	field_7163 = 0;
	field_8107 = 0;
	for(unsigned short i = 0; i < 1000; i++) {
		field_0[i] = 0;
		field_7167[i] = 0;
		field_810B[i] = 0;
	}
}
