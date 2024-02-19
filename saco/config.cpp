
#include "config.h"

CConfig::CConfig(char* a2)
{
	for(int i = 0; i != MAX_CONFIG_ENTRIES; i++) {
		memset(&field_0[i], 0, sizeof(UNNAMED_STRUCT_1));
		field_7A00[i] = 0;
	}

	field_8305 = 0;

	if(a2 && strlen(a2)) {
		strcpy(field_8200, a2);

		ReadFile();
	}
}

void CConfig::sub_10066270()
{
	// TODO: CConfig::sub_10066270 10066270
}

BOOL CConfig::ReadFile()
{
	// TODO: CConfig::sub_100661E0 100661E0
}

void CConfig::sub_10066180()
{
	// TODO: CConfig::sub_10066180 10066180
}

void CConfig::sub_100660E0()
{
	// TODO: CConfig::sub_100660E0 100660E0
}

void CConfig::sub_10065FD0()
{
	// TODO: CConfig::sub_10065FD0 10065FD0
}

void CConfig::sub_10065F30()
{
	// TODO: CConfig::sub_10065F30 10065F30 (unused)
}

void CConfig::sub_10065C90()
{
	// TODO: CConfig::sub_10065C90 10065C90
}

void CConfig::sub_10065D30()
{
	// TODO: CConfig::sub_10065D30 10065D30
}

void CConfig::sub_10065D50()
{
	// TODO: CConfig::sub_10065D50 10065D50
}

void CConfig::sub_10065C40()
{
	// TODO: CConfig::sub_10065C40 10065C40
}

void CConfig::sub_10065E10()
{
	// TODO: CConfig::sub_10065E10 10065E10
}

void CConfig::sub_10065C00()
{
	// TODO: CConfig::sub_10065C00 10065C00
}

void CConfig::sub_10065E40()
{
	// TODO: CConfig::sub_10065E40 10065E40
}

void CConfig::sub_10065E70()
{
	// TODO: CConfig::sub_10065E70 10065E70 (unused)
}

void CConfig::sub_10065EA0()
{
	// TODO: CConfig::sub_10065EA0 10065EA0 (unused)
}

void CConfig::sub_10065F00()
{
	// TODO: CConfig::sub_10065F00 10065F00 (unused)
}

void CConfig::sub_10066080()
{
	// TODO: CConfig::sub_10066080 10066080
}

