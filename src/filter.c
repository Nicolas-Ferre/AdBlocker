#include "filter.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>


int isAd(char * host, char* motsList[LIST_LINE_COUNT], int nombreMotsList) {
	int i;
	if ( host == NULL ) {
		return -1;
	}
	for (i = 0; i<nombreMotsList;i++) {
		char * res = strstr(host,motsList[i]);
		if (res != 0) {
			return 1;
		}
	}
	return 0;
}