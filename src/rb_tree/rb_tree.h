#include "../include/header.h"

/*
 * Data structure for RB-Tree
 */
typedef struct rb_tree
{

	void *rb_tree_data;
	bool rb_tree_color;
	struct rb_tree *rb_tree_parent;
	struct rb_tree *rb_tree_left;
	struct rb_tree *rb_tree_right;

}rb_tree_st;

#define RB_TREE_NODE_SIZE sizeof(rb_tree_st)

typedef void (*rb_tree_node_printer_t)(const rb_tree_st *node);

/*
 * APIs to use rb_tree
 */
void rb_tree_initialize_node(rb_tree_st *node, void *data, size_t len);
rb_tree_st *rb_tree_alloc_node(void *data, size_t len);
void rb_tree_dealloc_node(rb_tree_st **nodep);

rb_tree_st *rb_tree_insert(rb_tree_st *root,
                           void *data,
                           size_t len,
                           common_data_compare_t compare);

void bst_inorder_traversal(
			rb_tree_st *root,
			rb_tree_node_printer_t rb_tree_node_printer);

int rb_tree_traverse_bst(rb_tree_st *root,
                     rb_tree_st **parent,
                     rb_tree_st **node,
                     void *data,
                     common_data_compare_t compare);

rb_tree_st *rb_tree_delete_node_1(rb_tree_st *root,
                                  rb_tree_st *parent,
                                  rb_tree_st *node_to_delete);

rb_tree_st *rb_tree_delete_node_2(rb_tree_st *root,
                                  rb_tree_st *parent,
                                  rb_tree_st *node_to_delete);

rb_tree_st *rb_tree_delete_node_3(rb_tree_st *root, 
                                  rb_tree_st *parent,
                                  rb_tree_st *node_to_delete,
                                  size_t len);

rb_tree_st *rb_tree_delete_node(rb_tree_st *root,
                                void *data,
                                size_t len,
                                common_data_compare_t compare);

void rb_tree_destroy_tree(rb_tree_st *root);

void rb_tree_int_node_printer(const rb_tree_st *node);

