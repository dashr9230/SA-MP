
#pragma once

typedef struct _LABEL
{
	char *szText;
	char _gap4[25];
} LABEL;

class CLabelPool
{
private:
	LABEL m_Labels[MAX_LABELS];
	BOOL m_bLabelSlotState[MAX_LABELS];
public:
	CLabelPool();

	BOOL Delete(WORD wLabelID);

};
