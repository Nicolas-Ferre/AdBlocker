#include "extraction.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>

void decoupageListe(char* motsList[LIST_LINE_COUNT], int* wordCount) {
	
	char ligne[LIST_COLUMN_COUNT]; 
	int prochainMot = 0, i = 0, j =0;
	FILE *fp;
   	fp=fopen("assets/easylist.txt","r"); 
	if(fp!=NULL) {
	   while ( fgets ( ligne, LIST_COLUMN_COUNT, fp ) != NULL ) {
			char c = ligne[0];
			if ( c == '.' || c == '-' || c == '_'  || (c>='1' && c<='9') || (c>='A' && c<='Z') || (c>='a' && c<='z') ) {
				strncpy(motsList[prochainMot], ligne, strlen(ligne) - 1);
				prochainMot++;
				wordCount++;
				printf("%s\n",motsList[prochainMot - 1]);
			}
      	}
	}	
	 	
    else {
        fputs("erreur: mauvais chemin d'acces pour la liste\n",stderr);
    }
	fclose(fp);
}