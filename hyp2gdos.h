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

#ifndef UNUSED
# define UNUSED(x) (void)(x)
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
#define FLAG_04      0x04
#define FLAG_20      0x20
#define FLAG_40      0x40
#define FLAG_80      0x80

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
	/* 298 */ void *o298;
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

struct xo4 {
	struct xo4 *picdata;
	hyp_nodenr nodenr;
	long datalen;
	void *o10;
	char data[0];
};


struct pageinfo {
	/*  0 */ struct hypfile *hyp;
	/*  4 */ struct xo4 *o4;
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
	_WORD x1a02a;
	long border_left;
	long border_top;
	long border_right;
	long border_bottom;
	_BOOL add_head;
	char *head_left_str;
	char *head_center_str;
	char *head_right_str;
	_BOOL head_sep;
	_BOOL add_foot;
	char *foot_left_str;
	char *foot_center_str;
	char *foot_right_str;
	_BOOL foot_sep;
	_BOOL swap_layout;
	_BOOL skip_udo_header;
	int show_borders;
	int first_page_num;
	int first_page;
	int last_page;
	int skip_pages;
	long first_line;
	long last_line;
};

extern struct layout layout;
extern int hyp_errno;
extern char hypfold[MAXPATH];
extern char katalog[MAXPATH];
extern _WORD fonts[];


size_t conv_macroman(const char *src, char *dst);
int get_effects(struct hypfile *hyp, hyp_nodenr node, int *a3, _WORD *effects);
int get_nodetype(struct hypfile *hyp, hyp_nodenr node);
_BOOL should_abort(void);

int verboseout(const char *format, ...) __attribute__((format(printf, 1, 2)));


/*
 * file.c
 */
struct file *openfile(const char *filename);
int closefile(struct file *fp);
long readline(struct file *fp, char *line, size_t len);


/*
 * font.c
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
void vdi_draw_rounded_rect(struct vdi *v, const GRECT *gr);
void vdi_fill_attributes(struct vdi *v, _WORD color, _WORD mode, _WORD pattern);
void vdi_draw_bar(struct vdi *v, const GRECT *gr);
void vdi_draw_rounded_box(struct vdi *v, const GRECT *gr);
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
	_WORD unused1, _WORD unused2, void *mask);


/*
 * util.c
 */
void *xmalloc(size_t size);
void xfree(void *ptr);
_BOOL interrupted(void);

const char *mybasename(const char *filename);
int get_cwd(char *path);
void get_curdir(char *path);
char *get_bootdir(char *path);
char *gethomedir(char *path);
int setpath(const char *value, char *path);
_BOOL is_relative_path(const char *filename);
_BOOL get_basename(const char *filename, char *name);
_BOOL get_dirname(const char *filename, char *name);
_BOOL get_name(char *name, const char *filename);
_BOOL append_path(const char *dirname, const char *filename, char *buf);
_BOOL strmcpy(const char *src, char *dst, int len);
FILE *myfopen(const char *filename, const char *mode);
int clearpath(Path *path);
_BOOL pathequal(const Path *p1, const Path *p2);
int pathcopy(Path *dst, const Path *src);
_BOOL fexists(const char *filename);

#ifndef __PORTVDI_H__
void vq_scrninfo(_WORD handle, _WORD *workout);
#endif


struct hypfile *hyp_new(void);
void hyp_delete(struct hypfile **hyp);
void hyp_free(struct hypfile *hyp);
int hyp_load(struct hypfile *hyp, const Path *filename);
_BOOL can_scale_bitmaps(_WORD handle);
int hyp_init_pageinfo(struct pageinfo *page, struct hypfile *hyp, _WORD font_width, _WORD font_height, _BOOL history);
hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename);
int hyp_load_page(struct pageinfo *page, const Path *filename, hyp_nodenr node, _BOOL addhist, long *lineno);
int x14db6(struct pageinfo *page, _WORD *page_num, int *font_idx);
hyp_nodenr hyp_next_textnode(struct hypfile *hyp, hyp_nodenr node, _BOOL direction);
char *hyp_get_window_title(struct pageinfo *page, hyp_nodenr nodenr);
void hyp_free_pageinfo(struct pageinfo *page);
FILE *x14f38(const Path *filename);
size_t conv_nodename(unsigned char os, char *name);
void *hyp_find_extheader(struct hypfile *hyp, hyp_ext_header type);


void x183a6(long *dst, long a, long b, long c, long d);
int x1837c(char *str, char *end);
int x18352(const char *str, int d3, int d1);
int x15132(struct hypfile *hyp, struct xo4 **data, hyp_nodenr node);
void x1509e(struct hypfile *hyp, struct xo4 **data);
char *dec_255_decode(char *data, short *val);
char *dec_255_encode(char *data, short val);

int x18118(char *data, short width, short height, short planes);
long hyp_get_linewidth(struct pageinfo *page, long lineno);

char hyp_get_char_at(struct pageinfo *page, long lineno, long offset);
