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


#define MAXPATH 128
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
	char buf[256];
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
	/* 270 */ short num_nodes;
	/* 278 */ INDEX_ENTRY **nodes;
};



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
