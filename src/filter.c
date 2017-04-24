#include "filter.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>

int isAd(char * host) {

	if ( host == NULL ) {
		return -1;
		//retourne une erreur dans le cas d'un hostname vide
	}
	
	char ligne[LIST_COLUMN_COUNT];
	FILE *fp = fopen("assets/list.txt","r"); 
	if(fp!=NULL) {
	   while ( fgets ( ligne, LIST_COLUMN_COUNT, fp ) != NULL ) {

			ligne[strlen(ligne) - 1] = '\0';
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