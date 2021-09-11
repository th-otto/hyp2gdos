#include "hyp2gdos.h"

static char *sysvarptr;

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

static long _get_sysvar(void)
{
	return *sysvarptr;
}

/* ---------------------------------------------------------------------- */

static char get_sysvar(char *ptr)
{
	sysvarptr = ptr;
	return Supexec(_get_sysvar);
}

/* ---------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------- */

char *get_bootdir(char *path)
{
	path[0] = get_sysvar((char *)0x447) + 'A';
	path[1] = ':';
	path[2] = '\\';
	path[3] = '\0';
	
	return path;
}

/* ---------------------------------------------------------------------- */

char *gethomedir(char *path)
{
	const char *p;
	
	if ((p = getenv("HOME")) == NULL)
		return NULL;
	return addslash(strcpy(path, p));
}

/* ---------------------------------------------------------------------- */

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
}
