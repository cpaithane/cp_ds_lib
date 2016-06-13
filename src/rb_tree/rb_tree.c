#include "rb_tree.h"

/*
 * This is root node of RB tree initialized to NULL.
 */
rb_tree_st *rb_tree_root = NULL;

typedef enum rb_tree_color
{

	RED = 0,
	BLACK = 1

}rb_tree_color_et;

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
 * This function inserts data into rb_tree. compare function pointer can be 
 * overriden.
 */
rb_tree_st *rb_tree_insert(rb_tree_st *root,
			   void *data,
			   size_t len,
			   common_data_compare_t compare)
{

	int rc = EOK;

	/*
	 * If root node of rbt ree is NULL, allocate and set root node.
	 */
	if (root == NULL)
	{

		root = rb_tree_alloc_node(data, len);

		/*
		 * If global root node of RB tree is NULL, then it is first node 
		 * allocation.
		 */
		if (rb_tree_root == NULL)
		{
			rb_tree_root = root;
		}

		return root;

	}

	rc = compare(data, root->rb_tree_data);

	/*
	 * If data to be inserted is greater, then look into right subtree. Else, look into
	 * left subtree.
	 * As per property of BST, no two identical nodes present in BST.
	 */
	if (rc == FIRST_GREATER)
	{

		root->rb_tree_right = rb_tree_insert(
						root->rb_tree_right, 
						data, len, 
						compare);

	}
	else if (rc == FIRST_LESS)
	{

		root->rb_tree_left = rb_tree_insert(
						root->rb_tree_left,
						data, len,
						compare);

	}
	else
	{
		/*
		 * Control should not reach here. Messy stuff.
		 */ 
		CHECK_RC_ASSERT(rc == IDENTICAL, 1);
		return NULL;

	}

	return root;

}

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
 * This function prints integer data inside node
 */
void rb_tree_int_node_printer(const rb_tree_st *node)
{

	int *data;
	printf("\n");
	if (node)
	{

		data = (int *)node->rb_tree_data;
		printf("%d\t", *data);

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

