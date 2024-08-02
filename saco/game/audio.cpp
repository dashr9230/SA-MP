
#include "game.h"

//-----------------------------------------------------------

void CAudio::FUNC_100A21D0()
{
	if(field_0 && field_0 == 1)
	{
		if(ScriptCommand(&is_wav_loaded, 1))
		{
			ScriptCommand(&play_wav, 1);
			field_0 = 2;
		}
	}
}

//-----------------------------------------------------------

int CAudio::GetRadioStation() 
{
	int iRadioStation = 0;

	_asm mov ecx, 0x8CB6F8
	_asm xor eax, eax
	_asm mov al, [ecx+233]
	_asm mov iRadioStation, eax

	if(iRadioStation < 0 || iRadioStation > 12) return -1;

	return iRadioStation;
}

//-----------------------------------------------------------

void CAudio::StartRadio(int iStation)
{
	_asm push 0
	_asm push iStation
	_asm mov ecx, 0xB6BC90
	_asm mov edx, 0x507DC0
	_asm call edx
}

//-----------------------------------------------------------

void CAudio::StopRadio()
{
	_asm push 0
	_asm push 0
	_asm mov ecx, 0xB6BC90
	_asm mov edx, 0x506F70
	_asm call edx
}

//-----------------------------------------------------------

float CAudio::GetRadioVolume()
{
    return *(float*)0xB5FCC8;
}

//-----------------------------------------------------------

void CAudio::StopOutdoorAmbienceTrack()
{
	_asm mov ecx, 0x8AC15C
	_asm mov edx, 0x4D6D60
	_asm call edx
}

//-----------------------------------------------------------

void CAudio::SetOutdoorAmbienceTrack(int iTrack)
{
    _asm push iTrack
	_asm mov ecx, 0x8AC15C
	_asm mov edx, 0x4D6D50
	_asm call edx
}

//-----------------------------------------------------------

void CAudio::PlaySound(int iSound, float fX, float fY, float fZ) 
{
	if(iSound == 0)
	{
		if(field_0) {
			ScriptCommand(&unload_wav, 1);
			field_0 = iSound;
		}
		StopOutdoorAmbienceTrack();
		field_4 = false;
	}
	else if(iSound == 1)
	{
		field_4 = true;
	}
	else if(iSound < 1000)
	{
		SetOutdoorAmbienceTrack(iSound);
	}
	else if(iSound < 2000)
	{
		ScriptCommand(&play_sound, fX, fY, fZ, iSound);
	}
	else
	{
		ScriptCommand(&unload_wav, 1);
		ScriptCommand(&load_wav, 1, iSound);
		ScriptCommand(&set_wav_at_location, 1, fX, fY, fZ);
		field_0 = 1;
	}	
}

//-----------------------------------------------------------

bool CAudio::IsOutdoorAmbienceTrackDisabled()
{
	return field_4;
}

//-----------------------------------------------------------
