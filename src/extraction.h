#ifndef _EXTRACTION
#define _EXTRACTION

/*
	Permet d'extraire de Easylist.txt l'ensemble des termes que nous traiterons
	dans la suite du programme (ligne commençant par : -/_/./[a-z]/[A-Z]/[1-9])
*/

#include "constants.h"

void decoupageListe(char* motsList[LIST_LINE_COUNT], int* wordCount);
void decoupageListeDansFichier();

#endif