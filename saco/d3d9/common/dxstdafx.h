//--------------------------------------------------------------------------------------
// File: DxStdAfx.h
//
// Desc: Standard includes and precompiled headers for DXUT
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXSDK_STDAFX_H
#define DXSDK_STDAFX_H

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include <windows.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <math.h>      
#include <limits.h>      
#include <stdio.h>
#include <multimon.h> 

// CRT's memory leak detection
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL WM_MOUSELAST+1 
    // Message ID for IntelliMouse wheel
#endif

#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES   104
#endif


// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

// Hack for DXTrace (dxerr9.lib is damn 3mbs)
#ifdef DXTrace
#undef DXTrace
HRESULT WINAPI DXTraceWrapper( const char* strFile, DWORD dwLine, HRESULT hr, const char* strMsg, BOOL bPopMsgBox );
#define DXTrace DXTraceWrapper
#endif

// DirectSound includes
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>

#include "DXUT.h"
#include "DXUTmisc.h"
#include "DXUTenum.h"
#include "DXUTmesh.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#include "DXUTSound.h"

#ifndef V
	#define V(x)
#endif

#ifndef V_RETURN
	#define V_RETURN(x)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#endif // !defined(DXSDK_STDAFX_H)
