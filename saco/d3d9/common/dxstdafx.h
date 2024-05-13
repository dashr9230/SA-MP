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

// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>


#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>

#include "DXUTmisc.h"

#endif // !defined(DXSDK_STDAFX_H)
