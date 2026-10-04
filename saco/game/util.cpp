
#include <windows.h>
#include <stdio.h>
#include "../main.h"
#include "util.h"
#include <sys/stat.h>

extern int iGtaVersion;

DWORD dwPlayerPedPtrs[PLAYER_PED_SLOTS];

struc_13 VAR_1026C258[PLAYER_PED_SLOTS];

#define PI 3.14159265f

//-----------------------------------------------------------

BOOL __declspec(naked) ProcessLineOfSight(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
		DWORD *pHitEntity, int bCheckBuildings, int bCheckVehicles, int bCheckPeds,
		int bCheckObjects, int bCheckDummies, int bSeeThroughStuff,
		int  bIgnoreSomeObjectsForCamera, int bUnk1)
{
	_asm mov eax, 0x56BA00
	_asm jmp eax
}

//-----------------------------------------------------------

void __stdcall WorldAddEntity(DWORD *dwEnt)
{
	_asm push dwEnt
	_asm mov ebx, 0x563220
	_asm call ebx
	_asm pop ebx
}

//-----------------------------------------------------------

void __stdcall WorldRemoveEntity(DWORD *dwEnt)
{
	_asm push dwEnt
	_asm mov ebx, 0x563280
	_asm call ebx
	_asm pop ebx
}

//-----------------------------------------------------------

void __stdcall GameDisableCheatCodes()
{

}

//-----------------------------------------------------------

PED_TYPE * __stdcall GamePool_Ped_GetAt(int iID)
{
	PED_TYPE *pActorRet;

	_asm mov ebx, ADDR_PED_TABLE
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, ADDR_ACTOR_FROM_ID
	_asm call ebx
	_asm mov pActorRet, eax

	return pActorRet;	
}

//-----------------------------------------------------------

int __stdcall GamePool_Ped_GetIndex(PED_TYPE *pActor)
{
	int iRetVal;

	_asm mov ebx, ADDR_PED_TABLE
	_asm mov ecx, [ebx]
	_asm push pActor
	_asm mov ebx, ADDR_ID_FROM_ACTOR
	_asm call ebx
	_asm mov iRetVal, eax

	return iRetVal;
}

//-----------------------------------------------------------

VEHICLE_TYPE * __stdcall GamePool_Vehicle_GetAt(int iID)
{
	VEHICLE_TYPE *pVehicleRet;

	_asm mov ebx, ADDR_VEHICLE_TABLE
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, ADDR_VEHICLE_FROM_ID
	_asm call ebx
	_asm mov pVehicleRet, eax

	return pVehicleRet;
}

//-----------------------------------------------------------

DWORD __stdcall GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle)
{
	DWORD dwID=0;

	_asm mov eax, ADDR_VEHICLE_TABLE
	_asm mov ecx, [eax]
	_asm push pVehicle
	_asm mov edx, 0x424160
	_asm call edx
	_asm mov dwID, eax

	return dwID;
}

//-----------------------------------------------------------

ENTITY_TYPE * __stdcall GamePool_Object_GetAt(int iID)
{
	ENTITY_TYPE *pObjectRet;

	_asm mov ebx, 0xB7449C
	_asm mov ecx, [ebx]
	_asm push iID
	_asm mov ebx, 0x465040
	_asm call ebx
	_asm mov pObjectRet, eax

	return pObjectRet;
}

//-----------------------------------------------------------
// Return the PED_TYPE * of the local player actor.

PED_TYPE * __stdcall GamePool_FindPlayerPed()
{
	return *(PED_TYPE **)(0xB7CD98);
}

//-----------------------------------------------------------

BUILDING_TYPE * GamePool_GetBuilding()
{
	BUILDING_TYPE *pBuildingRet;

	_asm mov eax, 0xB74498
	_asm mov edx, [eax]
	_asm mov eax, [edx]
	_asm mov pBuildingRet, eax

	return pBuildingRet;
}

//-----------------------------------------------------------

DUMMY_TYPE * GamePool_GetDummy()
{
	DUMMY_TYPE *pDummyRet;

	_asm mov eax, 0xB744A0
	_asm mov edx, [eax]
	_asm mov eax, [edx]
	_asm mov pDummyRet, eax

	return pDummyRet;
}

//-----------------------------------------------------------

OBJECT_TYPE * GamePool_GetObject()
{
	OBJECT_TYPE *pObjectRet;

	_asm mov eax, 0xB7449C
	_asm mov edx, [eax]
	_asm mov eax, [edx]
	_asm mov pObjectRet, eax

	return pObjectRet;
}

// the pool slot in CPools; the used-slot count walks the byte map that starts
// four bytes into the pool itself
struct GAME_POOL_HOLDER
{
	BYTE *pPool;

	int CountUsedSlots()
	{
		int iCount = 0;
		int iSize = *(int *)(pPool + 8);

		for(int i = 0; i < iSize; i++)
		{
			if(pPool[i + 4] > 0)
				iCount++;
		}
		return iCount;
	}
};

//-----------------------------------------------------------

int FUNC_100B3CD0()
{
	return ((GAME_POOL_HOLDER *)0xB7449C)->CountUsedSlots();
}

//-----------------------------------------------------------

int FUNC_100B3D00()
{
	return ((GAME_POOL_HOLDER *)0xC8800C)->CountUsedSlots();
}

//-----------------------------------------------------------

bool FUNC_100B3D30(int nModelIndex)
{
	OBJECT_TYPE *pPoolStart;

	_asm mov eax, 0xB7449C
	_asm mov edx, [eax]
	_asm mov eax, [edx]
	_asm mov pPoolStart, eax

	OBJECT_TYPE *pObject = pPoolStart;

	for(int i = 0; i != 3000; i++, pObject++)
	{
		if(pObject && pObject->vtable && pObject->vtable != 0x863C40 &&
			pObject->nModelIndex == nModelIndex)
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------

void ReplaceBuildingModel(ENTITY_TYPE *pEntity, int iModelID)
{
	_asm push iModelID
	_asm mov ecx, pEntity
	_asm mov edx, 0x403EC0
	_asm call edx
}

//-----------------------------------------------------------

// set by the base-model-info relocation hack; the setter is not implemented yet
BOOL bBaseModelInfoRelocated;
DWORD **ppRelocatedModelInfo;

DWORD * __stdcall GetModelInfo(int iModel)
{
	if(bBaseModelInfoRelocated)
		return (DWORD *)ppRelocatedModelInfo[iModel];

	if(iModel < 0 || iModel > 20000) return NULL;

	return ((DWORD **)0xA9B0C8)[iModel];
}

//-----------------------------------------------------------

BOOL __stdcall IsValidModel(int iModel)
{
	return GetModelInfo(iModel) != NULL;
}

//-----------------------------------------------------------

BOOL __stdcall IsValidPedModel(int iModel)
{
	DWORD *pModelInfo;

	if(iModel >= 0 && iModel <= 30000 &&
		(pModelInfo = GetModelInfo(iModel)) != NULL &&
		*pModelInfo == 0x85BDC0)
	{
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

int FUNC_100B41F0(void *pData)
{
	int iRet = 0;
	DWORD dwFunc;
	DWORD dwCamera = *(DWORD *)0xC1703C;

	if(!dwCamera) return iRet;

	dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE310 : 0x7EE2D0;

	_asm push pData
	_asm push dwCamera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm mov iRet, eax

	return iRet;
}

//-----------------------------------------------------------

int __stdcall FUNC_100B4240(DWORD *pdwIn, DWORD dwParam)
{
	DWORD dwCamera;
	DWORD dwFunc;
	DWORD *pData;
	DWORD data[4];
	int iRet;

	data[0] = pdwIn[0];
	data[1] = pdwIn[1];
	data[2] = pdwIn[2];
	data[3] = dwParam;
	pData = data;
	iRet = 0;

	dwCamera = *(DWORD *)0xC1703C;
	if(!dwCamera) return iRet;

	dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE310 : 0x7EE2D0;

	_asm push pData
	_asm push dwCamera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm mov iRet, eax

	return iRet;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B42B0(DWORD *pdwEntity)
{
	if(pdwEntity && pdwEntity[0x118])
	{
		DWORD dwState = ((DWORD *)pdwEntity[0x118])[0x166];
		if(dwState != 0 && dwState != 1)
		{
			DWORD *pdwObject = (DWORD *)pdwEntity[0x118];

			_asm mov ecx, pdwObject
			_asm mov ebx, [ecx]
			_asm push 1
			_asm call dword ptr [ebx]

			pdwEntity[0x118] = 0;
		}
	}
}

//-----------------------------------------------------------

DWORD __stdcall FUNC_100B4430(int a1, int a2)
{
	DWORD *pFound = NULL;
	DWORD dwResult = 0;

	_asm push a2
	_asm push a1
	_asm mov edx, 0x4D3A60
	_asm call edx
	_asm mov pFound, eax
	_asm pop edx
	_asm pop edx

	if(pFound)
	{
		_asm mov edx, [eax+0x10]
		_asm mov eax, [edx]
		_asm mov dwResult, eax
	}

	return dwResult;
}

//-----------------------------------------------------------

int GetInvalidModelInfoCount()
{
	DWORD *dwModelInfos = (DWORD *)0xA9C068;
	int iCount = 0;
	for(int i = 0; i != 19000; i++)
	{
		if(dwModelInfos[i] == NULL)
			iCount++;
	}
	return iCount;
}

//-----------------------------------------------------------
// Translate Weapon model ID into actual weapon ID.

int __stdcall GameGetWeaponModelIDFromWeaponID(int iWeaponID)
{
	switch(iWeaponID)
	{
	case WEAPON_BRASSKNUCKLE:
		return WEAPON_MODEL_BRASSKNUCKLE;

	case WEAPON_GOLFCLUB:
		return WEAPON_MODEL_GOLFCLUB;

	case WEAPON_NITESTICK:
		return WEAPON_MODEL_NITESTICK;

	case WEAPON_KNIFE:
		return WEAPON_MODEL_KNIFE;

	case WEAPON_BAT:
		return WEAPON_MODEL_BAT;

	case WEAPON_SHOVEL:
		return WEAPON_MODEL_SHOVEL;

	case WEAPON_POOLSTICK:
		return WEAPON_MODEL_POOLSTICK;

	case WEAPON_KATANA:
		return WEAPON_MODEL_KATANA;

	case WEAPON_CHAINSAW:
		return WEAPON_MODEL_CHAINSAW;

	case WEAPON_DILDO:
		return WEAPON_MODEL_DILDO;

	case WEAPON_DILDO2:
		return WEAPON_MODEL_DILDO2;

	case WEAPON_VIBRATOR:
		return WEAPON_MODEL_VIBRATOR;

	case WEAPON_VIBRATOR2:
		return WEAPON_MODEL_VIBRATOR2;

	case WEAPON_FLOWER:
		return WEAPON_MODEL_FLOWER;

	case WEAPON_CANE:
		return WEAPON_MODEL_CANE;

	case WEAPON_GRENADE:
		return WEAPON_MODEL_GRENADE;

	case WEAPON_TEARGAS:
		return WEAPON_MODEL_TEARGAS;

	case WEAPON_MOLTOV:
		return WEAPON_MODEL_MOLTOV;

	case WEAPON_COLT45:
		return WEAPON_MODEL_COLT45;

	case WEAPON_SILENCED:
		return WEAPON_MODEL_SILENCED;

	case WEAPON_DEAGLE:
		return WEAPON_MODEL_DEAGLE;

	case WEAPON_SHOTGUN:
		return WEAPON_MODEL_SHOTGUN;

	case WEAPON_SAWEDOFF:
		return WEAPON_MODEL_SAWEDOFF;

	case WEAPON_SHOTGSPA:
		return WEAPON_MODEL_SHOTGSPA;

	case WEAPON_UZI:
		return WEAPON_MODEL_UZI;

	case WEAPON_MP5:
		return WEAPON_MODEL_MP5;

	case WEAPON_AK47:
		return WEAPON_MODEL_AK47;

	case WEAPON_M4:
		return WEAPON_MODEL_M4;

	case WEAPON_TEC9:
		return WEAPON_MODEL_TEC9;

	case WEAPON_RIFLE:
		return WEAPON_MODEL_RIFLE;

	case WEAPON_SNIPER:
		return WEAPON_MODEL_SNIPER;

	case WEAPON_ROCKETLAUNCHER:
		return WEAPON_MODEL_ROCKETLAUNCHER;

	case WEAPON_HEATSEEKER:
		return WEAPON_MODEL_HEATSEEKER;

	case WEAPON_FLAMETHROWER:
		return WEAPON_MODEL_FLAMETHROWER;

	case WEAPON_MINIGUN:
		return WEAPON_MODEL_MINIGUN;

	case WEAPON_SATCHEL:
		return WEAPON_MODEL_SATCHEL;

	case WEAPON_BOMB:
		return WEAPON_MODEL_BOMB;

	case WEAPON_SPRAYCAN:
		return WEAPON_MODEL_SPRAYCAN;

	case WEAPON_FIREEXTINGUISHER:
		return WEAPON_MODEL_FIREEXTINGUISHER;

	case WEAPON_CAMERA:
		return WEAPON_MODEL_CAMERA;

	case WEAPON_NIGHTVISION:
		return WEAPON_MODEL_NIGHTVISION;

	case WEAPON_INFRARED:
		return WEAPON_MODEL_INFRARED;

	case WEAPON_PARACHUTE:
		return WEAPON_MODEL_PARACHUTE;

	}

	return -1;
}

//-----------------------------------------------------------

DWORD * GetNextTaskFromTask(DWORD *task)
{
	DWORD *ret_task=NULL;

	if(!task || *task < 0x800000 || *task > 0x900000) return NULL;

	_asm pushad
	_asm mov edx, task
	_asm mov ebx, [edx]
	_asm mov edx, [ebx+8]
	_asm mov ecx, task
	_asm call edx
	_asm mov ret_task, eax
	_asm popad

	return ret_task;
}

//-----------------------------------------------------------

int GetTaskTypeFromTask(DWORD *task)
{
	int i = 0;

	if(!task || *task < 0x800000 || *task > 0x900000) return 0;

	_asm pushad
	_asm mov edx, task
	_asm mov ebx, [edx]
	_asm mov edx, [ebx+10h]
	_asm mov ecx, task
	_asm call edx
	_asm mov i, eax
	_asm popad

	return i;
}

//-----------------------------------------------------------

const char* GetTaskNameFromTask(DWORD *task)
{
	return "None";
}

//-----------------------------------------------------------

BOOL __stdcall GameIsEntityOnScreen(DWORD * pdwEnt)
{
	BOOL bResult = FALSE;

	_asm mov ecx, pdwEnt
	_asm mov edx, 0x534540
	_asm call edx
	_asm mov bResult, eax

	return bResult != FALSE;
}

//-----------------------------------------------------------

void __stdcall InitPlayerPedPtrRecords()
{
	memset(&dwPlayerPedPtrs[0],0,sizeof(DWORD) * PLAYER_PED_SLOTS);
	memset(&VAR_1026C258[0],0,sizeof(struc_13) * PLAYER_PED_SLOTS);
}

//-----------------------------------------------------------

void __stdcall SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr)
{
	dwPlayerPedPtrs[bytePlayer] = dwPedPtr;
	memset(&VAR_1026C258[bytePlayer], 0, sizeof(struc_13));
}

//-----------------------------------------------------------

DWORD __stdcall GetPlayerPedPtrRecord(BYTE bytePlayer)
{
	return dwPlayerPedPtrs[bytePlayer];
}

//-----------------------------------------------------------

BYTE __stdcall FindPlayerNumFromPedPtr(DWORD dwPedPtr)
{
	BYTE x = 0;
	while(x != PLAYER_PED_SLOTS)
	{
		if(dwPlayerPedPtrs[x] == dwPedPtr) return x;
		x++;
	}
	return 0;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4390(BYTE bytePlayer, DWORD a2, DWORD a3, DWORD a4, DWORD a5)
{
	if(bytePlayer < PLAYER_PED_SLOTS)
	{
		VAR_1026C258[bytePlayer].field_0 = a2;
		VAR_1026C258[bytePlayer].field_4 = a3;
		VAR_1026C258[bytePlayer].field_8 = a4;
		VAR_1026C258[bytePlayer].field_C = a5;
	}
}

//-----------------------------------------------------------

struc_13 * __stdcall FUNC_100B43D0(BYTE bytePlayer)
{
	if(bytePlayer < PLAYER_PED_SLOTS)
	{		
		return &VAR_1026C258[bytePlayer];
	}
	return NULL;
}

//-----------------------------------------------------------

BYTE __stdcall FUNC_100B43F0(DWORD *pdwRenderWare)
{
	BYTE x = 0;
	while(x != PLAYER_PED_SLOTS)
	{
		ENTITY_TYPE *pEntity = (ENTITY_TYPE *)dwPlayerPedPtrs[x];
		if(pEntity)
		{
			if(pEntity->pdwRenderWare)
			{
				if(pEntity->pdwRenderWare == pdwRenderWare)
					return x;
			}
		}
		x++;
	}
	return 0;
}

//-----------------------------------------------------------



DWORD __stdcall CRC32FromUpcaseString(char *szString)
{
	DWORD dwResult = 0;

	_asm push szString
	_asm mov edx, 0x53CF30
	_asm call edx
	_asm mov dwResult, eax
	_asm pop edx

	return dwResult;
}





bool FUNC_100B4A70(VECTOR *vec)
{
	if( vec->X <= 1.0f && vec->X >= -1.0f &&
		vec->Y <= 1.0f && vec->Y >= -1.0f &&
		vec->Z <= 1.0f && vec->Z >= -1.0f )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------

bool FUNC_100B4B50(VECTOR *vecPos)
{
	if( vecPos->X < 20000.0f && vecPos->X > -20000.0f &&
		vecPos->Y < 20000.0f && vecPos->Y > -20000.0f &&
		vecPos->Z < 100000.0f && vecPos->Z > -10000.0f )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------

int FUNC_100B4BC0(int a1, int a2, int a3)
{
	_asm push a3
	_asm push a2
	_asm push a1
	_asm mov edx, 0x59C730
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

int FUNC_100B4BE0(int a1, int a2, int a3)
{
	_asm push a3
	_asm push a2
	_asm push a1
	_asm mov edx, 0x59C790
	_asm call edx
}

//-----------------------------------------------------------

int FUNC_100B4C00(int a1, int a2, int a3)
{
	_asm push a3
	_asm push a2
	_asm push a1
	_asm mov edx, 0x59C810
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}






//----------------------------------------------------

bool HasCollisionLoaded(VECTOR *vec)
{
	bool bResult = false;

	_asm push 0
	_asm push vec
	_asm mov edx, 0x410CE0
	_asm call edx
	_asm mov bResult, al
	_asm pop edx
	_asm pop edx

	return bResult;
}

//----------------------------------------------------

DWORD dwEntityModelIndex;

BOOL __stdcall IsATrainPart(ENTITY_TYPE *pEntity)
{
	int nModel;

	if(pEntity) {
		nModel = pEntity->nModelIndex;
		dwEntityModelIndex = pEntity->nModelIndex;
		if(nModel == TRAIN_PASSENGER_LOCO) return TRUE;
		if(nModel == TRAIN_PASSENGER) return TRUE;
		if(nModel == TRAIN_FREIGHT_LOCO) return TRUE;
		if(nModel == TRAIN_FREIGHT) return TRUE;
		if(nModel == TRAIN_TRAM) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float FloatDifference(float f1, float f2)
{
	return f1 - f2;
}

//-----------------------------------------------------------

float FloatOffset(float f1, float f2)
{
	if(f1 >= f2) return f1 - f2;
	else return (f2 - f1);
}

//-----------------------------------------------------------

float NormalizeAngle(float fAngle)
{
	if(fAngle > 180.0f) fAngle -= 360.0f;
	if(fAngle < -180.0f) fAngle += 360.0f;
	return fAngle;
}

//-----------------------------------------------------------

float __stdcall SquaredDistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2)
{
	float fSX,fSY;

	fSX = (x1 - x2) * (x1 - x2);
	fSY = (y1 - y2) * (y1 - y2);

	return fSX + fSY;
}

//-----------------------------------------------------------

float __stdcall DistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2)
{
	float fSX,fSY;

	fSX = (x1 - x2) * (x1 - x2);
	fSY = (y1 - y2) * (y1 - y2);

	return (float)sqrt(fSX + fSY);
}

//-----------------------------------------------------------

float __stdcall DistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float fSX,fSY,fSZ;

	fSX = (x1 - x2) * (x1 - x2);
	fSY = (y1 - y2) * (y1 - y2);
	fSZ = (z1 - z2) * (z1 - z2);

	return (float)sqrt(fSX + fSY + fSZ);
}

//----------------------------------------------------

float __stdcall SquaredDistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float fSX,fSY,fSZ;

	fSX = (x1 - x2) * (x1 - x2);
	fSY = (y1 - y2) * (y1 - y2);
	fSZ = (z1 - z2) * (z1 - z2);

	return fSX + fSY + fSZ;
}

//----------------------------------------------------

float __stdcall SquaredDistanceBetweenPoints(VECTOR *vec1, VECTOR *vec2)
{
	float fDX,fDY,fDZ;

	fDZ = vec1->Z - vec2->Z;
	fDY = vec1->Y - vec2->Y;
	fDX = vec1->X - vec2->X;

	return (fDX * fDX) + (fDY * fDY) + (fDZ * fDZ);
}

//----------------------------------------------------

void _VectorNormalise(VECTOR *vec)
{
	_asm mov ecx, vec
	_asm mov edx, 0x59C910
	_asm call edx
}

//----------------------------------------------------

void VectorCrossProduct(VECTOR *vecOut, VECTOR *vec1, VECTOR *vec2)
{
	float f1, f2;

	// each product needs its own statement; in one expression MSVC canonicalises
	// the commutative operands and picks the other side to fld
	f1 = vec2->Z * vec1->Y;
	f2 = vec2->Y * vec1->Z;
	vecOut->X = f1 - f2;
	f1 = vec2->X * vec1->Z;
	f2 = vec2->Z * vec1->X;
	vecOut->Y = f1 - f2;
	f1 = vec2->Y * vec1->X;
	f2 = vec2->X * vec1->Y;
	vecOut->Z = f1 - f2;
}

//----------------------------------------------------

void VectorNormaliseTo(VECTOR *vecOut, VECTOR *vecIn)
{
	float fInv = 1.0f / (float)sqrt((double)(vecIn->X * vecIn->X + vecIn->Y * vecIn->Y +
		vecIn->Z * vecIn->Z));

	vecOut->X = fInv * vecIn->X;
	vecOut->Y = fInv * vecIn->Y;
	vecOut->Z = fInv * vecIn->Z;
}

//----------------------------------------------------

float GetNormalisation(VECTOR *vec)
{
	return ((vec->X * vec->X) + (vec->Y * vec->Y) + (vec->Z * vec->Z));
}

//----------------------------------------------------
// newton step on the 0x5F3759D5 inverse-sqrt seed, scaled back up by x

float FastSqrt(float x)
{
	float fOrig = x;
	float fHalf = x * 0.5f;
	int i = *(int *)&x;

	i = 0x5F3759D5 - (i >> 1);
	x = *(float *)&i;

	return fOrig * (x * (1.5f - fHalf * x * x));
}

//----------------------------------------------------

float DegToRad(float fDegrees)
{
	if (fDegrees > 360.0f || fDegrees < 0.0f) return 0.0f;
	if (fDegrees > 180.0f) {
		return (float)(-(PI - (((fDegrees - 180.0f) * PI) / 180.0f)));
	} else {
		return (float)((fDegrees * PI) / 180.0f);
	}
}

//----------------------------------------------------

float InvDegToRad(float fDegrees)
{
	if (fDegrees > 360.0f || fDegrees < 0.0f) return 0.0f;
	if (fDegrees > 180.0f) {
		return (float)((-(PI - (((fDegrees - 180.0f) * PI) / 180.0f))) * -1.0f);
	} else {
		return (float)(((fDegrees * PI) / 180.0f) * -1.0f);
	}
}

//----------------------------------------------------

bool IsNumeric(char * szString)
{
	while(*szString) {
		if(*szString < '0' || *szString > '9') {
			return false;
		}
		szString++;
	}
	return true;
}

//----------------------------------------------------

DWORD CamFrameBuffer=0;
DWORD FrameBufferTexture=0;

extern CChatWindow * pChatWindow;

void _VehicleEnvMappingFromScreen()
{
	/*
	if(FrameBufferTexture) {
		_asm push FrameBufferTexture
		_asm mov edx, 0x7F3820
		_asm call edx
		_asm pop edx
		FrameBufferTexture = 0;
	}*/

	// get the frame buffer from the main RwCamera
	_asm mov eax, 0xC402D8
	_asm mov ebx, [eax]
	_asm mov CamFrameBuffer, ebx

	_asm push CamFrameBuffer
	_asm mov edx, 0x7F37C0 // RwTextureCreate
	_asm call edx
	_asm pop edx
	_asm mov FrameBufferTexture, eax

	if(!FrameBufferTexture) {
		pChatWindow->AddDebugMessage("No texture!");
	}

	*(DWORD *)0xB4E690 = FrameBufferTexture;
	*(DWORD *)0xB4E68C = FrameBufferTexture;
	*(DWORD *)0xB4E47C = FrameBufferTexture;
	*(DWORD *)0xB4E3EC = FrameBufferTexture;
	*(float *)0x8A7780 = 2.0f;
}

//----------------------------------------------------

int Width,Height,Depth;
DWORD dwRwImage;

void WriteRwRasterToFile(DWORD raster,char *filename)
{
	// RwImageCreate(width,height,depth)
	_asm push Depth
	_asm push Height
	_asm push Width
	_asm mov edx, 0x8026E0
	_asm call edx
	_asm mov dwRwImage, eax
	_asm pop edx
	_asm pop edx
	_asm pop edx

	// RwImageAllocatePixels
	_asm push dwRwImage
	_asm mov edx, 0x8027A0
	_asm call edx
	_asm pop edx

	// RwImageSetFromRaster
	_asm push raster
	_asm push dwRwImage
	_asm mov edx, 0x804250
	_asm call edx
	_asm pop edx
	_asm pop edx

	// RtBmpImageWrite
	_asm push filename
	_asm push dwRwImage
	_asm mov edx, 0x7CE990
	_asm call edx
	_asm pop edx
	_asm pop edx

	// RwImageDestroy
	_asm push dwRwImage
	_asm mov edx, 0x802740
	_asm call edx
	_asm pop edx
}

//----------------------------------------------------

DWORD CamFrameBuffer2=0;
DWORD CamZBuffer2=0;

void SaveCameraRaster(char *filename)
{
	// get the frame buffer from the main RwCamera
	//_asm mov ebx, 0xC1703C
	//_asm mov esi, [ebx]
	//_asm mov ebx, [esi+96]

	_asm mov eax, 0xC402D8
	_asm mov ebx, [eax]
	_asm mov CamFrameBuffer, ebx

	_asm mov esi, CamFrameBuffer
	_asm mov eax, [esi+20]
	_asm mov Depth, eax
	_asm mov eax, [esi+16]
	_asm mov Height, eax
	_asm mov eax, [esi+12]
	_asm mov Width, eax

	if(CamFrameBuffer) {
		WriteRwRasterToFile(CamFrameBuffer,"Raster1.bmp");
		pChatWindow->AddDebugMessage("Width %u Height: %u Depth: %u",Width,Height,Depth);
	}

	//_asm mov eax, 0xC7C71C
	//_asm mov ebx, [eax]
	//_asm mov CamFrameBuffer, ebx

	if(!CamFrameBuffer2) return;

	_asm mov esi, CamFrameBuffer2
	_asm mov eax, [esi+20]
	_asm mov Depth, eax
	_asm mov eax, [esi+16]
	_asm mov Height, eax
	_asm mov eax, [esi+12]
	_asm mov Width, eax

	if(CamFrameBuffer2) {
		WriteRwRasterToFile(CamFrameBuffer2,"Raster2.bmp");
		pChatWindow->AddDebugMessage("Width %u Height: %u Depth: %u",Width,Height,Depth);
	}
}

//----------------------------------------------------

void CreateCameraRaster()
{
	if(CamFrameBuffer2 || CamZBuffer2) return;

	_asm mov eax, 0xC402D8
	_asm mov ebx, [eax]
	_asm mov CamFrameBuffer, ebx

	_asm mov esi, CamFrameBuffer
	_asm mov eax, [esi+20]
	_asm mov Depth, eax
	_asm mov eax, [esi+16]
	_asm mov Height, eax
	_asm mov eax, [esi+12]
	_asm mov Width, eax

	// RwRasterCreate
	_asm push 5
	_asm push Depth
	_asm push 1024
	_asm push 2048
	_asm mov edx, 0x7FB230
	_asm call edx
	_asm mov CamFrameBuffer2, eax
	_asm add esp, 16

	// RwRasterCreate
	_asm push 1
	_asm push Depth
	_asm push 1024
	_asm push 2048
	_asm mov edx, 0x7FB230
	_asm call edx
	_asm mov CamZBuffer2, eax
	_asm add esp, 16
}

//----------------------------------------------------

DWORD CamFrameBufferSave;
DWORD CamZBufferSave;

DWORD *pRwSceneCamera = (DWORD *)0xC1703C;

void FUNC_100B5D20()
{
	DWORD dwCamera;

	_asm pushad

	CreateCameraRaster();

	dwCamera = *pRwSceneCamera;
	*(DWORD *)0xC9BCC0 = dwCamera;

	_asm mov ebx, dwCamera
	_asm mov edx, [ebx+0x60]
	_asm mov CamFrameBufferSave, edx
	_asm mov edx, [ebx+0x64]
	_asm mov CamZBufferSave, edx
	_asm mov edx, CamFrameBuffer2
	_asm mov [ebx+0x60], edx
	_asm mov edx, CamZBuffer2
	_asm mov [ebx+0x64], edx

	_asm mov edx, 0x734650
	_asm call edx
	_asm mov edx, 0x53DF40
	_asm call edx
	_asm mov edx, 0x732F30
	_asm call edx

	_asm mov ebx, dwCamera
	_asm mov edx, CamFrameBufferSave
	_asm mov [ebx+0x60], edx
	_asm mov edx, CamZBufferSave
	_asm mov [ebx+0x64], edx

	_asm popad
}

//----------------------------------------------------

void ResetLocalPad(int unk1, int unk2)
{
	// CPad__GetPadAt(int index)
	_asm push 0
	_asm mov edx, 0x53FB70
	_asm call edx
	_asm mov ecx, eax
	_asm pop edx

	// CPad__Reset(int unk1, int unk2)
	_asm push unk2
	_asm push unk1
	_asm mov edx, 0x541A70
	_asm call edx
}

//----------------------------------------------------

BOOL IsFileOrDirectoryExists(char * szPath)
{
	struct _stat buf;

	return _stat(szPath, &buf) == 0;
}

//----------------------------------------------------


void ReplaceUnprintableChars(char *szString)
{
	while(*szString)
	{
		if((BYTE)*szString > 0x7F || (BYTE)*szString < ' ')
			*szString = ' ';
		szString++;
	}
}

BOOL IsHexChar(char c)
{
	return c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f';
}

BOOL IsHexChar(wchar_t c)
{
	return c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f';
}

DWORD GetColorFromEmbedCode(char *szString)
{
	if(szString[0] && szString[0] == '{'
		&& szString[1] && (szString[1] >= '0' && szString[1] <= '9' || szString[1] >= 'A' && szString[1] <= 'F' || szString[1] >= 'a' && szString[1] <= 'f')
		&& szString[2] && IsHexChar(szString[2])
		&& szString[3] && IsHexChar(szString[3])
		&& szString[4] && IsHexChar(szString[4])
		&& szString[5] && IsHexChar(szString[5])
		&& szString[6] && IsHexChar(szString[6])
		&& szString[7] && szString[7] == '}')
	{
		char szBuffer[17];
		memset(szBuffer,0,sizeof(szBuffer));
		strncpy(szBuffer,szString+1,6);
		return strtoul(szBuffer,NULL,16);
	}
	return 0xFFFFFFFF;
}

DWORD GetColorFromEmbedCode(wchar_t *szString)
{
	if(szString[0] && szString[0] == '{'
		&& szString[1] && IsHexChar(szString[1])
		&& szString[2] && IsHexChar(szString[2])
		&& szString[3] && IsHexChar(szString[3])
		&& szString[4] && IsHexChar(szString[4])
		&& szString[5] && IsHexChar(szString[5])
		&& szString[6] && IsHexChar(szString[6])
		&& szString[7] && szString[7] == '}')
	{
		wchar_t szBuffer[17];
		memset(szBuffer,0,sizeof(szBuffer));
		wcsncpy(szBuffer,szString+1,6);
		return wcstoul(szBuffer,NULL,16);
	}
	return 0xFFFFFFFF;
}

void RemoveColorEmbedsFromString(char *szString)
{
	while(*szString)
	{
		if(GetColorFromEmbedCode(szString) != 0xFFFFFFFF)
		{
			strcpy(szString, szString + 8);
			continue;
		}
		szString++;
	}
	*szString = 0;
}

DWORD unnamed_100B6100(char *szString, int nMaxLen)
{
	char tmp_buf[2049];
	memset(tmp_buf, 0, sizeof(tmp_buf));

	if(szString &&
		strlen(szString) < 400 &&
		strstr(szString, "~k~") != NULL)
	{
		strcpy(tmp_buf, szString);

		_asm lea edx, tmp_buf
		_asm push edx
		_asm mov edx, 0x69E160
		_asm call edx
		_asm pop edx

		if((int)strlen(tmp_buf) > nMaxLen) return 0;

		strcpy(szString, tmp_buf);
		return strlen(tmp_buf);
	}
	return 0;
}

int FUNC_100B61D0(int a1)
{
	_asm mov eax, 0x745C70
	_asm push a1
	_asm call eax
	_asm pop edx
}

int __stdcall FUNC_100B64E0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
	_asm push a9
	_asm push a8
	_asm push a7
	_asm push a6
	_asm push a5
	_asm push a4
	_asm push a3
	_asm push a2
	_asm mov ecx, a1
	_asm mov edx, 0x73FB10
	_asm call edx
}

//-----------------------------------------------------------

int __stdcall FUNC_100B6510(int a1, int a2)
{
	_asm push 0
	_asm push 0
	_asm push a2
	_asm mov ecx, a1
	_asm mov edx, 0x73AAC0
	_asm call edx
}

//-----------------------------------------------------------

UINT GetVehicleSubtypeFromVehiclePtr(VEHICLE_TYPE *pVehicle)
{
	if(!pVehicle) return 0;
	if(pVehicle->entity.vtable == 0x871120) {
		return VEHICLE_SUBTYPE_CAR;
	}
	else if(pVehicle->entity.vtable == 0x8721A0) {
		return VEHICLE_SUBTYPE_BOAT;
	}
	else if(pVehicle->entity.vtable == 0x871360) {
		return VEHICLE_SUBTYPE_BIKE;
	}
	else if(pVehicle->entity.vtable == 0x871948) {
		return VEHICLE_SUBTYPE_PLANE;
	}
	else if(pVehicle->entity.vtable == 0x871680) {
		return VEHICLE_SUBTYPE_HELI;
	}
	else if(pVehicle->entity.vtable == 0x871528) {
		return VEHICLE_SUBTYPE_PUSHBIKE;
	}
	else if(pVehicle->entity.vtable == 0x872370) {
		return VEHICLE_SUBTYPE_TRAIN;
	}
	return 0;
}




//-----------------------------------------------------------

void FUNC_100B4D10(VECTOR *vecOut, MATRIX4X4 *pMatrix, VECTOR *vecOffset)
{
	vecOut->X = pMatrix->at.X * vecOffset->Z + pMatrix->up.X * vecOffset->Y +
		pMatrix->right.X * vecOffset->X + pMatrix->pos.X;
	vecOut->Y = ((pMatrix->at.Y * vecOffset->Z + pMatrix->up.Y * vecOffset->Y) +
		pMatrix->right.Y * vecOffset->X) + pMatrix->pos.Y;
	vecOut->Z = ((pMatrix->at.Z * vecOffset->Z + pMatrix->up.Z * vecOffset->Y) +
		pMatrix->right.Z * vecOffset->X) + pMatrix->pos.Z;
}

//-----------------------------------------------------------

DWORD unnamed_1026BBB0;

BOOL __stdcall FUNC_100B4860(VEHICLE_TYPE *pVehicle)
{
	BOOL bTowed = FALSE;

	if(pVehicle)
	{
		VEHICLE_TYPE *pTractor = (VEHICLE_TYPE *)pVehicle->dwTractor;
		if(pTractor)
		{
			unnamed_1026BBB0 = pVehicle->dwTractor;

			// towed by a vehicle somebody is sitting in
			PED_TYPE *pDriver = pTractor->pDriver;
			if(pDriver)
			{
				if(pDriver->dwStateFlags & 0x100) return TRUE;
			}
		}
	}

	return bTowed;
}

//-----------------------------------------------------------

DWORD VAR_1026BBB8[PLAYER_PED_SLOTS];

void __stdcall FUNC_100B44C0(BYTE bytePlayer, DWORD dwValue)
{
	VAR_1026BBB8[bytePlayer] = dwValue;
}

//-----------------------------------------------------------

DWORD __stdcall FUNC_100B4520(DWORD *pdw)
{
	return *pdw;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4530(DWORD *pdwVtable)
{
	if(pdwVtable && *pdwVtable == 0x85BBF0)
		*pdwVtable = 0x85BD30;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4550(DWORD *pdw, float fValue)
{
	*(float *)((BYTE *)pdw + 0x18) = fValue;
}

//-----------------------------------------------------------

float __stdcall FUNC_100B4560(DWORD *pdw)
{
	return *(float *)((BYTE *)pdw + 0x18);
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4660(int iModel, int nTxdIndex)
{
	DWORD *pModelInfo = GetModelInfo(iModel);

	_asm mov edx, pModelInfo
	_asm mov eax, nTxdIndex
	_asm mov [edx+0x0A], ax
}

//-----------------------------------------------------------

void FUNC_100B46C0(DWORD dwValue)
{
	*(DWORD *)0x9689E0 = dwValue;
}

//-----------------------------------------------------------

float FUNC_100B4FE0(float x, float y, float z)
{
	return (x * x) + (y * y) + (z * z);
}

//-----------------------------------------------------------

int FUNC_100B56D0(int a1, int a2, int a3)
{
	_asm push a3
	_asm push a2
	_asm push a1
	_asm mov edx, 0x7EDDC0
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

DWORD VAR_1026DF80;
DWORD VAR_1026DF84;
DWORD VAR_1026DF88;

void FUNC_100B5720()
{
	_asm pushad
	_asm push VAR_1026DF80
	_asm push VAR_1026DF84
	_asm mov edx, VAR_1026DF88
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm popad
}

//-----------------------------------------------------------

BOOL __stdcall FUNC_100B4570(int iModel)
{
	DWORD *pModelInfo = GetModelInfo(iModel);

	// retail tests the sum, so a compare against -20 emits the wrong shape
	if(pModelInfo && *pModelInfo + 20 != 0)
	{
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

DWORD __stdcall FUNC_100B45A0(int iModel)
{
	DWORD *pModelInfo = GetModelInfo(iModel);
	DWORD dwRet = 0;

	_asm mov eax, pModelInfo
	_asm mov edx, [eax+0x1C]
	_asm mov dwRet, edx

	return dwRet;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B45D0(int iModel, int a2)
{
	DWORD *pModelInfo = GetModelInfo(iModel);

	_asm mov ecx, pModelInfo
	_asm mov edx, a2
	_asm mov [ecx+0x1C], edx
	_asm push a2
	_asm mov edx, [ecx]
	_asm call dword ptr [edx+0x40]
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4600(int iModel, int a2)
{
	DWORD *pModelInfo = GetModelInfo(iModel);

	_asm mov ecx, pModelInfo
	_asm mov edx, a2
	_asm mov [ecx+0x1C], edx
	_asm push a2
	_asm mov edx, [ecx]
	_asm call dword ptr [edx+0x3C]
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4630(int iModel, int a2)
{
	DWORD *pModelInfo = GetModelInfo(iModel);

	_asm mov ecx, pModelInfo
	_asm push 0
	_asm push a2
	_asm mov edx, 0x4C4BC0
	_asm call edx
}

//-----------------------------------------------------------

DWORD * __stdcall FUNC_100B46D0(int iModel)
{
	DWORD *pModelInfo = GetModelInfo(iModel);
	DWORD *pRet = 0;

	_asm mov eax, pModelInfo
	_asm mov edx, [eax+0x14]
	_asm mov pRet, edx

	return pRet;
}

//-----------------------------------------------------------

WORD __stdcall FUNC_100B4700(int iModel)
{
	DWORD *pModelInfo = GetModelInfo(iModel);
	WORD wRet = 0;

	_asm mov edx, pModelInfo
	_asm mov bx, [edx+8]
	_asm mov wRet, bx

	return wRet;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B4730(int iModel, WORD wValue)
{
	DWORD *pModelInfo = GetModelInfo(iModel);

	if(pModelInfo)
	{
		_asm mov edx, pModelInfo
		_asm mov bx, wValue
		_asm mov [edx+8], bx
	}
}

//-----------------------------------------------------------

BOOL __stdcall FUNC_100B44E0(DWORD *pModelInfo)
{
	if(pModelInfo && (*pModelInfo == 0x85BBF0 || *pModelInfo == 0x85BC30 ||
		*pModelInfo == 0x85BC70 || *pModelInfo == 0x85BCB0 || *pModelInfo == 0x85BCF0))
	{
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

WORD FUNC_100B6470(int a1, int a2, int a3, int a4, int a5)
{
	if(!a1) return 0;

	WORD wRet = 0;

	_asm lea edx, a1
	_asm xor eax, eax
	_asm mov ax, [edx+0x12]
	_asm mov wRet, ax

	return wRet;
}

//-----------------------------------------------------------

void VectorScale(VECTOR *vecOut, VECTOR *vecIn, float fScale)
{
	vecOut->X = fScale * vecIn->X;
	vecOut->Y = fScale * vecIn->Y;
	vecOut->Z = fScale * vecIn->Z;
}

//-----------------------------------------------------------

void FUNC_100B51E0(int a1, int a2)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EB600 : 0x7EB5C0;

	_asm push a2
	_asm push a1
	_asm mov eax, dwFunc
	_asm call eax
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

float FUNC_100B5180(float *pQuat)
{
	return pQuat[0] * pQuat[0] + (pQuat[1] * pQuat[1] + pQuat[2] * pQuat[2] +
		pQuat[3] * pQuat[3]);
}

//-----------------------------------------------------------

void FUNC_100B5910(int a1, int a2)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F1960 : 0x7F1920;

	_asm push a2
	_asm push a1
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

void FUNC_100B5940(int a1, int a2)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F2300 : 0x7F22C0;

	_asm push 1
	_asm push a2
	_asm push a1
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

void FUNC_100B58D0(int a1, int a2, int a3)
{
	int nCopy = a2;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F2010 : 0x7F1FD0;

	_asm push 1
	_asm push a3
	_asm push nCopy
	_asm push a1
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

// patched to the EU entry point on first use, so it has to be a real variable
DWORD VAR_101173FC = 0x7EDD90;

void FUNC_100B56F0(int a1, int a2, int a3)
{
	if(iGtaVersion == GTASA_VERSION_EU10)
		VAR_101173FC = 0x7EDDD0;

	_asm push a3
	_asm push 1
	_asm push a2
	_asm push a1
	_asm mov edx, VAR_101173FC
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

void FUNC_100B62B0(BYTE *pObject)
{
	if(pObject)
	{
		if(*pObject == 1)
		{
			DWORD dwOther = *(DWORD *)(pObject + 4);

			FUNC_100B1800(pObject);

			if(dwOther) FUNC_100B17D0(dwOther);
		}
		else if(*pObject == 2)
		{
			FUNC_100B1C80(pObject);
		}
	}
}

//-----------------------------------------------------------

BOOL __stdcall FUNC_100B62F0(BYTE *pObject)
{
	if(!pObject || *pObject != 1) return FALSE;

	DWORD dwFunc = (DWORD)FUNC_100B1430(pObject);

	_asm push pObject
	_asm mov ebx, dwFunc
	_asm call ebx
	_asm pop ebx

	return TRUE;
}

//-----------------------------------------------------------

void __stdcall FUNC_100B6330(BYTE *pObject)
{
	if(pObject)
	{
		if(*pObject == 1)
			FUNC_100B62F0(pObject);
		else if(*pObject == 2)
			FUNC_100B19D0(pObject);
	}
}

//-----------------------------------------------------------

void FUNC_100B5260(MATRIX4X4 *pMatrix, float *pQuatOut)
{
	FUNC_100B4D70(pMatrix->right.X, pMatrix->right.Y, pMatrix->right.Z,
		pMatrix->up.X, pMatrix->up.Y, pMatrix->up.Z,
		pMatrix->at.X, pMatrix->at.Y, pMatrix->at.Z,
		&pQuatOut[0], &pQuatOut[1], &pQuatOut[2], &pQuatOut[3]);
}

//-----------------------------------------------------------

void FUNC_100B5420(float *pQuat, MATRIX4X4 *pMatrix)
{
	FUNC_100B4F10(pQuat[0], pQuat[1], pQuat[2], pQuat[3],
		&pMatrix->right.X, &pMatrix->right.Y, &pMatrix->right.Z,
		&pMatrix->up.X, &pMatrix->up.Y, &pMatrix->up.Z,
		&pMatrix->at.X, &pMatrix->at.Y, &pMatrix->at.Z);
}

//-----------------------------------------------------------

VECTOR VAR_10117400[3];
DWORD VAR_10117424;

void FUNC_100B57E0(int a1, int nIndex, int a3, int a4)
{
	VECTOR *pEntry = &VAR_10117400[nIndex];
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EB800 : 0x7EB7C0;

	_asm push a4
	_asm push a3
	_asm push pEntry
	_asm push a1
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

void FUNC_100B5790(int a1, int nIndex, int a3)
{
	VECTOR *pEntry = &VAR_10117400[nIndex];

	VAR_10117424 = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F2010 : 0x7F1FD0;

	_asm push 1
	_asm push a3
	_asm push pEntry
	_asm push a1
	_asm mov ebx, VAR_10117424
	_asm call ebx
	_asm pop ebx
	_asm pop ebx
	_asm pop ebx
	_asm pop ebx
}

//-----------------------------------------------------------

void FUNC_100B5740(int a1, int a2)
{
	VAR_1026DF88 = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F20B0 : 0x7F2070;
	VAR_1026DF80 = a2;
	VAR_1026DF84 = a1;

	_asm pushad
	_asm push VAR_1026DF80
	_asm push VAR_1026DF84
	_asm mov edx, VAR_1026DF88
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm popad
}

//-----------------------------------------------------------
// same body as FUNC_100B5260; retail keeps both copies

void FUNC_100B52B0(MATRIX4X4 *pMatrix, float *pQuatOut)
{
	FUNC_100B4D70(pMatrix->right.X, pMatrix->right.Y, pMatrix->right.Z,
		pMatrix->up.X, pMatrix->up.Y, pMatrix->up.Z,
		pMatrix->at.X, pMatrix->at.Y, pMatrix->at.Z,
		&pQuatOut[0], &pQuatOut[1], &pQuatOut[2], &pQuatOut[3]);
}

//-----------------------------------------------------------

void FUNC_100B5210(int a1, void *a2)
{
	float quat[4];
	float *pQuat;
	DWORD dwFunc;

	pQuat = quat;
	dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EB600 : 0x7EB5C0;

	_asm push a1
	_asm push pQuat
	_asm mov eax, dwFunc
	_asm call eax
	_asm pop edx
	_asm pop edx

	((float *)a2)[0] = quat[3];
	((float *)a2)[1] = quat[0];
	((float *)a2)[2] = quat[1];
	((float *)a2)[3] = quat[2];
}

//-----------------------------------------------------------

void FUNC_100B5500(float *pQuat)
{
	D3DXQUATERNION qOut;
	D3DXQUATERNION qIn;

	qIn.w = pQuat[0];
	qIn.x = pQuat[1];
	qIn.y = pQuat[2];
	qIn.z = pQuat[3];

	D3DXQuaternionNormalize(&qOut, &qIn);

	pQuat[0] = qOut.w;
	pQuat[1] = qOut.x;
	pQuat[2] = qOut.y;
	pQuat[3] = qOut.z;
}

//-----------------------------------------------------------

void FUNC_100B58A0(DWORD dwFrame, VECTOR *vecOut, int *a3)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EBAC0 : 0x7EBA80;

	_asm push a3
	_asm push vecOut
	_asm push dwFrame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

//-----------------------------------------------------------

VECTOR VAR_1026DF90;
VECTOR VAR_1026DF9C;
float VAR_1026CF7C;

void FUNC_100B5830(int a1, int nIndex, float *pfOut)
{
	VAR_1026DF9C = VAR_10117400[nIndex];

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F2760 : 0x7F2720;

	_asm lea eax, VAR_1026DF90
	_asm push eax
	_asm lea eax, VAR_1026CF7C
	_asm push eax
	_asm lea eax, VAR_1026DF9C
	_asm push eax
	_asm push a1
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx

	*pfOut = VAR_1026CF7C;
}

//-----------------------------------------------------------

void FUNC_100B5480(float *pQuatOut, float *pQuatFrom, float *pQuatTo, float fT)
{
	D3DXQUATERNION qOut;
	D3DXQUATERNION q1;
	D3DXQUATERNION q2;

	q1.w = pQuatFrom[0];
	q1.x = pQuatFrom[1];
	q1.y = pQuatFrom[2];
	q1.z = pQuatFrom[3];

	q2.w = pQuatTo[0];
	q2.x = pQuatTo[1];
	q2.y = pQuatTo[2];
	q2.z = pQuatTo[3];

	D3DXQuaternionSlerp(&qOut, &q1, &q2, fT);

	pQuatOut[0] = qOut.w;
	pQuatOut[1] = qOut.x;
	pQuatOut[2] = qOut.y;
	pQuatOut[3] = qOut.z;
}

//-----------------------------------------------------------

void FUNC_100B5620(float *pQuat, float *pEuler)
{
	float f0 = pQuat[0] * pQuat[0];
	float f1 = pQuat[1] * pQuat[1];
	float f2 = pQuat[2] * pQuat[2];
	float f3 = pQuat[3] * pQuat[3];

	pEuler[0] = (float)atan2((pQuat[2] * pQuat[1] + pQuat[3] * pQuat[0]) * 2.0f,
		f1 - f2 - f3 + f0);
	pEuler[1] = (float)asin((pQuat[3] * pQuat[1] - pQuat[2] * pQuat[0]) * -2.0f);
	pEuler[2] = (float)atan2((pQuat[3] * pQuat[2] + pQuat[1] * pQuat[0]) * 2.0f,
		f3 - (f2 + f1) + f0);
}
