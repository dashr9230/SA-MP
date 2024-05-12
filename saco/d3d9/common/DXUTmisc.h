//--------------------------------------------------------------------------------------
// File: DXUTMisc.h
//
// Helper functions for Direct3D programming.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXUT_MISC_H
#define DXUT_MISC_H


//--------------------------------------------------------------------------------------
// Performs timer operations
// Use DXUTGetGlobalTimer() to get the global instance
//--------------------------------------------------------------------------------------
class CDXUTTimer
{
public:
    CDXUTTimer();

    void Reset(); // resets the timer
    void Start(); // starts the timer
    void Stop();  // stop (or pause) the timer
    void Advance(); // advance the timer by 0.1 seconds
    double GetAbsoluteTime(); // get the absolute system time
    double GetTime(); // get the current time
    double GetElapsedTime(); // get the time that elapsed between GetElapsedTime() calls
    bool IsStopped(); // returns true if timer stopped

protected:
    bool m_bUsingQPF;
    bool m_bTimerStopped;
    LONGLONG m_llQPFTicksPerSec;

    LONGLONG m_llStopTime;
    LONGLONG m_llLastElapsedTime;
    LONGLONG m_llBaseTime;
};

#endif
