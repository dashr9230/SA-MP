
#pragma once

//----------------------------------------------------

class CTextDraw
{
public:
	~CTextDraw(); // .text:100B2F50

	void FUNC_100B3480(); // .text:100B3480
	void FUNC_100B34A0(); // .text:100B34A0
	void FUNC_100B36B0(); // .text:100B36B0
	void FUNC_100B2F60(char *szText); // .text:100B2F60
};

//----------------------------------------------------

class CTextDrawPool
{
private:
	int field_0[MAX_TEXT_DRAWS];
	int field_2400[MAX_TEXT_DRAWS];
public:
	int GetAt(unsigned short TextDrawID)
	{
		if(TextDrawID >= MAX_TEXT_DRAWS) return 0;
		if(!field_0[TextDrawID]) return 0;
		return field_2400[TextDrawID];
	}

	CTextDrawPool();

	void FUNC_1001E7F0(unsigned short TextDrawID); // .text:1001E7F0

	void FUNC_1001E830(); // .text:1001E830

	void FUNC_1001E870(); // .text:1001E870

	~CTextDrawPool(); // .text:1001E8D0

	void FUNC_1001E8A0(); // .text:1001E8A0

	// creates the slot's CTextDraw, replacing any existing one; returns it or 0
	int FUNC_1001E910(unsigned short TextDrawID, char *pTransmit, char *szText); // .text:1001E910
};

//----------------------------------------------------
