#include "rb_tree.h"

extern rb_tree_st *rb_tree_root;

/*
 * This function deletes the node which contains specified value
 */
rb_tree_st *rb_tree_delete_node(rb_tree_st *root,
			void *data, 
			size_t len, 
			common_data_compare_t compare)
{

	int rc = EOK;
	rb_tree_st *temp;

	/*
	 * First traverse to the node of deletion.
	 */
	CHECK_RC_ASSERT((data == NULL), 0);

	if (root == NULL)
	{
		return root;
	}

	rc = compare(root->rb_tree_data, data);
	if (rc == FIRST_GREATER)
	{

		root->rb_tree_left = rb_tree_delete_node(
					root->rb_tree_left,
					data, len, compare);

	}
	else if (rc == FIRST_LESS)
	{

		root->rb_tree_right = rb_tree_delete_node(
					root->rb_tree_right,
					data, len, compare);

	}
	else
	{

		CHECK_RC_ASSERT((root == NULL), 0);

		if ((root->rb_tree_left == NULL) || 
		    (root->rb_tree_right == NULL))
		{

			temp = root->rb_tree_left ? 
				root->rb_tree_left : root->rb_tree_right;

			/*
			 * No child case.
			 */
			if (temp == NULL)
			{

				temp = root;
				root = NULL;

			}
			else
			{

				memcpy(root->rb_tree_data, temp->rb_tree_data, len);
				root->rb_tree_left = NULL;
				root->rb_tree_right = NULL;

			}

			if (rb_tree_root == temp)
			{
				rb_tree_root = NULL;
			}
			rb_tree_dealloc_node(&temp);

		}
		else
		{

			temp = rb_tree_get_min_value(root->rb_tree_right);
			memcpy(root->rb_tree_data, temp->rb_tree_data, len);
			root->rb_tree_right = rb_tree_delete_node(
						root->rb_tree_right,
						root->rb_tree_data, len, compare);

		}

	}

	if (root == NULL)
	{
		return root;
	}

	return root;

}

/*
 * This function handles Case 1 violation of deletion.
 * If node and its child are either red or black, then mark the replaced node as
 * black. This means, no change in black height.
 */
rb_tree_st *rb_tree_fix_delete_case1(rb_tree_st *node)
{

	rb_tree_st *left_child = NULL;
	rb_tree_st *right_child = NULL;
	rb_tree_st *child_node = NULL;
	rb_tree_color_et node_color, child_color;

	CHECK_RC_ASSERT((node == NULL), 0);

	left_child = node->rb_tree_left;
	right_child = node->rb_tree_right;

	if ((left_child == NULL) || (right_child == NULL))
	{
		child_node = (left_child ? left_child : right_child);
	}

	node_color = RB_TREE_GET_NODE_COLOR(node);
	child_color = RB_TREE_GET_NODE_COLOR(child_node);
	if ((node_color == RED) && (child_color == BLACK) ||
	    (node_color == BLACK) && (child_color == RED))
	{
		if (node_color == RED)
		{
			rb_tree_change_node_color(node);
		}

	}

	return node;

}

/*
 * 1. Sibling is black and one of two children are red.
 * 2. Here, two children for sibling should be present.
 * Fix :
 * 1. Rotate the sibling to left if sibling is right child of its parent.
 * 2. Rotate the sibling to right if sibling is left child of its parent.
 * 3. If right child of sibling is red, recolor it to black.
 */
rb_tree_st *rb_tree_fix_delete_case2(
		rb_tree_st *node,
		common_data_compare_t compare)
{

	rb_tree_st *sibling = NULL;
	rb_tree_st *left = NULL;
	rb_tree_st *right = NULL;
	rb_tree_color_et sib_color, lcolor, rcolor;
	bool right_child = FALSE;

	rb_tree_get_sibling_details(node, &sibling, &left, &right, &lcolor, 
		&rcolor, &sib_color, compare);
	if (sibling == NULL)
	{
		return node;
	}

	if ((sib_color == BLACK) && (left) && (right) && 
	    (lcolor == RED || rcolor == RED))
	{

		right_child = rb_tree_find_parent_rel(sibling, compare);
		if (right_child == TRUE)
		{
			rb_tree_left_rotate(sibling, compare);
		}
		else
		{
			rb_tree_right_rotate(sibling, compare);
		}

		if (rcolor == RED)
		{
			rb_tree_change_node_color(right);
		}

	}

	return node;

}

/*
 * 1. Sibling is black and left child of sibling is red. Right-Left case
 * Fix :
 * 1. Right rotate the sibling.
 * 2. Swap the color among the sibling and left child.
 * 3. Left rotate the parent of sibling.
 * Vice versa in case of Left-Right case.
 */
rb_tree_st *rb_tree_fix_delete_case3(
		rb_tree_st *node,
		common_data_compare_t compare)
{

	rb_tree_st *sibling = NULL;
	rb_tree_st *left = NULL;
	rb_tree_st *right = NULL;
	rb_tree_color_et sib_color, lcolor, rcolor;
	bool right_child = FALSE;
	bool subcase1, subcase2;

	rb_tree_get_sibling_details(node, &sibling, &left, &right, &lcolor, 
		&rcolor, &sib_color, compare);
	if (sibling == NULL)
	{
		return node;
	}

	right_child = rb_tree_find_parent_rel(sibling, compare);

	subcase1 = 	((sib_color == BLACK) &&
			(right_child == TRUE) &&
			(left != NULL && right == NULL) &&
			(lcolor == RED));
	subcase2 = 	((sib_color == BLACK) &&
			(right_child == FALSE) &&
			(left == NULL && right != NULL) &&
			(rcolor == RED));

	if (subcase1 == TRUE)
	{

		rb_tree_right_rotate(sibling, compare);
		SWAP(bool, sibling->rb_tree_color, left->rb_tree_color);
		rb_tree_left_rotate(rb_tree_get_parent(sibling), compare);

	}
	else if (subcase2 == TRUE)
	{

		rb_tree_left_rotate(sibling, compare);
		SWAP(bool, sibling->rb_tree_color, right->rb_tree_color);
		rb_tree_right_rotate(rb_tree_get_parent(sibling), compare);

	}

	return node;

}

/*
 * 1. Sibling is black and two children are black.
 * Fix :
 * 1. Change the color of sibling node to red.
 * 2. Recur to parent
 */
rb_tree_st *rb_tree_fix_delete_case4(
		rb_tree_st *node,
		common_data_compare_t compare)
{

	rb_tree_st *sibling = NULL;
	rb_tree_st *left = NULL;
	rb_tree_st *right = NULL;
	rb_tree_color_et sib_color, lcolor, rcolor;
	bool right_child = FALSE;
	bool subcase1, subcase2;

	rb_tree_get_sibling_details(node, &sibling, &left, &right, &lcolor, 
		&rcolor, &sib_color, compare);
	if (sibling == NULL)
	{
		return node;
	}

	right_child = rb_tree_find_parent_rel(sibling, compare);

	if ((sib_color == BLACK) && (lcolor == BLACK) && (rcolor == BLACK))
	{

		rb_tree_change_node_color(sibling);
		/*
		 * As color of sibling is changed to red, its parent could be red.
		 * Think more about this case and implement.
		 */

	}

	return node;

}

/*
 * 1. Sibling is red.
 * Fix :
 * 1. Left rotate parent of sibling if sibling is right child.
 * 2. Right rotate parent of sibling if sibling is left child.
 * 3. Recolor sibiling and parent of sibling.
 * 4. Now, previous left child of sibling could be red. This results into case D.
 */
rb_tree_st *rb_tree_fix_delete_case5(
                rb_tree_st *node,
                common_data_compare_t compare)
{

	rb_tree_st *sibling = NULL;
	rb_tree_st *par_sib = NULL;
	rb_tree_st *left = NULL;
	rb_tree_st *right = NULL;
	rb_tree_color_et sib_color, lcolor, rcolor;
	bool right_child = FALSE;

	par_sib = rb_tree_get_parent(sibling);
	rb_tree_get_sibling_details(node, &sibling, &left, &right, &lcolor, 
		&rcolor, &sib_color, compare);
	right_child = rb_tree_find_parent_rel(sibling, compare);

	if (right_child == TRUE)
	{
		rb_tree_left_rotate(par_sib, compare);
	}
	else
	{
		rb_tree_right_rotate(par_sib, compare);
	}

	rb_tree_change_node_color(par_sib);
	rb_tree_change_node_color(sibling);

	if (lcolor == RED)
	{
		rb_tree_fix_delete_case4(sibling, compare);
	}

	return node;

}

