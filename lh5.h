/***********************************************************
	lh5.h
***********************************************************/
#include <stdio.h>

#define LH5_NO_CRC 1

#ifndef LH5_NO_CRC
extern unsigned int crc;
#endif

extern unsigned char *lh5_buffer;
extern unsigned char *lh5_packedMem;
extern unsigned long lh5_compsize;
extern unsigned long lh5_origsize;

void lh5_decode1(unsigned int count);
void lh5_decode(unsigned long origsize, unsigned long pacsize);
