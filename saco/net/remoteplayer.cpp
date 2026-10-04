
#include "../main.h"
#include "../game/util.h"

extern CGame		 *pGame;

using namespace RakNet;
extern CNetGame* pNetGame;
extern CChatWindow *pChatWindow;

float fPosLerp = 0.1f;
float fVehLerp = 0.06f;
int bClampVehicleTurnSpeed = 1;

//----------------------------------------------------
// layout locks: a negative array size fails the build if any offset moves

typedef char CRemotePlayer_offsets[(
	offsetof(CRemotePlayer, field_C5) == 0xC5 &&
	offsetof(CRemotePlayer, field_C5.quat) == 0xD7 &&
	offsetof(CRemotePlayer, field_C5.vecOffset) == 0xF7 &&
	offsetof(CRemotePlayer, field_C5.wVehicleID) == 0x103 &&
	sizeof(((CRemotePlayer *)0)->field_C5) == 68 &&
	offsetof(CRemotePlayer, field_109) == 0x109 &&
	offsetof(CRemotePlayer, field_10A) == 0x10A &&
	offsetof(CRemotePlayer, field_10B) == 0x10B &&
	offsetof(CRemotePlayer, field_10C) == 0x10C &&
	offsetof(CRemotePlayer, field_160) == 0x160 &&
	offsetof(CRemotePlayer, field_17C) == 0x17C &&
	offsetof(CRemotePlayer, field_188) == 0x188 &&
	offsetof(CRemotePlayer, field_194) == 0x194 &&
	offsetof(CRemotePlayer, field_1A0) == 0x1A0 &&
	offsetof(CRemotePlayer, field_1AC) == 0x1AC &&
	offsetof(CRemotePlayer, field_1C5) == 0x1C5 &&
	offsetof(CRemotePlayer, field_1C9) == 0x1C9 &&
	offsetof(CRemotePlayer, m_pPlayerPed) == 0x1DD &&
	offsetof(CRemotePlayer, field_1E1) == 0x1E1 &&
	offsetof(CRemotePlayer, m_PlayerID) == 0x1E5 &&
	offsetof(CRemotePlayer, field_1E7) == 0x1E7 &&
	offsetof(CRemotePlayer, field_1F9) == 0x1F9) ? 1 : -1];

typedef char CNetPlayer_offsets[(offsetof(CNetPlayer, m_PlayerName) == 0x14) ? 1 : -1];
typedef char BULLET_SYNC_offsets[(offsetof(BULLET_SYNC_DATA, byteWeaponID) == 0x27 &&
	sizeof(BULLET_SYNC_DATA) == 0x28) ? 1 : -1];
typedef char TRAILER_SYNC_offsets[(offsetof(TRAILER_SYNC_DATA, vecTurnSpeed) == 0x2A &&
	sizeof(TRAILER_SYNC_DATA) == 0x36) ? 1 : -1];
typedef char UNOCCUPIED_SYNC_offsets[(offsetof(UNOCCUPIED_SYNC_DATA, vecTurnSpeed) == 0x33 &&
	offsetof(CVehiclePool, field_8E34) == 0x8E34 && offsetof(CVehicle, field_83) == 0x83) ? 1 : -1];
typedef char struc_41_offsets[(offsetof(struc_41, field_30) == 0x30) ? 1 : -1];
typedef char WEAPON_SLOT_offsets[(offsetof(WEAPON_SLOT_TYPE, field_8) == 8 &&
	sizeof(WEAPON_SLOT_TYPE) == 28) ? 1 : -1];
typedef char CPlayerPool_offsets[(offsetof(CPlayerPool, field_6) == 6 &&
	sizeof(std::string) == 0x1C) ? 1 : -1];


//----------------------------------------------------

CRemotePlayer::CRemotePlayer()
{
	field_1E7 = 0;
	field_10A = 0;
	m_PlayerID = INVALID_PLAYER_ID;
	m_pPlayerPed = NULL;
	field_1B8 = 0;
	field_109 = -1;
	field_4 = 1;
	field_0 = 0;
	field_8 = 0;
	field_1F9 = 0;
	field_C = 0;
	field_10C = 0;
	field_1C1 = 0;
	field_1D5 = GetTickCount();
	field_1D9 = GetTickCount();
	field_1B9 = GetTickCount();
	ResetAllSyncAttributes();
}

//----------------------------------------------------

void CRemotePlayer::ResetAllSyncAttributes()
{
	field_1E7 = 0;
	field_1B0 = 0;
	field_1AC = 0;
	field_1E1 = 0;
	field_10B = 0;
	field_110 = 0;
	field_1ED = 0;
	field_1F1 = 0;
	field_1F5 = 0;
	field_C = 0;
	field_1B4 = 0;
	field_1C5 = 2;
	memset(&field_C5, 0, sizeof(field_C5));
	memset(field_19, 0, sizeof(field_19));
	memset(field_AD, 0, sizeof(field_AD));
	memset(field_8E, 0, sizeof(field_8E));
	memset(&field_1C9, 0, sizeof(field_1C9));
	if(field_1F9)
	{
		pGame->DisableMarker(field_1F9);
		field_1F9 = 0;
	}
	field_1E9 = 0;
	if(pNetGame)
		field_1BD = pNetGame->GetTime();
}

//----------------------------------------------------

void CRemotePlayer::SetPlayerColor(DWORD dwColor)
{
	SetRadarColor(m_PlayerID,dwColor);
}

//----------------------------------------------------

DWORD CRemotePlayer::GetPlayerColorAsRGBA()
{
	return TranslateColorCodeToRGBA(m_PlayerID);
}

//----------------------------------------------------

DWORD CRemotePlayer::GetPlayerColorAsARGB()
{
	return (TranslateColorCodeToRGBA(m_PlayerID) >> 8) | 0xFF000000;
}

//----------------------------------------------------

void CRemotePlayer::EnterVehicle(VEHICLEID VehicleID, BOOL bPassenger)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle *pVehicle = (CVehicle *)pVehiclePool->GetAt(VehicleID);

	if(m_pPlayerPed && pVehicle && !m_pPlayerPed->IsInVehicle()) {
		int iGtaVehicleID = pVehiclePool->FindGtaIDFromID(VehicleID);
		if(iGtaVehicleID && iGtaVehicleID != INVALID_VEHICLE_ID) {
			m_pPlayerPed->SetKeys(0,0,0);
			m_pPlayerPed->EnterVehicle(iGtaVehicleID,bPassenger);
		}
	}
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10017530()
{
	if(m_pPlayerPed) {
		ResetAllSyncAttributes();
		pGame->DeletePlayerPed((int)m_pPlayerPed);
		m_pPlayerPed = NULL;
	}
	field_10A = 0;
	field_4 = 1;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10017570()
{
	if(m_pPlayerPed) {
		m_pPlayerPed->SetKeys(0,0,0);
		m_pPlayerPed->FUNC_100AFFD0();
	}
	if(field_10A != 32) field_10A = 32;
	ResetAllSyncAttributes();
}

//----------------------------------------------------

void CRemotePlayer::FUNC_100143A0()
{
	if(!m_pPlayerPed) return;
	if(!m_pPlayerPed->IsAdded()) return;
	if(FUNC_10014390()) return;
	if(pGame && !pGame->m_bHeadMove) return;
	if(m_pPlayerPed->FUNC_100ADC90()) return;

	MATRIX4X4 mat;
	m_pPlayerPed->GetMatrix(&mat);

	DWORD dwTick = GetTickCount();

	if((dwTick - field_1D5) > 1000) {
		CAMERA_AIM *pAim = GameGetRemotePlayerAim(m_pPlayerPed->m_bytePlayerNumber);
		VECTOR vecAim;
		vecAim.X = pAim->f1x * 20.0f + mat.pos.X;
		vecAim.Y = pAim->f1y * 20.0f + mat.pos.Y;
		vecAim.Z = pAim->f1z * 20.0f + mat.pos.Z;
		field_1D5 = dwTick;
		field_1C9.X = vecAim.X - mat.pos.X;
		field_1C9.Y = vecAim.Y - mat.pos.Y;
		field_1C9.Z = vecAim.Z - mat.pos.Z;
	}

	if((dwTick - field_1D9) > 200) {
		VECTOR vec;
		vec.X = mat.pos.X + field_1C9.X;
		vec.Y = mat.pos.Y + field_1C9.Y;
		vec.Z = mat.pos.Z + field_1C9.Z;
		m_pPlayerPed->ApplyCommandTask("FollowPedSA", 0, 1000, -1, &vec, 0, 0.1f, 500, 3, 0);
		field_1D9 = dwTick;
	}
}

//----------------------------------------------------

void CRemotePlayer::FUNC_100150D0(VECTOR *pPos, VECTOR *pMoveSpeed)
{
	field_17C.X = pPos->X;
	field_17C.Y = pPos->Y;
	field_17C.Z = pPos->Z;
	field_188.X = pMoveSpeed->X;
	field_188.Y = pMoveSpeed->Y;
	field_188.Z = pMoveSpeed->Z;
	m_pPlayerPed->SetMoveSpeedVector(field_188);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_100155E0(int a1, VECTOR *pPos, VECTOR *pMoveSpeed)
{
	FUNC_100B5210(a1, quat16C);
	field_194 = *pPos;
	field_1A0 = *pMoveSpeed;
	field_1E1->SetMoveSpeedVector(*pMoveSpeed);
}

//----------------------------------------------------

float CRemotePlayer::FUNC_100160A0(CRemotePlayer *pOther)
{
	if(!m_pPlayerPed) return 10000.0f;

	MATRIX4X4 matThis, matOther;
	m_pPlayerPed->GetMatrix(&matThis);
	pOther->m_pPlayerPed->GetMatrix(&matOther);

	return sqrt((matThis.pos.X - matOther.pos.X) * (matThis.pos.X - matOther.pos.X) +
		(matThis.pos.Y - matOther.pos.Y) * (matThis.pos.Y - matOther.pos.Y) +
		(matThis.pos.Z - matOther.pos.Z) * (matThis.pos.Z - matOther.pos.Z));
}

//----------------------------------------------------

float CRemotePlayer::FUNC_10016120()
{
	CEntity *pEntity = m_pPlayerPed;
	if(!pEntity) return 10000.0f;

	if(field_10A == 19 && field_1E1) pEntity = field_1E1;

	return pEntity->FUNC_1009F0C0();
}

//----------------------------------------------------

int CRemotePlayer::FUNC_10016330()
{
	if(m_pPlayerPed && m_pPlayerPed->IsAdded() && !m_pPlayerPed->FUNC_100AC5D0() &&
		!m_pPlayerPed->FUNC_100AC640()) return field_1C5;

	return 0;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10017610(char *szText)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	pChatWindow->AddChatMessage(pPlayerPool->GetPlayerName(m_PlayerID),
		(TranslateColorCodeToRGBA(m_PlayerID) >> 8) | 0xFF000000, szText);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014540(short a1, short a2, short a3)
{
	field_1E9 = 1;

	if(field_1F9) {
		pGame->DisableMarker(field_1F9);
		field_1F9 = 0;
	}

	if(m_pPlayerPed) return;

	field_1F9 = pGame->FUNC_100A0D90(0, (float)a1, (float)a2, (float)a3, m_PlayerID, 0);
	field_1ED = a1;
	field_1F1 = a2;
	field_1F5 = a3;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014650()
{
	if(field_10A != 17) return;
	if(!field_C5.wVehicleID) return;
	if(field_C5.wVehicleID == INVALID_VEHICLE_ID) return;
	if(field_C5.wVehicleID >= (MAX_VEHICLES + 1000)) return;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	CEntity *pEntity;

	if(field_C5.wVehicleID < MAX_VEHICLES) {
		if(!pVehiclePool->field_3074[field_C5.wVehicleID]) return;
		pEntity = (CEntity *)pVehiclePool->field_1134[field_C5.wVehicleID];
	} else {
		WORD wObjectID = field_C5.wVehicleID - MAX_VEHICLES;
		if(wObjectID > MAX_OBJECTS) return;
		if(!pObjectPool->field_4[wObjectID]) return;
		pEntity = (CEntity *)pObjectPool->field_FA4[wObjectID];
	}

	if(!pEntity) return;

	if(field_C5.vecOffset.X > 100.0f) return;
	if(field_C5.vecOffset.X < -100.0f) return;
	if(field_C5.vecOffset.Y > 100.0f) return;
	if(field_C5.vecOffset.Y < -100.0f) return;
	if(field_C5.vecOffset.Z > 100.0f) return;
	if(field_C5.vecOffset.Z < -100.0f) return;

	MATRIX4X4 matEntity;
	MATRIX4X4 matPed;
	VECTOR vecMove;
	VECTOR vecTurn;
	VECTOR vecOut;

	pEntity->GetMatrix(&matEntity);
	pEntity->GetMoveSpeedVector(&vecMove);
	pEntity->GetTurnSpeedVector(&vecTurn);
	m_pPlayerPed->GetMatrix(&matPed);

	FUNC_100B4D10(&vecOut, &matEntity, &field_C5.vecOffset);
	matPed.pos = vecOut;
	m_pPlayerPed->SetMatrix(matPed);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014800()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(!m_pPlayerPed) return;
	if(m_pPlayerPed->IsInVehicle()) return;
	if(field_1E7 >= MAX_VEHICLES) return;
	if(!pVehiclePool->field_3074[field_1E7]) return;
	if(!pVehiclePool->field_1134[field_1E7]) return;

	BYTE byteWeapon;
	if(field_10B) byteWeapon = field_AD[3];
	else byteWeapon = field_19[54];
	byteWeapon &= 0x3F;

	if(m_pPlayerPed->GetCurrentWeapon() != byteWeapon) {
		m_pPlayerPed->SetArmedWeapon(byteWeapon, false);
		if(m_pPlayerPed->GetCurrentWeapon() != byteWeapon) {
			m_pPlayerPed->GiveWeapon(byteWeapon, 9999);
			m_pPlayerPed->SetArmedWeapon(byteWeapon, false);
		}
	}

	int iGtaVehicleID = pVehiclePool->FindGtaIDFromID(field_1E7);
	m_pPlayerPed->FUNC_100AC290(iGtaVehicleID, field_10B);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014FF0()
{
	if(!m_pPlayerPed) return;

	MATRIX4X4 mat;
	float quatBlend[4];
	float quatCur[4];

	m_pPlayerPed->GetMatrix(&mat);
	FUNC_100B52B0(&mat, quatCur);
	FUNC_100B5480(quatBlend, field_C5.quat, quatCur, 0.75f);
	FUNC_100B6A80(quatBlend, &mat);
	m_pPlayerPed->SetMatrix(mat);

	float fAngle = atan2(-mat.up.X, mat.up.Y) * 57.2957764f;
	if(fAngle < 0.0f) fAngle += 360.0f;
	else if(fAngle >= 360.0f) fAngle -= 360.0f;
	m_pPlayerPed->FUNC_100ABF10(fAngle);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10017260(BYTE *pSync, int iTime)
{
	field_1E1 = NULL;
	if(iTime && (iTime - field_1BD) < 0) return;

	field_1BD = iTime;
	memcpy(&field_C5, pSync, sizeof(field_C5));
	field_1B0 = pSync[0x22];
	field_1AC = pSync[0x23];
	field_1B8 = 16;
	field_1B9 = GetTickCount();
	field_C = pSync[0x25];
	// the sign-bit mask is what emits jns; '< 0' emits jge
	if(field_C5.iAnimation & 0x80000000) field_C5.iAnimation = 0;

	if(m_pPlayerPed && m_pPlayerPed->IsInVehicle() && FUNC_10014380() != 3 && FUNC_10014380() != 4 &&
		!m_pPlayerPed->FUNC_100AC640()) FUNC_100148F0();

	if(field_10A != 17) field_10A = 17;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10016370(BULLET_SYNC_DATA *pSync)
{
	if(!m_pPlayerPed) return;
	if(!m_pPlayerPed->IsAdded()) return;

	SHOT_SYNC_DATA shotSync;

	memset(&shotSync, 0, sizeof(shotSync));
	shotSync.vecOrigin.X = pSync->vecHitOrigin.X;
	shotSync.vecOrigin.Y = pSync->vecHitOrigin.Y;
	shotSync.vecOrigin.Z = pSync->vecHitOrigin.Z;
	shotSync.vecColPoint.X = pSync->vecHitTarget.X;
	shotSync.vecColPoint.Y = pSync->vecHitTarget.Y;
	shotSync.vecColPoint.Z = pSync->vecHitTarget.Z;
	shotSync.vecOffset.X = pSync->vecCenterOfHit.X;
	shotSync.vecOffset.Y = pSync->vecCenterOfHit.Y;
	shotSync.vecOffset.Z = pSync->vecCenterOfHit.Z;

	if(pSync->byteHitType) {
		if(shotSync.vecOffset.X > 300.0f || shotSync.vecOffset.X < -300.0f ||
			shotSync.vecOffset.Y > 300.0f || shotSync.vecOffset.Y < -300.0f ||
			shotSync.vecOffset.Z > 300.0f || shotSync.vecOffset.Z < -300.0f) return;

		if(pSync->byteHitType == 1) {
			CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
			if(pPlayerPool) {
				PLAYERID playerId = pSync->wHitID;
				if(playerId == pPlayerPool->m_LocalPlayerID) {
					shotSync.pAttachedTo = (ENTITY_TYPE *)pGame->FindPlayerPed()->m_pPed;
				} else if(playerId == m_PlayerID) return;
				else if(pPlayerPool->GetSlotState(playerId)) {
					CPlayerPed *pPlayerPed = pPlayerPool->GetAt(playerId)->m_pPlayerPed;
					if(pPlayerPed) shotSync.pAttachedTo = (ENTITY_TYPE *)pPlayerPed->m_pPed;
				}
			}
		} else if(pSync->byteHitType == 2) {
			CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
			if(pVehiclePool) {
				VEHICLEID VehicleID = pSync->wHitID;
				if(pVehiclePool->GetSlotState(VehicleID))
					shotSync.pAttachedTo =
						(ENTITY_TYPE *)((CVehicle *)pVehiclePool->GetAt(VehicleID))->m_pVehicle;
			}
		}
	}

	if(m_pPlayerPed->IsAdded()) {
		if(m_pPlayerPed->GetCurrentWeapon() != pSync->byteWeaponID) {
			m_pPlayerPed->SetArmedWeapon(pSync->byteWeaponID, true);
			if(m_pPlayerPed->GetCurrentWeapon() != pSync->byteWeaponID) {
				m_pPlayerPed->GiveWeapon(pSync->byteWeaponID, 9999);
				m_pPlayerPed->SetArmedWeapon(pSync->byteWeaponID, true);
			}
		}
	}

	m_pPlayerPed->FUNC_100AF280(&shotSync);
	m_pPlayerPed->FUNC_100AFA70();
}

//----------------------------------------------------

void CRemotePlayer::FUNC_100158D0(UNOCCUPIED_SYNC_DATA *pSync)
{
	if(!pSync->VehicleID) return;
	if(pSync->VehicleID == INVALID_VEHICLE_ID) return;

	MATRIX4X4 mat;
	VECTOR vecMove;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(!pVehiclePool) return;

	CVehicle *pVehicle = (CVehicle *)pVehiclePool->GetAt(pSync->VehicleID);
	pVehiclePool->field_8E34[pSync->VehicleID] = m_PlayerID;
	if(!pVehicle) return;

	if(pVehicle->FUNC_100B70E0()) return;
	if(pVehicle->FUNC_100B7CF0()) return;
	if(!FUNC_100B4A70(&pSync->vecDirection)) return;
	if(!FUNC_100B4A70(&pSync->vecRoll)) return;
	if(!FUNC_100B4B50(&pSync->vecPos)) return;
	if(!FUNC_100B4AE0(&pSync->vecMoveSpeed)) return;
	if(!FUNC_100B4AE0(&pSync->vecTurnSpeed)) return;

	pVehicle->GetMatrix(&mat);

	mat.up.X = pSync->vecDirection.X;
	mat.up.Y = pSync->vecDirection.Y;
	mat.up.Z = pSync->vecDirection.Z;
	mat.right.X = pSync->vecRoll.X;
	mat.right.Y = pSync->vecRoll.Y;
	mat.right.Z = pSync->vecRoll.Z;

	if(FloatOffset(pSync->vecPos.X, mat.pos.X) <= 0.1f &&
		FloatOffset(pSync->vecPos.Y, mat.pos.Y) <= 0.1f &&
		FloatOffset(pSync->vecPos.Z, mat.pos.Z) <= 0.1f) return;

	if(FloatOffset(pSync->vecPos.X, mat.pos.X) > 6.0f ||
		FloatOffset(pSync->vecPos.Y, mat.pos.Y) > 6.0f ||
		FloatOffset(pSync->vecPos.Z, mat.pos.Z) > 3.0f) {
		mat.pos.X = pSync->vecPos.X;
		mat.pos.Y = pSync->vecPos.Y;
		mat.pos.Z = pSync->vecPos.Z;
		pVehicle->SetMatrix(mat);
		pVehicle->SetMoveSpeedVector(pSync->vecMoveSpeed);
		pVehicle->SetTurnSpeedVector(pSync->vecTurnSpeed);
		return;
	}

	pVehicle->SetMatrix(mat);
	pVehicle->SetMoveSpeedVector(pSync->vecMoveSpeed);
	pVehicle->SetTurnSpeedVector(pSync->vecTurnSpeed);

	vecMove.X = 0.0f;
	vecMove.Y = 0.0f;
	vecMove.Z = 0.0f;
	pVehicle->GetMoveSpeedVector(&vecMove);

	if(FloatOffset(pSync->vecPos.X, mat.pos.X) > 0.05)
		vecMove.X = (pSync->vecPos.X - mat.pos.X) * fVehLerp + vecMove.X;
	if(FloatOffset(pSync->vecPos.Y, mat.pos.Y) > 0.05)
		vecMove.Y = (pSync->vecPos.Y - mat.pos.Y) * fVehLerp + vecMove.Y;
	if(FloatOffset(pSync->vecPos.Z, mat.pos.Z) > 0.05)
		vecMove.Z = (pSync->vecPos.Z - mat.pos.Z) * fVehLerp + vecMove.Z;

	pVehicle->SetMoveSpeedVector(vecMove);
	pVehicle->field_83 = 1;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10015140()
{
	MATRIX4X4 mat;
	VECTOR vecMove;
	float fZLimit;

	vecMove.X = 0.0f;
	vecMove.Y = 0.0f;
	vecMove.Z = 0.0f;

	if(!field_1E1) return;

	field_1E1->GetMatrix(&mat);

	if(!field_1E1->IsAdded()) {
		mat.pos.X = field_194.X;
		mat.pos.Y = field_194.Y;
		mat.pos.Z = field_194.Z;
		field_1E1->SetMatrix(mat);
		return;
	}

	field_160 = FloatOffset(field_194.X, mat.pos.X);
	field_164 = FloatOffset(field_194.Y, mat.pos.Y);
	field_168 = FloatOffset(field_194.Z, mat.pos.Z);

	if(field_160 <= 0.05f && field_164 <= 0.05f && field_168 <= 0.05f) return;

	fZLimit = 0.5f;
	if(field_1E1->GetVehicleSubtype() == 4 || field_1E1->GetVehicleSubtype() == 5 ||
		field_1E1->GetVehicleSubtype() == 3) fZLimit = 2.0f;

	if(field_160 > 8.0f || field_164 > 8.0f || field_168 > fZLimit) {
		mat.pos.X = field_194.X;
		mat.pos.Y = field_194.Y;
		mat.pos.Z = field_194.Z;
		field_1E1->SetMatrix(mat);
		field_1E1->SetMoveSpeedVector(field_1A0);
		return;
	}

	field_1E1->GetMoveSpeedVector(&vecMove);
	if(field_160 > 0.05f) vecMove.X = (field_194.X - mat.pos.X) * fVehLerp + vecMove.X;
	if(field_164 > 0.05f) vecMove.Y = (field_194.Y - mat.pos.Y) * fVehLerp + vecMove.Y;
	if(field_168 > 0.05f) vecMove.Z = (field_194.Z - mat.pos.Z) * fVehLerp + vecMove.Z;

	if(FloatOffset(vecMove.X, 0.0f) > 0.01f || FloatOffset(vecMove.Y, 0.0f) > 0.01f ||
		FloatOffset(vecMove.Z, 0.0f) > 0.01f) field_1E1->SetMoveSpeedVector(vecMove);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10015C90(TRAILER_SYNC_DATA *pTrailer)
{
	if(!pTrailer->TrailerID) return;
	if(pTrailer->TrailerID == INVALID_VEHICLE_ID) return;

	MATRIX4X4 mat;
	VECTOR vecMove;

	CVehicle *pTrailerVeh = (CVehicle *)pNetGame->GetVehiclePool()->GetAt(pTrailer->TrailerID);
	CVehicle *pVehicle = field_1E1;
	if(!pVehicle) return;
	if(!pTrailerVeh) return;

	if(!pTrailerVeh->FUNC_100B7D70() && !pVehicle->FUNC_100B7DD0()) return;

	if(pVehicle->FUNC_100B7C90() != pTrailerVeh) {
		pVehicle->FUNC_100B7C80((int)pTrailerVeh);
		pVehicle->FUNC_100B7C10();
	}

	pTrailerVeh->GetMatrix(&mat);
	FUNC_100B6A80(pTrailer->quat, &mat);

	if(FloatOffset(pTrailer->vecPos.X, mat.pos.X) <= 0.5f &&
		FloatOffset(pTrailer->vecPos.Y, mat.pos.Y) <= 0.5f &&
		FloatOffset(pTrailer->vecPos.Z, mat.pos.Z) <= 0.5f) return;

	if(FloatOffset(pTrailer->vecPos.X, mat.pos.X) > 6.0f ||
		FloatOffset(pTrailer->vecPos.Y, mat.pos.Y) > 6.0f ||
		FloatOffset(pTrailer->vecPos.Z, mat.pos.Z) > 3.0f) {
		mat.pos.X = pTrailer->vecPos.X;
		mat.pos.Y = pTrailer->vecPos.Y;
		mat.pos.Z = pTrailer->vecPos.Z;
		pTrailerVeh->SetMatrix(mat);
		pTrailerVeh->SetMoveSpeedVector(pTrailer->vecMoveSpeed);
		pTrailerVeh->SetTurnSpeedVector(pTrailer->vecTurnSpeed);
		return;
	}

	pTrailerVeh->SetMatrix(mat);
	pTrailerVeh->SetTurnSpeedVector(pTrailer->vecTurnSpeed);

	vecMove.X = 0.0f;
	vecMove.Y = 0.0f;
	vecMove.Z = 0.0f;
	pTrailerVeh->GetMoveSpeedVector(&vecMove);

	if(FloatOffset(pTrailer->vecPos.X, mat.pos.X) > 0.05f)
		vecMove.X = (pTrailer->vecPos.X - mat.pos.X) * 0.025f + vecMove.X;
	if(FloatOffset(pTrailer->vecPos.Y, mat.pos.Y) > 0.05f)
		vecMove.Y = (pTrailer->vecPos.Y - mat.pos.Y) * 0.025f + vecMove.Y;
	if(FloatOffset(pTrailer->vecPos.Z, mat.pos.Z) > 0.05f)
		vecMove.Z = (pTrailer->vecPos.Z - mat.pos.Z) * 0.025f + vecMove.Z;

	pTrailerVeh->SetMoveSpeedVector(vecMove);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014950()
{
	if(!m_pPlayerPed) return;
	if(!m_pPlayerPed->IsAdded()) return;

	if(field_C == 1 && !m_pPlayerPed->FUNC_100AC690()) {
		m_pPlayerPed->FUNC_100AC6C0();
		return;
	}
	if(field_C != 1 && m_pPlayerPed->FUNC_100AC690()) m_pPlayerPed->FUNC_100AC720();

	if(field_C == 11 && !m_pPlayerPed->IsCellphoneEnabled()) {
		m_pPlayerPed->ToggleCellphone(1);
		return;
	}
	if(field_C != 11 && m_pPlayerPed->IsCellphoneEnabled()) {
		m_pPlayerPed->ToggleCellphone(0);
		return;
	}

	if(field_C == 2 && !m_pPlayerPed->IsInJetpackMode()) {
		field_8 = 1;
		m_pPlayerPed->StartJetpack();
		return;
	}
	if(field_C != 2 && m_pPlayerPed->IsInJetpackMode()) {
		field_8 = 0;
		m_pPlayerPed->StopJetpack();
		return;
	}

	if(field_C == 10 && !m_pPlayerPed->HasHandsUp()) m_pPlayerPed->HandsUp();
	if(field_C != 10 && m_pPlayerPed->HasHandsUp()) m_pPlayerPed->StopDancing();

	if(!m_pPlayerPed->FUNC_100AE260() && field_C == 68) m_pPlayerPed->FUNC_100AE100();
	if(m_pPlayerPed->FUNC_100AE260() && field_C != 68) m_pPlayerPed->FUNC_100AE1E0();

	if(field_C == 20 && m_pPlayerPed->FUNC_100ADC90() != 1) m_pPlayerPed->FUNC_100B02D0(1);
	if(field_C == 22 && m_pPlayerPed->FUNC_100ADC90() != 2) m_pPlayerPed->FUNC_100B02D0(2);
	if(field_C == 23 && m_pPlayerPed->FUNC_100ADC90() != 3) m_pPlayerPed->FUNC_100B02D0(3);
	if(field_C == 21 && m_pPlayerPed->FUNC_100ADC90() != 4) m_pPlayerPed->FUNC_100B02D0(4);

	if(m_pPlayerPed->FUNC_100ADC90()) {
		if(field_C == 20 || field_C == 22 || field_C == 23 || field_C == 21)
			m_pPlayerPed->FUNC_100ADCA0();
		else m_pPlayerPed->StopCarrying();
	}

	if(!m_pPlayerPed->FUNC_100ACEF0() && field_C == 24) m_pPlayerPed->FUNC_100ACF00(1);
	if(m_pPlayerPed->FUNC_100ACEF0() && field_C != 24 && field_C != 3 && field_C != 4)
		m_pPlayerPed->FUNC_100ACF00(0);

	if(!m_pPlayerPed->FUNC_100ACE40() && field_C == 25) m_pPlayerPed->FUNC_100ACE50(1);
	if(m_pPlayerPed->FUNC_100ACE40() && field_C != 25 && field_C != 3 && field_C != 4)
		m_pPlayerPed->FUNC_100ACE50(0);

	m_pPlayerPed->FUNC_100AD440();
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014C40()
{
	if(!m_pPlayerPed) return;

	MATRIX4X4 mat;
	VECTOR vecMove;
	float fOffZ;

	m_pPlayerPed->GetMatrix(&mat);

	if(!m_pPlayerPed->IsAdded()) {
		mat.pos.X = field_17C.X;
		mat.pos.Y = field_17C.Y;
		mat.pos.Z = field_17C.Z;
		m_pPlayerPed->SetMatrix(mat);
		return;
	}

	field_160 = FloatOffset(field_17C.X, mat.pos.X);
	field_164 = FloatOffset(field_17C.Y, mat.pos.Y);
	fOffZ = FloatOffset(field_17C.Z, mat.pos.Z);
	field_168 = fOffZ;

	if(field_160 <= 0.00001f && field_164 <= 0.00001f && fOffZ <= 0.00001f) return;

	if(field_160 > 2.0f || field_164 > 2.0f || fOffZ > 1.0f) {
		mat.pos.X = field_17C.X;
		mat.pos.Y = field_17C.Y;
		mat.pos.Z = field_17C.Z;
		m_pPlayerPed->SetMatrix(mat);
		return;
	}

	m_pPlayerPed->GetMoveSpeedVector(&vecMove);
	float fLerp = fPosLerp;
	if(field_160 > 0.00001f) vecMove.X = (field_17C.X - mat.pos.X) * fLerp + vecMove.X;
	if(field_164 > 0.00001f) vecMove.Y = (field_17C.Y - mat.pos.Y) * fLerp + vecMove.Y;
	if(field_168 > 0.00001f) vecMove.Z = (field_17C.Z - mat.pos.Z) * fLerp + vecMove.Z;
	m_pPlayerPed->SetMoveSpeedVector(vecMove);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014E60()
{
	if(!m_pPlayerPed) return;

	MATRIX4X4 mat;
	VECTOR vecMove;

	m_pPlayerPed->GetMatrix(&mat);

	if(!m_pPlayerPed->IsAdded()) {
		mat.pos.X = field_17C.X;
		mat.pos.Y = field_17C.Y;
		m_pPlayerPed->SetMatrix(mat);
		return;
	}

	field_160 = FloatOffset(field_17C.X, mat.pos.X);
	field_164 = FloatOffset(field_17C.Y, mat.pos.Y);

	if(field_160 <= 0.01f && field_164 <= 0.01f) return;

	if(field_160 > 2.0f || field_164 > 2.0f) {
		mat.pos.X = field_17C.X;
		mat.pos.Y = field_17C.Y;
		m_pPlayerPed->SetMatrix(mat);
		return;
	}

	m_pPlayerPed->GetMoveSpeedVector(&vecMove);
	if(field_160 > 0.001f) vecMove.X = (field_17C.X - mat.pos.X) * 0.1f + vecMove.X;
	if(field_164 > 0.001f) vecMove.Y = (field_17C.Y - mat.pos.Y) * 0.1f + vecMove.Y;
	m_pPlayerPed->SetMoveSpeedVector(vecMove);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10015460()
{
	MATRIX4X4 mat;
	float quatCur[4];
	float quatBlend[4];
	VECTOR vecTurn;

	vecTurn.X = 0.0f;
	vecTurn.Y = 0.0f;
	vecTurn.Z = 0.0f;

	if(!field_1E1) return;

	if(bClampVehicleTurnSpeed) {
		field_1E1->GetTurnSpeedVector(&vecTurn);

		if(vecTurn.X > 0.02f) vecTurn.X = 0.02f;
		else if(vecTurn.X < -0.02f) vecTurn.X = -0.02f;
		if(vecTurn.Y > 0.02f) vecTurn.Y = 0.02f;
		else if(vecTurn.Y < -0.02f) vecTurn.Y = -0.02f;
		if(vecTurn.Z > 0.02f) vecTurn.Z = 0.02f;
		else if(vecTurn.Z < -0.02f) vecTurn.Z = -0.02f;

		field_1E1->SetTurnSpeedVector(vecTurn);
	}

	field_1E1->GetMatrix(&mat);
	FUNC_100B5210((int)&mat, quatCur);
	FUNC_100B5480(quatBlend, quat16C, quatCur, 0.75f);
	FUNC_100B5500(quatBlend);
	FUNC_100B5470(quatBlend, &mat);
	field_1E1->SetMatrix(mat);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10015760(BYTE *pAim)
{
	if(!m_pPlayerPed) return;

	m_pPlayerPed->SetCameraMode(pAim[0]);

	CAMERA_AIM aim;
	VECTOR vecFront;
	VECTOR vecUp;

	vecUp.X = 0.0f;
	vecUp.Y = 0.0f;
	vecUp.Z = 0.0f;

	aim.f1x = *(float *)(pAim + 1);
	aim.f1y = *(float *)(pAim + 5);
	aim.f1z = *(float *)(pAim + 9);
	vecFront.X = *(float *)(pAim + 1);
	vecFront.Y = *(float *)(pAim + 5);
	vecFront.Z = *(float *)(pAim + 9);

	FUNC_100B6900(&vecFront, &vecUp);

	aim.f2x = vecUp.X;
	aim.f2y = vecUp.Y;
	aim.f2z = vecUp.Z;
	aim.pos1x = *(float *)(pAim + 0xD);
	aim.pos1y = *(float *)(pAim + 0x11);
	aim.pos1z = *(float *)(pAim + 0x15);
	aim.pos2x = *(float *)(pAim + 0xD);
	aim.pos2y = *(float *)(pAim + 0x11);
	aim.pos2z = *(float *)(pAim + 0x15);

	m_pPlayerPed->SetCurrentAim(&aim);
	m_pPlayerPed->SetAimZ(*(float *)(pAim + 0x19));
	m_pPlayerPed->SetCameraExtZoom((float)(pAim[0x1D] & 0x3F) * 0.015873017f,
		(float)pAim[0x1E] * 0.0039215689f);

	WEAPON_SLOT_TYPE *pSlot = m_pPlayerPed->GetCurrentWeaponSlot();
	BYTE byteState = pAim[0x1D] >> 6;
	if(byteState == 3) pSlot->field_4 = 2;
	else if(byteState != 2) pSlot->field_8 = pAim[0x1D] >> 6;
	else if(pSlot->field_8 < 2) pSlot->field_8 = 2;
}

//----------------------------------------------------

BOOL CRemotePlayer::FUNC_10017130(int a1, int iSkin, int a3, VECTOR *pPos, float fRotation,
	DWORD dwColor, BYTE byteStyle)
{
	if(!pGame->IsGameLoaded()) return FALSE;

	if(m_pPlayerPed) {
		pGame->DeletePlayerPed((int)m_pPlayerPed);
		m_pPlayerPed = NULL;
	}

	CPlayerPed *pPlayerPed = pGame->sub_100A0110(iSkin, pPos->X, pPos->Y, pPos->Z,
		fRotation, 1, field_10C);
	if(pPlayerPed) {
		if(dwColor) SetRadarColor(m_PlayerID, dwColor);

		if(field_1F9) {
			pGame->DisableMarker(field_1F9);
			field_1F9 = 0;
		}

		if(pNetGame->GetSettings()->field_30) pPlayerPed->ShowMarker(m_PlayerID);

		m_pPlayerPed = pPlayerPed;
		field_1B0 = 100.0f;
		pPlayerPed->SetKeys(0, 0, 0);

		if(byteStyle != 4) pPlayerPed->SetFightingStyle(byteStyle);

		if(field_10A != 33) field_10A = 33;
		return TRUE;
	}

	if(field_10A) field_10A = 0;
	return FALSE;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10015650(MATRIX4X4 *pMat, VECTOR *pMoveSpeed, float fT)
{
	int bTeleport = 0;

	if(!m_pPlayerPed) return;
	if(!field_1E1) return;

	MATRIX4X4 mat;
	VECTOR vecMove;
	float fDiff;

	field_1E1->GetMatrix(&mat);

	if(pMat->pos.X >= mat.pos.X) fDiff = pMat->pos.X - mat.pos.X;
	else fDiff = mat.pos.X - pMat->pos.X;
	if(fDiff > 10.0f) bTeleport = 1;

	if(pMat->pos.Y >= mat.pos.Y) fDiff = pMat->pos.Y - mat.pos.Y;
	else fDiff = mat.pos.Y - pMat->pos.Y;
	if(fDiff > 10.0f || bTeleport)
		field_1E1->TeleportTo(pMat->pos.X, pMat->pos.Y, pMat->pos.Z);

	field_1E1->GetMoveSpeedVector(&vecMove);
	vecMove.X = pMoveSpeed->X;
	vecMove.Y = pMoveSpeed->Y;
	vecMove.Z = pMoveSpeed->Z;
	field_1E1->SetMoveSpeedVector(vecMove);
	field_1E1->FUNC_100B7900(fT);
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10017340(BYTE *pSync, int iTime)
{
	if(iTime && (iTime - field_1BD) < 0) return;

	field_1BD = iTime;
	memcpy(field_19, pSync, sizeof(field_19));
	field_1E7 = *(WORD *)pSync;
	field_1E1 = (CVehicle *)pNetGame->GetVehiclePool()->GetAt(field_1E7);
	field_10B = 0;
	field_1B0 = pSync[0x34];
	field_1AC = pSync[0x35];
	field_1B8 = 17;
	field_1B9 = GetTickCount();
	field_C = 0;

	if(m_pPlayerPed && !m_pPlayerPed->IsInVehicle()) FUNC_10014800();

	if(field_10A != 19) {
		FUNC_10016270(19, field_10A);
		field_10A = 19;
	}
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10017440(BYTE *pSync)
{
	memcpy(field_AD, pSync, sizeof(field_AD));
	field_1E7 = *(WORD *)pSync;
	field_10B = pSync[2] & 0x3F;
	field_1E1 = (CVehicle *)pNetGame->GetVehiclePool()->GetAt(field_1E7);
	field_1B0 = pSync[4];
	field_1AC = pSync[5];
	field_1B8 = 18;
	field_1B9 = GetTickCount();
	field_C = 0;

	if(m_pPlayerPed && !m_pPlayerPed->IsInVehicle()) FUNC_10014800();

	if(field_10A != 18) field_10A = 18;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_100148F0()
{
	if(m_pPlayerPed && m_pPlayerPed->IsInVehicle()) {
		MATRIX4X4 mat;
		m_pPlayerPed->GetMatrix(&mat);
		m_pPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.X, mat.pos.Y, mat.pos.Z);
		field_1E1 = NULL;
	}
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10014500(int a1)
{
	if(!a1) {
		if(field_1F9) {
			pGame->DisableMarker(field_1F9);
			field_1F9 = a1;
		}
	}
	field_1E9 = a1;
}

//----------------------------------------------------

BOOL CRemotePlayer::FUNC_100145F0()
{
	if(field_10A == 17 && field_C5.wVehicleID && field_C5.wVehicleID != INVALID_VEHICLE_ID &&
		field_C5.wVehicleID < (MAX_VEHICLES + 1000)) return TRUE;

	return FALSE;
}

//----------------------------------------------------

BOOL CRemotePlayer::FUNC_10014620()
{
	if(field_10A == 17 && field_C5.wVehicleID && field_C5.wVehicleID != INVALID_VEHICLE_ID &&
		field_C5.wVehicleID >= MAX_VEHICLES && field_C5.wVehicleID < (MAX_VEHICLES + 1000)) return TRUE;

	return FALSE;
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10016270(BYTE byteNewState, BYTE byteOldState)
{
	if(byteNewState != 19 || byteOldState != 17) return;

	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(!pPlayerPed) return;
	if(!pPlayerPed->IsInVehicle()) return;
	if(pPlayerPed->IsAPassenger()) return;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if((WORD)pVehiclePool->FUNC_1001EB90((int)pPlayerPed->GetGtaVehicle()) == field_1E7) {
		MATRIX4X4 mat;
		pPlayerPed->GetMatrix(&mat);
		pPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.X, mat.pos.Y, mat.pos.Z + 1.0f);
		pGame->DisplayGameText("~r~Car Jacked~w~!", 1000, 5);
	}
}

//----------------------------------------------------

void CRemotePlayer::FUNC_100165B0(BYTE byteNewState)
{
	if(byteNewState != field_10A) {
		FUNC_10016270(byteNewState, field_10A);
		field_10A = byteNewState;
	}
}

//----------------------------------------------------

void CRemotePlayer::FUNC_10016660()
{
	if(field_1F9) {
		pGame->DisableMarker(field_1F9);
		field_1F9 = 0;
	}
	field_1E9 = 0;
	if(m_pPlayerPed) {
		pGame->DeletePlayerPed((int)m_pPlayerPed);
		m_pPlayerPed = NULL;
	}
}

//----------------------------------------------------

void CRemotePlayer::ExitVehicle()
{
	if(m_pPlayerPed && m_pPlayerPed->IsInVehicle()) {
		m_pPlayerPed->SetKeys(0,0,0);
		m_pPlayerPed->ExitCurrentVehicle();
	}
}

//----------------------------------------------------
