
#include "main.h"

#define CHECK_PARAMS(n) { if (params[0] != (n * sizeof(cell))) { logprintf("SCRIPT: Bad parameter count (Count is %d, Should be %d): ", params[0] / sizeof(cell), n); return 0; } }

//----------------------------------------------------------------------------------

// native print(const string[])
static cell AMX_NATIVE_CALL n_print(AMX *amx, cell *params)
{
	char* msg;
	amx_StrParam(amx, params[1], msg);
	//logprintf("%s",msg);
	return 0;
}

// native printf(const format[], {Float,_}:...)
static cell AMX_NATIVE_CALL n_printf(AMX *amx, cell *params)
{
	//int len;
	//logprintf("%s",format_amxstring(amx, params, 1, len));

	return 0;
}

// native format(output[], len, const format[], {Float,_}:...)
static cell AMX_NATIVE_CALL n_format(AMX *amx, cell *params)
{
	// TODO: n_format
	return 0;
}

// native SetTimer(funcname[], interval, repeating)
static cell AMX_NATIVE_CALL n_SetTimer(AMX *amx, cell *params)
{
	// TODO: n_SetTimer
	return 0;
}

// native KillTimer(timerid)
static cell AMX_NATIVE_CALL n_KillTimer(AMX *amx, cell *params)
{
	// TODO: n_KillTimer
	return 0;
}

// native GetTickCount()
static cell AMX_NATIVE_CALL n_GetTickCount(AMX *amx, cell *params)
{
	CHECK_PARAMS(0);

	return (cell)GetTickCount();
}

// native Float:asin(Float:value)
static cell AMX_NATIVE_CALL n_asin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(asin(amx_ctof(params[1])) * 180 / PI);
	return amx_ftoc(fResult);
}

// native Float:acos(Float:value)
static cell AMX_NATIVE_CALL n_acos(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(acos(amx_ctof(params[1])) * 180 / PI);
	return amx_ftoc(fResult);
}

// native Float:atan(Float:value)
static cell AMX_NATIVE_CALL n_atan(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(atan(amx_ctof(params[1])) * 180 / PI);
	return amx_ftoc(fResult);
}

// native Float:atan2(Float:x, Float:y)
static cell AMX_NATIVE_CALL n_atan2(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	float fResult = (float)(atan2(amx_ctof(params[1]), amx_ctof(params[2])) * 180 / PI);
	return amx_ftoc(fResult);
}

// native StartRecordingPlayback(playbacktype, recordname[])
static cell AMX_NATIVE_CALL n_StartRecordingPlayback(AMX *amx, cell *params)
{
	// TODO: n_StartRecordingPlayback
	return 0;
}

// native StopRecordingPlayback()
static cell AMX_NATIVE_CALL n_StopRecordingPlayback(AMX *amx, cell *params)
{
	pNetGame->StopRecordingPlayback();
	return 1;
}

// native PauseRecordingPlayback()
static cell AMX_NATIVE_CALL n_PauseRecordingPlayback(AMX *amx, cell *params)
{
	// TODO: n_PauseRecordingPlayback
	return 0;
}

// native ResumeRecordingPlayback()
static cell AMX_NATIVE_CALL n_ResumeRecordingPlayback(AMX *amx, cell *params)
{
	// TODO: n_ResumeRecordingPlayback
	return 0;
}

// native SendChat(msg[])
static cell AMX_NATIVE_CALL n_SendChat(AMX *amx, cell *params)
{
	// TODO: n_SendChat
	return 0;
}

// native SendCommand(commandtext[])
static cell AMX_NATIVE_CALL n_SendCommand(AMX *amx, cell *params)
{
	// TODO: n_SendCommand
	return 0;
}

// native GetPlayerState(playerid)
static cell AMX_NATIVE_CALL n_GetPlayerState(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerState
	return 0;
}

// native GetPlayerPos(playerid, &Float:x, &Float:y, &Float:z)
static cell AMX_NATIVE_CALL n_GetPlayerPos(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerPos
	return 0;
}

// native GetPlayerVehicleID(playerid)
static cell AMX_NATIVE_CALL n_GetPlayerVehicleID(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerVehicleID
	return 0;
}

// native GetPlayerArmedWeapon(playerid)
static cell AMX_NATIVE_CALL n_GetPlayerArmedWeapon(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerArmedWeapon
	return 0;
}

// native GetPlayerHealth(playerid)
static cell AMX_NATIVE_CALL n_GetPlayerHealth(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerHealth
	return 0;
}

// native GetPlayerArmour(playerid)
static cell AMX_NATIVE_CALL n_GetPlayerArmour(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerArmour
	return 0;
}

// native GetPlayerSpecialAction(playerid)
static cell AMX_NATIVE_CALL n_GetPlayerSpecialAction(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerSpecialAction
	return 0;
}

// native IsPlayerStreamedIn(playerid)
static cell AMX_NATIVE_CALL n_IsPlayerStreamedIn(AMX *amx, cell *params)
{
	if(!pNetGame->GetPlayerPool()->GetSlotState((PLAYERID)params[1])) return 0;

	return pNetGame->IsPlayerAdded((PLAYERID)params[1]);
}

// native IsVehicleStreamedIn(vehicleid)
static cell AMX_NATIVE_CALL n_IsVehicleStreamedIn(AMX *amx, cell *params)
{
	// TODO: n_IsVehicleStreamedIn
	return 0;
}

// native GetPlayerKeys(playerid, &keys, &updown, &leftright)
static cell AMX_NATIVE_CALL n_GetPlayerKeys(AMX *amx, cell *params)
{
	// TODO: n_GetPlayerKeys
	return 0;
}

// native GetMyPos(&Float:x, &Float:y, &Float:z)
static cell AMX_NATIVE_CALL n_GetMyPos(AMX *amx, cell *params)
{
	// TODO: n_GetMyPos
	return 0;
}

// native SetMyPos(Float:x, Float:y, Float:z)
static cell AMX_NATIVE_CALL n_SetMyPos(AMX *amx, cell *params)
{
	// TODO: n_SetMyPos
	return 0;
}

// native GetMyFacingAngle(&Float:ang)
static cell AMX_NATIVE_CALL n_GetMyFacingAngle(AMX *amx, cell *params)
{
	// TODO: n_GetMyFacingAngle
	return 0;
}

// native SetMyFacingAngle(Float:ang)
static cell AMX_NATIVE_CALL n_SetMyFacingAngle(AMX *amx, cell *params)
{
	pNetGame->SetMyZAngle(amx_ctof(params[1]));
	return 1;
}

// native GetDistanceFromMeToPoint(Float:X, Float:Y, Float:Z, &Float:Distance)
static cell AMX_NATIVE_CALL n_GetDistanceFromMeToPoint(AMX *amx, cell *params)
{
	// TODO: n_GetDistanceFromMeToPoint
	return 0;
}

// native IsPlayerInRangeOfPoint(playerid, Float:range, Float:X, Float:Y, Float:Z)
static cell AMX_NATIVE_CALL n_IsPlayerInRangeOfPoint(AMX *amx, cell *params)
{
	// TODO: n_IsPlayerInRangeOfPoint
	return 0;
}

// native GetPlayerName(playerid, const name[], len)
static cell AMX_NATIVE_CALL n_GetPlayerName(AMX *amx, cell *params)
{
	PLAYERID playerId = (PLAYERID)params[1];
	if (playerId >= MAX_PLAYERS || !pNetGame->GetPlayerPool()->GetSlotState(playerId)) return 0;
	return set_amxstring(amx, params[2], pNetGame->GetPlayerPool()->
		GetPlayerName(playerId), params[3]);
}

// native IsPlayerConnected(playerid)
static cell AMX_NATIVE_CALL n_IsPlayerConnected(AMX *amx, cell *params)
{
	PLAYERID playerId = (PLAYERID)params[1];
	if (playerId >= MAX_PLAYERS || pNetGame->GetPlayerPool()->GetSlotState(playerId)) return 0;
	return 1;
}

//----------------------------------------------------------------------------------

AMX_NATIVE_INFO custom_Natives[] =
{
	{ "print",						n_print },
	{ "printf",						n_printf },
	{ "format",						n_format },
	{ "SetTimer",					n_SetTimer },
	{ "KillTimer",					n_KillTimer },
	{ "GetTickCount",				n_GetTickCount },
	{ "asin",						n_asin },
	{ "acos",						n_acos },
	{ "atan2",						n_atan2 },
	{ "atan",						n_atan },
	{ "asin",						n_asin },
	{ "StartRecordingPlayback",		n_StartRecordingPlayback },
	{ "StopRecordingPlayback",		n_StopRecordingPlayback },
	{ "PauseRecordingPlayback",		n_PauseRecordingPlayback },
	{ "ResumeRecordingPlayback",	n_ResumeRecordingPlayback },
	{ "SendChat",					n_SendChat },
	{ "SendCommand",				n_SendCommand },
	{ "GetPlayerState",				n_GetPlayerState },
	{ "GetPlayerPos",				n_GetPlayerPos },
	{ "GetPlayerVehicleID",			n_GetPlayerVehicleID },
	{ "GetPlayerArmedWeapon",		n_GetPlayerArmedWeapon },
	{ "GetPlayerHealth",			n_GetPlayerHealth },
	{ "GetPlayerArmour",			n_GetPlayerArmour },
	{ "GetPlayerSpecialAction",		n_GetPlayerSpecialAction },
	{ "IsPlayerStreamedIn",			n_IsPlayerStreamedIn },
	{ "IsVehicleStreamedIn",		n_IsVehicleStreamedIn },
	{ "GetPlayerKeys",				n_GetPlayerKeys },
	{ "GetMyPos",					n_GetMyPos },
	{ "SetMyPos",					n_SetMyPos },
	{ "GetMyFacingAngle",			n_GetMyFacingAngle },
	{ "SetMyFacingAngle",			n_SetMyFacingAngle },
	{ "GetDistanceFromMeToPoint",	n_GetDistanceFromMeToPoint },
	{ "IsPlayerInRangeOfPoint",		n_IsPlayerInRangeOfPoint },
	{ "GetPlayerName",				n_GetPlayerName },
	{ "IsPlayerConnected",			n_IsPlayerConnected },

	{ NULL, NULL }
};

//----------------------------------------------------------------------------------

int amx_CustomInit(AMX *amx)
{
  return amx_Register(amx, custom_Natives, -1);
}

//----------------------------------------------------------------------------------
