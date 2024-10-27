
#ifndef _SYSTEM_H
#define _SYSTEM_H

#pragma pack(1)
typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;

#pragma pack(1)
typedef struct _MATRIX4X4 {
	VECTOR right;
	DWORD  flags;
	VECTOR up;
	float  pad_u;
	VECTOR at;
	float  pad_a;
	VECTOR pos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

#pragma pack(1)
typedef struct _QUATERNION {
	float W,X,Y,Z;
} QUATERNION, *PQUATERNION;

typedef struct _struc_64
{
	int iModelID;
	int iBoneID;
	VECTOR vecOffset;
	VECTOR vecRotation;
	VECTOR vecScale;
} struc_64;

//----------------------------------------------------

#ifdef _WIN32
# include <windows.h>
#else

#ifndef FALSE
#define FALSE               0
#endif
#ifndef TRUE
#define TRUE                1
#endif

typedef int                 BOOL;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef char                CHAR;
typedef char				*PCHAR;
#endif //!Win32

#endif // _SYSTEM_H
