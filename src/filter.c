#include "filter.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>


int isAd(char * host, char* motsList[LIST_LINE_COUNT] ) {
	int i;
	if ( host == NULL ) {
		return -1;
	}
	for (i = 0; i<LIST_LINE_COUNT;i++) {
		char * res = strstr(host,motsList[i]);
		if (res == motsList[i]) {
			return 1;
		}
	}
	return 0;
}