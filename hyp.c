#include "hyp2gdos.h"
#include <errno.h>
#include "lh5.h"

int hyp_errno;
char hypfold[MAXPATH];
char unused_path1[MAXPATH];
char unused_path2[MAXPATH];
char all_ref[MAXPATH];

static char emptystr[] = "";

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

FILE *hyp_find_in_hypfold(Path *filename)
{
	Path dirname;
	Path basename;
	FILE *fp;
	
	if (is_relative_path(filename->buf))
	{
		get_basename(filename->buf, basename.buf);
		append_path(hypfold, basename.buf, filename->buf);
	}
	fp = myfopen(filename->buf, "rb");
	if (fp == NULL)
	{
		/* BUG: nonsense, filename has already been modified above */
		get_dirname(filename->buf, dirname.buf);
		get_basename(filename->buf, basename.buf);
		strcat(basename.buf, ".hyp");
		append_path(dirname.buf, basename.buf, filename->buf);
		fp = myfopen(filename->buf, "rb");
	}
	return fp;
}

/* ---------------------------------------------------------------------- */

/* BUG: os should be unsigned */
size_t conv_nodename(char os, char *name)
{
	switch (os)
	{
	case HYP_OS_MAC:
		return conv_macroman(name, name);
	}
	return strlen(name);
}

/* ---------------------------------------------------------------------- */

hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename)
{
	hyp_nodenr i;
	hyp_nodenr ret;
	
	if (*pagename == '\0')
	{
		ret = HYP_NOINDEX;
		return ret;
	}
	for (i = 0; i < hyp->header.num_index; i++)
	{
		if (strcmp(hyp->indextable[i]->name, pagename) == 0)
			return i;
	}
	if (strcmp("Main", pagename) == 0)
		return 0;
	return HYP_NOINDEX;
}

/* ---------------------------------------------------------------------- */

void *hyp_find_extheader(struct hypfile *hyp, hyp_ext_header type)
{
	short *data;
	
	data = hyp->extdata;
	if (data == NULL)
		return NULL;
	while (*data != 0)
	{
		if (*data == (short)type)
			return data + 2;
		data = (short *)((char *)data + 4 + data[1]);
	}
	return NULL;
}

/* ---------------------------------------------------------------------- */

char *decode_char(char *data, short *val)
{
	*val = (unsigned char)*data++ - 1;
	return data;
}

/* ---------------------------------------------------------------------- */

char *dec_255_decode(char *data, short *val)
{
	unsigned short lo;
	
	lo = (unsigned char)*data++ - 1U;
	lo += ((unsigned char)*data++ - 1U) * 255;
	*val = lo;
	return data;
}

/* ---------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------- */

void hyp_free_node(struct hypfile *hyp, struct node **node)
{
	UNUSED(hyp);
	if (node == NULL)
		return;
	if (*node == NULL)
		return;
	(*node)->picdata = NULL;
	(*node)->datalen = 0;
	(*node)->nodenr = HYP_NOINDEX;
	(*node)->dither_params = NULL;
	free(*node);
	*node = NULL;
}

/* ---------------------------------------------------------------------- */

static void prepare_ascii(struct hypfile *hyp, struct node *node)
{
	char *data;
	char *end;
	char *dst;
	
	data = node->data;
	end = data + node->datalen;
	dst = data;
	while (data < end)
	{
		if (*data == 0x0d)
		{
			if (hyp->header.compiler_os == HYP_OS_UNKNOWN)
			{
				hyp->header.compiler_os = data[1] == 0x0a ? HYP_OS_ATARI : HYP_OS_MAC;
			}
			if (data[1] == 0x0a)
				data++;
			*dst++ = '\0';
			data++;
		} else if (*data == 0x0a)
		{
			*dst++ = '\0';
			data++;
		} else
		{
			*dst++ = *data++;
		}
	}
	*dst = '\0';
	node->datalen = dst - node->data;
}

/* ---------------------------------------------------------------------- */

int hyp_load_node(struct hypfile *hyp, struct node **node, hyp_nodenr nodenr)
{
	char *data;
	FILE *fp;
	long offset;
	long compressed_size;
	long uncompressed_size;
	long comp_diff;
	
	data = NULL;
	fp = NULL;
	if (nodenr < 0 || nodenr >= hyp->header.num_index)
	{
		hyp_errno = 1999;
		goto reterror;
	}
	offset = hyp->indextable[nodenr]->seek_offset;
	if (offset < 0)
	{
		hyp_errno = 1998;
		goto reterror;
	}
	/*
	 * BUG: may access illegal entry
	 */
	compressed_size = hyp->indextable[nodenr + 1]->seek_offset - offset;
	comp_diff = hyp->indextable[nodenr]->comp_diff;
	if (hyp->indextable[nodenr]->type == HYP_NODE_IMAGE)
	{
		/* BUG: next is sign-extended */
		comp_diff |= ((long) hyp->indextable[nodenr]->next) << 16;
	}
	uncompressed_size = compressed_size + comp_diff;
again:
	if (compressed_size >= uncompressed_size)
		data = malloc(compressed_size + sizeof(**node) + 1);
	else
		data = xmalloc(compressed_size + sizeof(**node) + 1);
	if (data == NULL)
	{
		hyp_errno = 2;
		goto reterror;
	}
	if (hyp->fp != NULL)
	{
		fp = hyp->fp;
	} else
	{
		fp = myfopen(hyp->filename.buf, "rb");
		if (fp == NULL)
		{
			hyp_errno = errno;
			goto reterror;
		}
		if (hyp->flags & KEEP_OPEN)
			hyp->fp = fp;
	}
	if (fseek(fp, offset, SEEK_SET) != 0 ||
		(long)fread(data + sizeof(**node), 1, compressed_size, fp) != compressed_size)
	{
		hyp_errno = errno;
		goto reterror;
	}
	if (compressed_size >= uncompressed_size)
	{
		*node = (struct node *)data;
		data = NULL;
	} else
	{
		*node = malloc(uncompressed_size + sizeof(**node) + 1);
		if (*node == NULL)
		{
			hyp_errno = 2;
			goto reterror;
		}
		/*
		 * setup globals for decoder
		 */
		lh5_packedMem = (unsigned char *)data + sizeof(**node);
		lh5_compsize = compressed_size;
		lh5_buffer = (unsigned char *)(*node)->data;
		lh5_origsize = uncompressed_size;
		lh5_decode1(TRUE);
		if ((long)lh5_compsize > 0)
		{
			/* not all data decompressed: error */
			free(*node);
			*node = NULL;
			xfree(data);
			data = NULL;
			uncompressed_size += 0x10000L;
			goto again;
		}
	}
	(*node)->picdata = NULL;
	(*node)->datalen = uncompressed_size;
	(*node)->nodenr = nodenr;
	(*node)->dither_params = NULL;
	if (hyp->header.magic != HYP_MAGIC_HYP)
		prepare_ascii(hyp, *node);
	
	hyp_errno = 0;
	goto retok;
	
reterror:
	hyp_free_node(hyp, node);
retok:
	if (hyp->fp == NULL && fp != NULL)
		fclose(fp);
	if (data != NULL)
		xfree(data);
	return hyp_errno;
}

/* ---------------------------------------------------------------------- */

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
	hyp->flags &= ~INDEX_LOADED;
	if (hyp->dither_params != NULL)
	{
		hyp->dither_params = NULL;
		hyp->o302 = NULL;
	}
	if (hyp->extdata != NULL)
	{
		free(hyp->extdata);
		hyp->extdata = NULL;
		hyp->extdatasize = 0;
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
	hyp->num_internal_nodes = 0;
	hyp->help_page = HYP_NOINDEX;
	hyp->main_page = HYP_NOINDEX;
	hyp->index_page = HYP_NOINDEX;
}

/* ---------------------------------------------------------------------- */

int hyp_load(struct hypfile *hyp, const Path *filename)
{
	Path pathbuf;
	void *exth;
	char basename[MAXPATH];
	FILE *fp;
	int len;
	hyp_nodenr i;
	
	if (filename == NULL)
		return 0;
	if (!(hyp->flags & FORCE_RELOAD) && pathequal(&hyp->filename, filename))
		return 0;
	if (hyp->fp != NULL)
	{
		fclose(hyp->fp);
		hyp->fp = NULL;
	}
	pathcopy(&pathbuf, filename);
	fp = hyp_find_in_hypfold(&pathbuf);
	if (fp == NULL)
	{
		hyp_errno = errno;
		goto reterror;
	}
	{
		size_t readlen;
		
		if (!(hyp->flags & FORCE_RELOAD) && pathequal(&hyp->filename, &pathbuf))
		{
			if (hyp->flags & KEEP_OPEN)
			{
				hyp->fp = fp;
			} else
			{
				fclose(fp);
			}
			return 0;
		}
		hyp_free(hyp);
		hyp->flags &= ~FORCE_RELOAD;
		pathcopy(&hyp->filename, &pathbuf);
		get_basename(pathbuf.buf, basename);
		len = (int)strlen(basename) + 1;
		if (len & 1)
			len++;
		readlen = fread(&hyp->header, 1, SIZEOF_HYP_HEADER, fp);
		if (readlen != SIZEOF_HYP_HEADER || hyp->header.magic != HYP_MAGIC_HYP)
		{
			if (readlen >= 4 && hyp->header.magic == HYP_MAGIC_REF)
			{
				/* BUG: why errno? */
				hyp_errno = errno;
				goto reterror;
			}
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
					hyp_errno = 2;
					goto reterror;
				}
			}
		}
		
		hyp->indexdata = malloc(hyp->header.itable_size);
		if (hyp->indexdata == NULL)
		{
			hyp_errno = 2;
			goto reterror;
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
		} else
		{
			if ((long)fread(hyp->indexdata, 1, hyp->header.itable_size, fp) != hyp->header.itable_size)
			{
				hyp_errno = errno;
				goto reterror;
			}
		}
		hyp->indextable = malloc(hyp->header.num_index * sizeof(hyp->indextable[0]));
		if (hyp->indextable == NULL)
		{
			hyp_errno = 2;
			goto reterror;
		}
		hyp->num_internal_nodes = 0;
		{
			INDEX_ENTRY *ptr;
			
			ptr = hyp->indexdata;
			for (i = 0; i < hyp->header.num_index; i++)
			{
				if (ptr->type == HYP_NODE_INTERNAL)
					hyp->num_internal_nodes++;
				hyp->indextable[i] = ptr;
				/* BUG: length sign-extended */
				ptr = (INDEX_ENTRY *)(((char *)ptr + (signed char)ptr->length));
			}
		}
		conv_all_nodenames(hyp);
		hyp->main_page = hyp_find_pagename(hyp, "Main");
		hyp->index_page = hyp_find_pagename(hyp, "Index");
		hyp->help_page = hyp_find_pagename(hyp, "Help");
		if (hyp->header.magic == HYP_MAGIC_HYP)
		{
			long minoffset;
			long firstoffset;
			long offset;
			
			minoffset = hyp->header.itable_size + SIZEOF_HYP_HEADER;
			firstoffset = 0x7fffffffL;
			
			/*
			 * FIXME: seek offsets have to be in ascending order anyway,
			 * so no need to loop through it
			 */
			for (i = 0; i < hyp->header.num_index; i++)
			{
				offset = hyp->indextable[i]->seek_offset;
				if (offset >= minoffset && offset < firstoffset)
					firstoffset = offset;
			}
			hyp->extdatasize = firstoffset - minoffset;
			if (hyp->extdatasize > 0)
			{
				hyp->extdata = malloc(hyp->extdatasize);
				if (hyp->extdata == NULL)
				{
					hyp_errno = 2;
					goto reterror;
				}
				if ((long)fread(hyp->extdata, 1, hyp->extdatasize, fp) != hyp->extdatasize)
				{
					/* BUG: a short read does not set errno */
					hyp_errno = errno;
					goto reterror;
				}
			}
		}
	}
	
	if ((exth = hyp_find_extheader(hyp, HYP_EXTH_WIDTH)) != NULL)
		hyp->width = *((unsigned char *)exth);
	else
		hyp->width = 75;
	if ((exth = hyp_find_extheader(hyp, HYP_EXTH_HELP)) != NULL)
		hyp->help_page = hyp_find_pagename(hyp, exth);
	hyp->flags |= INDEX_LOADED;
	hyp_errno = 0;
	if (hyp->flags & KEEP_OPEN)
	{
		hyp->fp = fp;
	} else
	{
		fclose(fp);
	}
	return hyp_errno;
reterror:
	hyp_free(hyp);
	if (fp)
		fclose(fp);
	return hyp_errno;
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
		hyp_errno = 2;
		goto reterror;
	} else
	{
		memset(hyp, 0, sizeof(*hyp));
		clearpath(&hyp->filename);
		hyp_errno = 0;
		return hyp;
	}
reterror:
	hyp_delete(&hyp);
	return hyp;
}

/* ---------------------------------------------------------------------- */

static struct history *alloc_history(void)
{
	struct history *p;
	
	p = malloc(sizeof(*p));
	if (p != NULL)
	{
		memset(p, 0, sizeof(*p));
		p->curr = HISTSIZE - 1;
		p->size = 0;
	}
	return p;
}

/* ---------------------------------------------------------------------- */

static void free_history(struct history *p)
{
	if (p)
		free(p);
}

/* ---------------------------------------------------------------------- */

static long get_real_lineno(struct pageinfo *page, long y);

static void add_history(struct history *hist, struct pageinfo *page)
{
	if (page == NULL)
		return;
	if (page->node == NULL)
		return;
	if (hist->size < HISTSIZE)
		hist->size++;
	hist->curr++;
	if (hist->curr >= HISTSIZE)
		hist->curr = 0;
	pathcopy(&hist->entry[hist->curr].filename, &page->hyp->filename);
	hist->entry[hist->curr].nodenr = page->node->nodenr;
	strcpy(hist->entry[hist->curr].window_title, page->window_title);
	hist->entry[hist->curr].lineno = get_real_lineno(page, page->lineno);
	hist->entry[hist->curr].o518 = page->o60;
	set_lrect(hist->entry[hist->curr].o522, 0, 0, 0, 0);
}

/* ---------------------------------------------------------------------- */

static long get_real_lineno(struct pageinfo *page, long y)
{
	long z;
	
	if (page->image_lines != 0)
	{
		if (y > page->num_lines)
			y = page->num_lines;
		z = y - 1;
		while (z >= 0)
		{
			if (page->text[z] == emptystr)
				--y;
			--z;
		}
	}
	return y;
}

/* ---------------------------------------------------------------------- */

typedef int (*cb)(Path *modulename, char *nodename, long lineno, char *entryname);

static int scan_ref(struct hypfile *hyp, const char *reffilename, char *data, cb callback, Path *modulename, char *errmsg, long *lineno)
{
	int os;
	long count;
	char *nodename;
	long i;
	Path filename;
	
	*errmsg = '\0';
	os = HYP_OS_ATARI;
	clearpath(modulename);
	count = ((const long *)data)[1];
	data += 2 * sizeof(long);
	nodename = NULL;
	for (i = 0; i < count; i++)
	{
		switch (*data)
		{
		case REF_OS:
			os = data[2];
			break;
		case REF_FILENAME:
			if (hyp != NULL)
			{
				get_dirname(reffilename, filename.buf);
				append_path(filename.buf, data + 2, modulename->buf);
			} else
			{
				get_name(modulename->buf, data + 2);
			}
			break;
		case REF_NODENAME:
			conv_nodename(os, data + 2);
			nodename = data + 2;
			if (ref_strcmp(nodename, &nodename[strlen(nodename)]) == 0)
				goto do_callback;
			break;
		case REF_ALIASNAME:
			conv_nodename(os, data + 2);
			if (ref_strcmp(data + 2, &data[strlen(data + 2)] + 2) == 0)
				goto do_callback;
			break;
		case REF_LABELNAME:
			conv_nodename(os, data + 2);
			if (ref_strcmp(data + 2, &data[strlen(data + 2)] + 2) == 0)
			{
				char *lineno_pos;

				lineno_pos = data + data[1] - 2;
				if (lineno != NULL)
				{
					*lineno = (((unsigned char)lineno_pos[0]) << 8) | ((unsigned char)lineno_pos[1]);
				}
			do_callback:
				if (callback == NULL)
					goto reterr;
				{
					int err = callback(modulename, nodename, lineno != NULL ? *lineno : 0, data + 2);
					if (err == 1423)
						goto reterr;
					if ((hyp_errno = err) != 0)
						return err;
				}
			}
			break;
		}
		/* BUG: length sign-extended */
		data += data[1];
	}
	return 0;
reterr:
	strcpy(errmsg, nodename);
	hyp_errno = 0;
	return 1423;
}

/* ---------------------------------------------------------------------- */

static int search_ref(struct hypfile *hyp, const char *filename, _BOOL caseinsens, _BOOL d3, char *name, cb callback, int (*cancel)(void), Path *modulename, char *errmsg, long *lineno)
{
	FILE *fp;
	char *data;
	int err;
	long magic;
	long refhead[2];
	
	fp = NULL;
	data = NULL;
	*errmsg = '\0';
	if (lineno != NULL)
		*lineno = 0;
	if ((err = set_ref_string(name, caseinsens, d3)) != 0)
		return err;
	if (hyp != NULL)
	{
		hyp_nodenr i;
		hyp_nodenr found;
		
		found = HYP_NOINDEX;
		for (i = 0; i < hyp->header.num_index && found == HYP_NOINDEX; i++)
		{
			switch (hyp->indextable[i]->type)
			{
			case 20: /* WTF? what's that ? */
				found = i;
				break;
			}
		}
		if (found != HYP_NOINDEX)
		{
			struct node *node;

			err = hyp_load_node(hyp, &node, found);
			if (err == 0)
			{
				data = node->data + 4;
				err = scan_ref(hyp, filename, data, callback, modulename, errmsg, lineno);
				hyp_free_node(hyp, &node);
			}
			if (err == 1423)
				goto retok;
			if ((hyp_errno = err) != 0)
				goto reterr;
			goto retok;
		}
	}
	
	fp = myfopen(filename, "rb");
	if (fp == NULL)
	{
		hyp_errno = errno;
		goto reterr;
	}
		
	if (fread(&magic, 1, sizeof(magic), fp) != sizeof(magic))
	{
		hyp_errno = errno;
		goto reterr;
	}
	
	if (magic != HYP_MAGIC_REF)
	{
		hyp_errno = 1996;
		goto reterr;
	}
	for (;;)
	{
		if (cancel != NULL && (err = cancel()) != 0)
		{
			hyp_errno = err;
			goto reterr;
		}
		if (fread(refhead, 1, sizeof(refhead), fp) != sizeof(refhead))
		{
			hyp_errno = errno;
			goto reterr;
		}
		if (refhead[0] == 0 && refhead[1] == 0)
			break;
		data = xmalloc(refhead[0] + sizeof(refhead));
		if (data == NULL)
		{
			hyp_errno = 2;
			goto reterr;
		}
		memcpy(data, refhead, sizeof(refhead));
		if ((long)fread(data + sizeof(refhead), 1, refhead[0], fp) != refhead[0])
		{
			hyp_errno = errno;
			goto reterr;
		}
		err = scan_ref(hyp, filename, data, callback, modulename, errmsg, lineno);
		if (err == 1423)
			goto retok;
		if ((hyp_errno = err) != 0)
			goto reterr;
		xfree(data);
	}
retok:
	hyp_errno = 0;
reterr:
	if (data != NULL)
		xfree(data);
	if (fp != NULL)
		fclose(fp);
	return hyp_errno;
}

/* ---------------------------------------------------------------------- */

static char *read_linktext(struct hypfile *hyp, char *data, int *offset)
{
	hyp_nodenr dest_page;
	int len;
	char *name;
	char c;
	
	data = dec_255_decode(data, &dest_page);
	/* BUG: sign-extended */
	len = *data++ - HYP_STRLEN_OFFSET;
	if (len <= 0)
	{
		/* no text; nodename is used */
		name = hyp->indextable[dest_page]->name;
		len = (int)strlen(name);
	} else
	{
		c = data[len];
		data[len] = '\0';
		conv_nodename(hyp->header.compiler_os, data);
		data[len] = c;
		data += len;
	}
	*offset += len;
	return data;
}

/* ---------------------------------------------------------------------- */

static int calc_textlen(struct hypfile *hyp, char *_data)
{
	int len;
	char *start;
	char c;
	char *data;
	
	len = 0;
	data = _data;
	while (*data != 0)
	{
		if (*data != HYP_ESC)
		{
			start = data;
			while (*data != '\0' && *data != HYP_ESC)
				data++;
			c = *data;
			*data = '\0';
			len += (int)conv_nodename(hyp->header.compiler_os, start);
			*data = c;
		} else
		{
			if (hyp->header.magic != HYP_MAGIC_HYP)
			{
				data++;
				len++;
			} else
			{
				data++;
				switch (*data)
				{
				case HYP_ESC_ESC:
					data++;
					len++;
					break;
				
				case HYP_ESC_LINK:
				case HYP_ESC_ALINK:
					data = read_linktext(hyp, data + 1, &len);
					break;
				
				case HYP_ESC_LINK_LINE:
				case HYP_ESC_ALINK_LINE:
					/* ignores target lineno */
					data = read_linktext(hyp, data + 3, &len);
					break;
				
				default:
					data++;
					break;
				}
			}
		}
	}
	return len;
}

/* ---------------------------------------------------------------------- */

static void calc_max_textlen(struct pageinfo *page)
{
	hyp_nodenr i;
	int maxlen;
	int len;
	
	maxlen = page->max_text_width;
	for (i = 0; i < page->num_lines; i++)
	{
		len = calc_textlen(page->hyp, page->text[i]);
		if (len > maxlen)
			maxlen = len;
	}
	page->max_text_width = maxlen;
}

/* ---------------------------------------------------------------------- */

static char *skip_gfx_cmds(struct pageinfo *page, char *data, short *y_offset, short *height)
{
	char *end;
	
	if (data == NULL)
	{
		data = page->node->data;
		*y_offset = -1;
	}
	end = page->node->data + page->node->datalen;
	if (page->hyp->header.magic == HYP_MAGIC_HYP)
	{
		while (data < end && *data == HYP_ESC)
		{
			switch (data[1])
			{
			case HYP_ESC_PIC:
				if (!(page->hyp->flags & IGNORE_IMAGES) && data[7] != 0 && data[8] == 0)
				{
					dec_255_decode(&data[5], y_offset);
					*height = data[7];
					data += 9;
					return data;
				}
				data += 9;
				break;
			case HYP_ESC_LINE:
			case HYP_ESC_BOX:
			case HYP_ESC_RBOX:
				data += 8;
				break;
			case HYP_ESC_DATA0:
			case HYP_ESC_DATA1:
			case HYP_ESC_DATA2:
			case HYP_ESC_DATA3:
			case HYP_ESC_DATA4:
			case HYP_ESC_DATA5:
			case HYP_ESC_DATA6:
			case HYP_ESC_DATA7:
			case HYP_ESC_EXTERNAL_REFS:
				/* BUG: sign-extended */
				data += data[2];
				break;
			case HYP_ESC_WINDOWTITLE:
				data += 2;
				data += strlen(data) + 1;
				break;
			case HYP_ESC_OBJTABLE:
				data += 10;
				break;
			default:
				*y_offset = -1;
				return data;
			}
		}
	}
	*y_offset = -1;
	return data;
}

/* ---------------------------------------------------------------------- */

static int prepare_page(struct pageinfo *page)
{
	char *data;
	char *end;
	int ypos;
	int num_lines;
	struct node *tmpnode;
	struct node *node;
	char *picdata;
	char *starttext;
	hyp_nodenr picnode;
	short y_offset;
	char *gfx;
	short y;
	short h;
	short dithermask;
	int i;
	
	tmpnode = page->node;
	page->window_title = page->hyp->indextable[tmpnode->nodenr]->name;
	ypos = 0;
	data = tmpnode->data;
	end = data + tmpnode->datalen;
	node = tmpnode;
	dithermask = 0xfefe;
	
	if (page->hyp->header.magic == HYP_MAGIC_HYP)
	{
		while (data < end && *data == HYP_ESC)
		{
			switch (data[1])
			{
			case HYP_ESC_DITHERMASK:
				dec_255_decode(data + 3, &dithermask);
				/* BUG: sign-extended */
				data += data[2];
				continue;
			
			case HYP_ESC_PIC:
				if (!(page->hyp->flags & IGNORE_IMAGES))
				{
					long planesize;
					long picsize;
					unsigned long endoffset;
					unsigned long diff;
					char *picend;
					short width;
					short height;
					
					dec_255_decode(data + 2, &picnode);
					if (hyp_load_node(page->hyp, &node->picdata, picnode) != 0)
						return 1;
					picdata = node->picdata->data;
					planesize = ((*((short *)picdata) + 15) / 16) * 2; /* width */
					planesize *= *((short *)(picdata + 2)); /* height */
					picsize = planesize * picdata[4]; /* planes */
					endoffset = picsize + 8;
					diff = node->picdata->datalen - endoffset;
					picend = node->picdata->data + endoffset;
					if (diff < 8 || *((long *)picend) != 0x4849434CL) /* 'HICL' */
						picend = NULL;
					if (picdata[4] > 1)
						node->picdata->dither_params = picend;
					node = node->picdata;
					picdata = node->data;
					if (picdata[4] > 1)
					{
						if (x18118(picdata + 8, *((short *)picdata), *((short *)(picdata + 2)), picdata[4]) != 0)
						{
							hyp_errno = 2;
							return 1;
						}
					}
					dec_255_decode(data + 5, &y_offset);
					dec_255_encode(data + 5, y_offset + ypos);
					width = *((short *)picdata);
					height = *((short *)(picdata + 2));
					if (page->hyp->flags & SCALE_IMAGES)
					{
						width = (width / HYP_PIC_FONTW) * page->font_width + *((short *)picdata) % HYP_PIC_FONTW;
						height = (height / HYP_PIC_FONTH) * page->font_height + *((short *)(picdata + 2)) % HYP_PIC_FONTH;
					}
					height = (height + page->font_height - 1) / page->font_height;
					if (data[7] != 0 && data[8] == 0 && !(page->hyp->flags & FLAG_04)) /* picture dimensions in chars */
					{
						data[7] = height;
						ypos += height;
					}
				}
				data += 9;
				continue;
			
			case HYP_ESC_LINE:
			case HYP_ESC_BOX:
			case HYP_ESC_RBOX:
				dec_255_decode(data + 3, &y_offset);
				dec_255_encode(data + 3, y_offset + ypos);
				data += 8;
				continue;

			case HYP_ESC_EXTERNAL_REFS:
				conv_nodename(page->hyp->header.compiler_os, data + 5);
				/* BUG: sign-extended */
				data += data[2];
				continue;
				
			case HYP_ESC_DATA0:
			case HYP_ESC_DATA1:
			case HYP_ESC_DATA2:
			case HYP_ESC_DATA3:
			case HYP_ESC_DATA4:
			case HYP_ESC_DATA5:
			case HYP_ESC_DATA6:
				/* BUG: sign-extended */
				data += data[2];
				continue;

			case HYP_ESC_WINDOWTITLE:
				data += 2;
				page->window_title = data;
				data += conv_nodename(page->hyp->header.compiler_os, data) + 1;
				continue;
			
			case HYP_ESC_OBJTABLE:
				data += 10;
				continue;
			}
			break;
		}
	}
	page->image_lines = ypos;
	num_lines = ypos;
	starttext = data;
	while (data < end)
	{
		num_lines++;
		while (*data != HYP_EOL)
			data++;
		data++;
	}
	
	page->text = malloc(num_lines * sizeof(page->text[0]));
	if (page->text == NULL)
	{
		hyp_errno = 2;
		return 1;
	}
	page->num_lines = num_lines;
	page->max_text_width = 0;
	if (ypos != 0)
	{
		gfx = skip_gfx_cmds(page, NULL, &y, &h);
		data = starttext;
		for (i = 0; i < num_lines; i++)
		{
			while (y == i)
			{
				while (--h >= 0)
				{
					page->text[i] = emptystr;
					i++;
				}
				if (i >= num_lines)
					return 0;
				gfx = skip_gfx_cmds(page, gfx, &y, &h);
			}
			page->text[i] = data;
			while (*data != HYP_EOL)
				data++;
			data++;
		}
	} else
	{
		data = starttext;
		for (i = 0; i < num_lines; i++)
		{
			page->text[i] = data;
			while (*data != HYP_EOL)
				data++;
			data++;
		}
	}
	
	return 0;
}

/* ---------------------------------------------------------------------- */

static void free_pageinfo(struct pageinfo *page)
{
	struct node *node;
	struct node *next;
	
	if (page == NULL)
		return;
	if (page->text != NULL)
		free(page->text);
	page->text = NULL;
	page->num_lines = 0;
	page->max_text_width = 0;
	set_lrect(page->o22, 0, 0, 0, 0);
	set_lrect(page->o38, 0, 0, 0, 0);
	page->lineno = 0;
	page->o60 = 0;
	if (page->errmsg != NULL)
		free(page->errmsg);
	page->errmsg = NULL;
	node = page->node;
	while (node != NULL)
	{
		next = node->picdata;
		hyp_free_node(page->hyp, &node);
		node = next;
	}
	page->node = NULL;
}

/* ---------------------------------------------------------------------- */

static int reset_pageinfo(struct pageinfo *page, hyp_nodenr node)
{
	struct hypfile *hyp;
	short font_width;
	short font_height;
	struct history *hist;
	
	if (page != NULL)
	{
		hyp = page->hyp;
		font_width = page->font_width;
		font_height = page->font_height;
		hist = page->hist;
		memset(page, 0, sizeof(*page));
		page->hyp = hyp;
		page->font_width = font_width;
		page->font_height = font_height;
		page->hist = hist;
		if (hyp_load_node(hyp, &page->node, node) != 0 ||
			prepare_page(page) != 0)
		{
			free_pageinfo(page);
			return hyp_errno;
		}
		calc_max_textlen(page);
		set_lrect(page->o22, 0, 0, page->num_lines - 1, hyp_get_linewidth(page, page->num_lines - 1));
		set_lrect(page->o38, 0, 0, 0, 0);
		page->lineno = 0;
		page->o60 = 0;
		return 0;
	}
	hyp_errno = 1;
	free_pageinfo(page);
	return hyp_errno;
}

/* ---------------------------------------------------------------------- */

static int load_page(struct pageinfo *page, const Path *filename, const char *pagename, hyp_nodenr node, _BOOL addhist, long *lineno)
{
	char hypfile[MAXPATH];
	Path newpath;
	FILE *fp;
	char errmsg[256];
	char *name;
	char *slash;

	hyp_errno = 1;
	if (page != NULL)
	{
		if (addhist && page->hist != NULL)
			add_history(page->hist, page);
		if (filename != NULL)
		{
			free_pageinfo(page);
			if (hyp_load(page->hyp, filename) != 0)
			{
				free_pageinfo(page);
				return hyp_errno;
			}
		}
		if (pagename != NULL)
			node = hyp_find_pagename(page->hyp, pagename);
		if (node == -11)
			node = page->hyp->main_page;
		if (page->node != NULL && page->node->nodenr == node)
			return 0;
		switch (get_nodetype(page->hyp, node))
		{
		case HYP_NODE_INTERNAL:
		case HYP_NODE_POPUP:
		case HYP_NODE_EXTERNAL_REF:
			break;
		default:
			return 2;
		}
		free_pageinfo(page);
		if (page->hyp->indextable[node]->type == HYP_NODE_EXTERNAL_REF)
		{
			name = page->hyp->indextable[node]->name;
			
			if ((slash = strchr(name, '/')) == NULL)
			{
				/*
				 * just a name; can be either filename or nodename
				 */
				setpath(name, newpath.buf);
				if ((fp = hyp_find_in_hypfold(&newpath)) != NULL)
				{
					/*
					 * was a valid filename
					 */
					fclose(fp);
					free_pageinfo(page);
					if (hyp_load(page->hyp, &newpath) != 0)
					{
						free_pageinfo(page);
						return hyp_errno;
					}
					node = page->hyp->main_page;
				} else
				{
					if (search_ref(NULL, all_ref, FALSE, FALSE, name, 0, 0, &newpath, errmsg, lineno) == 0 && *errmsg != '\0')
					{
						free_pageinfo(page);
						if (hyp_load(page->hyp, &newpath) != 0)
						{
							free_pageinfo(page);
							return hyp_errno;
						}
						node = hyp_find_pagename(page->hyp, errmsg);
					} else
					{
						hyp_errno = 1997;
						free_pageinfo(page);
						return hyp_errno;
					}
				}
			} else
			{
				strcpy(errmsg, slash + 1);
				strncpy(hypfile, name, MAXPATH - 1);
				hypfile[slash - name] = '\0';
				get_name(newpath.buf, hypfile);
				free_pageinfo(page);
				if (hyp_load(page->hyp, &newpath) != 0)
				{
					free_pageinfo(page);
					return hyp_errno;
				}
				node = hyp_find_pagename(page->hyp, errmsg);
				if (node < 0)
				{
					page->errmsg = strdup(errmsg);
					hyp_errno = 1;
					return hyp_errno;
				}
			}
		}
		
		if (reset_pageinfo(page, node) != 0)
		{
			free_pageinfo(page);
			return hyp_errno;
		}
		if (lineno != NULL)
		{
			if (*lineno < 0 || *lineno >= page->num_lines)
				*lineno = 0;
		}
		hyp_errno = 0;
		return hyp_errno;
	}
	free_pageinfo(page);
	return hyp_errno;
}

/* ---------------------------------------------------------------------- */

int hyp_load_page(struct pageinfo *page, const Path *filename, hyp_nodenr node, _BOOL addhist, long *lineno)
{
	return load_page(page, filename, NULL, node, addhist, lineno);
}

/* ---------------------------------------------------------------------- */

static char *get_linktext(struct hypfile *hyp, char *data, char **text, long *plen)
{
	short node;
	int len;
	
	data = dec_255_decode(data, &node);
	/* BUG: sign-extended */
	len = *data++ - HYP_STRLEN_OFFSET;
	if (len <= 0)
	{
		char *name = hyp->indextable[node]->name;
		*text = name;
		len = (int)strlen(name);
	} else
	{
		*text = data;
		data += len;
	}
	*plen = len;
	return data;
}

/* ---------------------------------------------------------------------- */

static char get_char_at(struct hypfile *hyp, char *data, long offset)
{
	char *start;
	long len;
	
	if (offset < 0)
		return HYP_EOL;
	while (*data != HYP_EOL)
	{
		if (*data != HYP_ESC)
		{
			start = data;
			while (*data != HYP_EOL && *data != HYP_ESC)
				data++;
			len = data - start;
		} else
		{
			if (hyp->header.magic != HYP_MAGIC_HYP)
			{
				start = data;
				data++;
				len = 1;
			} else
			{
				data++;
				switch (*data)
				{
				case HYP_ESC_ESC:
					start = data;
					data++;
					len = 1;
					break;
				
				case HYP_ESC_LINK:
				case HYP_ESC_ALINK:
					data = get_linktext(hyp, data + 1, &start, &len);
					break;
					
				case HYP_ESC_LINK_LINE:
				case HYP_ESC_ALINK_LINE:
					data = get_linktext(hyp, data + 3, &start, &len);
					break;
				
				default:
					data++;
					continue;
				}
			}
		}
		if ((offset -= len) < 0)
		{
			return *(start + offset + len);
		}
	}
	return HYP_EOL;
}

/* ---------------------------------------------------------------------- */

char hyp_get_char_at(struct pageinfo *page, long lineno, long offset)
{
	return get_char_at(page->hyp, page->text[lineno], offset);
}

/* ---------------------------------------------------------------------- */

long hyp_get_linewidth(struct pageinfo *page, long lineno)
{
	char *line;
	long len;
	char *data;
	char *text;
	long linklen;
	
	line = page->text[lineno];
	len = 0;
	data = line;
	while (*data != HYP_EOL)
	{
		if (*data != HYP_ESC)
		{
			char *start = data;
			while (*data != HYP_EOL && *data != HYP_ESC)
				data++;
			len += data - start;
		} else
		{
			if (page->hyp->header.magic != HYP_MAGIC_HYP)
			{
				data++;
				len++;
			} else
			{
				data++;
				switch (*data)
				{
				case HYP_ESC_ESC:
					data++;
					len++;
					break;
				
				case HYP_ESC_LINK:
				case HYP_ESC_ALINK:
					data = get_linktext(page->hyp, data + 1, &text, &linklen);
					len += linklen;
					break;
					
				case HYP_ESC_LINK_LINE:
				case HYP_ESC_ALINK_LINE:
					data = get_linktext(page->hyp, data + 3, &text, &linklen);
					len += linklen;
					break;
				
				default:
					data++;
					break;
				}
			}
		}
	}
	return len;
}

/* ---------------------------------------------------------------------- */

int hyp_init_pageinfo(struct pageinfo *page, struct hypfile *hyp, _WORD font_width, _WORD font_height, _BOOL history)
{
	memset(page, 0, sizeof(*page));
	page->hyp = hyp;
	page->font_width = font_width;
	page->font_height = font_height;
	if (history)
		page->hist = alloc_history();
	return 0;
}

/* ---------------------------------------------------------------------- */

void hyp_free_pageinfo(struct pageinfo *page)
{
	free_pageinfo(page);
	free_history(page->hist);
	page->hyp = NULL;
}

/* ---------------------------------------------------------------------- */

static int tree_has_title(struct hypfile *hyp, char *treedata, hyp_nodenr nodenr)
{
	short *head;
	int nodes;
	const short *bitdata;
	
	/*
	 * if extension not present, have to scan nodes
	 */
	if (treedata == NULL)
		return -1;
	/* skip back to extension header */
	head = (short *)(treedata - 4);
	/*
	 * extension present, but empty: no window titles present
	 */
	if (head[1] == 0)
		return 0;
	nodes = (head[1] - 4) << 3;
	if (nodes > hyp->header.num_index)
		nodes = hyp->header.num_index;
	if (nodenr < 0 || nodenr >= nodes)
		return 0;
	/* skip title length field */
	bitdata = (const short *)(treedata + 4);
	return (bitdata[nodenr >> 4] & (1 << (nodenr & 15))) != 0;
}

/* ---------------------------------------------------------------------- */

char *hyp_get_window_title(struct pageinfo *page, hyp_nodenr nodenr)
{
	if (get_nodetype(page->hyp, nodenr) != HYP_NODE_INTERNAL)
		return NULL;
	if (tree_has_title(page->hyp, hyp_find_extheader(page->hyp, HYP_EXTH_TREEHEADER), nodenr) == 0)
		return page->hyp->indextable[nodenr]->name;
	if (page->node != NULL)
	{
		if (page->node->nodenr == nodenr)
			return page->window_title;
		free_pageinfo(page);
	}
	if (reset_pageinfo(page, nodenr) != 0)
		return NULL;
	return page->window_title;
}

/* ---------------------------------------------------------------------- */

hyp_nodenr hyp_next_textnode(struct hypfile *hyp, hyp_nodenr node, int direction)
{
	if (direction)
	{
		/* scan forward */
		if (node == HYP_NOINDEX)
			node = -1;
		for (;;)
		{
			++node;
			if (node >= hyp->header.num_index)
				break;
			if (get_nodetype(hyp, node) == HYP_NODE_INTERNAL)
				return node;
		}
	} else
	{
		/* scan backward */
		if (node == HYP_NOINDEX)
			node = hyp->header.num_index;
		for (;;)
		{
			--node;
			if (node < 0)
				break;
			if (get_nodetype(hyp, node) == HYP_NODE_INTERNAL)
				return node;
		}
	}
	return HYP_NOINDEX;
}
