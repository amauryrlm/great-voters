#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>

#define min(x,y) ({ \
  typeof (x) __x = (x); \
  typeof (y) __y = (y); \
  (__x < __y) ? __x : __y; \
})
#define max(x,y) ({ \
  typeof (x) __x = (x); \
  typeof (y) __y = (y); \
  (__x > __y) ? __x : __y; \
})

struct graph_t {
  bool *mat;
  unsigned int *dist_mat;
  int size;
  int edges_per_vertex;
};

struct graph_t* build_unconnected_graph(int num_vertices);
struct graph_t* build_regular_graph(int num_vertices, int edges_per_vertex);
void randomise_graph(struct graph_t*, float);
unsigned int get_degree(struct graph_t*, int);
void print_graph(FILE*, struct graph_t*);
void delete_graph(struct graph_t*);

#endif

