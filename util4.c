#include "hyp2gdos.h"

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

_BOOL interrupted(void)
{
	if ((Kbshift(-1) & 3) == 3)
		return TRUE;
	return FALSE;
}

/* ---------------------------------------------------------------------- */

void get_date(int *day, int *month, int *year)
{
	short date;
	
	date = Tgetdate();
	*day = date & 0x1f;
	*month = (date >> 5) & 0x0f;
	*year = ((date >> 9) & 0x7f) + 1980;
}

/* ---------------------------------------------------------------------- */

FILE *x14f38(const Path *filename)
{
	(void) filename;
	hyp_get_char_at(0, 0, 0);
	return 0;
}


#ifdef __GNUC__
hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename)
{
	(void) hyp;
	(void) pagename;
	return HYP_NOINDEX;
}
#endif


#ifdef __GNUC__
int x14db6(struct pageinfo *page, _WORD *page_num, int *font_idx)
{
	(void)page;
	(void)page_num;
	(void)font_idx;
	return 0;
}
#endif


#ifdef __GNUC__
void *hyp_find_extheader(struct hypfile *hyp, hyp_ext_header type)
{
	(void) hyp;
	(void) type;
	return NULL;
}
#endif


#ifdef __GNUC__
size_t conv_nodename(unsigned char os, char *name)
{
	(void) os;
	(void) name;
	return 0;
}
#endif


#ifdef __GNUC__
void x1509e(struct hypfile *hyp, struct xo4 **data)
{
	(void) hyp;
	(void) data;
}
#endif


#ifdef __GNUC__
int x15132(struct hypfile *hyp, struct xo4 **data, hyp_nodenr node)
{
	(void) hyp;
	(void) data;
	(void) node;
	return 0;
}
#endif



#ifdef __GNUC__
char *dec_255_decode(char *data, short *val)
{
	unsigned short lo;
	
	lo = (unsigned char)*data++ - 1U;
	lo += ((unsigned char)*data++ - 1U) * 255;
	*val = lo;
	return data;
}
#endif






#ifdef __GNUC__
char *dec_255_encode(char *data, short val)
{
	short hi;
	short lo;
	
	hi = val / 255 + 1;
	lo = val % 255 + 1;
	*data++ = lo;
	*data++ = hi;
	return data;
}
#endif
