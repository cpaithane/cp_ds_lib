#include "graph.h"

int graph_test_case1()
{

	int rc = EOK;
	uint32_t graph_max_vertices = 5;
	graph_st *graph;

	graph = graph_allocate_graph(graph_max_vertices);
	CHECK_RC_ASSERT((graph == NULL), 0);

	/*
	 * Graph here is undirectional.
	 *
	 *  0   ----    1
	 *  |	     / 	 \
	 *  |	   /	 2
	 *  |	/	/
	 *  4   ----   3
	 *
	 */
	rc = graph_insert(graph, 0, 1, 5);
	CHECK_RC_ASSERT(rc, EOK);
	rc = graph_insert(graph, 1, 0, 5);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_insert(graph, 0, 4, 6);
	CHECK_RC_ASSERT(rc, EOK);
	rc = graph_insert(graph, 4, 0, 6);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_insert(graph, 1, 4, 1);
	CHECK_RC_ASSERT(rc, EOK);
	rc = graph_insert(graph, 4, 1, 1);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_insert(graph, 1, 2, 4);
	CHECK_RC_ASSERT(rc, EOK);
	rc = graph_insert(graph, 2, 1, 4);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_insert(graph, 2, 3, 3);
	CHECK_RC_ASSERT(rc, EOK);
	rc = graph_insert(graph, 3, 2, 3);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_insert(graph, 4, 3, 2);
	CHECK_RC_ASSERT(rc, EOK);
	rc = graph_insert(graph, 3, 4, 2);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_print_adjacency_matrix(graph);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_do_bfs(graph, 0);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_do_dfs(graph, 0);
	CHECK_RC_ASSERT(rc, EOK);

	rc = graph_deallocate_graph(graph);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

int graph_tc_execute()
{

	int rc = EOK;

	rc = graph_test_case1();
	return rc;

}

