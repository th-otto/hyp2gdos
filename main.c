#include "hyp2gdos.h"
#include <ctype.h>





char standard_font[LINEMAX];
char typewriter_font[LINEMAX];
char pagename[LINEMAX];
_BOOL pagename_is_title;
_BOOL case_insensitive;
_WORD vdihandle;
_BOOL abort_flag;
_BOOL scale_flag;
char x19c22[258];
_WORD x19d24;
_WORD num_loaded_fonts;
_WORD x19d28;

char head_left[LINEMAX];
char head_center[LINEMAX];
char head_right[LINEMAX];
char foot_left[LINEMAX];
char foot_center[LINEMAX];
char foot_right[LINEMAX];
struct layout layout;
static _BOOL mint_domain;

static char const usage[] =
	"Usage: hyp2gdos {-option} {file.hyp}\n"
	"Prints the file file.hyp to GDOS.\n"
	"-dN      device N (default 21)\n"
	"-tT      print only page titled T\n"
	"-nN      print only page internal named N\n"
	"-i       case insensitive titles\n"
	"-fFILE   load configuration from FILE\n"
	"-pN      set number of start page to N\n"
	"-bPAGE   first PAGE to print\n"
	"-ePAGE   last PAGE to print\n"
	"-sN      skip pages (N=1: odd, N=2: even)\n"
	"-oN      set tab size to N\n"
	"-gLINE   first LINE to print\n"
	"-hLINE   last LINE to print\n"
	"Press & hold SHIFT+SHIFT to cancel printing.\n";

_WORD gdos_device = 21;
long reserve_memory = 102400L;
int dither_method = 1;
_WORD standard_font_size = 10;
_WORD typewriter_font_size = 10;
_BOOL scale_images = TRUE;
_WORD nref_effects = TXT_THICKENED | TXT_UNDERLINED;
_WORD pref_effects = TXT_THICKENED | TXT_UNDERLINED;
_WORD xref_effects = TXT_THICKENED | TXT_UNDERLINED;
int verbose = 1;
static long magicmac = 0;
static unsigned char const macroman_cset[256] = {
	0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x09, 0x0a, 0x7f, 0x7f, 0x0d, 0x7f, 0x7f,
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x1b, 0x7f, 0x7f, 0x7f, 0x7f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
	0x8e, 0x8f, 0x80, 0x90, 0xa5, 0x99, 0x9a, 0xa0, 0x85, 0x83, 0x84, 0xb0, 0x86, 0x87, 0x82, 0x8a,
	0x88, 0x89, 0xa1, 0x8d, 0x8c, 0x8b, 0xa4, 0xa2, 0x95, 0x93, 0x94, 0xb1, 0xa3, 0x97, 0x96, 0x81,
	0xbb, 0xf8, 0x9b, 0x9c, 0xdd, 0xf9, 0xbc, 0x9e, 0xbe, 0xbd, 0xbf, 0xba, 0xb9, 0x7f, 0x92, 0xb2,
	0xdf, 0xf1, 0xf3, 0xf2, 0x9d, 0xe6, 0x7f, 0xe4, 0xe3, 0xe3, 0x7f, 0xa6, 0xa7, 0xea, 0x91, 0xb3,
	0xa8, 0xad, 0xaa, 0xfb, 0x9f, 0xf7, 0x7f, 0xae, 0xaf, 0x7f, 0x20, 0xb6, 0xb7, 0xb8, 0xb5, 0xb4,
	0x2d, 0x2d, 0x22, 0x22, 0x27, 0x27, 0xf6, 0x7f, 0x98, 0xf0, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f
};
int standard_font_id = 12;
int typewriter_font_id = 12;
int x18b56 = 0;
int x18b58 = 1;
_BOOL expand_spaces = TRUE;
_BOOL x18b5c = TRUE;
_BOOL use_standard = TRUE;
int tabsize = 4;
_WORD fonts[] = { 1, 9, 1, 10, 1, 9, 1, 10 };

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

static long get_jar(void)
{
	return *((long *)0x5a0);
}

/* ---------------------------------------------------------------------- */

static _BOOL getcookie(long id, long *value)
{
	long *jar;
	long *ptr;
	
	jar = (long *)Supexec(get_jar);
	ptr = jar;
	if (ptr != 0)
	{
		for (;;)
		{
			if (jar[0] == id)
			{
				if (value)
					*value = jar[1];
				return TRUE;
			}
			if (jar[0] == 0)
				break;
			jar += 2;
		}
	}
	return FALSE;
}

/* ---------------------------------------------------------------------- */

static void make_absolute(Path *filename, const char *cwd)
{
	Path name;
	Path res;
	
	if (is_relative_path(filename->buf))
	{
		get_basename(filename->buf, name.buf);
		append_path(cwd, name.buf, res.buf);
		if (fexists(res.buf))
			pathcopy(filename, &res);
	}
}

/* ---------------------------------------------------------------------- */

__attribute__((format(printf, 1, 2)))
static int debugout(const char *format, ...)
{
	UNUSED(format);
	return 0;
}

/* ---------------------------------------------------------------------- */

int verboseout(const char *format, ...)
{
	va_list args;
	int ret;
	
	ret = 0;
	if (verbose & 1)
	{
		va_start(args, format);
		ret = vfprintf(stdout, format, args);
		fflush(stdout);
		va_end(args);
	}
	return ret;
}

/* ---------------------------------------------------------------------- */

__attribute__((format(printf, 1, 2)))
static int trace(const char *format, ...)
{
	va_list args;
	int ret;
	
	ret = 0;
	if (verbose & 8)
	{
		va_start(args, format);
		ret = vfprintf(stdout, format, args);
		fflush(stdout);
		va_end(args);
	}
	return ret;
}

/* ---------------------------------------------------------------------- */

static void init_wk(_WORD handle)
{
	vswr_mode(handle, MD_REPLACE);
	vsl_color(handle, G_BLACK);
	vsl_ends(handle, 0, 0);
	vsl_width(handle, 1);
	vsl_type(handle, LT_SOLID);
	vst_color(handle, G_BLACK);
	vst_effects(handle, 0);
	vsf_color(handle, G_WHITE);
	vsf_interior(handle, FIS_SOLID);
	vsf_perimeter(handle, 1);
}

/* ---------------------------------------------------------------------- */

static void wk_info(void)
{
	_WORD workout[273];
	_UWORD width;
	_UWORD height;
	_UWORD pix_width;
	_UWORD pix_height;
	long xfac;
	long yfac;
	_WORD hdpi;
	_WORD vdpi;
	long mult;
	_WORD format;
	unsigned long colors;
	
	memset(workout, 0, sizeof(workout));
	vq_extnd(vdihandle, 0, workout);
	width = workout[0];
	height = workout[1];
	pix_width = workout[3];
	pix_height = workout[4];
	memset(workout, 0, sizeof(workout));
	vq_extnd(vdihandle, 1, workout);
	hdpi = workout[23];
	vdpi = workout[24];
	switch (workout[20])
	{
	case 1:
		mult = 10;
		break;
	case 2:
		mult = 100;
		break;
	case 3:
		mult = 1000;
		break;
	default:
		mult = 1;
		break;
	}
	if (mult == 1)
	{
		xfac = ((long)width * pix_width) / 1000;
		yfac = ((long)height * pix_height) / 1000;
	} else
	{
		xfac = (((long)width * (_UWORD)workout[21]) / 1000) / mult;
		yfac = (((long)height * (_UWORD)workout[22]) / 1000) / mult;
	}

	memset(workout, 0, sizeof(workout));
	vq_scrninfo(vdihandle, workout);
	format = workout[0];
	colors = *((unsigned long *)&workout[3]);
	verboseout("print device: %d, ", gdos_device);
	if (colors == 0)
	{
		verboseout(">2*10^31 colors");
	} else
	{
		verboseout("%lu colors", colors);
	}
	verboseout(", ");
	switch (format)
	{
	case 0:
		verboseout("interleaved format");
		break;
	case 1:
		verboseout("standard format");
		break;
	case 2:
		verboseout("packed pixel format");
		break;
	case -1:
		verboseout("hidden format");
		break;
	default:
		verboseout("undefined format");
		break;
	}
	verboseout("\n");
	verboseout("print device: ");
	verboseout("%ld x %ld mm", xfac, yfac);
	verboseout(", ");
	verboseout("%u x %u dpi", hdpi, vdpi);
	verboseout(", ");
	verboseout("%u x %u pixel", width, height);
	verboseout("\n");
}

/* ---------------------------------------------------------------------- */

size_t conv_macroman(const char *src, char *dst)
{
	char *start;
	const unsigned char (*table)[256];
	
	start = dst;
	table = &macroman_cset;
	while (*src != '\0')
	{
		*dst++ = (*table)[(unsigned char)*src++];
	}
	*dst = '\0';
	return dst - start;
}

/* ---------------------------------------------------------------------- */

int get_effects(struct hypfile *hyp, hyp_nodenr node, int *a3, _WORD *effects)
{
	int type;
	
	type = get_nodetype(hyp, node);
	*a3 = TRUE;
	switch (type)
	{
	case HYP_NODE_POPUP:
		*effects = pref_effects;
		break;
	case HYP_NODE_SYSTEM_ARGUMENT:
	case HYP_NODE_REXX_SCRIPT:
	case HYP_NODE_REXX_COMMAND:
	case HYP_NODE_QUIT:
	/* case HYP_NODE_CLOSE: */ /* BUG: missing */
		*effects = xref_effects;
		break;
	default:
		*effects = nref_effects;
		break;
	}
	return 0;
}

/* ---------------------------------------------------------------------- */

static void reset_abort(void)
{
	abort_flag = FALSE;
}

/* ---------------------------------------------------------------------- */

#ifdef __PUREC__
void unused(void)
{
	printf("n");
}
#endif

/* ---------------------------------------------------------------------- */

_BOOL should_abort(void)
{
	if (abort_flag == FALSE)
	{
		abort_flag = interrupted();
		if (abort_flag)
			fprintf(stderr, "Hyp2GDOS: Printing will be canceled!\n");
	}
	return abort_flag;
}

/* ---------------------------------------------------------------------- */

static _WORD find_font(const char *fontname)
{
	char name[80];
	char *end;
	_WORD id;
	_WORD idx;
	
	if (*fontname != '\0')
	{
		id = (_WORD)strtol(fontname, &end, 10);
		while (*end == ' ' || *end == '\t')
			end++;
		if (id != 0 && *end == '\0')
			return id;
		for (idx = 1; idx <= num_loaded_fonts; idx++)
		{
			id = vqt_name(vdihandle, idx, name);
			name[32] = '\0';
			debugout("%d %s\n", id, name);
			if (strnicmp(fontname, name, 16) == 0)
			{
				debugout("%s => %d\n", name, id);
				return id;
			}
		}
	}
	return 1;
}

/* ---------------------------------------------------------------------- */

#ifdef __PUREC__
void unused2(void)
{
	static const char *n = "n";
	printf(n);
	printf(" %d", 0);
}
int (*p_get_effects)(struct hypfile *hyp, hyp_nodenr node, int *a3, _WORD *effects) = get_effects;
#endif

/* ---------------------------------------------------------------------- */

static int printfile(const Path *filename)
{
	_WORD workin[11] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 };
	_WORD workout[57];
	struct pageinfo page;
	struct hypfile *hyp;
	char *title;
	_WORD page_num;
	Path pathbuf;
	int retcode;
	long memavail;
	void *mem;
	long node; /* FIXME: should be hyp_nodenr */
	int found;
	
	strmcpy(filename->buf, pathbuf.buf, (int)sizeof(pathbuf.buf));
	if ((hyp = hyp_new()) == NULL)
	{
		fprintf(stderr, "Not enough memory!\n");
		retcode = 4;
	} else
	{
		if ((retcode = hyp_load(hyp, filename)) != 0)
		{
			fprintf(stderr, "Can't load file \"%s\"!\n", pathbuf.buf);
		} else
		{
			memavail = (long)Malloc(-1);
			if (memavail > reserve_memory)
				memavail = reserve_memory;
			mem = (void *)Malloc(memavail);
			workin[0] = gdos_device;
			vdihandle = -1;
			v_opnwk(workin, &vdihandle, workout);
			if (mem)
				Mfree(mem);
			if (vdihandle <= 0)
			{
				fprintf(stderr, "Can't open printer %d!\n", gdos_device);
				retcode = 1;
			} else
			{
				x19d24 = 0;
				num_loaded_fonts = workout[10];
				num_loaded_fonts += vst_load_fonts(vdihandle, 0);
				if (x19d28 == 0 && num_loaded_fonts == 0)
				{
					fprintf(stderr, "No fonts!\n");
					retcode = 2;
				} else
				{
					standard_font_id = new_font_id();
					typewriter_font_id = new_font_id();
					set_font(standard_font_id, find_font(standard_font), standard_font_size);
					set_font(typewriter_font_id, find_font(typewriter_font), typewriter_font_size);
					scale_flag = vdi_can_scale_bitmaps(vdihandle);
					/* BUG: interrupted() only checks for shift */
					verboseout("Hyp2GDOS: Hold %s to cancel printing.\n", magicmac && (((long *)magicmac)[1] & 2) == 0 ? "Command-B" : "SHIFT+SHIFT");
					wk_info();
					verboseout("print document \"%s\"\n", pathbuf.buf);
					init_wk(vdihandle);
					print_handle = vdihandle;
					reset_abort();
					if (scale_images)
					{
						hyp->flags |= SCALE_IMAGES;
					} else
					{
						hyp->flags &= ~SCALE_IMAGES;
					}
					hyp_init_pageinfo(&page, hyp, HYP_PIC_FONTW, HYP_PIC_FONTH, FALSE);
					page_num = layout.first_page_num - 1;
					if (pagename[0] != '\0')
					{
						if (!pagename_is_title)
						{
							verboseout("looking for page \"%s\"\n", pagename);
							node = hyp_find_pagename(page.hyp, pagename);
							if (node < 0)
							{
								fprintf(stderr, "Page \"%s\" not found!\n", pagename);
							} else
							{
								if (hyp_load_page(&page, NULL, node, FALSE, NULL))
								{
									fprintf(stderr, "Couldn't load page \"%s\"!\n", pagename);
								} else
								{
									x14db6(&page, &page_num, &standard_font_id);
								}
							}
						} else
						{
							verboseout("looking for title \"%s\"\n", pagename);
							node = -1;
							found = 0;
							while ((node = hyp_next_textnode(page.hyp, node, TRUE)) != HYP_NOINDEX)
							{
								if ((title = hyp_get_window_title(&page, node)) != NULL)
								{
									if ((case_insensitive ? stricmp(title, pagename) : strncmp(title, pagename, strlen(pagename))) == 0)
									{
										found++;
										if (hyp_load_page(&page, NULL, node, FALSE, NULL))
										{
											fprintf(stderr, "Couldn't load page \"%s\"!\n", pagename);
										} else
										{
											x14db6(&page, &page_num, &standard_font_id);
											if (page_num >= layout.last_page)
												break;
										}
									}
								}
								if (should_abort())
									break;
							}
							if (found == 0)
							{
								fprintf(stderr, "Title \"%s\" not found!\n", pagename);
							}
						}
					} else
					{
						trace(">>>Try to print document\n");
						node = -1;
						while ((node = hyp_next_textnode(page.hyp, node, TRUE)) != HYP_NOINDEX)
						{
							trace(">>>Try to load page (index=%d)\n", (int)node);
							if (hyp_load_page(&page, NULL, node, FALSE, NULL))
							{
								fprintf(stderr, "Couldn't load page \"%s\"!\n", pagename);
							} else
							{
								trace(">>>Try to print page (index=%d)\n", (int)node);
								x14db6(&page, &page_num, &standard_font_id);
								if (page_num >= layout.last_page)
									break;
							}
							if (should_abort())
								break;
						}
					}
					hyp_free_pageinfo(&page);
					retcode = EXIT_SUCCESS;
				}
			}
		}
	}
	
	if (vdihandle > 0)
	{
		vst_unload_fonts(vdihandle, 0);
		v_clswk(vdihandle);
	}
	if (hyp != NULL)
		hyp_delete(&hyp);

	return retcode;
}

/* ---------------------------------------------------------------------- */

static void parseline(char *line)
{
	int i;
	long len;
	char *p;
	
	static const char *const keywords[] = {
		"H2G_DEVICE=",
		"H2G_BORDER_LEFT=",
		"H2G_BORDER_TOP=",
		"H2G_BORDER_RIGHT=",
		"H2G_BORDER_BOTTOM=",
		"H2G_ADD_HEAD=",
		"H2G_HEAD_LEFT=",
		"H2G_HEAD_CENTER=",
		"H2G_HEAD_RIGHT=",
		"H2G_HEAD_SEP=",
		"H2G_ADD_FOOT=",
		"H2G_FOOT_LEFT=",
		"H2G_FOOT_CENTER=",
		"H2G_FOOT_RIGHT=",
		"H2G_FOOT_SEP=",
		"H2G_STANDARD_FONT=",
		"H2G_TYPEWRITER_FONT=",
		"H2G_SWAP_LAYOUT=",
		"H2G_EXPAND_SPACES=",
		"H2G_SCALE_IMAGES=",
		"H2G_SKIP_UDO_HEADER=",
		"H2G_SHOW_BORDERS=",
		"H2G_RESERVE_MEMORY=",
		"H2G_DITHER_METHOD=",
		"H2G_USE_STANDARD=",
		"H2G_TABSIZE=",
		"H2G_NREF_EFFECTS=",
		"H2G_PREF_EFFECTS=",
		"H2G_XREF_EFFECTS=",
		"HYPFOLD=",
		"H2G_VERBOUS=",
		NULL
	};
	
	i = 0;
	for (;;)
	{
		if (keywords[i] == NULL)
			return;
		len = strlen(keywords[i]);
		if (strncmp(line, keywords[i], len) == 0)
			break;
		i++;
	}
	line += len;
	switch (i)
	{
	case 0:
		gdos_device = atoi(line);
		break;
	
	case 1:
		layout.border_left = atol(line) * 1000;
		break;
	
	case 2:
		layout.border_top = atol(line) * 1000;
		break;
	
	case 3:
		layout.border_right = atol(line) * 1000;
		break;
	
	case 4:
		layout.border_bottom = atol(line) * 1000;
		break;
	
	case 5:
		layout.add_head = atoi(line) != 0;
		break;
	
	case 6:
		strcpy(head_left, line);
		break;
	
	case 7:
		strcpy(head_center, line);
		break;
	
	case 8:
		strcpy(head_right, line);
		break;
	
	case 9:
		layout.head_sep = atoi(line);
		break;
	
	case 10:
		layout.add_foot = atoi(line) != 0;
		break;
	
	case 11:
		strcpy(foot_left, line);
		break;
	
	case 12:
		strcpy(foot_center, line);
		break;
	
	case 13:
		strcpy(foot_right, line);
		break;
	
	case 14:
		layout.foot_sep = atoi(line);
		break;
	
	case 15:
		strcpy(standard_font, line);
		p = strrchr(standard_font, ',');
		if (p != NULL)
		{
			*p = '\0';
			standard_font_size = atoi(p + 1);
		}
		break;
	
	case 16:
		strcpy(typewriter_font, line);
		p = strrchr(typewriter_font, ',');
		if (p != NULL)
		{
			*p = '\0';
			typewriter_font_size = atoi(p + 1);
		}
		break;
	
	case 17:
		layout.swap_layout = atoi(line) != 0;
		break;
	
	case 18:
		expand_spaces = atoi(line) != 0;
		break;
	
	case 19:
		scale_images = atoi(line) != 0;
		break;
	
	case 20:
		layout.skip_udo_header = atoi(line) != 0;
		break;
	
	case 21:
		layout.show_borders = atoi(line);
		break;
	
	case 22:
		reserve_memory = atol(line);
		break;
	
	case 23:
		dither_method = atoi(line);
		break;
	
	case 24:
		use_standard = atoi(line);
		break;
	
	case 25:
		tabsize = atoi(line);
		break;
	
	case 26:
		nref_effects = atoi(line) & (TXT_THICKENED | TXT_UNDERLINED | TXT_SKEWED);
		break;
	
	case 27:
		pref_effects = atoi(line) & (TXT_THICKENED | TXT_UNDERLINED | TXT_SKEWED);
		break;
	
	case 28:
		xref_effects = atoi(line) & (TXT_THICKENED | TXT_UNDERLINED | TXT_SKEWED);
		break;
	
	case 29:
		setpath(line, hypfold);
		break;
	
	case 30:
		verbose = atoi(line);
		break;
	}	
}

/* ---------------------------------------------------------------------- */

static _BOOL parseinf(const char *filename)
{
	struct file *fp;
	char line[LINEMAX];
	char *lineptr;
	
	fp = openfile(filename);
	if (fp == NULL)
		return TRUE;
	while (readline(fp, line, sizeof(line)) >= 0)
	{
		lineptr = line;
		while (*lineptr != '\0' && (*lineptr == ' ' || *lineptr == '\t'))
			lineptr++;
		if (*lineptr != '\0' && *lineptr != '#')
			parseline(lineptr);
	}
	closefile(fp);
	return FALSE;
}

/* ---------------------------------------------------------------------- */

static void get_stguideinf_path(char *path)
{
	if (gethomedir(path) == NULL)
		get_bootdir(path);
	strcat(path, "st-guide.inf");
}

/* ---------------------------------------------------------------------- */

static void set_domain(void)
{
	Pdomain(1);
	mint_domain = Pdomain(-1) == 1;
}

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

int main(int argc, char **argv)
{
	char stguide_inf[MAXPATH];
	Path filename;
	char cwd[MAXPATH];
	int i;
	char *arg;
	int retcode;
	
	const char *id = "@(#)hyp2gdos v1.1 [May 31 1997], (c) Martin Osieka (\\/)";
	(void)id;

	retcode = EXIT_SUCCESS;
	set_domain();
	get_cwd(cwd);
	getcookie(0x4D674D63L, &magicmac); /* 'MgMc' */

	layout.first_page_num = 1;
	layout.first_page = 1;
	layout.last_page = 32767;
	layout.head_left_str = head_left;
	layout.head_center_str = head_center;
	layout.head_right_str = head_right;
	layout.foot_left_str = foot_left;
	layout.foot_center_str = foot_center;
	layout.foot_right_str = foot_right;
	
	get_stguideinf_path(stguide_inf);
	parseinf(stguide_inf);
	
	trace(">>>scan arguments\n");
	trace(">>>argC = %d\n", argc);
	
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		trace(">>>argV[ %d] = \"%s\"\n", i, arg);
		if (*arg++ != '-')
			break;
		switch (*arg++)
		{
		case 'd':
		case 'D':
			gdos_device = atoi(arg);
			break;
		case 't':
		case 'T':
			strcpy(pagename, arg);
			pagename_is_title = TRUE;
			break;
		case 'n':
		case 'N':
			strcpy(pagename, arg);
			pagename_is_title = FALSE;
			break;
		case 'i':
		case 'I':
			case_insensitive = TRUE;
			break;
		case 'f':
		case 'F':
			parseinf(arg);
			break;
		case 'p':
		case 'P':
			layout.first_page_num = atoi(arg);
			if (layout.first_page_num < 1)
				layout.first_page_num = 1;
			break;
		case 'b':
		case 'B':
			layout.first_page = atoi(arg);
			break;
		case 'e':
		case 'E':
			layout.last_page = atoi(arg);
			break;
		case 's':
		case 'S':
			layout.skip_pages = atoi(arg);
			break;
		case 'o':
		case 'O':
			tabsize = atoi(arg);
			break;
		case 'g':
		case 'G':
			layout.first_line = atoi(arg);
			break;
		case 'h':
		case 'H':
			layout.last_line = atoi(arg);
			break;
		case 'v':
		case 'V':
			verbose = atoi(arg);
			break;
		}
	}
	
	if (layout.first_page < layout.first_page_num)
		layout.first_page = layout.first_page_num;
	
	if (i >= argc)
	{
		fprintf(stderr, usage);
		return EXIT_FAILURE;
	}
	
	while (i < argc)
	{
		retcode = EXIT_FAILURE;
		if (setpath(argv[i], filename.buf) != 0)
		{
#ifdef __GNUC__
			fprintf(stderr, "error: unknown file %s!\n", argv[i]);
#else
			fprintf(stderr, "error: unknown file %d!\n", argv[i]); /* BUG: wrong format */
#endif
			break;
		} else
		{
			make_absolute(&filename, cwd);
			retcode = printfile(&filename);
			if (retcode != EXIT_SUCCESS)
			{
				fprintf(stderr, "error: %d!\n", retcode);
				break;
			}
		}
		++i;
	}

	return retcode;
}
