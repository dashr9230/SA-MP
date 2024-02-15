
#pragma once

void __stdcall SetRadarColor(int nIndex,DWORD dwColor);

PED_TYPE * __stdcall GamePool_FindPlayerPed();

DWORD __stdcall TranslateColorCodeToRGBA(int iCode);

void GameResetRadarColors();

BOOL IsFileOrDirectoryExists(char * szPath);
