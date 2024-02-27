
#pragma once

//-----------------------------------------------------------

int FindTextureSlot(char *szName);
int AddTextureSlot(char *szName);
int LoadTexture(int iIndex, char *szName);
void AddTextureRef(int iIndex);
void RemoveTextureRef(int iIndex);
void PushCurrentTexture();
void PopCurrentTexture();
void SetCurrentTexture(int iIndex);
DWORD ReloadTexture(char *szName);
void DestroyTexture(DWORD dwTexture);
void RemoveTextureSlotByName(char *szName);
void RemoveTexture(int iIndex);
void RemoveTextureSlot(int iIndex);
int GetTextureNumRefs(int iIndex);

//-----------------------------------------------------------
