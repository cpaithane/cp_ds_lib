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

typedef enum rb_tree_color
{

	RED = 0,
	BLACK = 1

}rb_tree_color_et;

typedef enum rb_tree_rot_mode
{

	LEFT = 0,
	RIGHT = 1

}rb_tree_rot_mode_et;

#define RB_TREE_NODE_SIZE sizeof(rb_tree_st)
#define RB_TREE_GET_NODE_COLOR(node) \
({			\
	rb_tree_color_et color;	\
	color = BLACK;		\
	if (node)		\
	{			\
		color = node->rb_tree_color;	\
	}			\
	color;			\
})

typedef void (*rb_tree_node_printer_t)(const rb_tree_st *node, uint32_t *nr_nodes);

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

rb_tree_st *rb_tree_insert_internal(rb_tree_st *root,
                                    rb_tree_st **new_node,
                                    void *data,
                                    size_t len,
                                    common_data_compare_t compare,
                                    int *rc);

void rb_tree_inorder_traversal_internal(
                        rb_tree_st *root,
                        uint32_t *nr_nodes,
                        rb_tree_node_printer_t rb_tree_node_printer);
void bst_inorder_traversal(
			rb_tree_st *root,
			rb_tree_node_printer_t rb_tree_node_printer);

void rb_tree_validate_node(rb_tree_st *root);

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

void rb_tree_int_node_printer(const rb_tree_st *node, uint32_t *nr_nodes);

rb_tree_st *rb_tree_get_parent(const rb_tree_st *node);
rb_tree_st *rb_tree_get_grand_parent(const rb_tree_st *node);
rb_tree_st *rb_tree_get_uncle(const rb_tree_st *node, common_data_compare_t compare);
void rb_tree_change_node_color(rb_tree_st *node);

bool rb_tree_check_violation1(rb_tree_st *node, common_data_compare_t compare);
bool rb_tree_check_violation2(rb_tree_st *node, common_data_compare_t compare);
bool rb_tree_check_violation3(rb_tree_st *node, common_data_compare_t compare);

rb_tree_st *rb_tree_handle_violation1(
                rb_tree_st *root,
                common_data_compare_t compare);

rb_tree_st *rb_tree_handle_violation2(
                rb_tree_st *root,
                common_data_compare_t compare);

rb_tree_st *rb_tree_handle_violation3(
                rb_tree_st *root,
                common_data_compare_t compare);

rb_tree_st *rb_tree_check_fix_violation(
                rb_tree_st *node,
                common_data_compare_t compare);

rb_tree_st* rb_tree_left_rotate(rb_tree_st *root, common_data_compare_t compare);
rb_tree_st* rb_tree_right_rotate(rb_tree_st *root, common_data_compare_t compare);
rb_tree_st *rb_tree_handle_spl_right_rotate(rb_tree_st *root);
rb_tree_st *rb_tree_handle_spl_left_rotate(rb_tree_st *root);

rb_tree_rot_mode_et rb_tree_find_rotate_mode_vio2(
                        rb_tree_st *parent,
                        rb_tree_st *grand_parent,
                        rb_tree_st *root,
                        common_data_compare_t compare);

rb_tree_rot_mode_et rb_tree_find_rotate_mode_vio3(
                        rb_tree_st *parent,
                        rb_tree_st *grand_parent,
                        rb_tree_st *root,
                        common_data_compare_t compare);

void rb_tree_find_parent_details(rb_tree_st *parent,
                                 rb_tree_st *grand_parent,
                                 rb_tree_st *root, 
                                 common_data_compare_t compare,
                                 bool *right_parent,
                                 bool *left_child);

bool rb_tree_find_parent_rel(rb_tree_st *root, common_data_compare_t compare);

