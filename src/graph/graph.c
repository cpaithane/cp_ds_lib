#include "graph.h"

/*
 * This function allocates graph and associated data.
 */
graph_st* graph_allocate_graph(uint32_t graph_max_vertices)
{

	int num_bytes;
	graph_st *graph = NULL;

	num_bytes = sizeof(int) * graph_max_vertices * graph_max_vertices;

	graph = (graph_st *)malloc(sizeof(graph_st));
	if (graph == NULL)
	{
		return NULL;
	}
	memset(graph, 0, sizeof(graph_st));
	
	graph->graph_data = (int *)malloc(num_bytes);
	if (graph->graph_data == NULL)
	{

		free(graph);
		return NULL;

	}
	memset(graph->graph_data, 0, num_bytes);
	graph->graph_max_vertices = graph_max_vertices;

	return graph;

}

/*
 * This function frees the memory allocated for graph.
 */
int graph_deallocate_graph(graph_st *graph)
{

	free(graph->graph_data);
	free(graph);
	return EOK;

}

/*
 * This function returns weight stored in graph at (vertex1, vertex2).
 */
int graph_get_weight(graph_st *graph,
		    int vertex1,
		    int vertex2)
{

	int offset = (vertex1 * graph->graph_max_vertices + vertex2);
	return (graph->graph_data[offset]);

}

/*
 * This function runs validation checks before inserting edge into graph.
 */
int graph_validate_input(graph_st *graph,
			 int vertex1,
			 int vertex2,
			 int weight)
{

	int rc = EOK;

	/*
	 * Weight should not be invalid.
	 * graph and graph->graph_data should not be NULL.
	 * Both the vertices should be less than max_vertices.
	 * There should not be any edge present between vertex1 and vertex2.
	 * If any of the above conditions met, it is an invalid insertion request.
	 */
	if ((weight == GRAPH_INVALID_WEIGHT) ||
	    (graph == NULL) || 
	    (graph->graph_data == NULL) ||
	    (vertex1 >= graph->graph_max_vertices) ||
	    (vertex2 >= graph->graph_max_vertices) ||
	    (graph_get_weight(graph, vertex1, vertex2) != GRAPH_INVALID_WEIGHT))
	{
		rc = EINVAL;
	}

	return rc;

}

/*
 * This function inserts vertex1, vertex2 with weight into graph.
 */
int graph_insert(graph_st *graph,
		 int vertex1,
		 int vertex2,
		 int weight)
{

	int rc = EOK;
	int offset = 0;

	rc = graph_validate_input(graph,
				  vertex1,
				  vertex2,
				  weight);

	if (rc != EOK)
	{
		return rc;
	}

	offset = ((graph->graph_max_vertices * vertex1) + vertex2);
	graph->graph_data[offset] = weight;
	return rc;

}

/*
 * This function prints adjacency matrix.
 */
int graph_print_adjacency_matrix(graph_st *graph)
{

	int rc = EOK;
	uint32_t graph_max_vertices;
	int i, j, weight;

	if ((graph == NULL) || (graph->graph_data == NULL))
	{

		rc = EINVAL;
		return rc;

	}

	graph_max_vertices = graph->graph_max_vertices;
	printf("Graph : Its vertices and weights = \n");

	for (i = 0; i < graph_max_vertices; i++)
	{

		for (j = 0; j < graph_max_vertices; j++)
		{

			weight = graph_get_weight(graph, i, j);
			if (weight == GRAPH_INVALID_WEIGHT)
			{
				continue;
			}

			printf("V1 = %d V2 = %d Weight = %d\n", i, j, weight);

		}

	}

	return rc;

}

/*
 * This function uses queue to do breadth-first-search. Algorithm is as follow:
 * 1. Eneque start_vertex to queue.
 * 2. Till, the queue is not empty: dequeue the element, mark element as visited.
 * 3. Find out adjacent vertices of the just dequeued vertices and eneque them.
 * 4. Go to #2.
 */
int graph_do_bfs(graph_st *graph, int start_vertex)
{

	bool *visited;
	queue_st *q;
	int tmp_vertex, i, *data;
	int num_bytes = sizeof(bool) * graph->graph_max_vertices;
	int rc = EOK;

	/*
	 * visited array indicates whether the vertex is visited while BFS.
	 */
	visited = (bool*)malloc(num_bytes);
	if (visited == NULL)
	{

		rc = ENOMEM;
		return rc;

	}
	memset(visited, 0, num_bytes);

	q = queue_alloc_queue(sizeof(int));
	if (q == NULL)
	{

		free(visited);
		rc = ENOMEM;
		return rc;

	}

	/*
	 * Eneque a start_vertex into queue.
	 */
	rc = queue_eneque(q, &start_vertex, sizeof(int));
	CHECK_RC_ASSERT(rc, EOK);
	printf("\nBFS for the graph starting from %d\n", start_vertex);

	while (queue_is_queue_empty(q) == ENOTEMPTY)
	{

		data = (int *)queue_deque(q, &rc);
		if (rc != EOK)
		{
			break;
		}

		memcpy(&tmp_vertex, data, sizeof(int));
		if (visited[tmp_vertex] != VISITED)
		{
			printf("%d  ", tmp_vertex);
		}
		visited[tmp_vertex] = VISITED;

		/*
		 * Find out all the adjacent vertices and enque them into queue.
		 */
		for (i = 0; i < graph->graph_max_vertices; i++)
		{

			if ((graph_get_weight(graph, tmp_vertex, i) != 
			     GRAPH_INVALID_WEIGHT) && (visited[i] != VISITED))
			{

				rc = queue_eneque(q, &i, sizeof(int));
				CHECK_RC_ASSERT(rc, EOK);

			}

		}

	}
	printf("\n");

	rc = queue_dealloc_queue(q);
	CHECK_RC_ASSERT(rc, EOK);
	free(visited);
	return rc;

}

/*
 * This function uses stack to go deep down in the graph. Algorithm is as follow:
 *
 */
int graph_do_dfs(graph_st *graph, int start_vertex)
{

	bool *visited;
	stack_st *st;
	int tmp_vertex, i, *data;
	int num_bytes = sizeof(bool) * graph->graph_max_vertices;
	int rc = EOK;

	/*
	 * visited array indicates whether the vertex is visited while BFS.
	 */
	visited = (bool*)malloc(num_bytes);
	if (visited == NULL)
	{

		rc = ENOMEM;
		return rc;

	}
	memset(visited, 0, num_bytes);

	st = stack_alloc_stack(sizeof(int));
	if (st == NULL)
	{

		free(visited);
		rc = ENOMEM;
		return rc;

	}

	/*
	 * First push start_index on stack.
	 */
	rc = stack_push(st, &start_vertex, sizeof(int));
	CHECK_RC_ASSERT(rc, EOK);
	printf("\nDFS for the graph starting from %d\n", start_vertex);

	while (stack_is_stack_empty(st) == 0)
	{

		data = stack_pop(st, &rc);
		if (rc != EOK)
		{
			break;
		}

		memcpy(&tmp_vertex, data, sizeof(int));
		if (visited[tmp_vertex] != VISITED)
		{
			printf("%d  ", tmp_vertex);
		}

		visited[tmp_vertex] = VISITED;

		for (i = 0; i < graph->graph_max_vertices; i++)
		{

			if ((graph_get_weight(graph, tmp_vertex, i) != 
			     GRAPH_INVALID_WEIGHT) && (visited[i] != VISITED))
			{

				rc = stack_push(st, &i, sizeof(int));
				CHECK_RC_ASSERT(rc, EOK);

			}
		}
	
	}
	printf("\n");

	free(visited);
	rc = stack_dealloc_stack(st);
	return rc;

}

