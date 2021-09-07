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

/* Valid values for INDEX_ENTRY->type */
typedef enum {
	HYP_NODE_INTERNAL = 0,
	HYP_NODE_POPUP = 1,
	HYP_NODE_EXTERNAL_REF = 2,
	HYP_NODE_IMAGE = 3,
	HYP_NODE_SYSTEM_ARGUMENT = 4,
	HYP_NODE_REXX_SCRIPT = 5,
	HYP_NODE_REXX_COMMAND = 6,
	HYP_NODE_QUIT = 7,
	HYP_NODE_CLOSE = 8,
	HYP_NODE_EOF = 0xff
} hyp_indextype;
#define HYP_NODE_IS_TEXT(type) ((type) <= HYP_NODE_POPUP)

typedef short hyp_nodenr;

typedef struct
{
	unsigned char length;       /* Length */
	/* hyp_indextype */ char type;         /* Type */
	unsigned long seek_offset;  /* File seek offset*/
	unsigned short comp_diff;   /* Difference packed/unpacked in bytes */
	hyp_nodenr next;            /* Index of successor entry */
	hyp_nodenr previous;        /* Index of predecessor entry */
	hyp_nodenr toc_index;       /* Index of directory/parent entry */
	unsigned char name[1];      /* First character of 0-terminated entry name */
} INDEX_ENTRY;

struct hypfile {
	/*   0 */ unsigned short flags;
#define SCALE_IMAGES 0x02

	/* 270 */ hyp_nodenr num_nodes;
	/* 278 */ INDEX_ENTRY **nodes;
};


struct font {
	_BOOL used;
};

struct x76 {
	void *o0;
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
void hyp_delete(struct hypfile *hyp);
void hyp_free(struct hypfile **hyp);
int hyp_load(struct hypfile *hyp, const char *filename);
int x16fd8(void);
int x17008(int fontidx, _WORD font_id, _WORD size);
_BOOL can_scale_bitmaps(_WORD handle);
int x16734(struct x76 *p, struct hypfile *hyp, _WORD font_width, _WORD font_height, int x);
hyp_nodenr hyp_find_pagename(void *x, const char *pagename);
int hyp_load_page(struct x76 *x, void *y, hyp_nodenr node, int z, void *a);
int x14db6(struct x76 *x, _WORD *page_num, int *font_idx);
hyp_nodenr x16842(void *x, hyp_nodenr node, int direction);
char *hyp_get_window_title(struct x76 *x, hyp_nodenr nodenr);
void x16768(struct x76 *x);
