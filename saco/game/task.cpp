
#include "../main.h"
#include "task.h"

//==========================================================
// GENERIC TASK

CTask::CTask()
{
	m_pTaskType = NULL;
	m_pPlayerPed = NULL;
}

//----------------------------------------------------------

CTask::CTask(DWORD dwSize)
{
	m_pTaskType = NULL;
	m_pPlayerPed = NULL;
	Create(dwSize);
}

//----------------------------------------------------------

CTask::CTask(BYTE *pTaskType)
{
	m_pTaskType = NULL;
	m_pPlayerPed = NULL;
	Create(pTaskType);
}

//----------------------------------------------------------

CTask::~CTask()
{
}

//----------------------------------------------------------

void CTask::Create(DWORD dwSize)
{
	BYTE *pTaskType;
	__asm
	{
		push dwSize;
		mov eax, 0x61A5A0;	// CTask_New
		call eax;
		add esp, 4;
		mov pTaskType, eax;
	}
	m_bSelfCreated = TRUE;
	m_pTaskType = pTaskType;
}

//----------------------------------------------------------

void CTask::Create(BYTE *pTaskType)
{
	m_bSelfCreated = FALSE;
	m_pTaskType = pTaskType;
}

//----------------------------------------------------------

CTask* CTask::CreateCopy()
{
	BYTE *pTaskType = m_pTaskType;
	__asm
	{
		push ecx;
		mov ecx, pTaskType;
		mov eax, [ecx];
		call [eax+0x4];
		mov pTaskType, eax;
		pop ecx;
	}
	return new CTask(pTaskType);
}

//----------------------------------------------------------

void CTask::Destroy()
{
	if (!IsDestroyed())
	{
		BYTE *pTaskType = m_pTaskType;
		if (m_bSelfCreated)
		{
			__asm
			{
				push ecx;
				mov ecx, pTaskType;
				mov eax, [ecx];
				push 0;		// remove from pool?
				call [eax+0];
				pop ecx;
			}
		}
		else
		{
			__asm
			{
				push ecx;
				mov ecx, pTaskType;
				mov eax, [ecx];
				push 1;		// remove from pool?
				call [eax+0];
				pop ecx;
			}
		}
		m_pTaskType = NULL;
		m_pPlayerPed = NULL;
	}
}

//----------------------------------------------------------

/*void CTask::ApplyToPed(CActorPed *pPed)
{
	//m_pPlayerPed = pPed;
	PED_TASKS_TYPE *pPedTasksType = pPed->m_pPed->Tasks;
	BYTE *pTaskType = m_pTaskType;

	__asm
	{
		push ecx;

		mov ecx, pPedTasksType;
		push 0;
		push 3;
		push pTaskType;
		add ecx, 4;
		mov eax, 0x681AF0;	
		call eax;			// AddTaskToActor

		pop ecx;
	}
}*/

void CTask::ApplyToPed(CPlayerPed *pPed)
{
	extern BYTE *pbyteCurrentPlayer;
	m_pPlayerPed = pPed;
	PED_TASKS_TYPE *pPedTasksType = pPed->m_pPed->Tasks;
	BYTE *pTaskType = m_pTaskType;

	*pbyteCurrentPlayer = pPed->m_bytePlayerNumber;

	__asm
	{
		push ecx;
		
		mov ecx, pPedTasksType;
		push 0;
		push 3;
		push pTaskType;
		add ecx, 4;
		mov eax, 0x681AF0;	
		call eax;			// AddTaskToActor

		pop ecx;
	}

	*pbyteCurrentPlayer = 0;


	/*
	(aru) Disabling this messy thing... The one above works just fine for now.

	// Call a default applier here, but can be overloaded for special tasks
	PED_TYPE *pPedType = pPed->m_pPed;
	PED_TASKS_TYPE *pPedTasksType = pPedType->Tasks;
	BYTE *pTaskType = m_pTaskType;
	
	BYTE TaskAppBlock[24];

	DWORD dwSomeSpecialArg = 0;

	__asm
	{
		push ecx;
		push edi;

		push 0;
		push pTaskType;
		push 3;
		lea ecx, TaskAppBlock;
		mov eax, 0x4B0A00;
		call eax;			// Initializes the TaskAppBlock

		lea eax, TaskAppBlock;
		push eax;
		mov eax, pPedTasksType;
		lea ecx, [eax+0x68];
		mov eax, 0x4AB420;
		call eax;			// Does something with CPedIntelligence+0x68

		mov edi, eax;
		test edi, edi;
		jz skipExtended;	// Skip out of the extended stuff

		mov eax, 0x608540;	// This gets something and returns it in eax (3)
		call eax;

		mov ecx, dwSomeSpecialArg;
		push edi;
		push ecx;
		lea ecx, [eax+eax*4];
		push pPedType;
		lea ecx, ds:0xC0B1E8[ecx*4];
		mov eax, 0x608390;
		call eax;			// No idea what this does. (10)

skipExtended:

		lea ecx, TaskAppBlock;
		mov eax, 0x4B0A50;
		call eax;			// No idea what this does either (11)

		pop edi;
		pop ecx;
	}
	*/

}

//----------------------------------------------------------

DWORD CTask::GetID()
{
	BYTE *pTaskType = m_pTaskType;
	DWORD dwID;
	__asm
	{
		push ecx;
		mov ecx, pTaskType;
		mov eax, [ecx];
		call [eax+0x10];
		mov dwID, eax;
		pop ecx;
	}
	return dwID;
}

//----------------------------------------------------------

BOOL CTask::IsDestroyed()
{
	if (m_pTaskType == NULL)
		return TRUE;

	DWORD dwVTbl = *((DWORD*)m_pTaskType);

	if (dwVTbl == 0x86D48C)		// CTask_vtbl
		return TRUE;

	return FALSE;
}

//----------------------------------------------------------

BOOL CTask::IsSimple()
{
	BYTE *pTaskType = m_pTaskType;
	BOOL bRet;
	__asm
	{
		push ecx;
		mov ecx, pTaskType;
		mov eax, [ecx];
		call [eax+0xC];
		movzx eax, al;
		mov bRet, eax;
		pop ecx;
	}
	return bRet;
}

//==========================================================
// JETPACK TASK

CTaskJetpack::CTaskJetpack()
{
	m_pPlayerPed = NULL;

	Create(112);

	BYTE *pTaskType = m_pTaskType;
	__asm
	{
		push 0;
		push 0;
		push 0x41200000;		// 10.0f
		push 0;
		mov ecx, pTaskType;
		mov eax, 0x67B4E0;	//  CTaskJetpack_CreateJetpack
		call eax;
	}
}

//----------------------------------------------------------

CTaskJetpack::CTaskJetpack(BYTE *pTaskType)
{
	m_pPlayerPed = NULL;

	Create(pTaskType);
}

//----------------------------------------------------------

CTaskJetpack::~CTaskJetpack()
{
	if (m_pPlayerPed)
		m_pPlayerPed->m_pPed->Tasks->pdwJumpJetPack = NULL;
	CTask::Destroy();
}

//==========================================================
// TAKE_DAMAGE_FALL TASK

CTaskTakeDamageFall::CTaskTakeDamageFall(DWORD dwFallType, DWORD dwNum)
{
	m_pPlayerPed = NULL;

	Create(24);

	BYTE *pTaskType = m_pTaskType;
	__asm
	{
		push dwNum;
		push 0;
		push dwFallType;
		mov ecx, pTaskType;
		mov eax, 0x6786C0;
		call eax;				// CTaskTakeDamageFall_ctor2
	}
}

//==========================================================
// ACTIVATE_GOGGLES

CTaskGoggles::CTaskGoggles()
{
	m_pPlayerPed = NULL;
	Create(12);

	BYTE *pTaskType = m_pTaskType;
	_asm push ecx
	_asm mov ecx, pTaskType
	_asm mov eax, 0x634EF0
	_asm call eax
	_asm pop ecx
}

//-----------------------------------------------------------

