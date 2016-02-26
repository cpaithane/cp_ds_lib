#include "../include/header.h"

/*
 * This is data structure for doubly linked list
 */
typedef struct dll
{

	void *dll_data;
	struct dll *dll_prev;
	struct dll *dll_next;

}dll_st;

#define DLL_PRINT_NODE(node) \
do {\
	int *data = (int *)node->dll_data;\
	printf("%d-->", *data);\
}while(0)

/*
 * APIs used to access the dll_st.
 */
void dll_initialize(dll_st *node, void *data, size_t len);
dll_st *dll_alloc_node(void *data, size_t len);
void dll_dealloc_node(dll_st *node);

dll_st *dll_insert_node_n(dll_st *head, dll_st *new_node, int position);
dll_st *dll_insert_node_0(dll_st *head, dll_st *new_node);
dll_st *dll_insert_node_pos(dll_st *head, void *data, size_t len, int position);

dll_st *dll_remove_node_0(dll_st *head);
dll_st *dll_remove_node_n(dll_st *head, int position);
dll_st *dll_remove_node_pos(dll_st *head, int position);
int dll_remove_dll(dll_st *head);

int dll_traverse_list(dll_st *head, 
                      dll_st **prev, 
                      dll_st **tail, 
                      dll_st **next, 
                      int position);

int dll_print_list(dll_st *head);
int dll_print_list_rev(dll_st *head);

