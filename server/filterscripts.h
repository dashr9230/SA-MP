
#ifndef SAMPSRV_FILTERSCRIPTS_H
#define SAMPSRV_FILTERSCRIPTS_H

class CFilterScripts // size: WL 4148
{
private:

	int field_0[16];
	char gap40[4080];
	int field_1030;

public:
	CFilterScripts();
	~CFilterScripts();

	bool LoadFilterScript(char* pFileName);

	void RemoveFilterScript(int iIndex);
	void UnloadFilterScripts();

};

#endif
