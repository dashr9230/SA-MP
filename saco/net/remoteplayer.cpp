
#include "../main.h"
#include "../game/util.h"

extern CGame		 *pGame;

using namespace RakNet;
extern CNetGame* pNetGame;

//----------------------------------------------------

CRemotePlayer::CRemotePlayer()
{
	field_1E7 = 0;
	field_10A = 0;
	field_1E5 = -1;
	field_1DD = 0;
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
	memset(field_C5, 0, sizeof(field_C5));
	memset(field_19, 0, sizeof(field_19));
	memset(field_AD, 0, sizeof(field_AD));
	memset(field_8E, 0, sizeof(field_8E));
	memset(field_1C9, 0, sizeof(field_1C9));
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

