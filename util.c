#include "hyp2gdos.h"
#include <ctype.h>

static char *sysvarptr;




FILE *x14f38(const Path *filename)
{
	(void) filename;
	hyp_get_char_at(0, 0, 0);
	return 0;
}


_BOOL can_scale_bitmaps(_WORD handle)
{
	_WORD workout[57];
	
	vq_extnd(handle, 1, workout);
	return (workout[30] & 1) != 0;
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


void x183a6(long *dst, long a, long b, long c, long d)
{
	(void) dst;
	(void) a;
	(void) b;
	(void) c;
	(void) d;
}


int x1837c(char *str, char *end)
{
	(void) str;
	(void) end;
	return 0;
}


int x18352(const char *str, int d3, int d1)
{
	(void) str;
	(void) d3;
	(void) d1;
	return 0;
}


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


static int x185c8(void)
{
	sprintf(sysvarptr, "hello");
	if (strcmp(sysvarptr, " ")){}
	if (toupper(0)){}
	
	if (timezone * timezone)
		return 0;
	if (timezone / timezone)
		return 0;
	
	(void) Tgetdate();
	
	v_updwk(0);
	v_clrwk(0);
	vs_clip(0, 0, 0);
	vrt_cpyfm(0, 0, 0, 0, 0, 0);
	vro_cpyfm(0, 0, 0, 0, 0);

	return 1;
}

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

int clearpath(Path *path)
{
	memset(path, 0, sizeof(*path));
	return 0;
}


const char *mybasename(const char *filename)
{
	const char *p;

	if (filename[0] != '\0' && filename[1] == ':')
		filename += 2;
	p = filename;
	while (*p != '\0')
		p++;
	do
		--p;
	while (p > filename && *p != '\\');
	return p + 1;
}


_BOOL get_name(char *name, const char *filename)
{
	strcpy(name, mybasename(filename));
	return FALSE;
}


_BOOL is_relative_path(const char *filename)
{
	return mybasename(filename) == filename;
}


_BOOL get_basename(const char *filename, char *name)
{
	strcpy(name, mybasename(filename));
	return *name != '\0';
}


_BOOL get_dirname(const char *filename, char *name)
{
	char *p;
	
	strcpy(name, filename);
	p = (char *)mybasename(name);
	*p = '\0';
	return TRUE;
}


_BOOL append_path(const char *dirname, const char *filename, char *buf)
{
	char *p;

	strcpy(buf, dirname);
	p = (char *)mybasename(buf);
	*p = '\0';
	strcat(buf, filename);
	return TRUE;
}


_BOOL strmcpy(const char *src, char *dst, int len)
{
	strncpy(dst, src, len);
	dst[len - 1] = '\0';
	return TRUE;
}


int setpath(const char *value, char *path)
{
	strcpy(path, value);
	return 0;
}


FILE *myfopen(const char *filename, const char *mode)
{
	Path path;
	
	strmcpy(filename, path.buf, (int)sizeof(path.buf));
	return fopen(path.buf, mode);
}


int pathcopy(Path *dst, const Path *src)
{
	if (src == NULL)
		return clearpath(dst);
	*dst = *src;
	return 0;
}


_BOOL pathequal(const Path *p1, const Path *p2)
{
	return memcmp(p1, p2, sizeof(*p1)) == 0;
}


int get_cwd(char *path)
{
	get_curdir(path);
	return 1;
}


_BOOL fexists(const char *filename)
{
	FILE *fp;
	
	fp = myfopen(filename, "rb");
	if (fp != NULL)
	{
		fclose(fp);
		if (filename != NULL)
			return TRUE;
	}
	return FALSE;
}


static long _get_sysvar(void)
{
	return *sysvarptr;
}


static char get_sysvar(char *ptr)
{
	sysvarptr = ptr;
	return Supexec(_get_sysvar);
}


static char *addslash(char *path)
{
	char *p;
	
	p = path;
	while (*p != '\0')
		p++;
	if (p[-1] != '\\')
	{
		*p++ = '\\';
		*p = '\0';
	}
	return path;
}


char *get_bootdir(char *path)
{
	path[0] = get_sysvar((char *)0x447) + 'A';
	path[1] = ':';
	path[2] = '\\';
	path[3] = '\0';
	
	return path;
}


char *gethomedir(char *path)
{
	const char *p;
	
	if ((p = getenv("HOME")) == NULL)
		return NULL;
	return addslash(strcpy(path, p));
}


void get_curdir(char *path)
{
	int drv;
	char *p;
	
	drv = Dgetdrv();
	p = path;	
	*p++ = drv + 'A';
	*p++ = ':';
	Dgetpath(p, drv + 1);
	addslash(path);
	x185c8(); /* XXX */
}


void *xmalloc(size_t size)
{
	return malloc(size);
}


void xfree(void *p)
{
	free(p);
}


_BOOL interrupted(void)
{
	if ((Kbshift(-1) & 3) == 3)
		return TRUE;
	return FALSE;
}


#ifndef __GNUC__
/*
 * vq_scrninfo is missing in original Pure-C library
 */
#ifdef __PORTVDI_H__
typedef struct
{
    int    control[15];
    int    intin[132];
    int    intout[140];
    int    ptsin[145];
    int    ptsout[145];
} VDIPARBLK;
extern VDIPARBLK _VDIParBlk;
#else
#define control contrl
#endif

void vq_scrninfo(_WORD handle, _WORD *workout)
{
	VDIPB pb;
	
	memset(&_VDIParBlk, 0, sizeof(_VDIParBlk));
	pb.control = _VDIParBlk.control;
	pb.intin = _VDIParBlk.intin;
	pb.intout = workout;
	pb.ptsin = _VDIParBlk.ptsin;
	pb.ptsout = _VDIParBlk.ptsout;
	pb.intin[0] = 2; /* vq_scrninfo call */
	pb.control[0] = 102; /* vq_extnd */
	pb.control[1] = 0; /* #ptsin */
	pb.control[3] = 1; /* #intin */
	pb.control[5] = 1; /* sub opcode */
	pb.control[6] = handle;
	vdi(&pb);
}
#endif
