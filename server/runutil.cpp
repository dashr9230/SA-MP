#include <stdio.h>

//----------------------------------------------------

int CanFileBeOpenedForReading(char * filename)
{
	FILE *f;
	if(f=fopen(filename,"r")) {
		fclose(f);
		return 1;
	}
	return 0;
}

//----------------------------------------------------
