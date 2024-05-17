
#pragma once

class CAudioStream
{
private:
	bool m_bInited;

public:

	CAudioStream() {
		m_bInited = true;
	}

	bool Initialize();
	void ConstructInfo();
	static void CALLBACK SyncProc(HSYNC handle, DWORD channel, DWORD data, void* user);
	static void AudioStreamThread(PVOID v);
	bool Play(char* szURL, float fX, float fY, float fZ, float fRadius, bool bUsePos);
	bool Stop(bool bWaitThread);
	void ControlGameRadio();
	void DrawInfo();
};
