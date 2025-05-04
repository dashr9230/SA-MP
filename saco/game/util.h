
#pragma once

struct struc_13
{
	DWORD field_0;
	DWORD field_4;
	DWORD field_8;
	DWORD field_C;
};

void ProcessLineOfSight(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
		DWORD *pHitEntity, int bCheckBuildings, int bCheckVehicles, int bCheckPeds,
		int bCheckObjects, int bCheckDummies, int bSeeThroughStuff,
		int  bIgnoreSomeObjectsForCamera, int bUnk1);

float GetNormalisation(VECTOR *vec);

float FloatDifference(float f1, float f2);
float FloatOffset(float f1, float f2);
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
float __stdcall SquaredDistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2);
float DistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2);
float DistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2);
float __stdcall SquaredDistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2);
float __stdcall SquaredDistanceBetweenPoints(VECTOR *vec1, VECTOR *vec2);

void __stdcall _VectorNormalise(VECTOR *vec);
DWORD __stdcall CRC32FromUpcaseString(char *szString);

float DegToRad(float fDegrees);
float InvDegToRad(float fDegrees);

void SaveCameraRaster(char *filename);

BOOL IsFileOrDirectoryExists(char * szPath);

DWORD unnamed_100B6100(char *szString, int nMaxLen);

bool FUNC_100B4B50(VECTOR *vecPos);

BUILDING_TYPE * GamePool_GetBuilding();
DUMMY_TYPE * GamePool_GetDummy();
OBJECT_TYPE * GamePool_GetObject();

void ReplaceBuildingModel(ENTITY_TYPE *pEntity, int iModelID);
int GetInvalidModelInfoCount();

bool HasCollisionLoaded(VECTOR *vec);
BOOL __stdcall IsATrainPart(ENTITY_TYPE *pEntity);

BOOL IsHexChar(char c);
BOOL IsHexChar(wchar_t c);
DWORD GetColorFromEmbedCode(char *szString);
DWORD GetColorFromEmbedCode(wchar_t *szString);
void RemoveColorEmbedsFromString(char *szString);

UINT GetVehicleSubtypeFromVehiclePtr(VEHICLE_TYPE *pVehicle);

DWORD * GetNextTaskFromTask(DWORD *task);
int GetTaskTypeFromTask(DWORD *task);
const char* GetTaskNameFromTask(DWORD *task);

