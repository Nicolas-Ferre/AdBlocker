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

int isAdOpti(char * host ) {

	if ( host == NULL ) {
		return -1;
		//retourne une erreur dans le cas d'un hostname vide
	}
	
	char ligne[LIST_COLUMN_COUNT];
	char ligne2[LIST_COLUMN_COUNT];
	FILE *fp;
   	fp=fopen("assets/easylist.txt","r"); 
	if(fp!=NULL) {
	   while ( fgets ( ligne, LIST_COLUMN_COUNT, fp ) != NULL ) {

			ligne[strlen(ligne) - 2] = '\0';
			char * res = strstr(host,ligne);
			if (res != 0) {
				fclose(fp);
				return 1;
				// Si l'hostname contient motsList[i] alors c'est un lien vers un hébergeur de publicité, on renvoie vrai (1)
			}
      	}
		fclose(fp);
		return 0;
	}	
	 	
    else {
        fputs("erreur: mauvais chemin d'acces pour la liste\n",stderr);
		fclose(fp);
		return -1;
    }
}

int isAdOptiBis(char * host ) {

	if ( host == NULL ) {
		return -1;
		//retourne une erreur dans le cas d'un hostname vide
	}
	
	char ligne[LIST_COLUMN_COUNT];
	char ligne2[LIST_COLUMN_COUNT];
	FILE *fp;
   	fp=fopen("assets/list.txt","r"); 
	if(fp!=NULL) {
	   while ( fgets ( ligne, LIST_COLUMN_COUNT, fp ) != NULL ) {

			ligne[strlen(ligne) - 2] = '\0';
			char * res = strstr(host,ligne);
			if (res != 0) {
				fclose(fp);
				return 1;
				// Si l'hostname contient motsList[i] alors c'est un lien vers un hébergeur de publicité, on renvoie vrai (1)
			}
      	}
		fclose(fp);
		return 0;
	}	
	 	
    else {
        fputs("erreur: mauvais chemin d'acces pour la liste\n",stderr);
		fclose(fp);
		return -1;
    }
}