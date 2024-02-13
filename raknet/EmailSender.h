// TODO: Implement EmailSender.h

#ifndef __EMAIL_SENDER_H
#define __EMAIL_SENDER_H

class TCPInterface;
#include "NetworkTypes.h"

class EmailSender
{
public:
protected:
	char *GetResponse(TCPInterface *tcpInterface, const PlayerID &emailServer, bool doPrintf);
};

#endif
