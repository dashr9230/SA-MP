
#include <windows.h>
#include <string.h>

int unnamed_1026B568[200];
BOOL unnamed_1026B888[200];

BOOL unnamed_100B2AF0()
{
	memset(unnamed_1026B888, 0, sizeof(unnamed_1026B888));
	memset(unnamed_1026B568, 0, sizeof(unnamed_1026B568));

	return TRUE;
}

int unnamed_100B2B20()
{
	int x=0;
	while(x!=200)
	{
		if(!unnamed_1026B888[x])
		{
			unnamed_1026B888[x] = TRUE;
			return x;
		}
		x++;
	}
	return (-1);
}











