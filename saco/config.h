
#pragma once

#define MAX_CONFIG_ENTRIES 512

typedef struct _UNNAMED_STRUCT_1 {
	char _gap0[61];
} UNNAMED_STRUCT_1;

class CConfig
{
private:
	UNNAMED_STRUCT_1 field_0[MAX_CONFIG_ENTRIES];
	int field_7A00[MAX_CONFIG_ENTRIES];
	char field_8200[MAX_PATH+1];
	int field_8305;

public:

	CConfig(char* a2);
	void sub_10066270();

	BOOL ReadFile();

	void sub_10066180();
	void sub_100660E0();
	void sub_10065FD0();
	void sub_10065F30();
	void sub_10065C90();
	void sub_10065D30();
	void sub_10065D50();
	void sub_10065C40();
	void sub_10065E10();
	void sub_10065C00();
	void sub_10065E40();
	void sub_10065E70();
	void sub_10065EA0();
	void sub_10065F00();
	void sub_10066080();
};
