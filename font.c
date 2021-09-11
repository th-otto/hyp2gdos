#define __HVDI_IMPLEMENTATION__
#include "hyp2gdos.h"

#define MAX_FONTS 8
#define IDX_OFFSET 20

struct font {
	_BOOL used;
	_WORD id;
	_WORD size;
};

struct vdi {
	/*  0 */ _WORD handle;
	/*  2 */ int refcount;
	/*  4 */ _WORD line_color;
	/*  6 */ _WORD line_mode;
	/*  8 */ _WORD line_type;
	/* 10 */ _WORD line_width;
	/* 12 */ _WORD fill_color;
	/* 14 */ _WORD fill_mode;
	/* 16 */ _WORD fill_pattern;
	/* 18 */ _WORD text_color;
	/* 20 */ _WORD text_mode;
	/* 22 */ _WORD text_effects;
	/* 24 */ _WORD text_fontidx;
	/* 26 */ _WORD cell_width;
	/* 28 */ _WORD cell_height;
	/* 30 */ _WORD wrmode;
	/* 32 */ _WORD last_line_color;
	/* 34 */ _WORD last_line_type;
	/* 36 */ _WORD last_line_width;
	/* 38 */ _WORD last_fill_color;
	/* 40 */ _WORD last_pattern;
	/* 42 */ _WORD last_fontidx;
	/* 44 */ _WORD last_effects;
	/* 46 */ _WORD last_text_color;
	/* 48 */ _WORD fontid;
	/* 50 */ _WORD fontsize;
	/* 52 */ _WORD o52;
	/* 54 */ _WORD o54;
	/* 56 */ _WORD o56;
	/* 58 */ _WORD o58;
};

struct font fonttable[MAX_FONTS];
_WORD print_handle;


/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

int new_font_id(void)
{
	int i;
	
	for (i = 0; i < MAX_FONTS; i++)
	{
		if (!fonttable[i].used)
		{
			fonttable[i].used = TRUE;
			return i + IDX_OFFSET;
		}
	}
	return 0;
}

/* ---------------------------------------------------------------------- */

int set_font(int fontidx, _WORD font_id, _WORD size)
{
	fontidx -= IDX_OFFSET;
	if (fontidx < 0 || fontidx >= MAX_FONTS)
		return 1;
	if (!fonttable[fontidx].used)
		return 2;
	fonttable[fontidx].id = font_id;
	fonttable[fontidx].size = size;
	return 0;
}

/* ---------------------------------------------------------------------- */

static void select_font(struct vdi *v, int idx)
{
	_WORD dummy;
	
	v->last_fontidx = idx;
	if (idx == 2)
	{
		v->fontid = vst_font(v->handle, fonts[0]);
		v->fontsize = vst_point(v->handle, fonts[1], &dummy, &dummy, &v->cell_width, &v->cell_height);
	} else if (idx == 1)
	{
		v->fontid = vst_font(v->handle, fonts[2]);
		v->fontsize = vst_point(v->handle, fonts[3], &dummy, &dummy, &v->cell_width, &v->cell_height);
	} else if (idx >= IDX_OFFSET && idx < IDX_OFFSET + MAX_FONTS)
	{
		idx -= IDX_OFFSET;
		if (fonttable[idx].used)
		{
			v->fontid = vst_font(v->handle, fonttable[idx].id);
			v->fontsize = vst_point(v->handle, fonttable[idx].size, &dummy, &dummy, &v->cell_width, &v->cell_height);
		}
	} else if (idx == 9)
	{
		v->fontid = vst_font(v->handle, fonts[4]);
		v->fontsize = vst_point(v->handle, fonts[5], &dummy, &dummy, &v->cell_width, &v->cell_height);
	} else if (idx == 12)
	{
		v->fontid = vst_font(v->handle, fonts[6]);
		v->fontsize = vst_point(v->handle, fonts[7], &dummy, &dummy, &v->cell_width, &v->cell_height);
	}
}

/* ---------------------------------------------------------------------- */

static void set_effects(struct vdi *v, _WORD effects)
{
	v->last_effects = effects;
	vst_effects(v->handle, effects);
}

/* ---------------------------------------------------------------------- */

static void set_pattern(struct vdi *v, _WORD pattern)
{
	_WORD style;
	
	if (pattern == IP_HOLLOW)
	{
		vsf_interior(v->handle, FIS_HOLLOW);
		v->last_pattern = IP_HOLLOW;
		v->last_pattern = pattern;
	} else if (pattern == IP_SOLID)
	{
		vsf_interior(v->handle, FIS_SOLID);
		v->last_pattern = pattern;
	} else
	{
		vsf_interior(v->handle, FIS_PATTERN);
		switch (pattern)
		{
			case 1: style = IP_1PATT; break;
			case 2: style = IP_2PATT; break;
			case 3: style = IP_3PATT; break;
			case 4: style = IP_4PATT; break;
			case 5: style = IP_5PATT; break;
			case 6: style = IP_6PATT; break;
			case 8: style = IP_SOLID; break;
			default: return;
		}
		vsf_style(v->handle, style);
		v->last_pattern = pattern;
	}
}

/* ---------------------------------------------------------------------- */

static void set_wrmode(struct vdi *v, _WORD mode)
{
	v->wrmode = mode;
	switch (mode)
	{
		case S_ONLY: mode = MD_REPLACE; break;
		case S_OR_D: mode = MD_TRANS; break;
		case S_XOR_D: mode = MD_XOR; break;
		case NOTS_OR_D: mode = MD_ERASE; break;
		default: return;
	}
	vswr_mode(v->handle, mode);
}

/* ---------------------------------------------------------------------- */

_WORD vdi_get_handle(struct vdi *v)
{
	return v->handle;
}

/* ---------------------------------------------------------------------- */

struct vdi *vdi_alloc(void)
{
	struct vdi *v;
	
	v = malloc(sizeof(*v));
	if (v != NULL)
	{
		memset(v, 0, sizeof(*v));
		v->handle = print_handle;
		vdi_ref(v);
		vdi_unref(v);
	}
	return v;
}

/* ---------------------------------------------------------------------- */

void vdi_free(struct vdi *v)
{
	free(v);
}

/* ---------------------------------------------------------------------- */

void vdi_ref(struct vdi *v)
{
	if (v == NULL)
		exit(1);
	if (v->refcount++ == 0)
	{
		vdi_defaults(v);
	}
}

/* ---------------------------------------------------------------------- */

void vdi_unref(struct vdi *v)
{
	if (v == NULL)
		exit(1);
	--v->refcount;
}

/* ---------------------------------------------------------------------- */

void vdi_line_attributes(struct vdi *v, _WORD line_color, _WORD line_mode, _WORD line_type, _WORD line_width)
{
	if (line_color >= 0)
		v->line_color = line_color;
	if (line_mode >= 0)
		v->line_mode = line_mode;
	if (line_type >= 0)
		v->line_type = line_type;
	if (line_width >= 0)
		v->line_width = line_width;
}

/* ---------------------------------------------------------------------- */

/* BUG: type/mode swapped */
static void vdi_force_line_attributes(struct vdi *v, _WORD line_color, _WORD line_type, _WORD line_mode, _WORD line_width)
{
	vsl_width(v->handle, 1);
	vsl_ends(v->handle, 0, 0);
	v->wrmode = -1;
	v->last_line_color = -1;
	v->last_line_type = -1;
	v->last_line_width = -1;
	vdi_line_attributes(v, line_color, line_mode, line_type, line_width);
}

/* ---------------------------------------------------------------------- */

static void draw_line(struct vdi *v, _WORD npoints, _WORD *pxy)
{
	vdi_ref(v);
	if (v->line_mode != v->wrmode)
		set_wrmode(v, v->line_mode);
	if (v->line_type != v->last_line_type)
	{
		v->last_line_type = v->line_type;
		vsl_type(v->handle, v->line_type);
	}
	if (v->line_width != v->last_line_width)
	{
		v->last_line_width = v->line_width;
		vsl_width(v->handle, v->line_width);
	}
	if (v->line_color != v->last_line_color)
	{
		v->last_line_color = v->line_color;
		vsl_color(v->handle, v->line_color);
	}
	v_pline(v->handle, npoints, pxy);
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_line(struct vdi *v, _WORD *pxy)
{
	draw_line(v, 2, pxy);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_arrowed(struct vdi *v, _WORD *pxy, _WORD ends)
{
	vdi_ref(v);
	if (v->line_mode != v->wrmode)
		set_wrmode(v, v->line_mode);
	if (v->line_type != v->last_line_type)
	{
		v->last_line_type = v->line_type;
		vsl_type(v->handle, v->line_type);
	}
	if (v->line_width != v->last_line_width)
	{
		v->last_line_width = v->line_width;
		vsl_width(v->handle, v->line_width);
	}
	if (v->line_color != v->last_line_color)
	{
		v->last_line_color = v->line_color;
		vsl_color(v->handle, v->line_color);
	}
	vsl_ends(v->handle, ends & 1 ? 1 : 0, ends & 2 ? 1 : 0);
	v_pline(v->handle, 2, pxy);
	vsl_ends(v->handle, 0, 0);
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_rect(struct vdi *v, const GRECT *gr)
{
	_WORD pxy[10];
	
	pxy[0] = gr->g_x;
	pxy[1] = gr->g_y;
	pxy[2] = gr->g_x + gr->g_w - 1;
	pxy[3] = pxy[1];
	pxy[4] = pxy[2];
	pxy[5] = gr->g_y + gr->g_h - 1;
	pxy[6] = pxy[0];
	pxy[7] = pxy[5];
	pxy[8] = pxy[0];
	pxy[9] = pxy[1];
	draw_line(v, 5, pxy);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_rounded_rect(struct vdi *v, const GRECT *gr)
{
	_WORD pxy[4];

	vdi_ref(v);
	if (v->line_mode != v->wrmode)
		set_wrmode(v, v->line_mode);
	if (v->line_type != v->last_line_type)
	{
		v->last_line_type = v->line_type;
		vsl_type(v->handle, v->line_type);
	}
	if (v->line_width != v->last_line_width)
	{
		v->last_line_width = v->line_width;
		vsl_width(v->handle, v->line_width);
	}
	if (v->line_color != v->last_line_color)
	{
		v->last_line_color = v->line_color;
		vsl_color(v->handle, v->line_color);
	}
	pxy[0] = gr->g_x;
	pxy[1] = gr->g_y;
	pxy[2] = pxy[0] + gr->g_w - 1;
	pxy[3] = pxy[1] + gr->g_h - 1;
	/* BUG: should set pattern to FIS_FOLLOW first */
	v_rbox(v->handle, pxy);
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_fill_attributes(struct vdi *v, _WORD color, _WORD mode, _WORD pattern)
{
	if (color >= -1)
		v->fill_color = color;
	if (mode >= -1)
		v->fill_mode = mode;
	if (pattern >= -1)
		v->last_pattern = pattern;
}

/* ---------------------------------------------------------------------- */

static void vdi_force_fill_attributes(struct vdi *v, _WORD color, _WORD mode, _WORD pattern)
{
	/* BUG: why -1? */
	v->wrmode = -1;
	v->last_fill_color = -1;
	v->last_pattern = -1;
	vdi_fill_attributes(v, color, mode, pattern);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_bar(struct vdi *v, const GRECT *gr)
{
	_WORD pxy[4];
	
	vdi_ref(v);
	pxy[0] = gr->g_x;
	pxy[1] = gr->g_y;
	pxy[2] = pxy[0] + gr->g_w - 1;
	pxy[3] = pxy[1] + gr->g_h - 1;
	if (v->fill_mode != v->wrmode)
		set_wrmode(v, v->fill_mode);
	if (v->fill_color != v->last_fill_color)
	{
		v->last_fill_color = v->fill_color;
		vsf_color(v->handle, v->fill_color);
	}
	if (v->fill_pattern != v->last_pattern)
		set_pattern(v, v->fill_pattern);
	v_bar(v->handle, pxy);
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_rounded_box(struct vdi *v, const GRECT *gr)
{
	_WORD pxy[4];

	vdi_ref(v);
	if (v->fill_mode != v->wrmode)
		set_wrmode(v, v->fill_mode);
	if (v->fill_color != v->last_fill_color)
	{
		v->last_fill_color = v->fill_color;
		vsf_color(v->handle, v->fill_color);
	}
	if (v->fill_pattern != v->last_pattern)
		set_pattern(v, v->fill_pattern);
	pxy[0] = gr->g_x;
	pxy[1] = gr->g_y;
	pxy[2] = pxy[0] + gr->g_w - 1;
	pxy[3] = pxy[1] + gr->g_h - 1;
	/* BUG: does not set line color */
	/* BUG: should be v_rfbox */
	v_rbox(v->handle, pxy);
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_text_attributes(struct vdi *v, _WORD text_color, _WORD text_mode, _WORD text_effects, _WORD font_idx)
{
	vdi_ref(v);
	if (text_color >= 0)
		v->text_color = text_color;
	if (text_mode >= 0)
		v->text_mode = text_mode;
	if (text_effects >= 0)
		v->text_effects = text_effects;
	if (font_idx >= 0)
		v->text_fontidx = font_idx;
	if (v->text_effects != v->last_effects)
		set_effects(v, v->text_effects);
	if (v->text_fontidx != v->last_fontidx)
		select_font(v, v->last_fontidx); /* BUG: should be text_fontidx */
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_get_fontwidth(struct vdi *v, _WORD *cell_width, _WORD *cell_height)
{
	vdi_ref(v);
	if (v->text_mode != v->wrmode)
		set_wrmode(v, v->text_mode);
	if (v->text_color != v->last_text_color)
	{
		v->last_text_color = v->text_color;
		vst_color(v->handle, v->text_color);
	}
	if (v->text_effects != v->last_effects)
		set_effects(v, v->text_effects);
	if (v->text_fontidx != v->last_fontidx)
		select_font(v, v->text_fontidx);
	*cell_width = v->cell_width;
	*cell_height = v->cell_height;
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

void vdi_draw_text(struct vdi *v, _WORD x, _WORD y, char *str, _WORD len)
{
	char c;
	
	vdi_ref(v);
	if (v->text_mode != v->wrmode)
		set_wrmode(v, v->text_mode);
	if (v->text_color != v->last_text_color)
	{
		v->last_text_color = v->text_color;
		vst_color(v->handle, v->text_color);
	}
	if (v->text_effects != v->last_effects)
		set_effects(v, v->text_effects);
	if (v->text_fontidx != v->last_fontidx)
		select_font(v, v->text_fontidx);
	c = str[len];
	str[len] = '\0';
	v_gtext(v->handle, x, y, str);
	str[len] = c;
	vdi_unref(v);
}

/* ---------------------------------------------------------------------- */

_WORD vdi_get_textwidth(struct vdi *v, char *str, _WORD len)
{
	char c;
	_WORD pxy[8];
	
	vdi_ref(v);
	if (v->text_mode != v->wrmode)
		set_wrmode(v, v->text_mode);
	if (v->text_color != v->last_text_color)
	{
		v->last_text_color = v->text_color;
		vst_color(v->handle, v->text_color);
	}
	if (v->text_effects != v->last_effects)
		set_effects(v, v->text_effects);
	if (v->text_fontidx != v->last_fontidx)
		select_font(v, v->text_fontidx);
	c = str[len];
	str[len] = '\0';
	vqt_extent(v->handle, str, pxy);
	str[len] = c;
	vdi_unref(v);
	return pxy[2]; /* not quite right if italics is used */
}

/* ---------------------------------------------------------------------- */

static void vdi_force_text_attributes(struct vdi *v, _WORD text_color, _WORD text_mode, _WORD text_effects, _WORD font_idx)
{
	_WORD dummy;
	
	vst_alignment(v->handle, TA_LEFT, TA_TOP, &dummy, &dummy);
	v->wrmode = -1;
	v->last_text_color = -1;
	v->last_effects = -1;
	v->last_fontidx = -1;
	vdi_text_attributes(v, text_color, text_mode, text_effects, font_idx);
}

/* ---------------------------------------------------------------------- */

void vdi_defaults(struct vdi *v)
{
	vdi_force_line_attributes(v, G_BLACK, S_ONLY, LT_SOLID, 1);
	vdi_force_fill_attributes(v, G_BLACK, S_ONLY, IP_SOLID);
	vdi_force_text_attributes(v, G_BLACK, S_OR_D, 0, 1);
}

/* ---------------------------------------------------------------------- */

void vdi_clip(struct vdi *v, const GRECT *gr)
{
	if (gr != NULL)
		v_clip(v->handle, gr);
}

/* ---------------------------------------------------------------------- */

int vdi_draw_bitmap(struct vdi *v, void *data,
	_WORD x, _WORD y, _WORD dstwidth, _WORD dstheight,
	_WORD srcwidth, _WORD srcheight,
	_WORD planes, _WORD mode,
	_WORD unused1, _WORD unused2)
{
	MFDB src;
	MFDB dst;
	_WORD pxy[8];
	_WORD colors[2];
	
	unused1 = unused1;
	unused2 = unused2;
	if (planes != 1)
		return 1;
	
	vdi_ref(v);
	if (srcwidth != dstwidth || srcheight != dstheight)
	{
		if (vdi_can_scale_bitmaps(v->handle))
		{
			mode |= 0x8000;
		} else
		{
			x += (dstwidth - srcwidth) / 2;
			y += (dstheight - srcheight) / 2;
			dstwidth = srcwidth;
			dstheight = srcheight;
		}
	}
	
	pxy[0] = 0;
	pxy[1] = 0;
	pxy[2] = srcwidth - 1;
	pxy[3] = srcheight - 1;
	pxy[4] = x;
	pxy[5] = y;
	pxy[6] = x + dstwidth - 1;
	pxy[7] = y + dstheight - 1;
	src.fd_addr = data;
	src.fd_w = srcwidth;
	src.fd_h = srcheight;
	src.fd_wdwidth = (srcwidth + 15) / 16;
	src.fd_stand = TRUE;
	src.fd_nplanes = 1;
	src.fd_r1 = 0;
	src.fd_r2 = 0;
	src.fd_r3 = 0;
	dst.fd_addr = NULL;
	colors[0] = 1;
	colors[1] = 0;
	vrt_cpyfm(v->handle, mode, pxy, &src, &dst, colors);
	vdi_unref(v);
	
	return 0;
}

/* ---------------------------------------------------------------------- */

static unsigned short const plane2_params[][3] = {
	{ 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffff, 0x0004 },
	
	{ 0xffff, 0xffff, 0xffff },
	{ 0xffff, 0x0000, 0x0000 },
	{ 0x0000, 0xffff, 0x0000 },
	{ 0x0000, 0x0000, 0x0000 }
};
static unsigned short const plane3_params[][3] = {
	{ 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffff, 0x0008 },
	
	{ 0xffff, 0xffff, 0xffff },
	{ 0xffff, 0x0000, 0x0000 },
	{ 0x0000, 0xffff, 0x0000 },
	{ 0xffff, 0xffff, 0x0000 },
	{ 0x0000, 0x0000, 0xffff },
	{ 0xffff, 0x0000, 0xffff },
	{ 0x0000, 0xffff, 0xffff },
	{ 0x0000, 0x0000, 0x0000 }
};
static unsigned short const plane4_params[][3] = {
	{ 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffff, 0x0010 },
	
	{ 0xffff, 0xffff, 0xffff },
	{ 0xffff, 0x0000, 0x0000 },
	{ 0x0000, 0xffff, 0x0000 },
	{ 0xffff, 0xffff, 0x0000 },
	{ 0x0000, 0x0000, 0xffff },
	{ 0xffff, 0x0000, 0xffff },
	{ 0x0000, 0xffff, 0xffff },
	{ 0xaaaa, 0xaaaa, 0xaaaa },
	{ 0x5555, 0x5555, 0x5555 },
	{ 0xaaaa, 0x0000, 0x0000 },
	{ 0x0000, 0xaaaa, 0x0000 },
	{ 0xaaaa, 0xaaaa, 0x0000 },
	{ 0x0000, 0x0000, 0xaaaa },
	{ 0xaaaa, 0x0000, 0xaaaa },
	{ 0x0000, 0xaaaa, 0xaaaa },
	{ 0x0000, 0x0000, 0x0000 }
};
		
static int dither_image(const void *data, _WORD width, _WORD height, _WORD planes, const unsigned short params[][3], void *mono)
{
	long bytes_per_line;
	long planesize;
	const unsigned char *inputline;
	unsigned char *outputline;
	unsigned char outpix;
	_WORD x;
	_WORD y;
	unsigned short *linebuf;
	_WORD ret;
	short quant_error;
	short inputmask;
	unsigned char outputmask;
	short pixmask;
	long words;
	unsigned short *lineptr;
	const unsigned char *srcptr;
	unsigned char *dstptr;

/*
 * temporary hack to get reigster assignments mostly right
 */
#ifdef __PUREC__
	(void)params;
#pragma warn -def
	(void)lineptr;
#pragma warn .def
#endif
	ret = 0;
	linebuf = malloc((width + 2) * sizeof(*linebuf));
	if (linebuf == NULL)
		return 2;
	memset(linebuf, 0, (width + 2) * sizeof(*linebuf));
	words = (width + 15) / 16;
	bytes_per_line = words * 2;
	planesize = bytes_per_line * height;
	if (params == NULL)
	{
		switch (planes)
		{
			case 2: params = plane2_params; break;
			case 3: params = plane3_params; break;
			case 4: params = plane4_params; break;
		}
	}
	
	inputline = data;
	outputline = mono;
	for (y = 0; y < height; y++)
	{
		if (should_abort())
		{
			ret = 1;
			break;
		}
		lineptr = linebuf + 1;
		quant_error = lineptr[0];
		quant_error = (quant_error + 8) >> 4;
		lineptr[0] = 0;
		srcptr = inputline;
		inputmask = 0x80;
		dstptr = outputline;
		outputmask = 0x80;
		outpix = 0;
		for (x = 0; x < width; x++)
		{
			short plane;
			short pixval;
			short tmp;
			const unsigned char *planeptr;
			
			planeptr = srcptr;
			pixval = 0;
			pixmask = 1;
			for (plane = pixval; plane < planes; plane++)
			{
				if (*planeptr & inputmask)
					pixval |= pixmask;
				pixmask += pixmask;
				planeptr += planesize;
			}
			inputmask >>= 1;
			if (inputmask == 0)
			{
				inputmask = 0x80;
				srcptr++;
			}
			if (quant_error < -255)
				quant_error = -255;
			else if (quant_error > 255)
				quant_error = 255;
			if (params != NULL)
			{
				short r;
				short g;
				short b;
				r = ((long)params[pixval + 2][0] * 255) / (params[1][1] - 1);
				g = ((long)params[pixval + 2][1] * 255) / (params[1][1] - 1);
				b = ((long)params[pixval + 2][2] * 255) / (params[1][1] - 1);
				quant_error += (r * 30L + g * 59L + b * 11L) / 100;
			} else
			{
				quant_error += (pixval * 255) / (pixmask - 1);
			}
			if (quant_error <= 127)
				outpix |= outputmask;
			outputmask >>= 1;
			if (outputmask == 0)
			{
				outputmask = 0x80;
				*dstptr++ = outpix;
				outpix = 0;
			}
			if (quant_error > 127)
				quant_error -= 255;
			tmp = quant_error;
			lineptr[-1] += tmp * 3;
			*lineptr++ += tmp * 5;
			quant_error = tmp * 7 + *lineptr;
			*lineptr = tmp;
			quant_error = (quant_error + 8) >> 4;
		}
		
		inputline += bytes_per_line;
		outputline += bytes_per_line;
		if (outputmask != 0x80)
			*dstptr++ = outpix;
		while (dstptr < outputline)
			*dstptr++ = 0;
	}
	
	free(linebuf);
	
	return ret;
}

/* ---------------------------------------------------------------------- */

static _BOOL is_truecolor(struct vdi *v)
{
	_WORD workout[273];
	
	memset(workout, 0, sizeof(workout));
	vq_scrninfo(v->handle, workout);
	return workout[0] == 2 && workout[2] == 32;
}

/* ---------------------------------------------------------------------- */

static int draw_image(struct vdi *v, void *data,
	_WORD x, _WORD y, _WORD dstwidth, _WORD dstheight,
	_WORD srcwidth, _WORD srcheight,
	_WORD planes, _WORD mode)
{
	long words;
	MFDB src;
	MFDB dst;
	_WORD pxy[8];
	
	mode = S_ONLY; /* WTF */
	if (planes != 32)
		return 1;
	words = (srcwidth + 15) / 16;

	vdi_ref(v);
	if (srcwidth != dstwidth || srcheight != dstheight)
	{
		if (vdi_can_scale_bitmaps(v->handle))
		{
			mode |= 0x8000;
		} else
		{
			x += (dstwidth - srcwidth) / 2;
			y += (dstheight - srcheight) / 2;
			dstwidth = srcwidth;
			dstheight = srcheight;
		}
	}
	
	pxy[0] = 0;
	pxy[1] = 0;
	pxy[2] = srcwidth - 1;
	pxy[3] = srcheight - 1;
	pxy[4] = x;
	pxy[5] = y;
	pxy[6] = x + dstwidth - 1;
	pxy[7] = y + dstheight - 1;
	src.fd_addr = data;
	src.fd_w = srcwidth;
	src.fd_h = srcheight;
	src.fd_wdwidth = (short)words;
	src.fd_stand = FALSE;
	src.fd_nplanes = 32;
	src.fd_r1 = 0;
	src.fd_r2 = 0;
	src.fd_r3 = 0;
	dst.fd_addr = NULL;
	vro_cpyfm(v->handle, mode, pxy, &src, &dst);
	vdi_unref(v);
	return 0;
}

/* ---------------------------------------------------------------------- */

static int convert_image(const unsigned char *inputptr, _WORD width, _WORD height, _WORD planes, const unsigned short params[][3], void *color)
{
	long words;
	long bytes_per_line; /* 8 */
	long planesize; /* d6 */
	unsigned char *outputline; /* 4 */
	_WORD x;
	_WORD y; /* 0 */
	const unsigned char *srcptr; /* a5 */
	unsigned char *dstptr; /* a4 */
	short inputmask; /* d5 */
	
/*
 * temporary hack to get reigster assignments mostly right
 */
#ifdef __PUREC__
	(void) params;
	(void) inputptr;
#endif
	
	words = (width + 15) / 16;
	bytes_per_line = words * 2;
	planesize = bytes_per_line * height;

	if (params == NULL)
	{
		switch (planes)
		{
			case 2: params = plane2_params; break;
			case 3: params = plane3_params; break;
			case 4: params = plane4_params; break;
		}
	}
	
	outputline = color;
	for (y = 0; y < height; y++)
	{
		if (should_abort())
			return 1;
		srcptr = inputptr;
		dstptr = outputline;
		inputmask = 0x80;
		for (x = 0; x < width; x++)
		{
			short plane;
			short pixval;
			short pixmask;
			const unsigned char *planeptr;
			
			planeptr = srcptr;
			pixval = 0;
			pixmask = 1;
			for (plane = pixval; plane < planes; plane++)
			{
				if (*planeptr & inputmask)
					pixval |= pixmask;
				pixmask += pixmask;
				planeptr += planesize;
			}
			inputmask >>= 1;
			if (inputmask == 0)
			{
				inputmask = 0x80;
				srcptr++;
			}
			if (params != NULL)
			{
				*dstptr++ = 0;
				*dstptr++ = ((long)params[pixval + 2][0] * 255) / (params[1][1] - 1);
				*dstptr++ = ((long)params[pixval + 2][1] * 255) / (params[1][1] - 1);
				*dstptr++ = ((long)params[pixval + 2][2] * 255) / (params[1][1] - 1);
			} else
			{
				pixval = (pixval * 255) / (pixmask - 1);
				*dstptr++ = 0;
				*dstptr++ = pixval;
				*dstptr++ = pixval;
				*dstptr++ = pixval;
			}
			
		}
		inputptr = inputptr + bytes_per_line;
		outputline += bytes_per_line * 32;
	}
	return 0;
}

/* ---------------------------------------------------------------------- */

int vdi_draw_image(struct vdi *v, void *data,
	_WORD x, _WORD y, _WORD dstwidth, _WORD dstheight,
	_WORD srcwidth, _WORD srcheight,
	_WORD planes, _WORD mode,
	_WORD unused1, _WORD unused2, const unsigned short params[][3])
{
	int ret;
	long words;
	long bytes_per_line;
	long planesize;
	void *tmp;
	long allocsize;
	
	ret = 0;
	if (planes == 1)
	{
		vdi_draw_bitmap(v, data, x, y, dstwidth, dstheight, srcwidth, srcheight, planes, mode, unused1, unused2);
		return 0;
	}
	
	words = (srcwidth + 15) / 16;
	bytes_per_line = words * 2;
	planesize = bytes_per_line * srcheight;
	if (is_truecolor(v))
	{
		allocsize = planesize * 32;
		tmp = malloc(allocsize);
		if (tmp == NULL)
		{
			ret = 2;
		} else
		{
			ret = convert_image(data, srcwidth, srcheight, planes, params, tmp);
			if (ret == 0)
			{
				draw_image(v, tmp, x, y, dstwidth, dstheight, srcwidth, srcheight, 32, mode);
			}
			free(tmp);
		}
	} else
	{
		allocsize = planesize;
		tmp = malloc(allocsize);
		if (tmp == NULL)
		{
			ret = 2;
		} else
		{
			ret = dither_image(data, srcwidth, srcheight, planes, params, tmp);
			if (ret == 0)
			{
				vdi_draw_bitmap(v, tmp, x, y, dstwidth, dstheight, srcwidth, srcheight, 1, mode, unused1, unused2);
			}
			free(tmp);
		}
	}
	if (ret != 0)
	{
		GRECT gr;

		gr.g_x = x;
		gr.g_y = y;
		gr.g_w = dstwidth;
		gr.g_h = dstheight;
		vdi_draw_rect(v, &gr);
	}
	return ret;
}

/* ---------------------------------------------------------------------- */

int x18118(char *data, short width, short height, short planes)
{
	UNUSED(data);
	UNUSED(width);
	UNUSED(height);
	UNUSED(planes);
	return 0;
}

/* ---------------------------------------------------------------------- */

void vdi_get_outputsize(_WORD handle, GRECT *gr)
{
	_WORD workout[57];
	
	vq_extnd(handle, 0, workout);
	gr->g_x = 0;
	gr->g_y = 0;
	gr->g_w = workout[0] + 1;
	gr->g_h = workout[1] + 1;
}

/* ---------------------------------------------------------------------- */

void vdi_get_pagesize(_WORD handle, GRECT *gr)
{
	_WORD workout[57];
	
	vq_extnd(handle, 1, workout);
	vdi_get_outputsize(handle, gr);
	gr->g_x -= workout[40];
	gr->g_y -= workout[41];
	gr->g_w += workout[40] + workout[42];
	gr->g_h += workout[41] + workout[43];
}

/* ---------------------------------------------------------------------- */

_BOOL vdi_can_scale_bitmaps(_WORD handle)
{
	_WORD workout[57];
	
	vq_extnd(handle, 1, workout);
	return (workout[30] & 1) != 0;
}

/* ---------------------------------------------------------------------- */

void vdi_get_dpi(_WORD handle, _WORD *hdpi, _WORD *vdpi)
{
	_WORD workout[57];
	
	vq_extnd(handle, 1, workout);
	*hdpi = workout[23];
	*vdpi = workout[24];
}

/* ---------------------------------------------------------------------- */

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
	pb.ptsin = _VDIParBlk.ptsin;
	pb.intout = workout;
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

/* ---------------------------------------------------------------------- */

void v_clip(_WORD handle, const GRECT *gr)
{
	_WORD clip_flag;
	_WORD pxy[4];
	
	clip_flag = FALSE;
	if (gr != NULL &&
		(gr->g_x != 0 || gr->g_y != 0 || gr->g_w != 0 || gr->g_h != 0))
	{
		clip_flag = TRUE;
		pxy[0] = gr->g_x;
		pxy[1] = gr->g_y;
		pxy[2] = gr->g_x + gr->g_w - 1;
		pxy[3] = gr->g_y + gr->g_h - 1;
	}
	vs_clip(handle, clip_flag, pxy);
}

/* ---------------------------------------------------------------------- */

void vdi_clear_page(_WORD handle)
{
	_WORD pxy[10]; /* FIXME: 4 are enough */
	GRECT gr;
	
	vdi_get_outputsize(handle, &gr);
	pxy[0] = gr.g_x;
	pxy[1] = gr.g_y;
	pxy[2] = gr.g_x + gr.g_w - 1;
	pxy[3] = gr.g_y + gr.g_h - 1;
	v_clip(handle, &gr);
	vsf_interior(handle, FIS_SOLID);
	vswr_mode(handle, MD_REPLACE);
	vsf_color(handle, G_WHITE);
	v_bar(handle, pxy);
}
