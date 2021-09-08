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
	short o6;
	short o8;
	long o10;
	long pos;
	long avail;
	unsigned char buf[1024];
};

typedef struct {
	char buf[MAXPATH];
} Path;

struct hypfile {
	/*   0 */ unsigned short flags;
#define SCALE_IMAGES 0x02
#define FLAG_20      0x20
#define FLAG_40      0x40
#define FLAG_80      0x80

	/*   2 */ Path filename;
	/* 258 */ FILE *fp;
	/* 262 */ HYP_HEADER header;
	/* 274 */ void *indexdata;
	/* 278 */ INDEX_ENTRY **indextable;
	/* 282 */ void *o282;
	/* 286 */ void *o286;
	/* 290 */ short o290;
	/* 292 */ short o292;
	/* 294 */ short o294;
	/* 296 */ short o296;
	/* 298 */ void *o298;
	/* 302 */ void *o302;
	/* 306 */ short o306;
};


struct font {
	_BOOL used;
};

struct x76 {
	struct hypfile *hyp;
	char o4[72];
};

struct options {
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

extern struct font fonttable[8];
extern _WORD x1d9c4;
extern struct options options;
extern char hypfold[MAXPATH];


size_t conv_macroman(const char *src, char *dst);
int get_effects(struct hypfile *hyp, hyp_nodenr node, int *a3, _WORD *effects);
int get_nodetype(struct hypfile *hyp, hyp_nodenr node);
_BOOL should_abort(void);

int verboseout(const char *format, ...) __attribute__((format(printf, 1, 2)));



struct file *openfile(const char *filename);
int closefile(struct file *fp);
long readline(struct file *fp, char *line, size_t len);

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
int x16fd8(void);
int x17008(int fontidx, _WORD font_id, _WORD size);
_BOOL can_scale_bitmaps(_WORD handle);
int x16734(struct x76 *p, struct hypfile *hyp, _WORD font_width, _WORD font_height, int x);
hyp_nodenr hyp_find_pagename(struct hypfile *hyp, const char *pagename);
int hyp_load_page(struct x76 *x, void *y, hyp_nodenr node, int z, void *a);
int x14db6(struct x76 *x, _WORD *page_num, int *font_idx);
hyp_nodenr x16842(struct hypfile *hyp, hyp_nodenr node, int direction);
char *hyp_get_window_title(struct x76 *x, hyp_nodenr nodenr);
void x16768(struct x76 *x);
FILE *x14f38(const Path *filename);
void conv_nodename(unsigned char os, char *name);
