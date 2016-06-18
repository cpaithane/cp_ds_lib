#include "rb_tree.h"

/*
 * This is root node of RB tree initialized to NULL.
 */
rb_tree_st *rb_tree_root = NULL;

/*
 * This function initializes node, pointers etc.
 */
void rb_tree_initialize_node(rb_tree_st *node, void *data, size_t len)
{

	memcpy(node->rb_tree_data, data, len);
	node->rb_tree_left = NULL;
	node->rb_tree_right = NULL;
	node->rb_tree_parent = NULL;
	node->rb_tree_color = RED;

}

/*
 * This function allocates rb_tree node.
 */
rb_tree_st *rb_tree_alloc_node(void *data, size_t len)
{

	rb_tree_st *node = NULL;
	node = (rb_tree_st *)malloc(RB_TREE_NODE_SIZE);
	memset(node, 0, RB_TREE_NODE_SIZE);

	node->rb_tree_data = malloc(len);
	memset(node->rb_tree_data, 0, len);
	rb_tree_initialize_node(node, data, len);

	return node;

}

/*
 * This function deallocates node and data inside it.
 */
void rb_tree_dealloc_node(rb_tree_st **nodep)
{

	rb_tree_st *node = *nodep;
	free(node->rb_tree_data);
	free(node);
	*nodep = NULL;

}

/*
 * This function does inorder traversal of RBTree and validates the node.
 */
void rb_tree_inorder_traversal(
			rb_tree_st *root,
			rb_tree_node_printer_t rb_tree_node_printer)
{

	/*
	 * If root is NULL, return.
	 */
	if (root == NULL)
	{
		return;
	}

	rb_tree_validate_node(root);

	/*
	 * traverse to the left subtree
	 */
	rb_tree_inorder_traversal(root->rb_tree_left, rb_tree_node_printer);

	/*
	 * print the node.
	 */
	rb_tree_node_printer(root);

	/*
	 * traverse to the right subtree
	 */
	rb_tree_inorder_traversal(root->rb_tree_right, rb_tree_node_printer);

}

/*
 * Adding more checks to verify the node satisfies the RB tree propperty.
 */
void rb_tree_validate_node(rb_tree_st *root)
{

	rb_tree_color_et par_color, color;
	rb_tree_st *par_node;

	color = RB_TREE_GET_NODE_COLOR(root);

	/*
	 * root of the tree should be black.
	 */
	if (root == rb_tree_root)
	{
		CHECK_RC_ASSERT(color, BLACK);
	}

	/*
	 * If node is red, then its parent should be black.
	 */
	if (color == RED)
	{

		par_node = rb_tree_get_parent(root);
		par_color = RB_TREE_GET_NODE_COLOR(par_node);
		CHECK_RC_ASSERT(par_color, BLACK);

	}

}

/*
 * This function traverses RBTree till it reaches data uses compare function.
 */
int rb_tree_traverse_bst(rb_tree_st *root, 
		     rb_tree_st **parent, 
		     rb_tree_st **node, 
		     void *data, 
		     common_data_compare_t compare)
{

	int rc = EOK;

	CHECK_RC_ASSERT((data == NULL), 0);

	if (root == NULL)
	{
		return ENOENT;
	}

	rc = compare(root->rb_tree_data, data);
	if (rc == FIRST_GREATER)
	{

		*parent = root;
		return (rb_tree_traverse_bst(root->rb_tree_left,
				 parent, 
				 node, 
				 data, compare));

	}
	else if (rc == FIRST_LESS)
	{

		*parent = root;
		return (rb_tree_traverse_bst(root->rb_tree_right,
				 parent, 
				 node, 
				 data, compare));

	}
	else
	{

		*node = root;
		return EOK;

	}

}

/*
 * This function deletes leaf nodes of BST
 */
rb_tree_st *rb_tree_delete_node_1(rb_tree_st *root,
				  rb_tree_st *parent,
				  rb_tree_st *node_to_delete)
{

	/*
	 * If it is root node, return root as NULL.
	 */
	if (node_to_delete == rb_tree_root)
	{

		rb_tree_root = NULL;
		root = NULL;

	}
	/*
	 * Else, link from the parent needs to be broken.
	 */
	else
	{

		parent->rb_tree_left = NULL;
		parent->rb_tree_right = NULL;

	}

	rb_tree_dealloc_node(&node_to_delete);
	return root;

}

/*
 * This function deletes a node which has either left child or right child.
 */
rb_tree_st *rb_tree_delete_node_2(rb_tree_st *root,
				  rb_tree_st *parent,
				  rb_tree_st *node_to_delete)
{

	int is_left = (node_to_delete->rb_tree_left != NULL ? 1 : 0);

	/*
	 * If deletion is for root. That means, parent is NULL.
	 */
	if (node_to_delete == rb_tree_root)
	{

		if (is_left)
		{
			rb_tree_root = node_to_delete->rb_tree_left;
		}
		else
		{
			rb_tree_root = node_to_delete->rb_tree_right;
		}

		rb_tree_dealloc_node(&node_to_delete);
		return rb_tree_root;

	}

	/*
	 * There are two subcases in this case : 
	 * If left subtree present, assign this left subtree to the parent of the node 
	 * to be deleted.
	 */

	if (is_left)
	{
		parent->rb_tree_left = node_to_delete->rb_tree_left;
	}
	else
	{
		parent->rb_tree_right = node_to_delete->rb_tree_right;
	}

	rb_tree_dealloc_node(&node_to_delete);
	return root;

}

/*
 * This function deletes a node which has both the children.
 */
rb_tree_st *rb_tree_delete_node_3(rb_tree_st *root,
				  rb_tree_st *parent,
				  rb_tree_st *node_to_delete,
				  size_t len)
{

	rb_tree_st *in_suc = NULL;
	rb_tree_st *father_of_in_suc = NULL;
	rb_tree_st *father_of_right_subtree = node_to_delete;
	rb_tree_st *right_subtree = node_to_delete->rb_tree_right;

	/*
	 * Find out first inorder successor of the node to be deleted.
	 */
	while (right_subtree->rb_tree_left != NULL)
	{

		father_of_right_subtree = right_subtree;
		right_subtree = right_subtree->rb_tree_left;

	}

	in_suc = right_subtree;
	father_of_in_suc = father_of_right_subtree;
	CHECK_RC_ASSERT((in_suc == NULL), 0);
	CHECK_RC_ASSERT((father_of_in_suc, NULL), 0);

	/*
	 * Now, swap the contents of inorder successor and 
	 * node to be deleted.
	 */
	memcpy(node_to_delete->rb_tree_data, in_suc->rb_tree_data, len);

	/*
	 * Inorder successor can have right subtree.
	 * We need adjust pointers of right subtree.
	 */
	if (father_of_in_suc != node_to_delete)
	{
		node_to_delete->rb_tree_left = in_suc->rb_tree_right;
	}
	else
	{
		node_to_delete->rb_tree_right = in_suc->rb_tree_right;
	}

	/*
	 * Delete the node pointed by inorder predecessor.
	 */
	rb_tree_dealloc_node(&in_suc);

	return root;

}

/*
 * This function deletes the node which contains specified value
 */
rb_tree_st *rb_tree_delete_node(rb_tree_st *root,
				void *data,
				size_t len,
				common_data_compare_t compare)
{

	rb_tree_st *node_to_delete = NULL;
	rb_tree_st *parent = NULL;

	/*
	 * First traverse to the node of deletion.
	 */
	rb_tree_traverse_bst(root, &parent, &node_to_delete, data, compare);

	CHECK_RC_ASSERT((node_to_delete == NULL), 0);
	if (root != rb_tree_root)
	{
		CHECK_RC_ASSERT((parent == NULL), 0);
	}

	/*
	 * There are three cases of deletion
	 * 1. Deletion of leaf node
	 * 2. Deletion of node which has single child
	 * 3. Deletion of node which has two child
	 */

	/*
	 * Case 1 of deletion
	 */
	if ((node_to_delete->rb_tree_left == NULL) && 
	    (node_to_delete->rb_tree_right == NULL))
	{

		root = rb_tree_delete_node_1(root, parent, node_to_delete);
		return root;

	}

	/*
	 * Case 2 of deletion
	 */
	if ((node_to_delete->rb_tree_left == NULL) || 
	    (node_to_delete->rb_tree_right == NULL))
	{

		root = rb_tree_delete_node_2(root, parent, node_to_delete);
		return root;

	}

	/*
	 * Case 3 of deletion
	 */
	if ((node_to_delete->rb_tree_left) && 
	    (node_to_delete->rb_tree_right))
	{

		root = rb_tree_delete_node_3(root, parent, node_to_delete, len);
		return root;

	}

	/*
	 * Should not reach here by any means. Mess !
	 */
	CHECK_RC_ASSERT(1, 0);

}

/*
 * This function returns pointer to parent node.
 * rb_tree_root doesn't have parent_node. So, this will return NULL.
 * For every other node, it should will return non-NULL value.
 */
rb_tree_st *rb_tree_get_parent(const rb_tree_st *node)
{

	rb_tree_st *parent_node = NULL;

	if (node != NULL)
	{
		parent_node = node->rb_tree_parent;
	}

	return parent_node;

}

/*
 * This function returns grandparent of the RBTree node.
 */
rb_tree_st *rb_tree_get_grand_parent(const rb_tree_st *node)
{

	rb_tree_st *parent_node = NULL;
	rb_tree_st *grand_parent_node = NULL;
	parent_node = rb_tree_get_parent(node);
	if (parent_node != NULL)
	{
		grand_parent_node = rb_tree_get_parent(parent_node);
	}

	return grand_parent_node;

}

/*
 * This function returns uncle node of the RBTree node.
 */
rb_tree_st *rb_tree_get_uncle(const rb_tree_st *node, common_data_compare_t compare)
{

	int rc = EOK;
	rb_tree_st *grand_parent_node = NULL;
	rb_tree_st *uncle_node = NULL;

	grand_parent_node = rb_tree_get_grand_parent(node);
	if (grand_parent_node != NULL)
	{

		rc = compare(node->rb_tree_data, grand_parent_node->rb_tree_data);
		CHECK_RC_ASSERT((rc != IDENTICAL), 1);
		if (rc == FIRST_LESS)
		{
			uncle_node = grand_parent_node->rb_tree_right;
		}
		else
		{
			uncle_node = grand_parent_node->rb_tree_left;
		}

	}

	return uncle_node;

}

/*
 * This function prints integer data inside node
 */
void rb_tree_int_node_printer(const rb_tree_st *node)
{

	int *data;
	rb_tree_st *parent = NULL;

	printf("\n");
	if (node)
	{

		data = (int *)node->rb_tree_data;
		printf("node %d\t", *data);
		printf ("color %d\t", (int)node->rb_tree_color);
		parent = rb_tree_get_parent(node);
		if (parent)
		{
			printf("parent %d", *(int*)(parent->rb_tree_data));
		}

	}
	printf("\n");

}

/*
 * This function destroys tree rooted at root
 */
void rb_tree_destroy_tree(rb_tree_st *root)
{

	if (root == NULL)
	{
		return;
	}

	rb_tree_destroy_tree(root->rb_tree_left);
	rb_tree_destroy_tree(root->rb_tree_right);
	rb_tree_dealloc_node(&root);

}

/*
 * This function flips the color of the node.
 */
void rb_tree_change_node_color(rb_tree_st *node)
{

	if (node == NULL)
	{
		return;
	}

	if (node->rb_tree_color == RED)
	{
		node->rb_tree_color = BLACK;
	}
	else
	{
		node->rb_tree_color = RED;
	}

}

/*
 * This function checks whether type #1 violation present in the subsystem.
 * Case 1 of violation : Red node whose parent and uncle is also red.
 */
bool rb_tree_check_violation1(rb_tree_st *node, common_data_compare_t compare)
{

	bool valid = TRUE;

	rb_tree_st *parent = NULL;
	rb_tree_st *grand_parent = NULL;
	rb_tree_st *uncle = NULL;

	CHECK_RC_ASSERT((node == NULL), 0);

	uncle = rb_tree_get_uncle(node, compare);
	parent = rb_tree_get_parent(node);
	grand_parent = rb_tree_get_grand_parent(node);

	if ((RB_TREE_GET_NODE_COLOR(node) == RED) &&
	    (RB_TREE_GET_NODE_COLOR(parent) == RED) &&
	    (RB_TREE_GET_NODE_COLOR(uncle) == RED))
	{
		valid = FALSE;
	}

	return valid;

}

/*
 * Case 1 of violation : Red node whose parent and uncle is also red.
 * Fix : Change the color of parent, grand parent and uncle node.
 * If grand parent is root of RB tree, then don't change the color as root should
 * always be a black node.
 */
rb_tree_st *rb_tree_handle_violation1(
		rb_tree_st *root,
		common_data_compare_t compare)
{

	rb_tree_st *parent = NULL;
	rb_tree_st *grand_parent = NULL;
	rb_tree_st *uncle = NULL;

	uncle = rb_tree_get_uncle(root, compare);
	parent = rb_tree_get_parent(root);
	grand_parent = rb_tree_get_grand_parent(root);

	CHECK_RC_ASSERT((parent == NULL), 0);
	CHECK_RC_ASSERT((grand_parent == NULL), 0);
	CHECK_RC_ASSERT((parent->rb_tree_color), RED);
	CHECK_RC_ASSERT((uncle->rb_tree_color), RED);
	CHECK_RC_ASSERT((root->rb_tree_color), RED);

	rb_tree_change_node_color(uncle);
	rb_tree_change_node_color(parent);

	if (grand_parent != rb_tree_root)
	{
		rb_tree_change_node_color(grand_parent);

		/*
		 * Check if grand parent and parent of grand parent are red. If yes,
		 * then handle the violations.
		 */
		rb_tree_check_fix_violation(grand_parent, compare);

	}

	return root;

}

/*
 * This function checks whether type #2 violation present in the subsystem.
 * Case 2 of violation : 
 * 1. Red node whose parent is red and uncle is black.
 * 2. Also, the node is right child of left parent.
 * 3. Also, the node is left child of right parent.
 */
bool rb_tree_check_violation2(rb_tree_st *node, common_data_compare_t compare)
{

	bool valid = TRUE;
	bool right_parent, left_child;

	rb_tree_st *parent = NULL;
	rb_tree_st *grand_parent = NULL;
	rb_tree_st *uncle = NULL;

	uncle = rb_tree_get_uncle(node, compare);
	parent = rb_tree_get_parent(node);
	grand_parent = rb_tree_get_grand_parent(node);

	if ((RB_TREE_GET_NODE_COLOR(node) == RED) &&
	    (RB_TREE_GET_NODE_COLOR(parent) == RED) &&
	    (RB_TREE_GET_NODE_COLOR(uncle) == BLACK))
	{
		
		rb_tree_find_parent_details(parent, grand_parent, node, compare,
			&right_parent, &left_child);
		if ((right_parent == TRUE) && (left_child == TRUE))
		{
			valid = FALSE;
		}
		else if ((right_parent == FALSE) && (left_child == FALSE))
		{
			valid = FALSE;
		}

	}

	return valid;

}

/*
 * Case 2 of violation : 
 * 1. Red node whose parent is red and uncle is black.
 * 2. Also, the node is right child of left parent.
 * 3. Also, the node is left child of right parent.
 * Fix : 
 * 1. Left rotate around the parent node in case of #2.
 * 2. Right rotate around the parent node in case of #3.
 * 3. Go for case 3 fix.
 */
rb_tree_st *rb_tree_handle_violation2(
		rb_tree_st *root,
		common_data_compare_t compare)
{

	rb_tree_st *parent = NULL;
	rb_tree_st *tmp_root = NULL;
	rb_tree_st *grand_parent = NULL;
	rb_tree_st *uncle = NULL;
	rb_tree_rot_mode_et mode;

	CHECK_RC_ASSERT((root == NULL), 0);

	uncle = rb_tree_get_uncle(root, compare);
	parent = rb_tree_get_parent(root);
	grand_parent = rb_tree_get_grand_parent(root);

	/*
	 * If uncle is found NULL, then consider color of uncle as BLACK.
	 */
	CHECK_RC_ASSERT((parent == NULL), 0);
	CHECK_RC_ASSERT((grand_parent == NULL), 0);

	CHECK_RC_ASSERT((parent->rb_tree_color), RED);

	/*
	 * Figure out the mode in which rotation of the parent node to be done.
	 * And, go for case 3 level handling for leaf node.
	 */
	mode = rb_tree_find_rotate_mode_vio2(parent, grand_parent, root, compare);
	if (mode == LEFT)
	{

		tmp_root = rb_tree_left_rotate(parent, compare);
		return (rb_tree_handle_violation3(tmp_root->rb_tree_left, compare));

	}
	else
	{

		tmp_root = rb_tree_right_rotate(parent, compare);
		return (rb_tree_handle_violation3(tmp_root->rb_tree_right, compare));

	}

}

/*
 * This function checks whether type #3 violation present in the subsystem.
 * Violation 3 : 
 * 1. Uncle node is black.
 * 2. Node and parent node is red.
 * 3. Violating node is left child of left child.
 * 4. Violating node is right child of right child.
 */
bool rb_tree_check_violation3(rb_tree_st *node, common_data_compare_t compare)
{

	bool valid = TRUE;
	bool right_parent, left_child;

	rb_tree_st *parent = NULL;
	rb_tree_st *grand_parent = NULL;
	rb_tree_st *uncle = NULL;

	uncle = rb_tree_get_uncle(node, compare);
	parent = rb_tree_get_parent(node);
	grand_parent = rb_tree_get_grand_parent(node);

	if ((RB_TREE_GET_NODE_COLOR(node) == RED) &&
	    (RB_TREE_GET_NODE_COLOR(parent) == RED) &&
	    (RB_TREE_GET_NODE_COLOR(uncle) == BLACK))
	{

		rb_tree_find_parent_details(parent, grand_parent, node, compare,
			&right_parent, &left_child);
		if ((right_parent == TRUE) && (left_child == FALSE))
		{
			valid = FALSE;
		}
		else if ((right_parent == FALSE) && (left_child == TRUE))
		{
			valid = FALSE;
		}

	}

	return valid;

}

/*
 * Violation 3 : 
 * 1. Uncle node is black.
 * 2. Violating node is left child of left child.
 * 3. Violating node is right child of right child.
 * Fix : 
 * 1. Rotate the grand parent to right in case of #1.
 * 2. Rotate the grand parent to left in case of #2.
 * 3. Swap the colors of grand parent and parent node.
 */

rb_tree_st *rb_tree_handle_violation3(
		rb_tree_st *root,
		common_data_compare_t compare)
{

	rb_tree_st *parent = NULL;
	rb_tree_st *grand_parent = NULL;
	rb_tree_st *uncle = NULL;
	rb_tree_rot_mode_et mode;

	CHECK_RC_ASSERT((root == NULL), 0);

	uncle = rb_tree_get_uncle(root, compare);
	parent = rb_tree_get_parent(root);
	grand_parent = rb_tree_get_grand_parent(root);

	CHECK_RC_ASSERT((parent == NULL), 0);
	CHECK_RC_ASSERT((grand_parent == NULL), 0);

	mode = rb_tree_find_rotate_mode_vio3(parent, grand_parent, root, compare);
	if (mode == LEFT)
	{
		root = rb_tree_left_rotate(grand_parent, compare);
	}
	else
	{
		root = rb_tree_right_rotate(grand_parent, compare);
	}

	SWAP(bool, (grand_parent->rb_tree_color), (parent->rb_tree_color));

	return root;

}

/*
 * This function figures out correct mode of rotation in order to fix violation #2.
 */
rb_tree_rot_mode_et rb_tree_find_rotate_mode_vio2(
			rb_tree_st *parent,
			rb_tree_st *grand_parent,
			rb_tree_st *root,
			common_data_compare_t compare)
{

	rb_tree_rot_mode_et mode = LEFT;
	bool right_parent, left_child;

	rb_tree_find_parent_details(parent, grand_parent, root, compare,
		&right_parent, &left_child);

	if ((right_parent == TRUE) && (left_child == TRUE))
	{
		mode = RIGHT;
	}
	else if ((right_parent == FALSE) && (left_child == FALSE))
	{
		mode = LEFT;
	}
	else
	{
		/*
		 * Should not come here. Messy stuff !!!
		 */
		CHECK_RC_ASSERT(1, 0);

	}

	return mode;

}

/*
 * This function figures out correct mode of rotation in order to fix violation #3.
 */
rb_tree_rot_mode_et rb_tree_find_rotate_mode_vio3(
			rb_tree_st *parent,
			rb_tree_st *grand_parent,
			rb_tree_st *root,
			common_data_compare_t compare)
{

	rb_tree_rot_mode_et mode = LEFT;
	bool right_parent, left_child;

	rb_tree_find_parent_details(parent, grand_parent, root, compare,
		&right_parent, &left_child);

	if ((right_parent == TRUE) && (left_child == FALSE))
	{
		mode = LEFT;
	}
	else if ((right_parent == FALSE) && (left_child == TRUE))
	{
		mode = RIGHT;
	}
	else
	{

		/*
		 * Messy stuff !!!
		 */
		CHECK_RC_ASSERT(1, 0);

	}

	return mode;

}

/*
 * This function returns whether the root is right child of parent of root.
 */
bool rb_tree_find_parent_rel(rb_tree_st *root, common_data_compare_t compare)
{

	int rc = EOK;
	bool right_child = TRUE;

	if (root->rb_tree_parent != NULL)
	{

		rc = compare(root->rb_tree_data, root->rb_tree_parent->rb_tree_data);
		if (rc == FIRST_GREATER)
		{
			right_child = TRUE;
		}
		else if (rc == FIRST_LESS)
		{
			right_child = FALSE;
		}
		else
		{

			/*
			 * Messy stuff.
			 */
			CHECK_RC_ASSERT(1, 0);

		}

	}

	return right_child;

}

/*
 * This function figures out grand parent->parent->node direction.
 */
void rb_tree_find_parent_details(rb_tree_st *parent,
				 rb_tree_st *grand_parent,
				 rb_tree_st *root, 
				 common_data_compare_t compare,
				 bool *right_parent,
				 bool *left_child)
{

	int rc = EOK;

	rc = compare(grand_parent->rb_tree_data, parent->rb_tree_data);
	CHECK_RC_ASSERT((rc == IDENTICAL), 0);

	*right_parent = FALSE;
	if (rc == FIRST_LESS)
	{
		*right_parent = TRUE;
	}

	rc = compare(parent->rb_tree_data, root->rb_tree_data);
	CHECK_RC_ASSERT((rc == IDENTICAL), 0);

	*left_child = TRUE;
	if (rc == FIRST_LESS)
	{
		*left_child = FALSE;
	}

}

/*
 * This function does all the needful to rotate a subtree to left.
 * 		root			right
 *		/   \			/   \
 *		T1  right   ===>>>    root  T3
 *		    /   \	      /  \
 *		   T2   T3	      T1  T2
 */
rb_tree_st* rb_tree_left_rotate(rb_tree_st *root, common_data_compare_t compare)
{

	rb_tree_st *T1 = root->rb_tree_left;
	rb_tree_st *right = root->rb_tree_right;
	rb_tree_st *T2 = NULL;
	rb_tree_st *T3 = NULL;
	bool right_child = TRUE;

	/*
	 * Let's handle special case here.
	 */
	if (right == NULL)
	{
		return (rb_tree_handle_spl_left_rotate(root));
	}

	T2 = right->rb_tree_left;
	T3 = right->rb_tree_right;

	/*
	 * Perform rotation as shown in figure
	 */
	right->rb_tree_left = root;
	root->rb_tree_right = T2;

	/*
	 * If rotation is targetted for bst_root node, then bst_root pointer should be 
	 * changed to right pointer. Diagram proves that right will be a new bst_root 
	 * in this case.
	 */
	if (root == rb_tree_root)
	{

		rb_tree_root = right;
		rb_tree_root->rb_tree_parent = NULL;

	}
	else
	{

		right_child = rb_tree_find_parent_rel(root, compare);
		if (right_child == TRUE)
		{
			root->rb_tree_parent->rb_tree_right = right;
		}
		else
		{
			root->rb_tree_parent->rb_tree_left = right;
		}
		right->rb_tree_parent = root->rb_tree_parent;

	}

	/*
	 * Parent of root should be updated to right.
	 * Parent of T2 should be root.
	 * Parent of T1 and T3 should remain same.
	 * Parent of right should be NULL as this is new root.
	 */
	root->rb_tree_parent = right;

	if (T2)
	{
		T2->rb_tree_parent = root;
	}

	return right;

}

/*
 * This function does all the needful to rotate a subtree to right.
 * 		root			left
 *		/   \			/   \
 *	      left   T3   ===>>>       T1   root
 *	      /  \	    		    /  \
 *	      T1 T2			   T2  T3
 */
rb_tree_st* rb_tree_right_rotate(rb_tree_st *root, common_data_compare_t compare)
{

	bool right_child = TRUE;
	rb_tree_st *T3 = root->rb_tree_right;

	/*
	 * Let's handle a special case here.
	 */
	if (T3 == NULL)
	{
		return (rb_tree_handle_spl_right_rotate(root));
	}

	rb_tree_st *left = root->rb_tree_left;
	rb_tree_st *T1 = left->rb_tree_left;
	rb_tree_st *T2 = left->rb_tree_right;

	/*
	 * Perform rotation as shown in figure
	 */
	left->rb_tree_right = root;
	root->rb_tree_left = T2;

	/*
	 * If rotation is targetted for bst_root node, then bst_root pointer should be 
	 * changed to left pointer. Diagram proves that left will be a new bst_root 
	 * in this case.
	 */
	if (root == rb_tree_root)
	{

		rb_tree_root = left;
		rb_tree_root->rb_tree_parent = NULL;

	}
	else
	{

		right_child = rb_tree_find_parent_rel(root, compare);
		if (right_child == TRUE)
		{
			T2->rb_tree_parent->rb_tree_right = root;
		}
		else
		{
			T2->rb_tree_parent->rb_tree_left = root;
		}
		root->rb_tree_parent = left;

	}

	/*
	 * Parent of left should be NULL.
	 * Parents of T1 and T3 will remain same.
	 * Parent of T2 should be root.
	 */
	left->rb_tree_parent = NULL;
	if (T2)
	{
		T2->rb_tree_parent = root;
	}

	return left;

}

/*
 * This function handles special case of right skewedness.
 *
 * 	root			 T1
 *	  \			/  \
 *         T1	===>>>        root  T2
 *          \
 *          T2
 */
rb_tree_st *rb_tree_handle_spl_left_rotate(rb_tree_st *root)
{

	void *tmp = NULL;
	rb_tree_st *T1 = root->rb_tree_right;
	rb_tree_st *T2 = T1->rb_tree_right;

	/*
	 * Swap bst_data pointers between root and T1.
	 */
	tmp = root->rb_tree_data;
	root->rb_tree_data = T1->rb_tree_data;
	T1->rb_tree_data = tmp;

	/*
	 * Also, swap colors among the two nodes.
	 */
	SWAP(bool, root->rb_tree_color, T1->rb_tree_color);

	root->rb_tree_right = T2;
	root->rb_tree_left = T1;

	/*
	 * Handle a scenario where left subtree is available for T1.
	 */
	if (T1->rb_tree_left)
	{
		T1->rb_tree_right = T1->rb_tree_left;
		T1->rb_tree_left = NULL;
	}
	else
	{
		T1->rb_tree_right = NULL;
	}

	/*
	 * Update parents
	 * Parent of T1 and T2 should be root.
	 */
	T1->rb_tree_parent->rb_tree_right = T2;
	T1->rb_tree_parent = root;

	if (T2)
	{
		T2->rb_tree_parent = root;
	}

	return root;

}

/*
 * This function handles following special case of skewed left BST.
 * 
 * Lets handle a special case here:
 * 	root		 T1
 * 	/		/  \
 *     T1     ===>>    T2  root
 *     /
 *    T2
 */
rb_tree_st *rb_tree_handle_spl_right_rotate(rb_tree_st *root)
{

	void *tmp = NULL;
	rb_tree_st *T1 = root->rb_tree_left;
	rb_tree_st *T2 = T1->rb_tree_left;

	/*
	 * Swap bst_data pointers between root and T1.
	 */
	tmp = root->rb_tree_data;
	root->rb_tree_data = T1->rb_tree_data;
	T1->rb_tree_data = tmp;

	/*
	 * Also, swap colors among the two nodes.
	 */
	SWAP(bool, root->rb_tree_color, T1->rb_tree_color);

	root->rb_tree_left = T2;
	root->rb_tree_right = T1;

	/*
	 * There could scenario that T1 could have right child/subtree.
	 */
	if (T1->rb_tree_right)
	{

		T1->rb_tree_left = T1->rb_tree_right;
		T1->rb_tree_right = NULL;

	}
	else
	{
		T1->rb_tree_left = NULL;
	}

	/*
	 * Update parents : 
	 * Parent of T1 and T2 should be root.
	 */
	T1->rb_tree_parent->rb_tree_right = T1;
	T1->rb_tree_parent = root;

	if (T2)
	{
		T2->rb_tree_parent = root;
	}

	return root;

}

