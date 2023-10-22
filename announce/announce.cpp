
#ifdef WIN32
#include <windows.h>
#endif

#include "httpclient.h"

#ifdef WIN32

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	if(!strlen(lpszCmdLine) || strlen(lpszCmdLine) > 5) return 0;

	CHttpClient* pHttpClient = new CHttpClient(NULL);

	// TODO: WinMain

	delete pHttpClient;

	ExitProcess(0);

	return 0;
}

#else

int main(int argc, char *argv[])
{
	char *src = NULL;

	if((argc <= 1 || argc > 3) || strlen(argv[1]) > 5) return 0;

	if(argc == 3) {
		src = argv[2];
	}

	CHttpClient* pHttpClient = new CHttpClient(src);

	// TODO: main

	delete pHttpClient;

	return 0;
}

#endif