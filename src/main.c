#include "socket.h"
#include "http_request.h"
#include "proxy.h"
#include "string.h"
#include "constants.h"
#include "extraction.h"
#include <stdlib.h>
#include <netdb.h>
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>

int main (int argc, char *argv[])
{
	/*int wordCount = 0;
	char listeMots[LIST_LINE_COUNT][LIST_COLUMN_COUNT];
	decoupageListe(listeMots, &wordCount);

	int i = 0;
	for (i = 0; i < wordCount; ++i)
	{
		printf("%s\n",listeMots[i]);
	}

	printf("=== Done ===\n");*/

	char buffer[BUFFER_SIZE];
	int listenerSocket = createListenerSocket();

	while (listenerSocket != 0)
	{
		int clientSocket = getClientSocket(listenerSocket);

		if (fork() == 0)
		{
			close(listenerSocket);

			int n = recv(clientSocket, buffer, BUFFER_SIZE, 0);	// Récupération de la commande GET du navigateur

			if (strncmp(buffer + n - 4, "\r\n\r\n", 4) == 0)
			{
				if (strncmp(buffer, "GET", 3) == 0)
				{
					// Traitement de la requête GET
					char* host = getHostFromGetCommand(buffer);	// Extraction de l'hôte
					retrieveHostResponse(host, buffer, n, clientSocket);	// Envoie de la requête à l'hôte et envoie de la réponse au navigateur
				}
				else if (strncmp(buffer, "CONNECT", 7) == 0)
				{
					// Traitement de la requête CONNECT
					char* host = getHostFromGetCommand(buffer);	// Extraction de l'hôte
					send(clientSocket, "HTTP/1.1 200 Connection established\r\n\r\n", strlen("HTTP/1.1 200 Connection established\r\n\r\n"), 0);

					n = recv(clientSocket, buffer, BUFFER_SIZE, 0);
					retrieveHostResponse(host, buffer, n, clientSocket);
				}
			}

			// Fermeture de la connexion entre le navigateur et le serveur
			shutdown(clientSocket, SHUT_RDWR);
			close(clientSocket);
			exit(0);
		}
	}

	close(listenerSocket);
	return 0;
}