#include "hyp2gdos.h"

struct printinfo {
	/*  0 */ struct vdi *vdi;
	/*  4 */ GRECT text_area;
	/* 12 */ short o12;
	/* 14 */ short line_height;
	/* 16 */ struct layout *layout;
	/* 20 */ struct fontinfo *fonts;
};

struct x40 {
	/*  0 */ GRECT pagearea;
	/*  8 */ GRECT outputarea;
	/* 16 */ _WORD border_left;
	/* 18 */ _WORD border_top;
	/* 20 */ _WORD border_right;
	/* 22 */ _WORD border_bottom;
	/* 24 */ GRECT border_area;
	/* 32 */ GRECT text_area;
};

void x13332(struct printinfo *printinfo);
void print_line(struct printinfo *printinfo, struct pageinfo *page, long lineno, _WORD x, _WORD y);
long x13a2a(long lineno, long d1, long height);

#ifdef __GNUC__
void x13332(struct printinfo *printinfo)
{
	printinfo->line_height = 0;
}
void print_line(struct printinfo *printinfo, struct pageinfo *page, long lineno, _WORD x, _WORD y)
{
	(void) printinfo;
	(void) page;
	(void) lineno;
	(void) x;
	(void) y;
}
long x13a2a(long lineno, long d1, long height)
{
	(void)lineno;
	(void) d1;
	return height;
}
#endif

char udodatabase[LINEMAX];
char udotitle[LINEMAX];

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

static long x13cf8(struct printinfo *printinfo, struct pageinfo *page, long lineno, long d1, long numlines)
{
	(void) printinfo;
	(void) page;
	(void) lineno;
	(void) d1;
	(void) numlines;
	return 0;
}

static char *subst_vars(char *line, const char *format, struct pageinfo *page, _WORD pagenum)
{
	char *lineptr;
	const char *keyword;
	int i;
	char *ext;
	int day;
	int month;
	int year;
	struct pageinfo info;
	
	static const char *const keywords[] = {
		"@database@",
		"@subject@",
		"@version@",
		"@author@",
		"@title@",
		"@prev@",
		"@next@",
		"@toc@",
		"@page@",
		"@date@",
		"@udodatabase@",
		"@udotitle@",
		NULL
	};
	
	lineptr = line;
	while (*format != '\0')
	{
		if (*format != '@')
		{
			*lineptr++ = *format++;
		} else
		{
			keyword = format;
			format++;
			while (*format != '\0' && *format++ != '@')
				;
			if (format - keyword <= 2)
			{
				/* just a single '@' */
				*lineptr++ = '@';
			} else
			{
				for (i = (int)(sizeof(keywords) / sizeof(keywords[0])) - 2; i >= 0; i--)
				{
					if (strnicmp(keywords[i], keyword, format - keyword) == 0)
						break;
				}
				switch (i)
				{
				case 10:
					if (*udodatabase != '\0')
					{
						strcpy(lineptr, udodatabase);
						lineptr += strlen(lineptr);
						break;
					}
					/* fall through */
				case 0:
					if ((ext = hyp_find_extheader(page->hyp, HYP_EXTH_DATABASE)) != NULL)
						goto copyext; /* FIXME */
					break;
				case 1:
					if ((ext = hyp_find_extheader(page->hyp, HYP_EXTH_SUBJECT)) != NULL)
						goto copyext; /* FIXME */
					break;
				case 2:
					if ((ext = hyp_find_extheader(page->hyp, HYP_EXTH_VERSION)) != NULL)
						goto copyext; /* FIXME */
					break;
				case 3:
					if ((ext = hyp_find_extheader(page->hyp, HYP_EXTH_AUTHOR)) != NULL)
					{
					copyext:
						strcpy(lineptr, ext);
						conv_nodename(page->hyp->header.compiler_os, lineptr);
						lineptr += strlen(lineptr);
					}
					break;
				case 11:
					if (*udotitle != '\0')
					{
						strcpy(lineptr, udotitle);
						lineptr += strlen(lineptr);
						break;
					}
					/* fall through */
				case 4:
					strcpy(lineptr, page->window_title);
					lineptr += strlen(lineptr);
					break;
				case 5:
					i = page->hyp->indextable[page->node->nodenr]->previous;
					goto printnr;
				case 6:
					i = page->hyp->indextable[page->node->nodenr]->next;
					goto printnr;
				case 7:
					i = page->hyp->indextable[page->node->nodenr]->toc_index;
				printnr:
					if (i >= 0 && i != page->node->nodenr)
					{
						hyp_init_pageinfo(&info, page->hyp, HYP_PIC_FONTW, HYP_PIC_FONTH, FALSE);
						ext = hyp_get_window_title(&info, i);
						if (ext != NULL)
						{
							strcpy(lineptr, ext);
							lineptr += strlen(lineptr);
						}
						hyp_free_pageinfo(&info);
					}
					break;
				case 8:
					lineptr += sprintf(lineptr, "%d", pagenum);
					break;
				case 9:
					get_date(&day, &month, &year);
					lineptr += sprintf(lineptr, "%d.%d.%d", day, month, year);
					break;
				default:
					lineptr += sprintf(lineptr, "???");
					break;
				}
			}
		}
	}
	*lineptr = '\0';
	return line;
}

/* ---------------------------------------------------------------------- */

static long skip_udo_header(struct pageinfo *page)
{
	long lineno;
	char *str;
	int len;
	int linewidth;
	int i;
	int j;
	char c;
	
	udodatabase[0] = '\0';
	udotitle[0] = '\0';
	lineno = 0;
	if (layout.skip_udo_header && page->num_lines > 0)
	{
		str = page->text[0];
		len = (int)strlen(str);
		if (str[0] == HYP_ESC &&
			str[1] == HYP_ESC_TEXTATTR_FIRST + TXT_UNDERLINED &&
			str[len - 2] == HYP_ESC &&
			str[len - 1] == HYP_ESC_TEXTATTR_FIRST + TXT_NORMAL)
		{
			lineno = 2;
			linewidth = (int)hyp_get_linewidth(page, 0);
			for (i = linewidth - 1; i > 0; --i)
			{
				if (hyp_get_char_at(page, 0, i) == ' ')
				{
					c = hyp_get_char_at(page, 0, i - 1);
					if (c == ' ' ||
						(c == '.' && i > 1 && hyp_get_char_at(page, 0, i - 2) == '.'))
					{
						str = udodatabase;
						for (j = i + 1; j < linewidth; j++)
							*str++ = hyp_get_char_at(page, 0, j);
						*str++ = '\0';
						--i;
						while (i >= 0 && hyp_get_char_at(page, 0, i) == ' ')
							--i;
						str = udotitle;
						for (j = 0; j <= i; j++)
							*str++ = hyp_get_char_at(page, 0, j);
						*str++ = '\0';
						break;
					}
				}
			}
		}
	}
	return lineno;
}

/* ---------------------------------------------------------------------- */

static void calc_text_area(struct printinfo *printinfo, struct x40 *a5, short line_height)
{
	_WORD hdpi;
	_WORD vdpi;
	
	vdi_get_pagesize(vdi_get_handle(printinfo->vdi), &a5->pagearea);
	vdi_get_outputsize(vdi_get_handle(printinfo->vdi), &a5->outputarea);
	vdi_get_dpi(vdi_get_handle(printinfo->vdi), &hdpi, &vdpi);
	a5->border_left = (_WORD)((layout.border_left * hdpi) / 25400L);
	a5->border_top = (_WORD)((layout.border_top * vdpi) / 25400L);
	a5->border_right = (_WORD)((layout.border_right * hdpi) / 25400L);
	a5->border_bottom = (_WORD)((layout.border_bottom * vdpi) / 25400L);
	a5->border_area.g_x = a5->pagearea.g_x + a5->border_left;
	a5->border_area.g_y = a5->pagearea.g_y + a5->border_top;
	a5->border_area.g_w = a5->pagearea.g_w - (a5->border_left + a5->border_right);
	a5->border_area.g_h = a5->pagearea.g_h - (a5->border_top + a5->border_bottom);
	rc_intersect(&a5->outputarea, &a5->border_area);
	a5->text_area = a5->border_area;
	if (layout.add_head)
	{
		a5->text_area.g_y += 2 * line_height;
		a5->text_area.g_h -= 2 * line_height;
	}
	if (layout.add_foot)
	{
		a5->text_area.g_h -= 2 * line_height;
	}
	printinfo->text_area = a5->text_area;
}

/* ---------------------------------------------------------------------- */

static void calc_border_area(struct printinfo *printinfo, struct x40 *a5, _BOOL swap)
{
	if (!layout.swap_layout)
		return;
	a5->border_area.g_x = a5->pagearea.g_x + (swap ? a5->border_right : a5->border_left);
	a5->border_area.g_w = a5->pagearea.g_w - (a5->border_left + a5->border_right);
	rc_intersect(&a5->outputarea, &a5->border_area);
	a5->text_area.g_x = a5->border_area.g_x;
	/* text_area.g_w does not change when swapping layouts */
	printinfo->text_area = a5->text_area;
}

/* ---------------------------------------------------------------------- */

static void print_header(struct printinfo *printinfo, struct pageinfo *page, _WORD pagenum, _BOOL swap, struct x40 *a5)
{
	struct vdi *vdi;
	struct fontinfo *fonts;
	_WORD line_height;
	char left[LINEMAX];
	char center[LINEMAX];
	char right[LINEMAX];
	_WORD pxy[4];
	_WORD y;
	_WORD w;
	
	vdi = printinfo->vdi;
	fonts = printinfo->fonts;
	line_height = printinfo->line_height;
	if (printinfo->layout->skip)
		return;
	if (layout.add_head)
	{
		subst_vars(left, swap ? layout.head_right_str : layout.head_left_str, page, pagenum);
		subst_vars(center, layout.head_center_str, page, pagenum);
		subst_vars(right, swap ? layout.head_left_str : layout.head_right_str, page, pagenum);
		vdi_clip(vdi, &a5->border_area);
		y = a5->border_area.g_y;
		vdi_text_attributes(vdi, fonts->text_color, S_ONLY, 0, fonts->standard_font_id);
		vdi_draw_text(vdi, a5->border_area.g_x, y, left, (int)strlen(left));
		w = vdi_get_textwidth(vdi, right, (int)strlen(right));
		vdi_draw_text(vdi, a5->border_area.g_w - w + a5->border_area.g_x, y, right, (int)strlen(right));
		w = vdi_get_textwidth(vdi, center, (int)strlen(center));
		vdi_draw_text(vdi, (a5->border_area.g_w - w) / 2 + a5->border_area.g_x, y, center, (int)strlen(center));
		y += line_height;
		if (layout.head_sep != 0)
		{
			pxy[0] = a5->border_area.g_x;
			pxy[1] = y + line_height / 2 - 1;
			pxy[2] = a5->border_area.g_x + a5->border_area.g_w /* - 1 */; /* BUG */
			pxy[3] = pxy[1];
			vdi_line_attributes(vdi, fonts->text_color, S_OR_D, layout.head_sep, 1);
			vdi_draw_line(vdi, pxy);
		}
	}
}

/* ---------------------------------------------------------------------- */

static void print_footer(struct printinfo *printinfo, struct pageinfo *page, _WORD pagenum, _BOOL swap, struct x40 *a5)
{
	struct vdi *vdi;
	struct fontinfo *fonts;
	_WORD line_height;
	char left[LINEMAX];
	char center[LINEMAX];
	char right[LINEMAX];
	_WORD pxy[4];
	_WORD y;
	_WORD w;
	
	vdi = printinfo->vdi;
	fonts = printinfo->fonts;
	line_height = printinfo->line_height;
	if (printinfo->layout->skip)
		return;
	if (layout.add_foot)
	{
		subst_vars(left, swap ? layout.foot_right_str : layout.foot_left_str, page, pagenum);
		subst_vars(center, layout.foot_center_str, page, pagenum);
		subst_vars(right, swap ? layout.foot_left_str : layout.foot_right_str, page, pagenum);
		vdi_clip(vdi, &a5->border_area);
		y = a5->border_area.g_y + a5->border_area.g_h - line_height;
		vdi_text_attributes(vdi, fonts->text_color, S_ONLY, 0, fonts->standard_font_id);
		vdi_draw_text(vdi, a5->border_area.g_x, y, left, (int)strlen(left));
		w = vdi_get_textwidth(vdi, right, (int)strlen(right));
		vdi_draw_text(vdi, a5->border_area.g_w - w + a5->border_area.g_x, y, right, (int)strlen(right));
		w = vdi_get_textwidth(vdi, center, (int)strlen(center));
		vdi_draw_text(vdi, (a5->border_area.g_w - w) / 2 + a5->border_area.g_x, y, center, (int)strlen(center));
		if (layout.foot_sep != 0)
		{
			pxy[0] = a5->border_area.g_x;
			pxy[1] = y - line_height / 2;
			pxy[2] = a5->border_area.g_x + a5->border_area.g_w /* - 1 */; /* BUG */
			pxy[3] = pxy[1];
			vdi_line_attributes(vdi, fonts->text_color, S_OR_D, layout.foot_sep, 1);
			vdi_draw_line(vdi, pxy);
		}
	}
}

/* ---------------------------------------------------------------------- */

static void print_text(struct printinfo *printinfo, struct pageinfo *page, long *lineno, long lastline, struct x40 *a5)
{
	GRECT gr;
	struct vdi *vdi;
	struct fontinfo *fonts;
	long numlines;
	_WORD x, y;
	long d6;
	
	vdi = printinfo->vdi;
	fonts = printinfo->fonts;
	numlines = a5->text_area.g_h / printinfo->line_height;
	numlines = x13a2a(*lineno, 0, numlines);
	if (numlines > lastline - *lineno)
		numlines = lastline - *lineno;
	if (!printinfo->layout->skip)
	{
		gr = a5->text_area;
		gr.g_h = (_WORD)(numlines * printinfo->line_height);
		vdi_clip(vdi, &gr);
		x13cf8(printinfo, page, *lineno, 0, numlines);
		if (should_abort())
			return;
	}
	vdi_clip(vdi, &a5->text_area);
	vdi_text_attributes(vdi, fonts->text_color, S_OR_D, 0, page->hyp->header.magic == HYP_MAGIC_HYP || fonts->use_standard ? fonts->standard_font_id : fonts->typewriter_font_id);
	x = a5->text_area.g_x;
	y = a5->text_area.g_y;
	d6 = 0;
	while (d6 < numlines)
	{
		if (should_abort())
			break;
		if (!printinfo->layout->skip)
		{
			print_line(printinfo, page, *lineno, x, y);
		}
		y += printinfo->line_height;
		++(*lineno);
		d6++;
	}
}

/* ---------------------------------------------------------------------- */

static void print_borders(struct printinfo *printinfo, struct x40 *a5)
{
	struct vdi *vdi;
	
	vdi = printinfo->vdi;
	if (printinfo->layout->skip)
		return;
	if (!layout.show_borders)
		return;
	vdi_clip(vdi, &a5->outputarea);
	vdi_line_attributes(vdi, G_BLACK, S_ONLY, LT_DOTTED, 1);
	if (layout.show_borders & 1)
		vdi_draw_rect(vdi, &a5->outputarea);
	if (layout.show_borders & 2)
		vdi_draw_rect(vdi, &a5->border_area);
}

/* ---------------------------------------------------------------------- */

static _BOOL start_page(struct printinfo *printinfo)
{
	if (!printinfo->layout->skip)
	{
		if (x19d24)
			v_clrwk(vdi_get_handle(printinfo->vdi));
		x19d24 = TRUE;
	}
	should_abort();
	vdi_ref(printinfo->vdi);
	/* FIXME: should call vdi_defaults here rather than implicitly in vdi_ref() */
	return TRUE;
}

/* ---------------------------------------------------------------------- */

static void emit_page(struct printinfo *printinfo)
{
	vdi_unref(printinfo->vdi);
	if (!printinfo->layout->skip)
	{
		if (should_abort())
			vdi_clear_page(vdi_get_handle(printinfo->vdi));
		else
			v_updwk(vdi_get_handle(printinfo->vdi));
	}
	should_abort();
}

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

int print_page(struct pageinfo *page, _WORD *page_num, struct fontinfo *fonts)
{
	struct vdi *v;
	struct printinfo printinfo;
	struct x40 a5;
	long lineno;
	long lastline;
	_BOOL swap;
	
	v = vdi_alloc();
	if (v == NULL)
		return 2;
	
	printinfo.vdi = v;
	printinfo.fonts = fonts;
	printinfo.layout = &layout;
	x13332(&printinfo);
	calc_text_area(&printinfo, &a5, printinfo.line_height);
	lineno = skip_udo_header(page);
	if (layout.first_line != 0 && lineno < layout.first_line - 1)
		lineno = layout.first_line - 1;
	lastline = layout.last_line != 0 ? layout.last_line : page->num_lines;
	while (lineno < lastline && !should_abort() && *page_num < layout.last_page)
	{
		++(*page_num);
		printinfo.layout->skip =
			*page_num < layout.first_page ||
			*page_num > layout.last_page ||
			(layout.skip_pages != 0 && ((*page_num & 1) != (layout.skip_pages & 1)));
		verboseout("%s page %d: %s\n", printinfo.layout->skip ? "skip" : "print", *page_num, page->window_title);
		if (layout.swap_layout && (*page_num & 1) == 0)
			swap = TRUE;
		else
			swap = FALSE;
		calc_border_area(&printinfo, &a5, swap);
		start_page(&printinfo);
		if (!should_abort())
		{
			print_borders(&printinfo, &a5);
			if (!should_abort())
			{
				print_header(&printinfo, page, *page_num, swap, &a5);
				if (!should_abort())
				{
					print_footer(&printinfo, page, *page_num, swap, &a5);
					if (!should_abort())
					{
						print_text(&printinfo, page, &lineno, lastline, &a5);
						should_abort();
					}
				}
			}
		}
		emit_page(&printinfo);
	}
	
	vdi_free(v);
	
	return 0;
}
