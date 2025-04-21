
#include "../main.h"

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	/*
		TODO: CPlayerPool::CPlayerPool() - Figure out how and why generate this assembly line.
		
		Compiler should know EAX is 0 right at the begginning, not sure why it checks if it is above 1004
		and jump (to loc_10014014) only to set m_bPlayerSlotState[0] to 0 and still increment EAX and check if its below 1004

		.text:10013FFF                 xor     eax, eax
		.text:10014001                 cmp     ax, 3ECh
		.text:10014005                 mov     [esp+1Ch+var_4], ebx
		.text:10014009                 lea     ecx, [esi+2Ah]  ; offset of m_bPlayerSlotState[0]
		.text:1001400C                 ja      short loc_10014014
		.text:1001400E
		.text:1001400E loc_1001400E:                           ; CODE XREF: CPlayerPool::CPlayerPool(void)+4Ej
		.text:1001400E                 mov     [ecx+1F60h], ebx ; m_pPlayers[] = NULL
		.text:10014014
		.text:10014014 loc_10014014:                           ; CODE XREF: CPlayerPool::CPlayerPool(void)+3Cj
		.text:10014014                 mov     [ecx], ebx      ; m_bPlayerSlotState[] = FALSE
		.text:10014016                 inc     eax
		.text:10014017                 add     ecx, 4
		.text:1001401A                 cmp     ax, 3ECh
		.text:1001401E                 jb      short loc_1001400E
	*/
	
	// loop through and initialize all net players to null and slot states to false
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		m_pPlayers[playerId] = NULL;
		field_2A[playerId] = 0;
	}

	m_pLocalPlayer = new CLocalPlayer();

	field_0 = 0;
	field_22 = 0;
	m_LocalPlayerID = 0;
	field_2F3A = 0;
}

//----------------------------------------------------
