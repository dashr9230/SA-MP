
#include "main.h"

//----------------------------------------------------

float GetLength(PVECTOR vec)
{
	return (float)sqrt((vec->X * vec->X) + (vec->Y * vec->Y) + (vec->Z * vec->Z));
}

//----------------------------------------------------

