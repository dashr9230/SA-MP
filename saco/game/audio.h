
#pragma once

class CAudio // size: 5
{
public:
	int field_0;
	char field_4;

	CAudio() {
		// TODO: CAudio::CAudio()
		field_0 = 0;
		field_4 = 0;
	}

	void GetRadioStation();
	void StartRadio();
	void GetRadioVolume();
	void StopOutdoorAmbienceTrack();
	void SetOutdoorAmbienceTrack();
	void Play();
	void IsOutdoorAmbienceTrackDisabled();
};
