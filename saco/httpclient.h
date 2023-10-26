
//----------------------------------------------------

class CHttpClient
{
private:
	int field_0;
	char _gap4[22];
	char _gap1A[24];
	int field_32;

public:

	CHttpClient();
	~CHttpClient();
	int ProcessURL(int a2, char *a3, char *a4, char *a5);
	void InitRequest(int a2, char *a3, char *a4, char *a5);
	void Process();
	char Connect(char *name, u_short hostshort);
	char HandleEntity();
	int Recv(char *buf, int len);
	char Send(char *buf);
	int CloseConnection();
	char GetHeaderValue(char *a2, char *a3, signed int a4);
};

//----------------------------------------------------
