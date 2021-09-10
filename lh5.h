/***********************************************************
	lh5.h
***********************************************************/
#include <stdio.h>

/* io.c */

extern FILE *infile, *outfile;
extern unsigned int crc;

void decode_lh5(unsigned long orgsize, unsigned long pacsize);
