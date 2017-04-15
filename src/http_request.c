#include "http_request.h"
#include "string.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


char* getHostFromGetCommand(char* command)
{
	static char host[HOST_NAME_MAX];

	int propertyValuePosition = getPropertyValuePositionInString(command, "Host");
	char* hostPointer = getEndOfStringLine(command, propertyValuePosition);
	strcpy(host, hostPointer);
	free(hostPointer);

	return host;
}

int getPort(char* host)
{
	char port[6];
	int portStringIdToComplete = -1;
	int i = 0;

	for (i = 0; i < strlen(host); ++i)
	{
		if (portStringIdToComplete >= 0)
			port[portStringIdToComplete] = host[i];

		if (host[i] == ':' || portStringIdToComplete >= 0)
			++portStringIdToComplete;
	}

	port[portStringIdToComplete + 1] = '\0';

	if (portStringIdToComplete >= 0)
		return atoi(port);
	return 80;
}

char* getHostName(char* host)
{
	static char hostName[HOST_NAME_MAX];
	int separatorPosition = -1;
	int i = 0;

	for (i = 0; i < strlen(host) && separatorPosition == -1; ++i)
	{
		if (host[i] == ':')
			separatorPosition = i;
	}

	strcpy(hostName, host);
	if (separatorPosition != -1)
		hostName[separatorPosition] = '\0';

	return hostName;
}
