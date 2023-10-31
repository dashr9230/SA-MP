
typedef struct _GTA_CONTROLSET
{
	WORD wKeys1[24];
	WORD wKeys2[24];
	BYTE bytePadding1[212];
} GTA_CONTROLSET;

//-----------------------------------------------------------

void GameKeyStatesInit();
void GameStoreLocalPlayerKeys();
void GameSetLocalPlayerKeys();
void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET *pGcsKeyStates);
void GameSetRemotePlayerKeys(int iPlayer);
GTA_CONTROLSET *GameGetInternalKeys();
GTA_CONTROLSET *GameGetLocalPlayerKeys();
GTA_CONTROLSET *GameGetPlayerKeys(int iPlayer);
void GameResetPlayerKeys(int iPlayer);
void GameResetInternalKeys();

//-----------------------------------------------------------
