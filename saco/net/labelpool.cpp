
#include "../main.h"

CLabelPool::CLabelPool()
{
	for(WORD wLabelID = 0; wLabelID < MAX_LABELS; wLabelID++)
	{
		memset(&m_Labels[wLabelID], 0, sizeof(LABEL));
		m_bLabelSlotState[wLabelID] = FALSE;
	}
}

BOOL CLabelPool::Delete(WORD wLabelID)
{
	if(wLabelID >= MAX_LABELS) return FALSE;
	if(!m_bLabelSlotState[wLabelID]) return FALSE;

	if(m_Labels[wLabelID].szText)
		free(m_Labels[wLabelID].szText);
	memset(&m_Labels[wLabelID], 0, sizeof(LABEL));
	m_bLabelSlotState[wLabelID] = FALSE;

	return TRUE;
}
