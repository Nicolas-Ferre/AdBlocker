#include "socket.h"
#include "http_request.h"
#include "proxy.h"
#include "string.h"
#include "constants.h"
#include "extraction.h"
#include "filter.h"
#include <stdlib.h>
#include <netdb.h>
#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>
#include <signal.h>

int listenerSocket;
int clientSocket;
int hostSocket;

void clearProgram()
{
	shutdown(listenerSocket, SHUT_RDWR);
	close(listenerSocket);
	shutdown(clientSocket, SHUT_RDWR);
	close(clientSocket);
	shutdown(hostSocket, SHUT_RDWR);
	close(hostSocket);
	exit(0);
}

int main (int argc, char *argv[])
{
	signal(SIGINT, clearProgram);

	// Creation du socket d'écoute
	char buffer[BUFFER_SIZE];
	listenerSocket = createListenerSocket();

	decoupageListeDansFichier();

	while (listenerSocket != 0)
	{
		clientSocket = getClientSocket(listenerSocket);

		if (fork() == 0)
		{
			close(listenerSocket);
			int n = recv(clientSocket, buffer, BUFFER_SIZE, 0);	// Récupération de la commande GET du navigateur

			if (strncmp(buffer + n - 4, "\r\n\r\n", 4) == 0)
			{
				int isGetCommand = strncmp(buffer, "GET", 3) == 0;
				int isConnectCommand = strncmp(buffer, "CONNECT", 7) == 0;

				if (isGetCommand || isConnectCommand)
				{
					char* host = getHostFromGetCommand(buffer);	// Extraction de l'hôte

					if (isAdOptiBis(host) == 0)
					{
						// Envoie de la requête à l'hôte et envoie de la réponse au navigateur
						if (isGetCommand)
						{
							retrieveHostResponse(host, buffer, n, clientSocket, &hostSocket);
						}
						else
						{
							send(clientSocket, "HTTP/1.0 200 Connection established\r\n\r\n", strlen("HTTP/1.0 200 Connection established\r\n\r\n"), 0);
							retrieveHostSslResponse(host, clientSocket, &hostSocket);
						}
					}
					else if (isAdOptiBis(host) == 1)
					{
						send(clientSocket, "HTTP/1.0 403 Forbidden\r\n\r\n", strlen("HTTP/1.0 403 Forbidden\r\n\r\n"), 0);
						printf("Publicité supprimée : %s\n", host);
					}
					else
					{
						printf("Erreur : %s\n", host);
					}
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
