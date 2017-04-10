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
		struct timeval timeout = {5, 0};
		setsockopt(hostSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&timeout, sizeof(struct timeval));
		send(hostSocket, command, bufferSize, 0);

		while (1)
		{
			int n = recv(hostSocket, buffer, BUFFER_SIZE, 0);
			if (n <= 0)
				break;

			send(clientSocket, buffer, n, 0);
		}
	}

	shutdown(hostSocket, SHUT_RDWR);
	close(hostSocket);
}

void retrieveHostSslResponse(char* host, char* command, int bufferSize, int clientSocket)
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
		struct timeval timeout = {5, 0};
		fd_set fdset;
		int maxSocket = hostSocket > clientSocket ? hostSocket + 1 : clientSocket + 1;

		while (1)
		{
			FD_ZERO(&fdset);
			FD_SET(clientSocket, &fdset);
			FD_SET(hostSocket, &fdset);

			int error = select(maxSocket, &fdset, NULL, NULL, &timeout);
			if (error <= 0)
				break;

			if (FD_ISSET(clientSocket, &fdset))
			{
				int n = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				if (n <= 0)
					break;

				n = send(hostSocket, buffer, n, 0);
			}
			else if (FD_ISSET(hostSocket, &fdset))
			{
				int n = recv(hostSocket, buffer, BUFFER_SIZE, 0);
				if (n <= 0)
					break;

				n = send(clientSocket, buffer, n, 0);
			}
		}
	}

	shutdown(hostSocket, SHUT_RDWR);
	close(hostSocket);
}
