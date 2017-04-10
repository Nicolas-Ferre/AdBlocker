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
				// On selectionne les termes utilisables par notre bloqueur
				strncpy(motsList[prochainMot], ligne, strlen(ligne) - 1);
				prochainMot++;
				(*wordCount)++;
				// On ajoute chaque terme dans un tableau stocké en mémoir le temps de la connection
			}
      	}
	}	
	 	
    else {
        fputs("erreur: mauvais chemin d'acces pour la liste\n",stderr);
    }
	fclose(fp);
}


void decoupageListeDansFichier() {
	
	char ligne[LIST_COLUMN_COUNT]; 
	int prochainMot = 0, i = 0, j =0;
	FILE *fp;
	FILE *fp2;
   	fp=fopen("assets/easylist.txt","r"); 
	fp2=fopen("assets/list.txt","w");
	if(fp!=NULL && fp2 !=NULL) {
	   while ( fgets ( ligne, LIST_COLUMN_COUNT, fp ) != NULL ) {
			char c = ligne[0];
			if ( c == '.' || c == '-' || c == '_'  || (c>='1' && c<='9') || (c>='A' && c<='Z') || (c>='a' && c<='z') ) {
				// On selectionne les termes utilisables par notre bloqueur
				fprintf(fp2, "%s", ligne);
				// On ajoute chaque terme dans un fichier qui sera réutilisé lors des connexions
			}
      	}
	}	
	 	
    else {
        fputs("erreur: mauvais chemin d'acces pour les fichiers\n",stderr);
    }
	fclose(fp);
	fclose(fp2);
}