
#include "main.h"

#define NOBASSOVERLOADS
#include "bass/bass.h"

extern CAudioStream* pAudioStream;
extern CChatWindow* pChatWindow;
extern CFontRender* pDefaultFont;
extern CConfig* pConfig;
extern CGame* pGame;

#define BASS_STREAM_CREATE_URL_FLAG \
	BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE

HSTREAM bassStream = 0;
char g_szAudioStreamUrl[256 + 1];
float g_fAudioStreamX;
float g_fAudioStreamY;
float g_fAudioStreamZ;
float g_fAudioStreamRadius;
bool g_bAudioStreamUsePos;
bool g_bAudioStreamStop;
bool g_bAudioStreamThreadWorked;

char g_szAudioStreamTitle[256 + 1];
char g_szAudioStreamTagName[256 + 1];
char g_szAudioStreamTagUrl[256 + 1];

bool CAudioStream::Initialize()
{
	if (!BASS_Init(-1, 44100, 0, NULL, NULL))
		return false;

	DWORD dwVolume = DWORD(pGame->GetAudio()->GetRadioVolume() * 7000.0f);

	BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, "SA-MP/0.3");
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, dwVolume);
	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1);
	BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, 10000);

	if (!pConfig->IsVariableExist("audioproxyoff"))
		pConfig->SetIntVariable("audioproxyoff", 0);

	if (pConfig->GetIntVariable("audioproxyoff"))
		BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, NULL);

	BASS_SetEAXParameters(-1, 0.0f, -1.0f, -1.0f);
	m_bInited = true;
	return true;
}

void CAudioStream::ConstructInfo()
{
	memset(&g_szAudioStreamTitle, 0, 256);
	g_szAudioStreamTitle[256] = 0;

	const char* meta = BASS_ChannelGetTags(bassStream, BASS_TAG_META);
	if (meta)
	{
		const char* p1 = strstr(meta, "StreamTitle='");
		if (p1)
		{
			const char* p2 = strstr(p1, "';");
			if (p2)
			{
				char* t = _strdup(p1 + 13);
				t[p2 - (p1 + 13)] = 0;
				sprintf_s(g_szAudioStreamTitle, 256, "%s", t);
				free(t);
			}
		}
	}
	else
	{
		meta = BASS_ChannelGetTags(bassStream, BASS_TAG_OGG);
		if (meta)
		{
			const char* artist = NULL, * title = NULL, * p = meta;
			for (; *p; p += strlen(p) + 1)
			{
				if (!_strnicmp(p, "artist=", 7))
					artist = p + 7;
				if (!_strnicmp(p, "title=", 6))
					title = p + 6;
			}
			if (title)
			{
				if (artist)
					sprintf_s(g_szAudioStreamTitle, 256, "%s - %s", artist, title);
				else
					sprintf_s(g_szAudioStreamTitle, 256, "%s", title);
			}
		}
	}
}

void CALLBACK CAudioStream::SyncProc(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	pAudioStream->ConstructInfo();
}

void CAudioStream::AudioStreamThread(PVOID v)
{
	g_bAudioStreamThreadWorked = true;
	if (bassStream)
	{
		BASS_ChannelStop(bassStream);
		bassStream = 0;
	}

	bassStream = BASS_StreamCreateURL(g_szAudioStreamUrl, 0, BASS_STREAM_CREATE_URL_FLAG, 0, 0);
	BASS_ChannelPlay(bassStream, 0);

	memset(&g_szAudioStreamTagName, 0, 256);
	g_szAudioStreamTagName[256] = 0;
	memset(&g_szAudioStreamTagUrl, 0, 256);
	g_szAudioStreamTagUrl[256] = 0;

	const char* szTag = BASS_ChannelGetTags(bassStream, BASS_TAG_ICY);
	if (!szTag) szTag = BASS_ChannelGetTags(bassStream, BASS_TAG_HTTP);
	if (szTag) {
		for (; *szTag; szTag += strlen(szTag) + 1) {
			if (!_strnicmp(szTag, "icy-name:", 9))
			{
				sprintf_s(g_szAudioStreamTagName, 256, "%s", szTag + 9);
			}
			if (!_strnicmp(szTag, "icy-url:", 8))
			{
				sprintf_s(g_szAudioStreamTagUrl, 256, "%s", szTag + 8);
			}
		}
	}

	pAudioStream->ConstructInfo();

	BASS_ChannelSetSync(bassStream, BASS_SYNC_META, 0, SyncProc, 0);
	BASS_ChannelSetSync(bassStream, BASS_SYNC_OGG_CHANGE, 0, SyncProc, 0);

	if (!g_bAudioStreamStop)
	{
		while (!g_bAudioStreamStop)
		{
			BASS_StreamGetFilePosition(bassStream, BASS_FILEPOS_CONNECTED);
			if (pGame->IsMenuActive() || GetForegroundWindow() != pGame->GetMainWindowHwnd())
			{
				BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, 0);
			}
			else if (g_bAudioStreamUsePos)
			{
				CPlayerPed* pPlayerPed;
				float fDistance = 0.0f;

				if ((pPlayerPed = pGame->FindPlayerPed()) &&
					(fDistance = pPlayerPed->GetDistanceFromPoint(g_fAudioStreamX, g_fAudioStreamY, g_fAudioStreamZ)) <= g_fAudioStreamRadius)
				{
					BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, DWORD(1.0f - fDistance / g_fAudioStreamRadius * (pGame->GetAudio()->GetRadioVolume() * 8000.0f)));
				}
				else
				{
					BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, 0);
				}
			}
			else
			{
				BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, DWORD(pGame->GetAudio()->GetRadioVolume() * 7000.0f));
			}
			Sleep(20);
		}
	}

	BASS_ChannelStop(bassStream);
	bassStream = 0;
	g_bAudioStreamThreadWorked = false;
}

bool CAudioStream::Play(char* szURL, float fX, float fY, float fZ, float fRadius, bool bUsePos)
{
	if (!m_bInited) return false;

	Stop(true);

	if (bassStream)
	{
		BASS_ChannelStop(bassStream);
		bassStream = 0;
	}

	if (pGame->GetAudio()->GetRadioVolume() <= 0.0f)
		return false;

	memset(&g_szAudioStreamUrl, 0, 256);
	g_szAudioStreamUrl[256] = 0;
	strncpy(g_szAudioStreamUrl, szURL, 256);

	memset(&g_szAudioStreamTitle, 0, 256);
	g_szAudioStreamTitle[256] = 0;
	memset(&g_szAudioStreamTagName, 0, 256);
	g_szAudioStreamTagName[256] = 0;
	memset(&g_szAudioStreamTagUrl, 0, 256);
	g_szAudioStreamTagUrl[256] = 0;

	g_fAudioStreamX = fX;
	g_fAudioStreamY = fY;
	g_fAudioStreamZ = fZ;
	g_fAudioStreamRadius = fRadius;
	g_bAudioStreamUsePos = bUsePos;

	// TODO: Enable this after implementing CChatWindow::AddInfoMessage
	/*if (!pConfig->GetIntVariable("audiomsgoff"))
		pChatWindow->AddInfoMessage("Audio stream: %s", szURL);*/

	g_bAudioStreamStop = false;

	_beginthread(AudioStreamThread, 0, NULL);
	return true;
}

bool CAudioStream::Stop(bool bWaitThread)
{
	if (!m_bInited || !g_bAudioStreamThreadWorked)
		return false;

	g_bAudioStreamStop = true;

	if (bWaitThread)
	{
		while (g_bAudioStreamThreadWorked)
			Sleep(2);
	}

	BASS_StreamFree(bassStream);
	bassStream = 0;
	return true;
}

void CAudioStream::ControlGameRadio()
{
	if (!m_bInited) return;
	if (!g_bAudioStreamThreadWorked) return;

	pGame->GetAudio()->StartRadio(-1);
	pGame->GetAudio()->StopRadio();
}

void CAudioStream::DrawInfo()
{
	char szLabel[256 + 1];

	if (m_bInited)
	{
		if (g_bAudioStreamThreadWorked)
		{
			memset(&szLabel, 0, 256);
			if (strlen(g_szAudioStreamTitle) && strlen(g_szAudioStreamTagUrl))
			{
				sprintf_s(szLabel, 256, "%s - %s", g_szAudioStreamTitle, g_szAudioStreamTagUrl);

				RECT rect;

				rect.left = 15;
				rect.top = pGame->GetScreenHeight() - 20;
				rect.right = pGame->GetScreenWidth();
				rect.bottom = rect.top + 30;

				pDefaultFont->RenderText(0, szLabel, rect, 0x99FFFFFF, 0);
			}
		}
	}
}
