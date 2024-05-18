//--------------------------------------------------------------------------------------
// File: DXUTSettingsDlg.cpp
//
// Dialog for selection of device settings 
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#undef min // use __min instead
#undef max // use __max instead


//--------------------------------------------------------------------------------------
// Returns the string for the given D3DDEVTYPE.
//--------------------------------------------------------------------------------------
TCHAR* DXUTD3DDeviceTypeToString(D3DDEVTYPE devType)
{
    switch (devType)
    {
        case D3DDEVTYPE_HAL:        return "D3DDEVTYPE_HA";
        case D3DDEVTYPE_SW:         return "D3DDEVTYPE_SW";
        case D3DDEVTYPE_REF:        return "D3DDEVTYPE_REF";
        default:                    return "Unknown devType";
    }
}


