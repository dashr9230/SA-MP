/// \file
/// \brief Rudimentary class to send email from code.  Don't expect anything fancy.
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.rakkarsoft.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __EMAIL_SENDER_H
#define __EMAIL_SENDER_H

class FileList;
class TCPInterface;
#include "NetworkTypes.h"

/// \brief Rudimentary class to send email from code.
class EmailSender
{
public:
	/// Sends an email
	/// \param[in] hostAddress The address of the email server.
	/// \param[in] hostPort The port of the email server (usually 25)
	/// \param[in] sender The email address you are sending from.
	/// \param[in] recipient The email address you are sending to.
	/// \param[in] senderName The email address you claim to be sending from
	/// \param[in] recipientName The email address you claim to be sending to
	/// \param[in] subject Email subject
	/// \param[in] body Email body
	/// \param[in] attachedFiles List of files to attach to the email. (Can be 0 to send none).
	/// \param[in] doPrintf true to output SMTP info to console(for debugging?)
	/// \return 0 on success, otherwise a string indicating the error message
	char *Send(const char *hostAddress, unsigned short hostPort, const char *sender, const char *recipient, const char *senderName, const char *recipientName, const char *subject, const char *body, FileList *attachedFiles, bool doPrintf);

	// Returns how many bytes were written
	int Base64Encoding(const char *inputData, int dataLength, char *outputData, const char *base64Map);
protected:
	char *GetResponse(TCPInterface *tcpInterface, const PlayerID &emailServer, bool doPrintf);
};

#endif
