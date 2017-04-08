#ifndef _PROXY
#define _PROXY

/*
	Crée un client qui se connecte à l'hôte demandé, lui envoie la requête GET et attend en retour les données de la page web, qui seront envoyées au navigateur
	host : string au format "hostname(:port)"
	command : commande GET de la requête HTTP à effectuer
	clientSocket : socket de connexion avec le navigateur
*/
void retrieveHostResponse(char* host, char* command, int clientSocket);

#endif