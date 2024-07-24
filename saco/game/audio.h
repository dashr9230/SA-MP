
#pragma once

//-----------------------------------------------------------

class CAudio
{
private:
	int field_0;
	bool field_4;

public:
	CAudio() {
		field_0 = 0;
		field_4 = false;
	}

	void FUNC_100A21D0();
	int GetRadioStation();
	void StartRadio(int iStation);
	void StopRadio();
	float GetRadioVolume();
	void StopOutdoorAmbienceTrack();
	void SetOutdoorAmbienceTrack(int iTrack);
	void PlaySound(int iSound, float fX, float fY, float fZ);
	bool IsOutdoorAmbienceTrackDisabled();
};

//-----------------------------------------------------------
