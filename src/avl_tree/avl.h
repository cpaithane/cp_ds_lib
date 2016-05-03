#include "../include/header.h"

/*
 * Data structure for BST
 */
typedef struct avl
{

	unsigned int bst_height;
	void *bst_data;
	struct avl *bst_left;
	struct avl *bst_right;

}avl_st;

#define AVL_MIN_HEIGHT 1

typedef int (*bst_data_compare_t)(const void *, const void *);
typedef void (*avl_node_printer_t)(const avl_st *node);

/*
 * APIs to use bst
 */
void avl_initialize_node(avl_st *node, void *data, size_t len);
avl_st *avl_alloc_node(void *data, size_t len);
void avl_dealloc_node(avl_st **nodep);

void avl_int_node_printer(const avl_st *node);
avl_st *avl_insert(avl_st *root, void *data, size_t len, bst_data_compare_t bst_data_compare);

void avl_inorder_traversal(avl_st *root, avl_node_printer_t avl_node_printer);

int avl_traverse_bst(avl_st *root,
                     avl_st **parent,
                     avl_st **node,
                     void *data,
                     bst_data_compare_t compare);

avl_st *avl_delete_node_1(avl_st *root, avl_st *parent, avl_st *node_to_delete);

avl_st *avl_delete_node_2(avl_st *root, avl_st *parent, avl_st *node_to_delete);

avl_st *avl_delete_node_3(avl_st *root,
			  avl_st *parent,
			  avl_st *node_to_delete,
			  size_t len);

avl_st *avl_delete_node(avl_st *root, 
                        void *data, 
                        size_t len, 
                        bst_data_compare_t compare);

void avl_destroy_tree(avl_st *root);
avl_st* bst_left_rotate(avl_st *root);
avl_st* bst_right_rotate(avl_st *root);
unsigned int bst_get_node_height(avl_st *node);
int bst_get_balance_factor(avl_st *node);
void bst_update_node_height(avl_st *root);
avl_st *bst_do_balance(avl_st *root, 
                       void *data, 
                       bst_data_compare_t compare);
int avl_check_sanity(avl_st *root);
avl_st *bst_handle_spl_left_rotate(avl_st *root);
avl_st *bst_handle_spl_right_rotate(avl_st *root);

