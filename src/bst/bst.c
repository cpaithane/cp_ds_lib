#include "bst.h"

/*
 * This is root node of BST initialized to NULL.
 */
bst_st *bst_root = NULL;

/*
 * This function initializes node, pointers etc.
 */
void bst_initialize_node(bst_st *node, void *data, size_t len)
{

	memcpy(node->bst_data, data, len);
	node->bst_left = NULL;
	node->bst_right = NULL;

}

/*
 * This function allocates bst node.
 */
bst_st *bst_alloc_node(void *data, size_t len)
{

	bst_st *node = NULL;
	node = (bst_st *)malloc(sizeof(bst_st));
	memset(node, 0, sizeof(bst_st));

	node->bst_data = malloc(len);
	memset(node->bst_data, 0, len);
	bst_initialize_node(node, data, len);

	return node;

}

/*
 * This function deallocates node and data inside it.
 */
void bst_dealloc_node(bst_st **nodep)
{

	bst_st *node = *nodep;
	free(node->bst_data);
	free(node);
	*nodep = NULL;

}

/*
 * This function compares integer data and returns 
 * 0 if both are equal
 * 1 if data1 > data2
 * -1 if data1 < data2
 */
int bst_int_data_compare(const void *data1, const void *data2)
{

	int ret_val = IDENTICAL;
	int val1 = *((int *)data1);
	int val2 = *((int *)data2);

	if (val1 < val2)
	{
		ret_val = FIRST_LESS;
	}
	else if (val1 > val2)
	{
		ret_val = FIRST_GREATER;
	}
	
	return ret_val;

}

/*
 * This function inserts data into bst. compare function pointer can be overriden. 
 */
bst_st *bst_insert(bst_st *root, void *data, size_t len, bst_data_compare_t compare)
{

	int rc = EOK;

	/*
	 * If root node of btree is NULL, allocate and set root node.
	 */
	if (root == NULL)
	{

		root = bst_alloc_node(data, len);

		/*
		 * If global root node of BST is NULL, then it is first node 
		 * allocation.
		 */
		if (bst_root == NULL)
		{
			bst_root = root;
		}

		return root;

	}

	rc = compare(data, root->bst_data);

	/*
	 * If data to be inserted is greater, then look into right subtree. Else, look into
	 * left subtree.
	 * As per property of BST, no two identical nodes present in BST.
	 */
	if (rc == FIRST_GREATER)
	{

		root->bst_right = bst_insert(root->bst_right, 
					     data, len, 
					     compare);

	}
	else if (rc == FIRST_LESS)
	{

		root->bst_left = bst_insert(root->bst_left, 
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

void bst_inorder_traversal(bst_st *root, bst_node_printer_t bst_node_printer)
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
	bst_inorder_traversal(root->bst_left, bst_node_printer);

	/*
	 * print the node.
	 */
	bst_node_printer(root);

	/*
	 * traverse to the right subtree
	 */
	bst_inorder_traversal(root->bst_right, bst_node_printer);

}

/*
 * This function traverses BST till it reaches data uses compare function.
 */
int bst_traverse_bst(bst_st *root, 
		     bst_st **parent, 
		     bst_st **node, 
		     void *data, 
		     bst_data_compare_t compare)
{

	int rc = EOK;

	CHECK_RC_ASSERT((data == NULL), 0);

	if (root == NULL)
	{
		return ENOENT;
	}

	rc = compare(root->bst_data, data);
	if (rc == FIRST_GREATER)
	{

		*parent = root;
		return (bst_traverse_bst(root->bst_left,
				 parent, 
				 node, 
				 data, compare));

	}
	else if (rc == FIRST_LESS)
	{

		*parent = root;
		return (bst_traverse_bst(root->bst_right,
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
bst_st *bst_delete_node_1(bst_st *root, bst_st *parent, bst_st *node_to_delete)
{

	/*
	 * If it is root node, return root as NULL.
	 */
	if (node_to_delete == bst_root)
	{

		bst_root = NULL;
		root = NULL;

	}
	/*
	 * Else, link from the parent needs to be broken.
	 */
	else
	{

		parent->bst_left = NULL;
		parent->bst_right = NULL;

	}

	bst_dealloc_node(&node_to_delete);
	return root;

}

/*
 * This function deletes a node which has either left child or right child.
 */
bst_st *bst_delete_node_2(bst_st *root, bst_st *parent, bst_st *node_to_delete)
{

	int is_left = (node_to_delete->bst_left != NULL ? 1 : 0);

	/*
	 * If deletion is for root. That means, parent is NULL.
	 */
	if (node_to_delete == bst_root)
	{

		if (is_left)
		{
			bst_root = node_to_delete->bst_left;
		}
		else
		{
			bst_root = node_to_delete->bst_right;
		}

		bst_dealloc_node(&node_to_delete);
		return bst_root;

	}

	/*
	 * There are two subcases in this case : 
	 * If left subtree present, assign this left subtree to the parent of the node 
	 * to be deleted.
	 */

	if (is_left)
	{
		parent->bst_left = node_to_delete->bst_left;
	}
	else
	{
		parent->bst_right = node_to_delete->bst_right;
	}

	bst_dealloc_node(&node_to_delete);
	return root;

}

/*
 * This function deletes a node which has both the children.
 */
bst_st *bst_delete_node_3(bst_st *root, bst_st *parent, bst_st *node_to_delete, size_t len)
{

	bst_st *in_suc = NULL;
	bst_st *father_of_in_suc = NULL;
	bst_st *father_of_right_subtree = node_to_delete;
	bst_st *right_subtree = node_to_delete->bst_right;

	/*
	 * Find out first inorder successor of the node to be deleted.
	 */
	while (right_subtree->bst_left != NULL)
	{

		father_of_right_subtree = right_subtree;
		right_subtree = right_subtree->bst_left;

	}

	in_suc = right_subtree;
	father_of_in_suc = father_of_right_subtree;
	CHECK_RC_ASSERT((in_suc == NULL), 0);
	CHECK_RC_ASSERT((father_of_in_suc, NULL), 0);

	/*
	 * Now, swap the contents of inorder successor and 
	 * node to be deleted.
	 */
	memcpy(node_to_delete->bst_data, in_suc->bst_data, len);

	/*
	 * Inorder successor can have right subtree.
	 * We need adjust pointers of right subtree.
	 */
	if (father_of_in_suc != node_to_delete)
	{
		node_to_delete->bst_left = in_suc->bst_right;
	}
	else
	{
		node_to_delete->bst_right = in_suc->bst_right;
	}

	/*
	 * Delete the node pointed by inorder predecessor.
	 */
	bst_dealloc_node(&in_suc);

	return root;

}

/*
 * This function deletes the node which contains specified value
 */
bst_st *bst_delete_node(bst_st *root, 
			void *data, 
			size_t len, 
			bst_data_compare_t compare)
{

	bst_st *node_to_delete = NULL;
	bst_st *parent = NULL;

	/*
	 * First traverse to the node of deletion.
	 */
	bst_traverse_bst(root, &parent, &node_to_delete, data, compare);

	CHECK_RC_ASSERT((node_to_delete == NULL), 0);
	if (root != bst_root)
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
	if ((node_to_delete->bst_left == NULL) && 
	    (node_to_delete->bst_right == NULL))
	{

		root = bst_delete_node_1(root, parent, node_to_delete);
		return root;

	}

	/*
	 * Case 2 of deletion
	 */
	if ((node_to_delete->bst_left == NULL) || 
	    (node_to_delete->bst_right == NULL))
	{

		root = bst_delete_node_2(root, parent, node_to_delete);
		return root;

	}

	/*
	 * Case 3 of deletion
	 */
	if ((node_to_delete->bst_left) && 
	    (node_to_delete->bst_right))
	{

		root = bst_delete_node_3(root, parent, node_to_delete, len);
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
void bst_int_node_printer(const bst_st *node)
{

	int *data;
	printf("\n");
	if (node)
	{

		data = (int *)node->bst_data;
		printf("%d\t", *data);

	}
	printf("\n");

}

/*
 * This function destroys tree rooted at root
 */
void bst_destroy_tree(bst_st *root)
{

	if (root == NULL)
	{
		return;
	}

	bst_destroy_tree(root->bst_left);
	bst_destroy_tree(root->bst_right);
	bst_dealloc_node(&root);

}

