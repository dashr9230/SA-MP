
#ifndef _PLUGINS_H_INCLUDED
#define _PLUGINS_H_INCLUDED

#include "plugincommon.h"
#include "plugininternal.h"

class CPlugins // size: W: 1216 L: 1212
{
private:

	void* m_PluginData[MAX_PLUGIN_DATA];
	void* m_AMXExports[MAX_PLUGIN_AMX_EXPORT];

public:
	CPlugins();
	~CPlugins();

	void LoadPlugins(char *szSearchPath);
};

#endif
