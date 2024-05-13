//--------------------------------------------------------------------------------------
// File: DXUTMisc.cpp
//
// Shortcut macros and functions for using DX objects
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#undef min // use __min instead
#undef max // use __max instead

//--------------------------------------------------------------------------------------
// Global/Static Members
//--------------------------------------------------------------------------------------
CDXUTTimer* DXUTGetGlobalTimer()
{
    // Using an accessor function gives control of the construction order
    static CDXUTTimer timer;
    return &timer;
}


//--------------------------------------------------------------------------------------
CDXUTTimer::CDXUTTimer()
{
    m_bUsingQPF         = false;
    m_bTimerStopped     = true;
    m_llQPFTicksPerSec  = 0;

    m_llStopTime        = 0;
    m_llLastElapsedTime = 0;
    m_llBaseTime        = 0;

    // Use QueryPerformanceFrequency() to get frequency of timer.  
    LARGE_INTEGER qwTicksPerSec;
    m_bUsingQPF = (bool) (QueryPerformanceFrequency( &qwTicksPerSec ) != 0);
    m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}


//--------------------------------------------------------------------------------------
void CDXUTTimer::Reset()
{
    if( !m_bUsingQPF )
        return;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    m_llBaseTime        = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_llStopTime        = 0;
    m_bTimerStopped     = FALSE;
}


//--------------------------------------------------------------------------------------
void CDXUTTimer::Start()
{
    if( !m_bUsingQPF )
        return;

    // Get the current time
    LARGE_INTEGER qwTime;
    QueryPerformanceCounter( &qwTime );

    if( m_bTimerStopped )
        m_llBaseTime += qwTime.QuadPart - m_llStopTime;
    m_llStopTime = 0;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = FALSE;
}


//--------------------------------------------------------------------------------------
void CDXUTTimer::Stop()
{
    if( !m_bUsingQPF )
        return;

    if( !m_bTimerStopped )
    {
        // Get either the current time or the stop time
        LARGE_INTEGER qwTime;
        if( m_llStopTime != 0 )
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        m_llStopTime = qwTime.QuadPart;
        m_llLastElapsedTime = qwTime.QuadPart;
        m_bTimerStopped = TRUE;
    }
}


//--------------------------------------------------------------------------------------
void CDXUTTimer::Advance()
{
    if( !m_bUsingQPF )
        return;

    m_llStopTime += m_llQPFTicksPerSec/10;
}


//--------------------------------------------------------------------------------------
double CDXUTTimer::GetAbsoluteTime()
{
    if( !m_bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    double fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;

    return fTime;
}


//--------------------------------------------------------------------------------------
double CDXUTTimer::GetTime()
{
    if( !m_bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;

    return fAppTime;
}


//--------------------------------------------------------------------------------------
double CDXUTTimer::GetElapsedTime()
{
    if( !m_bUsingQPF )
        return -1.0;

    // Get either the current time or the stop time
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );

    double fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
    m_llLastElapsedTime = qwTime.QuadPart;

    return fElapsedTime;
}


//--------------------------------------------------------------------------------------
bool CDXUTTimer::IsStopped()
{
    return m_bTimerStopped;
}


//--------------------------------------------------------------------------------------
// Returns pointer to static media search buffer
//--------------------------------------------------------------------------------------
TCHAR* DXUTMediaSearchPath()
{
    static TCHAR s_strMediaSearchPath[MAX_PATH] = {0};
    return s_strMediaSearchPath;

}   

//--------------------------------------------------------------------------------------
LPCTSTR DXUTGetMediaSearchPath()
{
    return DXUTMediaSearchPath();
}


//--------------------------------------------------------------------------------------
HRESULT DXUTSetMediaSearchPath( LPCTSTR strPath )
{
    HRESULT hr;

    TCHAR* s_strSearchPath = DXUTMediaSearchPath();

    hr = StringCchCopy( s_strSearchPath, MAX_PATH, strPath );   
    if( SUCCEEDED(hr) )
    {
        // append slash if needed
        size_t ch;
        hr = StringCchLength( s_strSearchPath, MAX_PATH, &ch );
        if( SUCCEEDED(hr) && s_strSearchPath[ch-1] != L'\\')
        {
            hr = StringCchCat( s_strSearchPath, MAX_PATH, "\\" );
        }
    }

    return hr;
}


//--------------------------------------------------------------------------------------
// Search a set of typical directories
//--------------------------------------------------------------------------------------
bool DXUTFindMediaSearchTypicalDirs( TCHAR* strSearchPath, int cchSearch, LPCTSTR strLeaf, 
                                     TCHAR* strExePath, TCHAR* strExeName )
{
    // Typical directories:
    //      .\
    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
    //      %EXE_DIR%\..\..\%EXE_NAME%
    //      DXSDK media path

    // Search in .\  
    StringCchCopy( strSearchPath, cchSearch, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in ..\  
    StringCchPrintf( strSearchPath, cchSearch, "..\\%s", strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in ..\..\ 
    StringCchPrintf( strSearchPath, cchSearch, "..\\..\\%s", strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in ..\..\ 
    StringCchPrintf( strSearchPath, cchSearch, "..\\..\\%s", strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in the %EXE_DIR%\ 
    StringCchPrintf( strSearchPath, cchSearch, "%s\\%s", strExePath, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in the %EXE_DIR%\..\ 
    StringCchPrintf( strSearchPath, cchSearch, "%s\\..\\%s", strExePath, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in the %EXE_DIR%\..\..\ 
    StringCchPrintf( strSearchPath, cchSearch, "%s\\..\\..\\%s", strExePath, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in "%EXE_DIR%\..\%EXE_NAME%\".  This matches the DirectX SDK layout
    StringCchPrintf( strSearchPath, cchSearch, "%s\\..\\%s\\%s", strExePath, strExeName, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in "%EXE_DIR%\..\..\%EXE_NAME%\".  This matches the DirectX SDK layout
    StringCchPrintf( strSearchPath, cchSearch, "%s\\..\\..\\%s\\%s", strExePath, strExeName, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in media search dir 
    TCHAR* s_strSearchPath = DXUTMediaSearchPath();
    if( s_strSearchPath[0] != 0 )
    {
        StringCchPrintf( strSearchPath, cchSearch, "%s%s", s_strSearchPath, strLeaf ); 
        if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
            return true;
    }

    return false;
}



//--------------------------------------------------------------------------------------
// Search parent directories starting at strStartAt, and appending strLeafName
// at each parent directory.  It stops at the root directory.
//--------------------------------------------------------------------------------------
bool DXUTFindMediaSearchParentDirs( TCHAR* strSearchPath, int cchSearch, TCHAR* strStartAt, TCHAR* strLeafName )
{
    TCHAR strFullPath[MAX_PATH] = {0};
    TCHAR strFullFileName[MAX_PATH] = {0};
    TCHAR strSearch[MAX_PATH] = {0};
    TCHAR* strFilePart = NULL;

    GetFullPathName( strStartAt, MAX_PATH, strFullPath, &strFilePart );
    if( strFilePart == NULL )
        return false;
   
    while( strFilePart != NULL && *strFilePart != '\0' )
    {
        StringCchPrintf( strFullFileName, MAX_PATH, "%s\\%s", strFullPath, strLeafName ); 
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            StringCchCopy( strSearchPath, cchSearch, strFullFileName ); 
            return true;
        }

        StringCchPrintf( strSearch, MAX_PATH, "%s\\..", strFullPath ); 
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }

    return false;
}



//--------------------------------------------------------------------------------------
// Direct3D9 dynamic linking support -- calls top-level D3D9 APIs with graceful
// failure if APIs are not present.
//--------------------------------------------------------------------------------------

// Function prototypes
typedef IDirect3D9* (WINAPI * LPDIRECT3DCREATE9) (UINT);
typedef INT         (WINAPI * LPD3DPERF_BEGINEVENT)(D3DCOLOR, LPCTSTR);
typedef INT         (WINAPI * LPD3DPERF_ENDEVENT)(void);
typedef VOID        (WINAPI * LPD3DPERF_SETMARKER)(D3DCOLOR, LPCTSTR);
typedef VOID        (WINAPI * LPD3DPERF_SETREGION)(D3DCOLOR, LPCTSTR);
typedef BOOL        (WINAPI * LPD3DPERF_QUERYREPEATFRAME)(void);
typedef VOID        (WINAPI * LPD3DPERF_SETOPTIONS)( DWORD dwOptions );
typedef DWORD       (WINAPI * LPD3DPERF_GETSTATUS)( void );

// Module and function pointers
static HMODULE s_hModD3D9 = NULL;
static LPDIRECT3DCREATE9 s_DynamicDirect3DCreate9 = NULL;
static LPD3DPERF_BEGINEVENT s_DynamicD3DPERF_BeginEvent = NULL;
static LPD3DPERF_ENDEVENT s_DynamicD3DPERF_EndEvent = NULL;
static LPD3DPERF_SETMARKER s_DynamicD3DPERF_SetMarker = NULL;
static LPD3DPERF_SETREGION s_DynamicD3DPERF_SetRegion = NULL;
static LPD3DPERF_QUERYREPEATFRAME s_DynamicD3DPERF_QueryRepeatFrame = NULL;
static LPD3DPERF_SETOPTIONS s_DynamicD3DPERF_SetOptions = NULL;
static LPD3DPERF_GETSTATUS s_DynamicD3DPERF_GetStatus = NULL;

// Ensure function pointers are initialized
static bool DXUT_EnsureD3DAPIs( void )
{
    // If module is non-NULL, this function has already been called.  Note
    // that this doesn't guarantee that all D3D9 procaddresses were found.
    if( s_hModD3D9 != NULL )
        return true;

    // This may fail if DirectX 9 isn't installed
    TCHAR wszPath[MAX_PATH+1];
    if( !::GetSystemDirectory( wszPath, MAX_PATH+1 ) )
        return false;
    StringCchCat( wszPath, MAX_PATH, "\\d3d9.dll" );
    s_hModD3D9 = LoadLibrary( wszPath );
    if( s_hModD3D9 == NULL ) 
        return false;
    s_DynamicDirect3DCreate9 = (LPDIRECT3DCREATE9)GetProcAddress( s_hModD3D9, "Direct3DCreate9" );
    s_DynamicD3DPERF_BeginEvent = (LPD3DPERF_BEGINEVENT)GetProcAddress( s_hModD3D9, "D3DPERF_BeginEvent" );
    s_DynamicD3DPERF_EndEvent = (LPD3DPERF_ENDEVENT)GetProcAddress( s_hModD3D9, "D3DPERF_EndEvent" );
    s_DynamicD3DPERF_SetMarker = (LPD3DPERF_SETMARKER)GetProcAddress( s_hModD3D9, "D3DPERF_SetMarker" );
    s_DynamicD3DPERF_SetRegion = (LPD3DPERF_SETREGION)GetProcAddress( s_hModD3D9, "D3DPERF_SetRegion" );
    s_DynamicD3DPERF_QueryRepeatFrame = (LPD3DPERF_QUERYREPEATFRAME)GetProcAddress( s_hModD3D9, "D3DPERF_QueryRepeatFrame" );
    s_DynamicD3DPERF_SetOptions = (LPD3DPERF_SETOPTIONS)GetProcAddress( s_hModD3D9, "D3DPERF_SetOptions" );
    s_DynamicD3DPERF_GetStatus = (LPD3DPERF_GETSTATUS)GetProcAddress( s_hModD3D9, "D3DPERF_GetStatus" );
    return true;
}

IDirect3D9 * WINAPI DXUT_Dynamic_Direct3DCreate9(UINT SDKVersion) 
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicDirect3DCreate9 != NULL )
        return s_DynamicDirect3DCreate9( SDKVersion );
    else
        return NULL;
}

int WINAPI DXUT_Dynamic_D3DPERF_BeginEvent( D3DCOLOR col, LPCTSTR wszName )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_BeginEvent != NULL )
        return s_DynamicD3DPERF_BeginEvent( col, wszName );
    else
        return -1;
}

int WINAPI DXUT_Dynamic_D3DPERF_EndEvent( void )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_EndEvent != NULL )
        return s_DynamicD3DPERF_EndEvent();
    else
        return -1;
}

void WINAPI DXUT_Dynamic_D3DPERF_SetMarker( D3DCOLOR col, LPCTSTR wszName )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_SetMarker != NULL )
        s_DynamicD3DPERF_SetMarker( col, wszName );
}

void WINAPI DXUT_Dynamic_D3DPERF_SetRegion( D3DCOLOR col, LPCTSTR wszName )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_SetRegion != NULL )
        s_DynamicD3DPERF_SetRegion( col, wszName );
}

BOOL WINAPI DXUT_Dynamic_D3DPERF_QueryRepeatFrame( void )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_QueryRepeatFrame != NULL )
        return s_DynamicD3DPERF_QueryRepeatFrame();
    else
        return FALSE;
}

void WINAPI DXUT_Dynamic_D3DPERF_SetOptions( DWORD dwOptions )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_SetOptions != NULL )
        s_DynamicD3DPERF_SetOptions( dwOptions );
}

DWORD WINAPI DXUT_Dynamic_D3DPERF_GetStatus( void )
{
    if( DXUT_EnsureD3DAPIs() && s_DynamicD3DPERF_GetStatus != NULL )
        return s_DynamicD3DPERF_GetStatus();
    else
        return 0;
}

