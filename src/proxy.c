#include "proxy.h"
#include "http_request.h"
#include "socket.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>

void retrieveHostResponse(char* host, char* command, int bufferSize, int clientSocket)
{
	static char buffer[BUFFER_SIZE];

	char* hostName = getHostName(host);
	char port[6];
	sprintf(port, "%d", getPort(host));
	int hostSocket = getServerSocket(hostName, port);

	if (hostSocket == 0)
	{
		fprintf(stderr, "[ERROR] Connection - %s:%s\n", hostName, port);
		exit(1);
	}
	else
	{
		struct timeval tv = {5, 0};
		setsockopt(hostSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));

		send(hostSocket, command, bufferSize, 0);

		while (1)
		{
			int n = recv(hostSocket, buffer, BUFFER_SIZE, 0);
			if (n <= 0)
				break;

			send(clientSocket, buffer, n, 0);

			/*strncat(buffer, buffer, n);
			printf("=========================\n%s\n", buffer);*/
		}
	}

	shutdown(hostSocket, SHUT_RDWR);
	close(hostSocket);
}
