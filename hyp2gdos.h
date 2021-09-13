#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef __PUREC__
#include <tos.h>
#include <aes.h>
#include <vdi.h>
#else
#include <osbind.h>
#include <mintbind.h>
#include <gem.h>
#endif

/*
 * Things that should be defined in header files,
 * but aren't always
 */
#ifndef FALSE
#  define FALSE 0
#  define TRUE  1
#endif

#ifndef _BOOL
#  define _BOOL int
#endif

#ifndef _WORD
#  ifdef __GNUC__
#    define _WORD short
#    define _UWORD unsigned short
#  else
#    define _WORD int
#    define _UWORD unsigned int
#  endif
#endif

#ifndef __attribute__
#  ifndef __GNUC__
#    define __attribute__(x)
#  endif
#endif

#ifndef UNUSED
# define UNUSED(x) (void)(x)
#endif

/* text effects */
#ifndef TXT_THICKENED
#define TXT_NORMAL       0x0000
#define TXT_THICKENED    0x0001
#define TXT_LIGHT        0x0002
#define TXT_SKEWED       0x0004
#define TXT_UNDERLINED   0x0008
#define TXT_OUTLINED     0x0010
#define TXT_SHADOWED     0x0020
#endif
#ifndef LT_SOLID
#define LT_SOLID		SOLID
#define LT_DOTTED		DOT
#endif
#ifndef TA_LEFT
#  define TA_LEFT         	0 /* horizontal */
#  define TA_TOP          	5
#endif
#ifndef G_BLACK
#  define G_WHITE 0
#  define G_BLACK 1
#endif

#include "hyp.h"


#define MAXPATH 256
#define LINEMAX 128

struct file {
	FILE *fp;
	_BOOL eof;
	_BOOL overflow;
	int lastc;
	long lineno;
	long pos;
	long avail;
	unsigned char buf[1024];
};

typedef struct {
	char buf[MAXPATH];
} Path;

struct hypfile {
	/*   0 */ unsigned short flags;
#define IGNORE_IMAGES	0x01
#define SCALE_IMAGES 	0x02
#define FLAG_04         0x04
#define FORCE_RELOAD    0x20
#define KEEP_OPEN       0x40
#define INDEX_LOADED    0x80

	/*   2 */ Path filename;
	/* 258 */ FILE *fp;
	/* 262 */ HYP_HEADER header;
	/* 274 */ void *indexdata;
	/* 278 */ INDEX_ENTRY **indextable;
	/* 282 */ void *extdata;
	/* 286 */ long extdatasize;
	/* 290 */ hyp_nodenr num_internal_nodes;
	/* 292 */ hyp_nodenr main_page;
	/* 294 */ hyp_nodenr index_page;
	/* 296 */ hyp_nodenr help_page;
	/* 298 */ void *dither_params;
	/* 302 */ void *o302;
	/* 306 */ short width;
};


struct histent {
	Path filename;
	hyp_nodenr nodenr;
	char window_title[256];
	long lineno;
	long o518;
	long o522[4];
};

#define HISTSIZE 16
struct history {
	short size;
	short curr;
	struct histent entry[HISTSIZE];
};

struct node {
	struct node *picdata;
	hyp_nodenr nodenr;
	long datalen;
	void *dither_params;
	char data[0];
};


struct pageinfo {
	/*  0 */ struct hypfile *hyp;
	/*  4 */ struct node *node;
	/*  8 */ char **text;
	/* 12 */ short num_lines;
	/* 14 */ int max_text_width;
	/* 16 */ short image_lines;
	/* 18 */ char *window_title;
	/* 22 */ long o22[4];
	/* 38 */ long o38[4];
	/* 54 */ short o54;
	/* 56 */ long lineno;
	/* 60 */ long o60;
	/* 64 */ short font_width;
	/* 66 */ short font_height;
	/* 68 */ struct history *hist;
	/* 72 */ char *errmsg;
};

struct layout {
	/*  0 */ _WORD x1a02a;
	/*  2 */ long border_left;
	/*  6 */ long border_top;
	/* 10 */ long border_right;
	/* 14 */ long border_bottom;
	/* 18 */ _BOOL add_head;
	/* 20 */ char *head_left_str;
	/* 24 */ char *head_center_str;
	/* 28 */ char *head_right_str;
	/* 32 */ _BOOL head_sep;
	/* 34 */ _BOOL add_foot;
	/* 36 */ char *foot_left_str;
	/* 40 */ char *foot_center_str;
	/* 44 */ char *foot_right_str;
	/* 48 */ _BOOL foot_sep;
	/* 50 */ _BOOL swap_layout;
	/* 52 */ _BOOL skip_udo_header;
	/* 54 */ int show_borders;
	/* 56 */ int first_page_num;
	/* 58 */ int first_page;
	/* 60 */ int last_page;
	/* 62 */ int skip_pages;
	/* 64 */ long first_line;
	/* 68 */ long last_line;
	/* 72 */ _BOOL skip;
};

extern struct layout layout;
extern int hyp_errno;
extern char hypfold[MAXPATH];
extern char all_ref[MAXPATH];
extern _BOOL did_print_already;

struct fontinfo {
	int standard_font_id;
	int typewriter_font_id;
	int unknown;
	_WORD text_color;
	_BOOL expand_spaces;
	_BOOL image_borders;
	_BOOL use_standard;
	int tabsize;
	const char *sample;
	int (*p_get_effects)(struct hypfile *hyp, hyp_nodenr node, _WORD *color, _WORD *effects);
};


/*
 * main.c
 */
int get_effects(struct hypfile *hyp, hyp_nodenr node, _WORD *color, _WORD *effects);
_BOOL should_abort(void);
int verboseout(const char *format, ...) __attribute__((format(printf, 1, 2)));


/*
 * print.c
 */
int print_page(struct pageinfo *page, _WORD *page_num, struct fontinfo *fonts);


/*
 * file.c
 */
struct file *openfile(const char *filename);
int closefile(struct file *fp);
long readline(struct file *fp, char *line, size_t len);


/*
 * myvdi.c
 */
extern _WORD print_handle;

int new_font_id(void);
int set_font(int fontidx, _WORD font_id, _WORD size);
struct vdi;
#if defined(__PUREC__) && !defined(__HVDI_IMPLEMENTATION__)
struct vdi { int dummy; };
#endif
_WORD vdi_get_handle(struct vdi *v);
struct vdi *vdi_alloc(void);
void vdi_free(struct vdi *v);
void vdi_ref(struct vdi *v);
void vdi_unref(struct vdi *v);
void vdi_line_attributes(struct vdi *v, _WORD line_color, _WORD line_mode, _WORD line_type, _WORD line_width);
void vdi_draw_line(struct vdi *v, _WORD *pxy);
void vdi_draw_arrowed(struct vdi *v, _WORD *pxy, _WORD ends);
void vdi_draw_rect(struct vdi *v, const GRECT *gr);
void vdi_draw_rounded_rect(struct vdi *v, const GRECT *gr, _WORD unused);
void vdi_fill_attributes(struct vdi *v, _WORD color, _WORD mode, _WORD pattern);
void vdi_draw_bar(struct vdi *v, const GRECT *gr);
void vdi_draw_rounded_box(struct vdi *v, const GRECT *gr, _WORD unused);
void vdi_text_attributes(struct vdi *v, _WORD text_color, _WORD text_mode, _WORD text_effects, _WORD font_idx);
void vdi_get_fontwidth(struct vdi *v, _WORD *cell_width, _WORD *cell_height);
void vdi_draw_text(struct vdi *v, _WORD x, _WORD y, char *str, _WORD len);
_WORD vdi_get_textwidth(struct vdi *v, char *str, _WORD len);
void vdi_defaults(struct vdi *v);
void vdi_clip(struct vdi *v, const GRECT *gr);
void v_clip(_WORD handle, const GRECT *gr);
int vdi_draw_bitmap(struct vdi *v, void *data,
	_WORD x, _WORD y, _WORD dstwidth, _WORD dstheight,
	_WORD srcwidth, _WORD srcheight,
	_WORD planes, _WORD mode,
	_WORD unused1, _WORD unused2);
int vdi_draw_image(struct vdi *v, void *data,
	_WORD x, _WORD y, _WORD dstwidth, _WORD dstheight,
	_WORD srcwidth, _WORD srcheight,
	_WORD planes, _WORD mode,
	_WORD unused1, _WORD unused2, const unsigned short params[][3]);
void vdi_get_outputsize(_WORD handle, GRECT *gr);
void vdi_get_pagesize(_WORD handle, GRECT *gr);
_BOOL vdi_can_scale_bitmaps(_WORD handle);
void vdi_get_dpi(_WORD handle, _WORD *hdpi, _WORD *vdpi);
void vdi_clear_page(_WORD handle);

#ifndef __PORTVDI_H__
void vq_scrninfo(_WORD handle, _WORD *workout);
#endif


/*
 * util1.c
 */
int set_ref_string(const char *str, _BOOL caseinsens, _BOOL d1);
int ref_strcmp(const char *str1, const char *str2);
void set_lrect(long *dst, long a, long b, long c, long d);
_WORD rc_intersect(const GRECT *r1, GRECT *r2);
void rc_shrink(GRECT *gr, _WORD dx, _WORD dy);
int clearpath(Path *path);
const char *mybasename(const char *filename);
_BOOL get_name(char *name, const char *filename);
_BOOL is_relative_path(const char *filename);
_BOOL get_basename(const char *filename, char *name);
_BOOL get_dirname(const char *filename, char *name);
_BOOL append_path(const char *dirname, const char *filename, char *buf);
_BOOL strmcpy(const char *src, char *dst, int len);
int setpath(const char *value, char *path);
FILE *myfopen(const char *filename, const char *mode);
int pathcopy(Path *dst, const Path *src);
_BOOL pathequal(const Path *p1, const Path *p2);
int get_cwd(char *path);
_BOOL fexists(const char *filename);


/*
 * util2.c
 */
char *get_bootdir(char *path);
char *gethomedir(char *path);
void get_curdir(char *path);


/*
 * util3.c
 */
void *xmalloc(size_t size);
void xfree(void *ptr);


/*
 * util4.c
 */
_BOOL interrupted(void);
void get_date(int *day, int *month, int *year);


/*
 * hyp.c
 */
struct hypfile *hyp_new(void);
void hyp_delete(struct hypfile **hyp);
void hyp_free(struct hypfile *hyp);
int hyp_load(struct hypfile *hyp, const Path *filename);
int hyp_init_pageinfo(struct pageinfo *page, struct hypfile *hyp, _WORD font_width, _WORD font_height, _BOOL history);
hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename);
int hyp_load_page(struct pageinfo *page, const Path *filename, hyp_nodenr node, _BOOL addhist, long *lineno);
hyp_nodenr hyp_next_textnode(struct hypfile *hyp, hyp_nodenr node, _BOOL direction);
char *hyp_get_window_title(struct pageinfo *page, hyp_nodenr nodenr);
void hyp_free_pageinfo(struct pageinfo *page);
FILE *hyp_find_in_hypfold(Path *filename);
size_t conv_nodename(char os, char *name);
void *hyp_find_extheader(struct hypfile *hyp, hyp_ext_header type);
int get_nodetype(struct hypfile *hyp, hyp_nodenr node);


int hyp_load_node(struct hypfile *hyp, struct node **node, hyp_nodenr nodenr);
void hyp_free_node(struct hypfile *hyp, struct node **node);
char *decode_char(char *data, short *val);
char *dec_255_decode(char *data, short *val);
char *dec_255_encode(char *data, short val);

int x18118(char *data, short width, short height, short planes);
long hyp_get_linewidth(struct pageinfo *page, long lineno);

char hyp_get_char_at(struct pageinfo *page, long lineno, long offset);
int conv_macroman(const char *src, char *dst);
