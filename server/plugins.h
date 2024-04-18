
#ifndef _PLUGINS_H_INCLUDED
#define _PLUGINS_H_INCLUDED

#include "plugincommon.h"
#include "plugininternal.h"

#ifdef LINUX
	#include <dlfcn.h>

	#define PLUGIN_LOAD(p) dlopen(p, RTLD_LAZY)
	#define PLUGIN_UNLOAD dlclose
	#define PLUGIN_GETFUNCTION dlsym
#else
	#define PLUGIN_LOAD LoadLibrary
	#define PLUGIN_UNLOAD FreeLibrary
	#define PLUGIN_GETFUNCTION GetProcAddress
#endif

typedef bool (PLUGIN_CALL *ServerPluginLoad_t)(void **data);
typedef void (PLUGIN_CALL *ServerPluginUnload_t)();
typedef unsigned int (PLUGIN_CALL *ServerPluginSupports_t)();
typedef void (PLUGIN_CALL *ServerPluginProcessTick_t)();

typedef int (PLUGIN_CALL *ServerPluginAmxLoad_t)(AMX *amx);
typedef int (PLUGIN_CALL *ServerPluginAmxUnload_t)(AMX *amx);

struct ServerPlugin_s
{
#ifdef LINUX
	void *hModule;
#else
	HMODULE hModule;
#endif
	SUPPORTS_FLAGS dwSupportFlags;

	// Core Plugin Interface
	ServerPluginLoad_t		Load;
	ServerPluginUnload_t	Unload;
	ServerPluginSupports_t	Supports;
	ServerPluginProcessTick_t	ProcessTick;

	// AMX Plugin Interface
	ServerPluginAmxLoad_t	AmxLoad;
	ServerPluginAmxUnload_t AmxUnload;
};

typedef std::vector<ServerPlugin_s*> ServerPluginVector;

//---------------------------------------

class CPlugins
{
private:

	void* m_PluginData[MAX_PLUGIN_DATA];
	void* m_AMXExports[MAX_PLUGIN_AMX_EXPORT];

	ServerPluginVector m_Plugins;

	BOOL LoadSinglePlugin(char *szPluginPath);

public:
	CPlugins();
	~CPlugins();

	void LoadPlugins(char *szSearchPath);
	void LoadPluginsSearch(char *szSearchPath);
	DWORD GetPluginCount();
	ServerPlugin_s* GetPlugin(DWORD index);

	void DoProcessTick();

	void DoAmxLoad(AMX *amx);
	void DoAmxUnload(AMX *amx);
};

#endif
