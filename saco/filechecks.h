
typedef struct _BIN_TREE
{
	int dwFileHash;
	BYTE byteLeft;
	BYTE byteRight;
	BYTE byteData;
	BYTE bytePadding;
} BINARY_TREE;

typedef struct _MD5_DATA
{
	BYTE byteData[16];
} MD5_DATA;

BOOL IsCheckableFile(CHAR* szExt);
void CheckFileHash(int dwFileHash, HANDLE hFile);
int GetFileNameHash(char* szFileName);
