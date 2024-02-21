
#include <stdio.h>
#include <windows.h>
#include "fhicl-sha1.h"

//----------------------------------------------------------

void SHA1_HashData(PCHAR szData, DWORD dwLen, DWORD *pdwDigest)
{
	fhicl::SHA1 sha1;

	for(int i = 0; i < 5; i++) {
		pdwDigest[i] = 0;
	}

	if(!IsBadReadPtr(szData, dwLen))
	{
		sha1.Input(szData, dwLen);
		sha1.Result((unsigned int*)pdwDigest);
	}
}

//----------------------------------------------------------
