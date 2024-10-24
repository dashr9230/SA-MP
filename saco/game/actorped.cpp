
#include "../main.h"
#include "game.h"
#include "util.h"

extern CGame *pGame;

//-----------------------------------------------------------
// This is the constructor for creating new player.

CActorPed::CActorPed(int iSkin, float fX, float fY,float fZ,float fRotation)
{
	m_pPed=0;
	m_dwGTAId=0;
	m_byteImmune=0;
	int iCounter=0;
	DWORD dwActorID=0;

	if(!pGame->IsModelLoaded(iSkin)) {
		pGame->RequestModel(iSkin);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iSkin)) Sleep(1);
	}

	ScriptCommand(&create_actor,5,iSkin,fX,fY,fZ-1.0f,&dwActorID);
	ScriptCommand(&set_actor_z_angle,dwActorID,fRotation);

	m_dwGTAId = dwActorID;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pPed;
	ScriptCommand(&set_actor_can_be_decapitated,m_dwGTAId,0);
	ScriptCommand(&set_actor_dicision,m_dwGTAId,65542);
}

//-----------------------------------------------------------

CActorPed::~CActorPed()
{
	Destroy();
}

//-----------------------------------------------------------

void CActorPed::Destroy()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	// If it points to the CPlaceable vtable it's not valid
	if(!m_pPed || !GamePool_Ped_GetAt(m_dwGTAId) || m_pPed->entity.vtable == 0x863C40)
	{
		m_pPed = NULL;
		m_pEntity = NULL;
		m_dwGTAId = 0;
		return;
	}

	// DESTROY METHOD
	_asm mov ecx, dwPedPtr
	_asm mov ebx, [ecx] ; vtable
	_asm push 1
	_asm call [ebx] ; destroy

	m_pPed = NULL;
	m_pEntity = NULL;
}

//-----------------------------------------------------------

