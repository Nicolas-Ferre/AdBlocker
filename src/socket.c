#include "socket.h"
#include "constants.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>

struct sockaddr_in getAddress()
{
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_port = htons(PORT);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	return address;
}

int createListenerSocket()
{
	int listenerSocket;
	if ((listenerSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("[ERROR] Listener socket creation\n");
		return 0;
	}

	struct sockaddr_in address = getAddress();
	bind(listenerSocket, (struct sockaddr*)&address, sizeof(address));
	listen(listenerSocket, 5);

	return listenerSocket;
}

int getClientSocket(int listenerSocket)
{
	socklen_t addressSize = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddress;
	return accept(listenerSocket, (struct sockaddr*)&clientAddress, &addressSize);
}

int getServerSocket(char* hostName, char* port)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo* addressInfo;
	int error = getaddrinfo(hostName, port, &hints, &addressInfo);
	if (error != 0)
	{
		printf("[ERROR] getaddrinfo: %s - %s:%s\n", gai_strerror(error), hostName, port);
		return 0;
	}

	int hostSocket;
	struct addrinfo* address;
	for(address = addressInfo; address != NULL; address = address->ai_next)
	{	
		if ((hostSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) == -1
			|| connect(hostSocket, address->ai_addr, address->ai_addrlen) == -1)
		{
			printf("[ERROR] Unable to connect to %s:%s\n", hostName, port);
			close(hostSocket);
		}
		else
		{
			break;
		}
	}

	freeaddrinfo(addressInfo);
	return address == NULL ? 0 : hostSocket;
}