#ifndef ELEC_H
#define ELEC_H

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void init_opinion(struct graph_t*, float p0);
bool get_opinion(struct graph_t* g, int k);
void cellular_consensus(struct graph_t*);


#endif