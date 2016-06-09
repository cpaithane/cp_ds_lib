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

#ifdef SUPPORT

int bst_traverse_bst(bst_st *root,
                     bst_st **parent,
                     bst_st **node,
                     void *data,
                     bst_data_compare_t compare);

bst_st *bst_delete_node_1(bst_st *root, bst_st *parent, bst_st *node_to_delete);

bst_st *bst_delete_node_2(bst_st *root, bst_st *parent, bst_st *node_to_delete);

bst_st *bst_delete_node_3(bst_st *root, bst_st *parent, bst_st *node_to_delete, size_t len);

bst_st *bst_delete_node(bst_st *root, 
                        void *data, 
                        size_t len, 
                        bst_data_compare_t compare);

#endif

void rb_tree_destroy_tree(rb_tree_st *root);


