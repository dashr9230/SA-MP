
#ifndef SAMPSRV_NETGAME_H
#define SAMPSRV_NETGAME_H

class CNetGame // size: W: 14808 L: 14816
{
private:
  int field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  int field_24;
  int field_28;
  int field_2C;
  int field_30;
  int field_34;
  int field_38;
  int field_3C;
  int field_40;
  int field_44;
  int field_48;
  int field_4C;
  int field_50;
  char gap54[5];
  char field_59;
  char field_5A;
  char field_5B;
  char field_5C;
  char field_5D;
  int field_5E;
  float field_62;
  int field_66;
  char field_6A;
  char field_6B;
  char field_6C;
  float field_6E;
  float field_72;
  char field_76;
  char field_77;
  char gap78[1];
  char field_79;
  float field_7A;
  int field_7E;
#ifndef WIN32
  double field_82;
#endif
  int field_8A;
  char gap8A[14674];

public:
	CNetGame();
	~CNetGame();

	void Init(BOOL bFirst);
	void ShutdownForGameModeRestart();
	void Process();
};

#endif