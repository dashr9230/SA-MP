
#ifndef SAMPSRV_TEXTDRAWPOOL_H
#define SAMPSRV_TEXTDRAWPOOL_H

class CTextDrawPool // size: WL 2072576
{
private:

	int field_0[2048];
	void* field_2000[2048];
	void* field_4000[2048];
	char gap6000[2048000];

public:
	CTextDrawPool();
	~CTextDrawPool();
};

#endif
