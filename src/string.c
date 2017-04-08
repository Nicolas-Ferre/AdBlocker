#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getPropertyValuePositionInString(char* string, char* property)
{
	int i = 0;
	char temporaryString[strlen(string)];
	char strToCompare[strlen(property) + 2];
	sprintf(strToCompare, "%s: ", property);
	

	for (i = 0; i < strlen(string) - strlen(strToCompare); ++i)
	{
		temporaryString[0] = '\0';
		strncat(temporaryString, string + i, strlen(strToCompare));
		if (strcmp(temporaryString, strToCompare) == 0)
			return i + strlen(strToCompare);
	}

	return -1;
}

char* getEndOfStringLine(char* string, int firstPosition)
{
	int stringLength = strlen(string);
	int returnLinePosition = stringLength - 1;
	int i = 0;

	for (i = firstPosition; i < stringLength && returnLinePosition == stringLength - 1; ++i)
		if (string[i] == '\n')
			returnLinePosition = i - 1;

	int endOfLineSize = i - firstPosition - 1;
	char* endOfLine = malloc(sizeof(char) * endOfLineSize);
	strncat(endOfLine, string + firstPosition, endOfLineSize);
	endOfLine[endOfLineSize - 1] = '\0';
	return endOfLine;
}