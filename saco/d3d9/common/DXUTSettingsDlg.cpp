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


//--------------------------------------------------------------------------------------
// Returns the string for the given D3DMULTISAMPLE_TYPE.
//--------------------------------------------------------------------------------------
TCHAR* DXUTMultisampleTypeToString(D3DMULTISAMPLE_TYPE MultiSampleType)
{
    switch (MultiSampleType)
    {
    case D3DMULTISAMPLE_NONE:       return "D3DMULTISAMPLE_NONE";
    case D3DMULTISAMPLE_NONMASKABLE: return "D3DMULTISAMPLE_NONMASKABLE";
    case D3DMULTISAMPLE_2_SAMPLES:  return "D3DMULTISAMPLE_2_SAMPLES";
    case D3DMULTISAMPLE_3_SAMPLES:  return "D3DMULTISAMPLE_3_SAMPLES";
    case D3DMULTISAMPLE_4_SAMPLES:  return "D3DMULTISAMPLE_4_SAMPLES";
    case D3DMULTISAMPLE_5_SAMPLES:  return "D3DMULTISAMPLE_5_SAMPLES";
    case D3DMULTISAMPLE_6_SAMPLES:  return "D3DMULTISAMPLE_6_SAMPLES";
    case D3DMULTISAMPLE_7_SAMPLES:  return "D3DMULTISAMPLE_7_SAMPLES";
    case D3DMULTISAMPLE_8_SAMPLES:  return "D3DMULTISAMPLE_8_SAMPLES";
    case D3DMULTISAMPLE_9_SAMPLES:  return "D3DMULTISAMPLE_9_SAMPLES";
    case D3DMULTISAMPLE_10_SAMPLES: return "D3DMULTISAMPLE_10_SAMPLES";
    case D3DMULTISAMPLE_11_SAMPLES: return "D3DMULTISAMPLE_11_SAMPLES";
    case D3DMULTISAMPLE_12_SAMPLES: return "D3DMULTISAMPLE_12_SAMPLES";
    case D3DMULTISAMPLE_13_SAMPLES: return "D3DMULTISAMPLE_13_SAMPLES";
    case D3DMULTISAMPLE_14_SAMPLES: return "D3DMULTISAMPLE_14_SAMPLES";
    case D3DMULTISAMPLE_15_SAMPLES: return "D3DMULTISAMPLE_15_SAMPLES";
    case D3DMULTISAMPLE_16_SAMPLES: return "D3DMULTISAMPLE_16_SAMPLES";
    default:                        return "Unknown Multisample Type";
    }
}


//--------------------------------------------------------------------------------------
// Returns the string for the given vertex processing type
//--------------------------------------------------------------------------------------
TCHAR* DXUTVertexProcessingTypeToString(DWORD vpt)
{
    switch (vpt)
    {
    case D3DCREATE_SOFTWARE_VERTEXPROCESSING: return "Software vertex processing";
    case D3DCREATE_MIXED_VERTEXPROCESSING:    return "Mixed vertex processing";
    case D3DCREATE_HARDWARE_VERTEXPROCESSING: return "Hardware vertex processing";
    case D3DCREATE_PUREDEVICE:                return "Pure hardware vertex processing";
    default:                                  return "Unknown vertex processing type";
    }
}


