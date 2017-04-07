#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <netdb.h>

#define PORT 12345
#define BUFFER_SIZE 8192

char httpGetCommand[BUFFER_SIZE];
char receivedData[500];

/*
	Récupère l'adresse de la machine courante
*/
struct sockaddr_in getAddress()
{
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_port = htons(PORT);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	return address;
}

/*
	Crée un socket d'écoute
	address : adresse de la machine courante
*/
int createListenerSocket(struct sockaddr_in* address)
{
	int listenerSocket;
	if ((listenerSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("[ERROR] Listener socket creation\n");
		exit(0);
	}

	bind(listenerSocket, (struct sockaddr *)address, sizeof(*address));
	listen(listenerSocket, 5);

	return listenerSocket;
}

/*
	Crée un socket client en acceptant la connection
	listenerSocket : socket d'écoute
*/
int getClientSocket(int listenerSocket)
{
	int addressSize = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddress;
	return accept(listenerSocket, (struct sockaddr*)&clientAddress, &addressSize);
}

/*
	Extrait une propriété de la commande GET d'une requête HTTP
	property : propriété à extraire
	command : commande GET
*/
int getPropertyValuePositionInGetCommand(char* property, char* command)
{
	int i = 0;
	char temporaryString[strlen(command)];
	char strToCompare[strlen(property) + 2];
	sprintf(strToCompare, "%s: ", property);
	

	for (i = 0; i < strlen(command) - strlen(strToCompare); ++i)
	{
		temporaryString[0] = '\0';
		strncat(temporaryString, command + i, strlen(strToCompare));
		if (strcmp(temporaryString, strToCompare) == 0)
			return i + strlen(strToCompare);
	}

	return -1;
}

/*
	Extrait la fin de la ligne du string à partir d'une position dans ce string
	string : string dans lequel il faut extraire le résultat
	firstPosition : position du premier caractère à récupérer
*/
char* getEndOfStringLine(char* string, int firstPosition)
{
	int stringLength = strlen(string);
	int returnLinePosition = stringLength - 1;
	int i = 0;

	for (i = firstPosition; i < stringLength && returnLinePosition == stringLength - 1; ++i)
		if (string[i] == '\n')
			returnLinePosition = i - 1;

	int endOfLineSize = i - firstPosition - 1;
	char* endOfLine = malloc(sizeof(char) * endOfLineSize);
	strncat(endOfLine, string + firstPosition, endOfLineSize);
	endOfLine[endOfLineSize - 1] = '\0';
	return endOfLine;
}

/*
	Récupère le nom de l'hôte dans la commande GET d'une requête HTTP
*/
char* getHostFromGetCommand(char* command)
{
	static char host[HOST_NAME_MAX];

	int propertyValuePosition = getPropertyValuePositionInGetCommand("Host", command);
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

/*
	Crée un client et récupère les données de la page demandée par la commande GET
*/
void retrieveHostResponse(char* hostName, char* command, int clientSocket)
{	
	int port = getPort(hostName);
	hostName = getHostName(hostName);
	char portString[6];
	sprintf(portString, "%d", port);

	if (port == 80)
	{
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		int error;
		struct addrinfo* addressInfo;
		if ((error = getaddrinfo(hostName, portString, &hints, &addressInfo)) != 0)
		{
			fprintf(stderr, "[ERROR] getaddrinfo: %s\n", gai_strerror(error));
			exit(1);
		}

		struct addrinfo* address;
		for(address = addressInfo; address != NULL; address = address->ai_next)
		{
			int hostSocket;
			if ((hostSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) == -1)
			{
				printf("[ERROR] Host socket creation\n");
				continue;
			}

			if (connect(hostSocket, address->ai_addr, address->ai_addrlen) == -1)
			{
				printf("[ERROR] Unable to connect to %s\n", hostName);
				close(hostSocket);
				continue;
			}

			struct timeval tv = {5, 0};
			setsockopt(hostSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
			send(hostSocket, command, strlen(command), 0);

			while (1)
			{
				int n = recv(hostSocket, receivedData, BUFFER_SIZE, 0);
				if (n <= 0)
					break;

				send(clientSocket, receivedData, n, 0);
			}

			break;
		}

		if (address == NULL)
		{
			fprintf(stderr, "[ERROR] Connection\n");
			exit(1);
		}

		freeaddrinfo(addressInfo);


		/*int hostSocket;
		if ((hostSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			printf("[ERROR] Host socket creation\n");
			return;
		}

		struct hostent* host = gethostbyname(hostName);
		if (host == NULL)
		{
			printf("[ERROR] Host %s not found\n", hostName);
			return;
		}

		struct sockaddr_in hostAddress;
		bzero(&hostAddress, sizeof(hostAddress));
		hostAddress.sin_family = AF_INET;
		bcopy(host->h_addr, &hostAddress.sin_addr.s_addr, host->h_length);
		hostAddress.sin_port = htons(port);

		if (connect(hostSocket, (struct sockaddr*)&hostAddress, sizeof(hostAddress)) == -1)
		{
			printf("[ERROR] Unable to connect to %s\n", hostName);
			close(hostSocket);
			return;
		}

		struct timeval tv = {5, 0};
		setsockopt(hostSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
		send(hostSocket, command, strlen(command), 0);

		while (1)
		{
			int n = recv(hostSocket, receivedData, BUFFER_SIZE, 0);
			if (n <= 0)
				break;

			send(clientSocket, receivedData, n, 0);
		}

		close(hostSocket);*/
	}
}

int main (int argc, char *argv[])
{
	struct sockaddr_in address = getAddress();
	int listenerSocket = createListenerSocket(&address);

	while (1)
	{
		int clientSocket = getClientSocket(listenerSocket);

		if (fork() == 0)
		{
			close(listenerSocket);

			recv(clientSocket, httpGetCommand, BUFFER_SIZE, 0);	// Récupération de la commande GET du navigateur
			char* host = getHostFromGetCommand(httpGetCommand);	// Extraction de l'hôte
			retrieveHostResponse(host, httpGetCommand, clientSocket);	// Envoie de la requête à l'hôte

			// Fermeture de la connexion entre le navigateur et le serveur
			shutdown(clientSocket, SHUT_RDWR);
			close(clientSocket);
			exit(0);
		}
	}

	close(listenerSocket);
	return 0;
}