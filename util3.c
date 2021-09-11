#include "hyp2gdos.h"

void *xmalloc(size_t size)
{
	return malloc(size);
}


void xfree(void *p)
{
	free(p);
}


