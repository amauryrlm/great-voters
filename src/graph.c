#include "graph.h"
#include "electeurs.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

/**
 * Sets whether an edge is present between two vertices i & j
 * Assumes the graph is undirected
 */
static void set_edge(struct graph_t *g, int i, int j, bool connected)
{ 
  g->mat[(min(i,j)*g->size)+max(i,j)] = connected;
  g->mat[(max(i,j)*g->size)+min(i,j)] = connected;
  memset(g->dist_mat, 0, g->size * g->size * sizeof(int));
}

/**
 * Returns true if an edge is present between two vertices i & j, else false
 * Assumes the graph is undirected
 */
static bool get_edge(struct graph_t *g, int i, int j)
{
  return g->mat[(min(i,j)*g->size)+max(i,j)];
}


struct graph_t* build_unconnected_graph(int num_vertices)
{
  struct graph_t *g = (struct graph_t*) malloc(sizeof(struct graph_t));
  size_t count = (size_t)num_vertices * (size_t)num_vertices;
  g->mat = (bool*) calloc(count, sizeof(bool));
  assert(g->mat);
  g->dist_mat = (unsigned int*) calloc(count, sizeof(int));
  assert(g->dist_mat);
  g->size = num_vertices;
  return g;
}


struct graph_t* build_regular_graph(int num_vertices, int edges_per_vertex)
{
  struct graph_t* g = build_unconnected_graph(num_vertices);
  int i, j;

  #pragma omp parallel for private(i, j)
  for (i = 0; i < num_vertices; i++) {
    for (j = 1; j <= (edges_per_vertex >> 1); j++) {
      set_edge(g, i, (i + j) % g->size, true);
      set_edge(g, i, (g->size + i - j) % g->size, true);
    }
  }

  g->edges_per_vertex = edges_per_vertex;
  return g;
}


void randomise_graph(struct graph_t *g, float p)
{
  int i, j, k, new;
  srand((unsigned)time(0));
  for (k = 1; k <= (g->edges_per_vertex >> 1); k++)
    for (i = 0; i < g->size; i++)
    {
      j = (i + k) % g->size;
      if (get_edge(g, i, j) && (float)rand()/(float)RAND_MAX < p && get_degree(g, j) != 1 && get_degree(g, i) != g->size-1)
      {
        set_edge(g, i, j, false);
        do {
          new = (float)rand()/(float)RAND_MAX * g->size;
        }
        while (new == j || new == i || get_edge(g, i, new));
          set_edge(g, i, new, true);
      }
    }
}

// Calculates the degree of a node
unsigned int get_degree(struct graph_t *g, int node)
{
  unsigned int d = 0;
  for (int i = 0; i < g->size; i++)
  if(i!=node) d += !!get_edge(g, i, node);
  return d;
}

/**
 * Converts a number into an ASCII label
 * Characters represent the index in reverse, but it doesn't really matter
 */
char* get_graph_label(int n, char *str)
{
  int pos = 0;
  do {
    str[pos++] = 'A' + (n % 26);
    n /= 26;
  } while (n);
  str[pos] = 0;
  return str;
}

/**
 * Prints a graph in dot format.
 */
void print_graph(FILE* f, struct graph_t* g)
{
  // Print the graph header
  fprintf(f, "graph the_graph {\n"
      "ordering=out\n"
      "layout=\"neato\"\n");

  // Circo draws graphs in a circle sometimes but fails other times so 
  // we'll calculate node positions manually.
  double x, y, angle, radius = g->size/M_PI;
  // Print all nodes
  for (int i = 0; i < g->size; i++)
  {
    angle = ((M_PI*2)/(double)g->size)*(double)i;
    x = cos(angle)*radius;
    y = sin(angle)*radius;
    char label[50];
    get_graph_label(i, label);
    char *color_0 = "#E63946";
    char *color_1 = "#1D3557";
    char *color;
    if(get_opinion(g,i) == false) color = color_0;
    else if(get_opinion(g,i) == true) color = color_1;
    fprintf(f, "%s [label=\"%d\","
        "pos=\"%f,%f!\","
        "shape=circle,"
        "width=1.,"
        "fixedsize=true," 
        "style=filled,"
        "fillcolor=\"%s\"\n]\n",
        label, i, x, y, color);
  }
  // Print all arcs
  for (int i = 0; i < g->size; i++)
    for (int j = i; j < g->size; j++)
      if (get_edge(g, i, j) && j > i)
      {
        // Wont have a label longer than this...
        char i_lab[50], j_lab[50];
        get_graph_label(i, i_lab);
        get_graph_label(j, j_lab);
        fprintf(f, "%s -- %s\n", i_lab, j_lab);
      }

  // Finish off the graph
  fprintf(f, "}\n");
}

void delete_graph(struct graph_t* g)
{
  free(g->mat);
  free(g);
}
