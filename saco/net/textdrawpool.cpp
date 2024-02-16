
#include "../main.h"

BOOL unnamed_100B2AF0();

//----------------------------------------------------

CTextDrawPool::CTextDrawPool()
{
	int x=0;
	while(x!=MAX_TEXT_DRAWS) {
		field_2400[x] = 0;
		field_0[x] = 0;
		x++;
	}
	unnamed_100B2AF0();
}

//----------------------------------------------------
