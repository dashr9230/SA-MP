
#pragma once

#define GAMESTATE_WAIT_CONNECT	1

struct struc_41
{
	char field_0;
	int field_1;
	char _gap5[17];
	float field_16;
	char field_1A;
	char _gap1B[4];
	char field_1F;
	char field_20;
	char field_21;
	char field_22;
	int field_23;
	float field_27;
	char _gap2B;
	char field_2C;
	char field_2D;
	char field_2E;
	char field_2F;
	char _gap30[10];
};

class CNetGame // size: 994
{
private:

	RakClientInterface	*m_pRakClient;

	char _gap4[44];
	char m_szHostOrIp[257];
	char m_szHostName[257];
	char field_232;
	char field_233;
	char field_234;
	int m_iPort;
	int field_239;
	char field_23D[400];
	int m_iGameState;
	int field_3D1;
	struc_41 *field_3D5;
	char field_3D9;
	char _gap3DA[8];

public:

	CNetGame(char *a2, int a3, char *a4, char *a5);

	DWORD GetTime();

};
