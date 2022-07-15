#include "graph.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

const int DECISION = 4;
int *tab_opinions;

static bool get_edge(struct graph_t * g, int i, int j){ //return true if link exists between i and j
    return g->mat[i*g->size+j];
}

void init_opinion(struct graph_t* g, float p0) //initialize all nodes opinions to 1 with p0 probability
{
    tab_opinions = (int*)calloc((size_t)g->size, sizeof(int)); //cumulative sum of opinion of each node
    srand((unsigned)time(0));
    p0 = p0*10;
    for(int i=0; i < g->size; i++){
        int x = rand()%11;
        if(x > p0) {
            g->mat[i*g->size+i] = 0;
            tab_opinions[i] = -1;
        }
        else if(x <= p0) {
            g->mat[i*g->size+i] = 1;
            tab_opinions[i] = 1;
        }
    }
}

bool get_opinion(struct graph_t* g, int k){
    return g->mat[k*g->size+k];
}

void cellular_consensus(struct graph_t* g){
    
    for(int k=0; k < g->size; k++){ //iterate by the number of node
        int opinion_0 = 0; //number of neighbour's node with opinion 0
        int opinion_1 = 0; //number of neighbour's node with opinion 0
        int degree = get_degree(g,k) / 2; //number of nodes / 2

        if(tab_opinions[k] > DECISION || tab_opinions[k] < -DECISION) 
            g->mat[k*g->size+k] = g->mat[k*g->size+k]; //case in which node is already decided

        for(int i=0; i < g->size; i++){ //travels through its neighbours
            if(get_edge(g,i,k) && g->mat[i*g->size+i] == 0){ //if neighbour's opinion is 0
                opinion_0 ++ ;
            }
            else if(get_edge(g,i,k) && g->mat[i*g->size+i] == 1){ ////if neighbour's opinion is 0
                opinion_1 ++ ;
            }
        }

        if(opinion_0 > degree){
            g->mat[k*g->size+k] = 0;
            tab_opinions[k] = tab_opinions[k] - 1;
        }
        else if(opinion_1 > degree){
            g->mat[k*g->size+k] = 1;
            tab_opinions[k] = tab_opinions[k] + 1;
        }
        else g->mat[k*g->size+k] = g->mat[k*g->size+k]; // case in which same number of opinion 0 and 1
    } 
}

void delete_tab_opinions(){
    free(tab_opinions);
}
