#include "../include/header.h"
#include "../queue/queue.h"
#include "../stack/stack.h"

#define GRAPH_INVALID_WEIGHT 0
#define VISITED 1

/*
 * Data structure for graph. [row_index, column_index] represents vertices of the
 * one of edge of graph. Values in the array represent weight of the edge.
 */
typedef struct graph
{

	int *graph_data;
	uint32_t graph_max_vertices;

}graph_st;

/*
 * APIs used to access graph
 */
graph_st* graph_allocate_graph(uint32_t graph_max_vertices);
int graph_deallocate_graph(graph_st *graph);
int graph_insert(graph_st *graph,
                 int vertex1,
                 int vertex2,
                 int weight);
int graph_get_weight(graph_st *graph,
                    int vertex1,
                    int vertex2);
int graph_validate_input(graph_st *graph,
                         int vertex1,
                         int vertex2,
                         int weight);

int graph_print_adjacency_matrix(graph_st *graph);

int graph_do_bfs(graph_st *graph, int start_vertex);
int graph_do_dfs(graph_st *graph, int start_vertex);

