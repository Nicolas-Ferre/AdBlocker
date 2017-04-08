#ifndef _HTTPREQUEST
#define _HTTPREQUEST

/*
	Retourne le nom de l'hôte contenu dans la commande GET d'une requête HTTP
*/
char* getHostFromGetCommand(char* command);

/*
	Retourne le port dans une expression du type "hostname(:port)"
	Retourne 80 si aucun port n'est spécifié
	host : chaine contenant le port à extraire
*/
int getPort(char* host);

/*
	Retourne le nom d'hôte dans une expression du type "hostname(:port)"
	host : chaine contenant le nom d'hôte à extraire
*/
char* getHostName(char* host);

#endif