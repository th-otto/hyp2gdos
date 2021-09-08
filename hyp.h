typedef short hyp_nodenr;


/*
 * File related structures
 */
typedef struct
{
	long magic;                  /* Magic constant 'HDOC' */
	long itable_size;            /* Length of index table in bytes */
	hyp_nodenr num_index;        /* Number of entries in table */
	unsigned char compiler_vers; /* Hypertext compiler version */
	unsigned char compiler_os;   /* Operating system used for translation */
} HYP_HEADER;
/* sizeof(HYP_HEADER) on disk */
#define SIZEOF_HYP_HEADER 12


/* Valid values for HYP_HEADER->compiler_os */
typedef enum {
	HYP_OS_UNKNOWN = 0,
	HYP_OS_AMIGA = 1,
	HYP_OS_ATARI = 2,
	HYP_OS_MAC = 3, /* until System 9, not MacOSX */
	HYP_OS_WIN32 = 4,
	HYP_OS_UNIX = 5,
	HYP_OS_MACOS = 6, /* MacOSX/macOS */
	HYP_OS_RES2 = 7,
	HYP_OS_RES3 = 8,
	HYP_OS_RES4 = 9
} HYP_OS;


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

typedef struct
{
	unsigned char length;       /* Length */
	/* hyp_indextype */ char type;         /* Type */
	unsigned long seek_offset;  /* File seek offset*/
	unsigned short comp_diff;   /* Difference packed/unpacked in bytes */
	hyp_nodenr next;            /* Index of successor entry */
	hyp_nodenr previous;        /* Index of predecessor entry */
	hyp_nodenr toc_index;       /* Index of directory/parent entry */
	char name[0];               /* First character of 0-terminated entry name */
} INDEX_ENTRY;

/*
 * encoding of the index entry length as "unsigned char"
 * restricts the possible length of node names.
 * Length of entries in a REF file are also
 * encoded as "unsigned char", so the same restriction
 * applies to labels and aliases.
 */
#define HYP_NODENAME_MAX (253U - SIZEOF_INDEX_ENTRY)

/*
 * Largest value that can be encoded as base-255
 */
#define HYP_SHORT_MAX ((unsigned short)65025U)

/*
 * Node numbers are (sometimes) encoded as base-255, in
 * 2 bytes. This gives a theoretical maximum of 65025;
 * leave some place to be on the safe side.
 */
#define HYP_NODE_MAX ((hyp_nodenr)65000U)

/* same for line numbers */
#define HYP_LINENO_MAX ((hyp_lineno)65000U)

/*
 * The length of a link text is encoded
 * in an "unsigned char", with an offset of HYP_STRLEN_OFFSET
 */
#define HYP_LINKTEXT_MAX (255u - HYP_STRLEN_OFFSET)

/*
 * values used for extended headers
 */
typedef enum  {
	HYP_EXTH_EOF = 0,
	HYP_EXTH_DATABASE = 1,
	HYP_EXTH_DEFAULT = 2,
	HYP_EXTH_HOSTNAME = 3,
	HYP_EXTH_OPTIONS = 4,
	HYP_EXTH_AUTHOR = 5,
	HYP_EXTH_VERSION = 6,
	HYP_EXTH_HELP = 7,
	HYP_EXTH_SUBJECT = 8,
	HYP_EXTH_TREEHEADER = 9,
	HYP_EXTH_STGUIDE_FLAGS = 10,
	HYP_EXTH_WIDTH = 11,
	/* new definitions: */
	HYP_EXTH_CHARSET = 30,
	HYP_EXTH_LANGUAGE = 31
} hyp_ext_header;


#define HYP_EOL '\0'

/* ESC values used in (p)nodes */
#define HYP_ESC 				0x1b	/* value used to start esc sequences */
#define HYP_ESC_ESC 			0x1b	/* ESC value quoted */
#define HYP_ESC_WINDOWTITLE 	0x23    /* Window title */
#define HYP_ESC_LINK			0x24	/* link */
#define HYP_ESC_LINK_LINE		0x25	/* link with linenumber */
#define HYP_ESC_ALINK			0x26    /* alink */
#define HYP_ESC_ALINK_LINE		0x27	/* alink with line */
#define HYP_ESC_DATA0			0x28	/* data block */
#define HYP_ESC_DATA1			0x29	/* data block */
#define HYP_ESC_DATA2			0x2a	/* data block */
#define HYP_ESC_DATA3			0x2b	/* data block */
#define HYP_ESC_DATA4			0x2c	/* data block */
#define HYP_ESC_DATA5			0x2d	/* data block */
#define HYP_ESC_DATA6			0x2e	/* data block */
#define HYP_ESC_DATA7			0x2f	/* data block */
#define HYP_ESC_DITHERMASK	    HYP_ESC_DATA7 /* seems to be used for dithermask; undocumented */
#define HYP_ESC_EXTERNAL_REFS	0x30	/* external data block */
#define HYP_ESC_OBJTABLE		0x31	/* table with objects */
#define HYP_ESC_PIC 			0x32	/* picture */
#define HYP_ESC_LINE			0x33    /* line */
#define HYP_ESC_BOX				0x34	/* box */
#define HYP_ESC_RBOX			0x35	/* rounded box */
#define HYP_ESC_TEXTATTR_FIRST	0x64	/* text/font attribute */
#define HYP_ESC_TEXTATTR_LAST	0xa3
#define HYP_ESC_UNKNOWN_A4		0xa4	/* found in hyp2gdos.hyp */
#define HYP_ESC_FG_COLOR		0xa5
#define HYP_ESC_BG_COLOR		0xa6

#define HYP_TXT_NORMAL       0x0000
#define HYP_TXT_BOLD         0x0001
#define HYP_TXT_LIGHT        0x0002
#define HYP_TXT_ITALIC       0x0004
#define HYP_TXT_UNDERLINED   0x0008
#define HYP_TXT_OUTLINED     0x0010
#define HYP_TXT_SHADOWED     0x0020
#define HYP_TXT_MASK         0x003f

/* picture header structure */
typedef struct
{
	unsigned short width;        /* width in pixels */
	unsigned short height;       /* height in pixel */
	unsigned char planes;        /* number of planes (1..8) */
	unsigned char plane_pic;     /* available planes bit vector */
	unsigned char plane_on_off;  /* filled plane bit vector */
	unsigned char filler;        /* fill byte used to align size of structure */
} HYP_PICTURE;
/* sizeof(HYP_PICTURE) on disk */
#define SIZEOF_HYP_PICTURE 8


#define HYP_MAGIC_REF 0x48524546L   /* 'HREF', read as big-endian */
#define HYP_MAGIC_HYP 0x48444f43L   /* 'HDOC', read as big-endian */
#define HYP_MAGIC_TXT 0x54455854L   /* 'TEXT', read as big-endian */

/*
 * values used to calculate the "lines" and "columns" of a picture
 */
#define HYP_PIC_FONTW 8
#define HYP_PIC_FONTH 16


