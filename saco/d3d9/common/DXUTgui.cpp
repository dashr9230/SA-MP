//--------------------------------------------------------------------------------------
// File: DXUTgui.cpp
//
// Desc: 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "DXUTgui.h"
#undef min // use __min instead
#undef max // use __max instead

//--------------------------------------------------------------------------------------
CDXUTDialogResourceManager::CDXUTDialogResourceManager()
{
    m_pd3dDevice = NULL;
    m_pStateBlock = NULL;
    m_pSprite = NULL;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialogResourceManager::OnResetDevice()
{
    HRESULT hr = S_OK;

    for( int i=0; i < m_FontCache.GetSize(); i++ )
    {
        DXUTFontNode* pFontNode = m_FontCache.GetAt( i );

        if( pFontNode->pFont )
            pFontNode->pFont->OnResetDevice();
    }

    if( m_pSprite )
        m_pSprite->OnResetDevice();

    m_pd3dDevice->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock );

    return S_OK;
}

//--------------------------------------------------------------------------------------
void DXUTBlendColor::Init( D3DCOLOR defaultColor, D3DCOLOR disabledColor, D3DCOLOR hiddenColor )
{
    for( int i=0; i < MAX_CONTROL_STATES; i++ )
    {
        States[ i ] = defaultColor;
    }

    States[ DXUT_STATE_DISABLED ] = disabledColor;
    States[ DXUT_STATE_HIDDEN ] = hiddenColor;
    Current = hiddenColor;
}


//--------------------------------------------------------------------------------------
void DXUTBlendColor::Blend( UINT iState, float fElapsedTime, float fRate )
{
    D3DXCOLOR destColor = States[ iState ];
    D3DXColorLerp( &Current, &Current, &destColor, 1.0f - powf( fRate, 30 * fElapsedTime ) );
}



