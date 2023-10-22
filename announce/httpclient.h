
//----------------------------------------------------

#pragma pack(1)

class CHttpClient
{
private:
	int field_0;
	char field_4[17926];
	char field_460A[65040];
	int field_1441A;
	char field_1441E[256];
	int field_1451E;

public:
	CHttpClient(char *src);
	~CHttpClient();
	int ProcessURL(int, int, char *src, char *, char *);
	void GetHeaderValue();
	int Connect(char *name, unsigned short hostshort, char *);
	void CloseConnection();
	int Send(int, char *s);
	int Recv(int, void *buf, size_t n);
	int InitRequest(int, int, char *src, char *, char *);
	void Process();
	void HandleEntity();
};

//----------------------------------------------------
