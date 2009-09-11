#ifndef _HEXFILE_H_
#define _HEXFILE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct intelln {
	int addr, len, type;
	unsigned char * data;
	struct intelln * next;
} intelln_t;

int size(intelln_t * hexdata);
intelln_t * loadhexfile(char * filename);
void deletehexdata(intelln_t * hexdata);

#endif
