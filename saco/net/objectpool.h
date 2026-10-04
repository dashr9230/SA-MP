
#pragma once

// samp object; holds the material/text data
class CObject
{
public:
	char _gap0[68];
	int field_44;
	char _gap48[11];
	BYTE field_53;
	float field_54;
	char _gap58[4403];
	int field_118B;
	char _gap118F[10];
	BYTE field_1199;

public:
	void FUNC_100A7E20();
	void FUNC_100A9E30();
	void FUNC_100A9E70();
	void FUNC_100A80A0(VECTOR *pPos); // .text:100A80A0
	void FUNC_100A8030(); // .text:100A8030
	void FUNC_100A8530(int iModel, int iIndex, char *szTxd, char *szTexture, DWORD dwColour); // .text:100A8530
	void FUNC_100A86B0(int iIndex, char *szText, BYTE byteSize, char *szFont, BYTE byteBold,
		BYTE byteAlign, DWORD dwFontColour, DWORD dwBackColour, BYTE byteUnk); // .text:100A86B0
};

class CObjectPool
{
public:
	int field_0;
	int field_4[MAX_OBJECTS];
	int field_FA4[MAX_OBJECTS];
public:
	char FUNC_10012DE0(VECTOR *vecStart, VECTOR *vecEnd); // .text:10012DE0

	CObjectPool();
	~CObjectPool(); // .text:10013160

	void FUNC_10012850(WORD wObject); // .text:10012850

	void FUNC_10012A50(); // .text:10012A50
	void FUNC_10012B10(); // .text:10012B10
	void FUNC_10012B90(); // .text:10012B90
	void FUNC_10012BD0(); // .text:10012BD0

	CObject * FUNC_100129D0(ENTITY_TYPE *pEntity); // todo: implement
};
