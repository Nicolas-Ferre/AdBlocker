#ifndef _SOCKET
#define _SOCKET

/*
	Retourne l'adresse de la machine courante
*/
struct sockaddr_in getAddress();

/*
	Retourne un nouveau socket d'écoute
	Retourne 0 si le socket n'a pas pu être créé
*/
int createListenerSocket();

/*
	Retourne le socket client qui demande une connexion
	listenerSocket : socket d'écoute
*/
int getClientSocket(int listenerSocket);

/*
	Retourne le socket de la connexion vers un serveur
	Retourne 0 si la connexion a échoué
	hostName : nom d'hôte du serveur
	port : port de connexion au serveur
*/
int getServerSocket(char* hostName, char* port);

#endif