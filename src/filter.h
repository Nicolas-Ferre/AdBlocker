#ifndef _FILTER
#define _FILTER

/*
	Permet de vérifier si l'hostname contient un substring contenu dans
	la liste des termes correspondant aux hébergeurs de publicité.
*/

#include "constants.h"

int isAd(char * host, char* motsList[LIST_LINE_COUNT], int nombreMotsList);
int isAdOpti(char * host );
int isAdOptiBis(char * host );

#endif