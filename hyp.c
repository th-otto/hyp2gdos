#include "hyp2gdos.h"

int get_nodetype(struct hypfile *hyp, hyp_nodenr node)
{
	if (hyp != NULL &&
		hyp->nodes != NULL &&
		node >= 0 &&
		node < hyp->num_nodes)
		return hyp->nodes[node]->type;
	return -1;
}
