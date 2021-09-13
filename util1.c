#include "hyp2gdos.h"
#include <ctype.h>

#undef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#undef min
#define min(a, b) ((a) < (b) ? (a) : (b))

static char refstr[MAXPATH];
static _BOOL ref_caseinsens;

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

static int mystrcasecmp(const char *_s1, const char *_s2)
{
	const unsigned char *s1 = (const unsigned char *)_s1;
	const unsigned char *s2 = (const unsigned char *)_s2;
	unsigned char c1;
	unsigned char c2;
	
	while ((c1 = toupper(*s1++)) == (c2 = toupper(*s2++)))
	{
		if (c1 == 0)
			return 0;
	}
	return c1 - c2;
}

/* ---------------------------------------------------------------------- */

int set_ref_string(const char *str, _BOOL caseinsens, _BOOL d1)
{
	if (d1)
		return 1;
	strcpy(refstr, str);
	ref_caseinsens = caseinsens;
	return 0;
}

/* ---------------------------------------------------------------------- */

int ref_strcmp(const char *str, const char *end)
{
	UNUSED(end);
	return ref_caseinsens ? mystrcasecmp(refstr, str) : strcmp(refstr, str);
}

/* ---------------------------------------------------------------------- */

void set_lrect(long *dst, long a, long b, long c, long d)
{
	dst[0] = a;
	dst[1] = b;
	dst[2] = c;
	dst[3] = d;
}

/* ---------------------------------------------------------------------- */

_WORD rc_intersect(const GRECT *r1, GRECT *r2)
{
	_WORD tx, ty, tw, th;
	
	tx = max(r2->g_x, r1->g_x);
	ty = max(r2->g_y, r1->g_y);
	tw = min(r2->g_x + r2->g_w, r1->g_x + r1->g_w);
	th = min(r2->g_y + r2->g_h, r1->g_y + r1->g_h);
	if (tw > tx && th > ty)
	{
		r2->g_x = tx;
		r2->g_y = ty;
		r2->g_w = tw - tx;
		r2->g_h = th - ty;
		return TRUE;
	}
	return FALSE;
}

/* ---------------------------------------------------------------------- */

void rc_shrink(GRECT *gr, _WORD dx, _WORD dy)
{
	gr->g_w -= dx * 2;
	gr->g_h -= dy * 2;
	if (gr->g_w <= 0 || gr->g_h <= 0)
	{
		gr->g_w = gr->g_h = 0;
	} else
	{
		gr->g_x += dx;
		gr->g_y += dy;
	}
}

/* ---------------------------------------------------------------------- */

int clearpath(Path *path)
{
	memset(path, 0, sizeof(*path));
	return 0;
}

/* ---------------------------------------------------------------------- */

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
	while (p >= filename && *p != '\\');
	return p + 1;
}

/* ---------------------------------------------------------------------- */

_BOOL get_name(char *name, const char *filename)
{
	strcpy(name, mybasename(filename));
	return FALSE;
}

/* ---------------------------------------------------------------------- */

_BOOL is_relative_path(const char *filename)
{
	return mybasename(filename) == filename;
}

/* ---------------------------------------------------------------------- */

_BOOL get_basename(const char *filename, char *name)
{
	strcpy(name, mybasename(filename));
	return *name != '\0';
}

/* ---------------------------------------------------------------------- */

_BOOL get_dirname(const char *filename, char *name)
{
	char *p;
	
	strcpy(name, filename);
	p = (char *)mybasename(name);
	*p = '\0';
	return TRUE;
}

/* ---------------------------------------------------------------------- */

_BOOL append_path(const char *dirname, const char *filename, char *buf)
{
	char *p;

	strcpy(buf, dirname);
	p = (char *)mybasename(buf);
	*p = '\0';
	strcat(buf, filename);
	return TRUE;
}

/* ---------------------------------------------------------------------- */

_BOOL strmcpy(const char *src, char *dst, int len)
{
	strncpy(dst, src, len);
	dst[len - 1] = '\0';
	return TRUE;
}

/* ---------------------------------------------------------------------- */

int setpath(const char *value, char *path)
{
	strcpy(path, value);
	return 0;
}

/* ---------------------------------------------------------------------- */

FILE *myfopen(const char *filename, const char *mode)
{
	Path path;
	
	strmcpy(filename, path.buf, (int)sizeof(path.buf));
	return fopen(path.buf, mode);
}

/* ---------------------------------------------------------------------- */

int pathcopy(Path *dst, const Path *src)
{
	if (src == NULL)
		return clearpath(dst);
	*dst = *src;
	return 0;
}

/* ---------------------------------------------------------------------- */

_BOOL pathequal(const Path *p1, const Path *p2)
{
	return memcmp(p1, p2, sizeof(*p1)) == 0;
}

/* ---------------------------------------------------------------------- */

int get_cwd(char *path)
{
	get_curdir(path);
	return 1;
}

/* ---------------------------------------------------------------------- */

_BOOL fexists(const char *filename)
{
	FILE *fp;
	
	fp = myfopen(filename, "rb");
	if (fp != NULL)
		fclose(fp);
	if (fp != NULL)
		return TRUE;
	return FALSE;
}

