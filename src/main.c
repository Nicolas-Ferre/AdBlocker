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

int main (int argc, char *argv[])
{
	// Creation du socket d'écoute
	char buffer[BUFFER_SIZE];
	int listenerSocket = createListenerSocket();

	//decoupageListeDansFichier();

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

					if (isAdOptiBis(host) == 0)
						retrieveHostResponse(host, buffer, n, clientSocket);	// Envoie de la requête à l'hôte et envoie de la réponse au navigateur
					else if (isAdOptiBis(host) == 1)
						printf("Publicité supprimée : %s\n", host);
					else
						printf("Erreur : %s\n", host);
				}
				else if (strncmp(buffer, "CONNECT", 7) == 0)
				{
					// Traitement de la requête CONNECT
					char* host = getHostFromGetCommand(buffer);	// Extraction de l'hôte

					if (isAdOptiBis(host) == 0)
					{
						send(clientSocket, "HTTP/1.0 200 Connection established\r\n\r\n", strlen("HTTP/1.0 200 Connection established\r\n\r\n"), 0);
						retrieveHostSslResponse(host, buffer, n, clientSocket);
					}
					else if (isAdOptiBis(host) == 1)
						printf("Publicité supprimée : %s\n", host);
					else
						printf("Erreur : %s\n", host);
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