
#include "main.h"

CFilterScripts::CFilterScripts()
{
	field_1030 = 0;
	for (int i=0; i<MAX_FILTER_SCRIPTS; i++)
		field_0[i] = 0;
}

CFilterScripts::~CFilterScripts()
{
	UnloadFilterScripts();
}

void CFilterScripts::UnloadFilterScripts()
{
	// TODO: CFilterScripts::UnloadFilterScripts W .text:0046CEB0 L .text:080A15B0
}

