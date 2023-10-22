
#pragma once

//----------------------------------------------------

class CNetGame // size: 910 bytes
{
private:
	int field_0; // highly m_pRakClient
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	char _gap18[6];
	char field_1E; // m_byteWorldTime ?
	char field_1F;
	char field_20;
	char _gap21[16];
	char field_31;
	char _gap32[2];
	int field_34;
	int field_38;
	char field_3C;
	char field_3D;
	char field_3E;
	char _gap3F;
	char field_40;
	char _gap41[9];
	int field_4A[100];
	char _gap1DA[4];
	int field_1DE;
	int field_1E2;
	char _gap1E6[12];
	int field_1F2;
	char _gap1F6[4];
	int field_1FA;
	int field_1FE;
	char field_202[256];
	char field_302[128];
	int field_382;
	int field_386;
	int field_38A;

public:

	CNetGame();
	~CNetGame();

	void Init(char *a2, int a3, char *a4, char *a5, char *a6);
	void Process();

};

//----------------------------------------------------
