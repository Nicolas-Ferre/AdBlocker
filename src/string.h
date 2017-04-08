#ifndef _STRING
#define _STRING

/*
	Retourne la position du premier caractère de la valeur correspondant à la propriété cherchée "propriété1: value1\npropriété2: value2\n..."
	Retourne -1 si la propriété n'est pas trouvée
	property : propriété à extraire
	string : string dans lequel on cherche la propriété
*/
int getPropertyValuePositionInString(char* string, char* property);

/*
	Extrait et retourne la fin de la ligne (allouée dynamiquement) du string à partir d'une position dans ce string
	string : string dans lequel il faut extraire le résultat
	firstPosition : position du premier caractère à récupérer
*/
char* getEndOfStringLine(char* string, int firstPosition);

#endif