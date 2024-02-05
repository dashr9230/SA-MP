
#include "main.h"

void cmdDefaultCmdProc(PCHAR szCmd)
{
	// TODO: cmdDefaultCmdProc
}

void cmdTestDeathWindow(PCHAR szCmd)
{
	// TODO: cmdTestDeathWindow
}

void cmdCameraTargetDebug(PCHAR szCmd)
{
	// TODO: cmdCameraTargetDebug .text:100685E0
}

void cmdPageSize(PCHAR szCmd)
{
	// TODO: cmdPageSize .text:100685F0
}

void cmdFontSize(PCHAR szCmd)
{
	// TODO: cmdFontSize .text:10068670
}

void cmdNameTagStatus(PCHAR szCmd)
{
	// TODO: cmdNameTagStatus .text:10068720
}

void cmdTimestamp(PCHAR szCmd)
{
	// TODO: cmdTimestamp .text:10068730
}

void cmdAudioMsg(PCHAR szCmd)
{
	// TODO: cmdAudioMsg .text:10068790
}

void cmdLogUrls(PCHAR szCmd)
{
	// TODO: cmdLogUrls .text:10068800
}

void cmdHudScaleFix(PCHAR szCmd)
{
	// TODO: cmdHudScaleFix .text:10068870
}

void cmdMem(PCHAR szCmd)
{
	pChatWindow->AddDebugMessage("Memory: %u",*(DWORD *)0x8A5A80);
}

void cmdSetFrameLimit(PCHAR szCmd)
{
	// TODO: cmdSetFrameLimit .text:100688D0
}

void cmdHeadMove(PCHAR szCmd)
{
	// TODO: cmdSetFrameLimit .text:10068960
}

void cmdQuit(PCHAR szCmd)
{
	// TODO: cmdQuit .text:100689E0
}

void cmdCmpStat(PCHAR szCmd)
{
	// TODO: cmdCmpStat .text:100689F0
}

void cmdSavePos(PCHAR szCmd)
{
	// TODO: cmdSavePos .text:10068A00
}

void cmdRawSavePos(PCHAR szCmd)
{
	// TODO: cmdRawSavePos .text:10068B80
}

void cmdPlayerSkin(PCHAR szCmd)
{
	// TODO: cmdPlayerSkin .text:10068D00
}

void cmdCreateVehicle(PCHAR szCmd)
{
	// TODO: cmdCreateVehicle .text:10068D70
}

void cmdSelectVehicle(PCHAR szCmd)
{
	// TODO: cmdSelectVehicle .text:10068EB0
}

void cmdSetWeather(PCHAR szCmd)
{
	// TODO: cmdSetWeather .text:10068ED0
}

void cmdSetTime(PCHAR szCmd)
{
	// TODO: cmdSetTime .text:10068F20
}

void cmdShowInterior(PCHAR szCmd)
{
	// TODO: cmdShowInterior .text:10068FD0
}

void cmdToggleObjectLight(PCHAR szCmd)
{
	// TODO: cmdToggleObjectLight .text:10069000
}

void cmdDebugLabels(PCHAR szCmd)
{
	// TODO: cmdDebugLabels
}

void cmdRcon(PCHAR szCmd)
{
	// TODO: cmdRcon .text:10069030
}

//----------------------------------------------------

void SetupCommands()
{
	// RELEASE COMMANDS
	pCmdWindow->AddDefaultCmdProc(cmdDefaultCmdProc);
	pCmdWindow->AddCmdProc("quit",cmdQuit);
	pCmdWindow->AddCmdProc("q",cmdQuit);
	pCmdWindow->AddCmdProc("save",cmdSavePos);
	pCmdWindow->AddCmdProc("rs",cmdRawSavePos);
	pCmdWindow->AddCmdProc("rcon",cmdRcon);
	pCmdWindow->AddCmdProc("mem",cmdMem);
	pCmdWindow->AddCmdProc("fpslimit",cmdSetFrameLimit);
	pCmdWindow->AddCmdProc("pagesize",cmdPageSize);
	pCmdWindow->AddCmdProc("fontsize",cmdFontSize);
	pCmdWindow->AddCmdProc("nametagstatus",cmdNameTagStatus);
	pCmdWindow->AddCmdProc("timestamp",cmdTimestamp);
	pCmdWindow->AddCmdProc("headmove",cmdHeadMove);
	pCmdWindow->AddCmdProc("hudscalefix",cmdHudScaleFix);
	pCmdWindow->AddCmdProc("testdw",cmdTestDeathWindow);

#ifndef _DEBUG
	if(tSettings.bDebug)
	{
#endif
	pCmdWindow->AddCmdProc("vsel",cmdSelectVehicle);
	pCmdWindow->AddCmdProc("v",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("vehicle",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("player_skin",cmdPlayerSkin);
	pCmdWindow->AddCmdProc("set_weather",cmdSetWeather);
	pCmdWindow->AddCmdProc("set_time",cmdSetTime);
#ifndef _DEBUG
	}
#endif

	pCmdWindow->AddCmdProc("interior",cmdShowInterior);
	pCmdWindow->AddCmdProc("togobjlight",cmdToggleObjectLight);
	pCmdWindow->AddCmdProc("cmpstat",cmdCmpStat);
	pCmdWindow->AddCmdProc("dl",cmdDebugLabels);
	pCmdWindow->AddCmdProc("ctd",cmdCameraTargetDebug);
	pCmdWindow->AddCmdProc("audiomsg",cmdAudioMsg);
	pCmdWindow->AddCmdProc("logurls",cmdLogUrls);
}

//----------------------------------------------------
