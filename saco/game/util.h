
#pragma once

void __stdcall SetRadarColor(int nIndex,DWORD dwColor);

PED_TYPE * __stdcall GamePool_FindPlayerPed();

DWORD __stdcall TranslateColorCodeToRGBA(int iCode);
void __stdcall InitPlayerPedPtrRecords();
void __stdcall SetPlayerPedPtrRecord(BYTE bytePlayer, DWORD dwPedPtr);
DWORD __stdcall GetPlayerPedPtrRecord(BYTE bytePlayer);
BYTE __stdcall FindPlayerNumFromPedPtr(DWORD dwPedPtr);

void GameResetRadarColors();

BOOL IsFileOrDirectoryExists(char * szPath);

DWORD unnamed_100B6100(char *szString, int nMaxLen);
