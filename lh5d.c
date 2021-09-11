#include "lh5.h"
#include <stdlib.h>
#include <string.h>						/* memmove() */
#include <stdarg.h>
#include <limits.h>


#define INIT_CRC  0  /* CCITT: 0xFFFF */

#define BITBUFSIZ (CHAR_BIT * sizeof bitbuf)

/* encode.c and decode.c */

#define DICBIT    13    /* 12(-lh4-) or 13(-lh5-) */
#define DICSIZ (1U << DICBIT)
#define MATCHBIT   8    /* bits for MAXMATCH - THRESHOLD */
#define MAXMATCH 256    /* formerly F (not more than UCHAR_MAX + 1) */
#define THRESHOLD  3    /* choose optimal value */
#define PERC_FLAG 0x8000U

/* huf.c */

#define NC (UCHAR_MAX + MAXMATCH + 2 - THRESHOLD)
	/* alphabet = {0, 1, 2, ..., NC - 1} */
#define CBIT 9  /* $\lfloor \log_2 NC \rfloor + 1$ */
#define CODE_BIT  16  /* codeword length */


#define NP (DICBIT + 1)
#define NT (CODE_BIT + 3)
#define PBIT 4							/* smallest integer such that (1U << PBIT) > NP */
#define TBIT 5							/* smallest integer such that (1U << TBIT) > NT */
#if NT > NP
#define NPT NT
#else
#define NPT NP
#endif

static unsigned short left[2 * NC - 1];
static unsigned short right[2 * NC - 1];

static unsigned char c_len[NC];
static unsigned char pt_len[NPT];
static unsigned short c_table[4096];
static unsigned short pt_table[256];


#define CRCPOLY  0xA001					/* ANSI CRC-16 */
						 /* CCITT: 0x8408 */
#define UPDATE_CRC(c) \
	crc = crctable[(crc ^ (c)) & 0xFF] ^ (crc >> CHAR_BIT)

#ifndef LH5_NO_CRC
unsigned int crc;
unsigned short crctable[UCHAR_MAX + 1];
#endif


static unsigned int bitbuf;
static unsigned int subbitbuf;
static int bitcount;
static int blocksize;
unsigned char *lh5_packedMem;
unsigned long lh5_compsize;
unsigned char *lh5_buffer;
unsigned long lh5_origsize;

static int dec_j;						/* remaining bytes to copy */
static size_t dec_i;

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

#if 0 /* not used here */
static void error(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	putc('\n', stderr);
	vfprintf(stderr, fmt, args);
	putc('\n', stderr);
	va_end(args);
	exit(EXIT_FAILURE);
}
#endif

/* ---------------------------------------------------------------------- */

#ifndef LH5_NO_CRC
static void make_crctable(void)
{
	unsigned int i;
	unsigned int j;
	unsigned int r;

	static int crc_ready = 0;

	if (crc_ready == 0)
	{
		for (i = 0; i <= UCHAR_MAX; i++)
		{
			r = i;
			for (j = 0; j < CHAR_BIT; j++)
			{
				if (r & 1)
					r = (r >> 1) ^ CRCPOLY;
				else
					r >>= 1;
			}
			crctable[i] = r;
		}
		crc_ready = 1;
	}
}
#endif

/* ---------------------------------------------------------------------- */

static void fillbuf(int n)			/* Shift bitbuf n bits left, read n bits */
{
	bitbuf <<= n;
	while (n > bitcount)
	{
		bitbuf |= subbitbuf << (n -= bitcount);
		if (lh5_compsize != 0)
		{
			lh5_compsize--;
			subbitbuf = *lh5_packedMem++;
		} else
		{
			subbitbuf = 0;
		}
		bitcount = CHAR_BIT;
	}
	bitbuf |= subbitbuf >> (bitcount -= n);
}

/* ------------------------------------------------------------------------- */

static unsigned int getbits(int n)
{
	unsigned int x;

	x = bitbuf >> (BITBUFSIZ - n);
	fillbuf(n);
	return x;
}

/* ------------------------------------------------------------------------- */

#if 0 /* not used here */
static void fwrite_crc(unsigned char *p, int n, FILE * f)
{
	if (f != NULL)
		if ((int)fwrite(p, 1, n, f) < n)
			error("Unable to write");
#ifndef LH5_NO_CRC
	while (--n >= 0)
		UPDATE_CRC(*p++);
#endif
}
#endif

/* --------------------------- End of IO.C --------------------------- */

/***********************************************************
	decode.c
***********************************************************/



/* ----------------------Start of huf.c ------------------------------- */

/***********************************************************
	maketbl.c -- make table for decoding
***********************************************************/

static void make_table(unsigned int nchar, unsigned char bitlen[], unsigned int tablebits, unsigned short table[])
{
	unsigned short count[17];
	unsigned short weight[17];
	unsigned short start[18];
	unsigned short *p;
	unsigned int i, k, len, ch, jutbits, avail, nextcode, mask;

	for (i = 1; i <= 16; i++)
		count[i] = 0;
	for (i = 0; i < nchar; i++)
		count[bitlen[i]]++;

	start[1] = 0;
	for (i = 1; i <= 16; i++)
		start[i + 1] = start[i] + (count[i] << (16 - i));

#if 0
	if (start[17] != (unsigned short) (1U << 16))
	{
		/* error("Bad table"); */
		return;
	}
#endif

	jutbits = 16 - tablebits;
	for (i = 1; i <= tablebits; i++)
	{
		start[i] >>= jutbits;
		weight[i] = 1U << (tablebits - i);
	}
	while (i <= 16)
	{
#ifdef __GNUC__
		weight[i] = 1U << (16 - i);
		i++;
#else
		weight[i++] = 1U << (16 - i);
#endif
	}

	i = *(start + tablebits + 1) >> jutbits;
	if (i != (unsigned short) (1UL << 16))
	{
		k = 1U << tablebits;
		while (i != k)
			table[i++] = 0;
	}

	avail = nchar;
	mask = 1U << (15 - tablebits);
	for (ch = 0; ch < nchar; ch++)
	{
		if ((len = bitlen[ch]) == 0)
			continue;
		nextcode = start[len] + weight[len];
		if (len <= tablebits)
		{
			for (i = start[len]; i < nextcode; i++)
				table[i] = ch;
		} else
		{
			k = start[len];
			p = &table[k >> jutbits];
			i = len - tablebits;
			while (i != 0)
			{
				if (*p == 0)
				{
					right[avail] = left[avail] = 0;
					*p = avail++;
				}
				if (k & mask)
					p = &right[*p];
				else
					p = &left[*p];
				k <<= 1;
				i--;
			}
			*p = ch;
		}
		start[len] = nextcode;
	}
}

/* ------------------------------------------------------------------------- */

/***********************************************************
	huf.c -- static Huffman
***********************************************************/

static void read_pt_len(int nn, int nbit, int i_special)
{
	int i;
	int c;
	int n;
	unsigned int mask;

	n = getbits(nbit);
	if (n == 0)
	{
		c = getbits(nbit);
		for (i = 0; i < nn; i++)
			pt_len[i] = 0;
		for (i = 0; i < 256; i++)
			pt_table[i] = c;
	} else
	{
		i = 0;
		while (i < n)
		{
			c = bitbuf >> (BITBUFSIZ - 3);
			if (c == 7)
			{
				mask = 1U << (BITBUFSIZ - 1 - 3);
				while (mask & bitbuf)
				{
					mask >>= 1;
					c++;
				}
			}
			fillbuf((c < 7) ? 3 : c - 3);
			pt_len[i++] = c;
			if (i == i_special)
			{
				c = getbits(2);
				while (--c >= 0)
					pt_len[i++] = 0;
			}
		}
		while (i < nn)
			pt_len[i++] = 0;
		make_table(nn, pt_len, 8, pt_table);
	}
}

/* ------------------------------------------------------------------------- */

static void read_c_len(void)
{
	int i;
	int c;
	int n;
	unsigned int mask;

	n = getbits(CBIT);
	if (n == 0)
	{
		c = getbits(CBIT);
		for (i = 0; i < NC; i++)
			c_len[i] = 0;
		for (i = 0; i < 4096; i++)
			c_table[i] = c;
	} else
	{
		i = 0;
		while (i < n)
		{
			c = pt_table[bitbuf >> (BITBUFSIZ - 8)];
			if (c >= NT)
			{
				mask = 1U << (BITBUFSIZ - 1 - 8);
				do
				{
					if (bitbuf & mask)
						c = right[c];
					else
						c = left[c];
					mask >>= 1;
				} while (c >= NT);
			}
			fillbuf(pt_len[c]);
			if (c <= 2)
			{
				if (c == 0)
					c = 1;
				else if (c == 1)
					c = getbits(4) + 3;
				else
					c = getbits(CBIT) + 20;
				while (--c >= 0)
					c_len[i++] = 0;
			} else
			{
				c_len[i++] = c - 2;
			}
		}
		while (i < NC)
			c_len[i++] = 0;
		make_table(NC, c_len, 12, c_table);
	}
}

/* ------------------------------------------------------------------------- */

static unsigned int decode_c(void)
{
	unsigned int j;
	unsigned int mask;

	if (blocksize == 0)
	{
		blocksize = getbits(16);
		read_pt_len(NT, TBIT, 3);
		read_c_len();
		read_pt_len(NP, PBIT, -1);
	}
	blocksize--;
	j = c_table[bitbuf >> (BITBUFSIZ - 12)];
	if (j >= NC)
	{
		mask = 1U << (BITBUFSIZ - 1 - 12);
		do
		{
			if (bitbuf & mask)
				j = right[j];
			else
				j = left[j];
			mask >>= 1;
		} while (j >= NC);
	}
	fillbuf(c_len[j]);
	return j;
}

/* ------------------------------------------------------------------------- */

static unsigned int decode_p(void)
{
	unsigned int j;
	unsigned int mask;

	j = pt_table[bitbuf >> (BITBUFSIZ - 8)];
	if (j >= NP)
	{
		mask = 1U << (BITBUFSIZ - 1 - 8);
		do
		{
			if (bitbuf & mask)
				j = right[j];
			else
				j = left[j];
			mask >>= 1;
		} while (j >= NP);
	}
	fillbuf(pt_len[j]);
	if (j != 0)
		j = (1U << (j - 1)) + getbits(j - 1);
	return j;
}

/* ----------------------End   of huf.c ------------------------------- */

/* ------------------------------------------------------------------------- */

/* The calling function must keep the number of
   bytes to be processed.  This function decodes
   either 'count' bytes or 'DICSIZ' bytes, whichever
   is smaller, into the array 'buffer[]' of size
   'DICSIZ' or more.
   Call decode_start() once for each new file
   before calling this function. */
void lh5_decode1(unsigned int count)
{
	unsigned int c;
	size_t r;

	r = 0;
	if (count != 0)
	{
		bitbuf = 0;
		subbitbuf = 0;
		bitcount = 0;
		fillbuf((int)BITBUFSIZ);
		blocksize = 0;
		dec_j = 0;
	} else
	{
		while (--dec_j >= 0)
		{
			lh5_buffer[r] = lh5_buffer[dec_i];
#if 0
			dec_i = (dec_i + 1) & (DICSIZ - 1);
#else
			++dec_i;
#endif
			if (++r == lh5_origsize)
				return;
		}
	}
	for (;;)
	{
		c = decode_c();
		if (c <= UCHAR_MAX)
		{
			lh5_buffer[r] = c;
			if (++r == lh5_origsize)
				return;
		} else
		{
			dec_j = c - (UCHAR_MAX + 1 - THRESHOLD);
			dec_i = ((r - decode_p() - 1)) /* & (DICSIZ - 1) */;
			while (--dec_j >= 0)
			{
				lh5_buffer[r] = lh5_buffer[dec_i];
#if 0
				dec_i = (dec_i + 1) & (DICSIZ - 1);
#else
				++dec_i;
#endif
				if (++r == lh5_origsize)
					return;
			}
		}
	}
}

/* ------------------------------------------------------------------------- */

#if 0 /* not used here */
void lh5_decode(unsigned long origsize, unsigned long pacsize)
{
	int n;

#ifndef LH5_NO_CRC
	make_crctable();
	crc = INIT_CRC;
#endif
	lh5_compsize = pacsize;

	bitbuf = 0;
	subbitbuf = 0;
	bitcount = 0;
	fillbuf((int)BITBUFSIZ);
	blocksize = 0;
	dec_j = 0;

	while (origsize != 0)
	{
		n = (unsigned int) ((origsize > DICSIZ) ? DICSIZ : origsize);

		lh5_decode1(n);
		fwrite_crc(lh5_buffer, n, outfile);
		origsize -= n;
	}
}
#endif
