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
	if (!(hyp->flags & FLAG_20) && pathequal(&hyp->filename, filename))
		return 0;
	if (hyp->fp != NULL)
	{
		fclose(hyp->fp);
		hyp->fp = NULL;
	}
	pathcopy(&pathbuf, filename);
	fp = x14f38(&pathbuf);
	if (fp == NULL)
	{
		hyp_errno = errno;
		goto reterror;
	}
	{
		size_t readlen;
		
		if (!(hyp->flags & FLAG_20) && pathequal(&hyp->filename, &pathbuf))
		{
			if (hyp->flags & FLAG_40)
			{
				hyp->fp = fp;
			} else
			{
				fclose(fp);
			}
			return 0;
		}
		hyp_free(hyp);
		hyp->flags &= ~FLAG_20;
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
	
	/* 1569e */
	if ((exth = hyp_find_extheader(hyp, HYP_EXTH_WIDTH)) != NULL)
		hyp->width = *((unsigned char *)exth);
	else
		hyp->width = 75;
	if ((exth = hyp_find_extheader(hyp, HYP_EXTH_HELP)) != NULL)
		hyp->help_page = hyp_find_pagename(hyp, exth);
	hyp->flags |= FLAG_80;
	hyp_errno = 0;
	if (hyp->flags & FLAG_40)
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

struct x21a4 *x15774(void)
{
	struct x21a4 *p;
	
	p = malloc(sizeof(*p));
	if (p != NULL)
	{
		memset(p, 0, sizeof(*p));
		p->o2 = 15;
		p->o0 = 0;
	}
	return p;
}

/* ---------------------------------------------------------------------- */

void x157a0(struct x21a4 *p)
{
	if (p)
		free(p);
}

/* ---------------------------------------------------------------------- */

static long x158c4(struct x76 *x, long y);

static void x157b0(struct x21a4 *o68, struct x76 *x)
{
	if (x == NULL)
		return;
	if (x->o4 == NULL)
		return;
	if (o68->o0 < 16)
		o68->o0++;
	o68->o2++;
	if (o68->o2 >= 16)
		o68->o2 = 0;
	pathcopy(&o68->o4[o68->o2].filename, &x->hyp->filename);
	o68->o4[o68->o2].o256 = x->o4->nodenr;
	strcpy(o68->o4[o68->o2].o258, x->window_title);
	o68->o4[o68->o2].o514 = x158c4(x, x->o56);
	o68->o4[o68->o2].o518 = x->o60;
	x183a6(o68->o4[o68->o2].o522, 0, 0, 0, 0);
}

/* ---------------------------------------------------------------------- */

static char emptystr[] = "";

static long x158c4(struct x76 *x, long y)
{
	long z;
	
	if (x->o16 != 0)
	{
		if (y > x->max_text_width)
			y = x->max_text_width;
		z = y - 1;
		while (z >= 0)
		{
			if (x->text[z] == emptystr)
				--y;
			--z;
		}
	}
	return y;
}

/* ---------------------------------------------------------------------- */

typedef int (*cb)(Path *modulename, char *nodename, long lineno, char *entryname);

static int x15900(struct hypfile *hyp, const char *reffilename, char *data, cb o308, Path *modulename, char *errmsg, long *lineno)
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
			if (x1837c(nodename, &nodename[strlen(nodename)]) == 0)
				goto do_callback;
			break;
		case REF_ALIASNAME:
			conv_nodename(os, data + 2);
			if (x1837c(data + 2, &data[strlen(data + 2)] + 2) == 0)
				goto do_callback;
			break;
		case REF_LABELNAME:
			conv_nodename(os, data + 2);
			if (x1837c(data + 2, &data[strlen(data + 2)] + 2) == 0)
			{
				char *lineno_pos;

				lineno_pos = data + data[1] - 2;
				if (lineno != NULL)
				{
					*lineno = (((unsigned char)lineno_pos[0]) << 8) | ((unsigned char)lineno_pos[1]);
				}
			do_callback:
				if (o308 == NULL)
					goto reterr;
				{
					int err = o308(modulename, nodename, lineno != NULL ? *lineno : 0, data + 2);
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

static int x15a8c(struct hypfile *hyp, const char *filename, int d5, int d3, char *o48, cb o52, int (*o56)(void), Path *modulename, char *errmsg, long *lineno)
{
	FILE *fp;
	char *a3;
	int err;
	long magic;
	long refhead[2];
	
	fp = NULL;
	a3 = NULL;
	*errmsg = '\0';
	if (lineno != NULL)
		*lineno = 0;
	if ((err = x18352(o48, d5, d3)) != 0)
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
			struct xo4 *o16;

			err = x15132(hyp, &o16, found);
			if (err == 0)
			{
				a3 = o16->data + 4;
				err = x15900(hyp, filename, a3, o52, modulename, errmsg, lineno);
				x1509e(hyp, &o16);
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

		if (o56 != NULL && (err = o56()) != 0)
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
		a3 = xmalloc(refhead[0] + sizeof(refhead));
		if (a3 == NULL)
		{
			hyp_errno = 2;
			goto reterr;
		}
		memcpy(a3, refhead, sizeof(refhead));
		if ((long)fread(a3 + sizeof(refhead), 1, refhead[0], fp) != refhead[0])
		{
			hyp_errno = errno;
			goto reterr;
		}
		err = x15900(hyp, filename, a3, o52, modulename, errmsg, lineno);
		if (err == 1423)
			goto retok;
		if ((hyp_errno = err) != 0)
			goto reterr;
		xfree(a3);
	}
retok:
	hyp_errno = 0;
reterr:
	if (a3 != NULL)
		xfree(a3);
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
			len += conv_nodename(hyp->header.compiler_os, start);
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

static void calc_max_textlen(struct x76 *x)
{
	hyp_nodenr i;
	int maxlen;
	int len;
	
	maxlen = x->max_text_width;
	for (i = 0; i < x->num_lines; i++)
	{
		len = calc_textlen(x->hyp, x->text[i]);
		if (len > maxlen)
			maxlen = len;
	}
	x->max_text_width = maxlen;
}

/* ---------------------------------------------------------------------- */

static char *skip_gfx_cmds(struct x76 *x, char *data, short *y_offset, short *width)
{
	char *end;
	
	if (data == NULL)
	{
		data = x->o4->data;
		*y_offset = -1;
	}
	end = x->o4->data + x->o4->datalen;
	if (x->hyp->header.magic == HYP_MAGIC_HYP)
	{
		while (data < end && *data == HYP_ESC)
		{
			switch (data[1])
			{
			case HYP_ESC_PIC:
				if (!(x->hyp->flags & IGNORE_IMAGES) && data[7] != 0 && data[8] == 0)
				{
					dec_255_decode(&data[5], y_offset);
					*width = data[7];
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

static int x15ebc(struct x76 *x)
{
	char *data;
	char *end;
	int ypos;
	int d4;
	struct xo4 *a0;
	struct xo4 *a5;
	char *picdata;
	char *starttext; /* o14 */
	hyp_nodenr picnode; /* o12 */
	short y_offset; /* o10 */
	char *o6;
	short h;
	short y;
	short dithermask; /* o0 */
	
	a0 = x->o4;
	x->window_title = x->hyp->indextable[a0->nodenr]->name;
	ypos = 0;
	data = a0->data;
	end = data + a0->datalen;
	a5 = a0;
	dithermask = 0xfefe;
	
	if (x->hyp->header.magic == HYP_MAGIC_HYP)
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
				if (!(x->hyp->flags & IGNORE_IMAGES))
				{
					long planesize;
					long picsize;
					long diff;
					char *picend;
					short width;
					short height;
					
					dec_255_decode(data + 2, &picnode);
					if (x15132(x->hyp, &a5->picdata, picnode) != 0)
						return 1;
					picdata = a5->picdata->data;
					planesize = ((*((short *)picdata) + 15) / 16) * 2; /* width */
					planesize *= *((short *)(picdata + 2)); /* height */
					picsize = planesize * picdata[4]; /* planes */
					picsize = picsize + 8;
					diff = picsize - a5->picdata->datalen;
					picend = a5->picdata->data + picsize;
					if (diff < 8 || *((long *)picend) != 0x4849434CL) /* 'HICL' */
						picend = NULL;
					if (picdata[4] > 1)
						a5->picdata->o10 = picend;
					a5 = a5->picdata;
					picdata = a5->data;
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
					if (x->hyp->flags & SCALE_IMAGES)
					{
						int scalew = (width / HYP_PIC_FONTW) * x->font_width;
						scalew += *((short *)picdata) % HYP_PIC_FONTW;
						width = scalew; /* ??? */
						width = (height / HYP_PIC_FONTH) * x->font_height;
						width += *((short *)(picdata + 2)) % HYP_PIC_FONTH;
						height = width;
						height = (height + x->font_height - 1) / x->font_height;
						if (data[7] != 0 && data[8] == 0 && !(x->hyp->flags & FLAG_04)) /* picture dimensions in chars */
						{
							data[7] = height;
							ypos += height;
						}
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
				conv_nodename(x->hyp->header.compiler_os, data + 5);
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
				x->window_title = data;
				data += conv_nodename(x->hyp->header.compiler_os, data) + 1;
				continue;
			
			case HYP_ESC_OBJTABLE:
				data += 10;
				continue;
			}
			break;
		}
	}
	x->o16 = ypos;
	d4 = ypos;
	starttext = data;
	while (data < end)
	{
		d4++;
		while (*data != HYP_EOL)
			data++;
		data++;
	}
	
	x->text = malloc(d4 * sizeof(x->text[0]));
	if (x->text == NULL)
	{
		hyp_errno = 2;
		return 1;
	}
	x->num_lines = d4;
	x->max_text_width = 0;
	if (ypos != 0)
	{
		int i;
		
		o6 = skip_gfx_cmds(x, NULL, &y, &h);
		data = starttext;
		i = 0;
		goto nexti;
		{
			do
			{
				x->text[i] = emptystr;
				i++;
			nextline:
				;
			} while (--h >= 0);
			if (i >= d4)
				return 0;
			o6 = skip_gfx_cmds(x, starttext, &y, &h);
			do
			{
				if (y == i)
					goto nextline;
				x->text[i] = data;
				while (*data != HYP_EOL)
					data++;
				data++;
				i++;
			nexti:
				;
			} while (i < d4);
		}
	} else
	{
	}
	
	return 0;
}

/* ---------------------------------------------------------------------- */

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


#ifdef __GNUC__
hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename)
{
	(void) hyp;
	(void) pagename;
	return HYP_NOINDEX;
}
#endif


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
	x157a0(x15774()); /* XXX */
	x157b0(0, 0);
	x15a8c(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	calc_max_textlen(0);
	skip_gfx_cmds(0, 0, 0, 0);
	x15ebc(0);
	dec_255_encode(0, 0);
	return 0;
}


#ifdef __GNUC__
void *hyp_find_extheader(struct hypfile *hyp, hyp_ext_header type)
{
	(void) hyp;
	(void) type;
	return NULL;
}
#endif


#ifdef __GNUC__
int conv_nodename(unsigned char os, char *name)
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


int x18118(char *data, short width, short height, short planes)
{
	UNUSED(data);
	UNUSED(width);
	UNUSED(height);
	UNUSED(planes);
	return 0;
}
