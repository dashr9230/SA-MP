
#pragma once

typedef struct _LABEL
{
	char *szText;
	char _gap4[25];
} LABEL;

class CLabelPool
{
public:
	LABEL m_Labels[MAX_LABELS];
	BOOL m_bLabelSlotState[MAX_LABELS];
public:
	CLabelPool();
	~CLabelPool();

	BOOL Delete(WORD wLabelID);

	void FUNC_10001350(); // .text:10001350

	// .text:100011D0
	void sub_100011D0(WORD wLabelID, PCHAR szText, DWORD dwColor, float fX, float fY, float fZ,
		float fDrawDistance, BYTE byteShowBehindWalls, WORD wAttachedPlayer, WORD wAttachedVehicle);

};
