#include "hyp2gdos.h"


/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

struct file *openfile(const char *filename)
{
	struct file *f;
	FILE *fp;
	
	fp = fopen(filename, "rb");
	if (fp == NULL)
		return NULL;
	f = malloc(sizeof(*f));
	if (f == NULL)
	{
		fclose(fp);
		return NULL;
	}
	memset(f, 0, sizeof(*f));
	f->fp = fp;
	f->avail = sizeof(f->buf);
	f->pos = f->avail;
	f->eof = FALSE;
	f->overflow = FALSE;
	f->lastc = 0;
	f->lineno = 0;
	return f;
}

/* ---------------------------------------------------------------------- */

int closefile(struct file *f)
{
	fclose(f->fp);
	free(f);
	return 0;
}

/* ---------------------------------------------------------------------- */

static int f_getc(struct file *f)
{
	if (f->pos >= f->avail)
	{
		if (f->avail <= 0)
			return EOF;
		f->avail = fread(f->buf, 1, sizeof(f->buf), f->fp);
		f->pos = 0;
		if (f->avail <= 0)
			return EOF;
	}
	return f->buf[f->pos++];
}

/* ---------------------------------------------------------------------- */

long readline(struct file *f, char *line, size_t len)
{
	int c;
	size_t pos;
	
	f->overflow = FALSE;
	pos = 0;
	for (;;)
	{
		c = f_getc(f);
		switch (c)
		{
		case EOF:
			line[pos] = '\0';
			if (f->eof)
				return EOF;
			++f->lineno;
			f->eof = TRUE;
			return pos;
		
		case 0x0d:
			f->lastc = 0x17;
			c = f_getc(f);
			if (c != 0x0a && c != EOF)
			{
				f->lastc = 0x0d;
				f->pos--;
			}
			line[pos] = '\0';
			++f->lineno;
			return pos;
	
		case 0x0a:
			f->lastc = 0x0a;
			line[pos] = '\0';
			++f->lineno;
			return pos;
		
		default:
			if (pos == len - 1)
			{
				f->pos--;
				line[pos] = '\0';
				f->overflow = TRUE;
				return pos;
			}
			line[pos] = c;
			pos++;
			break;
		}
	}
}

