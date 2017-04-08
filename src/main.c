#include "socket.h"
#include "http_request.h"
#include "proxy.h"
#include "constants.h"
#include <stdlib.h>
#include <netdb.h>
#include  <unistd.h>

int main (int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	int listenerSocket = createListenerSocket();

	while (listenerSocket != 0)
	{
		int clientSocket = getClientSocket(listenerSocket);

		if (fork() == 0)
		{
			close(listenerSocket);

			recv(clientSocket, buffer, BUFFER_SIZE, 0);	// Récupération de la commande GET du navigateur
			char* host = getHostFromGetCommand(buffer);	// Extraction de l'hôte
			retrieveHostResponse(host, buffer, clientSocket);	// Envoie de la requête à l'hôte et envoie de la réponse au navigateur

			// Fermeture de la connexion entre le navigateur et le serveur
			shutdown(clientSocket, SHUT_RDWR);
			close(clientSocket);
			exit(0);
		}
	}

	close(listenerSocket);
	return 0;
}