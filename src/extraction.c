#include <stdio.h>
#include <string.h>



void decoupageListe(char *motsList[x /* a remplacer par la taille de la liste? */]) {
	
	
	char ligne[70]; 
	int prochainMot = 0, i = 0, j =0;
	FILE *fp;
   	fp=fopen("../assets/easylist.txt","r"); 
	if(fp!=NULL) {
	   while ( fgets ( ligne, 70, fp ) != NULL ) { 
 				 	
   			for(i = 0; i < (int)strlen(ligne); i++) {
        		if(ligne[i]=='\n') {
       				motsList[prochainMot][j] = '\0';
       				prochainMot++;
        			j = 0;
        		}
				
        		else if(ligne[i] != '\0' ) {
					motsDico[prochainMot][j] = ligne[i];
					j++;
       			}		
			}
			
   		motsDico[prochainMot][j] = '\0';
      	}
	}	
	 	
    	 else {
      	
          	fputs("erreur: mauvais chemin d'acces pour la liste\n",stderr);
        	
          		
        }
	fclose(fp);
	fp=NULL;
	
}