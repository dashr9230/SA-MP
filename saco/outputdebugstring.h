
#pragma once

// Ensure OutputDebugString() is disabled on release builds

#ifndef _DEBUG

#undef OutputDebugString
#define OutputDebugString(a) NULL

#undef OutputDebugStringW
#define OutputDebugStringW(a) NULL

#endif
