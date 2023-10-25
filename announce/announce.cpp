
#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "httpclient.h"

#ifdef WIN32

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	if(!strlen(lpszCmdLine) || strlen(lpszCmdLine) > 5) return 0;

	CHttpClient* pHttpClient = new CHttpClient(NULL);

	//OutputDebugString(lpszCmdLine);

	char szURL[1025];
	memset(szURL,0,sizeof(szURL));
	sprintf(szURL, "server.sa-mp.com/0.3.7/announce/%s",lpszCmdLine);

	pHttpClient->ProcessURL(HTTP_GET, szURL, NULL, "Bonus");

	delete pHttpClient;

	ExitProcess(0);

	return 0;
}

#else

int main(int argc, char *argv[])
{
	char *szBindAddress = NULL;

	if((argc <= 1 || argc > 3) || strlen(argv[1]) > 5) return 0;

	if(argc == 3) {
		szBindAddress = argv[2];
	}

	CHttpClient* pHttpClient = new CHttpClient(szBindAddress);

	char szURL[255];
	sprintf(szURL, "server.sa-mp.com/0.3.7/announce/%s",argv[1]);

	pHttpClient->ProcessURL(HTTP_GET, szURL, NULL, "Bonus");

	delete pHttpClient;

	return 0;
}

#endif