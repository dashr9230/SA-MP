
#include <windows.h>
#include <stdio.h>
#include "../main.h"
#include <sys/stat.h>

DWORD dwPlayerPedPtrs[PLAYER_PED_SLOTS];

struct struc_13
{
	char _gap0[16];
};
struc_13 VAR_1026C258[PLAYER_PED_SLOTS];

#define PI 3.14159265f

//-----------------------------------------------------------

void __declspec(naked) ProcessLineOfSight(VECTOR *vecOrigin, VECTOR *vecLine, VECTOR *colPoint,
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

//-----------------------------------------------------------

void ReplaceBuildingModel(ENTITY_TYPE *pEntity, int iModelID)
{
	_asm push iModelID
	_asm mov ecx, pEntity
	_asm mov edx, 0x403EC0
	_asm call edx
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

float GetNormalisation(VECTOR *vec)
{
	return ((vec->X * vec->X) + (vec->Y * vec->Y) + (vec->Z * vec->Z));
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
		&& szString[1] && IsHexChar(szString[1])
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



