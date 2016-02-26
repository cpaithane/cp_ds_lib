#include "../include/header.h"

/*
 * Data structure for singly linked list
 */
typedef struct sll {

	void *sll_data;
	struct sll *sll_next;

}sll_st;

/*
 * API for singly linked list implementation
 */
sll_st *sll_alloc_node(size_t len, void *data);
void sll_dealloc_node(sll_st *node);

void sll_initialize(sll_st *head);
int sll_remove_sll(sll_st *head);

sll_st *sll_insert_node_0(sll_st *head, sll_st *new_node);
sll_st *sll_insert_node_n(sll_st *head, sll_st *new_node, int position);
sll_st *sll_insert_node_pos(sll_st *head, void *data, int position, size_t len);

sll_st *sll_remove_node_0(sll_st *head);
sll_st *sll_remove_node_n(sll_st *head, int position);
sll_st *sll_remove_node_pos(sll_st *head, int position);

int sll_print_sll(sll_st *head);

