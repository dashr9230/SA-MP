
#include <stdio.h>
#include <windows.h>
#include "fhicl-sha1.h"

//----------------------------------------------------------

void CalcSHA1(char *data, unsigned int len, unsigned int *digest)
{
	fhicl::SHA1 sha1;

	memset(digest, 0, sizeof(unsigned int) * 5);

	if(IsBadReadPtr(data, len)) return;

	sha1.Input(data, len);
	sha1.Result(digest);
}

//----------------------------------------------------------
