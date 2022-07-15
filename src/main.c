#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"
#include "electeurs.h"

static void print_usage(FILE *f)
{
  fprintf(f, "Usage: ./strogatz num_vertices edges_per_vertex prob_of_swap\n\n");
  fprintf(f, "  num_vertices - an int\n");
  fprintf(f, "  edges_per_vertex - an int\n");
  fprintf(f, "  prob_of_swap - a float between 0 and 1\n");
}

int main(int argc, char** argv)
{
  int num_vertices, edges_per_vertex, iterations;
  float prob_of_swap, p0;
  struct graph_t *g;
  FILE *fptr1, *fptr2, *fptr3;
  fptr1 = fopen("./graphs/graph1.dot","w");
  fptr2 = fopen("./graphs/graph2.dot","w");
  fptr3 = fopen("./graphs/graph3.dot","w");

  if (argc < 4)
    goto err_usage;
  if (sscanf(argv[1], "%i", &num_vertices) != 1)
    goto err_usage;
  if (sscanf(argv[2], "%i", &edges_per_vertex) != 1)
    goto err_usage;
  if (sscanf(argv[3], "%f", &prob_of_swap) != 1)
    goto err_usage;
  if (sscanf(argv[4], "%f", &p0) != 1)
    goto err_usage;

  if (edges_per_vertex % 2) {
    fprintf(stdout, "edges_per_vertex should be even\n");
    return EXIT_FAILURE;
  }

  g = build_regular_graph(num_vertices, edges_per_vertex);
  randomise_graph(g, prob_of_swap);

  init_opinion(g,p0); 

  print_graph(fptr1,g);

  int i = 0;
  float sum = 0.5;

  while(sum!= 0 && sum!=1){
    sum = 0;
    i++;
    cellular_consensus(g);
    for(int j = 0; j < g->size; j++) sum = sum + g->mat[j*g->size + j];
    sum = sum / g->size;
    printf("i = %d, sum = %f\n",i,sum);
  }

  print_graph(fptr2,g);
  
  delete_tab_opinions();
  delete_graph(g);
  return 0;

err_usage:
  print_usage(stdout);
  return EXIT_FAILURE;
}
