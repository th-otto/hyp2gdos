#include "hyp2gdos.h"

char hypfold[MAXPATH];
_WORD x1d9c4;

/**************************************************************************/
/* ---------------------------------------------------------------------- */
/**************************************************************************/

int get_nodetype(struct hypfile *hyp, hyp_nodenr node)
{
	if (hyp != NULL &&
		hyp->nodes != NULL &&
		node >= 0 &&
		node < hyp->num_nodes)
		return hyp->nodes[node]->type;
	return -1;
}

/* ---------------------------------------------------------------------- */

struct hypfile *hyp_new(void)
{
	return 0;
}

/* ---------------------------------------------------------------------- */

int hyp_load(struct hypfile *hyp, const char *filename)
{
	(void) hyp;
	(void) filename;
	return 0;
}

/* ---------------------------------------------------------------------- */

void hyp_delete(struct hypfile *hyp)
{
	(void) hyp;
}

/* ---------------------------------------------------------------------- */

void hyp_free(struct hypfile **hyp)
{
	struct hypfile *h;
	
	h = *hyp;
	if (h != NULL)
	{
		hyp_delete(h);
		free(h);
		*hyp = NULL;
	}
}


int x16734(struct x76 *p, struct hypfile *hyp, _WORD font_width, _WORD font_height, int x)
{
	(void)p;
	(void) hyp;
	(void) font_width;
	(void) font_height;
	(void) x;
	return 0;
}


int x16fd8(void)
{
	return 0;
}


int x17008(int fontidx, _WORD font_id, _WORD size)
{
	(void) fontidx;
	(void) font_id;
	(void) size;
	return 0;
}


_BOOL can_scale_bitmaps(_WORD handle)
{
	_WORD workout[57];
	
	vq_extnd(handle, 1, workout);
	return (workout[30] & 1) != 0;
}

hyp_nodenr hyp_find_pagename(void *x, const char *pagename)
{
	(void) x;
	(void) pagename;
	return -1;
}


int hyp_load_page(struct x76 *x, void *y, hyp_nodenr node, int z, void *a)
{
	(void)x;
	(void)node;
	(void)y;
	(void)z;
	(void)a;
	return 0;
}

int x14db6(struct x76 *x, _WORD *page_num, int *font_idx)
{
	(void)x;
	(void)page_num;
	(void)font_idx;
	return 0;
}


hyp_nodenr x16842(void *x, hyp_nodenr node, int direction)
{
	(void)x;
	(void)node;
	(void)direction;
	return node;
}


char *hyp_get_window_title(struct x76 *x, hyp_nodenr nodenr)
{
	(void)x;
	(void)nodenr;
	return 0;
}


void x16768(struct x76 *x)
{
	(void)x;
}
