// Useful sites
// http://www.faqs.org\rfcs\rfc2821.html
// http://en.wikipedia.org/wiki/Base64
// http://www2.rad.com\networks/1995/mime/examples.htm

#include "EmailSender.h"
#include "TCPInterface.h"
#include "GetTime.h"
#include "Rand.h"
#include "FileList.h"
#include <stdio.h>

#if defined(_COMPATIBILITY_1)
#include "Compatibility1Includes.h"
#endif

#include "RakSleep.h"

char *EmailSender::Send(const char *hostAddress, unsigned short hostPort, const char *sender, const char *recipient, const char *senderName, const char *recipientName, const char *subject, const char *body, FileList *attachedFiles, bool doPrintf)
{
	Packet *packet;
	char query[1024];
	char *response;
	TCPInterface tcpInterface;
	PlayerID emailServer;
	if (tcpInterface.Start(0, 0)==false)
		return "Unknown error starting TCP";
	emailServer=tcpInterface.Connect(hostAddress, hostPort);
	if (emailServer==UNASSIGNED_PLAYER_ID)
		return "Failed to connect to host";
	RakNetTime timeoutTime = RakNet::GetTime()+3000;
	packet=0;
	while (RakNet::GetTime() < timeoutTime)
	{
		packet = tcpInterface.Receive();
		if (packet)
		{
			if (doPrintf)
				printf("%s", packet->data);
			break;
		}
		RakSleep(250);
	}

	if (packet==0)
		return "Timeout while waiting for initial data from server.";
	
	tcpInterface.Send("HELO\r\n", 6, emailServer);
	
	response=GetResponse(&tcpInterface, emailServer, doPrintf);
	if (response!=0)
		return response;

	if (sender)
		sprintf(query, "MAIL From: <%s>\r\n", sender);
	else
		sprintf(query, "MAIL From: <>\r\n");
	tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	response=GetResponse(&tcpInterface, emailServer, doPrintf);
	if (response!=0)
		return response;

	if (recipient)
		sprintf(query, "RCPT TO: <%s>\r\n", recipient);
	else
		sprintf(query, "RCPT TO: <>\r\n");
	tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	response=GetResponse(&tcpInterface, emailServer, doPrintf);
	if (response!=0)
		return response;

	tcpInterface.Send("DATA\r\n", (unsigned int)strlen("DATA\r\n"), emailServer);

	response=GetResponse(&tcpInterface, emailServer, doPrintf);
	if (response!=0)
		return response;

	if (subject)
	{
		sprintf(query, "Subject: %s\r\n", subject);
		tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	}
	if (senderName)
	{
		sprintf(query, "From: %s\r\n", senderName);
		tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	}
	if (recipientName)
	{
		sprintf(query, "To: %s\r\n", recipientName);
		tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	}

	const char base64Map[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const int boundarySize=60;
	char boundary[boundarySize+1];
	int i,j;
	if (attachedFiles && attachedFiles->fileList.Size())
	{
		seedMT(RakNet::GetTime());
		// Random multipart message boundary
		for (i=0; i < boundarySize; i++)
			boundary[i]=base64Map[randomMT()%64];
		boundary[boundarySize]=0;
	}

	sprintf(query, "MIME-version: 1.0\r\n");
	tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);

	if (attachedFiles && attachedFiles->fileList.Size())
	{
		sprintf(query, "Content-type: multipart/mixed; BOUNDARY=\"%s\"\r\n\r\n", boundary);
		tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);

		sprintf(query, "This is a multi-part message in MIME format.\r\n\r\n--%s\r\n", boundary);
		tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	}
	
	sprintf(query, "Content-Type: text/plain; charset=\"US-ASCII\"\r\n\r\n");
	tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);

	// Write the body of the email, doing some lame shitty shit where I have to make periods at the start of a newline have a second period.
	char *newBody;
	int bodyLength;
	bodyLength=(int)strlen(body);
	newBody = new char [bodyLength*3];
	if (bodyLength>0)
		newBody[0]=body[0];
	for (i=1, j=1; i < bodyLength; i++)
	{
		// Transform \n . \r \n into \n . . \r \n
		if (i < bodyLength-2 &&
			body[i-1]=='\n' &&
			body[i+0]=='.' &&
			body[i+1]=='\r' &&
			body[i+2]=='\n')
		{
			newBody[j++]='.';
			newBody[j++]='.';
			newBody[j++]='\r';
			newBody[j++]='\n';
			i+=2;
		}
		// Transform \n . . \r \n into \n . . . \r \n
		// Having to process .. is a bug in the mail server - the spec says ONLY \r\n.\r\n should be transformed
		else if (i <= bodyLength-3 &&
			body[i-1]=='\n' &&
			body[i+0]=='.' &&
			body[i+1]=='.' &&
			body[i+2]=='\r' &&
			body[i+3]=='\n')
		{
			newBody[j++]='.';
			newBody[j++]='.';
			newBody[j++]='.';
			newBody[j++]='\r';
			newBody[j++]='\n';
			i+=3;
		}
		// Transform \n . \n into \n . . \r \n (this is a bug in the mail server - the spec says do not count \n alone but it does)
		else if (i < bodyLength-1 &&
			body[i-1]=='\n' &&
			body[i+0]=='.' &&
			body[i+1]=='\n')
		{
			newBody[j++]='.';
			newBody[j++]='.';
			newBody[j++]='\r';
			newBody[j++]='\n';
			i+=1;
		}
		// Transform \n . . \n into \n . . . \r \n (this is a bug in the mail server - the spec says do not count \n alone but it does)
		// In fact having to process .. is a bug too - because the spec says ONLY \r\n.\r\n should be transformed
		else if (i <= bodyLength-2 &&
			body[i-1]=='\n' &&
			body[i+0]=='.' &&
			body[i+1]=='.' &&
			body[i+2]=='\n')
		{
			newBody[j++]='.';
			newBody[j++]='.';
			newBody[j++]='.';
			newBody[j++]='\r';
			newBody[j++]='\n';
			i+=2;
		}
		else
			newBody[j++]=body[i];
	}
	
	newBody[j++]='\r';
	newBody[j++]='\n';
	tcpInterface.Send(newBody, j, emailServer);

	delete [] newBody;
	int outputOffset;

	// What a pain in the rear.  I have to map the binary to printable characters using 6 bits per character.
	if (attachedFiles && attachedFiles->fileList.Size())
	{
		for (i=0; i < (int) attachedFiles->fileList.Size(); i++)
		{
			// Write boundary
			sprintf(query, "\r\n--%s\r\n", boundary);
			tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);

			sprintf(query, "Content-Type: APPLICATION/Octet-Stream; SizeOnDisk=%i; name=\"%s\"\r\nContent-Transfer-Encoding: BASE64\r\nContent-Description: %s\r\n\r\n", attachedFiles->fileList[i].dataLength, attachedFiles->fileList[i].filename, attachedFiles->fileList[i].filename);
			tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);

			newBody = new char[(attachedFiles->fileList[i].dataLength*3)/2];

			outputOffset=Base64Encoding(attachedFiles->fileList[i].data, attachedFiles->fileList[i].dataLength, newBody, base64Map);

			// Send the base64 mapped file.
			tcpInterface.Send(newBody, outputOffset, emailServer);
			delete [] newBody;

		}

		// Write last boundary
		sprintf(query, "\r\n--%s--\r\n", boundary);
		tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	}


	sprintf(query, "\r\n.\r\n");
	tcpInterface.Send(query, (unsigned int)strlen(query), emailServer);
	response=GetResponse(&tcpInterface, emailServer, doPrintf);
	if (response!=0)
		return response;

	tcpInterface.Send("QUIT\r\n", (unsigned int)strlen("QUIT\r\n"), emailServer);

	RakSleep(30);
	if (doPrintf)
	{
		packet = tcpInterface.Receive();
		while (packet)
		{
			printf("%s", packet->data);
			packet = tcpInterface.Receive();
		}
	}
	tcpInterface.Stop();
	return 0; // Success
}

char *EmailSender::GetResponse(TCPInterface *tcpInterface, const PlayerID &emailServer, bool doPrintf)
{
	Packet *packet;
	RakNetTime timeout;
	timeout=RakNet::GetTime()+5000;
	while (1)
	{
		if (tcpInterface->HasLostConnection()==emailServer)
			return "Connection to server lost.";
		packet = tcpInterface->Receive();
		if (packet)
		{
			if (doPrintf)
				printf("%s", packet->data);
			if (strstr((const char*)packet->data, "250"))
				return 0; // OK
			if (strstr((const char*)packet->data, "354"))
				return 0; // OK
			if (strstr((const char*)packet->data, "550"))
				return "Failed on error code 550";
			if (strstr((const char*)packet->data, "553"))
				return "Failed on error code 553";
		}
		if (RakNet::GetTime() > timeout)
			return "Timed out";
		RakSleep(100);
	}
}

int EmailSender::Base64Encoding(const char *inputData, int dataLength, char *outputData, const char *base64Map)
{
	int outputOffset, charCount;
	int write3Count;
	outputOffset=0;
	charCount=0;
	int j;

	write3Count=dataLength/3;
	for (j=0; j < write3Count; j++)
	{
		// 6 leftmost bits from first byte, shifted to bits 7,8 are 0
		outputData[outputOffset++]=base64Map[inputData[j*3+0] >> 2];
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}

		// Remaining 2 bits from first byte, placed in position, and 4 high bits from the second byte, masked to ignore bits 7,8
		outputData[outputOffset++]=base64Map[((inputData[j*3+0] << 4) | (inputData[j*3+1] >> 4)) & 63];
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}

		// 4 low bits from the second byte and the two high bits from the third byte, masked to ignore bits 7,8
		outputData[outputOffset++]=base64Map[((inputData[j*3+1] << 2) | (inputData[j*3+2] >> 6)) & 63]; // Third 6 bits
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}

		// Last 6 bits from the third byte, masked to ignore bits 7,8
		outputData[outputOffset++]=base64Map[inputData[j*3+2] & 63];
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}
	}

	if (dataLength % 3==1)
	{
		// One input byte remaining
		outputData[outputOffset++]=base64Map[inputData[j*3+0] >> 2];
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}
	
		// Pad with two equals
		outputData[outputOffset++]='=';
		outputData[outputOffset++]='=';
	}
	else if (dataLength % 3==2)
	{
		// Two input bytes remaining

		// 6 leftmost bits from first byte, shifted to bits 7,8 are 0
		outputData[outputOffset++]=base64Map[inputData[j*3+0] >> 2];
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}

		// Remaining 2 bits from first byte, placed in position, and 4 high bits from the second byte, masked to ignore bits 7,8
		outputData[outputOffset++]=base64Map[((inputData[j*3+0] << 4) | (inputData[j*3+1] >> 4)) & 63];
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}

		// 4 low bits from the second byte, followed by 00
		outputData[outputOffset++]=base64Map[(inputData[j*3+1] << 2) & 63]; // Third 6 bits
		if ((++charCount % 76)==0) {outputData[outputOffset++]='\r'; outputData[outputOffset++]='\n'; charCount=0;}

		// Pad with one equal
		outputData[outputOffset++]='=';
		//outputData[outputOffset++]='=';
	}

	// Append \r\n
	outputData[outputOffset++]='\r';
	outputData[outputOffset++]='\n';
	outputData[outputOffset]=0;

	return outputOffset;
}
