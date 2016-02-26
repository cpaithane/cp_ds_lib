#include "../include/header.h"

/*
 * Data structure for singly circular linked list
 */
typedef struct scll
{

	void *scll_data;
	struct scll *scll_next;

}scll_st;

#define SCLL_PRINT_NODE(node)	\
do {\
	int *data;\
	data = (int *)node->scll_data;\
	printf("%d-->", *data);\
} while(0)

/*
 * APIs to access singly circular linked list
 */
scll_st *scll_alloc_scll_node(size_t len, void *data);
void scll_dealloc_node(scll_st *node);

int scll_print_list(scll_st *head);
void scll_initialize(scll_st *node, void *data, size_t len);
int scll_remove_scll(scll_st *head);
int scll_traverse_list(scll_st *head, scll_st **prev, scll_st **tmp, scll_st **next, int position);

scll_st *scll_insert_node_0(scll_st *head, scll_st *new_node);
scll_st *scll_insert_node_n(scll_st *head, scll_st *new_node, int position);
scll_st *scll_insert_node_pos(scll_st *head, void *data, int position, size_t len);

scll_st *scll_remove_node_0(scll_st *head);
scll_st *scll_remove_node_n(scll_st *head, int position);
scll_st *scll_remove_node_pos(scll_st *head, int position);

