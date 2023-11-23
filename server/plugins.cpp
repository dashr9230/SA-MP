
#include "main.h"

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

	// TODO: CPlugins::CPlugins W: 0046A1D0 L: constructor
}

CPlugins::~CPlugins()
{
	// TODO: CPlugins::~CPlugins W: 00469DB0 L: 080D20E0
}

void CPlugins::LoadPlugins(char *szSearchPath)
{
	// TODO: CPlugins::LoadPlugins W: 0046A570 L: 080D29E0
}
