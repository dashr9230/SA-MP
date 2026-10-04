
#pragma once

#include "game.h"
#include "aimstuff.h"
#include "entity.h"

struct struc_13;

// where a synced shot came from, snapshotted by CPlayerPed::FUNC_100AFA70
struct SHOT_SYNC_DATA
{
	DWORD			field_0;
	VECTOR			vecOrigin;
	VECTOR			vecColPoint;
	VECTOR			vecOffset;
	ENTITY_TYPE		*pAttachedTo;
};

DWORD __stdcall GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle);
void FUNC_100AE520(); // .text:100AE520

struct struc_97
{
	char _gap0[52];
};

//-----------------------------------------------------------

class CPlayerPed : public CEntity
{
public:

	void ResetPointers();
	void SetInitialState();

	void  SetKeys(WORD wKeys, WORD lrAnalog, WORD udAnalog);
	WORD  GetKeys(WORD * lrAnalog, WORD * udAnalog);

	BYTE GetSpecialKey();

	CAMERA_AIM * GetCurrentAim();
	void SetCurrentAim(CAMERA_AIM *pAim);

	BYTE GetCameraMode() {
		if(!m_bytePlayerNumber)
			return GameGetLocalPlayerCameraMode();
		else
			return GameGetPlayerCameraMode(m_bytePlayerNumber);
	};

	void SetCameraMode(BYTE byteMode) { // .text:10014340
		GameSetPlayerCameraMode(byteMode, m_bytePlayerNumber);
	};

	void SetCameraExtZoom(float fZoom, float fRatio) { // .text:10014360
		GameSetPlayerCameraExtZoom(m_bytePlayerNumber, fZoom, fRatio);
	};

	void  SetInterior(BYTE byteInterior, BOOL bRefresh); // .text:100AD340
	void  ShowMarker(int iMarkerColorID);
	void  HideMarker();
	BYTE  GetCurrentWeapon();
	int   GetCurrentVehicleID() {
		if(!m_pPed) return 0;
		VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)m_pPed->pVehicle;
		return GamePool_Vehicle_GetIndex(pVehicle);
	};
	BOOL  IsOnScreen();
	float GetHealth();
	void  SetHealth(float fHealth);
	float GetArmour();
	void  SetArmour(float fArmour);

	void  SetSkin(int iSkin); // .text:100AFF50
	void  FUNC_100ABBD0(); // .text:100ABBD0
	void  FUNC_100ABD70(float a1); // .text:100ABD70
	void  FUNC_100AC010(); // .text:100AC010
	void  FUNC_100AC790(int a1); // .text:100AC790
	BYTE  FUNC_100AC850(WORD *pKiller); // .text:100AC850
	DWORD GetStateFlags();
	void  SetStateFlags(DWORD dwStateFlags);
	BOOL  IsDead();
	BOOL  IsInVehicle();
	BOOL  FUNC_100AC5D0(); // .text:100AC5D0
	BOOL  FUNC_100AC640(); // .text:100AC640
	BYTE  GetActionTrigger();
	void  SetActionTrigger(BYTE byteTrigger);

	WORD  GetAmmo();

	int   GetFightingStyle();
	void  SetFightingStyle(int iStyle);

	float GetTargetRotation();
	void  SetTargetRotation(float fRotation);
	void  ForceTargetRotation(float fRotation);

	void GiveWeapon(int iWeaponID, int iAmmo);
	void SetArmedWeapon(int iWeaponType, bool bUnk);
	void FUNC_100AC290(int iGtaVehicleID, int iSeat); // .text:100AC290
	void ClearAllWeapons();
	void RemoveWeaponWhenEnteringVehicle();
	WEAPON_SLOT_TYPE * GetCurrentWeaponSlot();
	WEAPON_SLOT_TYPE * FindWeaponSlot(DWORD dwWeapon);
	BOOL HasAmmoForCurrentWeapon();

	void SetImmunities(BOOL bBullet, BOOL bFire, BOOL bExplosion, BOOL bDamage, BOOL bUnknown);

	void PutDirectlyInVehicle(int iVehicleID, int iSeat);
	void EnterVehicle(int iVehicleID, BOOL bPassenger);
	void ExitCurrentVehicle();
	void RemoveFromVehicleAndPutAt(float fX, float fY, float fZ);

	BOOL IsAPassenger();

	VEHICLE_TYPE * GetGtaVehicle();
	VEHICLE_TYPE * GetGtaContactVehicle();
	ENTITY_TYPE * GetGtaContactEntity();

	int GetVehicleSeatID();
	void TogglePlayerControllable(int iControllable);

	float GetDistanceFromVehicle(CVehicle *pVehicle);

	void StartJetpack();
	void StopJetpack();
	BOOL IsInJetpackMode();

	void StartGoggles();
	void StopGoggles();
	BOOL HasGoggles();

	void SetAnimationSet(PCHAR szAnim);
	void SetMoney(int iAmount);
	void ApplyAnimation(char *szAnimName, char *szAnimFile, float fT,
						int opt1, int opt2, int opt3, int opt4, int iUnk);

	BOOL IsPerformingAnimation(char *szAnimName);

	CPlayerPed();
	~CPlayerPed();

	int			sub_100ADFA0(); // .text:100ADFA0
	BOOL		FUNC_100ABFC0(); // .text:100ABFC0
	BYTE		FUNC_100ABC50(); // .text:100ABC50
	void		FUNC_100AC410(DWORD dwGtaId, int a2); // .text:100AC410
	void		FUNC_100ADFB0(int iLevel); // .text:100ADFB0
	int			FUNC_100ADC90(); // .text:100ADC90
	void		FUNC_100ADFD0(char *szName, int a2, int a3, int a4, VECTOR *pvecPos, int a6, float a7, int a8, int a9, int a10); // .text:100ADFD0
	void		FUNC_100ABF10(float fAngle); // .text:100ABF10


	void		ProcessVehicleHorn();

	void		StartDancing(int iStyle);
	void		StopDancing();
	BOOL		IsDancing();
	void		HandsUp();
	BOOL		HasHandsUp();
	void		HoldItem(int iObject);

	void		ProcessMarkers(BOOL bMarkerStreamingEnabled, float fMarkerStreamRadius, BOOL bVisible);

	void		ApplyCommandTask(char *szTaskName, int p1, int p2, int p3, 
								VECTOR *p4, int p5, float p6, int p7, int p8, int p9);

	void		DestroyFollowPedTask();
	void		ToggleCellphone(int iOn);
    int			IsCellphoneEnabled();

	BOOL		IsJumping();
	BOOL		IsFighting();
	BOOL		IsFalling();
	BOOL		IsSwimming();

	float		GetAimZ();
	void		SetAimZ(float fAimZ);

	PCHAR		GetLoadedShopName();
	void		LoadShoppingDataSubsection(PCHAR szSectionName);
	PCHAR		GetShopName();
	void		SetShopName(char *szNewShopName);

	void		SetWeaponSkillLevel(int iSkill, WORD wLevel);
	void		SetWeaponAmmo(BYTE byteWeapon, WORD wAmmo); // .text:100B0080

	VECTOR*		GetBonePosition(int iBone, VECTOR *vecPos);
	VECTOR*		GetTransformedBonePosition(int iBone, VECTOR *vecOffset);

	BOOL		FUNC_100AFA70(); // .text:100AFA70
	void		FUNC_100AFC70(VECTOR *vecIn, VECTOR *vecOut); // .text:100AFC70
	BOOL		FUNC_100ACEF0(); // .text:100ACEF0
	void		FUNC_100AF280(void *pShotSync); // .text:100AF280
	void		FUNC_100AEEE0(MATRIX4X4 *pMatrix); // .text:100AEEE0

	BOOL		HasObjectAttached(); // .text:100AEE30
	void		RemoveAttachedObject(int iIndex); // .text:100AEE50
	void		FUNC_100B0B10(int iIndex, char *pAttachData); // .text:100B0B10
	BOOL		GetAttachedObjectSlotState(int iIndex); // .text:100AEEB0
	void		RemoveAllAttachedObjects(); // .text:100B0AB0
	void		StopCarrying(); // .text:100ADC00
	void		Destroy(); // .text:100B0FA0
	void		ProcessDrunkCamera(); // .text:100ADDF0
	void		FUNC_100AD0F0(); // .text:100AD0F0
	void		FUNC_100AFFD0(); // .text:100AFFD0

	int   FUNC_100AE260(); // .text:100AE260
	int   FUNC_100ACDC0(); // .text:100ACDC0
	int   FUNC_100ACB60(); // .text:100ACB60
	int   FUNC_100ADB60(); // .text:100ADB60
	int   FUNC_100AE0A0(); // .text:100AE0A0
	void  FUNC_100AE100(); // .text:100AE100
	void  FUNC_100ADCA0(); // .text:100ADCA0
	void  FUNC_100B02D0(int a1); // .text:100B02D0
	BOOL  FUNC_100ACE40(); // .text:100ACE40
	void  FUNC_100ACF00(int a1); // .text:100ACF00
	BOOL  FUNC_100AD860(); // .text:100AD860
	int   FUNC_100ABDD0(); // .text:100ABDD0
	void  FUNC_100ABDF0(int a1); // .text:100ABDF0
	void  FUNC_100AD880(); // .text:100AD880
	void  FUNC_100AC720(); // .text:100AC720
	void  FUNC_100AEC90(); // .text:100AEC90
	int   FUNC_100AEF60(); // .text:100AEF60

	int   FUNC_100AC690(); // .text:100AC690
	void  FUNC_100ACE50(int a1); // .text:100ACE50
	BOOL  FUNC_100ADA00(); // .text:100ADA00
	void  FUNC_100AD3E0(); // .text:100AD3E0
	BOOL  FUNC_100AD030(); // .text:100AD030
	void  FUNC_100AD080(); // .text:100AD080
	int   FUNC_100AE960(int a1); // .text:100AE960
	void  FUNC_100AD8B0(int iWeapon); // .text:100AD8B0
	struc_13 * FUNC_100AE5D0(); // .text:100AE5D0

	DWORD FUNC_100AEBB0(int a1); // .text:100AEBB0
	void  FUNC_100AE560(int iType, int a2, int a3, int a4); // .text:100AE560

	DWORD FUNC_100AE990(); // .text:100AE990
	DWORD FUNC_100AE9E0(); // .text:100AE9E0
	DWORD FUNC_100AEA30(); // .text:100AEA30
	void  FUNC_100AC6C0(); // .text:100AC6C0

	void  FUNC_100AE1E0(); // .text:100AE1E0

	void  FUNC_100AEA80(MATRIX4X4 *pMatrix, int iFrame); // .text:100AEA80
	void  FUNC_100AEB10(MATRIX4X4 *pMatrix, int iFrame); // .text:100AEB10

	void  FUNC_100ACE90(); // .text:100ACE90

	void  FUNC_100AEC10(MATRIX4X4 *pMatrix, int iBone); // .text:100AEC10
	void  FUNC_100AEC50(MATRIX4X4 *pMatrix, int iBone); // .text:100AEC50

	int		FUNC_100AD9C0(); // .text:100AD9C0
	int		FUNC_100AD9E0(); // .text:100AD9E0
	void		FUNC_100AF1F0(); // .text:100AF1F0
	void		FUNC_100AEF90(); // .text:100AEF90
	void		FUNC_100AE530(int a1, int a2); // .text:100AE530

	float		FUNC_100AF220(); // .text:100AF220
	DWORD		FUNC_100AE790(); // .text:100AE790
	DWORD *		FUNC_100AE6E0(); // .text:100AE6E0
	DWORD		FUNC_100AE5F0(DWORD *pTask); // .text:100AE5F0
	DWORD		FUNC_100AE6A0(); // .text:100AE6A0

	void		FUNC_100AD440(); // .text:100AD440

	char *		FUNC_100ADB70(int iIndex); // .text:100ADB70

	int			m_iCellPhoneEnabled;
	int field_4C[10];
	struc_97 field_74[10];
	int field_27C[10];
	PED_TYPE    *m_pPed;
	int field_2A8;
	int field_2AC;
	BYTE		m_bytePlayerNumber;
	char _gap2B1[8];
	int field_2B9;
	int field_2BD;
	int field_2C1;
	int field_2C5;
	int field_2C9;
	DWORD 		m_dwArrow;
	char _gap2D1;
	int			m_iDanceState;
	int			m_iDanceStyle;
	char _gap2DA[4];
	int field_2DE;
	int field_2E2;
	BOOL		m_bGoggleState;
	char _gap2EA[8];
	int field_2F2;
	char field_2F6;
	char field_2F7;
	char field_2F8;
	int field_2F9;
	char field_2FD[44];
	int field_329;

};

//-----------------------------------------------------------
