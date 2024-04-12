
#ifndef _PLUGININTERNAL_H_INCLUDED
#define _PLUGININTERNAL_H_INCLUDED

#define MAX_PLUGIN_DATA 256
#define MAX_PLUGIN_AMX_EXPORT 44

enum PLUGIN_DATA_TYPE_INTERNAL 
{
	PLUGIN_DATA_NETGAME		  = 0xE1,	// CNetGame* GetNetGame();
	PLUGIN_DATA_RAKSERVER	  = 0xE2, // RakServerInterface* PluginGetRakServer()
	PLUGIN_DATA_LOADFSCRIPT	  = 0xE3, // bool LoadFilterscriptFromMemory(char* pFileName, char* pFileData)
	PLUGIN_DATA_UNLOADFSCRIPT = 0xE5, // bool UnloadFilterScript(char* pFileName)
	PLUGIN_DATA_CONSOLE		  = 0xE4, // CConsole* GetConsole();
};

#endif
