
#include "main.h"

BOOL	bRconSocketReply	= FALSE;

SOCKET	cur_sock			= INVALID_SOCKET;
char*	cur_data			= NULL;
int		cur_datalen			= 0;
sockaddr_in to;

void RconSocketReply(char* szMessage)
{
	if (bRconSocketReply)
	{
		char* newdata = (char*)malloc(cur_datalen + strlen(szMessage) + sizeof(WORD));
		char* keep_ptr = newdata;
		memcpy(newdata, cur_data, cur_datalen);
		newdata += cur_datalen;
		*(WORD*)newdata = (WORD)strlen(szMessage);
		newdata += sizeof(WORD);
		memcpy(newdata, szMessage, strlen(szMessage));
		newdata += strlen(szMessage);
		sendto(cur_sock, keep_ptr, (int)(newdata - keep_ptr), 0, (sockaddr*)&to, sizeof(to));
		free(keep_ptr);
	}
}
