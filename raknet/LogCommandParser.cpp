#include "LogCommandParser.h"
#include "TransportInterface.h"
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if (defined(__GNUC__)  || defined(__GCCXML__))
#define _vsnprintf vsnprintf
#define _stricmp strcasecmp
#endif


#ifdef _MSC_VER
#pragma warning( push )
#endif

LogCommandParser::LogCommandParser()
{
	RegisterCommand(CommandParserInterface::VARIABLE_NUMBER_OF_PARAMETERS,"Subscribe","[<ChannelName>] - Subscribes to a named channel, or all channels");
	RegisterCommand(CommandParserInterface::VARIABLE_NUMBER_OF_PARAMETERS,"Unsubscribe","[<ChannelName>] - Unsubscribes from a named channel, or all channels");
	memset(channelNames,0,sizeof(channelNames));
}
LogCommandParser::~LogCommandParser()
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
bool LogCommandParser::OnCommand(const char *command, unsigned numParameters, char **parameterList, TransportInterface *transport, PlayerID playerId, const char *originalString)
{
	if (strcmp(command, "Subscribe")==0)
	{
		unsigned channelIndex;
		if (numParameters==0)
		{
			Subscribe(playerId, 0);
			transport->Send(playerId, "Subscribed to all channels.\r\n");
		}
		else if (numParameters==1)
		{
			if ((channelIndex=Subscribe(playerId, parameterList[0]))!=(unsigned)-1)
			{
				transport->Send(playerId, "You are now subscribed to channel %s.\r\n", channelNames[channelIndex]);
			}
			else
			{
				transport->Send(playerId, "Cannot find channel %s.\r\n", parameterList[0]);
				PrintChannels(playerId, transport);
			}
		}
		else
		{
			transport->Send(playerId, "Subscribe takes either 0 or 1 parameters.\r\n");
		}
	}
	else if (strcmp(command, "Unsubscribe")==0)
	{
		unsigned channelIndex;
		if (numParameters==0)
		{
			Unsubscribe(playerId, 0);
			transport->Send(playerId, "Unsubscribed from all channels.\r\n");
		}
		else if (numParameters==1)
		{
			if ((channelIndex=Unsubscribe(playerId, parameterList[0]))!=(unsigned)-1)
			{
				transport->Send(playerId, "You are now unsubscribed from channel %s.\r\n", channelNames[channelIndex]);
			}
			else
			{
				transport->Send(playerId, "Cannot find channel %s.\r\n", parameterList[0]);
				PrintChannels(playerId, transport);
			}
		}
		else
		{
			transport->Send(playerId, "Unsubscribe takes either 0 or 1 parameters.\r\n");
		}
	}

	return true;
}
char *LogCommandParser::GetName(void) const
{
	return "Logger";
}
void LogCommandParser::SendHelp(TransportInterface *transport, PlayerID playerId)
{
	transport->Send(playerId, "The logger will accept user log data via the Log(...) function.\r\n");
	transport->Send(playerId, "Each log is associated with a named channel.\r\n");
	transport->Send(playerId, "You can subscribe to or unsubscribe from named channels.\r\n");
	PrintChannels(playerId, transport);
}
void LogCommandParser::AddChannel(const char *channelName)
{
	unsigned channelIndex;
	channelIndex = GetChannelIndexFromName(channelName);
	// Each channel can only be added once.
	assert(channelIndex==(unsigned)-1);

	unsigned i;
	for (i=0; i < 32; i++)
	{
		if (channelNames[i]==0)
		{
			// Assuming a persistent static string.
			channelNames[i]=channelName;
			return;
		}
	}

	// No more available channels - max 32 with this implementation where I save subscribed channels with bit operations
	assert(0);
}
void LogCommandParser::WriteLog(const char *channelName, const char *format, ...)
{
	if (channelName==0 || format==0)
		return;

	unsigned channelIndex;
	channelIndex = GetChannelIndexFromName(channelName);
	if (channelIndex==(unsigned)-1)
	{
		AddChannel(channelName);
	}

	char text[REMOTE_MAX_TEXT_INPUT];
	va_list ap;
	va_start(ap, format);
	_vsnprintf(text, REMOTE_MAX_TEXT_INPUT, format, ap);
	va_end(ap);
	text[REMOTE_MAX_TEXT_INPUT-1]=0;

	// Make sure that text ends in \r\n
	int textLen;
	textLen=(int)strlen(text);
	if (textLen==0)
		return;
	if (text[textLen-1]=='\n')
	{
		text[textLen-1]=0;
	}
	if (textLen < REMOTE_MAX_TEXT_INPUT-4)
		strcat(text, "\r\n");
	else
	{
		text[textLen-3]='\r';
		text[textLen-2]='\n';
		text[textLen-1]=0;
	}

	// For each user that subscribes to this channel, send to them.
	unsigned i;
	for (i=0; i < remoteUsers.Size(); i++)
	{
		if (remoteUsers[i].channels & (1 << channelIndex))
		{
			trans->Send(remoteUsers[i].playerId, text);
		}
	}
}
void LogCommandParser::PrintChannels(PlayerID playerId, TransportInterface *transport) const
{
	unsigned i;
	bool anyChannels=false;
	transport->Send(playerId, "CHANNELS:\r\n");
	for (i=0; i < 32; i++)
	{
		if (channelNames[i])
		{
			transport->Send(playerId, "%i. %s\r\n", i+1,channelNames[i]);
			anyChannels=true;
		}
	}
	if (anyChannels==false)
		transport->Send(playerId, "None.\r\n");
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void LogCommandParser::OnNewIncomingConnection(PlayerID playerId, TransportInterface *transport)
{
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void LogCommandParser::OnConnectionLost(PlayerID playerId, TransportInterface *transport)
{
	Unsubscribe(playerId, 0);
}
unsigned LogCommandParser::Unsubscribe(PlayerID playerId, const char *channelName)
{
	unsigned i;
	for (i=0; i < remoteUsers.Size(); i++)
	{
		if (remoteUsers[i].playerId==playerId)
		{
			if (channelName==0)
			{
				// Unsubscribe from all and delete this user.
				remoteUsers[i]=remoteUsers[remoteUsers.Size()-1];
				remoteUsers.Del();
				return 0;
			}
			else
			{
				unsigned channelIndex;
				channelIndex = GetChannelIndexFromName(channelName);
				if (channelIndex!=(unsigned)-1)
				{
					remoteUsers[i].channels&=0xFFFF ^ (1<<channelIndex); // Unset this bit
				}
				return channelIndex;
			}
		}
	}
	return (unsigned)-1;
}
unsigned LogCommandParser::Subscribe(PlayerID playerId, const char *channelName)
{
	unsigned i;
	unsigned channelIndex=(unsigned)-1;
	if (channelName)
	{
		channelIndex = GetChannelIndexFromName(channelName);
		if (channelIndex==(unsigned)-1)
			return channelIndex;
	}

	for (i=0; i < remoteUsers.Size(); i++)
	{
		if (remoteUsers[i].playerId==playerId)
		{
			if (channelName)
				remoteUsers[i].channels|=1<<channelIndex; // Set this bit for an existing user
			else
				remoteUsers[i].channels=0xFFFF;
			return channelIndex;
		}
	}

	// Make a new user
	PlayerIDAndChannel newUser;
	newUser.playerId = playerId;
	if (channelName)
		newUser.channels=1<<channelIndex;
	else
		newUser.channels=0xFFFF;
	remoteUsers.Insert(newUser);
	return channelIndex;
}
unsigned LogCommandParser::GetChannelIndexFromName(const char *channelName)
{
	unsigned i;
	for (i=0; i < 32; i++)
	{
		if (channelNames[i]==0)
			return (unsigned) -1;

		if (_stricmp(channelNames[i], channelName)==0)
			return i;
	}
	return (unsigned)-1;
}

void LogCommandParser::OnTransportChange(TransportInterface *transport)
{
	// I don't want users to have to pass TransportInterface *transport to Log.
	trans=transport;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
