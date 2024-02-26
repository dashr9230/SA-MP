
#pragma once

class CAudio // size: 5
{
public:
	int field_0;
	char field_4;

	CAudio() {
		field_0 = 0;
		field_4 = 0;
	};

	int GetRadioStation();
	void StartRadio(int iStation);
	float GetRadioVolume();
	void StopOutdoorAmbienceTrack();
	void SetOutdoorAmbienceTrack(int iTrack);
	void PlaySound(int iSound, float fX, float fY, float fZ);
	bool IsOutdoorAmbienceTrackDisabled();
};
