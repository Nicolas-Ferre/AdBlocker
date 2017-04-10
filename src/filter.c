#include "filter.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>


int isAd(char * host, char* motsList[LIST_LINE_COUNT], int nombreMotsList) {
	int i;
	if ( host == NULL ) {
		return -1;
		//retourne une erreur dans le cas d'un hostname vide
	}
	for (i = 0; i<nombreMotsList;i++) {
		char * res = strstr(host,motsList[i]);
		if (res != 0) {
			return 1;
			// Si l'hostname contient motsList[i] alors c'est un lien vers un hébergeur de publicité, on renvoie vrai (1)
		}
	}
	return 0;
	//Si toute la liste a été parcourue, alors l'hostname ne contient pas de publicité
}