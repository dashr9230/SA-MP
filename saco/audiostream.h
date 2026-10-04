
#pragma once

class CAudioStream // size=1
{
private:
	char field_0;

public:

	CAudioStream() {
		field_0 = 0;
	}

	int Reset();
	void Stop(char byteResetRadio);
	void ConstructInfo();
	void SyncProc();
	void Process();
	void Play();
	void FUNC_10066960(char *szURL, float fX, float fY, float fZ, float fRadius, char bUsePos); // .text:10066960
	void ControlGameRadio();
	void DrawInfo();
};
