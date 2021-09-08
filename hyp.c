#include "hyp2gdos.h"
#include <errno.h>

int hyp_errno;
char hypfold[MAXPATH];
_WORD x1d9c4;

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

int get_nodetype(struct hypfile *hyp, hyp_nodenr node)
{
	if (hyp != NULL &&
		hyp->indextable != NULL &&
		node >= 0 &&
		node < hyp->header.num_index)
		return hyp->indextable[node]->type;
	return -1;
}

/* ---------------------------------------------------------------------- */

static void conv_all_nodenames(struct hypfile *hyp)
{
	hyp_nodenr i;
	
	for (i = 0; i < hyp->header.num_index; i++)
	{
		switch (hyp->indextable[i]->type)
		{
		case HYP_NODE_INTERNAL:
		case HYP_NODE_POPUP:
			conv_nodename(hyp->header.compiler_os, hyp->indextable[i]->name);
			break;
		}
	}
}

/* ---------------------------------------------------------------------- */

void hyp_free(struct hypfile *hyp)
{
	if (hyp == NULL)
		return;
	hyp->flags &= ~FLAG_80;
	if (hyp->o298 != NULL)
	{
		hyp->o298 = NULL;
		hyp->o302 = NULL;
	}
	if (hyp->o282 != NULL)
	{
		free(hyp->o282);
		hyp->o282 = NULL;
		hyp->o286 = NULL;
	}
	if (hyp->indextable != NULL)
	{
		free(hyp->indextable);
		hyp->indextable = NULL;
	}
	if (hyp->indexdata != NULL)
	{
		free(hyp->indexdata);
		hyp->indexdata = NULL;
	}
	if (hyp->fp != NULL)
	{
		fclose(hyp->fp);
		hyp->fp = NULL;
	}
	clearpath(&hyp->filename);
	hyp->o290 = 0;
	hyp->o296 = -1;
	hyp->o292 = -1;
	hyp->o294 = -1;
}

/* ---------------------------------------------------------------------- */

int hyp_load(struct hypfile *hyp, const Path *filename)
{
	Path pathbuf;
	char basename[MAXPATH];
	FILE *fp;
	int len;
	
	if (filename == NULL)
		return 0;
	if (!(hyp->flags & FLAG_20) && pathequal(&hyp->filename, filename))
		return 0;
	if (hyp->fp != NULL)
	{
		fclose(hyp->fp);
		hyp->fp = NULL;
	}
	pathcopy(&pathbuf, filename);
	fp = x14f38(&pathbuf);
	if (fp != NULL)
	{
		size_t readlen;
		
		if (!(hyp->flags & FLAG_20) && pathequal(&hyp->filename, filename))
		{
			if (hyp->flags & FLAG_40)
			{
				hyp->fp = fp;
			} else
			{
				fclose(fp);
				return 0;
			}
		}
		hyp_free(hyp);
		hyp->flags &= ~FLAG_20;
		pathcopy(&hyp->filename, &pathbuf);
		get_basename(pathbuf.buf, basename);
		len = (int)strlen(basename) + 1;
		if (len & 1)
			len++;
		readlen = fread(&hyp->header, 1, sizeof(hyp->header), fp);
		if (readlen != sizeof(hyp->header) || hyp->header.magic != HYP_MAGIC_HYP)
		{
			if (readlen < 4 || hyp->header.magic != HYP_MAGIC_REF)
			{
				hyp->header.magic = HYP_MAGIC_TXT;
				hyp->header.itable_size = (long)len + 2 * sizeof(INDEX_ENTRY) + 2;
				hyp->header.num_index = 2;
				hyp->header.compiler_vers = 1;
				hyp->header.compiler_os = HYP_OS_UNKNOWN;
				hyp->indexdata = malloc(hyp->header.itable_size);
				/* BUG: no malloc check */
				memset(hyp->indexdata, 0, hyp->header.itable_size);
				if (hyp->indexdata == NULL)
				{
					hyp_errno = ENOENT;
					hyp_free(hyp);
					if (fp)
						fclose(fp);
					return hyp_errno;
				}
			}
		}
		
		hyp->indexdata = malloc(hyp->header.itable_size);
		if (hyp->indexdata == NULL)
		{
			hyp_errno = ENOENT;
			hyp_free(hyp);
			if (fp)
				fclose(fp);
			return hyp_errno;
		}
		if (hyp->header.magic != HYP_MAGIC_HYP)
		{
			INDEX_ENTRY *ptr;
			
			ptr = hyp->indexdata;
			ptr->length = len + sizeof(INDEX_ENTRY);
			ptr->type = HYP_NODE_INTERNAL;
			ptr->seek_offset = 0;
			ptr->comp_diff = 0;
			ptr->next = 0;
			ptr->previous = 0;
			ptr->toc_index = 0;
			strcpy(ptr->name, basename);
			/* BUG: length sign-extended */
			ptr = (INDEX_ENTRY *)(((char *)ptr + (signed char)ptr->length));
			ptr->length = sizeof(INDEX_ENTRY) + 2;
			ptr->type = HYP_NODE_EOF;
			fseek(fp, 0, SEEK_END);
			ptr->seek_offset = ftell(fp);
			ptr->comp_diff = 0;
			ptr->next = 0;
			ptr->previous = 0;
			ptr->toc_index = 0;
			strcpy(ptr->name, "");
		}
	}

	return 0;
}

/* ---------------------------------------------------------------------- */

void hyp_delete(struct hypfile **hyp)
{
	struct hypfile *h;
	
	h = *hyp;
	if (h != NULL)
	{
		hyp_free(h);
		free(h);
		*hyp = NULL;
	}
}

/* ---------------------------------------------------------------------- */

struct hypfile *hyp_new(void)
{
	struct hypfile *hyp;
	
	hyp = NULL;
	hyp = malloc(sizeof(*hyp));
	if (hyp == NULL)
	{
		hyp_errno = ENOENT;
	} else
	{
		memset(hyp, 0, sizeof(*hyp));
		clearpath(&hyp->filename);
		hyp_errno = 0;
		return hyp;
	}
	hyp_delete(&hyp);
	return hyp;
}

int x16734(struct x76 *p, struct hypfile *hyp, _WORD font_width, _WORD font_height, int x)
{
	(void)p;
	(void) hyp;
	(void) font_width;
	(void) font_height;
	(void) x;
	return 0;
}


int x16fd8(void)
{
	return 0;
}


int x17008(int fontidx, _WORD font_id, _WORD size)
{
	(void) fontidx;
	(void) font_id;
	(void) size;
	return 0;
}


_BOOL can_scale_bitmaps(_WORD handle)
{
	_WORD workout[57];
	
	vq_extnd(handle, 1, workout);
	return (workout[30] & 1) != 0;
}

hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename)
{
	(void) hyp;
	(void) pagename;
	return -1;
}


int hyp_load_page(struct x76 *x, void *y, hyp_nodenr node, int z, void *a)
{
	(void)x;
	(void)node;
	(void)y;
	(void)z;
	(void)a;
	return 0;
}


#ifdef __GNUC__
int x14db6(struct x76 *x, _WORD *page_num, int *font_idx)
{
	(void)x;
	(void)page_num;
	(void)font_idx;
	return 0;
}
#endif


hyp_nodenr x16842(struct hypfile *hyp, hyp_nodenr node, int direction)
{
	(void)hyp;
	(void)node;
	(void)direction;
	return node;
}


char *hyp_get_window_title(struct x76 *x, hyp_nodenr nodenr)
{
	(void)x;
	(void)nodenr;
	return 0;
}


void x16768(struct x76 *x)
{
	(void)x;
}

FILE *x14f38(const Path *filename)
{
	(void) filename;
	conv_all_nodenames(0);
	return 0;
}

#ifdef __GNUC__
void conv_nodename(unsigned char os, char *name)
{
	(void) os;
	(void) name;
}
#endif
