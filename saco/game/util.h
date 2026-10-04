
#pragma once

struct struc_13
{
	DWORD field_0;
	DWORD field_4;
	DWORD field_8;
	DWORD field_C;
};

BOOL ProcessLineOfSight(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
		DWORD *pHitEntity, int bCheckBuildings, int bCheckVehicles, int bCheckPeds,
		int bCheckObjects, int bCheckDummies, int bSeeThroughStuff,
		int  bIgnoreSomeObjectsForCamera, int bUnk1);

float GetNormalisation(VECTOR *vec);

float FloatDifference(float f1, float f2);
float FloatOffset(float f1, float f2); // .text:100B48B0
float NormalizeAngle(float fAngle);

void __stdcall WorldRemoveEntity(DWORD *dwEnt);
void __stdcall WorldAddEntity(DWORD *dwEnt);

int __stdcall GameGetWeaponModelIDFromWeaponID(int iWeaponID);
void __stdcall GameDisableCheatCodes();

PED_TYPE * __stdcall GamePool_Ped_GetAt(int iID);
int __stdcall GamePool_Ped_GetIndex(PED_TYPE *pActor);
VEHICLE_TYPE * __stdcall GamePool_Vehicle_GetAt(int iID);
DWORD __stdcall GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle);
ENTITY_TYPE * __stdcall GamePool_Object_GetAt(int iID);
PED_TYPE * __stdcall GamePool_FindPlayerPed();

BOOL __stdcall GameIsEntityOnScreen(DWORD * pdwEnt);
void __stdcall InitPlayerPedPtrRecords();
void __stdcall SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr);
DWORD __stdcall GetPlayerPedPtrRecord(BYTE bytePlayer);
BYTE __stdcall FindPlayerNumFromPedPtr(DWORD dwPedPtr);
void __stdcall FUNC_100B4390(BYTE bytePlayer, DWORD a2, DWORD a3, DWORD a4, DWORD a5);
struc_13 * __stdcall FUNC_100B43D0(BYTE bytePlayer);
BYTE __stdcall FUNC_100B43F0(DWORD *pdwRenderWare);
float __stdcall SquaredDistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2);
float __stdcall DistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2);
float __stdcall DistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2);
float __stdcall SquaredDistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2);
float __stdcall SquaredDistanceBetweenPoints(VECTOR *vec1, VECTOR *vec2);

void _VectorNormalise(VECTOR *vec);
DWORD __stdcall CRC32FromUpcaseString(char *szString);

float DegToRad(float fDegrees);
float InvDegToRad(float fDegrees);

void SaveCameraRaster(char *filename);

BOOL IsFileOrDirectoryExists(char * szPath);

DWORD unnamed_100B6100(char *szString, int nMaxLen);
int FUNC_100B61D0(int a1); // .text:100B61D0

bool FUNC_100B4B50(VECTOR *vecPos);
bool FUNC_100B4AE0(VECTOR *vec); // .text:100B4AE0
void FUNC_100B4D10(VECTOR *vecOut, MATRIX4X4 *pMatrix, VECTOR *vecOffset);
int FUNC_100B4BE0(int a1, int a2, int a3); // .text:100B4BE0
int FUNC_100B4C00(int a1, int a2, int a3); // .text:100B4C00

BUILDING_TYPE * GamePool_GetBuilding();
DUMMY_TYPE * GamePool_GetDummy();
OBJECT_TYPE * GamePool_GetObject();

void ReplaceBuildingModel(ENTITY_TYPE *pEntity, int iModelID);
int GetInvalidModelInfoCount();
int FUNC_100B3CD0(); // .text:100B3CD0
int FUNC_100B3D00(); // .text:100B3D00

DWORD * __stdcall GetModelInfo(int iModel); // .text:100A7A40
BOOL __stdcall IsValidModel(int iModel); // .text:100B3DB0
BOOL __stdcall IsValidPedModel(int iModel); // .text:100B3DD0
DWORD * __stdcall FUNC_100B46D0(int iModel); // .text:100B46D0
WORD __stdcall FUNC_100B4700(int iModel); // .text:100B4700

bool HasCollisionLoaded(VECTOR *vec);
BOOL __stdcall IsATrainPart(ENTITY_TYPE *pEntity);
BOOL __stdcall FUNC_100B4860(VEHICLE_TYPE *pVehicle); // .text:100B4860
BOOL __stdcall FUNC_100B6950(VEHICLE_TYPE *pVehicle); // .text:100B6950
int __stdcall FUNC_100B64E0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9); // .text:100B64E0
int __stdcall FUNC_100B6510(int a1, int a2); // .text:100B6510

BOOL IsHexChar(char c);
BOOL IsHexChar(wchar_t c);
DWORD GetColorFromEmbedCode(char *szString);
DWORD GetColorFromEmbedCode(wchar_t *szString);
void ReplaceUnprintableChars(char *szString); // .text:100B5DE0
void RemoveColorEmbedsFromString(char *szString);

UINT GetVehicleSubtypeFromVehiclePtr(VEHICLE_TYPE *pVehicle);

DWORD * GetNextTaskFromTask(DWORD *task);
int GetTaskTypeFromTask(DWORD *task);
const char* GetTaskNameFromTask(DWORD *task);

int FUNC_100B4BC0(int a1, int a2, int a3); // .text:100B4BC0
float FastSqrt(float x); // .text:100B5A60
void VectorCrossProduct(VECTOR *vecOut, VECTOR *vec1, VECTOR *vec2); // .text:100B4C20
void VectorNormaliseTo(VECTOR *vecOut, VECTOR *vecIn); // .text:100B4CC0
bool FUNC_100B4A70(VECTOR *vec); // .text:100B4A70
bool FUNC_100B3D30(int nModelIndex); // .text:100B3D30
int FUNC_100B41F0(void *pData); // .text:100B41F0
int __stdcall FUNC_100B4240(DWORD *pdwIn, DWORD dwParam); // .text:100B4240
void __stdcall FUNC_100B42B0(DWORD *pdwEntity); // .text:100B42B0
DWORD __stdcall FUNC_100B4430(int a1, int a2); // .text:100B4430
void FUNC_100B5D20(); // .text:100B5D20
void FUNC_100B52B0(MATRIX4X4 *pMatrix, float *pQuatOut); // .text:100B52B0
void FUNC_100B5480(float *pQuatOut, float *pQuatFrom, float *pQuatTo, float fT); // .text:100B5480
void FUNC_100B6A80(float *pQuat, MATRIX4X4 *pMatrix); // .text:100B6A80
void __stdcall FUNC_100B6900(VECTOR *pFront, VECTOR *pOut); // .text:100B6900
void FUNC_100B5470(float *pQuat, MATRIX4X4 *pMatrix); // .text:100B5470
void FUNC_100B5500(float *pQuat); // .text:100B5500
int FUNC_100B4100(int iModel); // .text:100B4100
int FUNC_100B4180(int iModel); // .text:100B4180
int FUNC_100B1DA0(DWORD *pRenderWare, int a2, int a3); // .text:100B1DA0
void FUNC_100B58A0(DWORD dwFrame, VECTOR *vecOut, int *a3); // .text:100B58A0
void __stdcall FUNC_100B44C0(BYTE bytePlayer, DWORD dwValue); // .text:100B44C0
DWORD __stdcall FUNC_100B4520(DWORD *pdw); // .text:100B4520
void __stdcall FUNC_100B4530(DWORD *pdwVtable); // .text:100B4530
void __stdcall FUNC_100B4550(DWORD *pdw, float fValue); // .text:100B4550
float __stdcall FUNC_100B4560(DWORD *pdw); // .text:100B4560
void __stdcall FUNC_100B4660(int iModel, int nTxdIndex); // .text:100B4660
void FUNC_100B46C0(DWORD dwValue); // .text:100B46C0
float FUNC_100B4FE0(float x, float y, float z); // .text:100B4FE0
int FUNC_100B56D0(int a1, int a2, int a3); // .text:100B56D0
void FUNC_100B5720(); // .text:100B5720
BOOL __stdcall FUNC_100B4570(int iModel); // .text:100B4570
DWORD __stdcall FUNC_100B45A0(int iModel); // .text:100B45A0
void __stdcall FUNC_100B45D0(int iModel, int a2); // .text:100B45D0
void __stdcall FUNC_100B4600(int iModel, int a2); // .text:100B4600
void __stdcall FUNC_100B4630(int iModel, int a2); // .text:100B4630
WORD __stdcall FUNC_100B4700(int iModel); // .text:100B4700
void __stdcall FUNC_100B4730(int iModel, WORD wValue); // .text:100B4730
BOOL __stdcall FUNC_100B44E0(DWORD *pModelInfo); // .text:100B44E0
WORD FUNC_100B6470(int a1, int a2, int a3, int a4, int a5); // .text:100B6470
void VectorScale(VECTOR *vecOut, VECTOR *vecIn, float fScale); // .text:100B51B0
void FUNC_100B51E0(int a1, int a2); // .text:100B51E0
float FUNC_100B5180(float *pQuat); // .text:100B5180
DWORD FUNC_100B1430(BYTE *pObject); // .text:100B1430, todo: implement
void FUNC_100B17D0(DWORD dwOther); // .text:100B17D0, todo: implement
void FUNC_100B1800(BYTE *pObject); // .text:100B1800, todo: implement
void FUNC_100B19D0(BYTE *pObject); // .text:100B19D0, todo: implement
void FUNC_100B1C80(BYTE *pObject); // .text:100B1C80, todo: implement
void FUNC_100B5910(int a1, int a2); // .text:100B5910
void FUNC_100B5940(int a1, int a2); // .text:100B5940
void FUNC_100B58D0(int a1, int a2, int a3); // .text:100B58D0
void FUNC_100B56F0(int a1, int a2, int a3); // .text:100B56F0
void FUNC_100B62B0(BYTE *pObject); // .text:100B62B0
BOOL __stdcall FUNC_100B62F0(BYTE *pObject); // .text:100B62F0
void __stdcall FUNC_100B6330(BYTE *pObject); // .text:100B6330
void FUNC_100B4D70(float m00, float m01, float m02, float m10, float m11, float m12,
		float m20, float m21, float m22, float *pW, float *pX, float *pY, float *pZ); // .text:100B4D70, todo: implement
void FUNC_100B4F10(float w, float x, float y, float z, float *pM00, float *pM01,
		float *pM02, float *pM10, float *pM11, float *pM12, float *pM20, float *pM21,
		float *pM22); // .text:100B4F10, todo: implement
void FUNC_100B5260(MATRIX4X4 *pMatrix, float *pQuatOut); // .text:100B5260
void FUNC_100B5420(float *pQuat, MATRIX4X4 *pMatrix); // .text:100B5420
void FUNC_100B57E0(int a1, int nIndex, int a3, int a4); // .text:100B57E0
void FUNC_100B5790(int a1, int nIndex, int a3); // .text:100B5790
void FUNC_100B5740(int a1, int a2); // .text:100B5740
void FUNC_100B5830(int a1, int nIndex, float *pfOut); // .text:100B5830
void FUNC_100B5620(float *pQuat, float *pEuler); // .text:100B5620
void FUNC_100B5210(int a1, void *a2); // .text:100B5210, __cdecl: the caller's
// stack cleanup is folded into the next call's argument setup

