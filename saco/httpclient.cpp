
#include <windows.h>
#include <stdio.h>
#include "httpclient.h"

//----------------------------------------------------

CHttpClient::CHttpClient()
{
	// TODO: CHttpClient::CHttpClient() 100022E0
}

//----------------------------------------------------

CHttpClient::~CHttpClient()
{
	// TODO: CHttpClient::~CHttpClient() 10002340
}

//----------------------------------------------------

int CHttpClient::ProcessURL(int a2, char *a3, char *a4, char *a5)
{
	// TODO: CHttpClient::ProcessURL 10002C40

	printf("CHttpClient::ProcessURL %s\n", a3); // Thanks Kalcor!

	return 1;
}

void CHttpClient::InitRequest(int a2, char *a3, char *a4, char *a5)
{
	// TODO: CHttpClient::InitRequest 100024B0
}

void CHttpClient::Process()
{
	// TODO: CHttpClient::Process() 10002A60
}

char CHttpClient::Connect(char *name, u_short hostshort)
{
	// TODO: CHttpClient::Connect 100029A0
	return 0;
}

char CHttpClient::HandleEntity()
{
	// TODO: CHttpClient::HandleEntity() 10002680
	return 0;
}

int CHttpClient::Recv(char *buf, int len)
{
	// TODO: CHttpClient::Recv 10002490
	return 0;
}

char CHttpClient::Send(char *buf)
{
	// TODO: CHttpClient::Send 10002450
	return 0;
}

int CHttpClient::CloseConnection()
{
	// TODO: CHttpClient::CloseConnection() 10002440
	return 0;
}

char CHttpClient::GetHeaderValue(char *a2, char *a3, signed int a4)
{
	// TODO: CHttpClient::GetHeaderValue 100023B0
	return 0;
}
