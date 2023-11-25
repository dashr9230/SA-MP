
#include "main.h"

CTextDrawPool::CTextDrawPool()
{
	for (WORD wText = 0; wText < MAX_TEXT_DRAWS; wText++)
	{
		field_0[wText] = 0;
		field_2000[wText] = 0;
		field_4000[wText] = 0;
	}
}

CTextDrawPool::~CTextDrawPool()
{
	for (WORD wText = 0; wText < MAX_TEXT_DRAWS; wText++)
	{
		if(field_2000[wText])
		{
			free(field_2000[wText]);
			field_2000[wText] = NULL;
		}
		if(field_4000[wText])
		{
			free(field_4000[wText]);
			field_4000[wText] = NULL;
		}
	}
}

