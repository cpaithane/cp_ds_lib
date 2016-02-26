#include "../include/header.h"

/*
 * Data structure for BST
 */
typedef struct bst
{

	void *bst_data;
	struct bst *bst_left;
	struct bst *bst_right;

}bst_st;

typedef int (*bst_data_compare_t)(const void *, const void *);
typedef void (*bst_node_printer_t)(const bst_st *node);

/*
 * APIs to use bst
 */
void bst_initialize_node(bst_st *node, void *data, size_t len);
bst_st *bst_alloc_node(void *data, size_t len);
void bst_dealloc_node(bst_st **nodep);

void bst_int_node_printer(const bst_st *node);
int bst_int_data_compare(const void *data1, const void *data2);
bst_st *bst_insert(bst_st *root, void *data, size_t len, bst_data_compare_t bst_data_compare);

void bst_inorder_traversal(bst_st *root, bst_node_printer_t bst_node_printer);

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

void bst_destroy_tree(bst_st *root);

