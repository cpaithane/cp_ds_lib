#include "rb_tree.h"

extern rb_tree_st *rb_tree_root;

rb_tree_st *rb_tree_insert(rb_tree_st *root,
			   void *data,
			   size_t len,
			   common_data_compare_t compare)
{

	int rc = EOK;
	rb_tree_st *new_node = NULL;

	root = rb_tree_insert_internal(root, &new_node, data, len, compare, &rc);
	rb_tree_check_fix_violation(new_node, compare);
	return root;

}

/*
 * This function inserts data into rb_tree. compare function pointer can be 
 * overriden.
 */
rb_tree_st *rb_tree_insert_internal(rb_tree_st *root,
				    rb_tree_st **new_node,
				    void *data,
				    size_t len,
				    common_data_compare_t compare,
				    int *rc)
{

	*rc = EOK;
	int rc1 = EOK;

	/*
	 * If root node of rbt ree is NULL, allocate and set root node.
	 */
	if (root == NULL)
	{

		root = rb_tree_alloc_node(data, len);
		*new_node = root;

		/*
		 * If global root node of RB tree is NULL, then it is first node 
		 * allocation. Parent of the root will be NULL. So, no need to do
		 * something special for rb_tree_root. As root should be BLACK,
		 * change the root's color to BLACK.
		 */
		if (rb_tree_root == NULL)
		{

			rb_tree_root = root;
			rb_tree_change_node_color(root);

		}

		return root;

	}

	*rc = compare(data, root->rb_tree_data);

	/*
	 * If data to be inserted is greater, then look into right subtree. Else,
	 * look into left subtree.
	 * As per property of BST, no two identical nodes present in BST.
	 * Assign proper parent.
	 */
	if (*rc == FIRST_GREATER)
	{

		root->rb_tree_right = rb_tree_insert_internal(
						root->rb_tree_right,
						new_node, 
						data, len, 
						compare, &rc1);
		CHECK_RC_ASSERT((root->rb_tree_right == NULL), 0);
		root->rb_tree_right->rb_tree_parent = root;

	}
	else if (*rc == FIRST_LESS)
	{

		root->rb_tree_left = rb_tree_insert_internal(
						root->rb_tree_left,
						new_node,
						data, len,
						compare, &rc1);
		CHECK_RC_ASSERT((root->rb_tree_left == NULL), 0);
		root->rb_tree_left->rb_tree_parent = root;

	}
	else
	{

		/*
		 * Control should not reach here. Messy stuff.
		 */ 
		CHECK_RC_ASSERT(*rc == IDENTICAL, 1);
		return NULL;

	}

	return root;

}

/*
 * This function detects 3 types of violations and fixes them.
 */
rb_tree_st *rb_tree_check_fix_violation(
		rb_tree_st *node,
		common_data_compare_t compare)
{

	bool valid = TRUE;

	/*
	 * Check type 1 violation.
	 */
	valid = rb_tree_check_violation1(node, compare);
	if (valid == FALSE)
	{
		return (rb_tree_handle_violation1(node, compare));
	}

	/*
	 * Check type 2 violation.
	 */
	valid = rb_tree_check_violation2(node, compare);
	if (valid == FALSE)
	{
		return (rb_tree_handle_violation2(node, compare));
	}

	/*
	 * Check type 3 violation.
	 */
	valid = rb_tree_check_violation3(node, compare);
	if (valid == FALSE)
	{
		return (rb_tree_handle_violation3(node, compare));
	}

	return node;

}

