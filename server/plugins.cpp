
#include "main.h"

//---------------------------------------
// Some Helpers

extern "C" RakServerInterface* PluginGetRakServer()
{
	if (pNetGame != NULL)
		return pNetGame->GetRakServer();
	else
		return NULL;
}

extern "C" CNetGame* PluginGetNetGame()
{
	return pNetGame;
}

extern "C" CConsole* PluginGetConsole()
{
	return pConsole;
}

extern "C" bool PluginUnloadFilterScript(char* pFileName)
{
	if (pNetGame != NULL)
		return pNetGame->GetFilterScripts()->UnloadOneFilterScript(pFileName);
	else
		return false;
}

extern "C" bool PluginLoadFilterScriptFromMemory(char* pFileName, char* pFileData)
{
	if (pNetGame != NULL)
		return pNetGame->GetFilterScripts()->LoadFilterScriptFromMemory(pFileName, pFileData);
	else
		return false;
}

extern "C" int PluginCallPublicFS(char *szFunctionName)
{
	if (pNetGame != NULL)
		return pNetGame->GetFilterScripts()->CallPublic(szFunctionName);
	else
		return 0;
}

extern "C" int PluginCallPublicGM(char *szFunctionName)
{
	if (pNetGame != NULL && pNetGame->GetGameMode())
		return pNetGame->GetGameMode()->CallPublic(szFunctionName);
	else
		return 0;
}

//---------------------------------------

CPlugins::CPlugins()
{
	// Set up the table of AMX functions to be exported
#if defined AMX_ALIGN || defined AMX_INIT
	m_AMXExports[PLUGIN_AMX_EXPORT_Align16] = (void*)&amx_Align16;
	m_AMXExports[PLUGIN_AMX_EXPORT_Align32] = (void*)&amx_Align32;
#if defined _I64_MAX || defined HAVE_I64
	m_AMXExports[PLUGIN_AMX_EXPORT_Align64] = (void*)&amx_Align64;
#endif
#endif
	m_AMXExports[PLUGIN_AMX_EXPORT_Allot] = (void*)&amx_Allot;
	m_AMXExports[PLUGIN_AMX_EXPORT_Callback] = (void*)&amx_Callback;
	m_AMXExports[PLUGIN_AMX_EXPORT_Cleanup] = (void*)&amx_Cleanup;
	m_AMXExports[PLUGIN_AMX_EXPORT_Clone] = (void*)&amx_Clone;
	m_AMXExports[PLUGIN_AMX_EXPORT_Exec] = (void*)&amx_Exec;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindNative] = (void*)&amx_FindNative;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindPublic] = (void*)&amx_FindPublic;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindPubVar] = (void*)&amx_FindPubVar;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindTagId] = (void*)&amx_FindTagId;
	m_AMXExports[PLUGIN_AMX_EXPORT_Flags] = (void*)&amx_Flags;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetAddr] = (void*)&amx_GetAddr;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetNative] = (void*)&amx_GetNative;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetPublic] = (void*)&amx_GetPublic;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetPubVar] = (void*)&amx_GetPubVar;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetString] = (void*)&amx_GetString;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetTag] = (void*)&amx_GetTag;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetUserData] = (void*)&amx_GetUserData;
	m_AMXExports[PLUGIN_AMX_EXPORT_Init] = (void*)&amx_Init;
	m_AMXExports[PLUGIN_AMX_EXPORT_InitJIT] = (void*)&amx_InitJIT;
	m_AMXExports[PLUGIN_AMX_EXPORT_MemInfo] = (void*)&amx_MemInfo;
	m_AMXExports[PLUGIN_AMX_EXPORT_NameLength] = (void*)&amx_NameLength;
	m_AMXExports[PLUGIN_AMX_EXPORT_NativeInfo] = (void*)&amx_NativeInfo;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumNatives] = (void*)&amx_NumNatives;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumPublics] = (void*)&amx_NumPublics;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumPubVars] = (void*)&amx_NumPubVars;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumTags] = (void*)&amx_NumTags;
	m_AMXExports[PLUGIN_AMX_EXPORT_Push] = (void*)&amx_Push;
	m_AMXExports[PLUGIN_AMX_EXPORT_PushArray] = (void*)&amx_PushArray;
	m_AMXExports[PLUGIN_AMX_EXPORT_PushString] = (void*)&amx_PushString;
	m_AMXExports[PLUGIN_AMX_EXPORT_RaiseError] = (void*)&amx_RaiseError;
	m_AMXExports[PLUGIN_AMX_EXPORT_Register] = (void*)&amx_Register;
	m_AMXExports[PLUGIN_AMX_EXPORT_Release] = (void*)&amx_Release;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetCallback] = (void*)&amx_SetCallback;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetDebugHook] = (void*)&amx_SetDebugHook;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetString] = (void*)&amx_SetString;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetUserData] = (void*)&amx_SetUserData;
	m_AMXExports[PLUGIN_AMX_EXPORT_StrLen] = (void*)&amx_StrLen;
	//m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Check] = (void*)&amx_UTF8Check;
	//m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Get] = (void*)&amx_UTF8Get;
	//m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Len] = (void*)&amx_UTF8Len;
	//m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Put] = (void*)&amx_UTF8Put;

	// Set up table of Plugin exports
	m_PluginData[PLUGIN_DATA_LOGPRINTF] = (void*)&logprintf;

	m_PluginData[PLUGIN_DATA_AMX_EXPORTS] = m_AMXExports;
	m_PluginData[PLUGIN_DATA_CALLPUBLIC_FS] = (void*)&PluginCallPublicFS;
	m_PluginData[PLUGIN_DATA_CALLPUBLIC_GM] = (void*)&PluginCallPublicGM;

	// Internals
	m_PluginData[PLUGIN_DATA_NETGAME] = (void*)&PluginGetNetGame;
	m_PluginData[PLUGIN_DATA_CONSOLE] = (void*)&PluginGetConsole;
	m_PluginData[PLUGIN_DATA_RAKSERVER] = (void*)&PluginGetRakServer;
	m_PluginData[PLUGIN_DATA_LOADFSCRIPT] = (void*)&PluginLoadFilterScriptFromMemory;
	m_PluginData[PLUGIN_DATA_UNLOADFSCRIPT] = (void*)&PluginUnloadFilterScript;
}

//---------------------------------------

CPlugins::~CPlugins()
{
	ServerPluginVector::iterator itor;

	for(itor=m_Plugins.begin(); itor!=m_Plugins.end(); itor++) 
	{
		ServerPlugin_s* pSPlugin = *itor;
		if (pSPlugin->Unload)
			pSPlugin->Unload();
		PLUGIN_UNLOAD(pSPlugin->hModule);
		delete pSPlugin;
	}
}

//---------------------------------------

BOOL CPlugins::LoadSinglePlugin(char *szPluginPath)
{
	// Load it
	ServerPlugin_s* pSPlugin;
	pSPlugin = new ServerPlugin_s();

	pSPlugin->hModule = PLUGIN_LOAD(szPluginPath);
	if (pSPlugin->hModule == NULL)
	{
		// Failed to load
		delete pSPlugin;
		return FALSE;
	}

	pSPlugin->Load = (ServerPluginLoad_t)PLUGIN_GETFUNCTION(pSPlugin->hModule, "Load");
	pSPlugin->Unload = (ServerPluginUnload_t)PLUGIN_GETFUNCTION(pSPlugin->hModule, "Unload");
	pSPlugin->Supports = (ServerPluginSupports_t)PLUGIN_GETFUNCTION(pSPlugin->hModule, "Supports");

	if (pSPlugin->Load == NULL || pSPlugin->Supports == NULL) 
	{
		// Not proper architecture!
		logprintf("  Plugin does not conform to architecture.");
		PLUGIN_UNLOAD(pSPlugin->hModule);
		delete pSPlugin;
		return FALSE;
	}

	pSPlugin->dwSupportFlags = (SUPPORTS_FLAGS)pSPlugin->Supports();

	if ((pSPlugin->dwSupportFlags & SUPPORTS_VERSION_MASK) > SUPPORTS_VERSION) 
	{
		// Unsupported version, sorry!
		logprintf("  Unsupported version - This plugin requires version %x.", (pSPlugin->dwSupportFlags & SUPPORTS_VERSION_MASK));
		PLUGIN_UNLOAD(pSPlugin->hModule);
		delete pSPlugin;
		return FALSE;
	}

	if ((pSPlugin->dwSupportFlags & SUPPORTS_AMX_NATIVES) != 0)
	{
		pSPlugin->AmxLoad = (ServerPluginAmxLoad_t)PLUGIN_GETFUNCTION(pSPlugin->hModule, "AmxLoad");
		pSPlugin->AmxUnload = (ServerPluginAmxUnload_t)PLUGIN_GETFUNCTION(pSPlugin->hModule, "AmxUnload");
	} 
	else
	{
		pSPlugin->AmxLoad = NULL;
		pSPlugin->AmxUnload = NULL;
	}

	if ((pSPlugin->dwSupportFlags & SUPPORTS_PROCESS_TICK) != 0)
	{
		pSPlugin->ProcessTick = (ServerPluginProcessTick_t)PLUGIN_GETFUNCTION(pSPlugin->hModule, "ProcessTick");
	}
	else
	{
		pSPlugin->ProcessTick = NULL;
	}

	if (!pSPlugin->Load(m_PluginData))
	{
		// Initialize failed!
		PLUGIN_UNLOAD(pSPlugin->hModule);
		delete pSPlugin;
		return FALSE;
	}

	m_Plugins.push_back(pSPlugin);

	return TRUE;
}

//---------------------------------------

void CPlugins::LoadPlugins(char *szSearchPath)
{
	char szPath[MAX_PATH];
	char szFullPath[MAX_PATH];
	char *szDlerror = NULL;
	strcpy(szPath, szSearchPath);

#ifdef LINUX
	if (szPath[strlen(szPath)-1] != '/')
		strcat(szPath, "/");
#else
	if (szPath[strlen(szPath)-1] != '\\')
		strcat(szPath, "\\");
#endif

	logprintf("");
	logprintf("Server Plugins");
	logprintf("--------------");

	char* szFilename = strtok(pConsole->GetStringVariable("plugins"), " ");
	while (szFilename)
	{
		logprintf(" Loading plugin: %s", szFilename);

		strcpy(szFullPath, szPath);
		strcat(szFullPath, szFilename);

		if (LoadSinglePlugin(szFullPath))
		{
			logprintf("  Loaded.");
		}
		else
		{
#ifdef LINUX
			szDlerror = PLUGIN_GETERROR();
			if (szDlerror)
				logprintf("  Failed (%s)", szDlerror);
			else
				logprintf("  Failed.");
#else
			logprintf("  Failed.");
#endif
		}

		szFilename = strtok(NULL, " ");
	}
	logprintf(" Loaded %d plugins.\n", GetPluginCount());
}

// [OBSOLETE: Using the non search defined method]
void CPlugins::LoadPluginsSearch(char *szSearchPath)
{
#ifdef LINUX
	DIR *dir = opendir(szSearchPath);
	struct dirent *file;
	char *szFilename = NULL;
	char *szExt = NULL;
	int iStrLen;
	char szPath[255];
	strcpy(szPath, szSearchPath);
	if (szPath[strlen(szPath)-1] != '/') strcat(szPath, "/");
	char szFullPath[255];
	char *szDlerror = NULL;
	if (dir)
	{
		logprintf("");
		logprintf("Server Plugins");
		logprintf("--------------");

		// Load the plugins
		while (dir)
		{
			strcpy(szFullPath, szPath);
			if ((file = readdir(dir)) != NULL)
			{
				szFilename = (char*)file->d_name;
				iStrLen = strlen(szFilename);
				if (iStrLen > 3)
				{
					szExt = szFilename+iStrLen-3;
					if (strcmp(".so", szExt) == 0)
					{
						logprintf(" Loading plugin: %s", szFilename);
						strcat(szFullPath, szFilename);
						bool bSuccess = LoadSinglePlugin(szFullPath);
						if (!bSuccess)
						{
							szDlerror = PLUGIN_GETERROR();
							if (szDlerror)
								logprintf("  Failed (%s)", szDlerror);
							else
								logprintf(("  Failed.");
						}
					}
				}
			}
			else
			{
				closedir(dir);
				dir = NULL;
			}
		}
		logprintf("");
	}
#else
	char szPath[MAX_PATH];
	char szSearch[MAX_PATH];

	strcpy(szPath, szSearchPath);

	char cLastChar = szPath[strlen(szPath)-1];
	if (cLastChar != '\\' && cLastChar != '/')
		strcat(szPath, "\\");

	DWORD szPathLen = strlen(szPath);

	strcpy(szSearch, szPath);
	strcat(szSearch, "*.dll");

	WIN32_FIND_DATA fdFindData;
	HANDLE hFindFile = FindFirstFile(szSearch, &fdFindData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		logprintf("");
		logprintf("Server Plugins");
		logprintf("--------------");

		// Load the plugins
		while(true) {
			logprintf(" Loading plugin: %s", fdFindData.cFileName);
			strcpy(szPath+szPathLen, fdFindData.cFileName);
			BOOL bSuccess = LoadSinglePlugin(szPath);
			if (!bSuccess)
				logprintf("  Failed.");
			if(!FindNextFile(hFindFile, &fdFindData))
				break;
		}
		logprintf("");
	}
	FindClose(hFindFile);
#endif
}

//---------------------------------------

DWORD CPlugins::GetPluginCount()
{
	return (DWORD)m_Plugins.size();
}

//---------------------------------------

ServerPlugin_s* CPlugins::GetPlugin(DWORD index)
{
	return m_Plugins[index];
}

//---------------------------------------

void CPlugins::DoProcessTick()
{
	ServerPluginVector::iterator itor;

	for(itor=m_Plugins.begin(); itor!=m_Plugins.end(); itor++) {
		ServerPlugin_s* pSPlugin = *itor;
		if ((pSPlugin->dwSupportFlags & SUPPORTS_PROCESS_TICK) != 0)
			if (pSPlugin->ProcessTick != NULL)
				pSPlugin->ProcessTick();
	}
}

//---------------------------------------

void CPlugins::DoAmxLoad(AMX *amx)
{
	ServerPluginVector::iterator itor;

	for(itor=m_Plugins.begin(); itor!=m_Plugins.end(); itor++) {
		ServerPlugin_s* pSPlugin = *itor;
		if ((pSPlugin->dwSupportFlags & SUPPORTS_AMX_NATIVES) != 0)
			if (pSPlugin->AmxLoad != NULL)
				pSPlugin->AmxLoad(amx);
	}
}

//---------------------------------------

void CPlugins::DoAmxUnload(AMX *amx)
{
	ServerPluginVector::iterator itor;

	for(itor=m_Plugins.begin(); itor!=m_Plugins.end(); itor++) {
		ServerPlugin_s* pSPlugin = *itor;
		if ((pSPlugin->dwSupportFlags & SUPPORTS_AMX_NATIVES) != 0)
			if (pSPlugin->AmxUnload != NULL)
				pSPlugin->AmxUnload(amx);
	}
}

//---------------------------------------
