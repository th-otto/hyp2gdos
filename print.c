#include "hyp2gdos.h"

struct printinfo {
	/*  0 */ struct vdi *vdi;
	/*  4 */ GRECT text_area;
	/* 12 */ short cell_width;
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
void x136da(struct pageinfo *page, struct printinfo *printinfo, long y, long x, _WORD *x1);
char *print_link(struct hypfile *hyp, struct printinfo *printinfo, _WORD *x, _WORD *y, char *text, short *column);
void x132c8(struct vdi *vdi, _WORD x, _WORD x1, _WORD x2);


#ifdef __GNUC__
void x13332(struct printinfo *printinfo)
{
	printinfo->line_height = 0;
	printinfo->cell_width = 0;
}
void x136da(struct pageinfo *page, struct printinfo *printinfo, long y, long x, _WORD *x1)
{
	(void) printinfo;
	(void) page;
	(void) x;
	(void) y;
	*x1 = 0;
}
#endif

static char udodatabase[LINEMAX];
static char udotitle[LINEMAX];

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

static void print_line(struct printinfo *printinfo, struct pageinfo *page, long lineno, _WORD x, _WORD y)
{
	struct fontinfo *fonts;
	struct vdi *vdi;
	_WORD d3;
	_WORD effects;
	_WORD fontid;
	char *text;
	short column;
	char *start;
	
	fonts = printinfo->fonts;
	vdi = printinfo->vdi;
	d3 = x;
	effects = 0;
	if (page->hyp->header.magic == HYP_MAGIC_HYP || fonts->use_standard)
		fontid = fonts->standard_font_id;
	else
		fontid = fonts->typewriter_font_id;
	vdi_text_attributes(vdi, fonts->text_color, -1, effects, fontid);
	text = page->text[lineno];
	column = 0;
	while (*text)
	{
		if (*text != HYP_ESC)
		{
			start = text;
			if (fonts->expand_spaces)
			{
				if (*text == '\t')
				{
					text++;
					column = ((column + fonts->tabsize) / fonts->tabsize) * fonts->tabsize;
					goto x13870;
				} else
				{
					if (*text++ == ' ' && *text == ' ')
					{
						short x2;
						
						do
							text++;
						while (*text == ' ');
						column += text - start;
				x13870:
						x2 = printinfo->cell_width * column + d3;
						if (effects != 0)
							x132c8(vdi, y, x, x2);
						x = x2;
					} else
					{
						for (;;)
						{
							switch (*text)
							{
							case HYP_EOL:
							case HYP_ESC:
							case '\t':
								goto printout;
							case ' ':
								if (text[1] == ' ')
									goto printout;
								text++;
								break;
							default:
								text++;
								break;
							}
						}
					}
				}
			} else
			{
				_WORD len;
				
				/* 138be */
				text++;
				while (*text != HYP_EOL && *text != HYP_ESC)
					text++;
				/* 138ca */
			printout:
				len = (_WORD)(text - start);
				vdi_draw_text(vdi, x, y, start, len);
				x += vdi_get_textwidth(vdi, start, len);
				column += len;
			}
		} else
		{
			if (page->hyp->header.magic != HYP_MAGIC_HYP)
			{
				vdi_draw_text(vdi, x, y, text, 1);
				x += vdi_get_textwidth(vdi, text, 1);
				text++;
				column += 1;
			} else
			{
				text++;
				switch (*text)
				{
				case HYP_ESC_ESC:
					vdi_draw_text(vdi, x, y, text, 1);
					x += vdi_get_textwidth(vdi, text, 1);
					text++;
					column += 1;
					break;
				
				case HYP_ESC_LINK:
				case HYP_ESC_ALINK:
					text = print_link(page->hyp, printinfo, &x, &y, text + 1, &column);
					vdi_text_attributes(vdi, fonts->text_color, -1, effects, fontid);
					break;

				case HYP_ESC_LINK_LINE:
				case HYP_ESC_ALINK_LINE:
					text = print_link(page->hyp, printinfo, &x, &y, text + 3, &column);
					vdi_text_attributes(vdi, fonts->text_color, -1, effects, fontid);
					break;
				
				default:
					/*
					 * BUG: should check for upper limit, too.
					 */
					if ((unsigned char) *text >= HYP_ESC_TEXTATTR_FIRST)
					{
						effects = (unsigned char) *text - HYP_ESC_TEXTATTR_FIRST;
						if (effects & 0x40)
							fontid = fonts->typewriter_font_id;
						else
							fontid = fonts->standard_font_id;
						vdi_text_attributes(vdi, fonts->text_color, -1, effects, fontid);
					}
					text++;
					break;
				}
			}
		}
	}
	
	vdi_text_attributes(vdi, -1, -1, 0, page->hyp->header.magic == HYP_MAGIC_HYP || fonts->use_standard ? fonts->standard_font_id : fonts->typewriter_font_id);
}

/* ---------------------------------------------------------------------- */

static long calc_text_height(struct printinfo *printinfo, struct pageinfo *page, long lineno, long column, long height)
{
	char *data;
	char *end; /* 16 */
	short picnode; /* 14 */
	short y_offset; /* 12 */
	short x_offset; /* 10 */
	short char_width; /* 8 */
	short char_height; /* 6 */
	short attributes;
	long y; /* 0 */
	long pixwidth;
	long pixheight;
	long w;
	struct node *pic;
	char *picdata;
	
	data = page->node->data;
	end = data + page->node->datalen;
	if (page->hyp->header.magic == HYP_MAGIC_HYP)
	{
		while (data < end && *data == HYP_ESC)
		{
			switch (data[1])
			{
			case HYP_ESC_PIC:
				dec_255_decode(data + 2, &picnode);
				pic = page->node->picdata;
				while (pic && pic->nodenr != picnode)
					pic = pic->picdata;
				if (pic == NULL)
				{
					char_width = 1;
					char_height = 1;
					if (page->hyp->flags & SCALE_IMAGES)
					{
						pixwidth = char_width * printinfo->cell_width;
						pixheight = char_height * printinfo->line_height;
					} else
					{
						pixwidth = char_width * HYP_PIC_FONTW;
						pixheight = char_height * HYP_PIC_FONTH;
					}
				} else
				{
					picdata = pic->data;
					if (page->hyp->flags & SCALE_IMAGES)
					{
						pixwidth = (*((short *)picdata) / HYP_PIC_FONTW) * printinfo->cell_width +
							*((short *)picdata) % HYP_PIC_FONTW;
						pixheight = (*((short *)(picdata + 2)) / HYP_PIC_FONTH) * printinfo->line_height +
							*((short *)(picdata + 2)) % HYP_PIC_FONTH;
					} else
					{
						pixwidth = *((short *)picdata);
						pixheight = *((short *)(picdata + 2));
					}
				}
				char_width = (pixwidth + printinfo->cell_width - 1) / printinfo->cell_width;
				char_height = (pixheight + printinfo->line_height - 1) / printinfo->line_height;
				decode_char(data + 4, &x_offset);
				if (x_offset < 0)
				{
					w = (page->hyp->width * printinfo->cell_width - pixwidth) / 2;
					if (w < 0)
						w = 0;
					w = (printinfo->text_area.g_x + (int)w) - (_WORD)(column * printinfo->cell_width);
				} else
				{
					w = (_WORD)((x_offset - column) * printinfo->cell_width) + printinfo->text_area.g_x;
				}
				dec_255_decode(data + 5, &y_offset);
				y = (_WORD)((y_offset - lineno) * printinfo->line_height) + printinfo->text_area.g_y;
				if (char_height <= height && y_offset > lineno &&
					y_offset < lineno + height &&
					y_offset + char_height > lineno + height)
				{
					height = y_offset - lineno;
				}
				data += 9;
				(void) &y; /* XXX */
				break;
	
			case HYP_ESC_LINE:
			case HYP_ESC_BOX:
			case HYP_ESC_RBOX:
				data = decode_char(data + 2, &x_offset);
				data = dec_255_decode(data, &y_offset);
				data = decode_char(data, &char_width);
				data = decode_char(data, &char_height);
				data = decode_char(data, &attributes);
				char_height += 1;
				if (char_height <= height && y_offset > lineno &&
					y_offset < lineno + height &&
					y_offset + char_height > lineno + height)
				{
					height = y_offset - lineno;
				}
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
			
			case HYP_ESC_LINK:
			case HYP_ESC_LINK_LINE:
			case HYP_ESC_ALINK:
			case HYP_ESC_ALINK_LINE:
			default:
				data = end;
				break;
			}
		}
	}
	return height;
}

/* ---------------------------------------------------------------------- */

#ifdef __PUREC__
static void nothing1(void) 0x4a6f;
static void nothing2(void) 0x000a;
#endif

static void print_graphics(struct printinfo *printinfo, struct pageinfo *page, long lineno, long column, long numlines)
{
	struct vdi *vdi;
	char *data;
	struct fontinfo *fonts;
	char *end;
	GRECT gr;
	short attributes;
	short y_offset;
	short x_offset;
	short char_width;
	short char_height;
	short rnode;
	
	fonts = printinfo->fonts;
	vdi = printinfo->vdi;
	data = page->node->data;
	end = data + page->node->datalen;
	if (page->hyp->header.magic != HYP_MAGIC_HYP)
		return;
	while (data < end && *data == HYP_ESC)
	{
		switch (data[1])
		{
		case HYP_ESC_PIC:
			{
				char *picdata;
				short picnode;
				struct node *pic;

				dec_255_decode(data + 2, &picnode);
				pic = page->node->picdata;
				while (pic && pic->nodenr != picnode)
					pic = pic->picdata;
				if (pic == NULL)
				{
					char_width = 1;
					char_height = 1;
					if (page->hyp->flags & SCALE_IMAGES)
					{
						gr.g_w = char_width * printinfo->cell_width;
						gr.g_h = char_height * printinfo->line_height;
					} else
					{
						gr.g_w = char_width * HYP_PIC_FONTW;
						gr.g_h = char_height * HYP_PIC_FONTH;
					}
				} else
				{
					picdata = pic->data;
					if (page->hyp->flags & SCALE_IMAGES)
					{
						gr.g_w = (*((short *)picdata) / HYP_PIC_FONTW) * printinfo->cell_width +
							*((short *)picdata) % HYP_PIC_FONTW;
						gr.g_h = (*((short *)(picdata + 2)) / HYP_PIC_FONTH) * printinfo->line_height +
							*((short *)(picdata + 2)) % HYP_PIC_FONTH;
					} else
					{
						gr.g_w = *((short *)picdata);
						gr.g_h = *((short *)(picdata + 2));
					}
				}
				char_width = (gr.g_w + printinfo->cell_width - 1) / printinfo->cell_width;
				char_height = (gr.g_h + printinfo->line_height - 1) / printinfo->line_height;
				decode_char(data + 4, &x_offset);
				if (x_offset < 0)
				{
					long w = (page->hyp->width * printinfo->cell_width - gr.g_w) / 2;
					if (w < 0)
						w = 0;
					gr.g_x = (printinfo->text_area.g_x + (int)w) - (_WORD)(column * printinfo->cell_width);
				} else
				{
					gr.g_x = (_WORD)((x_offset - column) * printinfo->cell_width) + printinfo->text_area.g_x;
				}
				dec_255_decode(data + 5, &y_offset);
				gr.g_y = (_WORD)((y_offset - lineno) * printinfo->line_height) + printinfo->text_area.g_y;
				if (y_offset < lineno + numlines && y_offset + char_height >= lineno)
				{
					if (pic == NULL)
					{
						vdi_line_attributes(vdi, G_BLACK, S_OR_D, LT_SOLID, 1);
						vdi_draw_rect(vdi, &gr);
					} else
					{
						if (picdata[4] == 1) /* planes */
						{
							vdi_draw_bitmap(vdi, picdata + 8, gr.g_x, gr.g_y, gr.g_w, gr.g_h, *((short *)picdata), *((short *)(picdata + 2)), 1, S_OR_D, 1, 0);
						} else
						{
							vdi_draw_image(vdi, picdata + 8, gr.g_x, gr.g_y, gr.g_w, gr.g_h, *((short *)picdata), *((short *)(picdata + 2)), picdata[4], S_OR_D, 1, 0,
								pic->dither_params != NULL ? pic->dither_params : page->hyp->dither_params);
						}
					}
				}
				data += 9;
			}
			/*
			 * ??? Maybe Hyperion extension?
			 */
			if (*data == HYP_ESC && data[1] == HYP_ESC_DATA0 && fonts->x18b5c)
			{
				_WORD color;
				_WORD effects;
				
				dec_255_decode(data + 4, &rnode);
				rc_shrink(&gr, -2, -2);
				fonts->p_get_effects(page->hyp, rnode, &color, &effects);
				vdi_line_attributes(vdi, color, S_OR_D, LT_SOLID, 1);
				/* vdi_draw_rect(vdi, &gr); */
				goto draw_rect; /* FIXME */
			}
			break;
		
		case HYP_ESC_LINE:
			{
				_WORD pxy[4];
				_WORD ends;
				
				data = decode_char(data + 2, &x_offset);
				data = dec_255_decode(data, &y_offset);
				data = decode_char(data, &char_width);
				data = decode_char(data, &char_height);
				data = decode_char(data, &attributes);
				gr.g_x = x_offset - (_WORD)column;
				gr.g_y = y_offset - (_WORD)lineno;
				gr.g_w = char_width - 127;
				gr.g_h = char_height;
				gr.g_x = gr.g_x * printinfo->cell_width;
				gr.g_y = gr.g_y * printinfo->line_height;
				gr.g_w = gr.g_w * printinfo->cell_width;
				gr.g_h = gr.g_h * printinfo->line_height;
				gr.g_x += printinfo->text_area.g_x;
				gr.g_y += printinfo->text_area.g_y;
				pxy[0] = gr.g_x;
				pxy[1] = gr.g_y;
				pxy[2] = gr.g_x + gr.g_w /* - 1 */; /* BUG */
				pxy[3] = gr.g_y + gr.g_h /* - 1 */; /* BUG */
				ends = attributes & 3;
				attributes = (attributes >> 3);
				attributes += 1;
				vdi_line_attributes(vdi, G_BLACK, S_OR_D, attributes, 1);
				vdi_draw_arrowed(vdi, pxy, ends);
			}
			break;
		
		case HYP_ESC_BOX:
			{
				_WORD x1;
				_WORD x2;
				
				data = decode_char(data + 2, &x_offset);
				data = dec_255_decode(data, &y_offset);
				data = decode_char(data, &char_width);
				data = decode_char(data, &char_height);
				data = decode_char(data, &attributes);
				gr.g_x = x_offset;
				gr.g_y = y_offset - (_WORD)lineno;
				gr.g_w = char_width + 1;
				gr.g_h = char_height + 1;
				if (gr.g_y + gr.g_h >= 0)
				{
					if (gr.g_h == 1)
					{
						x136da(page, printinfo, y_offset, gr.g_x, &x1);
						x136da(page, printinfo, y_offset, gr.g_x + gr.g_w, &x2);
						gr.g_x = x1;
						gr.g_w = x2 - x1;
						gr.g_y = gr.g_y * printinfo->line_height;
						gr.g_h = printinfo->line_height;
					} else
					{
						gr.g_x = gr.g_x * printinfo->cell_width;
						gr.g_y = gr.g_y * printinfo->line_height;
						gr.g_w = gr.g_w * printinfo->cell_width;
						gr.g_h = gr.g_h * printinfo->line_height;
					}
					gr.g_x += printinfo->text_area.g_x - (_WORD)column * printinfo->cell_width;
					gr.g_y += printinfo->text_area.g_y;
					if (attributes >= 7)
					{
						attributes = attributes == 7 ? 8 : 7;
					} else
					{
#ifdef __PUREC__
						/* XXX */
						nothing1();
						nothing2();
#endif
					}
					vdi_fill_attributes(vdi, G_BLACK, S_OR_D, attributes);
					vdi_draw_bar(vdi, &gr);
					vdi_line_attributes(vdi, G_BLACK, S_OR_D, LT_SOLID, 1);
				draw_rect:
					vdi_draw_rect(vdi, &gr);
				}
			}
			break;
		
		case HYP_ESC_RBOX:
			{
				data = decode_char(data + 2, &x_offset);
				data = dec_255_decode(data, &y_offset);
				data = decode_char(data, &char_width);
				data = decode_char(data, &char_height);
				data = decode_char(data, &attributes);
				gr.g_x = x_offset - (_WORD)column;
				gr.g_y = y_offset - (_WORD)lineno;
				gr.g_w = char_width + 1;
				gr.g_h = char_height + 1;
				if (gr.g_y + gr.g_h >= 0)
				{
					gr.g_x = gr.g_x * printinfo->cell_width;
					gr.g_y = gr.g_y * printinfo->line_height;
					gr.g_w = gr.g_w * printinfo->cell_width;
					gr.g_h = gr.g_h * printinfo->line_height;
					gr.g_x += printinfo->text_area.g_x;
					gr.g_y += printinfo->text_area.g_y;
					if (attributes >= 7)
					{
						attributes = attributes == 7 ? 8 : 7;
					} else
					{
#ifdef __PUREC__
						/* XXX */
						nothing1();
						nothing2();
#endif
					}
					vdi_fill_attributes(vdi, G_BLACK, S_OR_D, attributes);
					vdi_draw_rounded_box(vdi, &gr, 10);
					vdi_line_attributes(vdi, G_BLACK, S_OR_D, LT_SOLID, 1);
					vdi_draw_rounded_rect(vdi, &gr, 10);
				}
			}
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
		
		case HYP_ESC_LINK:
		case HYP_ESC_LINK_LINE:
		case HYP_ESC_ALINK:
		case HYP_ESC_ALINK_LINE:
		default:
			data = end;
			break;
		}
	}
	
}

/* ---------------------------------------------------------------------- */

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
	long line;
	
	vdi = printinfo->vdi;
	fonts = printinfo->fonts;
	numlines = a5->text_area.g_h / printinfo->line_height;
	numlines = calc_text_height(printinfo, page, *lineno, 0, numlines);
	if (numlines > lastline - *lineno)
		numlines = lastline - *lineno;
	if (!printinfo->layout->skip)
	{
		gr = a5->text_area;
		gr.g_h = (_WORD)(numlines * printinfo->line_height);
		vdi_clip(vdi, &gr);
		print_graphics(printinfo, page, *lineno, 0, numlines);
		if (should_abort())
			return;
	}
	vdi_clip(vdi, &a5->text_area);
	vdi_text_attributes(vdi, fonts->text_color, S_OR_D, 0, page->hyp->header.magic == HYP_MAGIC_HYP || fonts->use_standard ? fonts->standard_font_id : fonts->typewriter_font_id);
	x = a5->text_area.g_x;
	y = a5->text_area.g_y;
	line = 0;
	while (line < numlines)
	{
		if (should_abort())
			break;
		if (!printinfo->layout->skip)
		{
			print_line(printinfo, page, *lineno, x, y);
		}
		y += printinfo->line_height;
		++(*lineno);
		line++;
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
